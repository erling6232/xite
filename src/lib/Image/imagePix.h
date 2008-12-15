
/*
  This header-file is produced automatically (at least in part) by
  makeHeader of BLAB, Ifi, UiO.
*/


/*C*

________________________________________________________________

        imagePix.h
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

#ifndef _IMAGEPIX_H_
#define _IMAGEPIX_H_

_XITE_CPLUSPLUS_BEGIN

#include "ImagePixType.h"

#ifdef IMAGE_OVERLAY
# include "ImageOverlay.h"
#else
# include "Image.h"
#endif

extern char * ImageBytePrintf  _XITE_PARAMS(( char *buf, ImageBytePix *pix ));
extern char * ImageSignedBytePrintf  _XITE_PARAMS(( char *buf, ImageSignedBytePix *pix ));
extern char * ImageDoublePrintf  _XITE_PARAMS(( char *buf, ImageDoublePix *pix ));
extern char * ImageFloatPrintf  _XITE_PARAMS(( char *buf, ImageFloatPix *pix ));
extern char * ImageComplexPrintf  _XITE_PARAMS(( char *buf, ImageComplexPix *pix ));
extern char * ImageDoubleComplexPrintf  _XITE_PARAMS(( char *buf, ImageDoubleComplexPix *pix ));
extern char * ImageLongPrintf  _XITE_PARAMS(( char *buf, ImageLongPix *pix ));
#ifdef IMAGE_OVERLAY
extern char * ImageOverlayPrintf  _XITE_PARAMS(( char *buf, ImageBytePix *pix ));
#endif
extern char * ImageShortPrintf _XITE_PARAMS(( char *buf, ImageShortPix *pix ));
extern char * ImageUnsignedShortPrintf _XITE_PARAMS(( char *buf, ImageUnsignedShortPix *pix ));

extern void ImageByteResize _XITE_PARAMS(( ImageWidget wid, int x, int y, int width, int height ));
extern void ImageSignedByteResize _XITE_PARAMS(( ImageWidget wid, int x, int y, int width, int height ));
extern void ImageDoubleResize _XITE_PARAMS(( ImageWidget wid, int x, int y, int width, int height ));
extern void ImageFloatResize _XITE_PARAMS(( ImageWidget wid, int x, int y, int width, int height ));
extern void ImageComplexResize _XITE_PARAMS(( ImageWidget wid, int x, int y, int width, int height ));
extern void ImageDoubleComplexResize _XITE_PARAMS(( ImageWidget wid, int x, int y, int width, int height ));
extern void ImageLongResize _XITE_PARAMS(( ImageWidget wid, int x, int y, int width, int height ));
#ifdef IMAGE_OVERLAY
extern void ImageOverlayResize _XITE_PARAMS(( ImageOverlayWidget wid, int x, int y, int width, int height ));
#endif
extern void ImageShortResize _XITE_PARAMS(( ImageWidget wid, int x, int y, int width, int height ));
extern void ImageUnsignedShortResize _XITE_PARAMS(( ImageWidget wid, int x, int y, int width, int height ));

_XITE_CPLUSPLUS_END

#endif /* _IMAGEPIX_H_ */
