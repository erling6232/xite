#
#      Makefile definitions for lib/fwf/MenuBar
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

#
FWFOBJ  = MenuBar.o
FWFINCL = MenuBar.i MenuBarP.i
ROFF3   = XfwfMenuBar.man

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libfwfMenuBar.la

libfwfMenuBar_la_SOURCES = MenuBar.c
