
/*

________________________________________________________________

        thresMinErr
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
#include <xite/histo.h>
#include <xite/readarg.h>
#include <xite/message.h>
#include <xite/threshold.h>
#include XITE_STRING_H



/*P:thresMinErr*

________________________________________________________________

		thresMinErr
________________________________________________________________

Name:		thresMinErr - minimum error threshold.

Syntax:		| thresMinErr [<switches>] <inimage> [<outimage>]

Description:	'thresMinErr' calculates, and eventually applies,
		the threshold value needed to threshold the 
                image according to the minimum error thresholding metods
                as described in Kittler & Illingworth and Gonzalez & Wintz.

		| -iter      - use iterative methode
		|              (thresMinErrIter)
		| -crit      - use criterion function method
		|              (thresMinErrCrit)
		| -eqVar     - use equal variances method
		|              (thresMinErrEqVar)

		| -mean      - initial threshold is mean (default)
		| -t <tinit> - initial threshold 0<=tinit<=255
		| -ridcal    - initial threshold = thresRidCal(h)

		| -l <lpv>   - output for low pixels (below thrs.).
		|              Default 0
		| -h <hpv>   - output for high pixels (above thrs.).
		|              Default 255

		| inimage  - input image
		| outimage - output image

		If an output image is given, the input image is
		thresholded and written to the output image.
		Otherwise the threshold value is only reported
		to standard output.

Reference:      | J. Kittler & J. Illingworth: "Minimum Error Thresholding"
                | Pattern Recognition, Vol 19, nr 1. 1986, pp. 41-47.
		| Gonzales, Rafael C & Wintz, Paul:
		| "Digital Image Processing"
                | Addison-Wesley, 1977.
		| Q.-Z. Ye & P.-E. Danielsson:
		| "On Minimum Error Thresholding and Its Implementations"
		| Pattern Recognition Letters 7,
		| 1988, pp. 201-206.
 
Restrictions:	Only UNS_BYTE pixels are supported

Author:		Olav Borgli, BLAB, ifi, UiO

Examples:	| thresMinErr -iter -mean mona.img monaThres.img
		| thresMinErr -iter mona.img monaThres.img
		| thresMinErr -crit mona.img monaThres.img
		| thresMinErr -eqVar -t 112 mona.img

Id:             $Id$
________________________________________________________________
*/

int main(int argc, char **argv)
{
   IMAGE i;
   histogram h;
   int t, tinit, bn, lpv, hpv, output, j;
   int f_iter, f_crit, f_eqVar, f_mean, f_ridcal;
   char *args;

   Iset_message(TRUE); 
   Iset_abort(TRUE);
   InitMessage(&argc, argv, xite_app_std_usage_text(
     "Usage: %s [-iter] [-crit] [-eqVar] [-mean] [-t <tinit>] [-ridcal]\n\
       [-l <lpv>] [-h <hpv>] <inimage> [<outimage>]\n"));

   if (argc == 1) Usage(1, NULL);
   args = argvOptions(argc, argv);

   lpv      = read_iswitch(&argc, argv, "-l", 0);
   hpv      = read_iswitch(&argc, argv, "-h", 255);

   f_iter   = read_bswitch(&argc, argv, "-iter");
   f_crit   = read_bswitch(&argc, argv, "-crit");
   f_eqVar  = read_bswitch(&argc, argv, "-eqVar");

   f_mean   = read_bswitch(&argc, argv, "-mean");
   tinit    = read_iswitch(&argc, argv, "-t", -1);
   f_ridcal = read_bswitch(&argc, argv, "-ridcal");

   if ((argc < 2) OR (argc > 3)) Usage(1, "Illegal number of arguments.\n");

   /* Choose what initial threshold value to use */
   if (tinit != (-1)) {
     if (tinit<0 || tinit>255) {
       Error(2, "-t value was out of bounds.\n");
     }
   }
   else if (f_ridcal)  tinit=(-2);
   else tinit=(-1);

   i = Iread_image(argv[1]);
   output = (argc == 3);
   if (output) Ihistory(i, argv[0], args);

   for (bn=1; bn<=Inbands(i); INC bn) {
     mkHisto(i[bn], h);
     if      (f_crit)   t = thresMinErrCrit(h);
     else if (f_iter)   t = thresMinErrIter(h,tinit);
     else if (f_eqVar)  t = thresMinErrEqVar(h,tinit);
     else {
       Error(2, "No method selected.\n");
     }
     if (output) {
       thresholdSpOut(i[bn], i[bn], t, lpv, hpv);
     }
     else Message(0, "Threshold value in band %d: %d\n", bn, t);
   }
   if (output) Iwrite_image(i, argv[2]);
   return(0);
 }
