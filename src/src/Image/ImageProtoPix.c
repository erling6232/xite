

/*

________________________________________________________________

        ImageProtoPix.c
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

#include <xite/includes.h>
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#include <ctype.h>
#include <X11/Xos.h>
#include <X11/IntrinsicP.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <X11/StringDefs.h>
#ifdef IMAGE_OVERLAY
#include "ImageOverlayP.h"
#endif
#include "ImageP.h"
#ifdef HAVE_MALLOC_H
# include <malloc.h>
#endif
#include <xite/Visual.h>
#include <xite/debug.h>

#define ERR_OK		0
#define ERR_MEM		1



/*L:ImageXxxxResize*

________________________________________________________________

		ImageXxxxResize
________________________________________________________________

Name:		ImageXxxxResize
Syntax:		| ImageXxxxResize(wid)
                | ImageWidget wid;
Description:	Resize the widget
Return value:
Author:		Otto Milvang
________________________________________________________________

*/

/* The following macros are already defined when the image consists of unsigned
 * byte pixels or overlay pixels. For other pixel types, these macros will be
 * used. */

#ifndef RESIZE_NO
#define RESIZE_NO(a) *a++
#define RESIZE_YES(a) a[*xptr_s]
#define clip(F, a, b) (F < a ? a : (F>b ? b : F ))
#define EQ_PRO(left, OP, right) left OP f1 * right + f2
#define EQ_24(a) \
  { EQ_PRO(f0, =, a); *ximg_ptr_s++ = trans_s[(int)clip(f0, 0.0, 255.0)]; }
#define EQ_8(a)\
  { EQ_PRO(f0, =, a); *ximg_ptr_s++ = trans_s[(int)clip(f0, 0.0, 255.0)]; }
#define EQ_4A(a)\
  { EQ_PRO(f0, =, a); j = clip(f0, 0.0, 15.0); *ximg_ptr_s++ += trans_s[j]; }
#define EQ_4B(a)\
  { EQ_PRO(f0, =, a); j = clip(f0, 0.0, 15.0); *ximg_ptr_s = trans_s[j << 4]; }
#define EQ_2A(a) EQ_PRO(0.0, <=, a)  
#define EQ_2B    = 
#endif


#ifndef INIT24
#define INIT24\
  f1 = scalea_s * 256.0/(maxval_s-minval_s);\
  f2 = (scaleb_s-minval_s) * 256.0/(maxval_s-minval_s);
#define INIT21\
  f1 = scalea_s * cells_s/(maxval_s-minval_s);\
  f2 = (scaleb_s-minval_s) * cells_s/(maxval_s-minval_s);
#define INIT8 INIT24
#define INIT7 INIT21
#define INIT4\
  f1 = scalea_s * 16.0/(maxval_s-minval_s);\
  f2 = (scaleb_s-minval_s) * 16.0/(maxval_s-minval_s);
#define INIT2\
  f1 = scalea_s /(maxval_s-minval_s);\
  f2 = (scaleb_s-minval_s) /(maxval_s-minval_s) - 0.5;
#endif

#ifndef ADDRESS
#define ADDRESS ImageScreenXYtoAdr
#endif

static  ImagePart *ip_s;
static  XImage *ximg_s;
static  int ximg_width_s, ximg_height_s, new_width_s, new_height_s,
        xstart_s, ystart_s, bytes_per_line_s, lpl_s; 
static  int resize_s, *xarr_s;
static  ImagePixel *btr_s, *obtr_s;
static  unsigned char *ximg_ptr_s, *optr_s, *fptr_s;
static  int i, cells_s, *xptr_s, pix_s;
static  double scalea_s, scaleb_s, minval_s, maxval_s;
static  unsigned char trans_s[256];

#if !defined(COMPLEXPIX) && !defined(DOUBLECOMPLEXPIX)
static  int j;
#endif

#if defined(IMAGE_OVERLAY)
static  int k;
#endif



#ifdef IMAGE_OVERLAY
  static void Resize24LSB(ImageOverlayWidget wid, long rmask, long gmask, long bmask)
#else
  static void Resize24LSB(ImageWidget wid, long rmask, long gmask, long bmask)
