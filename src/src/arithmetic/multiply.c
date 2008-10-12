/*

________________________________________________________________

        multiply
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


#include <xite/includes.h>
#include <xite/biff.h>
#include <xite/arithmetic.h>
#include <xite/readarg.h>
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/message.h>

#ifndef MIN
# define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

/*F:fftMult=multiply*/
/*F:multiply*

________________________________________________________________

		multiply
________________________________________________________________

Name:		multiply, fftMult - multiply two bands pixel by pixel

Syntax:         | #include <xite/arithmetic.h>
		|
                | int multiply( IBAND band1, IBAND band2,
                |    IBAND output, double scale );
		|
		| #include <xite/fft.h>
		|
		| int fftMult( IC_BAND in_band1,
		|    IC_BAND in_band2, IC_BAND out_band );
		|
Description:	'multiply' calculates, pixel by pixel, the product 
                of band1, band2, and scale, and assigns the result
		to output.

		| output(x,y) = band1(x,y) * band2(x,y) * scale

		Resulting values outside the domain of the actual
		pixel type of 'output' is truncated to the nearest legal value.

		'fftMult' is retained for compatibility with previous XITE
		versions. It is a macro call to 'multiply' with 'scale' equal
		to 1.0.

Restrictions:   All pixel types are accepted, as long as all three
		bands have the same pixel type. There are two exceptions:
		'band2' may be real if 'band1' and 'band3' are complex, and
		'band2'	may be double if 'band1' and 'band3' are double
		complex.
  
		Only the largest rectangle common to all three bands
                is multiplied.

Return value:	| Status, 0 => ok
		| 1 => Illegal combination of pixel types
		| 2 => Unknown pixel type

See also:       multiply(1), divide(3), absDiff(3), signDiff(3), addw(3),
                average(3), scale(3)

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Modified:       Svein Bøe, BLAB, Ifi, UiO (allowed combinations of complex
                times real and double complex times double - this takes care
		of the capabilities of 'fftMult').

Id:             $Id$
________________________________________________________________

*/

