#
#      Makefile.am for src/classification
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions for cdoc
#
PROG  = kMeansCluster   isoCluster   
XPROG =
MANP  = kMeansCluster.1 isoCluster.1
CINCL =  classification.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

isoCluster_SOURCES = isoCluster_main.c
kMeansCluster_SOURCES = kMeansCluster_main.c
