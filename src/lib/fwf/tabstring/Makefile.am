#
#      Makefile definitions for lib/fwf/tabstring
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

#
FWFOBJ  = DrawImageString.o DrawString.o Tablist2Tabs.o TextWidth.o strnchr.o
FWFINCL = TabString.i
ROFF3   =

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libfwftabstring.la

libfwftabstring_la_SOURCES = DrawImageString.c DrawString.c Tablist2Tabs.c TextWidth.c strnchr.c
