#
#      Makefile.am for src/median
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#
PROG  = lit maxHisto median mode pseudomedian3x3 rank \
        fractile
MANP  = lit.m maxHisto.m median.m  mode.m pseudomedian3x3.m rank.m \
        fractile.m fractile_mask.m
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
