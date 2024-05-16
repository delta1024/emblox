notrans_man3_MANS  = lox_state.3.gz \
	     lox_error.3.gz

SUFFIXES = .3 .gz

lox_state.3.gz: $(top_srcdir)/man/lox_state.3
	gzip $< -9 --stdout > $@
lox_error.3.gz: $(top_srcdir)/man/lox_error.3
	gzip $< -9 --stdout > $@

