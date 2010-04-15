#
#      Makefile.am for src/canny
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions for canny
#
PROG  = canny
XPROG =
MANP	= canny.1
CINCL    = canny.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)
man3_MANS=canny.3 canny_scale.3 canny_filters.3

LDADD = ../../lib/libxite.la

canny_SOURCES = canny_main.c
