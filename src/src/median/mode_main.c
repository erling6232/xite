
/*

________________________________________________________________

        mode
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
#include <xite/readarg.h>

#ifndef MAX
# define MAX(a,b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef MIN
# define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

/*P:mode*

________________________________________________________________

		mode
________________________________________________________________

Name:		mode - noise removal and edge sharpening

Syntax:		| mode <inimage> <outimage> [<dx> [<dy>]]

Description:    'mode' noise filtering with window 'dx' x 'dy'
                using histogram updating technique. The pixel
                value occurring most frequently within the window
		becomes the new pixel value in the window center.

		'dx' is the horisontal window size, default value 3.
		'dy' is the vertical window size, default value 'dx'.

Restrictions:   Input image bands must have pixel type unsigned byte.

See also:	median(1), pseudomedian3x3(1), sigma(1), snn(1), knn(1),
                kncn(1), convolve(1)

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Examples:       | mode mona.img monamode.img
                | mode mona.img monamode.img 5
		| mode mona.img monamode.img 5 7

Id:             $Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
   IMAGE i1, i2;
   int bn, dx, dy;
   char *args;

   Iset_message(TRUE); 
   Iset_abort(TRUE);
   InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <inimage> <outimage> [<dx> [<dy>]]\n"));

   if (argc == 1) Usage(1, NULL);
   args = argvOptions(argc, argv);
   if (argc < 3) Usage(1, "Illegal number of arguments.\n");

   if (argc GE 4) dx = atoi(argv[3]); else dx = 3;
   if (argc GE 5) dy = atoi(argv[4]); else dy = dx;
   if ((dx LT 1) OR (dy LT 1))
     Error(2, "Window sizes should be greater than 0.\n");
 
   i1 = Iread_image(argv[1]);
   i2 = Icopy_init(i1);

   for (bn=1; bn LE Inbands(i1); INC bn)
     if (mode(i1[bn], i2[bn], dx, dy))
       Error(2, "Error in band %d\n", bn);

   Ihistory(i2, argv[0], args); 
   Iwrite_image(i2,argv[2]);

   return(0);
}
