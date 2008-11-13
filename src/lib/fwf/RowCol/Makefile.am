#
#      Makefile definitions for lib/fwf/RowCol
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

#
FWFOBJ  = RowCol.o
FWFINCL = RowCol.i RowColP.i
ROFF3   = XfwfRowCol.man

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libfwfRowCol.la

libfwfRowCol_la_SOURCES = RowCol.c
