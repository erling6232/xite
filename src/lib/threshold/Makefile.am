#
#      Makefile.am for lib/threshold
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxitethreshold.la

MANF =  thresBernsen.3 thresholdSpOut.3 threshold.3 thresLloyd.3 \
	thresMinErrIter.3 thresMinErrCrit.3 thresMinErrEqVar.3 \
	thresOtsu.3 thresPtile.3 thresRidCal.3 thresSigma.3 \
	thresUniErr.3

man3_MANS=$(MANF)

libxitethreshold_la_SOURCES = threshold.c thresLloyd.c thresOtsu.c \
	thresPtile.c thresRidCal.c thresBernsen.c thresSigma.c \
	thresMinErr.c thresUniErr.c

thresholdSpOut.3:	threshold.3
thresMinErrIter.3:	thresMinErr.3
thresMinErrCrit.3:	thresMinErr.3
thresMinErrEqVar.3:	thresMinErr.3
