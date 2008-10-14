#
#      Makefile.am for lib/convert
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxiteconvert.la

libxiteconvert_la_SOURCES = biffConvert.c ascii2biff.c pseudo2rgb.c \
	biff2raw.c biff2ascii.c biffswap.c
