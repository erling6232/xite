/*

________________________________________________________________

        ImageShortResize.c
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
#include "ImageShortPix.h"
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif

static ipixel shortpixel =
{
  /* pixelname   */ ImageShortPixel,
  /* printformat */ NULL,
  /* pixelsize   */ 2,
  /* resize      */ ImageShortResize,
  /* printf      */ (imageprintffunc) ImageShortPrintf,
  /* txtlen      */ 6,
  /* scaletype   */ SCALE_LINEAR,
  /* scalea      */ 1.0,
  /* scaleb      */ 0.0,
  /* minval      */ SIGN_SHORT_MIN,
  /* maxval      */ SIGN_SHORT_MAX,
  /* next        */ NULL,
};

ipixelptr ShortPixel(void)
{
  return(&shortpixel);
}

char *ImageShortPrintf(char *buf, ImageShortPix *pix_value)
{
  char number[30];

  sprintf(number, shortpixel.printformat, *pix_value);
  (void) center_text(buf, number, shortpixel.txtlen);

  return(buf);

} /* ImageShortPrintf() */

typedef ImageShortPix ImagePixel; 

#define SHORTPIX 1
#define ImageXxxxResize ImageShortResize

#include "ImageProtoPix.c"