#endif
{
  ImagePixel *r_btr, *g_btr, *b_btr;
#if !defined(BYTEPIX) && !defined(IMAGE_OVERLAY) && !defined(COMPLEXPIX) && !defined(DOUBLECOMPLEXPIX)
  double f0, f1, f2;
#endif
  int rx=0, gx=0, bx=0;
  int x, y;

  ENTER_FUNCTION_DEBUG("ImageProtoPix.c: Resize24LSB");

  if (rmask == 0xff0000) rx = 1; else
  if (gmask == 0xff0000) gx = 1; else 
    bx = 1;
  if (rmask == 0xff00) rx = 2; else
  if (gmask == 0xff00) gx = 2; else 
    bx = 2;
  if (rmask == 0xff) rx = 3; else
  if (gmask == 0xff) gx = 3; else 
    bx = 3;

  /* 4 bytes per pixel is sufficient for 24 bits. */
  bytes_per_line_s = ximg_s -> bytes_per_line = ximg_width_s * 4;

  if (!ximg_s->data) ximg_s->data = malloc(bytes_per_line_s*ximg_height_s);
  if (ximg_s->data == NULL) ImageError((Widget) wid, ERR_MEM);

  INIT24

  lpl_s = new_width_s * 4;
  for(y = 0; y < new_height_s; y++) {
    r_btr =
      (ImagePixel*) ADDRESS(wid, resize_s ? -1 : xstart_s, ystart_s+y, rx);
    g_btr =
      (ImagePixel*) ADDRESS(wid, resize_s ? -1 : xstart_s, ystart_s+y, gx); 
    b_btr =
      (ImagePixel*) ADDRESS(wid, resize_s ? -1 : xstart_s, ystart_s+y, bx);

    if (obtr_s == r_btr) fptr_s = optr_s; else fptr_s = NULL; 
    obtr_s = r_btr;

    optr_s = ximg_ptr_s = (unsigned char *)
      &ximg_s->data[(ystart_s+y)*bytes_per_line_s+xstart_s*4]; 
    xptr_s = &xarr_s[xstart_s];

    if (resize_s == 0) {
      for(x = 0; x < new_width_s; x++) {
	EQ_24(RESIZE_NO(b_btr));
	EQ_24(RESIZE_NO(g_btr));
	EQ_24(RESIZE_NO(r_btr));
	*ximg_ptr_s++ = 0;
      }
    } else {
      if (fptr_s != NULL) {
	for (x=0; x < lpl_s; x++) *ximg_ptr_s++ = *fptr_s++;
      } else {
        for(x = 0; x < new_width_s; x++, xptr_s++) {
	  EQ_24(RESIZE_YES(b_btr));
	  EQ_24(RESIZE_YES(g_btr));
	  EQ_24(RESIZE_YES(r_btr));
	  *ximg_ptr_s++ = 0;
	}
      }
    }
  }

  LEAVE_FUNCTION_DEBUG("ImageProtoPix.c: Resize24LSB");
}



#ifdef IMAGE_OVERLAY
  static void Resize21LSB(ImageOverlayWidget wid, long rmask, long gmask, long bmask)
#else
  static void Resize21LSB(ImageWidget wid, long rmask, long gmask, long bmask)
#endif
{
  ImagePixel *r_btr, *g_btr, *b_btr;
#if !defined(BYTEPIX) && !defined(IMAGE_OVERLAY) && !defined(COMPLEXPIX) && !defined(DOUBLECOMPLEXPIX)
  double f0, f1, f2;
#endif
  int rx=0, gx=0, bx=0;
  int x, y;

  ENTER_FUNCTION_DEBUG("ImageProtoPix.c: Resize21LSB");

  if (rmask == 0xff0000) rx = 1; else
  if (gmask == 0xff0000) gx = 1; else 
    bx = 1;
  if (rmask == 0xff00) rx = 2; else
  if (gmask == 0xff00) gx = 2; else 
    bx = 2;
  if (rmask == 0xff) rx = 3; else
  if (gmask == 0xff) gx = 3; else 
    bx = 3;

  /* 4 bytes per pixel is sufficient for 24 bits. */
  bytes_per_line_s = ximg_s -> bytes_per_line = ximg_width_s * 4;

  if (!ximg_s->data) ximg_s->data = malloc(bytes_per_line_s*ximg_height_s);
  if (ximg_s->data == NULL) ImageError((Widget) wid, ERR_MEM);

  INIT21

  lpl_s = new_width_s * 4;
  for(y = 0; y < new_height_s; y++) {
    r_btr =
      (ImagePixel*) ADDRESS(wid, resize_s ? -1 : xstart_s, ystart_s+y, rx);
    g_btr =
      (ImagePixel*) ADDRESS(wid, resize_s ? -1 : xstart_s, ystart_s+y, gx); 
    b_btr =
      (ImagePixel*) ADDRESS(wid, resize_s ? -1 : xstart_s, ystart_s+y, bx);

    if (obtr_s == r_btr) fptr_s = optr_s; else fptr_s = NULL; 
    obtr_s = r_btr;

    optr_s = ximg_ptr_s = (unsigned char *)
      &ximg_s->data[(ystart_s+y)*bytes_per_line_s+xstart_s*4]; 
    xptr_s = &xarr_s[xstart_s];

    if (resize_s == 0) {
      for(x = 0; x < new_width_s; x++) {
	EQ_24(RESIZE_NO(b_btr));
	EQ_24(RESIZE_NO(g_btr));
	EQ_24(RESIZE_NO(r_btr));
	*ximg_ptr_s++ = 0;
      }
    } else {
      if (fptr_s != NULL) {
	for (x=0; x < lpl_s; x++) *ximg_ptr_s++ = *fptr_s++;
      } else {
        for(x = 0; x < new_width_s; x++, xptr_s++) {
	  EQ_24(RESIZE_YES(b_btr));
	  EQ_24(RESIZE_YES(g_btr));
	  EQ_24(RESIZE_YES(r_btr));
	  *ximg_ptr_s++ = 0;
	}
      }
    }
  }

  LEAVE_FUNCTION_DEBUG("ImageProtoPix.c: Resize21LSB");
}



