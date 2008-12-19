
/*
  This header-file is produced automatically (at least in part) by
  makeHeader of BLAB, Ifi, UiO.
*/


/*

________________________________________________________________

        Visual.h
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

#ifndef _Visual_H_
#define _Visual_H_

_XITE_CPLUSPLUS_BEGIN

#include <X11/Intrinsic.h>

#define ClassOfVisual(v)           ((v)->class)
#define MapEntriesOfVisual(v)      ((v)->map_entries)
#define RedMaskOfVisual(v)         ((v)->red_mask)
#define GreenMaskOfVisual(v)       ((v)->green_mask)
#define BlueMaskOfVisual(v)        ((v)->blue_mask)
#define PseudoMaskOfVisual(v)      ((v)->blue_mask)
#define BitsPerRgbOfVisual(v)      ((v)->bits_per_rgb)
#define VisualIDOfVisual(v)        ((v)->visualid)

#define RedMaskOfVisualInfo(vi)    ((vi).red_mask)
#define GreenMaskOfVisualInfo(vi)  ((vi).green_mask)
#define BlueMaskOfVisualInfo(vi)   ((vi).blue_mask)
#define ClassOfVisualInfo(vi)      ((vi).class)
#define ScreenOfVisualInfo(vi)     ((vi).screen)
#define DepthOfVisualInfo(vi)      ((vi).depth)
#define BitsPerRgbOfVisualInfo(vi) ((vi).bits_per_rgb)
#define VisualOfVisualInfo(vi)     ((vi).visual)
#define MapSizeOfVisualInfo(vi)    ((vi).colormap_size)



extern void InitializeVisualInfos _XITE_PARAMS(( Display *dpy, int screen, char *imageVisualClass, XVisualInfo **imageVisualInfo ));
extern void RequestedImageVisual _XITE_PARAMS(( Display *dpy, int screen, int *planes, Visual **visual ));
extern int RequestedPlanes _XITE_PARAMS(( XVisualInfo *visualInfo ));
extern XVisualInfo *Pseudo8VisualInfo _XITE_PARAMS(( void ));
extern XVisualInfo *Direct24VisualInfo _XITE_PARAMS(( void ));
extern XVisualInfo *True24VisualInfo _XITE_PARAMS(( void ));
extern XVisualInfo *VisualInfoFromClass _XITE_PARAMS(( int visualClass, int planes ));
extern Visual *visualFromName _XITE_PARAMS(( char *name, int planes ));
extern Visual *visualFromClass _XITE_PARAMS(( int visualClass, int planes ));
extern char *visualClassToName _XITE_PARAMS(( int visualClass ));
extern int visualNameToClass _XITE_PARAMS(( char *name ));
extern int DepthOfVisual _XITE_PARAMS(( Visual *visual ));
extern int DepthToReducedPlanes _XITE_PARAMS(( int depth, Visual *visual ));
extern int PlanesToDepth _XITE_PARAMS(( int planes ));

_XITE_CPLUSPLUS_END

#endif /* _Visual_H_ */
