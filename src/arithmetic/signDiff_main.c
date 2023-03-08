
/*

________________________________________________________________

        signDiff
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
#include XITE_STDIO_H
#include <xite/readarg.h>
#include <stdlib.h>
#include <xite/message.h>

#ifndef MIN
# define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

/*P:signDiff*

________________________________________________________________

		signDiff
________________________________________________________________

Name:		signDiff - signed difference between two images

Syntax:		| signDiff <input1> <input2> <output>
                |        [-scale <f>] [-offset <r>]

Description:    'signDiff' calulates the signed difference between
                two input images pixel by pixel:

		| output = (input1 - input2)*scale + offset

		For complex and double-complex bands, the result is

		| output(x,y).re =
		|     (input1(x,y).re-input2(x,y).re)*scale + offset
		| output(x,y).im =
		|     (input1(x,y).im-input2(x,y).im)*scale + offset

                If the result is outside the pixeltype range, the result is
                truncated. Within the pixeltype range, the result is
		rounded to the nearest possible pixelvalue.

		Default argument values:
		| scale = 1.0
		| offset = 0.0

Restrictions:   Corresponding bands in each input image must be of the
                same type. Only the largest rectangle common to two
		corresponding bands is processed.

See also:       signDiff(3), absDiff(1), addw(1), multiply(1), divide(1)

Author:		Tor Lønnestad, BLAB, ifi, UiO
Revised:        Svein Bøe, Ifi, UiO. Implemented for all pixel types, not
                only unsigned byte.

Examples:       | signDiff mona1.img mona2.img monaDiff.img
                | signDiff m1.img m2.img mdiff.img -scale 1.0 -offset 0

Id:             $Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
   IMAGE i1, i2, i3;
   int bands, bn;
   double scale, offset;
   char *args;

   Iset_message(TRUE); 
   Iset_abort(TRUE);

   InitMessage(&argc, argv, xite_app_std_usage_text(
     "Usage: %s [<option>...] <input1> <input2> <output>\n\
       where <option> is chosen from\n\
         -scale <f>  : floating point scale-factor (default 1.0)\n\
         -offset <n> : floating point offset (default 0.0)\n"));

   if (argc == 1) Usage(1, NULL);
   args = argvOptions(argc, argv);

   scale  = read_dswitch(&argc, argv, "-scale",  1.0);
   offset = read_dswitch(&argc, argv, "-offset", 0.0);

   if (argc != 4) Usage(2, "Illegal number of arguments.\n");

   i1 = Iread_image(argv[1]);
   i2 = Iread_image(argv[2]);
   i3 = Icopy_init(i1);
   bands = MIN(Inbands(i1),Inbands(i2));

   for (bn=1; bn <= bands; ++bn) {
     if (signDiff(i1[bn], i2[bn], i3[bn], scale, offset)) {
       fprintf(stderr, "Error in band %d\n", bn);
       exit(1);
     }
   }

   Ihistory(i3, argv[0], args);
   Iwrite_image(i3, argv[3]);

   return(0);
}
