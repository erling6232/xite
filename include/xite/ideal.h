
/*

________________________________________________________________

        ideal.h
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

#ifndef _IDEAL_H_
#define _IDEAL_H_

_XITE_CPLUSPLUS_BEGIN

#include <xite/biff.h>

/* Window functions. */

typedef enum {
  RECTANGLE,
  BARTLETT,
  TRIANGLE,
  HAMMING,
  HANNING
} window_type;

typedef enum {
  LOWPASS,
  HIGHPASS,
  BANDPASS,
  BANDSTOP,
  LOWPASS_IDEAL,
  BUTTER_LP,
  EXP_LP
} filter_type;


extern _XITE_WIN_DLL double hamming  _XITE_PARAMS(( double n, int size ));
extern _XITE_WIN_DLL double hanning  _XITE_PARAMS(( double n, int size ));
extern _XITE_WIN_DLL double rectangle  _XITE_PARAMS(( double n, int size ));
extern _XITE_WIN_DLL double bartlett  _XITE_PARAMS(( double n, int size ));
extern _XITE_WIN_DLL double triangle  _XITE_PARAMS(( double n, int size ));
extern _XITE_WIN_DLL window_type parseWindowName _XITE_PARAMS(( char *name ));
extern _XITE_WIN_DLL int window  _XITE_PARAMS(( IBAND band, int size, window_type win_type ));
extern _XITE_WIN_DLL int lowpass  _XITE_PARAMS(( IBAND cb, double cut_frequency, int filter_size, window_type win_type ));
extern _XITE_WIN_DLL int lowpassu  _XITE_PARAMS(( IBAND cb, double cut_frequency, int filter_size, window_type win_type, double *sum ));
extern _XITE_WIN_DLL BiffStatus lowpassf  _XITE_PARAMS(( IBAND out_band, double cut_frequency, int filter_size, window_type win_type ));
extern _XITE_WIN_DLL int highpass  _XITE_PARAMS((IBAND cb, double cut_frequency, int *filter_size, window_type win_type ));
extern _XITE_WIN_DLL BiffStatus highpassf  _XITE_PARAMS(( IBAND out_band, double cut_frequency, int *filter_size, window_type win_type ));
extern _XITE_WIN_DLL int bandpass  _XITE_PARAMS(( IBAND cb, double low_cut_frequency, double high_cut_frequency, int filter_size, window_type win_type ));
extern _XITE_WIN_DLL int bandpassu  _XITE_PARAMS(( IBAND cb, double low_cut_frequency, double high_cut_frequency, int filter_size, window_type win_type, double *sum ));
extern _XITE_WIN_DLL BiffStatus bandpassf  _XITE_PARAMS(( IBAND out_band, double low_cut_frequency, double high_cut_frequency, int filter_size, window_type win_type ));
extern _XITE_WIN_DLL int bandstop  _XITE_PARAMS(( IBAND cb, double low_cut_frequency, double high_cut_frequency, int *filter_size, window_type win_type ));
extern _XITE_WIN_DLL BiffStatus bandstopf  _XITE_PARAMS((IBAND out_band, double low_cut_frequency, double high_cut_frequency, int *filter_size, window_type win_type ));
extern _XITE_WIN_DLL BiffStatus lowpassIdealf  _XITE_PARAMS(( IBAND out_band, double cut_frequency ));
extern _XITE_WIN_DLL BiffStatus butterworthf _XITE_PARAMS(( IBAND band, double cut_frequency, int filter_order ));
extern _XITE_WIN_DLL int exponentialf _XITE_PARAMS(( IBAND band, double cut_frequency, int filter_order ));
extern _XITE_WIN_DLL filter_type parseFilterTypeName _XITE_PARAMS(( char *name ));

_XITE_CPLUSPLUS_END

#endif /* _IDEAL_H_ */
