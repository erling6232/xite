
/*

________________________________________________________________

        minarea
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
#include <xite/minmax.h>
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/message.h>
#include <xite/readarg.h>

/*P:minarea*

________________________________________________________________

		minarea
________________________________________________________________

Name:		minarea - min operator over a local window

Syntax:		| minarea <inimage> <outimage> [<dx> [<dy>]]

Description:	'minarea' performs a min operation on the input 
                image 'inimage' and writes the result to 'outimage'.
                The size of the local window is 'dx' * 'dy'. If only
		one is given, the area is assumed to be quadratic.
		If none, area is set to 3x3.

Restrictions:   The bands of 'inimage' must have pixel type unsigned byte.

Return value:	0 if ok, positive otherwise

See also:	maxarea(1)

Author:		Otto Milvang & Tor Lønnestad, BLAB, Ifi, UiO

Examples:	| minarea mona.img monamin.img 
                | minarea mona.img monamin.img 7
                | minarea mona.img monamin.img 3 5

Id:             $Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
   IMAGE img1, img2;
   int bn, dx, dy, stat;
   char *args;

   Iset_message(TRUE);
   Iset_abort(TRUE);
   InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <inimage> <outimage> [<dx> [<dy>]]\n"));

   if (argc == 1) Usage(1, NULL);
   args = argvOptions(argc, argv);
   if ((argc LT 3) OR (argc GT 5)) Usage(2, "Illegal number of arguments.\n");

   if (argc GE 4) dx = atoi(argv[3]); else dx = 3;
   if (argc GE 5) dy = atoi(argv[4]); else dy = dx;

   if ((img1 = Iread_image(argv[1]))) {
     img2 = Icopy_init(img1);
     for (bn=1; bn LE Inbands(img1); bn++)
       if ((stat = minarea(img1[bn],img2[bn],dx,dy)))
         Warning(stat, "Error in band %d\n", bn);

     Ihistory(img2, argv[0], args);
     Iwrite_image(img2,argv[2]);
   } else {
     Error(5, "Can't read input file %s\n", argv[1]);
   }

   return(0);
}  /* main */
