AUTOMAKE_OPTIONS = foreign

include $(srcdir)/libs/mkshift/include.am

SUBDIRS          = libs/mkshift

bin_PROGRAMS     = kdock

kdock_SOURCES    = src/main.cpp src/collision.cpp src/collision.hpp src/layout.cpp src/layout.hpp
kdock_LDADD      = libs/mkshift/libmkshift.a libs/mkshift/libs/yoga/libyoga.a
CLEANFILES      := $(shell find . -wholename "*~") $(shell find -name "#*#")

CXXFLAGS         = -Ofast -Wall -Wextra -Wpedantic
CFLAGS           = -Ofast -Wall -Wextra -Wpedantic

AM_CXXFLAGS     := -std=c++17 $(INCLUDEMKSHIFT)
AM_CFLAGS       := -std=c17 $(INCLUDEMKSHIFT)
