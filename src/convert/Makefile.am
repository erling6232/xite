#
#      Makefile.am for src/convert
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
PROG    = biffConvert raw2biff ascii2biff pseudo2rgb biff2raw biff2ascii \
	  biffswap
XPROG   =
MANP    = biffConvert.1 raw2biff.1 ascii2biff.1 pseudo2rgb.1 biff2raw.1 \
	  biff2ascii.1 biffswap.1
CINCL   = convert.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

ascii2biff_SOURCES = ascii2biff_main.c
biff2ascii_SOURCES = biff2ascii_main.c
biff2raw_SOURCES   = biff2raw_main.c
biffConvert_SOURCES= biffConvert_main.c
biffswap_SOURCES   = biffswap_main.c
pseudo2rgb_SOURCES = pseudo2rgb_main.c
raw2biff_SOURCES   = raw2biff_main.c
