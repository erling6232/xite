#
#      Makefile.am for src/geometry
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#
PROG    = affine quadratic reorganize rotate shift_img transpose_img \
          warp
#SCRIPTS = warpExAnalysis
XPROG   = xwarp
MANP    = affine.m quadratic.m reorganize.m rotate.m shift_img.m\
          transpose_img.m warp.m xwarp.m
CINCL   = geometry.i matrix.i

bin_PROGRAMS=$(PROG) $(XPROG)
bin_SCRIPTS = warpExAnalysis

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
xwarp_LDADD = $(LDADD) ../../lib/libxiteX.la ../../lib/fwf/libfwf.la $(LIBGUI)
