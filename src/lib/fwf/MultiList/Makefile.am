#
#      Makefile definitions for lib/fwf/MultiList
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

#
FWFOBJ  = MultiList.o
FWFINCL = MultiList.i MultiListP.i
ROFF3   = XfwfMultiList.man

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libfwfMultiList.la

libfwfMultiList_la_SOURCES = MultiList.c
