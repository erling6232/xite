#
#      Makefile.am for lib/histo
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxitehisto.la

MANF =  combineTransf.3 gammaAdjust.3 histo.3 histoEq.3 histoEqCol.3 \
	histoNorm.3 histoSpecify.3 histoTransf.3 invertHisto.3 \
	mkCumHisto.3 mkIhistogram.3 mkDhistogram.3 mkHisto.3 \
	scaled2col.3 scaleHisto.3

man3_MANS=$(MANF)

libxitehisto_la_SOURCES = combineTransf.c histo.c histoEq.c histoEqCol.c \
	histoNorm.c histoSpecify.c gammaAdjust.c histoTransf.c invertHisto.c \
	mkCumHisto.c mkHisto.c scaleHisto.c scaled2col.c

mkIhistogram.3:	mkHisto.3
mkDhistogram.3:	mkHisto.3
