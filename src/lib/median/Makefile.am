#
#      Makefile.am for lib/median
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxitemedian.la

libxitemedian_la_SOURCES = lit.c maxHisto.c median.c mode.c \
	pseudomedian3x3.c rank.c fractile.c fractile_mask.c
