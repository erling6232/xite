
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
#ifdef HAVE_STRINGS_H
#  include <strings.h>
#else
#  ifdef HAVE_STRING_H
#    include <string.h>
#  endif
#endif

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

int combine(IBAND b1, IBAND b2, IBAND b3, int expr1, int expr2, int expr3, int expr4, char *relop)
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
