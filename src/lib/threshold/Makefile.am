#
#      Makefile.am for lib/threshold
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxitethreshold.la

libxitethreshold_la_SOURCES = threshold.c thresLloyd.c thresOtsu.c \
	thresPtile.c thresRidCal.c thresBernsen.c thresSigma.c \
	thresMinErr.c thresUniErr.c
