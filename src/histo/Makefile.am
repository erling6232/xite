#
#      Makefile.am for src/histo
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
PROG  = mkHisto histoEq histoEqCol histoNorm pixel_mapper gammaAdjust
MANP  = histoEq.1 histoEqCol.1 histoNorm.1 \
	mkHisto.1 \
	pixel_mapper.1 gammaAdjust.1
CINCL  = histo.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

mkHisto_SOURCES = mkHisto_main.c
histoEq_SOURCES = histoEq_main.c
histoEqCol_SOURCES = histoEqCol_main.c
histoNorm_SOURCES = histoNorm_main.c
pixel_mapper_SOURCES = pixel_mapper_main.c
gammaAdjust_SOURCES = gammaAdjust_main.c
