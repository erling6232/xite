
/*
  This header-file is produced automatically (at least in part) by
  makeHeader of BLAB, Ifi, UiO.
*/


/*

________________________________________________________________

        ImageFormat.h
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

#ifndef _ImageFormat_H_
#define _ImageFormat_H_

_XITE_CPLUSPLUS_BEGIN

#include <xite/biff.h>

extern char *ImagePixPrintFormat  _XITE_PARAMS(( IPIXTYP pixtyp ));
extern IPIXTYP ImagePixNameToBIFFPixTyp  _XITE_PARAMS(( char *image_pix_name ));
extern char *ImagePixName  _XITE_PARAMS(( IBAND band ));
extern char *ImageTitle  _XITE_PARAMS(( IMAGE img ));
extern int ImageHeight  _XITE_PARAMS(( IBAND band ));
extern int ImageLineLength  _XITE_PARAMS(( IBAND band ));
extern int ImageWidth  _XITE_PARAMS(( IBAND band ));
extern int ImageXstart  _XITE_PARAMS(( IBAND band ));
extern int ImageYstart  _XITE_PARAMS(( IBAND band ));
extern unsigned char *ImageAddress  _XITE_PARAMS(( IBAND band ));

_XITE_CPLUSPLUS_END

#endif /* _ImageFormat_H_ */
