
/*

________________________________________________________________

        squareRoot
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



#include <math.h>
#include <stdlib.h>
#include <xite/includes.h>
#include <xite/biff.h>
#include <xite/arithmetic.h>
#include <xite/readarg.h>
#include <xite/message.h>
#include XITE_STDIO_H

#ifndef MIN
# define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

/*F:squareRoot*

________________________________________________________________

		squareRoot
________________________________________________________________

Name:		squareRoot - take the square root of each pixel value

Syntax:         | #include <xite/arithmetic.h>
		|
                | int squareRoot( IBAND input, IBAND output,
                |    double scale );
		|
Description:	'squareRoot' places the square root of the input band
                into the output band:

		| output(x,y) = sqrt(input(x,y)) * scale

		Only the largest rectangle common to the two bands
		is handled.

		Accepts all pixel types except complex and double complex.
		Input and output must have the same pixel type, otherwise use
		convertBiff.

		If the result is outside the domain of the actual pixel type,
		the nearest possible value is returned.

		Identical arguments for input and output are allowed.

Return value:	| 0 => ok
		| 1 => Illegal combination of pixel types
		| 2 => Illegal pixel type

See also:	squareRoot(1), square(3), power(3), absValue(3), phase(3),
                imag(3), real(3), scale(3), multiply(3), divide(3),
		convertBand(3)

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Id:             $Id$
________________________________________________________________

*/

int squareRoot(IBAND input, IBAND output, double scale)
{
  int x, y, xsize, ysize;
  double val;
  IPIXTYP ipt, opt;

  ipt = Ipixtyp(input);
  opt = Ipixtyp(output);
  if (ipt != opt) {
    return(Error(1, "squareRoot: Illegal combination of pixel types.\n"));
  }

  xsize = MIN(Ixsize(input),Ixsize(output));
  ysize = MIN(Iysize(input),Iysize(output));

  switch ((int) ipt) {
  case (Iu_byte_typ): {
    IUB_BAND bnd1 = (IUB_BAND) input;
    IUB_BAND bnd2 = (IUB_BAND) output;
    for (y=1; y <= ysize; ++y) {
      for (x=1; x <= xsize; ++x) {
	val = sqrt((double)bnd1[y][x]) * scale;
	bnd2[y][x] =
	  CLIP_RANGE_ROUND_CAST(val, 0.0, UNS_BYTE_MAX, UNS_BYTE);
      }
    }
    return(0);
  }

  case (Is_byte_typ): {
    ISB_BAND bnd1 = (ISB_BAND)input;
    ISB_BAND bnd2 = (ISB_BAND)output;
    for (y=1; y <= ysize; ++y) {
      for (x=1; x <= xsize; ++x) {
	val = sqrt((double)bnd1[y][x]) * scale;
	bnd2[y][x] =
	  CLIP_RANGE_ROUND_CAST(val, SIGN_BYTE_MIN, SIGN_BYTE_MAX,
				SIGN_BYTE);
      }
    }
    return(0);
  }

  case (Iu_short_typ): {
    IUS_BAND bnd1 = (IUS_BAND)input;
    IUS_BAND bnd2 = (IUS_BAND)output;
    for (y=1; y <= ysize; ++y) {
      for (x=1; x <= xsize; ++x) {
	val = sqrt((double)bnd1[y][x]) * scale;
	bnd2[y][x] =
	  CLIP_RANGE_ROUND_CAST(val, 0, UNS_SHORT_MAX, UNS_SHORT);
      }
    }
    return(0);
  }

  case (Is_short_typ): {
    ISS_BAND bnd1 = (ISS_BAND)input;
    ISS_BAND bnd2 = (ISS_BAND)output;
    for (y=1; y <= ysize; ++y) {
      for (x=1; x <= xsize; ++x) {
	val = sqrt((double)bnd1[y][x]) * scale;
	bnd2[y][x] =
	  CLIP_RANGE_ROUND_CAST(val, SIGN_SHORT_MIN, SIGN_SHORT_MAX,
				SIGN_SHORT);
      }
    }
    return(0);
  }

  case (Integer_typ): {
    II_BAND bnd1 = (II_BAND)input;
    II_BAND bnd2 = (II_BAND)output;
    for (y=1; y <= ysize; ++y) {
      for (x=1; x <= xsize; ++x) {
	val = sqrt((double)bnd1[y][x]) * scale;
	bnd2[y][x] =
	  CLIP_RANGE_ROUND_CAST(val, INTEGER_MIN, INTEGER_MAX, INTEGER);
      }
    }
    return(0);
  }

  case (Ireal_typ): {
    IR_BAND bnd1 = (IR_BAND)input;
    IR_BAND bnd2 = (IR_BAND)output;
    for (y=1; y <= ysize; ++y) {
      for (x=1; x <= xsize; ++x) {
	val = sqrt((double)bnd1[y][x]) * scale;
	bnd2[y][x] = CLIP_RANGE_CAST(val, -REAL_MAX, REAL_MAX, REAL);
      }
    }
    return(0);
  }

  case (Idouble_typ): {
    ID_BAND bnd1 = (ID_BAND)input;
    ID_BAND bnd2 = (ID_BAND)output;
    for (y=1; y <= ysize; ++y) {
      for (x=1; x <= xsize; ++x) {
	bnd2[y][x] = sqrt((double)bnd1[y][x]) * scale;
      }
    }
    return(0);
  }

  default: return(Error(2, "squareRoot: Illegal pixeltype.\n"));

  } /* switch */
}  /* squareRoot() */
