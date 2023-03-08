#
#      Makefile definitions for lib/fwf/Button
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

#
FWFOBJ  = Button.o
FWFINCL = Button.i ButtonP.i
ROFF3   = XfwfButton.man

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libfwfButton.la
AM_CPPFLAGS = -I$(top_srcdir)/include

libfwfButton_la_SOURCES = Button.c
