#
#      Makefile.am for src/geometry
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#
PROG    = affine quadratic reorganize rotate shift_img transpose_img \
          warp
#SCRIPTS = warpExAnalysis
XPROG   = xwarp
MANP    = affine.1 quadratic.1 reorganize.1 rotate.1 shift_img.1\
          transpose_img.1 warp.1 xwarp.1
CINCL   = geometry.i matrix.i

bin_PROGRAMS=$(PROG) $(XPROG)
bin_SCRIPTS = warpExAnalysis
man1_MANS=$(MANP)

warpExAnalysis:	warpExAnalysis.sh
	cp -p $(srcdir)/warpExAnalysis.sh warpExAnalysis

LDADD = ../../lib/libxite.la

affine_SOURCES        = affine_main.c
quadratic_SOURCES     = quadratic_main.c
reorganize_SOURCES    = reorganize_main.c
rotate_SOURCES        = rotate_main.c
shift_img_SOURCES     = shift_img_main.c
transpose_img_SOURCES = transpose_img_main.c
warp_SOURCES          = warp_main.c
xwarp_SOURCES         = xwarp_main.c
xwarp_LDADD = ../../lib/libxiteX.la $(LDADD) $(LIBGUI)
