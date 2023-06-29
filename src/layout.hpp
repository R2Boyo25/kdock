#pragma once
#include <yoga/Yoga.h>
#include <vector>
#include "collision.hpp"
#include <cairo-xlib.h>
#include <inttypes.h>
#include <memory>

void layoutTest();

struct Color {
  uint8_t r, g, b, a;
};

struct Element {
  Element();
  ~Element();

  void addChild(Element *child);
  void removeAllChildren();
  void recalc(double available_width, double available_height, YGDirection ownerdirection = YGDirection::YGDirectionInherit);
  
  aabb asAABB();
  virtual void draw(cairo_t *cr);
  void drawChildren(cairo_t *cr);

  virtual void predraw(cairo_t *cr);
  virtual void postdraw(cairo_t *cr);
  
  YGNodeRef ygnode;
  #warning color not changing
  Color color = Color {200, 0, 200, 255};
  
private:
  
  std::vector<Element*> children;
};

struct RoundedRect : public Element {
  RoundedRect();
  RoundedRect(double radius);
 
  virtual void draw(cairo_t *cr);
  double radius = 0;
};

void cairo_rounded_rectangle(cairo_t *cr, double x, double y, double width, double height, double radius);
