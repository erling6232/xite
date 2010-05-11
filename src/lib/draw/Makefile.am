#
#      Makefile.am for lib/draw
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxitedraw.la

MANF =  bdf2biff.3 drawBand.3

man3_MANS=$(MANF)

libxitedraw_la_SOURCES = mkImg.c bdf2biff.c

drawBand.3:	mkImg.3
