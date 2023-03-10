

/*C*

________________________________________________________________

        phase.c
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
#include <xite/arithmetic.h>
#include XITE_STDIO_H
#include <xite/geometry.h>
#include <xite/readarg.h>
#include <xite/message.h>

#ifndef M_PI
# define M_PI    3.14159265358979323846
#endif

/*F:fftPhase=phase*/
/*F:phase*

________________________________________________________________

		phase
________________________________________________________________

Name:		phase, fftPhase - phase of complex image band

Syntax:         | #include <xite/arithmetic.h>
		|
                | int phase( IBAND in_band, IBAND out_band );
		|
		| #include <xite/fft.h>
		|
                | int fftPhase( IC_BAND in_band,
                |    IR_BAND out_band );
		|
Description:    'phase' computes the phase in radians of a complex or double
                complex 'in_band'. If 'out_band' is an unsigned byte band,
		the result will be scaled so that the interval 0 - 2*pi is
		scaled to the interval 0 - 255. If 'out_band' is a
                real or double band, the result will be left in radians.

		'fftPhase' is retained for compatibility with previous XITE
		versions. It is a macro call to 'phase'.

Restrictions:   'in_band' must be complex or double complex. 'out_band' must
                be real, double or unsigned byte.

See also:	phase(1), power(3), square(3), squareRoot(3), absValue(3),
                imag(3), real(3), fftDisplay(3)

Restrictions:   None
Return value:   | 0 - OK
                | 1 - bands different xsize
                | 2 - bands different ysize
                | 3 - in_band has wrong pixtyp
                | 4 - out_band has wrong pixtyp  

Author:		Trond Muri S?rensen (wrote fftPhase)
Modified:       Svein B?e

Id: 		$Id$
________________________________________________________________

*/


int phase(IBAND in_band, IBAND out_band)
{
  int xsize, ysize, y, x;
  IPIXTYP ipt, opt;
  float re, img;
  double PPI;
  
  PPI=(2*M_PI);
  
  xsize = Ixsize (in_band);
  ysize = Iysize (in_band);
  ipt   = Ipixtyp(in_band);
  opt   = Ipixtyp(out_band);

  if (xsize != Ixsize(out_band))
    return(Error(1, "Bands have different horizontal size.\n"));
  if (ysize != Iysize(out_band))
    return(Error(2, "Bands have different vertical size.\n"));

  if(ipt != Icomplex_typ && ipt != Id_complex_typ)
    return(Error(3, "Input band has wrong pixeltype.\n"));
  if (opt != Ireal_typ && opt != Idouble_typ && opt != Iu_byte_typ)
    return(Error(4, "Output band has wrong pixeltype.\n"));

  switch ((int) ipt) {
  case Icomplex_typ: {
    IC_BAND in = (IC_BAND) in_band;

    if (opt == Ireal_typ) {
      IR_BAND out = (IR_BAND) out_band;

      for (y = 1; y <= ysize; y++) 
	for (x = 1; x <= xsize; x++) {
	  re  = in[y][x].re;
	  img = in[y][x].im;
	  out[y][x] = (REAL) (atan2((double) img, (double) re) + M_PI);
	}
    } else if (opt == Idouble_typ) {
      ID_BAND out = (ID_BAND) out_band;

      for (y = 1; y <= ysize; y++) 
	for (x = 1; x <= xsize; x++) {
	  re  = in[y][x].re;
	  img = in[y][x].im;
	  out[y][x] = (DOUBLE) (atan2((double) img, (double) re) + M_PI);
	}
    } else if (opt == Iu_byte_typ) {
      IUB_BAND out = (IUB_BAND) out_band;
      double val;

      for (y = 1; y <= ysize; y++) 
	for (x = 1; x <= xsize; x++) {
	  re  = in[y][x].re;
	  img = in[y][x].im;
	  val = (atan2((double) img, (double) re) + M_PI) * UNS_BYTE_MAX / PPI;
	  out[y][x] = CLIP_RANGE_ROUND_CAST(val, 0, UNS_BYTE_MAX, UNS_BYTE);
	}
    }
  }
    break;
  case Id_complex_typ: {
    IDC_BAND in = (IDC_BAND) out_band;

    if (opt == Ireal_typ) {
      IR_BAND out = (IR_BAND) out_band;

      for (y = 1; y <= ysize; y++) 
	for (x = 1; x <= xsize; x++) {
	  re  = in[y][x].re;
	  img = in[y][x].im;
	  out[y][x] = (REAL) (atan2((double) img, (double) re) + M_PI);
	}
    } else if (opt == Idouble_typ) {
      ID_BAND out = (ID_BAND) out_band;

      for (y = 1; y <= ysize; y++) 
	for (x = 1; x <= xsize; x++) {
	  re  = in[y][x].re;
	  img = in[y][x].im;
	  out[y][x] = (DOUBLE) (atan2((double) img, (double) re) + M_PI);
	}
    } else if (opt == Iu_byte_typ) {
      IUB_BAND out = (IUB_BAND) out_band;
      double val;

      for (y = 1; y <= ysize; y++) 
	for (x = 1; x <= xsize; x++) {
	  re  = in[y][x].re;
	  img = in[y][x].im;
	  val = (atan2((double) img, (double) re) + M_PI) * UNS_BYTE_MAX / PPI;
	  out[y][x] = CLIP_RANGE_ROUND_CAST(val, 0, UNS_BYTE_MAX, UNS_BYTE);
	}
    }
  }
    break;
  } /* switch */

  return(0);

} /* phase() */
