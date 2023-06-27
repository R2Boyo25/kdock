AUTOMAKE_OPTIONS = foreign
bin_PROGRAMS     = kdock

kdock_SOURCES    = src/main.cpp
CLEANFILES      := *~ \#*\#

CXXFLAGS         = -Ofast -Wall -Wextra -Wpedantic -g
CFLAGS           = -Ofast -Wall -Wextra -Wpedantic -g

PKG_CONFIG      := $(shell pkg-config xext x11 cairo-xlib-xrender --cflags --libs) \
                   $(shell pkg-config xrandr --cflags --libs --silence-errors) \
                   $(shell pkg-config xinerama --cflags --libs --silence-errors)

AM_CXXFLAGS     := -std=c++17 $(PKG_CONFIG) 
AM_CFLAGS       := -std=c17 $(PKG_CONFIG)

