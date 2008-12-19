
/*
  This header-file is produced automatically (at least in part) by
  makeHeader of BLAB, Ifi, UiO.
*/


/*

________________________________________________________________

        Xcolor.h
        $Id$
        Copyright 1990, Blab, UiO
        Image processing lab, Department of Informatics
        University of Oslo
        E-mail: blab@ifi.uio.no
________________________________________________________________
  
  Permission to use, copy, modify and distribute this software and its
  documentation for any purpose and without fee is hereby granted, 
  provided that this copyright notice appear in all copies and that 
  both that copyright notice and this permission notice appear in supporting
  documentation and that the name of B-lab, Department of Informatics or
  University of Oslo not be used in advertising or publicity pertaining 
  to distribution of the software without specific, written prior permission.

  B-LAB DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL B-LAB
  BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
  OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
  CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 

*/

#ifndef _XCOLOR_H_
#define _XCOLOR_H_

_XITE_CPLUSPLUS_BEGIN

#include <xite/Image.h>

extern void AddColormapCopy _XITE_PARAMS(( Widget w, Visual *visual, char *entryName ));
extern Colormap Color_map  _XITE_PARAMS(( int map, int xcolors ));
extern Colormap Color_map2  _XITE_PARAMS(( int map, int xcolors, Visual *visual ));
extern int *Xcolor_deftab  _XITE_PARAMS(( int n ));
extern int Color_max  _XITE_PARAMS(( void ));
extern int Color_ovl_max  _XITE_PARAMS(( void ));
extern int Color_ovl_set  _XITE_PARAMS(( ImageWidget wid, int color ));
extern int Color_set  _XITE_PARAMS(( ImageWidget wid, int color ));
extern void Color_add  _XITE_PARAMS(( XColor coltab[], char *name, int len ));
extern void Color_exit  _XITE_PARAMS(( void ));
extern void ColorsMenuInit  _XITE_PARAMS(( Display *display, int screenno, Widget parent, char *colorfile ));
extern void OverlaysMenuInit  _XITE_PARAMS(( Display *display, int screenno, Widget parent, char *colorfile ));
extern void Color_ovl_add  _XITE_PARAMS(( XColor coltab[], char *name, int len ));
extern void Color_ovl_read  _XITE_PARAMS(( char *file_name ));
extern void Color_read  _XITE_PARAMS(( char *file_name ));
extern Boolean Has_changeable_map _XITE_PARAMS(( Display *dpy, int screen, Visual *visual ));
extern char *ColortabName _XITE_PARAMS(( int coltab ));
extern char *OvltabName _XITE_PARAMS(( int coltab ));

_XITE_CPLUSPLUS_END

#endif /* _XCOLOR_H_ */
