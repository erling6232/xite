/*

________________________________________________________________

        ImageDoubleComplexResize.c
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
#include "ImageDoubleComplexPix.h"
#include XITE_STDIO_H

static ipixel doublecomplexpixel =
{
  /* pixelname   */ ImageDoubleComplexPixel,
  /* printformat */ NULL,
  /* pixelsize   */ 16,
  /* resize      */ ImageDoubleComplexResize,
  /* printf      */ (imageprintffunc) ImageDoubleComplexPrintf,
  /* txtlen      */ 22,
  /* scaletype   */ SCALE_LINEAR,
  /* scalea      */ 0.0,
  /* scaleb      */ 0.0,
  /* minval      */ 0.0,
  /* maxval      */ 0.0,
  /* next        */ NULL,
};

ipixelptr DoubleComplexPixel(void)
{
  return(&doublecomplexpixel);
}

char *ImageDoubleComplexPrintf(char *buf, ImageDoubleComplexPix *pix_value)
{
  char number[30], *ptr;

  sprintf(number, doublecomplexpixel.printformat, pix_value->re);
  ptr = strchr(number, ' ');
  if (ptr == NULL) ptr = number + strlen(number);

  if (pix_value->im >= 0) (void) strcpy(ptr++, "+");
  sprintf(ptr, doublecomplexpixel.printformat, pix_value->im);
  ptr = strchr(ptr, ' ');
  if (ptr == NULL) ptr = number + strlen(number);
  (void) strcpy(ptr, "j");

  (void) center_text(buf, number, doublecomplexpixel.txtlen);

  return(buf);

} /* ImageDoubleComplexPrintf() */

typedef ImageDoubleComplexPix ImagePixel; 

#define RESIZE_NO(a)            *a++
#define RESIZE_YES(a)           a[*xptr_s]
#define EQ_24(a)                *ximg_ptr_s++ = trans_s[0];
#define EQ_8(a)                 EQ_24(a)
#define EQ_4A(a)                EQ_24(a)
#define EQ_4B(a)                EQ_24(a)
#define EQ_2A(a)                1
#define EQ_2B                   =
#define INIT24                  
#define INIT21                  INIT24
#define INIT8                   INIT24
#define INIT7                   INIT24
#define INIT4                   INIT24
#define INIT2                   INIT24

#define DOUBLECOMPLEXPIX 1
#define ImageXxxxResize ImageDoubleComplexResize

#include "ImageProtoPix.c"
