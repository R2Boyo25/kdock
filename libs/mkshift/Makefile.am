AUTOMAKE_OPTIONS      = foreign
SUBDIRS               = libs/yoga

include $(srcdir)/include.am

noinst_LIBRARIES      = libmkshift.a

libmkshift_a_SOURCES  = mkshift/dummy.cpp
libmkshift_a_LIBADD   = libs/yoga/libyoga.a
libmkshift_a_AR      := bash ar.sh $(AR) $(ARFLAGS)
# libmkshift_a_LDFLAGS := -Wl,--whole-archive # prevent empty .a
CLEANFILES           := $(shell find . -wholename "*~") $(shell find -name "#*#")

AM_CXXFLAGS          := $(INCLUDEMKSHIFT)
AM_CFLAGS            := $(INCLUDEMKSHIFT)
