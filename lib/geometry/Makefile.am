#
#      Makefile.am for lib/geometry
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxitegeometry.la

MANF =  affine.3 quadratic.3 rotate.3 shift_band.3 transpose_band.3 \
	warp_resample.3 oiWarpMapping.3 warp.3

libxitegeometry_la_SOURCES = affine.c quadratic.c rotate.c shift_img.c \
	transpose_img.c matrix.c warp.c

shift_band.3:	shift_img.3
transpose_band.3:	transpose_img.3
warp_resample.3:	warp.3
oiWarpMapping.3:	warp.3
