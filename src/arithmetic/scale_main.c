
/*

________________________________________________________________

        scale
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
#include <xite/arithmetic.h>
#include <xite/readarg.h>
#include <xite/message.h>
#include XITE_STDIO_H

#ifndef MIN
# define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

/*P:scale*

________________________________________________________________

		scale
________________________________________________________________

Name:		scale - linear scaling of pixel values

Syntax:		| scale [-scale <scale>] [-offset <offset>] <input>
                |       <output>

Description:    'scale' scales the input image band by band, according
		to the formula:

		| output(x,y) = scale*input(x,y) + offset

		Accepts all pixel types. If the result is outside
		the domain of the actual pixel type, the nearest
		possible value is returned.

Options:        &-scale scale
                Default: 1.0.

		&-offset offset
		Default: 0.0.

See also:	biffConvert(1)

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Examples:       | scale -scale 0.5 mona.img darkmona.img 
		| scale -scale 0.5 -offset 100 mona.img lightmona.img
		| scale -scale -1.0 -offset 255.0 mona.img negmona.img

Id:             $Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{

   IMAGE img;
   int nbands, bn;
   char *args;
   double Scale, offset;

   Iset_message(TRUE); 
   Iset_abort(TRUE);
   InitMessage(&argc, argv, xite_app_std_usage_text(
     "Usage: %s [-scale <scale>] [-offset <offset>] <inimage> <outimage>\n"));

   if (argc == 1) Usage(1, NULL);
   args = argvOptions(argc, argv);

   Scale  = read_dswitch(&argc, argv, "-scale",  1.0);
   offset = read_dswitch(&argc, argv, "-offset", 0.0);

   if (argc != 3) Usage(1, "Illegal number of arguments.\n");

   /* read images */
   img    = Iread_image(argv[1]);
   nbands = Inbands(img);

   for (bn=1; bn <= nbands; ++bn)
     if (scale(img[bn], img[bn], Scale, offset))
       Error(2, "Error in band %d\n", bn);

   Ihistory(img, argv[0], args);
   Iwrite_image(img, argv[2]);

   return(0);
}
