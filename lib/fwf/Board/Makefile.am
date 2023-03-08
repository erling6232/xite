#
#      Makefile definitions for lib/fwf/Board
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

#
FWFOBJ  = Board.o
FWFINCL = Board.i BoardP.i
ROFF3   = XfwfBoard.man

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libfwfBoard.la
AM_CPPFLAGS = -I$(top_srcdir)/include

libfwfBoard_la_SOURCES = Board.c
