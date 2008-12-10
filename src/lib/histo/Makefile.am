#
#      Makefile.am for lib/histo
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxitehisto.la

libxitehisto_la_SOURCES = combineTransf.c histo.c histoEq.c histoEqCol.c \
	histoNorm.c histoSpecify.c gammaAdjust.c histoTransf.c invertHisto.c \
	mkCumHisto.c mkHisto.c scaleHisto.c scaled2col.c
