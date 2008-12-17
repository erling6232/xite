#
#      Makefile.am for src/statistics
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#
PROG  = statistics image_minmax histoMeanStd
MANP  = statistics.m band_minmax.m histoMeanStd.m
CINCL =  statistics.i

bin_PROGRAMS=$(PROG) $(XPROG)

LDADD = libxite.la

statistics_SOURCES   = statistics_main.c
image_minmax_SOURCES = image_minmax_main.c
histoMeanStd_SOURCES = histoMeanStd_main.c
