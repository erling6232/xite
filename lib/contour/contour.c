
/*

________________________________________________________________

        contour
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
#include <xite/contour.h>
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/message.h>
#include <xite/readarg.h>

#ifndef MIN
# define MIN(a,b) ((a)<(b) ? a : b)
#endif

/*F:contour*

________________________________________________________________

		contour
________________________________________________________________

Name:		contour - create a contour map

Syntax:         | #include <xite/contour.h>
                |
                | int contour( IBAND input, IBAND output,
                |    int eqd, int cpv, int bpv );

Description:    Creates a contour map of the input image, with
                equidistance 'eqd'. Contour pixels receive the value 'cpv',
		contour pixel value, and background pixels receive the value
		'bpv', background pixel value. Contours should
		really be drawn 'between' the pixels, but are
		shifted half a pixel to the left or half a pixel
		upward to hit a pixel.

Restrictions:   'input' and 'output' must have pixel type unsigned byte.
                'eqd' must be in [2, 255].

Return value:   | 0 => ok
                | 2 => bad pixel type input
		| 3 => bad pixel type output
		| 4 => bad eqd value (less than 2 or greater than 255)

See also:       contour(1), crossing(3)

Author:		Tor Lønnestad, BLAB, Ifi, UiO.

Id:             $Id$
________________________________________________________________

*/

int contour(IBAND input, IBAND output, int eqd, int cpv, int bpv)
{
   int x, y, xsize, ysize, i, mapval;
   UNS_BYTE map[256];

   if (Ipixtyp(input) NE Iu_byte_typ)
     return(Error(2, "contour: Input pixel type must be unsigned byte.\n"));
   if (Ipixtyp(output) NE Iu_byte_typ)
     return(Error(3, "contour: Output pixel type must be unsigned byte.\n"));
   if ((eqd LT 2) OR (eqd GT 255))
     return(Error(4, "contour: bad eqd argument value.\n"));

   if (cpv LT 0)   cpv = 0;
   if (cpv GT 255) cpv = 255;
   if (bpv LT 0)   bpv = 0;
   if (bpv GT 255) bpv = 255;

   for (i=0; i LE 255; ++ i) map[i] = i/eqd;

   xsize = MIN(Ixsize(input), Ixsize(output));
   ysize = MIN(Iysize(input), Iysize(output));

   FOR (y=1; y LT ysize; ++ y)
     FOR (x=1; x LT xsize; ++ x)
       mapval = map[input[y][x]];
       if ((mapval NE map[input[y][x+1]])
         OR (mapval NE map[input[y+1][x]]))
         output[y][x] = cpv; /* contour */
       else output[y][x] = bpv; /* background */
     ENDFOR;  /* all x */
     output[y][xsize] =
       (map[input[y][x]] NE map[input[y+1][x]]) ? cpv : bpv;
   ENDFOR;  /* all y */

   for (x=1; x LT xsize; ++ x)
     output[ysize][x] = 
       (map[input[ysize][x]] NE map[input[ysize][x+1]]) ? cpv : bpv;

   output[ysize][xsize] = bpv;

   return(0);

}  /* contour */
