#
#      Makefile definitions for lib/fwf/PullDown
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

#
FWFOBJ  = PullDown.o
FWFINCL = PullDown.i PullDownP.i
ROFF3   = XfwfPullDown.man

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libfwfPullDown.la

libfwfPullDown_la_SOURCES = PullDown.c