int multiply(IBAND band1, IBAND band2, IBAND output, double scale)
{
  long x, y, xsize, ysize;
  IPIXTYP pt1, pt2, pto;
  double val;

  pt1 = Ipixtyp(band1);
  pt2 = Ipixtyp(band2);
  pto = Ipixtyp(output);
  if (pt1 != pt2) {
    if (!((pt1 == Icomplex_typ   && pt2 == Ireal_typ) ||
	  (pt1 == Id_complex_typ && pt2 == Idouble_typ)))
    return(Error(1, "Illegal combination of pixel types.\n"));
  }
  if (pt1 != pto) return(Error(1, "Illegal combination of pixel types.\n"));

  xsize = MIN(MIN(Ixsize(band1),Ixsize(band2)),Ixsize(output));
  ysize = MIN(MIN(Iysize(band1),Iysize(band2)),Iysize(output));

  switch (pt1)
    {
    case (Iu_byte_typ): {
      IUB_BAND bnd1 = (IUB_BAND)band1;
      IUB_BAND bnd2 = (IUB_BAND)band2;
      IUB_BAND out  = (IUB_BAND)output;

      for (y=1; y <= ysize; ++ y) {
	for (x=1; x <= xsize; ++ x) {
	  val = bnd1[y][x] * bnd2[y][x] * scale;
	  out[y][x] =
             CLIP_RANGE_ROUND_CAST(val, 0.0, UNS_BYTE_MAX, UNS_BYTE);
	}
      }
      return(Iok);
    }

    case (Is_byte_typ): {
      ISB_BAND bnd1 = (ISB_BAND)band1;
      ISB_BAND bnd2 = (ISB_BAND)band2;
      ISB_BAND out  = (ISB_BAND)output;
      for (y=1; y <= ysize; ++ y) {
	for (x=1; x <= xsize; ++ x) {
	  val = bnd1[y][x] * bnd2[y][x] * scale;
	  out[y][x] =
             CLIP_RANGE_ROUND_CAST(val, SIGN_BYTE_MIN, SIGN_BYTE_MAX,
                                   SIGN_BYTE);
	}
      }
      return(Iok);
    }

    case (Iu_short_typ): {
      IUS_BAND bnd1 = (IUS_BAND)band1;
      IUS_BAND bnd2 = (IUS_BAND)band2;
      IUS_BAND out  = (IUS_BAND)output;
      for (y=1; y <= ysize; ++ y) {
	for (x=1; x <= xsize; ++ x) {
	  val = bnd1[y][x] * bnd2[y][x] * scale;
	   out[y][x] =
             CLIP_RANGE_ROUND_CAST(val, 0, UNS_SHORT_MAX, UNS_SHORT);
	}
      }
      return(Iok);
    }

    case (Is_short_typ): {
      ISS_BAND bnd1 = (ISS_BAND)band1;
      ISS_BAND bnd2 = (ISS_BAND)band2;
      ISS_BAND out  = (ISS_BAND)output;
      for (y=1; y <= ysize; ++ y) {
	for (x=1; x <= xsize; ++ x) {
	  val = bnd1[y][x] * bnd2[y][x] * scale;
	  out[y][x] =
	    CLIP_RANGE_ROUND_CAST(val, SIGN_SHORT_MIN, SIGN_SHORT_MAX,
                                   SIGN_SHORT);
	}
      }
      return(Iok);
    }

    case (Integer_typ): {
      II_BAND bnd1 = (II_BAND)band1;
      II_BAND bnd2 = (II_BAND)band2;
      II_BAND out  = (II_BAND)output;
      for (y=1; y <= ysize; ++ y) {
	for (x=1; x <= xsize; ++ x) {
	  val = bnd1[y][x] * bnd2[y][x] * scale;
	  out[y][x] =
             CLIP_RANGE_ROUND_CAST(val, INTEGER_MIN, INTEGER_MAX, INTEGER);
	}
      }
      return(Iok);
    }

    case (Ireal_typ): {
      IR_BAND bnd1 = (IR_BAND)band1;
      IR_BAND bnd2 = (IR_BAND)band2;
      IR_BAND out  = (IR_BAND)output;
      for (y=1; y <= ysize; ++ y) {
	for (x=1; x <= xsize; ++ x) {
	  val = bnd1[y][x] * bnd2[y][x] * scale;
	  out[y][x] = CLIP_RANGE_CAST(val, -REAL_MAX, REAL_MAX, REAL);
	}
      }
      return(Iok);
    }

    case (Icomplex_typ): {
      IC_BAND bnd1 = (IC_BAND)band1;
      IC_BAND out  = (IC_BAND)output;

      if (pt2 == Icomplex_typ) {
	IC_BAND bnd2 = (IC_BAND)band2;
	for (y=1; y <= ysize; ++ y) {
	  for (x=1; x <= xsize; ++ x) {
	    val = (bnd1[y][x].re * bnd2[y][x].re -
		   bnd1[y][x].im * bnd2[y][x].im) * scale;
	    out[y][x].re = CLIP_RANGE_CAST(val, -REAL_MAX, REAL_MAX, REAL);

	    val = (bnd1[y][x].re * bnd2[y][x].im +
		   bnd1[y][x].im * bnd2[y][x].re) * scale;
	    out[y][x].im = CLIP_RANGE_CAST(val, -REAL_MAX, REAL_MAX, REAL);
	  }
	}
      } else {
	IR_BAND bnd2 = (IR_BAND)band2;
	for (y=1; y <= ysize; ++ y) {
	  for (x=1; x <= xsize; ++ x) {
	    val = (bnd1[y][x].re * bnd2[y][x]) * scale;
	    out[y][x].re = CLIP_RANGE_CAST(val, -REAL_MAX, REAL_MAX, REAL);

	    val = (bnd1[y][x].im * bnd2[y][x]) * scale;
	    out[y][x].im = CLIP_RANGE_CAST(val, -REAL_MAX, REAL_MAX, REAL);
	  }
	}
      }
      return(Iok);
    }

    case (Idouble_typ): {
      ID_BAND bnd1 = (ID_BAND)band1;
      ID_BAND bnd2 = (ID_BAND)band2;
      ID_BAND out  = (ID_BAND)output;
      for (y=1; y <= ysize; ++ y) {
	for (x=1; x <= xsize; ++ x) {
	  out[y][x] = bnd1[y][x] * bnd2[y][x] * scale;
	}
      }
      return(Iok);
    }

    case (Id_complex_typ): {
      IDC_BAND bnd1 = (IDC_BAND)band1;
      IDC_BAND out  = (IDC_BAND)output;

      if (pt2 == Id_complex_typ) {
	IDC_BAND bnd2 = (IDC_BAND)band2;

	for (y=1; y <= ysize; ++ y) {
	  for (x=1; x <= xsize; ++ x) {
	    out[y][x].re = (bnd1[y][x].re * bnd2[y][x].re -
			    bnd1[y][x].im * bnd2[y][x].im) * scale;
	    out[y][x].im = (bnd1[y][x].re * bnd2[y][x].im +
			    bnd1[y][x].im * bnd2[y][x].re) * scale;
	  }
	}
      } else {
	ID_BAND bnd2 = (ID_BAND)band2;

	for (y=1; y <= ysize; ++ y) {
	  for (x=1; x <= xsize; ++ x) {
	    out[y][x].re = (bnd1[y][x].re * bnd2[y][x]) * scale;
	    out[y][x].im = (bnd1[y][x].im * bnd2[y][x]) * scale;
	  }
	}
      }
      return(Iok);
    }

    default: return(Error(2, "Unknown pixel-type.\n"));

    } /* switch */

}  /* multiply() */
