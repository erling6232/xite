

/*

________________________________________________________________

        lit
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

/*P:lit*

________________________________________________________________

		lit
________________________________________________________________

Name:		lit - Local Information Transform texture measure

Syntax:		lit <inimage> <outimage> <dp> [<dx> [<dy>]]

Description:    lit transform with window size 'dx' * 'dy' using
                the histogram updating technique. lit counts the 
		number of pixels, inside the 'dx' * 'dy' filtering 
		window, having a pixel value in the interval
		cp-dp..cp+dp, where cp is the value of the
		center pixel, and 'dp' is the delta pixel value
		argument.

		'dx' is the horizontal window size. Default value 3.
		'dy' is the vertical window size. Default value 'dx'.

Restrictions:   Input image must have bands with pixel type unsigned byte.
                'dx', 'dy' must be larger than 0, but not larger than the
		input image size. 'dp' must be 0..255.

Reference:      Lowitz, Gabriel E.: "Extracting the Local 
		Information Content of a Spatial Image"
		in Proc. 7.ICPR, Montral, Canada, Aug. 1984,
		1266-1268.

See also:	litSnn(1)

Author:		Tor Lønnestad, BLAB, Ifi, UiO.

Examples:       | lit mona.img monalit.img 0
                | lit mona.img monalit.img 5 7
		| lit mona.img monalit.img 5 7 11

Id:             $Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
   IMAGE i1,i2;
   int bn, dx, dy, dp;
   char *args;

   Iset_message(TRUE); 
   Iset_abort(TRUE);
   InitMessage(&argc, argv, xite_app_std_usage_text(
     "Usage: %s <inimage> <outimage> <dp> [<dx> [<dy>]]\n"));

   if (argc == 1) Usage(1, NULL);
   args = argvOptions(argc, argv);
   if ((argc < 4) OR (argc GT 6)) Usage(1, "Illegal number of arguments.\n");

   dp = atoi(argv[3]);
   if (argc GE 5) dx = atoi(argv[4]); else dx = 3;
   if (argc GE 6) dy = atoi(argv[5]); else dy = dx;
   if ((dp LT 0) OR (dp GT 255))
     Error(2, "Delta pixel value 'dp' must be 0..255.\n");

   if ((dx LT 1) OR (dy LT 1))
     Error(2, "Window size should be greater than 0.\n");
 
   i1 = Iread_image(argv[1]);
   i2 = Icopy_init(i1);

   for (bn=1; bn LE Inbands(i1); INC bn)
     if (lit(i1[bn], i2[bn], dx, dy, dp))
       Error(2, "Error in band %d\n", bn);

   Ihistory(i2, argv[0], args);
   Iwrite_image(i2,argv[2]);

   return(0);
}
