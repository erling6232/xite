
/*

________________________________________________________________

        logarithm.c
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
#include <xite/blab.h>
#include <xite/convert.h>
#include <xite/geometry.h>
#include <xite/message.h>
#include <xite/readarg.h>

#ifndef MAX
# define MAX(a, b) ((a) >= (b) ? (a) : (b))
#endif

#ifndef XITE_ABS
# define XITE_ABS(i) ((i) >= 0 ? (i) : (-(i)))
#endif



/*P:fftDisplay=logarithm*/
/*P:logarithm*

________________________________________________________________

		logarithm
________________________________________________________________

Name:		logarithm, fftDisplay - Take logarithm of BIFF band (compress
                dynamic range)

Syntax:		logarithm [<option>...] <inimage> <outimage>

Description:    Takes the logarithm of the magnitude of the pixelvalues of a
                real or complex band according to the formula

		| out_pix = (log10(eps + |in_pix|) + offset) * scale

		This mapping will compress the dynamic range of an image and
		make small pixelvalues better visible (e.g. used to display
		Fourier	transform magnitude).

		All pixeltypes are accepted in 'inimage'. For an integral
		output pixeltype, the resulting pixel-values are rounded. The
		pixelvalues are limited to fit within the range of the output
		pixeltype.

		'fftDisplay' is retained for compatibility with previous
		XITE versions. It is a script which invokes 'logarithm'
		with the option '-shift'.

Options:        &-eps eps
                Default value: 1 / (10 * xsize * ysize)

		&-offset offset
		Default value: -log10(eps)

                &-scale scale
                Default value: 20.0

		&-min minimum
		Default: No minimum (only limited by the data type)

		&-pt pixtyp
		Determines pixeltype of bands in 'outimage'. Legal values for
                'pixtyp' are

		| ub or  3 : unsigned byte
		| r  or  8 : real
		| d  or 10 : double

		Default: unsigned byte

		&-shift
		The resulting bands are shifted horizontally xsize/2,
		vertically ysize/2. This is convenient to be able to center
		the (0,0) frequency of a Fourier transform image.

Return value:	| 0 - ok
                | 1 - Illegal pixeltype for 'outimage'.
		| 2 - Error in logarithm function.
		
See also:	logarithm(3), power(1), phase(1), real(1), imag(1),
                absValue(1)

Author:		Trond Sørensen.

Id: 		$Id$
________________________________________________________________

*/

static void usage(char *s)
{
  fprintf(stderr, 
    "Usage: %s [-scale <scale>] <inimage> <outimage>\n", s);
  exit(1);
}

int main(int argc, char **argv)
{
  IMAGE img1, img2;
  IBAND b1, b2;
  double scale, eps, offset, minimum;
  int bn, stat, shift;
  long xsize, ysize;
  IPIXTYP pt;
  char *args, *pta;

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [<option>...] <inimage> <outimage>\n\
       where <option> is one of\n\
         -eps <eps>         \n\
         -offset <offset>   \n\
         -scale <scale>     \n\
         -min <minimum>     \n\
         -pt <pixeltype>    \n\
         -shift             \n"));

  if (argc == 1) Usage(1, NULL);

  args    = argvOptions(argc, argv);
  eps     = read_dswitch(&argc, argv, "-eps",    DOUBLE_MIN);
  offset  = read_dswitch(&argc, argv, "-offset", DOUBLE_MAX);
  scale   = read_dswitch(&argc, argv, "-scale",  0.0);
  pta     = read_switch( &argc, argv, "-pt", 1,  "ub");
  shift   = read_bswitch(&argc, argv, "-shift");
  minimum = read_dswitch(&argc, argv, "-min",    -DOUBLE_MAX);

  pt = IparsePixtyp(pta);
  if (pt != Iu_byte_typ && pt != Ireal_typ && pt != Idouble_typ) {
    Error(1, "Illegal pixeltype for outimage.\n");
    exit(1);
  }
  
  if (argc != 3) Usage(2, "Illegal number of arguments\n");

  img1 = Iread_image(argv[1]);
  img2 = Init_image(Inbands(img1), Ititle(img1));

  for (bn=1; bn<=Inbands(img1); bn++) {
    b1    = img1[bn];
    xsize = Ixsize(b1);
    ysize = Iysize(b1);
    b2    = (IBAND) Imake_band(pt, xsize, ysize);
    
    if ((stat = logarithm(b1, b2, eps, offset, scale, minimum))) {
      Error(2, "Error in logarithm, band %d.\n", bn);
      exit(stat);
    }

    if (shift) {
      if ((stat = shift_band(b2, b2, xsize/2, ysize/2)) != Iok)
	return(Error(5, "Error in shift_band.\n"));
    }

    img2[bn] = b2;
  }

  Icopy_text(img1, img2);
  Ihistory(img2, argv[0], args);
  Iwrite_image(img2, argv[2]);

  return(0);
}
