#
#      Makefile.am for lib/xshow
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxitexshow.la

MANF =  start_xshow.3

man3_MANS=$(MANF)

libxitexshow_la_SOURCES = fork_xshow.c

start_xshow.3:	fork_xshow.3
