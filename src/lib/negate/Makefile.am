#
#      Makefile.am for lib/negate
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxitenegate.la

MANF =  fftAmp.3 absValue.3 complexConjug.3 negate.3

man3_MANS=$(MANF)

libxitenegate_la_SOURCES = negate.c absValue.c complexConjug.c

fftAmp.3:	absValue.3
