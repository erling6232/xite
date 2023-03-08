

/*

________________________________________________________________

        maxHisto
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


#include <stdlib.h>
#include <xite/includes.h>
#include <xite/biff.h>
#include <xite/median.h>
#include <xite/histo.h>
#include <xite/message.h>
#include <xite/readarg.h>

/*P:maxHisto*

________________________________________________________________

		maxHisto
________________________________________________________________

Name:		maxHisto - texture measure

Syntax:		| maxHisto <inimage> <outimage> [<nx> [<ny>]]

Description:    Filters the input image into the output image
                using a local filter with window size 'nx' * 'ny'.
                The number of occurrences of the pixel value
		occurring most frequently in the window becomes
		the new center value.

		Even values of 'nx' or 'ny' are increased by one. Uses
		histogram updating when moving horizontally from pixel to
		pixel. If histogram value is larger than 255, 255 is used.

		Default value for 'nx' is 3. Default value for 'ny' is 'nx'.

Restrictions:   'inimage' must have bands with pixel type unsigned byte.

References:	Lowitz, Gabriel E.: "Can a Local Histogram
                Really Map Texture Information?" Pattern Rec.,
		Vol. 16, pp141-147, 1983.

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Examples:       | maxHisto mona.img monaMH.img
                | maxHisto mona.img monaMH.img 5
		| maxHisto mona.img monaMH.img 5 7
________________________________________________________________

*/

int main(int argc, char **argv)
{
   IMAGE i1, i2;
   int bn, nx, ny;
   char *args;

   Iset_message(TRUE); 
   Iset_abort(TRUE);
   InitMessage(&argc, argv, xite_app_std_usage_text(
	       "Usage: %s <oldfile> <newfile> [<nx> [<ny>]]\n"));

   if (argc == 1) Usage(1, NULL);
   args = argvOptions(argc, argv);

   if (argc < 3) Usage(1, "Illegal number of arguments.\n");
   if (argc >= 4) nx = atoi(argv[3]); else nx = 3;
   if (argc >= 5) ny = atoi(argv[4]); else ny = nx;
   if ((nx < 1) || (ny < 1))
     Usage(2, "Window sizes should be greater than 0.\n");
 
   i1 = Iread_image(argv[1]);
   i2 = Icopy_init(i1);

   for (bn=1; bn <= Inbands(i1); ++ bn)
     if (maxHisto(i1[bn], i2[bn], nx, ny))
       Message(2, "%s: Error in band %d\n", argv[0], bn);

   Ihistory(i2, argv[0], args);
   Iwrite_image(i2,argv[2]);

   return(0);
}
