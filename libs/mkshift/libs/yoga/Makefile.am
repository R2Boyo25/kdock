AUTOMAKE_OPTIONS      = foreign
noinst_LIBRARIES      = libyoga.a

libyoga_a_SOURCES     = src/dummy.cpp
#libyoga_a_LDFLAGS    := -Wl,--whole-archive # prevent empty .a
libyoga_a_LIBADD      = yoga/build/yoga/libyogacore.a
libyoga_a_AR         := bash $(shell dirname $(shell dirname $(shell realpath .)))/ar.sh $(AR) $(ARFLAGS)
CLEANFILES           := $(shell find . -wholename "*~") $(shell find -name "#*#") $(shell find yoga/build -wholename "*.[oa]")

EXTRA_DIST            = yoga/yoga yoga/CMakeLists.txt yoga/cmake yoga/tests

CXXFLAGS         = -Ofast -Wall -Wextra -Wpedantic
CFLAGS           = -Ofast -Wall -Wextra -Wpedantic

AM_CXXFLAGS     := -Iyoga/
AM_CFLAGS       := -Iyoga/

distclean-local:
	rm -rf yoga/build

yoga/build/yoga/libyogacore.a:
	$(MAKE) yogacore -C $(dir $@)
