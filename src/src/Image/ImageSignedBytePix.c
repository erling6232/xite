

/*

________________________________________________________________

        ImageSignedBytePix.c
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




/*L:ImageSignedBytePix.c*

________________________________________________________________

		ImageSignedBytePix.c
________________________________________________________________

Name:		ImageSignedBytePix.c
Description:	This module contains definitions for initialization of a
                look-up table to handle linear scaling and thresholding of
		signed byte pixel values. There are also definitions which
		use the look-up table for such transformations, including
		possible geometric transformation because of resizing of
		X image.
See also:	ImageProtoPix.c
Restrictions:	Intended for signed byte pixel values only.
Author:		Otto Milvang
Doc:            Svein Bøe
Id: 		$Id$
________________________________________________________________

*/

#include <xite/includes.h>
#include <xite/strings.h>
#include "ImageSignedBytePix.h"
#include XITE_STDIO_H

/* 'btrans_s' is a look-up table for scaling and tresholding of incoming byte
   pixel values.

   This array will transform an incoming byte pixel-value. The transformation
   look-up table is based on linear scaling or thresholding, and is calculated
   once only. Since this file is used only for signed byte-pixels, there is
   no need for more than 256 elements. */

/* static unsigned char btrans_s[256]; */

/*

#define RESIZE_NO(a) *a++
#define RESIZE_YES(a) a[*xptr_s]
#define EQ_PRO(left, OP, right) left OP btrans_s[right]
#define EQ_24(a) EQ_PRO(*ximg_ptr_s++, =, a)
#define EQ_8(a)  EQ_PRO(*ximg_ptr_s++, =, a)
#define EQ_4A(a) EQ_PRO(*ximg_ptr_s++, +=, a)
#define EQ_4B(a) EQ_PRO(j, =, a); *ximg_ptr_s = j << 4
#define EQ_2A(a) (btrans_s[a])  
#define EQ_2B    = 
*/

/* SCALE_THRESHOLD is used when thresholding during histogram manipulation.
   'scalea_s' represents threshold level. Initialize 'btrans_s' so that
   each element is either minimum (0 or trans_s[0]) or maximum
   (255, 191, 15 or trans_s[cells_s-1]), depending on whether the element
   number is below or above the threshold. */


/* The following macro is used by the functions Resize24?SB.

   For tresholding: Initialize 'btrans_s' so that each element is either
   minimum (0) or maximum (255), depending on whether the element number is
   below or above the threshold.

   Otherwise: Initialize 'btrans_s' so that each element is the scaling,
   relative to the band pixel interval, into [0, 256] of the corresponding
   element number. Clip outside [0, 255]. */

/*
#define INIT24\
  if (wid->image.scaletype == SCALE_THRESHOLD) { \
      for(i=0; i<256; i++) btrans_s[i] = (scalea_s <= i) ?  255 : 0; \
  } else { \
    for(i=0; i<256; i++) { \
      j = (scalea_s * i + scaleb_s-minval_s)*256.0/(maxval_s-minval_s) + 0.5;\
      if (j>255) j = 255;\
      if (j<0)   j = 0;\
      btrans_s[i] = j;\
    } \
  }
*/

/* The following macro is used by the function Resize21?SB.

   For thresholding: Initialize 'trans_s' so that each element is either
   minimum (64 or 'trans_s[0]') or maximum (191 or 'trans_s[cells_s-1]'),
   depending on whether the element number is below or above the threshold.

   Otherwise: Initialize 'btrans_s' so that each element is the scaling,
   relative to the band pixel interval, into [0, cells_s] of the corresponding
   element number. Clip outside [0, cells_s-1]. */

/*
#define INIT21\
  if (wid->image.scaletype == SCALE_THRESHOLD) { \
    for(i=0; i<256; i++) { \
      btrans_s[i] = ((scalea_s <= i) ? \
        (cells_s ? trans_s[cells_s-1] : 191) : (cells_s ? trans_s[0] : 64)); \
    } \
  } else { \
    for(i=0; i<256; i++) { \
      j = (scalea_s * i + scaleb_s-minval_s) \
			     * cells_s/(maxval_s-minval_s) + 0.5;\
      if (j>=cells_s) j = cells_s-1;\
      if (j<0)   j = 0;\
      btrans_s[i] = trans_s[j];\
    } \
  }
*/

/* The following macro is used by the function Resize8.

   For thresholding: Initialize 'btrans_s' so that each element is either
   minimum (0 or 'trans_s[0]') or maximum (255 or 'trans_s[cells_s-1]'),
   depending on whether the element number is below or above the threshold.

   Otherwise: Initialize 'btrans_s' so that each element is the scaling,
   relative to the band pixel interval, into [0, 256] of the corresponding
   element number. Clip outside [0, 255]. */
