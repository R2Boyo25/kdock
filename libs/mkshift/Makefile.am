AUTOMAKE_OPTIONS      = foreign
SUBDIRS               = libs/yoga

include $(srcdir)/include.am

noinst_LIBRARIES      = libmkshift.a

libmkshift_a_SOURCES  = mkshift/layout.cpp mkshift/layout.hpp mkshift/collision.cpp mkshift/collision.hpp
libmkshift_a_LIBADD   = libs/yoga/libyoga.a
libmkshift_a_AR      := bash ar.sh $(AR) $(ARFLAGS)
# libmkshift_a_LDFLAGS := -Wl,--whole-archive # prevent empty .a
CLEANFILES           := $(shell find . -wholename "*~") $(shell find -name "#*#")

AM_CXXFLAGS          := $(INCLUDEMKSHIFT)
AM_CFLAGS            := $(INCLUDEMKSHIFT)
