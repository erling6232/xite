#
#      Makefile.am for src/arithmetic
#


@SET_MAKE@

SHELL = /bin/sh
VPATH = @srcdir@:@top_srcdir@/include:@configdir@/include
srcdir = @srcdir@
top_srcdir = @top_srcdir@
configdir = @configdir@

include $(configdir)/@common_makefile@

# AM_CPPFLAGS = -I. -I$(srcdir) -I$(top_srcdir)/include -I$(configdir)/include $(LOCALINCLUDES)

# subdirs = @subdirs@
# infodir = $(prefix)/info


# all: install-include install-bin

#install-include:
#	$(configdir)/mkinstalldirs $(includedir)/xite
#	for file in *.h ; do \
#		$(INSTALL_DATA) $$file $(includedir)/xite ; \
#	done
#
#install-bin:

#
#   Makefile definitions for arithmetic
#
OBJ   = absDiff.o  addw.o    average.o   crossSection.o divide.o   logical.o \
	multiply.o profile.o rms.o       scale.o        signDiff.o square.o  \
	squareRoot.o real.o  imag.o      power.o        phase.o    logarithm.o
PROG  = absDiff    addw      average     crossSection   divide     logical \
	multiply   profile   rms         scale          signDiff   square  \
	squareRoot real      imag        power          phase      logarithm
XPROG = xadd
MANP  = absDiff.m  addw.m    average.m   crossSection.m divide.m   logical.m \
	multiply.m profile.m rms.m       scale.m        signDiff.m square.m \
	squareRoot.m xadd.m  real.m      imag.m         power.m    phase.m \
	logarithm.m
CINCL = arithmetic.i

bin_PROGRAMS=$(PROG) $(XPROG)
include_HEADERS=arithmetic.h

absDiff_SOURCES = absDiff_main.c absDiff.c arithmetic.h

#include:cincl
#object: cobj
#lib:    clib
#dso:    xite_dso
#xprog:  $(XPROG)
#program:cprog xprog
#doc:    manpage

