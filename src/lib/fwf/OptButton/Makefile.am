#
#      Makefile definitions for lib/fwf/OptButton
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

#
FWFOBJ  = OptButton.o
FWFINCL = OptButton.i OptButtonP.i
ROFF3   = XfwfOptionButton.man

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libfwfOptButton.la

libfwfOptButton_la_SOURCES = OptButton.c
