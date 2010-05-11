#
#      Makefile.am for lib/fft
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxitefft.la

MANF =  fft2d.3 fft.3

man3_MANS=$(MANF)

libxitefft_la_SOURCES = tab.c dintime.c idintime.c bitrev.c b512.c dint.c idint.c fft2d.c

fft.3:	fft2d.3
