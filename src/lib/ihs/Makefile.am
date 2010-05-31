#
#      Makefile.am for lib/ihs
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxiteihs.la

MANF =  rgb2ihs_img.3 \
	ihs2rgb_img.3 rgb2ihs.3 ihs2rgb.3 rgb2ihs_byte.3 ihs2rgb_byte.3 \
	rgb2ihs_word.3 ihs2rgb_word.3 ihs.3

man3_MANS=$(MANF)

libxiteihs_la_SOURCES = ihs.c ihs2rgb.c rgb2ihs.c

rgb2ihs_img.3:	rgb2ihs.3
ihs2rgb_img.3:	ihs2rgb.3
rgb2ihs.3:	ihs.3
ihs2rgb.3:	ihs.3
rgb2ihs_byte.3:	ihs.3
ihs2rgb_byte.3:	ihs.3
rgb2ihs_word.3:	ihs.3
ihs2rgb_word.3:	ihs.3
