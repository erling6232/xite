#
#      Makefile.am for src/knn
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
PROG  = knn mbknn
MANP  = knn.1 mbknn.1
CINCL =  knn.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

knn_SOURCES   = knn_main.c
mbknn_SOURCES = mbknn_main.c
