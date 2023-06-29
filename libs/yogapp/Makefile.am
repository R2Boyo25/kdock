AUTOMAKE_OPTIONS      = foreign
noinst_LIBRARIES      = libyogapp.a

libyogapp_a_SOURCES   = yogapp/yogapp.cpp
libyogapp_a_LIBADD    = libs/yoga/build/yoga/libyogacore.a
CLEANFILES           := $(shell find . -wholename "*~") $(shell find -name "#*#") $(shell find libs/yoga/build -wholename "*.[oa]")

EXTRA_DIST            = libs/yoga/yoga libs/yoga/CMakeLists.txt libs/yoga/cmake libs/yoga/tests

CXXFLAGS         = -Ofast -Wall -Wextra -Wpedantic
CFLAGS           = -Ofast -Wall -Wextra -Wpedantic

AM_CXXFLAGS     := -std=c++17 -Ilibs/yoga/
AM_CFLAGS       := -std=c17 -Ilibs/yoga/

distclean-local:
	rm -rf libs/yoga/build

libs/yoga/build/yoga/libyogacore.a:
	$(MAKE) yogacore -C $(dir $@)
