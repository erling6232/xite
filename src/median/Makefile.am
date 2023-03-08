#
#      Makefile.am for src/median
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
PROG  = lit maxHisto median mode pseudomedian3x3 rank \
        fractile
MANP  = lit.1 maxHisto.1 median.1  mode.1 pseudomedian3x3.1 rank.1 \
        fractile.1
CINCL =  median.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

lit_SOURCES             = lit_main.c
maxHisto_SOURCES        = maxHisto_main.c
median_SOURCES          = median_main.c
mode_SOURCES            = mode_main.c
pseudomedian3x3_SOURCES = pseudomedian3x3_main.c
rank_SOURCES            = rank_main.c
fractile_SOURCES        = fractile_main.c
