#
#      Makefile definitions for lib/fwf/ScrolledList
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

#
FWFOBJ  = ScrList.o
FWFINCL = ScrList.i ScrListP.i
ROFF3   = XfwfScrolledList.man

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libfwfScrolledList.la

libfwfScrolledList_la_SOURCES = ScrList.c
