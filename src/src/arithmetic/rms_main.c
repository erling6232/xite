
/*

________________________________________________________________

        rms
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
#include <stdlib.h>
#include <xite/includes.h>
#include <xite/biff.h>
#include <xite/arithmetic.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include XITE_STDIO_H

#ifndef MIN
# define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

/*P:rms*

________________________________________________________________

		rms
________________________________________________________________

Name:		rms - root mean square difference between two images

Syntax:		rms <inimage1> <inimage2> [<edge>]

Description:	'rms' calculates the root mean square difference
		between corresponding pairs of bands and writes
		the result to stdout.

		rms = SQRT{ SUM [ |inp1[y][x] - inp2[y][x]| **2 ] / N }

		where SUM is over all N pixels in the band minus an edge of
                width 'edge' which is not included in the calculaton. The
                purpose of this is to exclude differences due to different
                processing of the edge by local filters. Default edge value is
                0.

		All pixeltypes are allowed, but corresponding bands in
		'inimage1' and 'inimage2' must be of the same pixeltype.

See also:	rms(3), absDiff(1), signDiff(1)

Author:		Tor Lønnestad, BLAB, Ifi, UiO.

Examples:	| rms monaMed.img monaAver.img 1

Id:             $Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
   IMAGE i1, i2;
   int bn, edge, maxbn;
   double rmss;
   char *args;

   Iset_message(1);
   Iset_abort(1);
   InitMessage(&argc, argv, xite_app_std_usage_text(
     "Usage: %s <image1> <image2> [<edge>]\n"));

   if (argc == 1) Usage(1, NULL);
   args = argvOptions(argc, argv);
   if (argc < 3 || argc > 5) Usage(2, "Illegal number of arguments.\n");

   edge = (argc == 3) ? 0 : atoi(argv[3]);
   i1 = Iread_image(argv[1]);
   i2 = Iread_image(argv[2]);
   maxbn = MIN(Inbands(i1),Inbands(i2));

   for (bn=1; bn <= maxbn; bn++) {
     rmss = rms(i1[bn], i2[bn], edge);
     printf(" RMS of band %d : %.2f\n", bn, rmss);
   }

   return(0);

}  /* main */
