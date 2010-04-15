#
#      Makefile.am for src/segmentation
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
PROG  = segmSpannWilson   segmRandom
MANP  = segmSpannWilson.1 segmRandom.1
CINCL  = segmentation.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

segmSpannWilson_SOURCES = segmSpannWilson_main.c
segmRandom_SOURCES = segmRandom_main.c