#ifdef IMAGE_OVERLAY
  static void Resize24MSB(ImageOverlayWidget wid, long rmask, long gmask, long bmask)
#else
  static void Resize24MSB(ImageWidget wid, long rmask, long gmask, long bmask)
#endif
{
  ImagePixel *r_btr, *g_btr, *b_btr;
#if !defined(BYTEPIX) && !defined(IMAGE_OVERLAY) && !defined(COMPLEXPIX) && !defined(DOUBLECOMPLEXPIX)
  double f0, f1, f2;
#endif
  int rx=0, gx=0, bx=0;
  int x, y;

  ENTER_FUNCTION_DEBUG("ImageProtoPix.c: Resize24MSB");

  if (rmask == 0xff0000) rx = 1; else
  if (gmask == 0xff0000) gx = 1; else 
      bx = 1;
  if (rmask == 0xff00) rx = 2; else
  if (gmask == 0xff00) gx = 2; else 
      bx = 2;
  if (rmask == 0xff) rx = 3; else
  if (gmask == 0xff) gx = 3; else 
      bx = 3;

  bytes_per_line_s = ximg_s -> bytes_per_line = ximg_width_s*4;

  if (!ximg_s->data) ximg_s->data = malloc(bytes_per_line_s*ximg_height_s);
  if (ximg_s->data == NULL) ImageError((Widget) wid, ERR_MEM);

  INIT24

  lpl_s = new_width_s * 4;
  for(y = 0; y < new_height_s; y++) {
    r_btr =
      (ImagePixel*) ADDRESS(wid, resize_s ? -1 : xstart_s, ystart_s+y, rx); 
    g_btr =
      (ImagePixel*) ADDRESS(wid, resize_s ? -1 : xstart_s, ystart_s+y, gx); 
    b_btr =
      (ImagePixel*) ADDRESS(wid, resize_s ? -1 : xstart_s, ystart_s+y, bx);

    if (obtr_s == r_btr) fptr_s = optr_s; else fptr_s = NULL; 
    obtr_s = r_btr;

    optr_s = ximg_ptr_s = (unsigned char *)
      &ximg_s->data[(ystart_s+y)*bytes_per_line_s+xstart_s*4]; 
    xptr_s = &xarr_s[xstart_s];

    if (resize_s == 0) {
      for(x = 0; x <new_width_s; x++) {
	*ximg_ptr_s++ = 0;
	EQ_24(RESIZE_NO(r_btr));
	EQ_24(RESIZE_NO(g_btr));
	EQ_24(RESIZE_NO(b_btr));
      }
    } else {
      if (fptr_s != NULL) {
	for (x=0; x<lpl_s; x++) *ximg_ptr_s++ = *fptr_s++;
      }
      else {
	for(x = 0; x <new_width_s; x++, xptr_s++) {
	  *ximg_ptr_s++ = 0;
	  EQ_24(RESIZE_YES(r_btr));
	  EQ_24(RESIZE_YES(g_btr));
	  EQ_24(RESIZE_YES(b_btr));
	}
      }
    }
  }

  LEAVE_FUNCTION_DEBUG("ImageProtoPix.c: Resize24MSB");
}



#ifdef IMAGE_OVERLAY
  static void Resize21MSB(ImageOverlayWidget wid, long rmask, long gmask, long bmask)
#else
  static void Resize21MSB(ImageWidget wid, long rmask, long gmask, long bmask)
