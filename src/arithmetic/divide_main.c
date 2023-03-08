/*C*

________________________________________________________________

        divide
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
#include <xite/message.h>
#include XITE_STDIO_H
#include <stdlib.h>

#ifndef MIN
# define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

/*P:divide*

________________________________________________________________

		divide
________________________________________________________________

Name:		divide - divide two images pixel by pixel

Syntax:		| divide [-s <scale>] [-t <title>]
                |     <input1> <input2> <output>

Description:    'divide' divides, pixel by pixel, for each pair of
                bands, input1 on input2, and multiplies with scale.

		| output(x,y) = input1(x,y) / input2(x,y) * scale

		If divisor is zero, we look at dividend. If dividend
		is zero, result is set to zero, otherwise result is
		set to "max".

		All pixel types are accepted, as long as the two
		input images have the same pixel type for each pair
		of bands. The output image will be of the same type.
		Resulting values outside the domain of the actual
		pixel type is truncated to the nearest legal value.

		Default <scale> value is 1.0. <title> is optional
		title of image. Default is  "Image division".

See also:	multiply(1), scale(1)

Restrictions:   Only the largest rectangle common to each pair of bands
                in the two input images is divided.

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Examples:       | divide mona1.img mona2.img monaDiv.img

Id:             $Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
   IMAGE i1, i2, i3;
   int bands, bn, pt;
   char *args, *title;
   double scale;

   Iset_message(1); 
   Iset_abort(1); 
   InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-s <scale>] [-t <title>] <inimage1> <inimage2> <outimage>\n"));
 
   if (argc == 1) Usage(1, NULL);
   args = argvOptions(argc, argv);

   scale = read_dswitch(&argc, argv, "-s", 1.0);
   scale = read_dswitch(&argc, argv, "-scale", scale);
   title = read_switch(&argc, argv, "-t", 1, "Image division");
   title = read_switch(&argc, argv, "-title", 1, title);

   if (argc < 4) Usage(2, "Illegal number of arguments.\n");

   /* read images */
   i1 = Iread_image(argv[1]);
   i2 = Iread_image(argv[2]);
   if ((!i1) || (!i2)) exit(2); 
   bands = MIN(Inbands(i1),Inbands(i2));
   i3 = Init_image(bands, title);
   if (!i3) Error(2, "Error in initializing image structure.\n");

   for (bn=1; bn <= bands; ++bn) {
     pt = Ipixtyp(i1[bn]);

     if (pt != Ipixtyp(i2[bn])) {
       Error(3, "Different pixel types in band %d\n", bn);
     } else {
       i3[bn] = Imake_band(pt,
         MIN(Ixsize(i1[bn]),Ixsize(i2[bn])),
         MIN(Iysize(i1[bn]),Iysize(i2[bn])));

       if (divide(i1[bn], i2[bn], i3[bn], scale))
         Error(4, "Error in division of band %d\n", bn);
     }
   }

   Ihistory(i3, argv[0], args);
   Iwrite_image(i3, argv[3]);

   return(0);
}
