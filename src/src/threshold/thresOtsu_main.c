
/*

________________________________________________________________

        thresOtsu
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
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/histo.h>
#include <xite/readarg.h>
#include <xite/message.h>
#include <xite/threshold.h>



/*P:thresOtsu*

________________________________________________________________

		thresOtsu
________________________________________________________________

Name:		thresOtsu - threshold by the method of Otsu

Syntax:		| thresOtsu [-l <lpv>] [-h <hpv>] <inimage> [<outimage>]

Description:	'thresOtsu' calculates, and eventually applies, the
		threshold value by using the method of Otsu. The
		method attempts to maximize the between-class variance
		between the class of pixels above the threshold, and
		the class of pixels below.

                If an output image is given, the input image is
                thresholded and written to the output image.
                Otherwise the threshold value is only reported
                to standard output.

Options:        &-l lpv
                Output for low pixels (below threshold). Default 0.

                &-h hpv
		Output for high pixels (above threshold). Default 255.

Reference:	&Otsu, N.
                "A Threshold Selection Method from Gray-Level histograms"
		IEEE Trans. Syst., Man and Cyb.,
		Vol SMC-9, No.1, pp.62-66.

See also:	thresOtsu(3), threshold(1), thresPtile(1),
                thresRidCal(1),	thresLloyd(1)

Restrictions:   'inimage' must have bands with pixel type unsigned byte.

Author:         Tor Lønnestad, BLAB, Ifi, UiO

Examples:       | thresOtsu mona.img
                | thresOtsu -h 1 mona.img monaThres.img

Id:             $Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
   IMAGE input, output;
   histogram h;
   int t, bn, lpv, hpv;
   char *args;

   Iset_message(TRUE); 
   Iset_abort(TRUE); 
   InitMessage(&argc, argv, xite_app_std_usage_text(
     "Usage: %s  [-l <lpv>] [-h <hpv>] <inimage> [<outimage>]\n"));

   if (argc == 1) Usage(1, NULL);
   args = argvOptions(argc, argv);

   lpv = atoi(read_switch(&argc, argv, "-l", TRUE, "0"));
   hpv = atoi(read_switch(&argc, argv, "-h", TRUE, "255"));

   if (argc < 2) Usage(1, "Illegal number of arguments.\n");

   input = Iread_image(argv[1]);
   if (argc >= 3) output = Icopy_init(input); else output = 0; 

   for (bn=1; bn <= Inbands(input); ++ bn) {
     mkHisto(input[bn], h);
     t = thresOtsu(h);
     if (output) thresholdSpOut(input[bn], output[bn], t, lpv, hpv);
     else Message(0, "Threshold value in band %d: %d.\n", bn, t);
   }
   if (output) {
     Ihistory(output, argv[0], args);
     Iwrite_image(output,argv[2]);
   }

   return(0);
}
