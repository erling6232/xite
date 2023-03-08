#
#      Makefile.am for lib/sigma
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxitesigma.la

MANF =  sigmat.3 noiseStDev.3 sigma.3

man3_MANS=$(MANF)

libxitesigma_la_SOURCES = sigma.c

sigmat.3:	sigma.3
noiseStDev.3:	sigma.3
