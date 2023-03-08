#
#      Makefile.am for src/texture
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
PROG  = glcm   glcmParameter   glRunLength   peanoScan 
MANP  = glcm.1 glcmParameter.1 glRunLength.1 peanoScan.1 
CINCL =  texture.i

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/libxite.la

glcm_SOURCES          = glcm_main.c
glcmParameter_SOURCES = glcmParameter_main.c
glRunLength_SOURCES   = glRunLength_main.c
peanoScan_SOURCES     = peanoScan_main.c
