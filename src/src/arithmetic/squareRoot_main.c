
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

/*P:squareRoot*

________________________________________________________________

		squareRoot
________________________________________________________________

Name:		squareRoot - take the square root of each pixel value

Syntax:		| squareRoot [-s <scale>] <input> <output>

Description:    'squareRoot' takes the square root of every pixel in
                the input image according to the formula:

		| output(x,y) = sqrt(input(x,y)) * scale

		Accepts all pixel types except complex and double complex. If
		the result is outside the domain of the actual pixel type, the
		nearest possible value is returned. Output image will have the
		same pixel type as input.

Options:        &-s scale
		Default: 1.0.

See also:	squareRoot(3), square(1), absValue(1), power(1), phase(1),
                real(1), imag(1), scale(1), multiply(1), divide(1),
                biffConvert(1)

Author:		Tor Lønnestad, BLAB, ifi, UiO

Examples:       | squareRoot mona.img sqrtMona.img 
		| squareRoot -scale 10.0 mona.img sqrtMona.img

Id:             $Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
   IMAGE img;
   int nbands, bn;
   char *args;
   double scale;

   Iset_message(1); 
   Iset_abort(1);
   InitMessage(&argc, argv, xite_app_std_usage_text(
     "Usage: %s [-s <scale>] <inimage> <outimage>\n"));

   if (argc == 1) Usage(1, NULL);
   args  = argvOptions(argc, argv);

   scale = read_dswitch(&argc, argv, "-scale", 1.0);
   scale = read_dswitch(&argc, argv, "-s", scale);

   if (argc != 3) Usage(1, "Illegal number of arguments.\n");

   /* read images */
   img = Iread_image(argv[1]);
   nbands = Inbands(img);

   for (bn=1; bn <= nbands; ++bn)
     if (squareRoot(img[bn], img[bn], scale))
       Error(2, "Error in band %d\n", bn);

   Ihistory(img, argv[0], args);
   Iwrite_image(img, argv[2]);

   return(0);
}
