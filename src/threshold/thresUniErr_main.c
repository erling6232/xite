
/*

________________________________________________________________

        thresUniErr
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
#include <xite/includes.h>
#include <xite/biff.h>
#include <stdlib.h>
#include XITE_STDIO_H
#include <xite/blab.h>
#include <xite/readarg.h>
#include <xite/message.h>
#include <xite/threshold.h>
#include XITE_STRING_H



/*P:thresUniErr*

________________________________________________________________

		thresUniErr
________________________________________________________________

Name:		thresUniErr - Uniform Error Threshold

Syntax:		| thresUniErr [-<sw>] <inimage> [<outimage>]

Description:	'thresUniErr' calculates, and eventually applies,
		the threshold value needed to threshold the 
                image according to the uniform error thresholding metod
                as described in Dunn & Harwood & Davis.

		If an output image is given, the input image is
		thresholded and written to the output image.
		Otherwise the threshold value is only reported
		to standard output.

		Switches:
		| -lpv <lpv>  - low pixel value, default 0
		| -hpv <hpv>  - high pixel value, default 255

Reference:      | S. M. Dunn & D. Harwood & L. S. Davis:
                | "Local Estimation of the Uniform Error Threshold"
		| IEEE Trans. on PAMI, Vol PAMI-6, No 6, Nov 1984.
 
Restrictions:	Only UNS_BYTE pixels are supported

Bugs:       	It only works well on images whith large objects.

Author:		Olav Borgli, BLAB, ifi, UiO

Examples:	| thresUniErr mona.img 
                | thresUniErr mona.img monaThres.img

Id:             $Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
   IMAGE img;
   int t, bn, lpv, hpv, output;
   char *args;

   Iset_message(TRUE); 
   Iset_abort(TRUE);
   InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-<sw>] %s <inimage> [<outimage>]\n"));

   if (argc == 1) Usage(1, NULL);
   args = argvOptions(argc, argv);

   lpv = read_iswitch(&argc, argv, "-lpv", 0);
   hpv = read_iswitch(&argc, argv, "-hpv", 255);

   if ((argc < 2) OR (argc > 3)) Usage(1, "Illegal number of arguments.\n");

   img = Iread_image(argv[1]);
   output = (argc == 3);
   if (output) Ihistory(img, argv[0], args);

   for (bn=1; bn<=Inbands(img); bn++) {
     t = thresUniErr(img[bn]);
     if (output) {
       thresholdSpOut(img[bn], img[bn], t, lpv, hpv);
     }
     else printf("Threshold value in band %d : %d\n", bn, t);
   }
   if (output) Iwrite_image(img, argv[2]);

   return(0);
}
