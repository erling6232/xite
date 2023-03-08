#
#      Makefile.am for lib/segmentation
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxitesegmentation.la

MANF =  segmRandom.3 reclassifyBoundary.3 segmSpannWilson.3

man3_MANS=$(MANF)

libxitesegmentation_la_SOURCES = segmSpannWilson.c segmRandom.c

reclassifyBoundary.3:	segmSpannWilson.3
