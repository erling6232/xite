#
#      Makefile.am for src/thresMl
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
PROG  = thresMlReddi   thresMlWaHa   thresMlCentroid   thresMl        \
	thresMlComCur
MANP  = thresMlReddi.m thresMlWaHa.m thresMlCentroid.m thresMl.m \
	thresMlComCur.m
CINCL =  thresMl.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

thresMlReddi_SOURCES    = thresMlReddi_main.c
thresMlWaHa_SOURCES     = thresMlWaHa_main.c
thresMlCentroid_SOURCES = thresMlCentroid_main.c
thresMl_SOURCES         = thresMl_main.c
thresMlComCur_SOURCES   = thresMlComCur_main.c
