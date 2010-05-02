#
#      Makefile.am for lib/convolve
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxiteconvolve.la

MANF =  conv3x3.3 convolve.3 crossing.3 sobel.3 prewitt.3 roberts.3 \
	grad.3 gradient.3 laplace.3 lapZeroCross.3

man3_MANS=$(MANF)

libxiteconvolve_la_SOURCES = convolve.c conv3x3.c gradient.c laplace.c \
	prewitt.c roberts.c sobel.c grad.c crossing.c lapZeroCross.c

sobel.3:	gradient.3
prewitt.3:	gradient.3
roberts.3:	gradient.3
grad.3:		gradient.3
gradient.3:	gradient_op.3
