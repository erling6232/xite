
/*
  This header-file is produced automatically (at least in part) by
  makeHeader of BLAB, Ifi, UiO.
*/


/*

________________________________________________________________

        fht.h
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

#ifndef _FHT_H_
#define _FHT_H_

_XITE_CPLUSPLUS_BEGIN

#include <xite/biff.h>


extern _XITE_WIN_DLL int fhtInitTables  _XITE_PARAMS(( float *sinTab, float *cosTab, int *perm, int size ));
extern _XITE_WIN_DLL void fht1d  _XITE_PARAMS(( float *data, float *sinTab, float *cosTab, float *work, int *perm, int size, int power, int forw ));
extern _XITE_WIN_DLL BiffStatus fht2d  _XITE_PARAMS(( IR_BAND inband, IR_BAND outband, int forward ));
extern _XITE_WIN_DLL int fhtPhase  _XITE_PARAMS(( IR_BAND inband, IR_BAND outband ));
extern _XITE_WIN_DLL int fhtPower  _XITE_PARAMS(( IR_BAND inband, IR_BAND outband ));
extern _XITE_WIN_DLL int ft2ht  _XITE_PARAMS(( IC_BAND inband, IR_BAND outband ));
extern _XITE_WIN_DLL int ht2ft  _XITE_PARAMS(( IR_BAND inband, IC_BAND outband ));

_XITE_CPLUSPLUS_END

#endif /* _FHT_H_ */
