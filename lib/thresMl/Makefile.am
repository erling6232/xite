#
#      Makefile.am for lib/thresMl
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxitethresMl.la

MANF =  thresMlAppScale.3 thresMlAppMean.3 thresMlAppMedian.3 \
	thresMlApply.3 histoCentroid.3 thresMlCentroid.3 mkComCur.3 \
	thresMlComCur.3 thresMlReddi.3 thresMlWaHa.3

man3_MANS=$(MANF)

libxitethresMl_la_SOURCES = thresMlReddi.c thresMlWaHa.c thresMlCentroid.c \
	thresMlApply.c thresMlComCur.c

thresMlAppScale.3:	thresMlApply.3
thresMlAppMean.3:	thresMlApply.3
thresMlAppMedian.3:	thresMlApply.3
histoCentroid.3:	thresMlCentroid.3
mkComCur.3:	thresMlComCur.3
