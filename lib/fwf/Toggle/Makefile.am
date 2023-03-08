#
#      Makefile definitions for lib/fwf/Toggle
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

#
FWFOBJ  = Toggle.o
FWFINCL = Toggle.i ToggleP.i
ROFF3   = XfwfToggle.man

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libfwfToggle.la
AM_CPPFLAGS = -I$(top_srcdir)/include

libfwfToggle_la_SOURCES = Toggle.c
