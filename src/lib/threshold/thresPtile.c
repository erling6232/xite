
/*

________________________________________________________________

        thresPtile
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



#ifndef MAIN

/*F:thresPtile*

________________________________________________________________

		thresPtile
________________________________________________________________

Name:		thresPtile - threshold at specified percentage

Syntax:         | #include <xite/threshold.h>
		|
                | int thresPtile( histogram h, float pTile );
Description:	Calculates the threshold value needed to perform
                thresholding of the image behind the histogram <h>
		according to the <pTile> argument. This argument
		describes the percentage of the pixels that should
		be below the threshold. The actual thresholding
		is not performed, see the routine "threshold".
		
Restrictions:   pTile should be in 0.0 .. 100.0 

Return value:	threshold value. Pixel values up to, and including,
		the return value, is below the threshold.

Author:		Tor L|nnestad, BLAB, ifi, UiO

Id:             $Id$
________________________________________________________________

*/

int thresPtile ( histogram h, float pTile )
{
  int i, sum, limit ;

  if (pTile LT 0.0) return(-1);
  if (pTile GT 100.0) return(255);
  for (sum=0, i=0; i LE 255;  sum += h[i++]) ;
  limit = (int)((sum*pTile/100.0)+0.5);
  for (sum=0, i=0; sum LT limit; sum += h[i++]) ;
  return(--i);
}  /*  thresPtile  */

#endif /* not MAIN */



/*P:thresPtile*

________________________________________________________________

		thresPtile
________________________________________________________________

Name:		thresPtile - threshold at specified percentage

Syntax:		| thresPtile [-l <lpv>] [-h <hpv>]
		|   <inimage> [<outimage>] <pTile>

Description:	'thresPtile' calculates, and eventually applies,
		the threshold needed to threshold the image
		according to the 'pTile' argument, 'pTile' percent 
		of the pixels will be below the threshold, and
		(100 - <pTile>) percent will be above.
		
		| lpv - output for low pixels (below thrs.) Default 0
		| hpv - output for high pixels (above thrs.) Default 255
		| inimage - input image
		| outimage - output image
		| pTile - percent of pixels below or at threshold

		If an output image is given, the input image is
		thresholded and written to the output image.
		Otherwise the threshold value is only reported
		to standard output.

Files:		
See also:	
Diagnostics:	

Restrictions:	| If (pTile < 0.0) pTile is set to 0.0
		| If (pTile > 100.0) pTile is set to 100.0
		| Only UNS_BYTE pixels are supported

Author:		Tor L|nnestad, BLAB, ifi, UiO

Examples:	| thresPtile mona.img 65
		| thresPtile -h 1 mona.img monaThres.img 73.5

Id:             $Id$
________________________________________________________________

*/

#ifdef MAIN

int main(int argc, char **argv)
{
   IMAGE i;
   histogram h;
   int t, bn, lpv, hpv, output=FALSE;
   double pTile;
   char *args;

   Iset_message(TRUE); 
   Iset_abort(TRUE); 
   InitMessage(&argc, argv, xite_app_std_usage_text(
     "Usage: %s thresPtile [-l <lpv>] [-h <hpv>]\n\
       <inimage> [<outimage>] <pTile>\n"));

   if (argc == 1) Usage(1, NULL);
   args = argvOptions(argc, argv);

   lpv = atoi(read_switch(&argc, argv, "-l", TRUE, "0"));
   hpv = atoi(read_switch(&argc, argv, "-h", TRUE, "255"));

   if ((argc < 3) || (argc > 4)) Usage(1, "Illegal number of arguments.\n");

   i = Iread_image(argv[1]);
   if ((output = (argc >= 4))) pTile = atof(argv[3]);
   else pTile = atof(argv[2]);

   for (bn=1; bn LE Inbands(i); ++ bn) {
     mkHisto(i[bn], h);
     t = thresPtile(h, pTile);
     if (output) thresholdSpOut(i[bn], i[bn], t, lpv, hpv);
     else Message(0, "Threshold value in band %d: %d.\n", bn, t);
   }

   if (output) {
     Ihistory(i, argv[0], args); 
     Iwrite_image(i, argv[2]);
   }

   return(0);
}

#endif /* MAIN */
