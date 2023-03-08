
/*

________________________________________________________________

        pseudomedian3x3
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
#include <xite/blab.h>
#include <xite/biff.h>
#include <xite/median.h>
#include <xite/message.h>
#include XITE_MALLOC_H
#include XITE_STDIO_H

/*P:pseudomedian3x3*

________________________________________________________________

		pseudomedian3x3
________________________________________________________________

Name:		pseudomedian3x3 - median like noise reduction filtering

Syntax:		pseudomedian3x3 <input> <output>

Description:    Filters the input image into the output image
                band by band, using a median like filter with
                window size 3 x 3.
                Given the neighborhood a..i :

                | a b c
                | d e f
		| g h i

                First the median of a, b and c is computed, likewise the
                median of d, e and f, and the median of g, h and i. Finally
                the median of these three median values replaces the value e
                in the output band. Corner pixels are copied from input image,
                border pixels are set to the median of the border pixel and
                the two neighboring border pixels.

See also:       median(1)

Restrictions:   'input' must have pixeltype unsigned byte.

Author:		Tor Lønnestad, BLAB, Ifi, UiO.

Examples:       pseudomedian3x3 mona.img monamed.img

Id:             $Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
   IMAGE i1, i2;
   int bn, stat;

   InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <inimage> <outimage> \n"));
   Iset_message(TRUE);
   Iset_abort(TRUE);

   if (argc == 1) Usage(1, NULL);

   if (argc NE 3) Usage(2, "Illegal number of arguments.\n");

   i1 = Iread_image(argv[1]);
   i2 = Icopy_init(i1);

   for (bn=1; bn LE Inbands(i1); bn++)
     if ((stat = pseudomedian3x3(i1[bn],i2[bn])))
       Warning(stat, "Error in band %d\n", bn);

   Ihistory(i2, argv[0], ""); 
   Iwrite_image(i2,argv[2]);

   return(0);
}
