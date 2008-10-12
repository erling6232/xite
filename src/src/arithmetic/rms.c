
/*

________________________________________________________________

        rms
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


#include <math.h>
#include <stdlib.h>
#include <xite/includes.h>
#include <xite/biff.h>
#include <xite/arithmetic.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include XITE_STDIO_H

#ifndef MIN
# define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAIN

/*F:rms*

________________________________________________________________

		rms
________________________________________________________________

Name:		rms - root mean square difference between two bands

Syntax:         | #include <xite/arithmetic.h>
		|
                | double rms( IBAND band1, IBAND band2,
                |    int edge );
		|

Description:	The root mean square difference between the two
                input bands is calculated and written to stdout.

		| rms = SQRT{ SUM [ |band1[y][x] - band2[y][x]| **2 ] / N }

		where SUM is over all N pixels in the band minus an edge of
		width 'edge' which is not included in the calculaton. The
		purpose of this is to exclude differences due to different
		processing of the edge by local filters.

		All pixeltypes are allowed, but 'band1' and 'band2' must be
		of the same pixeltype.

Return value:	| >= 0 : The rms difference
                | -1   : Different pixeltypes in the two bands
		| -2   : Unknown pixeltype

See also:	rms(1), absDiff(3), signDiff(3)

Author:		Tor Lønnestad, BLAB, Ifi, UiO.

Modified:       Svein Bøe, allowing all pixeltypes.

Id:             $Id$
________________________________________________________________

*/

double rms(IBAND band1, IBAND band2, int edge)
{
  long x, y, xstop, ystop, size;
  double sum;
  IPIXTYP pt1, pt2;
  int status = 0;

  pt1   = Ipixtyp(band1);
  pt2   = Ipixtyp(band2);
  if (pt1 != pt2) return(Error(-1, "Different pixeltypes.\n"));

  xstop = MIN(Ixsize(band1), Ixsize(band2)) - edge;
  ystop = MIN(Iysize(band1), Iysize(band2)) - edge;
  size  = (xstop-edge)*(ystop-edge);

  switch ((int) pt1) {
  case Iu_byte_typ: {
    IUB_BAND bnd1 = (IUB_BAND) band1;
    IUB_BAND bnd2 = (IUB_BAND) band2;
    int diff;

    sum = 0;
    if ((UNS_BYTE_MAX + 1) < size) {
      /* Faster to use lookup table. */
      double square[UNS_BYTE_MAX + 1];

      for (x=0; x <= UNS_BYTE_MAX; x++) square[x] = x*x;

      for (y=edge+1; y <= ystop; y++) {
	for (x=edge+1; x <= xstop; x++) {
	  diff = bnd1[y][x] - bnd2[y][x];
	  if (diff < 0) diff = -diff;
	  sum += square[diff];
	}
      }
    } else {
      for (y=edge+1; y <= ystop; y++) {
	for (x=edge+1; x <= xstop; x++) {
	  diff = bnd1[y][x] - bnd2[y][x];
	  sum += diff * diff;
	}
      }
    }
  }
    break;

  case Is_byte_typ: {
    ISB_BAND bnd1 = (ISB_BAND) band1;
    ISB_BAND bnd2 = (ISB_BAND) band2;
    int diff;

    sum = 0;
    if ((SIGN_BYTE_MAX - SIGN_BYTE_MIN + 1) < size) {
      /* Faster to use lookup table. */
      double square[SIGN_BYTE_MAX - SIGN_BYTE_MIN + 1];

      for (x = 0; x <= SIGN_BYTE_MAX - SIGN_BYTE_MIN; x++)
	square[x] = x*x;

      for (y=edge+1; y <= ystop; y++) {
	for (x=edge+1; x <= xstop; x++) {
	  diff = bnd1[y][x] - bnd2[y][x];
	  if (diff < 0) diff = -diff;
	  sum += square[diff];
	}
      }
    } else {
      for (y=edge+1; y <= ystop; y++) {
	for (x=edge+1; x <= xstop; x++) {
	  diff = bnd1[y][x] - bnd2[y][x];
	  sum += diff * diff;
	}
      }
    }
  }
    break;

  case Iu_short_typ: {
    IUS_BAND bnd1 = (IUS_BAND) band1;
    IUS_BAND bnd2 = (IUS_BAND) band2;
    int diff;

    sum = 0;
    if ((UNS_SHORT_MAX + 1) < size) {
      /* Faster to use lookup table. */
      double square[UNS_SHORT_MAX + 1];

      for (x = 0; x <= UNS_SHORT_MAX; x++)
	square[x] = x*x;

      for (y=edge+1; y <= ystop; y++) {
	for (x=edge+1; x <= xstop; x++) {
	  diff = bnd1[y][x] - bnd2[y][x];
	  if (diff < 0) diff = -diff;
	  sum += square[diff];
	}
      }
    } else {
      for (y=edge+1; y <= ystop; y++) {
	for (x=edge+1; x <= xstop; x++) {
	  diff = bnd1[y][x] - bnd2[y][x];
	  sum += diff * diff;
	}
      }
    }
  }
    break;

  case Is_short_typ: {
    ISS_BAND bnd1 = (ISS_BAND) band1;
    ISS_BAND bnd2 = (ISS_BAND) band2;
    int diff;

    sum = 0;
    if ((SIGN_SHORT_MAX - SIGN_SHORT_MIN + 1) < size) {
      /* Faster to use lookup table. */
      double square[SIGN_SHORT_MAX - SIGN_SHORT_MIN + 1];

      for (x = 0; x <= SIGN_SHORT_MAX - SIGN_SHORT_MIN; x++)
	square[x] = x*x;

      for (y=edge+1; y <= ystop; y++) {
	for (x=edge+1; x <= xstop; x++) {
	  diff = bnd1[y][x] - bnd2[y][x];
	  if (diff < 0) diff = -diff;
	  sum += square[diff];
	}
      }
    } else {
      for (y=edge+1; y <= ystop; y++) {
	for (x=edge+1; x <= xstop; x++) {
	  diff = bnd1[y][x] - bnd2[y][x];
	  sum += diff * diff;
	}
      }
    }

  }
    break;

  case Integer_typ: {
    II_BAND bnd1 = (II_BAND) band1;
    II_BAND bnd2 = (II_BAND) band2;
    long diff;

    sum = 0;
    for (y=edge+1; y <= ystop; y++) {
      for (x=edge+1; x <= xstop; x++) {
	diff = bnd1[y][x] - bnd2[y][x];
	sum += diff * diff;
      }
    }
  }
    break;

  case Ireal_typ: {
    IR_BAND bnd1 = (IR_BAND) band1;
    IR_BAND bnd2 = (IR_BAND) band2;
    double diff;

    sum = 0;
    for (y=edge+1; y <= ystop; y++) {
      for (x=edge+1; x <= xstop; x++) {
	diff = bnd1[y][x] - bnd2[y][x];
	sum += diff * diff;
      }
    }    

  }
    break;

  case Idouble_typ: {
    ID_BAND bnd1 = (ID_BAND) band1;
    ID_BAND bnd2 = (ID_BAND) band2;
    double diff;

    sum = 0;
    for (y=edge+1; y <= ystop; y++) {
      for (x=edge+1; x <= xstop; x++) {
	diff = bnd1[y][x] - bnd2[y][x];
	sum += diff * diff;
      }
    }
  }
    break;

  case Icomplex_typ: {
    IC_BAND bnd1 = (IC_BAND) band1;
    IC_BAND bnd2 = (IC_BAND) band2;
    double diff_re, diff_im;

    sum = 0;
    for (y=edge+1; y <= ystop; y++) {
      for (x=edge+1; x <= xstop; x++) {
	diff_re = bnd1[y][x].re - bnd2[y][x].re;
	diff_im = bnd1[y][x].im - bnd2[y][x].im;
	sum += (diff_re * diff_re + diff_im * diff_im);
      }
    }
  }
    break;

  case Id_complex_typ: {
    IDC_BAND bnd1 = (IDC_BAND) band1;
    IDC_BAND bnd2 = (IDC_BAND) band2;
    double diff_re, diff_im;

    sum = 0;
    for (y=edge+1; y <= ystop; y++) {
      for (x=edge+1; x <= xstop; x++) {
	diff_re = bnd1[y][x].re - bnd2[y][x].re;
	diff_im = bnd1[y][x].im - bnd2[y][x].im;
	sum += (diff_re * diff_re + diff_im * diff_im);
      }
    }
  }
    break;

  default: status = -2;
    break;

  } /* switch */

  if (status) return((double) Error(-2, "Unknown pixeltype.\n"));

  return(sqrt((double)sum/(double)size)); 

} /* rms() */

