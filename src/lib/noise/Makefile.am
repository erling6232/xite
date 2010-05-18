#
#      Makefile.am for lib/noise
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxitenoise.la

MANF =  addGauss.3 addPoisson.3 mkGauss.3 mkPoisson.3 multGauss.3

man3_MANS=$(MANF)

libxitenoise_la_SOURCES = mkGauss.c addGauss.c multGauss.c mkPoisson.c addPoisson.c
