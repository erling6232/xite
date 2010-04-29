#
#      Makefile.am for lib/convert
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxiteconvert.la

MANF =  ascii2biff.3 biff2ascii.3 biff2raw.3 mkConvertBand.3 convertBand.3 \
	biff_swap.3 pseudo2rgb.3

man3_MANS=$(MANF)

libxiteconvert_la_SOURCES = biffConvert.c ascii2biff.c pseudo2rgb.c \
	biff2raw.c biff2ascii.c biffswap.c

mkConvertBand.3: biffConvert.c
convertBand.3: biffConvert.c
