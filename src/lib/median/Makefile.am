#
#      Makefile.am for lib/median
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxitemedian.la

MANF =  fractile.3 fractile_mask.3 lit.3 maxHisto.3 median.3 mode.3 \
	pseudomedian3x3.3 rank.3

man3_MANS=$(MANF)

libxitemedian_la_SOURCES = lit.c maxHisto.c median.c mode.c \
	pseudomedian3x3.c rank.c fractile.c fractile_mask.c