#endif
{
  ImagePixel *r_btr, *g_btr, *b_btr;
#if !defined(BYTEPIX) && !defined(IMAGE_OVERLAY) && !defined(COMPLEXPIX) && !defined(DOUBLECOMPLEXPIX)
  double f0, f1, f2;
#endif
  int rx=0, gx=0, bx=0;
  int x, y;

  ENTER_FUNCTION_DEBUG("ImageProtoPix.c: Resize21MSB");

  if (rmask == 0xff0000) rx = 1; else
  if (gmask == 0xff0000) gx = 1; else 
      bx = 1;
  if (rmask == 0xff00) rx = 2; else
  if (gmask == 0xff00) gx = 2; else 
      bx = 2;
  if (rmask == 0xff) rx = 3; else
  if (gmask == 0xff) gx = 3; else 
      bx = 3;

  bytes_per_line_s = ximg_s -> bytes_per_line = ximg_width_s*4;

  if (!ximg_s->data) ximg_s->data = malloc(bytes_per_line_s*ximg_height_s);
  if (ximg_s->data == NULL) ImageError((Widget) wid, ERR_MEM);

  INIT21

  lpl_s = new_width_s * 4;
  for(y = 0; y < new_height_s; y++) {
    r_btr =
      (ImagePixel*) ADDRESS(wid, resize_s ? -1 : xstart_s, ystart_s+y, rx); 
    g_btr =
      (ImagePixel*) ADDRESS(wid, resize_s ? -1 : xstart_s, ystart_s+y, gx); 
    b_btr =
      (ImagePixel*) ADDRESS(wid, resize_s ? -1 : xstart_s, ystart_s+y, bx);

    if (obtr_s == r_btr) fptr_s = optr_s; else fptr_s = NULL; 
    obtr_s = r_btr;

    optr_s = ximg_ptr_s = (unsigned char *)
      &ximg_s->data[(ystart_s+y)*bytes_per_line_s+xstart_s*4]; 
    xptr_s = &xarr_s[xstart_s];

    if (resize_s == 0) {
      for(x = 0; x <new_width_s; x++) {
	*ximg_ptr_s++ = 0;
	EQ_24(RESIZE_NO(r_btr));
	EQ_24(RESIZE_NO(g_btr));
	EQ_24(RESIZE_NO(b_btr));
      }
    } else {
      if (fptr_s != NULL) {
	for (x=0; x<lpl_s; x++) *ximg_ptr_s++ = *fptr_s++;
      }
      else {
	for(x = 0; x <new_width_s; x++, xptr_s++) {
	  *ximg_ptr_s++ = 0;
	  EQ_24(RESIZE_YES(r_btr));
	  EQ_24(RESIZE_YES(g_btr));
	  EQ_24(RESIZE_YES(b_btr));
	}
      }
    }
  }

  LEAVE_FUNCTION_DEBUG("ImageProtoPix.c: Resize21MSB");
}



#ifdef IMAGE_OVERLAY
  static void Resize8(ImageOverlayWidget wid)
#else
  static void Resize8(ImageWidget wid)
#endif
{
#if !defined(BYTEPIX) && !defined(IMAGE_OVERLAY) && !defined(COMPLEXPIX) && !defined(DOUBLECOMPLEXPIX)
  double f0, f1, f2;
#endif
  int x, y;

  ENTER_FUNCTION_DEBUG("ImageProtoPix.c: Resize8");

  bytes_per_line_s = ximg_s -> bytes_per_line = ximg_width_s;

  if (!ximg_s->data) ximg_s->data = malloc(bytes_per_line_s*ximg_height_s);
  if (ximg_s->data == NULL) ImageError((Widget) wid, ERR_MEM);

  INIT8

  lpl_s = new_width_s;
  for(y = 0; y < new_height_s; y++) {
    btr_s = 
      (ImagePixel*) ADDRESS(wid, resize_s ? -1 : xstart_s, ystart_s+y, 0); 

    if (obtr_s == btr_s) fptr_s = optr_s; else fptr_s = NULL; 
    obtr_s = btr_s;
    optr_s = ximg_ptr_s =
      (unsigned char *) &ximg_s->data[(ystart_s+y)*bytes_per_line_s+xstart_s];
    xptr_s = &xarr_s[xstart_s];
    if (resize_s == 0) {
      for(x = 0; x < new_width_s; x++) EQ_8(RESIZE_NO(btr_s));
    } else { 
      if (fptr_s != NULL) {
	for (x=0; x < lpl_s; x++) *ximg_ptr_s++ = *fptr_s++;
      } else {
	for(x = 0; x < new_width_s; x++, xptr_s++) EQ_8(RESIZE_YES(btr_s));
      }
    }
  }

  LEAVE_FUNCTION_DEBUG("ImageProtoPix.c: Resize8");
}




