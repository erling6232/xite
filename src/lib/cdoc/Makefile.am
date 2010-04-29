#
#      Makefile.am for lib/cdoc
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxitecdoc.la

MANF =  add_html_toc.3 open_files.3 close_files.3 getfiles.3 attgetopt.3

man3_MANS=$(MANF)

libxitecdoc_la_SOURCES = add_html_toc.c getfiles.c getopt.c

open_files.3: getfiles.c
close_files.3: getfiles.c
attgetopt.3: getopt.c
