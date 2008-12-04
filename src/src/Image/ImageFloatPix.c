/*

________________________________________________________________

        ImageFloatResize.c
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
#include "ImageFloatPix.h"
#include XITE_STDIO_H

static ipixel floatpixel =
{
  /* pixelname   */ ImageFloatPixel,
  /* printformat */ NULL,
  /* pixelsize   */ 4,
  /* resize      */ ImageFloatResize,
  /* printf      */ (imageprintffunc) ImageFloatPrintf,
  /* txtlen      */ 9,
  /* scaletype   */ SCALE_LINEAR,
  /* scalea      */ 1.0,
  /* scaleb      */ 0.0,
  /* minval      */ REAL_MIN,
  /* maxval      */ REAL_MAX,
  /* next        */ NULL,
};

ipixelptr FloatPixel(void)
{
  return(&floatpixel);
}

char *ImageFloatPrintf(char *buf, ImageFloatPix *pix_value)
{
  char number[30];

  sprintf(number, floatpixel.printformat, *pix_value);
  (void) center_text(buf, number, floatpixel.txtlen);

  return(buf);

} /* ImageFloatPrintf() */

typedef ImageFloatPix ImagePixel;

#define FLOATPIX 1

#define ImageXxxxResize ImageFloatResize
#include "ImageProtoPix.c"
