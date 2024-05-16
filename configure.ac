AC_INIT([lox], [0.0.0], [https://www.github.com/delta1024/emblox/issues])
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])
AC_PROG_CC
AM_PROG_AR
AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_HEADERS([config.h])
AC_DEFINE([DEBUG_TRACE_EXECUTION], [], [Trace execution])
AC_CONFIG_FILES([
	Doxyfile
	doxygen-awesome.css
	Makefile
	man/Makefile
	src/lib/Makefile
	src/bin/Makefile
])
AC_ENABLE_SHARED
AC_DISABLE_STATIC
LT_INIT
AC_OUTPUT
