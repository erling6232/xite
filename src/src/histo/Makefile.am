#
#      Makefile.am for src/histo
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#
PROG  = mkHisto histoEq histoEqCol histoNorm pixel_mapper gammaAdjust
MANP  = combineTransf.m histo.m histoEq.m histoEqCol.m histoNorm.m \
	histoSpecify.m histoTransf.m invertHisto.m mkCumHisto.m mkHisto.m \
	scaleHisto.m scaled2col.m pixel_mapper.m gammaAdjust.m
CINCL  = histo.i

bin_PROGRAMS=$(PROG) $(XPROG)

LDADD = ../../lib/libxite.la

mkHisto_SOURCES = mkHisto_main.c
histoEq_SOURCES = histoEq_main.c
histoEqCol_SOURCES = histoEqCol_main.c
histoNorm_SOURCES = histoNorm_main.c
pixel_mapper_SOURCES = pixel_mapper_main.c
gammaAdjust_SOURCES = gammaAdjust_main.c
