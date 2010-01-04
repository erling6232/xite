#
#      Makefile.am for src/knn
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#
PROG  = knn mbknn
MANP  = knn.m mbknn.m
CINCL =  knn.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

knn_SOURCES   = knn_main.c
mbknn_SOURCES = mbknn_main.c