/*
#define INIT8\
  if (wid->image.scaletype == SCALE_THRESHOLD) { \
    for(i=0; i<256; i++) { \
      btrans_s[i] = ((scalea_s <= i) ? \
        (cells_s ? trans_s[cells_s-1] : 255) : (cells_s ? trans_s[0] : 0)); \
    } \
  } else { \
    for(i=0; i<256; i++) { \
      j = (scalea_s * i + scaleb_s-minval_s)*256.0/(maxval_s-minval_s) + 0.5;\
      if (j>255) j = 255;\
      if (j<0)   j = 0;\
      btrans_s[i] = trans_s[j];\
    } \
  }
*/
/* The following macro is used by the function Resize7.

   For thresholding: Initialize 'trans_s' so that each element is either
   minimum (64 or 'trans_s[0]') or maximum (191 or 'trans_s[cells_s-1]'),
   depending on whether the element number is below or above the threshold.

   Otherwise: Initialize 'btrans_s' so that each element is the scaling,
   relative to the band pixel interval, into [0, cells_s] of the corresponding
   element number. Clip outside [0, cells_s-1]. */
/*
#define INIT7\
  if (wid->image.scaletype == SCALE_THRESHOLD) { \
    for(i=0; i<256; i++) { \
      btrans_s[i] = ((scalea_s <= i) ? \
        (cells_s ? trans_s[cells_s-1] : 191) : (cells_s ? trans_s[0] : 64)); \
    } \
  } else { \
    for(i=0; i<256; i++) { \
      j = 1.0*(scalea_s * i + scaleb_s-minval_s) \
			     * cells_s/(maxval_s-minval_s) + 0.5;\
      if (j>=cells_s) j = cells_s-1;\
      if (j<0)   j = 0;\
      btrans_s[i] = trans_s[j];\
    } \
  }
*/
/* The following macro is used by the function Resize4.

   For thresholding: Initialize 'btrans_s' so that each element is either
   minimum (0) or maximum (15), depending on whether the element number is
   below or above the threshold.

   Otherwise: Initialize 'btrans_s' so that each element is the scaling,
   relative to the band pixel interval, into [0, 256] of the corresponding
   element number. Clip outside [0, 255] and divide by 16. The result is an
   element which is scaled in [0, 15]. */
/*
#define INIT4\
  if (wid->image.scaletype == SCALE_THRESHOLD) { \
    for(i=0; i<256; i++) btrans_s[i] = (scalea_s <= i) ?  15 : 0; \
  } else { \
    for(i=0; i<256; i++) { \
      j = (scalea_s * i + scaleb_s-minval_s)*256.0/(maxval_s-minval_s) + 0.5;\
      if (j>255) j = 255;\
      if (j<0)   j = 0;\
      btrans_s[i] = (j>>4);\
    } \
  }
*/
/* The following macro is used by the functions Resize?SB.

   For thresholding: Initialize 'btrans_s' so that each element is either
   minimum (0) or maximum (255), depending on whether the element number is
   below or above the threshold.

   Otherwise: Initialize 'btrans_s' so that each element is either 0 or 255,
   depending on whether the scaled, corresponding element number is above or
   below the midpoint of the band pixel interval. */
/*
#define INIT2\
  if (wid->image.scaletype == SCALE_THRESHOLD) { \
    for(i=0; i<256; i++) btrans_s[i] = (scalea_s <= i) ?  255 : 0; \
  } else { \
    for(i=0; i<256; i++) { \
      j = (scalea_s * i + scaleb_s) > (maxval_s+minval_s)/2 ? 255:0;\
      btrans_s[i] = j;\
    } \
  }
*/

static ipixel signedbytepixel =
{
  /* pixelname   */ ImageSignedBytePixel,
  /* printformat */ NULL,
  /* pixelsize   */ 1,
  /* resize      */ ImageSignedByteResize,
  /* printf      */ (imageprintffunc) ImageSignedBytePrintf,
  /* txtlen      */ 4,
  /* scaletype   */ SCALE_LINEAR,
  /* scalea      */ 1.0,
  /* scaleb      */ 0.0,
  /* minval      */ SIGN_BYTE_MIN,
  /* maxval      */ SIGN_BYTE_MAX,
  /* next        */ NULL,
};

#ifndef FUNCPROTO
ipixelptr SignedBytePixel()
#else /* FUNCPROTO */
ipixelptr SignedBytePixel(void)
#endif /* FUNCPROTO */
{
  return(&signedbytepixel);
}

#ifndef FUNCPROTO
char *ImageSignedBytePrintf(buf, pix_value)
char *buf;
ImageSignedBytePix *pix_value;
#else /* FUNCPROTO */
char *ImageSignedBytePrintf(char *buf, ImageSignedBytePix *pix_value)
#endif /* FUNCPROTO */
{
  char number[30];

  sprintf(number, signedbytepixel.printformat, *pix_value);
  (void) center_text(buf, number, signedbytepixel.txtlen);

  return(buf);

} /* ImageSignedBytePrintf() */

typedef ImageSignedBytePix ImagePixel;

#define SIGNED_BYTEPIX 1
#define ImageXxxxResize ImageSignedByteResize

#include "ImageProtoPix.c"