#endif /* not MAIN */



/*P:rms*

________________________________________________________________

		rms
________________________________________________________________

Name:		rms - root mean square difference between two images

Syntax:		rms <inimage1> <inimage2> [<edge>]

Description:	'rms' calculates the root mean square difference
		between corresponding pairs of bands and writes
		the result to stdout.

		rms = SQRT{ SUM [ |inp1[y][x] - inp2[y][x]| **2 ] / N }

		where SUM is over all N pixels in the band minus an edge of
                width 'edge' which is not included in the calculaton. The
                purpose of this is to exclude differences due to different
                processing of the edge by local filters. Default edge value is
                0.

		All pixeltypes are allowed, but corresponding bands in
		'inimage1' and 'inimage2' must be of the same pixeltype.

See also:	rms(3), absDiff(1), signDiff(1)

Author:		Tor Lønnestad, BLAB, Ifi, UiO.

Examples:	| rms monaMed.img monaAver.img 1

Id:             $Id$
________________________________________________________________

*/

#ifdef MAIN

int main(int argc, char **argv)
{
   IMAGE i1, i2;
   int bn, edge, maxbn;
   double rmss;
   char *args;

   Iset_message(1);
   Iset_abort(1);
   InitMessage(&argc, argv, xite_app_std_usage_text(
     "Usage: %s <image1> <image2> [<edge>]\n"));

   if (argc == 1) Usage(1, NULL);
   args = argvOptions(argc, argv);
   if (argc < 3 || argc > 5) Usage(2, "Illegal number of arguments.\n");

   edge = (argc == 3) ? 0 : atoi(argv[3]);
   i1 = Iread_image(argv[1]);
   i2 = Iread_image(argv[2]);
   maxbn = MIN(Inbands(i1),Inbands(i2));

   for (bn=1; bn <= maxbn; bn++) {
     rmss = rms(i1[bn], i2[bn], edge);
     printf(" RMS of band %d : %.2f\n", bn, rmss);
   }

   return(0);

}  /* main */

#endif /* MAIN */
