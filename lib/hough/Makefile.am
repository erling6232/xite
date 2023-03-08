#
#      Makefile.am for lib/hough
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxitehough.la

MANF =  drawCircle.3 houghCircles.3 houghCircle.3 houghLine.3 drawLine.3

man3_MANS=$(MANF)

libxitehough_la_SOURCES = houghCircle.c houghLine.c

drawCircle.3:	houghCircle.3
houghCircles.3:	houghCircle.3
drawLine.3:	houghLine.3
