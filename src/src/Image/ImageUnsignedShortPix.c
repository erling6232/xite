/*

________________________________________________________________

        ImageUnsignedShortResize.c
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
#include <xite/strings.h>
#include "ImageUnsignedShortPix.h"
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif

static ipixel unsignedshortpixel =
{
  /* pixelname   */ ImageUnsignedShortPixel,
  /* printformat */ NULL,
  /* pixelsize   */ 2,
  /* resize      */ ImageUnsignedShortResize,
  /* printf      */ (imageprintffunc) ImageUnsignedShortPrintf,
  /* txtlen      */ 5,
  /* scaletype   */ SCALE_LINEAR,
  /* scalea      */ 1.0,
  /* scaleb      */ 0.0,
  /* minval      */ 0.0,
  /* maxval      */ UNS_SHORT_MAX,
  /* next        */ NULL,
};

ipixelptr UnsignedShortPixel(void)
{
  return(&unsignedshortpixel);
}

char *ImageUnsignedShortPrintf(char *buf, ImageUnsignedShortPix *pix_value)
{
  char number[30];

  sprintf(number, unsignedshortpixel.printformat, *pix_value);
  (void) center_text(buf, number, unsignedshortpixel.txtlen);

  return(buf);

} /* ImageUnsignedShortPrintf() */

typedef ImageUnsignedShortPix ImagePixel; 

#define UNSIGNED_SHORT_PIX 1
#define ImageXxxxResize ImageUnsignedShortResize

#include "ImageProtoPix.c"
