#
#      Makefile definitions for lib/fwf/FileSelector
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

#
FWFOBJ  = FileSel.o
FWFINCL = FileSel.i FileSelP.i
ROFF3   = XfwfFileSelector.man

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libfwfFileSelector.la

libfwfFileSelector_la_SOURCES = FileSel.c
