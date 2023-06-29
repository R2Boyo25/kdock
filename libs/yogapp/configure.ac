AC_INIT([yogapp], [0.1.0])
AM_INIT_AUTOMAKE([subdir-objects])
AC_PROG_CXX
AC_PROG_CC
AC_PROG_RANLIB

AC_CONFIG_FILES([Makefile])

echo "Configuring Yoga with CMake..."
mkdir -p libs/yoga/build
cmake -B libs/yoga/build -S libs/yoga
echo "Done."

AC_OUTPUT
