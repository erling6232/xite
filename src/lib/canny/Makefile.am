#
#      Makefile.am for src/canny
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxitecanny.la

MANF =  canny_filters.3 canny.3 canny_scale.3

man3_MANS=$(MANF)

libxitecanny_la_SOURCES = canny.c

canny_filters.3: canny.c
canny_scale.3:   canny.c
