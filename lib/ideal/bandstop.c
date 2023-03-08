

/*C*

________________________________________________________________

        bandstopf
        $Id$
        Copyright 1991, Blab, UiO
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



int bandstop(IBAND band, double low_cut_frequency, double high_cut_frequency, int *filter_size, window_type win_type)
{
  int stat;
  long xsize, ysize;
  IPIXTYP pt;

  xsize = Ixsize(band);
  ysize = Iysize(band);

  if (((*filter_size) % 2 == 0) && (*filter_size) < xsize &&
      (*filter_size) < ysize) {
    (*filter_size)++;
    Warning(1, "Filter-size must be odd, increased by one.\n");
  } else if ((*filter_size) % 2 == 0) {
    (*filter_size)--;
    Warning(1, "Filter-size must be odd, decreased by one.\n");
  }

  stat = bandpass(band, low_cut_frequency, high_cut_frequency,
		  (*filter_size), win_type);
  if (stat != 0) return(Error(stat, "%s\n", e_s[stat]));

  /* Change sign */

  stat = scale(band, band, -1.0, 0.0);
  if (stat) return(Error(E_PIXTYP, "%s\n", e_s[E_PIXTYP]));

  /* Change to bandstop filter. */

  stat = 0;
  pt = Ipixtyp(band);
  switch ((int) pt) {
  case Ireal_typ: {
    IR_BAND bnd = (IR_BAND) band;

    bnd[1][1]++;
  }
    break;
  case Idouble_typ: {
    ID_BAND bnd = (ID_BAND) band;

    bnd[1][1]++;
  }
    break;
  case Icomplex_typ: {
    IC_BAND bnd = (IC_BAND) band;

    bnd[1][1].re++;
  }
    break;
  case Id_complex_typ: {
    IDC_BAND bnd = (IDC_BAND) band;

    bnd[1][1].re++;
  }
    break;
  default:
    stat = E_PIXTYP;
    break;
  } /* switch */
  
  if (stat != 0) return(Error(stat, "%s\n", e_s[E_PIXTYP]));

  return(0);

} /* bandstop() */




BiffStatus bandstopf(IBAND band, double low_cut_frequency, double high_cut_frequency, int *filter_size, window_type win_type)
{
   int stat;

   if (Ipixtyp(band) != Icomplex_typ)
     return(Error(E_PIXTYP, "%d\n", e_s[E_PIXTYP]));

   stat = bandstop(band, low_cut_frequency, high_cut_frequency,
		   filter_size, win_type);
   if (stat != 0) return(Error(stat, "%s\n", e_s[stat]));

   if (fft2d(band, band, 0, 1.0))
     return(Error(E_FFT, "%s\n", e_s[E_FFT]));

   return(0);
}
