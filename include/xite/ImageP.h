

/*

________________________________________________________________

        ImageP.h
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


#ifndef _ImageP_h
#define _ImageP_h

_XITE_CPLUSPLUS_BEGIN

/* include superclass private header file */
#include <X11/CoreP.h>
#include <X11/CompositeP.h>

#include "Image.h"

/* define unique representation types not found in <X11/StringDefs.h> */

#define XtRImageResource		"ImageResource"

typedef struct {
  int count,                /* Number of widgets in 'widget_list'. */
      widget_list_length;   /* Max. no of widgets allowed in 'widget_list'. */
  Widget *widget_list;      /* Reference to list of widgets. */
  ipixelptr pixel_class;    /* Reference to list of installed pixel types. */
  int zoom_all;
} ImageClassPart;


typedef struct _ImageClassRec {
  CoreClassPart	core_class;
  CompositeClassPart  composite_class;
  ImageClassPart	image_class;
} ImageClassRec;

extern ImageClassRec imageClassRec;

typedef struct {

  /*********************************************
   *
   * resources (public variables)
   *
   * *******************************************/

  struct imagestruct
		  *image;        /* Image pointer. */
  struct bandstruct              /* Band pointers. */
		  *band,
		  *red_band,
		  *green_band,
		  *blue_band;
  ImageData                      /* Pointers to the actual data, not the
				  * info records of BIFF format. */
		  data,
		  red_data,
		  green_data,
		  blue_data;
  Boolean         image_rgb;     /* Image widget displays three-band rgb. */

  char            *title;
  char 	          *pixname;      /* One of ImageBytePixel,
				  * ImageUnsignedShortPixel, ... */

  Boolean         print_values;

  /* The folling five geometry attributes don't change during a session.
   * They are all relative to the BIFF image coordinate system.
   */
  int             width,         /* Width of BIFF image in pixels. */
		  height,        /* Height of BIFF image in pixels. */
		  line_length,   /* Line-length of BIFF image in pix. */
		  xstart, ystart;/* Start of BIFF image. */

  Boolean                        /* TRUE if variable aspect ratio. */
		  variable_aspect;
  int             x_aspect,      /* Aspect ratio if variable_aspect == 0. */
		  y_aspect;
  display_method  displaymethod; /* Possible values defined in Image.h.
				  * Initialized to 0 for 7-bit color, 1 for
				  * 8-bit, monochrome and others (in the
				  * function 'Display_picture'). */
  int             colorcells;
  int             *colorptr;
  int             colortab;
  scale_type      scaletype;
  float           scalea,        /* Used for scaling and thresh. level. */
		  scaleb,        /* Used for scaling. */
		  minval, maxval;
  char            *range;        /* How to scale pixel values. */
  Visual          *visual;
  XFontStruct     *font[5];
  XtCallbackList  button_action;
  XtCallbackList  resize_action;

  Boolean         menubar;       /* Whether or not to display menubar. */
  update_status   update;        /* UPDATE_RESIZE: Resize,
				  * UPDATE_ZOOMPAN: Zoom/pan values changed,
				  * UPDATE_REDISPLAY: Redisplay area,
				  * UPDATE_NEWSIZE: New image width and or
				  *                 image height.
				  * UPDATE_MINMAX: Recalculate minval and
				  *                maxval to get correct
				  *                pixel scaling. */ 

  /*********************************************
   *
   * private variables
   *
   * *******************************************/

  /* The following two attributes are always reset to the size of the
   * new pixmap.
   */
  int            internal_width; /* Last width, before autom. resize */
  int            internal_height;/* Last height, before autom. resize */

  /* Attributes for original (first-time, i.e. when the window is first
     created) position and size. */
  Position       orig_x,
                 orig_y;
  Dimension      orig_width,
                 orig_height;

  int		 has_colormap;   /* True if Visual Type is StaticGray,
				  * StaticColor or TrueColor. False if
				  * Visual type is GrayScale, PseudoColor
				  * or DirectColor. */
  int		 cells;          /* Map_entries (maximum of each primary
				  * if composite colormap) */
  XImage         *ximg;          /* Holds the pixmap which is displayed in
				  * a window. ximg->widht and ximg->height
				  * vary with window size, but not with
				  * zooming/panning. For zooming/panning
				  * only a region of the window is shown. */
  GC             gc;
  XFontStruct    *font_info;
  int            resize;         /* False if one-to-one mapping between
				  * window pixmap size and BIFF image size.
				  */
  double         factor;         /* Ratio of pixels visible in widget. */
  float          zoom;           /* Magnification factor. */
  int            xpan, ypan;     /* Refer to the BIFF image coordinate
				  * system. */

  int            *xptr, *yptr;   /* Pointers which will refer to
				  * transformation vectors or lookup tables
				  * from window pixel number to BIFF
				  * image pixel number in x- and y-
				  * direction respectively. */
  ipixelptr      pixtype;


  int            updatex, updatey,
                 updatew, updateh;/* Coordinates which tell what area of an
				   * image is to be updated when
				   * update==UPDATE_REDISPLAY. */

} ImagePart;

typedef struct _ImageRec {
  CorePart	    core;
  CompositePart   composite;
  ImagePart	    image;
} ImageRec;

_XITE_CPLUSPLUS_END

#endif  /* _ImageP_h */
