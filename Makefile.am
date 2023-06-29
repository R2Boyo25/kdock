AUTOMAKE_OPTIONS = foreign

SUBDIRS          = libs/yogapp

bin_PROGRAMS     = kdock

kdock_SOURCES    = src/main.cpp src/collision.cpp src/collision.hpp src/layout.cpp src/layout.hpp
kdock_LDADD      = libs/yogapp/libs/yoga/build/yoga/libyogacore.a
CLEANFILES      := $(shell find . -wholename "*~") $(shell find -name "#*#")

CXXFLAGS         = -Ofast -Wall -Wextra -Wpedantic
CFLAGS           = -Ofast -Wall -Wextra -Wpedantic

PKG_CONFIG      := $(shell pkg-config xext x11 cairo-xlib-xrender --cflags --libs) \
                   $(shell pkg-config xrandr --cflags --libs --silence-errors) \
                   $(shell pkg-config xinerama --cflags --libs --silence-errors)

AM_CXXFLAGS     := -std=c++17 $(PKG_CONFIG) -Ilibs/yogapp/libs/yoga
AM_CFLAGS       := -std=c17 $(PKG_CONFIG) -Ilibs/yogapp/libs/yoga

libs/yogapp/libs/yoga/build/yoga/libyogacore.a:
	make -C libs/yogapp
