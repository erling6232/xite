#
#      Makefile.am for src/statistics
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
PROG  = statistics image_minmax histoMeanStd
MANP  = statistics.1 image_minmax.1 histoMeanStd.1
CINCL =  statistics.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

statistics_SOURCES   = statistics_main.c
image_minmax_SOURCES = image_minmax_main.c
histoMeanStd_SOURCES = histoMeanStd_main.c
