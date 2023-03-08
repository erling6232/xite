#
#      Makefile.am for lib/pyramid
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxitepyramid.la

MANF =  pyramidAver2.3 pyramidRank2.3 pyramidGauss5.3 mkPyramidAver2.3 \
	mkPyramidRank2.3 mkPyramidGauss5.3 pyramid.3

man3_MANS=$(MANF)

libxitepyramid_la_SOURCES = pyramid.c

pyramidAver2.3:	pyramid.3
pyramidRank2.3:	pyramid.3
pyramidGauss5.3:	pyramid.3
mkPyramidAver2.3:	pyramid.3
mkPyramidRank2.3:	pyramid.3
mkPyramidGauss5.3:	pyramid.3
