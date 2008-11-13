#
#      Makefile definitions for lib/fwf/Rows
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

#
FWFOBJ  = Rows.o
FWFINCL = Rows.i RowsP.i
ROFF3   = XfwfRows.man

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libfwfRows.la

libfwfRows_la_SOURCES = Rows.c
