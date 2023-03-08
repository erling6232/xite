#
#      Makefile.am for lib/texture
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxitetexture.la

MANF =  glcm.3 glcmAsm.3 glcmCont.3 glcmCorr.3 glcmIdm.3 glcmEntropy.3 \
	glcmVar.3 glcmProm.3 glcmShade.3 glcmInertia.3 glcmDiagmoment.3 \
	glcmParameter.3 glrlSre.3 glrlLre.3 glrlGln.3 glrlRln.3 \
	glrlRp.3 glrlSgre.3 glrlLgre.3 glrlParameters.3 glRunLength.3 \
	peanoScan.3

man3_MANS=$(MANF)

libxitetexture_la_SOURCES = glcm.c glcmParameter.c glRunLength.c peanoScan.c

glcmAsm.3:	glcmParameter.3
glcmCont.3:	glcmParameter.3
glcmCorr.3:	glcmParameter.3
glcmIdm.3:	glcmParameter.3
glcmEntropy.3:	glcmParameter.3
glcmVar.3:	glcmParameter.3
glcmProm.3:	glcmParameter.3
glcmShade.3:	glcmParameter.3
glcmInertia.3:	glcmParameter.3
glcmDiagmoment.3:	glcmParameter.3
glrlSre.3:	glrlParameters.3
glrlLre.3:	glrlParameters.3
glrlGln.3:	glrlParameters.3
glrlRln.3:	glrlParameters.3
glrlRp.3:	glrlParameters.3
glrlSgre.3:	glrlParameters.3
glrlLgre.3:	glrlParameters.3
glrlParameters.3:	glRunLength.3
