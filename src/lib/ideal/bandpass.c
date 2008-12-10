
/*

________________________________________________________________

        bandpassf
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

static char *Id = "$Id$, Blab, UiO";


#include <stdlib.h>
#include <math.h>
#include <xite/includes.h>
#include <xite/biff.h>
#include XITE_STDIO_H
#include XITE_STRING_H
#include <xite/fft.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include <xite/arithmetic.h>
#include <xite/ideal.h>
#include "ideal_L.h"

#ifndef M_PI
#define M_PI    3.14159265358979323846
#endif



int bandpass(IBAND band, double low_cut_frequency, double high_cut_frequency, int filter_size, window_type win_type)
{
  int stat;
  IBAND b2;

  stat = lowpass(band, high_cut_frequency, filter_size, win_type);
  if (stat != 0) return(Error(stat, "%s\n", e_s[stat]));

  b2 = Imake_band(Ipixtyp(band), Ixsize(band), Iysize(band));

  stat = lowpass(b2, low_cut_frequency, filter_size, win_type);
  if (stat != 0) return(Error(stat, "%s\n", e_s[stat]));

  stat = signDiff(band, b2, band, 1.0, 0.0);
  if (stat != 0) return(1);

  Idel_band(&b2);

  return(0);

} /* bandpass() */



BiffStatus bandpassf (IBAND band, double low_cut_frequency, double high_cut_frequency, int filter_size, window_type win_type)
{
  int stat;

  if (Ipixtyp(band) != Icomplex_typ)
    return(Error(E_PIXTYP, "%d\n", e_s[E_PIXTYP]));

  stat = bandpass(band, low_cut_frequency, high_cut_frequency, filter_size,
		  win_type);
  if (stat != 0) return(Error(stat, "%s\n", e_s[stat]));

  if (fft2d(band, band, 0, 1.0))
    return(Error(E_FFT, "%s\n", e_s[E_FFT]));

  return(0);
}
