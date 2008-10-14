
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

#ifndef MAIN

/*F:combine*

________________________________________________________________

		combine
________________________________________________________________

Name:		combine - combine two bands

Syntax:         | #include <xite/combine.h>
                |
                | int combine( IBAND b1, IBAND b2, IBAND b3,
                |    int expr1, int expr2, int expr3, int expr4,
                |    char* relop );
Description:	Combines two bands into a third as follows:

                | FOR (every pixel) DO
		|   IF (expr1 relop expr2) 
		|   THEN b3 = expr3
		|   ELSE b3 = expr4;

                -where:

		| b1 - first band to combine
		| b2 - second band to combine
		| b3 - output band
                | expr1, expr2, expr3, expr4 can have values:
		|      -1 for b1[y][x]
		|      -2 for b2[y][x]
		|      0..255 for 0..255
		| relop - one of ("EQ", "NE", "LT", "LE", "GT", "GE")

Return value:	| 0 => ok
                | 1 => bad pixel types
		| 2 => bad sizes
		| 3 => unknown relop

Author:		Tor L|nnestad, BLAB, ifi, UiO.

Id:             $Id$
________________________________________________________________

*/

#ifndef FUNCPROTO
int combine(b1, b2, b3, expr1, expr2, expr3, expr4, relop)
   IBAND b1, b2, b3;
   int expr1, expr2, expr3, expr4;
   char *relop;
#else /* FUNCPROTO */
int combine(IBAND b1, IBAND b2, IBAND b3, int expr1, int expr2, int expr3, int expr4, char *relop)
#endif /* FUNCPROTO */
{
   int x, y, xsize, ysize, a, b, c, d, irelop;

   if ((Ipixtyp(b1) NE Iu_byte_typ)
     OR (Ipixtyp(b2) NE Iu_byte_typ)
     OR (Ipixtyp(b3) NE Iu_byte_typ))
     return(Error(1, "combine: Bad pixel type\n"));

   if ((Ixsize(b1) NE Ixsize(b2))
     OR (Iysize(b1) NE Iysize(b2))
     OR (Ixsize(b1) NE Ixsize(b3))
     OR (Iysize(b1) NE Iysize(b3)))
     return(Error(2, "combine: Bad sizes\n"));

   if (strcmp(relop,"EQ") EQ 0)       irelop = 1;
   else if (strcmp(relop,"NE") EQ 0)  irelop = 2;
   else if  (strcmp(relop,"LT") EQ 0) irelop = 3;
   else if  (strcmp(relop,"LE") EQ 0) irelop = 4;
   else if  (strcmp(relop,"GT") EQ 0) irelop = 5;
   else if  (strcmp(relop,"GE") EQ 0) irelop = 6;
   else return(Error(3, "combine: Unknown relop\n"));

   xsize = Ixsize(b1);
   ysize = Iysize(b1);
   for (y=1; y LE ysize; INC y) {
     for (x=1; x LE xsize; INC x) {
       switch (expr1) {
         case -1: a = b1[y][x]; break;
         case -2: a = b2[y][x]; break;
         default: a = expr1;    break;
       }

       switch (expr2) {
         case -1: b = b1[y][x]; break;
         case -2: b = b2[y][x]; break;
         default: b = expr2;    break;
       }

       switch (expr3) {
         case -1: c = b1[y][x]; break;
         case -2: c = b2[y][x]; break;
         default: c = expr3;    break;
       }

       switch (expr4) {
         case -1: d = b1[y][x]; break;
         case -2: d = b2[y][x]; break;
         default: d = expr4;    break;
       }

       switch (irelop) {
         case 1: if (a EQ b) b3[y][x] = c; else b3[y][x] = d; break;
         case 2: if (a NE b) b3[y][x] = c; else b3[y][x] = d; break;
         case 3: if (a LT b) b3[y][x] = c; else b3[y][x] = d; break;
         case 4: if (a LE b) b3[y][x] = c; else b3[y][x] = d; break;
         case 5: if (a GT b) b3[y][x] = c; else b3[y][x] = d; break;
         case 6: if (a GE b) b3[y][x] = c; else b3[y][x] = d; break;
       }
     }
   }
   return(0);
}

#endif /* not MAIN */



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

#ifdef MAIN

#ifndef FUNCPROTO
int main(argc,argv)
int argc;
char *argv[];
#else /* FUNCPROTO */
int main(int argc, char **argv)
#endif /* FUNCPROTO */
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

#endif /* MAIN */
