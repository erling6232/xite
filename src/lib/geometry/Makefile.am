#
#      Makefile.am for lib/geometry
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxitegeometry.la

libxitegeometry_la_SOURCES = affine.c quadratic.c rotate.c shift_img.c \
	transpose_img.c matrix.c warp.c
