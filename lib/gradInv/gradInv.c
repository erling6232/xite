
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

/*F:gradInv*

________________________________________________________________

		gradInv
________________________________________________________________

Name:		gradInv - gradient invers noise reduction

Syntax:         | #include <xite/gradInv.h>
		|
                | int gradInv( IBAND input, IBAND output, int dx,
                |    int dy );

Description:    'gradInv' performs noise reduction by gradient
                invers smoothing.

		'dx' is the horizontal window size.
		'dy' is the vertical window size.

		The border is filtered as good as possible.

Return value:   | 0 => ok
                | 1 => bad pixel type input
		| 2 => bad pixel type output
		| 3 => bad dx value (less than 2)
		| 4 => bad dy value (less than 2)

See also:       gradInv(1)

References:     &Wang, Vagnucci and Li,
                "Gradient Inverse Weighted Smoothing Scheme and the Evaluation
		of Its Performance",
		Comp. Graph. and Im. Proc. '15', 167-181 (1981).

Author:		Tor L�nnestad, BLAB, Ifi, UiO.

Id:             $Id$
________________________________________________________________

*/

int gradInv(IBAND input, IBAND output, int dx, int dy)
{
   int x, y, xx, yy, xstart, ystart, xstop, ystop, dxhalf, dyhalf;
   int xsize, ysize, grad;
   float sumGrad, sumGradPixval, invgrad;

   if (Ipixtyp(input) NE Iu_byte_typ)
     return(Error(1, "gradInv: Input pixel type must be unsigned byte.\n"));
   if (Ipixtyp(output) NE Iu_byte_typ)
     return(Error(2, "gradInv: Output pixel type must be unsigned byte.\n"));
   if (dx LT 2)
     return(Error(3, "gradInv: Horizontal window size is too small.\n"));
   if (dy LT 2)
     return(Error(4, "gradInv: Vertical window size is too small.\n"));

   xsize = MIN(Ixsize(input), Ixsize(output));
   ysize = MIN(Iysize(input), Iysize(output));
   dxhalf = dx/2; dx = dxhalf*2 + 1;
   dyhalf = dy/2; dy = dyhalf*2 + 1;

   FOR (y=1; y LE ysize; INC y)
     ystart = MAX(1, y-dyhalf);
     ystop = MIN(ysize, y+dyhalf);
     FOR (x=1; x LE xsize; INC x)
       xstart = MAX(1, x-dxhalf);
       xstop = MIN(xsize, x+dxhalf);
       sumGrad = 0.0; sumGradPixval = 0.0;
       FOR (yy=ystart; yy LE ystop; INC yy)
         FOR (xx=xstart; xx LE xstop; INC xx)
           if ((xx EQ x) AND (yy EQ y)) continue;
           grad = input[yy][xx] - input[y][x];
           IF (grad LT 0) THEN invgrad = -1.0/grad;
           ELSEIF (grad EQ 0) THEN invgrad = 2.0;
           ELSE invgrad = 1.0/grad;
           ENDIF;
           sumGrad += invgrad;
           sumGradPixval += invgrad*input[yy][xx];
         ENDFOR;
       ENDFOR;

       output[y][x] = 
        (int)(((float)input[y][x] + sumGradPixval/sumGrad) * 0.5 + 0.5);
     ENDFOR;  /* all x */
   ENDFOR;  /* all y */

   return(0);

}  /* gradInv */
