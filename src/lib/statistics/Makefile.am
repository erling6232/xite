#
#      Makefile.am for lib/statistics
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxitestatistics.la

MANF =  band_minmax_disp.3 band_minmax.3 histoMeanStd.3 statisticMask.3 \
	statisticMasks.3 hstatistics.3 statistic.3 cstatistic.3 \
	statistics.3

man3_MANS=$(MANF)

libxitestatistics_la_SOURCES = statistics.c band_minmax.c histoMeanStd.c

band_minmax_disp.3:	band_minmax.3
statisticMask.3:	statistics.3
statisticMasks.3:	statistics.3
hstatistics.3:	statistics.3
statistic.3:	statistics.3
cstatistic.3:	statistics.3
