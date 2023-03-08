
/*

________________________________________________________________

        gradInv
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
#include <xite/includes.h>
#include <xite/biff.h>
#include <xite/gradInv.h>
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/message.h>
#include <xite/readarg.h>

#define NIL 0
#ifndef MAX
# define MAX(a,b) ((a)>(b) ? a : b)
#endif
#ifndef MIN
# define MIN(a,b) ((a)<(b) ? a : b)
#endif

/*P:gradInv*

________________________________________________________________

		gradInv
________________________________________________________________

Name:		gradInv - gradient invers noise reduction

Syntax:		gradInv <inimage> <outimage> [<dx> [<dy>]]

Description:    Noise reduction by gradient invers smoothing.

		'dx' is the horizontal window size. Default is 3.
		'dy' is the vertical window size. Default is 'dx'.

                The image border is filtered as good as possible.

Restrictions:   'input' and 'output' must have pixel type unsigned byte.
                'dx' and 'dy' must be greater than 1.
		If 'dx' (or 'dy') are even, 'dx'+1 (or 'dy'+1) is used.

See also:	gradInv(3)

References:     &Wang, Vagnucci and Li,
                "Gradient Inverse Weighted Smoothing Scheme and the Evaluation
		of Its Performance",
		Comp. Graph. and Im. Proc. '15', 167-181 (1981).

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Examples:       | gradInv mona.img monaGrad.img
                | gradInv mona.img monaGrad.img 5
                | gradInv mona.img monaGrad.img 5 7

Id:             $Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
   IMAGE input, output;
   int bn, dx, dy;
   char *args;

   Iset_message(TRUE);
   Iset_abort(TRUE);
   InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <inimage> <outimage> [<dx> [<dy>]]\n"));

   if (argc == 1) Usage(1, NULL);
   args = argvOptions(argc, argv);

   if ((argc < 3) OR (argc > 5)) Usage(1, "Illegal number of arguments.\n");

   input = Iread_image(argv[1]);
   if (argc GE 4) dx = atoi(argv[3]); else dx = 3;
   if (argc GE 5) dy = atoi(argv[4]); else dy = dx;

   output = Icopy_init(input);
   for (bn=1; bn LE Inbands(input); INC bn)
     if (gradInv(input[bn], output[bn], dx, dy))
       Error(2, "Error in gradInv() for band %d.\n", bn);

   Ihistory(output, argv[0], args);
   Iwrite_image(output, argv[2]);

   return(0);
}
