

/*

________________________________________________________________

        ImageOverlayP.h
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


#ifndef _ImageOverlayP_h
#define _ImageOverlayP_h

_XITE_CPLUSPLUS_BEGIN

#include "ImageOverlay.h"

/* include superclass private header file */
#include <X11/CoreP.h>
#include <X11/CompositeP.h>
#include "ImageP.h"

typedef struct {
  int dummy;
} ImageOverlayClassPart;

typedef struct _ImageOverlayClassRec {
    CoreClassPart	   core_class;
    CompositeClassPart     composite_class;
    ImageClassPart	   image_class;
    ImageOverlayClassPart  image_overlay_class;
} ImageOverlayClassRec;

extern ImageOverlayClassRec imageOverlayClassRec;

typedef struct {
  
  /******* Variables for overlay *******/

  /* Resources */
  struct bandstruct *overlay;
  Boolean protect;
  Boolean odisplay;        /* Whether or not to display overlay image. */
  int cells;
  int *colorptr;
  int overlaytab;
  int draw_value;
  XtCallbackList draw_action;

  /* Private state */
  ImageData data;
  ipixelptr pixtype;
  int line_length, oldx, oldy, origo, xorigo, yorigo, xsetorigo, ysetorigo;

  /******* Variables for roi *******/

  /* Resources */
  int x, y, width, height; /* Position and size of roi, relative to screen. */
  Boolean roi_zoom_pan;    /* True if roi is to zoom and pan with the image. */
  int display;             /* Whether or not to display roi. */
  int activated;           /* Whether roi is being changed. */
  int permanent;           /* True if roi is to permanently displayed. */
  int fill;                /* Fill roi. */
  int xaspect;             /* Non-zero if roi should be square. */
  int yaspect;
  XtCallbackList roi_action;

  /* Private state */
  int x_im, y_im;          /* Position of roi, relative to image. */
  int width_im, height_im; /* Size of roi, relative to image. */
  
} ImageOverlayPart;

typedef struct _ImageOverlayRec {
    CorePart        core;
    CompositePart   composite;
    ImagePart       image;
    ImageOverlayPart    image_overlay;
} ImageOverlayRec;

_XITE_CPLUSPLUS_END

#endif  /* _ImageOverlayP_h */
