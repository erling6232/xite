

/*C*

________________________________________________________________

        fht2d
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

#define PRINTA(a,b) printf(" %s   %f %f %f %f\n",a,b[1],b[2],b[3],b[4]); 
#define PRINTAR(a,b) PRINTA(a,b[1]); PRINTA("",b[2]); PRINTA("",b[3]); PRINTA("",b[4]);

#include <stdlib.h>
#include <math.h>
#include <xite/includes.h>
#include <xite/biff.h>
#include <xite/fht.h>
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#include <xite/convert.h>
#include <xite/copy.h>
#include <xite/message.h>
#include <xite/readarg.h>
#ifdef HAVE_MALLOC_H
# include <malloc.h>
#endif

/*P:fht2d*

________________________________________________________________

		fht2d
________________________________________________________________

Name:		fht2d - 2 dimensional fast Hartley transform

Syntax:		fht2d [-i] [-b] <inimage> <outimage>

Description:	'fht2d' performs 2 dimensional forward or inverse fast
                Hartley transform. All pixel types are accepted as input
		and converted to real.

		The forward and inverse discrete Hartley transforms
		are given respectively as:

		|
		|           M   N
		| H(u,v) = Sum Sum f(x,y) [cas(2*Pi(u*x/M + v*y/N))]
		|          x=0 y=0
		|
		|           M   N
		| f(x,y) = Sum Sum H(u,v) [cas(2*Pi(u*x/M + v*y/N))]
		|          x=0 y=0
		|
		where
		| cas(x) = cos(x) + sin(x)

		The relationships between the Fourier and Hartley transforms
		are described in the documentation of 'ht2ft(1)' and
		'ft2ht(1)'.

		Convolution of two images can be accomplished with the
		Hartley transform:

		| f1(x,y) * f2(x,y) -> F1(u,v) F2(u,v)
		|    = H1(u,v) H2_e(u,v) + H1(-u,-v) H2_o(u,v)
		where H2_e and H2_o are the even and odd parts of H2
		respectively, i.e.

		| H2_e(u,v) = (H2(u,v) + H2(-u, -v))/2
		| H2_o(u,v) = (H2(u,v) - H2(-u, -v))/2

		If f2(x,y) has vertical and horizontal even symmetry,
		then H2_o(u,v) = 0 and the convolution can be performed
		with Hartley transforms as follows:

		| f1(x,y) * f2(x,y) -> H1(u,v) H2(u,v)

		If f2(x,y) has vertical and horizontal odd symmetry, then
		H2_e(u,v) = 0 and the convolution can be done as

	        | f1(x,y) * f2(x,y) -> H1(-u,-v) H2(u,v)

Options:        &-i
                Return the inverse transform. Default: Forward transform.

                &-b
		Use pixeltype unsigned byte for the output image.
		Default: Pixeltype real.

Reference:	&[1] Mark A. O'Neill
                "Faster than Fast Fourier",
		BYTE April 1988 293-300

		&[2] R. N. Bracewell
		"The Hartley Transform",
		Oxford Univ. Press 1986,
		ISBN 0-19-503969-6

See also:	fht2d(3), fft2d(1), haar2d(1), fht1d(3), fhtInitTables(3),
                fhtPower(1), fhtPhase(1), ht2ft(1), ft2ht(1)

Restrictions:	Bands should be quadratic and have size some power of two.

Return value:	| 0 => ok
		| 1 => bad number of arguments
		| 2 => can't read input
		| 3 => can't write output

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Examples:	fht2d mona.img monaHt.img

Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  int nbands, bn, i, b;
  IMAGE img1, img2;
  IBAND band;
  char *args;

  Iset_message(TRUE);
  Iset_abort(TRUE);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-i] <input> <output>\n"));

  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);

  i = read_bswitch(&argc, argv, "-i");
  b = read_bswitch(&argc, argv, "-b");

  if (argc != 3) Usage(1, "Illegal number of arguments.\n");

  img1 = Iread_image(argv[1]);
  if (!img1) Error(2, "Can't read %s.\n", argv[1]);

  nbands = Inbands(img1);
  for (bn=1; bn <= nbands; bn++)
    if (Ipixtyp(img1[bn]) != Ireal_typ) {
      band = (IBAND)img1[bn];
      img1[bn] = (IBAND)mkConvertBand(band, Ireal_typ);
      Idel_band(&band);
    }

  img2 = Icopy_init(img1);

  for (bn=1; bn <= nbands; bn++)
    if (fht2d((IR_BAND)img1[bn], (IR_BAND)img2[bn], !i))
      Error(2, "Error in fht2d, band %d\n", bn);

  Ihistory(img2, argv[0], args);
  if (b)
    for (bn=1; bn <= nbands; bn++) {
      band = (IBAND)img2[bn];
      img2[bn] = (IBAND)mkConvertBand(band, Iu_byte_typ);
      Idel_band(&band);
    }

  if (Iwrite_image(img2, argv[2])) Error(2, "Can't write %s\n", argv[2]);

  return(0);
}
