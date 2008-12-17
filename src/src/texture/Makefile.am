#
#      Makefile.am for src/texture
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#
PROG  = glcm   glcmParameter   glRunLength   peanoScan 
MANP  = glcm.m glcmParameter.m glRunLength.m peanoScan.m 
CINCL =  texture.i

bin_PROGRAMS=$(PROG) $(XPROG)

LDADD = libxite.la

glcm_SOURCES          = glcm_main.c
glcmParameter_SOURCES = glcmParameter_main.c
glRunLength_SOURCES   = glRunLength_main.c
peanoScan_SOURCES     = peanoScan_main.c
