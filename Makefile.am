AM_CPPFLAGS       = -Iinclude
ACLOCAL_AMFLAGS   = -Im4
SUBDIRS           = src/lib src/bin
include_HEADERS	  = include/lox.h \
		    include/lox/aux.h \
		    include/lox/error.h \
		    include/lox/stack.h \
		    include/lox/state.h \
		    include/lox/value.h
man3_MANS         = man/lox_state.3 \
		    man/lox_error.3
.PHONY: doc
doc:
	doxygen Doxyfile


