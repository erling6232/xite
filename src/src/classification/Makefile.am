#
#      Makefile.am for src/classification
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions for cdoc
#
PROG  = kMeansCluster   isoCluster   
XPROG =
MANP  = kMeansCluster.m isoCluster.m
CINCL =  classification.i

bin_PROGRAMS=$(PROG) $(XPROG)

LDADD = ../../lib/libxite.la

isoCluster_SOURCES = isoCluster_main.c
kMeansCluster_SOURCES = kMeansCluster_main.c