#ifdef IMAGE_OVERLAY
  static void Resize7(ImageOverlayWidget wid)
#else
  static void Resize7(ImageWidget wid)
#endif
{
#if !defined(BYTEPIX) && !defined(IMAGE_OVERLAY) && !defined(COMPLEXPIX) && !defined(DOUBLECOMPLEXPIX)
  double f0, f1, f2;
#endif
  int x, y;

  ENTER_FUNCTION_DEBUG("ImageProtoPix.c: Resize7");

  bytes_per_line_s = ximg_s -> bytes_per_line = ximg_width_s;
  if (!ximg_s->data) ximg_s->data = malloc(bytes_per_line_s * ximg_height_s);
  if (ximg_s->data == NULL) ImageError((Widget) wid, ERR_MEM);

  INIT7

  lpl_s = new_width_s;
  for(y = 0; y < new_height_s; y++) {
    btr_s =
      (ImagePixel*) ADDRESS(wid, resize_s ? -1 : xstart_s, ystart_s+y, 0);

    if (obtr_s == btr_s) fptr_s = optr_s; else fptr_s = NULL;
    obtr_s = btr_s;
    optr_s = ximg_ptr_s =
      (unsigned char *) &ximg_s->data[(ystart_s+y)*bytes_per_line_s+xstart_s];
    xptr_s = &xarr_s[xstart_s];
    if (resize_s == 0) {
      for(x = 0; x < new_width_s; x++)
	EQ_8(RESIZE_NO(btr_s));
    } else {
      if (fptr_s != NULL) {
	for (x=0; x < lpl_s; x++) *ximg_ptr_s++ = *fptr_s++;
      } else {
	for(x = 0; x < new_width_s; x++, xptr_s++)
	  EQ_8(RESIZE_YES(btr_s));
      }
    }
  }

  LEAVE_FUNCTION_DEBUG("ImageProtoPix.c: Resize7");
}



#ifdef IMAGE_OVERLAY
  static void Resize4(ImageOverlayWidget wid)
#else
  static void Resize4(ImageWidget wid)
#endif
{
#if !defined(BYTEPIX) && !defined(IMAGE_OVERLAY) && !defined(COMPLEXPIX) && !defined(DOUBLECOMPLEXPIX)
  double f0, f1, f2;
#endif
  int x, y;

  ENTER_FUNCTION_DEBUG("ImageProtoPix.c: Resize4");

  bytes_per_line_s = ximg_s -> bytes_per_line = (ximg_width_s+1)/2;
  if (!ximg_s->data) ximg_s->data = malloc(bytes_per_line_s*ximg_height_s);
  if (ximg_s->data == NULL) ImageError((Widget) wid, ERR_MEM);
  ximg_s->byte_order = MSBFirst;

  INIT4

  if (xstart_s & 1) { xstart_s--; new_width_s++; }
  if (new_width_s & 1) if (xstart_s + new_width_s < ximg_width_s) new_width_s++; 

  lpl_s = (new_width_s+1)/2;    
  for(y = 0; y < new_height_s; y++) {
    btr_s = 
      (ImagePixel*) ADDRESS(wid, resize_s ? -1 : xstart_s, ystart_s+y, 0); 

    if (obtr_s == btr_s) fptr_s = optr_s; else fptr_s = NULL; 
    obtr_s = btr_s;
    optr_s = ximg_ptr_s =
      (unsigned char *) &ximg_s->data[(ystart_s+y)* bytes_per_line_s +xstart_s/2];
    xptr_s = &xarr_s[xstart_s];
    if (resize_s == 0) {
      for(x = 0; x <new_width_s; x++, xptr_s++) {
	if (x & 1) {
	  EQ_4A(RESIZE_NO(btr_s));
	} else {
	  EQ_4B(RESIZE_NO(btr_s));
	}
      }
    } else {
      if (fptr_s != NULL) {
	for (x=0; x<lpl_s; x++) *ximg_ptr_s++ = *fptr_s++;
      } else {
	for(x = 0; x <new_width_s; x++, xptr_s++) {
	  if (x & 1) {
	    EQ_4A(RESIZE_YES(btr_s));
	  }
	  else {
	    EQ_4B(RESIZE_YES(btr_s));
	  }
	}
      }
    }
    if (new_width_s & 1) ximg_ptr_s++;
  }

  LEAVE_FUNCTION_DEBUG("ImageProtoPix.c: Resize4");
}	
  
  


#ifdef IMAGE_OVERLAY
  static void ResizeMSB(ImageOverlayWidget wid)
