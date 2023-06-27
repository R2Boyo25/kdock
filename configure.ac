AC_INIT([kdock], [0.1.0])
AM_INIT_AUTOMAKE([subdir-objects])
AC_PROG_CXX
AC_PROG_CC

AC_CHECK_HEADER([X11/extensions/Xrandr.h], [AC_SUBST([XRANDR], ["xrandr"])], [AC_SUBST([XRANDR], [""])])
AC_CHECK_HEADER([X11/extensions/Xinerama.h], [AC_SUBST([XINERAMA], ["xinerama"])], [AC_SUBST([XINERAMA], [""])])
AC_CHECK_HEADERS([X11/extensions/Xrandr.h X11/extensions/Xinerama.h])

AC_CONFIG_FILES([Makefile])
AC_CONFIG_HEADERS([config.h])
AC_OUTPUT
