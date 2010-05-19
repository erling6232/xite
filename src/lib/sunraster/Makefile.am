#
#      Makefile.am for lib/sunraster
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxitesunraster.la

MANF =  write_sunraster.3 read_sunraster.3

man3_MANS=$(MANF)

libxitesunraster_la_SOURCES = biff2sunraster.c sunraster2biff.c

write_sunraster.3:	biff2sunraster.3
read_sunraster.3:	sunraster2biff.3
