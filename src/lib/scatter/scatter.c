
/*

________________________________________________________________

        scatter
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
#include <stdlib.h>
#include <xite/scatter.h>
#include XITE_STDIO_H
#include <xite/blab.h>
#include <xite/message.h>
#include <xite/readarg.h>

#ifndef MIN
# define MIN(a,b) (((a)<(b)) ? (a) : (b))
#endif



/*F:scatter*

________________________________________________________________

		scatter
________________________________________________________________

Name:		scatter - make a scatterplot

Syntax:         | #include <xite/scatter.h>
		|
                | int scatter( IBAND input1, IBAND input2,
                |    IBAND output, int *overflows );
		|
Description:	Creates a scatterplot (a two dimensional
                histogram) of the two input bands. Every
		scatter update that would increase the
		scatter value beyond 255 increases the
		parameter 'overflows' instead.

Restrictions:   All bands must have pixel type unsigned byte.

Return value:	| 0 => ok
                | 1 => Bad pixel type in input1
                | 2 => Bad pixel type in input2
                | 3 => Bad pixel type in output
		| 4 => Output has xsize less than 256
		| 5 => Output has ysize less than 256

Author:		Tor Lønnestad, BLAB, ifi, UiO

Id:             $Id$
________________________________________________________________

*/



int scatter(IBAND input1, IBAND input2, IBAND output, int *overflows)
{
   int x, y, val1, val2, xsize, ysize;

   *overflows = 0;
   if (Ipixtyp(input1) NE Iu_byte_typ)
     return(Error(1, "%s\n",
		  "scatter(): input1 must have pixel type unsigned byte."));
   if (Ipixtyp(input2) NE Iu_byte_typ)
     return(Error(2, "%s\n",
		  "scatter(): input2 must have pixel type unsigned byte."));
   if (Ipixtyp(output) NE Iu_byte_typ)
     return(Error(3, "%s\n",
		  "scatter(): output must have pixel type unsigned byte."));
   if (Ixsize(output) LT 256)
     return(Error(4, "scatter(): Output has xsize less than 256.\n"));
   if (Iysize(output) LT 256)
     return(Error(5, "scatter(): Output has ysize less than 256.\n"));

   xsize = MIN(Ixsize(input1),Ixsize(input2));
   ysize = MIN(Iysize(input1),Iysize(input2));

   for (y=1; y LE 256; y++)
     for (x=1; x LE 256; x++)
       output[y][x] = 0;

   FOR (y=1; y LE ysize; y++)
     FOR (x=1; x LE xsize; x++)
       val1 = input1[y][x]+1;
       val2 = input2[y][x]+1;
       if (output[val1][val2] LT 255)
         ++ output[val1][val2];
       else ++ (*overflows);
     ENDFOR;
   ENDFOR;
   return 0;

}  /*  scatter() */
