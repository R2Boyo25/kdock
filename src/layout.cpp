#include "layout.hpp"
#include "yoga/Yoga.h"

extern int taskbar_height;
extern int taskbar_width;
extern int taskbar_gap;
extern double scaling_ratio;
extern Element root_element;

Element::Element() {
  this->ygnode = YGNodeNew();
}

Element::~Element() {
  YGNodeFree(this->ygnode);
}

void Element::addChild(Element *child) {
  child->parent = this;
  
  this->children.push_back(child);
  
  YGNodeRemoveAllChildren(this->ygnode);

  for (size_t i = 0; i < this->children.size(); i++) {
    YGNodeInsertChild(this->ygnode, this->children[i]->ygnode, i);
  }
}

void Element::removeAllChildren() {
  this->children.clear();
}

void Element::recalc(double available_width, double available_height, YGDirection ownerdirection) {
  YGNodeCalculateLayout(this->ygnode, available_width, available_height, ownerdirection);
}

aabb Element::asAABB() {
  return aabb {
    YGNodeLayoutGetLeft(this->ygnode),
    YGNodeLayoutGetTop(this->ygnode),
    YGNodeLayoutGetWidth(this->ygnode),
    YGNodeLayoutGetHeight(this->ygnode)
  };
}

void Element::draw(cairo_t *cr) {
  printf("Element: ");
  this->predraw(cr);
  this->drawChildren(cr);
  this->postdraw(cr);
}

void Element::drawChildren(cairo_t *cr) { 
  for (auto& child : this->children) {
    child->draw(cr);
  }
}

void Element::predraw(cairo_t *cr) {
  cairo_set_source_rgba(cr, this->color.r, this->color.g, this->color.b, this->color.a);

  print_aabb(this->asAABB());
}

void Element::postdraw(cairo_t *cr) {
  (void) cr;
}

Element *Element::getRoot() {
  Element *target = this;

  while (target->parent != nullptr) {
    target = target->parent;
  }

  return target;
}

void cairo_rounded_rectangle(cairo_t *cr, double x, double y, double width, double height, double radius) {
  if (radius == 0) {
    cairo_rectangle(cr, x, y, width, height);
    
    return;
  }
  
  double degrees = M_PI / 180.0;

  cairo_new_sub_path(cr);
  cairo_arc (cr, x + width - radius, y + radius, radius, -90 * degrees, 0 * degrees);
  cairo_arc (cr, x + width - radius, y + height - radius, radius, 0 * degrees, 90 * degrees);
  cairo_arc (cr, x + radius, y + height - radius, radius, 90 * degrees, 180 * degrees);
  cairo_arc (cr, x + radius, y + radius, radius, 180 * degrees, 270 * degrees);
  cairo_close_path(cr);
}

Box::Box() {

}

Box::Box(double radius) {
  this->radius = radius;
}

void Box::draw(cairo_t *cr) {
  this->predraw(cr);
  
  aabb bb = this->asAABB();
  cairo_rounded_rectangle(cr, bb.x, bb.y, bb.width, bb.height, this->radius);
  cairo_fill(cr);

  printf("RoundedRect: ");
  this->drawChildren(cr);
}

void layoutTest() {
  auto root_node = YGNodeNew();
  YGNodeStyleSetWidthPercent(root_node, 100.0);
  YGNodeStyleSetHeightPercent(root_node, 100.0);
  //YGNodeStyleSetAlignItems(root_node, YGAlign::YGAlignCenter);
  YGNodeStyleSetDisplay(root_node, YGDisplay::YGDisplayFlex);
  YGNodeStyleSetJustifyContent(root_node, YGJustify::YGJustifyCenter);
  YGNodeStyleSetGap(root_node, YGGutter::YGGutterColumn, taskbar_gap);
  YGNodeStyleSetFlexDirection(root_node, YGFlexDirection::YGFlexDirectionRow);

  root_element.ygnode = root_node;
  
  auto taskbar_node = YGNodeNew();
  YGNodeStyleSetWidth(taskbar_node, 300 * scaling_ratio);
  YGNodeStyleSetHeightPercent(taskbar_node, 100.0);
  
  Box *taskbar_element = new Box(15 * scaling_ratio);
  taskbar_element->ygnode = taskbar_node;
  root_element.addChild(taskbar_element);
  
  Box *taskbar_element_2 = new Box(15 * scaling_ratio);
  taskbar_element_2->ygnode = YGNodeClone(taskbar_node);
  root_element.addChild(taskbar_element_2);

  Box *taskbar_element_3 = new Box(15 * scaling_ratio);
  taskbar_element_3->ygnode = YGNodeClone(taskbar_node);
  root_element.addChild(taskbar_element_3);
  
  root_element.recalc(taskbar_width, taskbar_height);
}
