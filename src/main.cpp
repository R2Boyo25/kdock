#include <cairo-xlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <config.h>
#include <stdio.h>
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

static cairo_surface_t *surface;
static cairo_t *cairo;
static Display *dpy;

static cairo_surface_t *shape_surface;
static cairo_t *shape_cairo;
static Pixmap shape;

void paint(Window w) {
  cairo_set_source_rgb(cairo, 0, 0, 0);
  cairo_rectangle(cairo, 0, 0, 200, 200);
  cairo_fill(cairo);

  cairo_set_line_width(cairo, 5);
  cairo_set_source_rgb(cairo, 255, 255, 255);
  cairo_rectangle(cairo, 20, 20, 50, 50);
  cairo_rectangle(cairo, 80, 80, 50, 50);
  cairo_stroke(cairo);

  cairo_set_operator(shape_cairo, CAIRO_OPERATOR_CLEAR);
  cairo_rectangle(shape_cairo, 0, 0, 200, 200);
  cairo_fill(shape_cairo);

  cairo_set_line_width(shape_cairo, 1);
  cairo_set_operator(shape_cairo, CAIRO_OPERATOR_OVER);
  cairo_rectangle(shape_cairo, 20, 20, 50, 50);
  cairo_rectangle(shape_cairo, 80, 80, 50, 50);
  cairo_stroke(shape_cairo);
  
  XShapeCombineMask(dpy, w, ShapeBounding, 0, 0, 
                    cairo_xlib_surface_get_drawable(shape_surface), ShapeSet);
}

int main() {
  int screen_width, screen_height, screen_id;
  
  dpy = XOpenDisplay(NULL);
  if (dpy == NULL) {
    fprintf(stderr, "Error: Can't open display. Is DISPLAY set?\n");
    return 1;
  }

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
    fprintf(stderr, "Error: XRandR is not supported.");
    return 1;
  }
  
  screen_width = xrandr_screen_config->width;
  screen_height = xrandr_screen_config->height;
# endif
#endif
  
  printf("Width: %d\nHeight: %d\n", screen_width, screen_height);
  Window w;
  w = XCreateSimpleWindow(dpy, RootWindow(dpy, 0),
                          300, 200, 200, 200, 0, 0, BlackPixel(dpy, 0));
  
  XSetWindowAttributes winattr;
  winattr.override_redirect = 1;
  XChangeWindowAttributes(dpy, w, CWOverrideRedirect, &winattr);

  XSelectInput(dpy, w, StructureNotifyMask | ExposureMask);
  XMapWindow(dpy, w);

  surface = cairo_xlib_surface_create(dpy, w, DefaultVisual(dpy, 0), 200, 200);
  cairo = cairo_create(surface);

  shape = XCreatePixmap(dpy, w, 200, 200, 1);
  shape_surface = cairo_xlib_surface_create_for_bitmap(dpy, shape, 
                                                       DefaultScreenOfDisplay(dpy),
                                                       200, 200);
  
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
