#
#      Makefile.am for lib/convolve
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxiteconvolve.la

libxiteconvolve_la_SOURCES = convolve.c conv3x3.c gradient.c laplace.c \
	prewitt.c roberts.c sobel.c grad.c crossing.c lapZeroCross.c
