

/*

________________________________________________________________

        ImageOverlayResize.c
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

#define IMAGE_OVERLAY

#include <xite/includes.h>
#include <xite/strings.h>
#include "ImageOverlayPix.h"
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif

static unsigned char btrans_s[256];

static ipixel overlaypixel =
{
  /* pixelname   */ ImageOverlayPixel,
  /* printformat */ "%-3d",
  /* pixelsize   */ 1,
  /* resize      */ ImageOverlayResize,
  /* printf      */ (imageprintffunc) ImageOverlayPrintf,
  /* txtlen      */ 0,
  /* scaletype   */ SCALE_NONE,
  /* scalea      */ 0.0,
  /* scaleb      */ 0.0,
  /* minval      */ 0.0,
  /* maxval      */ UNS_BYTE_MAX,
  /* next        */ NULL,
};

ipixelptr OverlayPixel(void)
{
  return(&overlaypixel);
}

#define RESIZE_NO(a) *a++
#define RESIZE_YES(a) a[*xptr_s]
#define EQ_PRO(left, OP, right) left OP btrans_s[right]
#define EQ_24(a) \
  { j = a; if (j) *ximg_ptr_s++ = btrans_s[j]; else ximg_ptr_s++; }
#define EQ_8(a) \
  { j = a; if (j) *ximg_ptr_s++ = btrans_s[j]; else ximg_ptr_s++; }
#define EQ_4A(a) \
  { j = a; if (j){k = (*ximg_ptr_s & 0xf) << 4; *ximg_ptr_s &= 0xf0;\
				   *ximg_ptr_s += btrans_s[k];} ximg_ptr_s++;}
#define EQ_4B(a) \
  { j = a; if (j){k = *ximg_ptr_s; *ximg_ptr_s &= 0x0f;\
				   *ximg_ptr_s += (btrans_s[k] << 4);}}
#define EQ_2A(a) (a)
#define EQ_2B    ^= 


#define INIT24\
  for(i=0; i<256; i++) btrans_s[i] = i >=128 ? i-128 : i+128;
#define INIT21\
  cells_s = wid->image_overlay.cells;\
  if (wid->image_overlay.colorptr)\
    for(i=0; i<256; i++) btrans_s[i] = \
      wid->image_overlay.colorptr[i % cells_s];\
  else\
    for(i=0; i<256; i++) btrans_s[i] = (i >=128) ? (i-128) : (i+128);
#define INIT8\
  for(i=0; i<256; i++) btrans_s[i] = (i * 147) & 0xff;

/* Example for INIT7:
 * cells_s = 32 and colorptr[0..31] = [192..223]
 * means that [0..255] % cells_s = [0..31]
 * and btrans_s[0..255] = [192..223, 192..223, 192..223, 192..223, ...].
 */
#define INIT7\
  cells_s = wid->image_overlay.cells;\
  if (wid->image_overlay.colorptr)\
    for(i=0; i<256; i++) btrans_s[i] = \
      wid->image_overlay.colorptr[i % cells_s];\
  else\
    for(i=0; i<256; i++) btrans_s[i] = (i >=128) ? (i-128) : (i+128);
#define INIT4\
  for(i=0; i<256; i++) btrans_s[i] = i >=128 ? ((i-128) >> 4) : ((i+128) >> 4);
#define INIT2

char *ImageOverlayPrintf(char *buf, ImageBytePix *pix_value)
{
  char number[30];

  sprintf(number, overlaypixel.printformat, *pix_value);
  (void) center_text(buf, number, overlaypixel.txtlen);

  return(buf);

} /* ImageOverlayPrintf() */

typedef ImageBytePix ImagePixel; 

#define ImageXxxxResize ImageOverlayResize
#define ADDRESS(wid, a, b, c) \
        ImageOverlayScreenXYtoAdr((ImageOverlayWidget) (wid), a, b, c)

#include "ImageProtoPix.c"
