AC_INIT([lox], [0.0.0], [https://www.github.com/delta1024/emblox/issues])
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])
AC_PROG_CC
AM_PROG_AR
AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_HEADERS([config.h])
AC_ARG_ENABLE([stacktrace], 
	      AS_HELP_STRING([--enable-stacktrace],
			      [debug stack tracing])
	      , 
	      [], [with_stacktrace=yes])
AS_IF([test "x$with_stacktrace" != xno],
      [AC_DEFINE([DEBUG_STACK_TRACE], [],[Trace stack execution])],[])
AC_CONFIG_FILES([
	Doxyfile
	doxygen-awesome.css
	Makefile
])
AC_ENABLE_SHARED
AC_DISABLE_STATIC
LT_INIT
AC_OUTPUT
