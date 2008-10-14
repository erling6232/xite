#
#      Makefile.am for src/convert
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#
PROG    = biffConvert raw2biff ascii2biff pseudo2rgb biff2raw biff2ascii \
	  biffswap
XPROG   =
MANP    = biffConvert.m raw2biff.m ascii2biff.m pseudo2rgb.m biff2raw.m \
	  biff2ascii.m biffswap.m
CINCL   = convert.i

bin_PROGRAMS=$(PROG) $(XPROG)

LDADD = libxite.la

ascii2biff_SOURCES = ascii2biff_main.c
biff2ascii_SOURCES = biff2ascii_main.c
biff2raw_SOURCES   = biff2raw_main.c
biffConvert_SOURCES= biffConvert_main.c
biffswap_SOURCES   = biffswap_main.c
pseudo2rgb_SOURCES = pseudo2rgb_main.c
raw2biff_SOURCES   = raw2biff_main.c
