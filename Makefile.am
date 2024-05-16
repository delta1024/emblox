AM_CPPFLAGS       = -I$(top_srcdir)/include
ACLOCAL_AMFLAGS   = -Im4

include $(top_srcdir)/man/Makefile.am
include $(top_srcdir)/src/bin/Makefile.am
include $(top_srcdir)/src/lib/Makefile.am

include_HEADERS	  = include/lox.h \
		    include/lox/aux.h \
		    include/lox/error.h \
		    include/lox/stack.h \
		    include/lox/state.h \
		    include/lox/value.h
.PHONY: doc
doc:
	doxygen Doxyfile


