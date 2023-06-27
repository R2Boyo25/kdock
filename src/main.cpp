#include <cairo-xlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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

double scaling_ratio;
int taskbar_margin = 12;
int taskbar_height = 56;
int taskbar_width;

static cairo_surface_t *surface;
static cairo_t *cairo;
static Display *dpy;

static cairo_surface_t *shape_surface;
static cairo_t *shape_cairo;
static Pixmap shape;

void rounded_rect(
                  cairo_t *cr,
                  double x,        /* parameters like cairo_rectangle */
                  double y,
                  double width,
                  double height,
                  double aspect,     /* aspect ratio */
                  double corner_radius) {

  double radius = corner_radius / aspect;
  double degrees = M_PI / 180.0;

  cairo_new_sub_path (cr);
  cairo_arc (cr, x + width - radius, y + radius, radius, -90 * degrees, 0 * degrees);
  cairo_arc (cr, x + width - radius, y + height - radius, radius, 0 * degrees, 90 * degrees);
  cairo_arc (cr, x + radius, y + height - radius, radius, 90 * degrees, 180 * degrees);
  cairo_arc (cr, x + radius, y + radius, radius, 180 * degrees, 270 * degrees);
  cairo_close_path (cr);

  cairo_set_source_rgb (cr, 0.5, 0.5, 1);
  cairo_fill_preserve (cr);
  cairo_set_source_rgba (cr, 0.5, 0, 0, 0.5);
  cairo_set_line_width (cr, 10.0);
  cairo_stroke (cr);
}

void cairo_rounded_rectangle(cairo_t * cr, double x, double y, double width, double height, double radius) {
	double degrees = M_PI / 180.0;

	cairo_new_sub_path(cr);
	cairo_arc (cr, x + width - radius, y + radius, radius, -90 * degrees, 0 * degrees);
	cairo_arc (cr, x + width - radius, y + height - radius, radius, 0 * degrees, 90 * degrees);
	cairo_arc (cr, x + radius, y + height - radius, radius, 90 * degrees, 180 * degrees);
	cairo_arc (cr, x + radius, y + radius, radius, 180 * degrees, 270 * degrees);
	cairo_close_path(cr);
}

void paint(Window w) {
  cairo_set_source_rgb(cairo, 0, 0, 0);
  cairo_rectangle(cairo, 0, 0, 200, 200);
  cairo_fill(cairo);

  cairo_set_line_width(cairo, 5);
  cairo_set_source_rgb(cairo, 255, 255, 255);
  //cairo_rectangle(shape_cairo, 0, 1, taskbar_width-2, taskbar_height-1); // border
  cairo_stroke(cairo);

  cairo_set_operator(shape_cairo, CAIRO_OPERATOR_CLEAR);
  //cairo_rectangle(shape_cairo, 0, 0, taskbar_width, taskbar_height);
  cairo_fill(shape_cairo);

  cairo_set_line_width(shape_cairo, 1);
  cairo_set_operator(shape_cairo, CAIRO_OPERATOR_OVER);
  //cairo_rectangle(shape_cairo, 0, 1, taskbar_width-2, taskbar_height - 1); // border
  //rounded_rect(shape_cairo, 0, 0, taskbar_width, taskbar_height, 1, 24);
  cairo_rounded_rectangle(shape_cairo, 0, 0, taskbar_width, taskbar_height, 12 * scaling_ratio);
  cairo_stroke_preserve(shape_cairo);
  cairo_fill(shape_cairo);
  
  XShapeCombineMask(dpy, w, ShapeBounding, 0, 0, 
                    cairo_xlib_surface_get_drawable(shape_surface), ShapeSet);
}

typedef struct {
  int width;
  int height;
} ScreenDimensions;

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

int main() {  
  dpy = XOpenDisplay(NULL);
  if (dpy == NULL) {
    fprintf(stderr, "Error: Can't open display. Is DISPLAY set?\n");
    return 1;
  }
  
  ScreenDimensions screen_dim = screen_dimensions(dpy);
  scaling_ratio = std::max((double)screen_dim.width / 1920, 1.0);
  screen_dim.width = 372 * scaling_ratio;
  taskbar_height *= scaling_ratio;
  taskbar_width               = screen_dim.width - (2 * taskbar_margin);

  printf("W%d H%d S%lf\n", taskbar_width, taskbar_height, scaling_ratio);
  
  Window w;
  w = XCreateSimpleWindow(dpy, RootWindow(dpy, 0),
                          taskbar_margin, screen_dim.height - taskbar_height - taskbar_margin, taskbar_width, taskbar_height, 0, 0, BlackPixel(dpy, 0));
  
  XSetWindowAttributes winattr;
  winattr.override_redirect = 1;
  XChangeWindowAttributes(dpy, w, CWOverrideRedirect, &winattr);

  XSelectInput(dpy, w, StructureNotifyMask | ExposureMask);
  XMapWindow(dpy, w);

  surface = cairo_xlib_surface_create(dpy, w, DefaultVisual(dpy, 0), taskbar_width, taskbar_height);
  cairo = cairo_create(surface);

  shape = XCreatePixmap(dpy, w, taskbar_width, taskbar_height, 1);
  shape_surface = cairo_xlib_surface_create_for_bitmap(dpy, shape, 
                                                       DefaultScreenOfDisplay(dpy),
                                                       taskbar_width, taskbar_height);
  
  shape_cairo = cairo_create(shape_surface);

  
  
  
  while (1) {
    XEvent e;
    XNextEvent(dpy, &e);
    printf("Got event: %d\n", e.type);

    switch (e.type) {
      case MapNotify:
      case ConfigureNotify:
      case Expose:
        paint(w);
        break;
    }
  }

  return 0;
}