#else
  static void ResizeMSB(ImageWidget wid)
#endif
{
#if !defined(BYTEPIX) && !defined(IMAGE_OVERLAY) && !defined(COMPLEXPIX) && !defined(DOUBLECOMPLEXPIX)
  double f1, f2;
# if !defined(SIGNED_BYTEPIX) && !defined(SHORTPIX) && !defined(UNSIGNED_SHORT_PIX) && !defined(LONGPIX) && !defined(FLOATPIX) && !defined(DOUBLEPIX)
    double f0;
# endif
#endif

  int count;
  int x, y;

  ENTER_FUNCTION_DEBUG("ImageProtoPix.c: ResizeMSB");

  bytes_per_line_s = ximg_s -> bytes_per_line = (ximg_width_s+7)/8;
  if (!ximg_s->data) ximg_s->data = malloc(bytes_per_line_s*ximg_height_s);
  if (ximg_s->data == NULL) ImageError((Widget) wid, ERR_MEM);

  INIT2

  count = xstart_s & 7;
  if (count) {
    xstart_s -= count;
    new_width_s += count;
  }    

  if (new_width_s & 7) new_width_s = new_width_s - (new_width_s & 7) + 8;
  if (xstart_s + new_width_s > ximg_width_s)
    new_width_s = ximg_width_s-xstart_s; 

  lpl_s = (new_width_s+7)/8;
  for(y = 0; y < new_height_s; y++) {
    btr_s = 
      (ImagePixel*) ADDRESS(wid, resize_s ? -1 : xstart_s, ystart_s+y, 0); 

    if (obtr_s == btr_s) fptr_s = optr_s; else fptr_s = NULL; 
    obtr_s = btr_s;
    optr_s = ximg_ptr_s =
      (unsigned char *) &ximg_s->data[(ystart_s+y)* bytes_per_line_s
				     + xstart_s/8];
    xptr_s = &xarr_s[xstart_s];
    count = 8;
    pix_s = 0;
    if (resize_s == 0) {
      for(x = 0; x <new_width_s; x++) {
	pix_s <<= 1;
	if (EQ_2A(RESIZE_NO(btr_s))) pix_s++; 
	if (--count == 0) {
	  *ximg_ptr_s++ EQ_2B pix_s;
	  count = 8;
	  pix_s = 0;
	}
      }
    } else if (fptr_s != NULL) {
      for (x=0; x<lpl_s; x++) *ximg_ptr_s++ = *fptr_s++;
    } else {
      for(x = 0; x <new_width_s; x++, xptr_s++) {
	pix_s <<= 1;
	if (EQ_2A(RESIZE_YES(btr_s))) pix_s++;
	if (--count == 0) {
	  *ximg_ptr_s++ EQ_2B pix_s;
	  count = 8;
	  pix_s = 0;
	}
      }
    }
    if (count < 8) {
      pix_s <<= count;
      *ximg_ptr_s++ EQ_2B pix_s;
    }
  }

  LEAVE_FUNCTION_DEBUG("ImageProtoPix.c: ResizeMSB");
}




#ifdef IMAGE_OVERLAY
  static void ResizeLSB(ImageOverlayWidget wid)
#else
  static void ResizeLSB(ImageWidget wid)
