AM_CPPFLAGS       = -Iinclude
ACLOCAL_AMFLAGS   = -Im4
SUBDIRS           = src/lib src/bin ./man
include_HEADERS	  = include/lox.h \
		    include/lox/aux.h \
		    include/lox/error.h \
		    include/lox/stack.h \
		    include/lox/state.h \
		    include/lox/value.h
.PHONY: doc
doc:
	doxygen Doxyfile


