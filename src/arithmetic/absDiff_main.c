
/*

________________________________________________________________

        absDiff
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


/*I*
________________________________________________________________

		include files
________________________________________________________________
*/

#include <xite/includes.h>
#include <xite/biff.h>
#include <xite/arithmetic.h>
#include <xite/message.h>
#include <xite/readarg.h>

#ifndef MIN
# define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

/*P:absDiff*

________________________________________________________________

		absDiff
________________________________________________________________

Name:		absDiff - absolute difference between two images

Syntax:		absDiff <inimage1> <inimage2> <outimage>

Description:    'absDiff' calulates the absolute value of the grey
                level difference between two input images, and
		assigns the result to the output image.

                If the result is outside the pixeltype range, the result is
                truncated.

See also:	absDiff(3), signDiff(1), addw(1), multiply(1), divide(1)

Restrictions:   Accepts all pixel types, as long as corresponding
		pairs of bands have the same pixel type.

Author:		Tor L|nnestad, BLAB, ifi, UiO

Examples:       | absDiff mona1.img mona2.img monaDiff.img

Id:             $Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
   IMAGE i1, i2, i3;
   int bands, bn;
   char *args, buf[100];

   InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <input1> <input2> <output>\n"));
   Iset_message(1);

   if (argc == 1) Usage(1, NULL);
   args = argvOptions(argc, argv);
   if (argc != 4) Usage(2, "Illegal number of arguments.\n");
 
   i1    = Iread_image(argv[1]);
   i2    = Iread_image(argv[2]);
   i3    = Icopy_init(i1);

   Isetpos(i3, 1); Idel_text(i3);
   sprintf(buf, "%% %s input one:", argv[0]); Iappend_line(i3, buf);
   if (Icopy_text(i1, i3) != Iok)
     Error(3, "Error in copying text field of input one.\n");
   sprintf(buf, "%% %s input two:", argv[0]); Iappend_line(i3, buf);
   if (Icopy_text(i2, i3) != Iok)
     Error(3, "Error in copying text field of input two.\n");
   sprintf(buf, "%% %s output:", argv[0]);
   Iappend_line(i3, buf);

   bands = MIN(Inbands(i1),Inbands(i2));

   for (bn=1; bn <= bands; ++bn)
     absDiff(i1[bn], i2[bn], i3[bn]);

   Ihistory(i3, argv[0], args);
   Iwrite_image(i3,argv[3]);

   return(0);
}