#endif
{
  int count;
  int x, y;
#if !defined(BYTEPIX) && !defined(IMAGE_OVERLAY) && !defined(COMPLEXPIX) && !defined(DOUBLECOMPLEXPIX)
  double f1, f2;
# if !defined(SIGNED_BYTEPIX) && !defined(SHORTPIX) && !defined(UNSIGNED_SHORT_PIX) && !defined(LONGPIX) && !defined(FLOATPIX) && !defined(DOUBLEPIX)
    double f0;
# endif
#endif

  ENTER_FUNCTION_DEBUG("ImageProtoPix.c: ResizeLSB");

  bytes_per_line_s = ximg_s -> bytes_per_line = (ximg_width_s+7)/8;
  if (!ximg_s->data) ximg_s->data = malloc(bytes_per_line_s*ximg_height_s);
  if (ximg_s->data == NULL) ImageError((Widget) wid, ERR_MEM);

  INIT2

  count = xstart_s & 7;
  if (count) 
    {
      xstart_s -= count;
      new_width_s += count;
    }    
  if (new_width_s & 7) new_width_s = new_width_s - (new_width_s & 7) + 8;
  if (xstart_s + new_width_s > ximg_width_s)
    new_width_s = ximg_width_s-xstart_s; 

  lpl_s = (new_width_s+7)/8;
  for(y = 0; y < new_height_s; y++) {
    btr_s =
      (ImagePixel*) ADDRESS(wid, resize_s ? -1 : xstart_s, ystart_s+y, 0);

    if (obtr_s == btr_s) fptr_s = optr_s; else fptr_s = NULL; 
    obtr_s = btr_s;
    optr_s = ximg_ptr_s =
      (unsigned char *) &ximg_s->data[(ystart_s+y)* bytes_per_line_s
				     + xstart_s/8];
    xptr_s = &xarr_s[xstart_s];
    count = 8;
    pix_s = 0;
    if (resize_s == 0) {
      for(x = 0; x <new_width_s; x++) {
	pix_s >>= 1;
	if (EQ_2A(RESIZE_NO(btr_s))) pix_s |= 0x80;
	if (--count == 0) {
	  *ximg_ptr_s++ EQ_2B pix_s;
	  count = 8;
	  pix_s = 0;
	}
      }
    } else if (fptr_s != NULL) {
      for (x=0; x<lpl_s; x++) *ximg_ptr_s++ = *fptr_s++;
    } else {
      for(x = 0; x <new_width_s; x++, xptr_s++) {
	pix_s >>= 1;
	if (EQ_2A(RESIZE_YES(btr_s))) pix_s |= 0x80;
	if (--count == 0) {
	  *ximg_ptr_s++ EQ_2B pix_s;
	  count = 8;
	  pix_s = 0;
	}
      }
    }
    if (count < 8) {
      pix_s <<= count;
      *ximg_ptr_s++ EQ_2B pix_s;
    }
  }

  LEAVE_FUNCTION_DEBUG("ImageProtoPix.c: ResizeLSB");
}



/*F:ImageByteResize=ImageResize*/
/*F:ImageOverlayResize=ImageResize*/
/*F:ImageSignedByteResize=ImageResize*/
/*F:ImageUnsignedShortResize=ImageResize*/
/*F:ImageShortResize=ImageResize*/
/*F:ImageLongResize=ImageResize*/
/*F:ImageFloatResize=ImageResize*/
/*F:ImageDoubleResize=ImageResize*/
/*F:ImageResize*

________________________________________________________________

		ImageResize
________________________________________________________________

Name:		ImageResize, ImageByteResize, ImageOverlayResize,
                ImageSignedByteResize, ImageUnsignedShortResize,
		ImageShortResize, ImageLongResize, ImageFloatResize,
		ImageDoubleResize - Resize XITE Image (pixeltype-specific
		routines)

Syntax:		| #include "imagePix.h"
                |
                | void ImageXxxxResize(ImageWidget wid, int x, int y,
                |    int width, int height);
		| void ImageOverlayResize(ImageWidget wid, int x,
		|    int y, int width, int height);

Description:	These functions resize the image of a particular pixeltype.
                They are called by the resize method (and thus also by the
		set_values method) of the Image widget supplied with the XITE
		distribution.

                Images with three-primary colormaps are also handled
		(DirectColor and TrueColor 24-bit visuals).

		Byte-order for pixmaps and bitmaps is defined by the X
		server, not the client. These functions handle swapping
		of the byte-order for 24-bit DirectCololor, 24-bit
		TrueColor and 1-bit StaticGray visuals. For 8- and 4-bit
		visual depths, no byte-swapping is necessary. The swapping
		could have been done by XPutImage, if necessary, but
		doing it in these functions requires no extra work.

                One of these functions (depending on the pixeltype of the
                band at hand) should be called when any one of the following
		has changed:

		&- Widget instance variable scaletype
		This determines linear lookup in colormap or thresholding

		&- Widget instance variables scalea or scaleb
		These determine offset and inclination of linearity if
		scaletype implies linear lookup

		&- Widget instance variables minval or maxval
		These determine the range used in linear lookup

		&- Widget instance variable colorcells
		If less than 256, colors will be repeated cyclically
		because lookup indices will repeat cyclically

		&- Widget instance variable colorptr
		Determines lookup indices

		&- Widget instance variable displaymethod
		When switching between 'reduced color' display and full-color
		display.

		&- Widget instance variables xptr or yptr
		These tables determine transformation from image coordinates to
		screen window coordinates in the event that the image is
		zoomed or panned

		&- Widget instance variable resize
		This is zero if no geometric transformation is necessary

		&- Widget instance variables xpan, ypan, zoom
		These are determined by zoom and pan (and influence the
		geometric transformation tables)

		&- Overlay widget instance variables cells and colorptr
		Determines lookup indices into colormap

See also:       Image(3), ImageOverlay(3), ImageFormat(3)

Author:		Otto Milvang
Revised:        Svein Bøe
Doc:            Svein Bøe
Id: 		$Id$
________________________________________________________________

*/

