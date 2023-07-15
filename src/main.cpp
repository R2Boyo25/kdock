/**
 * @file
 * @brief A highly customizable Linux dock using MkShift.
 */

#include <X11/X.h>
#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <cairo-xlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
//#include <X11/keysymdef.h>
#include <X11/Xatom.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>

#include <X11/extensions/shape.h>
#if HAVE_X11_EXTENSIONS_XINERAMA_H
# include <X11/extensions/Xinerama.h>
#else
# if HAVE_X11_EXTENSIONS_XRANDR_H
#  include <X11/extensions/Xrandr.h>
# else
#  error XRandR or Xinerama are required.
# endif
#endif

#include <mkshift/collision.hpp>
#include <mkshift/layout.hpp>

double scaling_ratio; ///< ratio to multiple sizes by
int taskbar_margin = 12; ///< the margin of the taskbar
int taskbar_height = 56; ///< the height of the taskbar
int taskbar_width; ///< the width of the taskbar
int taskbar_gap = 9; ///< the space between taskbar groups.
int screen_width; ///< width of the main screen.
int screen_height; ///< height of the main screen.
Element root_element; ///< the root MkShift Element 

static cairo_surface_t *surface;
static cairo_t *cairo;
static Display *dpy;

static cairo_surface_t *shape_surface;
static cairo_t *shape_cairo;
static Pixmap shape;

/**
 * @brief Recalculate and redraw root_element;
 *
 * @param w The Window to draw to.
 */
void paint(Window w) {
  root_element.recalc(taskbar_width, taskbar_height);
  root_element.draw(shape_cairo);
  
  XShapeCombineMask(dpy, w, ShapeBounding, 0, 0, 
                    cairo_xlib_surface_get_drawable(shape_surface), ShapeSet);
}

/// @brief The dimensions of a screen.
typedef struct {
  int width; ///< The width of the screen.
  int height; ///< The height of the screen.
} ScreenDimensions;


/**
 * @brief Query the screen dimensions of the main screen
 *
 * TODO: add a way to select which monitor to check
 * Uses Xinerama or XRandR, preferring Xinerama.
 * @param dpy The Xlib Display to query.
 * @return The dimensions of the main monitor.
 */
ScreenDimensions screen_dimensions(Display *dpy) {
  int screen_width, screen_height, screen_id;
  screen_id = DefaultScreen(dpy);
#if HAVE_X11_EXTENSIONS_XINERAMA_H
  XineramaScreenInfo *xinerama_screen_config = XineramaQueryScreens(dpy, &screen_id);

  screen_width = xinerama_screen_config->width;
  screen_height = xinerama_screen_config->height;
#else 
# if HAVE_X11_EXTENSIONS_XRANDR_H
  int xrandr_num_sizes = 0;
  XRRScreenSize *xrandr_screen_config = XRRSizes(dpy, 
                                                 screen_id, &xrandr_num_sizes);

  if (xrandr_num_sizes == 0) {
    fprintf(stderr, "Error: XRandR is unsupported by the current machine.");
    std::exit(1);
  }
  
  screen_width = xrandr_screen_config->width;
  screen_height = xrandr_screen_config->height;
# endif
#endif
  return {screen_width, screen_height};
}

/// @brief Get the screen size and update all the values for taskbar sizing.
void updateScreenSize() {
  ScreenDimensions screen_dim = screen_dimensions(dpy);
  scaling_ratio   = std::max((double)screen_dim.width / 1920, 1.0);
  screen_width    = screen_dim.width;
  screen_height   = screen_dim.height;
  taskbar_height *= scaling_ratio;
  taskbar_width   = screen_dim.width - (2 * taskbar_margin);
  taskbar_gap    *= scaling_ratio;

  printf("W%d H%d S%lf\n", taskbar_width, taskbar_height, scaling_ratio);
}

/**
 * @brief Initialializes the Xlib Window.
 *
 * @return The initialized window.
 */
Window initializeWindow() {
  dpy = XOpenDisplay(NULL);
  if (dpy == NULL) {
    fprintf(stderr, "Error: Can't open display. Is DISPLAY set?\n");
    std::exit(1);
  }

  updateScreenSize();
  
  Window w;
  w = XCreateSimpleWindow(dpy, RootWindow(dpy, 0),
                          taskbar_margin, screen_height - taskbar_height - taskbar_margin, taskbar_width, taskbar_height, 0, 0, BlackPixel(dpy, 0));
  
  XSetWindowAttributes winattr;
  winattr.override_redirect = 1;
  XChangeWindowAttributes(dpy, w, CWOverrideRedirect, &winattr);

  XSelectInput(dpy, w, StructureNotifyMask | ExposureMask);
  XMapWindow(dpy, w);

  return w;
}

/**
 * @brief Initializes the Cairo surfaces
 *
 * Creates cairo_surface and shape_cairo
 * @param w The Window to initalize the surfaces for.
 */
void initializeCairo(Window w) {
  surface = cairo_xlib_surface_create(dpy, w, DefaultVisual(dpy, 0), taskbar_width, taskbar_height);
  cairo = cairo_create(surface);

  shape = XCreatePixmap(dpy, w, taskbar_width, taskbar_height, 1);
  shape_surface = cairo_xlib_surface_create_for_bitmap(dpy, shape, 
                                                       DefaultScreenOfDisplay(dpy),
                                                       taskbar_width, taskbar_height);
  
  shape_cairo = cairo_create(shape_surface);
}

/// @brief Initialize the MkShift layout.
void initializeLayout() {
  YGNodeRef root_node = YGNodeNew();
  YGNodeStyleSetWidthPercent(root_node, 100.0);
  YGNodeStyleSetHeightPercent(root_node, 100.0);
  //YGNodeStyleSetAlignItems(root_node, YGAlign::YGAlignCenter);
  YGNodeStyleSetDisplay(root_node, YGDisplay::YGDisplayFlex);
  YGNodeStyleSetJustifyContent(root_node, YGJustify::YGJustifyCenter);
  YGNodeStyleSetGap(root_node, YGGutter::YGGutterColumn, taskbar_gap);
  YGNodeStyleSetFlexDirection(root_node, YGFlexDirection::YGFlexDirectionRow);

  root_element.ygnode = root_node;
  
  YGNodeRef taskbar_node = YGNodeNew();
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

#warning TODO: get_visible_windows
/**
 * @brief Get a list of all visible windows on the screen.
 *
 * @return The list of Windows.
 * @exception SIGSEGV currently.
 */
std::vector<Window> get_visible_windows() {
  Atom ret;
  int format, status;
  unsigned char* data = 0;
  unsigned long nitems, after;
  
  XGetWindowProperty(dpy, RootWindow(dpy, 0), XInternAtom(dpy, "_NET_CLIENT_LIST", true), 0, 0, false, XA_WINDOW, &ret, &format, &nitems, &after, &data);
}

#warning TODO: hide_unhide
/**
 * @brief Calculate's whether the dock should be hidden and applies that.
 *
 * TODO: currently unimplemented.
 *
 * @param w The Window to use to determine this.
 */
void hide_unhide(Window w) {
  
}

/// Main entrypoint to KDock
int main() {
  Window w = initializeWindow();
  initializeCairo(w);
  initializeLayout();
  
  while (1) {
    XEvent e;
    XNextEvent(dpy, &e);
    //printf("Got event: %d\n", e.type);
 
    switch (e.type) {
    case MotionNotify:
      
      break;
    case MapNotify:
    case ConfigureNotify:
    case Expose:
      paint(w);
      break;
    }
  }

  return 0;
}
