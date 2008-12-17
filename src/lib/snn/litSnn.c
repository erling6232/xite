
/*

________________________________________________________________

        litSnn
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
#include <xite/snn.h>
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/message.h>
#include <xite/readarg.h>

#define NIL 0
#ifndef MIN
# define MIN(a,b) (((a) LT (b)) ? (a) : (b))
#endif
#define ADDCLOSEST(val1,val2,center)				\
    diff = val1-center; if (diff LT 0) diff = -diff;		\
    IF (diff LE dp) ++ npixels;					\
    ELSE 							\
      diff = val2-center; if (diff LT 0) diff = -diff;		\
      if (diff LE dp) ++ npixels;				\
    ENDIF;


#define LITSNN							\
       npixels = 0;						\
       yy2 = yystop;						\
       center = input[y][x];					\
       FOR (yy1=yystart; yy1 LT y; yy1++)			\
         xx2 = xxstop;						\
         FOR (xx1=xxstart; xx1 LE xxstop; )			\
           pixval1 = input[yy1][xx1++];				\
           pixval2 = input[yy2][xx2--];				\
           ADDCLOSEST(pixval1, pixval2, center);		\
         ENDFOR;						\
         -- yy2;						\
       ENDFOR;							\
       xx2 = xxstop;						\
       FOR (xx1=xxstart; xx1 LT x; )				\
         pixval1 = input[y][xx1++];				\
         pixval2 = input[y][xx2--];				\
         ADDCLOSEST(pixval1, pixval2, center);			\
       ENDFOR;							\
       output[y][x] = MIN(255,npixels);



/*F:litSnn*

________________________________________________________________

		litSnn
________________________________________________________________

Name:		litSnn - texture measure, combination of lit and snn

Syntax:         | #include <xite/snn.h>
		|
                | int litSnn( IBAND input, IBAND output, int dp,
                |    int dx, int dy );

Description:    Combination of lit - local information transform -
                and snn - symmetric nearest neighbour. Only those
		pixels selected by the snn filter are included in
		the counting of pixels within the pixel value
		interval [centerpixel-dp..centerpixel+dp]. The
		output pixel value is the number of such pixels
		within the window. Uses a rectangular processing
		window of size 'dx' * 'dy'.

		'dp' is the delta pixel value.

Restrictions:   'input' and 'output' must have pixel type unsigned byte.
                Even values of 'dx' and 'dy' are increased by one.
		'dx', 'dy' and 'dp' should be positive.

Return value:   | 0 => ok
                | 1 => Bad input pixel type
                | 2 => Bad output pixel type
                | 3 => Bad dx value (less than 0)
		| 4 => Bad dy value (less than 0)
		| 5 => Bad dp value (less than 0)

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Id:             $Id$
________________________________________________________________

*/

int litSnn(IBAND input, IBAND output, int dp, int dx, int dy)
{
   int x, y, xx1, yy1, xx2, yy2, xxstart, yystart, xxstop, yystop, diff,
       dxhalf, dyhalf, xsize, ysize, npixels, pixval1, pixval2, center;

   if (Ipixtyp(input) NE Iu_byte_typ)
     return(Error(1, "Input pixel type must be unsigned byte.\n"));
   if (Ipixtyp(output) NE Iu_byte_typ)
     return(Error(2, "Output pixel type must be unsigned byte.\n"));
   if (dx LT 0) return(Error(3, "Bad value for horizontal window size.\n"));
   if (dy LT 0) return(Error(4, "Bad value for vertical window size.\n"));
   if (dp LT 0) return(Error(5, "Bad value for pixel range.\n"));

   dxhalf = dx/2; dx = 2*dxhalf+1;
   dyhalf = dy/2; dy = 2*dyhalf+1;

   xsize = MIN(Ixsize(input),Ixsize(output));
   ysize = MIN(Iysize(input),Iysize(output));

   FOR (y=1; y LE dyhalf; y++)
     yystart=1; yystop=(y<<1)-1;
     xxstart=1;
     for (x=1; x LE dxhalf; x++)
       { xxstop=(x<<1)-1; LITSNN;}
     xxstart=x-dxhalf; xxstop=x+dxhalf;
     for (; x LE (xsize-dxhalf); x++)
       { LITSNN; ++ xxstart; ++ xxstop; }
     xxstop=xsize;
     FOR (; x LE xsize; x++)
       xxstart = (x<<1)-xsize;
       LITSNN;
     ENDFOR;
   ENDFOR;

   yystart=y-dyhalf; yystop=y+dyhalf;
   FOR (; y LE (ysize-dyhalf); y++)
     xxstart=1;
     for (x=1; x LE dxhalf; x++)
       { xxstop=(x<<1)-1; LITSNN;}
     xxstart=x-dxhalf; xxstop=x+dxhalf;
     for (; x LE (xsize-dxhalf); x++)
       { LITSNN; ++ xxstart; ++ xxstop; }
     xxstop=xsize;
     FOR (; x LE xsize; x++)
       xxstart = (x<<1)-xsize;
       LITSNN;
     ENDFOR;
     ++ yystart; ++ yystop;
   ENDFOR;

   yystop=ysize;
   FOR (; y LE ysize; y++)
     yystart = (y<<1)-ysize;
     xxstart=1;
     for (x=1; x LE dxhalf; x++)
       { xxstop=(x<<1)-1; LITSNN;}
     xxstart=x-dxhalf; xxstop=x+dxhalf;
     for (; x LE (xsize-dxhalf); x++)
       { LITSNN; ++ xxstart; ++ xxstop; }
     xxstop=xsize;
     FOR (; x LE xsize; x++)
       xxstart = (x<<1)-xsize;
       LITSNN;
     ENDFOR;
   ENDFOR;
   return(0);
}   /* litSnn */
