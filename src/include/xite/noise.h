
/*
  This header-file is produced automatically (at least in part) by
  makeHeader of BLAB, Ifi, UiO.
*/


/*

________________________________________________________________

        noise.h
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

#ifndef _NOISE_H_
#define _NOISE_H_

_XITE_CPLUSPLUS_BEGIN

#include <xite/biff.h>


extern _XITE_WIN_DLL BiffStatus addGauss  _XITE_PARAMS(( IBAND b1, IBAND b2, double my, double sigma ));
extern _XITE_WIN_DLL int addPoisson _XITE_PARAMS(( IBAND input, IBAND output ));
extern _XITE_WIN_DLL BiffStatus mkGauss  _XITE_PARAMS(( IBAND b, double my, double sigma ));
extern _XITE_WIN_DLL int mkPoisson _XITE_PARAMS(( double alfa, IBAND output ));
extern _XITE_WIN_DLL int multGauss  _XITE_PARAMS(( IBAND b1, IBAND b2, double my, double sigma ));

_XITE_CPLUSPLUS_END

#endif /* _NOISE_H_ */
