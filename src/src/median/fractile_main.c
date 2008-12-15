
/*

________________________________________________________________

        fractile
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
#include <xite/median.h>
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/histo.h>
#include <xite/message.h>

/*P:fractile*

________________________________________________________________

		fractile
________________________________________________________________

Name:		fractile - noise reduction filtering

Syntax:		fractile <inimage> <outimage> fractile [<dx> [<dy>]]

Description:    Fractile noise filtering with window 'dx' * 'dy'
                using the histogram updating technique.

		Sorts all pixels within the window and picks the 
		'fractile' pixel. 'fractile = 1/dx/dy' gives min-filter,
		'fractile = 0.5' gives median filter, and 
		'fractile = 1.0-1/dx/dy' gives max-filter.

		'fractile' must be non-negative and maximum 1.0.
		Default value for 'dx' is 3, for 'dy' it is 'dx'.

Reference:      Huang, Yang and Tang:
		"A Fast Two-Dimensional Median Filtering Algorithm"
		IEEE Trans. Ac., Speech, and Signal Proc, Vol ASSP-27,
		No.1, Feb.1979

See also:	pseudomedian3x3(1), median(1), minarea(1), maxarea(1)

Restrictions:   Accepts only images with pixel type unsigned byte.
                'dx' and 'dy' must be larger than 0. 

Author:		Tor Lønnestad, BLAB, Ifi, UiO
Modified:	Otto Milvang, BLAB, Ifi, UiO

Examples:       | fractile mona.img mona.30.img 0.3
                | fractile mona.img mona.40.img 0.4 7
		| fractile mona.img mona.60.img 0.6 7 5

Id:             $Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
   IMAGE i1,i2;
   int bn, dx, dy;
   char arg[200];
   double frac;

   InitMessage(&argc, argv, xite_app_std_usage_text(
	       "Usage: %s <inimage> <outimage> fractile [<dx> [<dy>]] \n"));
   Iset_message(TRUE); 
   Iset_abort(TRUE);
   if ((argc < 4) OR (argc GT 6)) Usage(0, NULL);

   frac = atof(argv[3]);
   if ((frac LT 0.0) OR (frac > 1.0)) 
     Error(2, "Frac should be in the range 0.0 .. 1.0\n");
   if (argc GE 5) dx = atoi(argv[4]); else dx = 3;
   if (argc GE 6) dy = atoi(argv[5]); else dy = dx;
   if ((dx LT 1) OR (dy LT 1))
     Error(2, "Window size should be greater than 0 \n");
 
   i1 = Iread_image(argv[1]);
   i2 = Icopy_init(i1);

   for (bn=1; bn LE Inbands(i1); INC bn)
     if (fractile(i1[bn], i2[bn], dx, dy, frac))
       fprintf(stderr, "%s: Error in band %d\n", argv[0], bn);

   sprintf(arg, " with frac = %5.2f, dx = %d, dy = %d", frac, dx, dy);
   Ihistory(i2, argv[0], arg);
   Iwrite_image(i2,argv[2]);
   return(0);
}
