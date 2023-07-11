AC_INIT([yoga], [0.1.0])
AM_INIT_AUTOMAKE([subdir-objects])

AC_PROG_CXX
AC_PROG_RANLIB

AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_FILES([Makefile])

echo "Configuring Yoga with CMake..."
mkdir -p yoga/build
cmake -B yoga/build -S yoga
echo "Done."

AC_OUTPUT
