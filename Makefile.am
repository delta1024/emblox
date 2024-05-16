AM_CPPFLAGS       = -I$(top_srcdir)/include
ACLOCAL_AMFLAGS   = -Im4

include $(top_srcdir)/man/Makefile.am
include $(top_srcdir)/src/bin/Makefile.am
include $(top_srcdir)/src/lib/Makefile.am
include $(top_srcdir)/include/Makefile.am

.PHONY: doc
doc:
	doxygen Doxyfile


