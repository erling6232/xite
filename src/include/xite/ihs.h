
/*
  This header-file is produced automatically (at least in part) by
  makeHeader of BLAB, Ifi, UiO.
*/


/*

________________________________________________________________

        ihs.h
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

#ifndef _IHS_H_
#define _IHS_H_

_XITE_CPLUSPLUS_BEGIN

#include <xite/biff.h>


extern _XITE_WIN_DLL void rgb2ihs  _XITE_PARAMS(( double r, double g, double b, double *i, double *h, double *s, int norm ));
extern _XITE_WIN_DLL void ihs2rgb  _XITE_PARAMS(( double i, double h, double s, double *r, double *g, double *b, int norm ));
extern _XITE_WIN_DLL void rgb2ihs_byte  _XITE_PARAMS(( unsigned char r, unsigned char g, unsigned char b, unsigned char *i, unsigned char *h, unsigned char *s ));
extern _XITE_WIN_DLL void ihs2rgb_byte  _XITE_PARAMS(( unsigned char i, unsigned char h, unsigned char s, unsigned char *r, unsigned char *g, unsigned char *b ));
extern _XITE_WIN_DLL void rgb2ihs_word  _XITE_PARAMS(( unsigned short r, unsigned short g, unsigned short b, unsigned short *i, unsigned short *h, unsigned short *s ));
extern _XITE_WIN_DLL void ihs2rgb_word  _XITE_PARAMS(( unsigned short i, unsigned short h, unsigned short s, unsigned short *r, unsigned short *g, unsigned short *b ));
extern _XITE_WIN_DLL int ihs2rgb_img  _XITE_PARAMS(( IBAND i, IBAND h, IBAND s, IBAND r, IBAND g, IBAND b ));
extern _XITE_WIN_DLL int rgb2ihs_img  _XITE_PARAMS(( IBAND r, IBAND g, IBAND b, IBAND i, IBAND h, IBAND s ));

_XITE_CPLUSPLUS_END

#endif /* _IHS_H_ */