#ifdef IMAGE_OVERLAY
  void ImageXxxxResize(ImageOverlayWidget wid, int x, int y, int width, int height)
#else
  void ImageXxxxResize(ImageWidget wid, int x, int y, int width, int height)
#endif
{
  XVisualInfo vip, *vir;
  int nv, rmask, gmask, bmask;

  ENTER_FUNCTION_DEBUG("ImageProtoPix.c: ImageXxxxResize");

  xstart_s      = x;
  ystart_s      = y;
  new_width_s   = width;
  new_height_s  = height; 
  ip_s          = &(wid->image);
  ximg_s        = ip_s->ximg;
  ximg_width_s  = ximg_s->width;
  ximg_height_s = ximg_s->height;

  if (ximg_width_s == 0 || ximg_height_s == 0) {
    LEAVE_FUNCTION_DEBUG("ImageProtoPix.c: ImageXxxxResize");
    return;
  }

  resize_s      = ip_s->resize;
  xarr_s        = ip_s->xptr;
  scalea_s      = ip_s->scalea;
  scaleb_s      = ip_s->scaleb;
  minval_s      = ip_s->minval;
  maxval_s      = ip_s->maxval;
  optr_s        = NULL;
  obtr_s        = NULL;

  /* Repeat colors cyclically if less than 256. Otherwise, initialize. */
  cells_s = ip_s->colorcells;  
  if (ip_s->colorptr)
    for(i=0; i<256; i++) trans_s[i] = ip_s->colorptr[i % cells_s];
  else
    for(i=0; i<256; i++) trans_s[i] = i;

  FPRINTF3("    xstart_s: %4d, ystart_s: %4d", xstart_s, ystart_s);
  FPRINTF3(", new_width_s: %4d, new_height_s: %4d\n", new_width_s, new_height_s);
  FPRINTF2("    ximg_width_s: %d", ximg_width_s);
  FPRINTF2(", ximg_height_s: %d\n", ximg_height_s);

  switch(ip_s->displaymethod)
    {
    case TRUE_COLOR_24:
    case DIRECT_COLOR_24_A:
      vip.visualid = (VisualID) XVisualIDFromVisual(ip_s->visual);
      vir = XGetVisualInfo(XtDisplay(wid), VisualIDMask, &vip, &nv);
      rmask = RedMaskOfVisualInfo(*vir);
      gmask = GreenMaskOfVisualInfo(*vir);
      bmask = BlueMaskOfVisualInfo(*vir);
      XFree((char *)vir);
      FPRINTF3("    Visual masks (%d), red = %08x, ", nv, rmask);
      FPRINTF3("green = %08x, blue = %08x\n", gmask, bmask);
      if ( ximg_s->byte_order == LSBFirst &&
	  ((rmask|gmask|bmask) == 0xffffff) )
	  Resize21LSB(wid, rmask, gmask, bmask);
      else Resize21MSB(wid, rmask, gmask, bmask);
      break;
    case DIRECT_COLOR_24_B:
      vip.visualid = (VisualID) XVisualIDFromVisual(ip_s->visual);
      vir = XGetVisualInfo(XtDisplay(wid), VisualIDMask, &vip, &nv);
      rmask = RedMaskOfVisualInfo(*vir);
      gmask = GreenMaskOfVisualInfo(*vir);
      bmask = BlueMaskOfVisualInfo(*vir);
      XFree((char *)vir);
      FPRINTF3("    Visual masks (%d), red = %08x, ", nv, rmask);
      FPRINTF3("green = %08x, blue = %08x\n", gmask, bmask);
      if ( ximg_s->byte_order == LSBFirst &&
	  ((rmask|gmask|bmask) == 0xffffff) )
	  Resize24LSB(wid, rmask, gmask, bmask);
      else Resize24MSB(wid, rmask, gmask, bmask);
      break;
    case STATIC_GRAY_8:
    case PSEUDO_COLOR_8_B:
      Resize8(wid);
      break;
    case GRAY_SCALE_8:
    case PSEUDO_COLOR_8_A:
      Resize7(wid);
      break;
    case STATIC_GRAY_4:
    case GRAY_SCALE_4:
      Resize4(wid);
      break;
    case STATIC_GRAY_1:
      if (ximg_s->bitmap_bit_order == LSBFirst) ResizeLSB(wid);
      else ResizeMSB(wid);
      break;
    default:
      Resize8(wid);
      break;

    }

  LEAVE_FUNCTION_DEBUG("ImageProtoPix.c: ImageXxxxResize");
}
