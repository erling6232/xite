#
#      Makefile.am for src/negate
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
PROG  = negate absValue complexConjug
MANP  = negate.1 absValue.1 complexConjug.1
CINCL =  negate.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

negate_SOURCES        = negate_main.c
absValue_SOURCES      = absValue_main.c
complexConjug_SOURCES = complexConjug_main.c
