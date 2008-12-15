#
#      Makefile.am for lib/morph
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxitemorph.la

libxitemorph_la_SOURCES = morphDilate.c morphErode.c morphOpen.c morphClose.c mkMorph.c
