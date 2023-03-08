

/*

________________________________________________________________

        fft2d.c
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
#include <xite/includes.h>
#include <xite/biff.h>
#include <stdlib.h>
#include <xite/fft.h>
#include XITE_STDIO_H
#include <xite/blab.h>
#include <xite/convert.h>
/*#include "fft_L.h"*/
#include <xite/geometry.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include XITE_MALLOC_H



/*P:fft2d*

________________________________________________________________

		fft2d
________________________________________________________________

Name:		fft2d - Two dimensional Fourier Transform

Syntax:		| fft2d [<option>...] <inimage> <outimage> 

Description:	fft2d performs a 2 dimensional Fourier transform.

                If 'inimage' is of another pixeltype than single precision
		complex, 'inimage' will be converted to single precision
		complex. When 'inimage' is double precision complex, this
		is only allowed when using option '-r' below.

		The forward and inverse discrete Fourier transforms
		with standard scaling are given respectively as:

		|                                       
		|           1    M   N          -j*2*pi*(ux/M + vy/N)
		| F(u,v) = ---  Sum Sum f(x,y) e
		|          M*N  x=0 y=0
		|
		|           M   N          j*2*pi*(ux/M + vy/N)
		| f(x,y) = Sum Sum F(u,v) e
		|          u=0 v=0
		|
		where M=Ixsize(f), N=Iysize(f).

Restrictions:   Horizontal and vertical image size must be powers of two.

Options:        &-i
                Inverse Fourier transform.

                &-s scale, -fftscale scale
                The output image is multiplied by 'scale'. Default 'scale' is
                1/(xsize*ysize) for forward transform and 1.0 for inverse
                transform.

                &-b
                Pixeltype for 'outimage' will be unsigned byte, through
                conversion of the result. Values outside the range of unsigned
                byte will be limited to the min/max values for unsigned byte.

		Default: Complex.

                &-r
		Allow pixeltype of 'inimage' to be double complex. Convert
		image to pixeltype complex before applying the Fourier
		transform.

		Default: Allow only single-precision complex or non-complex
		pixeltype for 'inimage', not double complex. Convert the
		image to pixeltype complex before applying the Fourier
		transform.

Return value:	| 0 - ok
                | 1 - error

See also:       fft2d(3), absValue(1), power(1), phase(1), real(1), imag(1),
                fftMagPhase(1), multiply(1)

Author:		Otto Milvang
Doc:            Otto Milvang and Svein Bøe

Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE img;
  IBAND i1;
  double scale;
  int k, i, b, r;
  char *args;

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-i] [-b] [-r] [-s <scale> | -fftscale <scale>] <inimage> <outimage>\n"));

  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);

  i = read_bswitch(&argc, argv, "-i");
  b = read_bswitch(&argc, argv, "-b");
  r = read_bswitch(&argc, argv, "-r");
  scale = read_dswitch(&argc, argv, "-s", 0.0);
  scale = (scale == 0.0) ? read_dswitch(&argc, argv, "-fftscale", 0.0) : scale;

  if (argc != 3) Usage(2, "Illegal number of arguments.\n");
  img = Iread_image(argv[1]);

  for (k=1; k<=Inbands(img); k++)
    {
      i1 = img[k];
      if (Ipixtyp(i1) != Icomplex_typ && 
	  (Ipixtyp(i1) != Id_complex_typ || r != 0))
	i1 = (IBAND) mkConvertBand(i1, Icomplex_typ);
      if (fft2d(i1, i1, i, scale)) exit(Error(1, "Error in fft-routine\n"));
      img[k] = b ? (IBAND) mkConvertBand(i1, Iu_byte_typ) : i1;
    }
  Ihistory(img, argv[0], args);
  Iwrite_image(img, argv[2]);

  return(0);
}
