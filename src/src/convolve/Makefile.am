#
#      Makefile.am for src/convert
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
PROG  = convolve conv3x3 gradient laplace prewitt roberts sobel grad crossing lapZeroCross
XPROG   =
MANP  = convolve.1 conv3x3.1 gradient.1 laplace.1 crossing.1 lapZeroCross.1
CINCL =  convolve.i
#MANP = sobel.1 prewitt.1 roberts.1 grad.1 

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

conv3x3_SOURCES      = conv3x3_main.c
convolve_SOURCES     = convolve_main.c
crossing_SOURCES     = crossing_main.c
gradient_SOURCES     = gradient_main.c
grad_SOURCES         = grad_main.c
laplace_SOURCES      = laplace_main.c
lapZeroCross_SOURCES = lapZeroCross_main.c
prewitt_SOURCES      = prewitt_main.c
roberts_SOURCES      = roberts_main.c
sobel_SOURCES        = sobel_main.c
