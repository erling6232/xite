#
#      Makefile definitions for lib/fwf/Frame
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

#
FWFOBJ  = Frame.o
FWFINCL = Frame.i FrameP.i stip4.i
ROFF3   = XfwfFrame.man

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libfwfFrame.la
AM_CPPFLAGS = -I$(top_srcdir)/include

libfwfFrame_la_SOURCES = Frame.c
