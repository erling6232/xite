
/*

________________________________________________________________

        square
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

/*P:square*

________________________________________________________________

		square
________________________________________________________________

Name:		square - take the square of each pixel value

Syntax:		| square [-s <scale>] <input> <output>

Description:    'square' squares the input image band by band,
		according to the formula:

		| output(x,y) = input(x,y) * input(x,y) * scale

		Accepts all pixel types. If the result is outside
		the domain of the actual pixel type, the nearest
		possible value is returned. Output image will have
		the same pixel type as input.

Options:        &-s scale
		Default: 1.0

See also:	square(3), squareRoot(1), phase(1), power(1), absValue(1),
                imag(1), real(1), scale(1), multiply(1), divide(1),
		biffConvert(1)

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Examples:       | square mona.img sqmona.img 
		| square -scale 0.05 mona.img sqmona.img

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
    "Usage: %s [-s <scale>] <input> <output>\n"));

   if (argc == 1) Usage(1, NULL);
   args  = argvOptions(argc, argv);

   scale = read_dswitch(&argc, argv, "-scale", 1.0);
   scale = read_dswitch(&argc, argv, "-s", scale);

   if (argc != 3) Usage(2, "Illegal number of arguments.\n");

   /* read images */
   img = Iread_image(argv[1]);
   nbands = Inbands(img);

   for (bn=1; bn <= nbands; ++bn)
     if (square(img[bn], img[bn], scale))
       Warning(3, "Error in band %d\n", bn);

   Ihistory(img, argv[0], args);
   Iwrite_image(img, argv[2]);

   return(0);
}
