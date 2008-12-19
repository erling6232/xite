#
#      Makefile definitions for lib/fwf/TextMenu
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

#
FWFOBJ  = TextMenu.o
FWFINCL = TextMenu.i TextMenuP.i
ROFF3   = XfwfTextMenu.man

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libfwfTextMenu.la
AM_CPPFLAGS = -I$(top_srcdir)/include

libfwfTextMenu_la_SOURCES = TextMenu.c
