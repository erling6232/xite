
/*

________________________________________________________________

        power
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


#include <xite/includes.h>
#include <xite/biff.h>
#include <xite/arithmetic.h>
#include <xite/readarg.h>
#include <xite/message.h>
#include <stdlib.h>
#include XITE_STDIO_H

#ifndef MIN
# define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAIN

/*F:fftPower=power*/
/*F:power*

________________________________________________________________

		power
________________________________________________________________

Name:		power, fftPower - take the power of each pixel value

Syntax:         | #include <xite/arithmetic.h>
		|
                | int power( IBAND input, IBAND output,
                |    double scale );
		|
		| #include <xite/fft.h>
		|
		| int fftPower( IC_BAND in_band,
                |    IR_BAND out_band );
		|
Description:	'power' finds the absolute magnitude squared of each
                pixel in 'input' and inserts this value in 'output'.

		| output(x,y) = |input(x,y)| * |input(x,y)| * scale

		Only the largest rectangle common to the two bands
		is powerd.

		Accepts all pixel types. Both bands must have the same pixel
		type, except that if 'input' is complex, then 'output' may be
		either complex or real, and if 'input' is double complex, then
		'output' may be either double complex or double.
		
		If the result is outside the domain of the actual pixel type,
		the result is rounded.

		Identical arguments for input and output are allowed.

		'power' is identical to 'square' for non-complex bands.

		'fftPower' is retained for compatibility with previous XITE
		versions. It is a macro call to 'power' with 'scale' equal
		to 1.0

Return value:	| 0 => ok
		| 1 => Illegal combination of pixel types
		| 2 => Unknown pixel type

See also:	power(1), square(3), squareRoot(3), phase(3), absValue(3),
                imag(3), real(3), multiply(3), divide(3), scale(3),
		convertBand(3)

Author:		Svein Bøe, BLAB, Ifi, UiO

Id:             $Id$
________________________________________________________________

*/

#ifndef FUNCPROTO
int power(input, output, scale)
IBAND input, output;
double scale;
#else /* FUNCPROTO */
int power(IBAND input, IBAND output, double scale)
#endif /* FUNCPROTO */
{
  int x, y, xsize, ysize, status = 0;
  IPIXTYP ipt, opt;
  double val;

  ipt = Ipixtyp(input);
  opt = Ipixtyp(output);
  if (ipt != opt) {
    if (!((ipt == Icomplex_typ   && opt == Ireal_typ) ||
          (ipt == Id_complex_typ && opt == Idouble_typ)))
      return(Error(2, "power: Illegal combination of pixel types\n"));
  }

  xsize = MIN(Ixsize(input),Ixsize(output));
  ysize = MIN(Iysize(input),Iysize(output));

  switch ((int) ipt)
    {
    case Iu_byte_typ:
    case Is_byte_typ:
    case Iu_short_typ:
    case Is_short_typ:
    case Integer_typ:
    case Ireal_typ:
    case Idouble_typ:
      status = square(input, output, scale);
      break;
    case (Icomplex_typ): {
      double a, b;
      IC_BAND bnd1 = (IC_BAND)input;

      if (opt == ipt) {
	IC_BAND bnd2 = (IC_BAND)output;
	for (y=1; y <= ysize; ++y) {
	  for (x=1; x <= xsize; ++x) {
	    a = bnd1[y][x].re;
	    b = bnd1[y][x].im;

	    val = (a*a + b*b) * scale;
	    bnd2[y][x].re = CLIP_RANGE_CAST(val, -REAL_MAX, REAL_MAX, REAL);
	    bnd2[y][x].im = 0.0;
	  }
	}
      } else {
	IR_BAND bnd2 = (IR_BAND)output;
	for (y=1; y <= ysize; ++y) {
	  for (x=1; x <= xsize; ++x) {
	    a = bnd1[y][x].re;
	    b = bnd1[y][x].im;

	    val = (a*a + b*b) * scale;
	    bnd2[y][x] = CLIP_RANGE_CAST(val, -REAL_MAX, REAL_MAX, REAL);
	  }
	}
      }
      status = 0;
    }
    break;

    case (Id_complex_typ): {
      double a, b;
      IDC_BAND bnd1 = (IDC_BAND)input;

      if (opt == ipt) {
	IDC_BAND bnd2 = (IDC_BAND)output;
	for (y=1; y <= ysize; ++y) {
	  for (x=1; x <= xsize; ++x) {
	    a = bnd1[y][x].re;
	    b = bnd1[y][x].im;
	    bnd2[y][x].re = (a*a + b*b) * scale;
	    bnd2[y][x].im = 0.0;
	  }
	}
      } else {
	ID_BAND bnd2 = (ID_BAND)output;
	for (y=1; y <= ysize; ++y) {
	  for (x=1; x <= xsize; ++x) {
	    a = bnd1[y][x].re;
	    b = bnd1[y][x].im;
	    bnd2[y][x] = (a*a + b*b) * scale;
	  }
	}
      }
      status = 0;
    }
    break;

    default: status = 2;
      break;
    } /* switch */

  if (!status) return(0);
  else return(Error(2, "power: Unknown pixel type\n"));

 } /* power() */

