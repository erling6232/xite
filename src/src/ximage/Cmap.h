
/*
  This header-file is produced automatically (at least in part) by
  makeHeader of BLAB, Ifi, UiO.
*/


/*

________________________________________________________________

        Cmap.h
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

#ifndef _Cmap_H_
#define _Cmap_H_

_XITE_CPLUSPLUS_BEGIN

#include <X11/Intrinsic.h>


extern int RGBPixel _XITE_PARAMS(( int pix, Visual *visual ));
extern void InitColortablePixels _XITE_PARAMS(( XColor *col, int len, int offset, int composite ));
extern void InitColortableGray _XITE_PARAMS(( XColor *col, int len, int step, int composite ));
extern void InitColortableGrayPart _XITE_PARAMS(( XColor *col, int min, int max, int step, int composite ));
extern void SpreadGray _XITE_PARAMS(( XColor *col, int num_colors, int pix_min, int num_cells ));
extern Colormap cmapOfVisualClass _XITE_PARAMS(( int visualClass, int planes ));
extern Colormap cmapOfVisual _XITE_PARAMS(( Visual *visual, int planes ));
extern void PrintColormap _XITE_PARAMS(( Display *dpy, Colormap map, int len, int composite ));
extern void PrintWidgetColormap _XITE_PARAMS(( Display *dpy, Widget wid ));
extern void ColormapStd  _XITE_PARAMS(( Display *dpy, int screen, Colormap map, int visualClass, int nr ));
extern void ColormapCopy  _XITE_PARAMS(( Display *dpy, int screen, Colormap dest, Colormap source, int composite ));
extern void GetImageColormap _XITE_PARAMS(( Display *dpy, int screen, int planes, Visual *visual, Colormap *colormap ));

_XITE_CPLUSPLUS_END

#endif /* _Cmap_H_ */
