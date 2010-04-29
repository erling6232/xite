#
#      Makefile.am for lib/classification
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxiteclassification.la

MANF =  classifyNearest.3 classMeans.3

man3_MANS=$(MANF)

libxiteclassification_la_SOURCES = kMeansCluster.c
classifyNearest.3: kMeansCluster.c
classMeans.3: kMeansCluster.c
