
/*

________________________________________________________________

        thresRidCal
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
#include <xite/message.h>
#include <xite/readarg.h>
#include <xite/threshold.h>



/*P:thresRidCal*

________________________________________________________________

		thresRidCal
________________________________________________________________

Name:		thresRidCal - threshold by the Ridler and Calvard method

Syntax:		| thresRidCal [-l <lpv>] [-h <hpv>]
		|   <inimage> [<outimage>]

Description:	'thresRidCal' calculates, and eventuall applies,
		the threshold needed to threshold 'inimage' 
                according to the method of Ridler and Calvard.
		
		| lpv - output for low pixels (below thrs.) Default 0
		| hpv - output for high pixels (above thrs.) Default 255
		| inimage - input image
		| outimage - eventual output image

		If an output image is given, the input image is
		thresholded and written to the output image.
		Otherwise the threshold value is only reported
		to standard output.

Files:		
See also:	
Diagnostics:	

Restrictions:	Only UNS_BYTE pixels are supported

Author:		Tor L|nnestad, BLAB, ifi, UiO

Examples:	| thresRidCal mona.img
		| thresRidCal -h 1 mona.img monaThres.img

Id:             $Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
   IMAGE i;
   histogram h;
   int t, bn, lpv, hpv, output;
   char *args;

   InitMessage(&argc, argv, xite_app_std_usage_text(
     "Usage: %s [-l <lpv>] [-h <hpv>] <inimage> [<outimage>]\n"));
   Iset_message(TRUE);
   Iset_abort(TRUE);

   if (argc == 1) Usage(1, NULL);
   args = argvOptions(argc, argv);

   lpv = atoi(read_switch(&argc, argv, "-l", TRUE, "0"));
   hpv = atoi(read_switch(&argc, argv, "-h", TRUE, "255"));

   if (argc > 3) Usage(2, "Illegal number of arguments.\n");

   i = Iread_image(argv[1]);
   output = (argc GE 3);

   FOR (bn=1; bn LE Inbands(i); bn++)
     mkHisto(i[bn], h);
     t = thresRidCal(h);
     if (output) thresholdSpOut(i[bn], i[bn], t, lpv, hpv);
     else fprintf(stderr, "Threshold value in band %d : %d\n", bn, t);
   ENDFOR;

   IF (output)
     Ihistory(i, argv[0], args);
     Iwrite_image(i, argv[2]);
   ENDIF;

   return(0);
}
