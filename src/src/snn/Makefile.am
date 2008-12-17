#
#      Makefile.am for src/snn
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#
PROG  = litSnn snn
MANP  = litSnn.m snn.m
CINCL =  snn.i

bin_PROGRAMS=$(PROG) $(XPROG)

LDADD = libxite.la

litSnn_SOURCES = litSnn_main.c
snn_SOURCES    = snn_main.c
