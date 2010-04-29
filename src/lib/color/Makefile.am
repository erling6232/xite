#
#      Makefile.am for lib/color
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxitecolor.la

MANF =  color2ascii.3 color_create.3 color_read.3 color_write.3 color_convertfromimg.3 \
	PrintColortable.3 read_colormap.3 colorfile.3

man3_MANS=$(MANF)

libxitecolor_la_SOURCES = colorfile.c color2ascii.c

color_create.3: colorfile.c
color_read.3:	colorfile.c
color_write.3:	colorfile.c
color_convertfromimg.3: colorfile.c
PrintColortable.3: colorfile.c
read_colormap.3: colorfile.c
