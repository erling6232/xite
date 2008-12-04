
/*

________________________________________________________________

        combine
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
#include <xite/blab.h>
#include <xite/biff.h>
#include <xite/combine.h>
#include <xite/message.h>
#include XITE_STRING_H

/*P:combine*

________________________________________________________________

		combine
________________________________________________________________

Name:		combine - combine two images

Syntax:		combine <inp1><inp2><outp><expr1><expr2><expr3><expr4><relop>

Description:	The program combines two images into a new image
                according to the following algorithm:

		| FOR (every pixel) DO
		|   IF (expr1 RELOP expr2)
		|   THEN output = expr3
		|   ELSE output = expr4

                The program requires the following arguments:

		| input 1 - first image to combine
		| input 2 - second image to combine
		| output  - name of output image
		| expr1, expr2, expr3 & expr4 - one of the values:
		|       -1 for inputimage1[y][x]
		|       -2 for inputimage2[y][x]
		|       0..255 - the constant values 0..255
		| relop - one of (EQ, NE, LT, LE, GT, GE)

Restrictions:	The input images should have UNS_BYTE pixels and
                equal sizes.

Author:		Tor Lønnestad, BLAB, ifi, UiO.

Examples:	| combine mona.img lena.img res.img -1 -2 50 -2 LT

                -which does the following:

		| FOR (every pixel (x,y)) DO
		|   IF ( mona[y][x] LT lena[y][x] )
		|   THEN res[y][x] = 50
		|   ELSE res[y][x] = lena[y][x]

Id:             $Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
   IMAGE i1,i2,i3;
   int e1, e2, e3, e4, n;

   InitMessage(&argc, argv, xite_app_std_usage_text(
     "Usage: %s <inp1><inp2><outp><expr1><expr2><expr3><expr4><relop>\n"));
   if (argc == 1) Usage(1, (char*)0);
   if (argc != 9) Usage(2, "Bad number of arguments\n");
   Iset_message(TRUE);
   i1 = Iread_image(argv[1]);
   i2 = Iread_image(argv[2]);
   i3 = Icopy_init(i1);
   e1 = atoi(argv[4]);
   e2 = atoi(argv[5]);
   e3 = atoi(argv[6]);
   e4 = atoi(argv[7]);

   for (n=1; n LE Inbands(i1); INC n)
     combine(i1[n],i2[n],i3[n],e1,e2,e3,e4,argv[8]);

   Ihistory(i3, argv[0], "");
   Iwrite_image(i3,argv[3]);
   return(0);
}  /* main */
