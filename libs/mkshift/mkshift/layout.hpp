/**
 * @file
 * @brief The Element class and associated functions
 */

#pragma once
#include <yoga/Yoga.h>
#include <vector>
#include "collision.hpp"
#include <cairo-xlib.h>
#include <inttypes.h>

/// @brief A temporary function to setup layout.
/// Will be renamed and stuff later.
void layoutTest();

/// A RGBA color container.
struct Color {
  uint8_t
    r, ///< Red
    g, ///< Green
    b, ///< Blue
    a; ///< Alpha
};


/**
 * @brief The main Element class.
 *
 * Implements some default functionality and provides an interface to
 * implement in children.
 */
struct Element {
  Element(); ///< Construct a new Element with a new YGNode (ygnode)
  ~Element(); ///< Free ygnode

  /**
   * @brief Add a child to this->children
   *
   * Child->parent is set to `this`.
   *
   * @param child The child to add.
   *  THIS MUST BE ALLOCATED WITH NEW. DO NOT PASS A POINTER TO AN EXISTING ELEMENT. IT CAUSES SEGFAULTS FOR UNKNOWN REASONS.
   */
  void addChild(Element *child);
  void removeAllChildren(); ///< Clears/Frees all children in this->children.

  /**
   * @brief Recalculate Yoga layout.
   *
   * Recalculates only for this node and its children. Call this on the result getRoot() to recalculate everything.
   *
   * @param available_width  The available width to calculate for
   * @param available_height The available height to calculate for
   * @param ownerdirection Dunno what this does. (It's here just in case someone does know lol)
   */
  void recalc(double available_width, double available_height, YGDirection ownerdirection = YGDirection::YGDirectionInherit);

  /// @brief Returns the AABB for the calculated layout.
  /// @returns the AABB of the Element's layout.
  aabb asAABB();

  /**
   * @brief Draws the Element to cr.
   *
   * MUST call this->drawChildren() if you want to draw children.
   * Should also call this->predraw() and this->postdraw().
   *
   * @param cr The Cairo surface to draw to.
   */
  virtual void draw(cairo_t *cr);
  /**
   * @brief Calls draw(cr) for each child
   *
   * @param cr The Cairo surface to draw to.
   */
  void drawChildren(cairo_t *cr);

  /**
   * @brief Code to run before drawing
   *
   * Sets up colors by default.
   *
   * @param cr The Cairo surface to draw to.
   */
  virtual void predraw(cairo_t *cr);
  /**
   * @brief Code to run after drawing.
   *
   * Does nothing by default.
   *
   * @param cr The Cairo surface to draw to.
   */
  virtual void postdraw(cairo_t *cr);

  Element *parent = nullptr; ///< The parent of this node.
  /**
   * @brief Gets the effective root of tree.
   *
   * Recurses up the tree until it finds an element with no parent.
   *
   * @return Root of the tree.
   */
  Element *getRoot();

#warning add clone()
#warning add methods for setting the style of ygnode
  
  YGNodeRef ygnode; ///< The Yoga layout node.
#warning color not changing
  Color color = Color {200, 0, 200, 255}; ///< The primary draw color.
  
private:
  
  std::vector<Element*> children;
};

/// @brief An Element that draws a rectangle with optional rounded corners.
struct Box : public Element {
  Box();
  Box(double radius); ///< Radius is the corner-radius.

  virtual void draw(cairo_t *cr);
  double radius = 0; ///< The corner-radius of the box. 0.0 will give a plain rectangle. (it switches functions to cairo_rect)
};

/**
 * @brief Draws a rounded rectangle.
 *
 * @param cr     The Cairo surface to draw to.
 * @param x      The x position of the top left corner.
 * @param y      The y position of the top left corner.
 * @param width  The width of the rectangle.
 * @param height The height of the rectangle.
 * @param radius The corner-radius. TODO: separate corners.
 */
void cairo_rounded_rectangle(cairo_t *cr, double x, double y, double width, double height, double radius);
