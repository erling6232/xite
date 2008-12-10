#
#      Makefile.am for lib/Image
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxiteImage.la

libxiteImage_la_SOURCES = Image.c ImageFormat.c ImageBytePix.c ImageSignedBytePix.c \
	ImageShortPix.c ImageUnsignedShortPix.c ImageLongPix.c \
	ImageFloatPix.c ImageDoublePix.c ImageOverlay.c \
	ImageOverlayPix.c ImageComplexPix.c ImageDoubleComplexPix.c