#endif /* not MAIN */



/*P:fftPower=power*/
/*P:power*

________________________________________________________________

		power
________________________________________________________________

Name:		power, fftPower - take the power of each pixel value

Syntax:		| power [-s <scale>] [-real] <input> <output>
                | fftPower <input> <output>

Description:    'power' squares the absolute value of the input image band
                by band, according to the formula:

		| output(x,y) = |input(x,y)| * |input(x,y)| * scale

		Accepts all pixel types. Both bands must have the same pixel
		type, except that if 'input' is complex, then 'output' may be
		either complex or real, and if 'input' is double complex, then
		'output' may be either double complex or double. 

		If the result is outside the domain of the actual pixel type,
		the result is rounded.

		Output image will have the same pixel type as input.

		'power' is identical to 'square' for non-complex images.

		'fftPower' is retained for compatibility with previous XITE
		versions. It is a script which invokes 'power' with option
		'-real'.

Options:        &-s scale
		Default: 1.0

		&-real
                For each complex input-band, let the resulting output band
                be real. For each double complex input-band, let the
                resulting output band be double.

See also:	power(3), square(1), squareRoot(1), phase(1), absValue(1),
                imag(1), real(1), multiply(1), divide(1), scale(1),
		biffConvert(1)

Author:		Tor Lønnestad, BLAB, ifi, UiO

Examples:       | power mona.img sqmona.img 
		| power -scale 0.05 mona.img sqmona.img

Id:             $Id$
________________________________________________________________

*/

#ifdef MAIN

#ifndef FUNCPROTO
int main(argc, argv)
int argc;
char *argv[];
#else /* FUNCPROTO */
int main(int argc, char **argv)
#endif /* FUNCPROTO */
{
  IMAGE img1, img2;
  IBAND bnd1, bnd2;
  int nbands, bn, real, status;
  char *args;
  double scale;

  Iset_message(1); 
  Iset_abort(1);
  InitMessage(&argc, argv, xite_app_std_usage_text(
	      "Usage: %s [-s <scale>] [-real] <input> <output>\n"));

  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);

  scale = read_dswitch(&argc, argv, "-scale", 1.0);
  scale = read_dswitch(&argc, argv, "-s", scale);
  real  = read_bswitch(&argc, argv, "-real");

  if (argc != 3) Usage(2, "Illegal number of arguments\n");

  img1   = Iread_image(argv[1]);
  nbands = Inbands(img1);
  img2   = Init_image(nbands, Ititle(img1));

  if ((status = Icopy_text(img1, img2)) != Iok) {
    Error(status, "Error in Icopy_text.\n");
    exit(status);
  }

  for (bn=1; bn <= nbands; ++bn) {
    bnd1 = img1[bn];

    if (Ipixtyp(bnd1) == Icomplex_typ && real) {
      bnd2 = Imake_band(Ireal_typ, Ixsize(bnd1), Iysize(bnd1));
    } else if (Ipixtyp(bnd1) == Id_complex_typ && real) {
      bnd2 = Imake_band(Idouble_typ, Ixsize(bnd1), Iysize(bnd1));
    } else bnd2 = bnd1;

    if (bnd2 != bnd1) {
      Iset_start(bnd2, Ixstart(bnd1), Iystart(bnd1));
      Iset_mag(bnd2, Ixmag(bnd1), Iymag(bnd1));
    }

    if ((status = power(bnd1, bnd2, scale)) != Iok)
      Warning(status, "Error in band %d\n", bn);

    img2[bn] = bnd2;
  }

  Ihistory(img2, argv[0], args);
  Iwrite_image(img2, argv[2]);

  return(0);
}

#endif /* MAIN */
