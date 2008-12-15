

/*C*

________________________________________________________________

        ImageFloatPix.h
        $Id$
        Copyright 1994, Blab, UiO
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

#ifndef _Imagefloatpix_h
#define _Imagefloatpix_h

#include <xite/biff.h>
#include <xite/Image.h>

_XITE_CPLUSPLUS_BEGIN

typedef REAL ImageFloatPix;

extern ipixelptr FloatPixel _XITE_PARAMS(( void ));
extern void ImageFloatResize _XITE_PARAMS(( ImageWidget wid, int x, int y, int width, int height ));
extern char *ImageFloatPrintf _XITE_PARAMS(( char *buf, ImageFloatPix *pix_value ));

_XITE_CPLUSPLUS_END

#endif /* _Imagefloatpix_h */
