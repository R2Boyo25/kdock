AUTOMAKE_OPTIONS = foreign
bin_PROGRAMS     = kdock

kdock_SOURCES    = src/main.cpp
CLEANFILES      := *~ \#*\#

CXXFLAGS         = -Ofast -Wall -Wextra -Wpedantic -g
CFLAGS           = -Ofast -Wall -Wextra -Wpedantic -g

AM_CXXFLAGS     := -std=c++17 $(shell pkg-config xext x11 cairo-xlib-xrender $(XRANDR) $(XINERAMA) --cflags --libs)
AM_CFLAGS       := -std=c17 $(shell pkg-config xext x11 cairo-xlib-xrender $(XRANDR) $(XINERAMA) --cflags --libs)

