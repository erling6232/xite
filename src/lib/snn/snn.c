
/*

________________________________________________________________

        snn
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
#define ADDCLOSEST(val1,val2,center,sum)		\
   IF ((val1)+(val2) GT (center<<1))			\
     sum += ((val1) < (val2)) ? (val1) : (val2)		\
   ELSEIF ((val1)+(val2) LT (center<<1))		\
     sum += ((val1) > (val2)) ? (val1) : (val2)		\
   ELSE sum += center					\
   ENDIF

#define SNN						\
       sum = 0;						\
       yy2 = yystop;					\
       center = input[y][x];				\
       FOR (yy1=yystart; yy1 LT y; yy1++)		\
         xx2 = xxstop;					\
         FOR (xx1=xxstart; xx1 LE xxstop; )		\
           pixval1 = input[yy1][xx1++];			\
           pixval2 = input[yy2][xx2--];			\
           ADDCLOSEST(pixval1, pixval2, center, sum);	\
         ENDFOR;					\
         DEC yy2;					\
       ENDFOR;						\
       xx2 = xxstop;					\
       FOR (xx1=xxstart; xx1 LT x; )			\
         pixval1 = input[y][xx1++];			\
         pixval2 = input[y][xx2--];			\
         ADDCLOSEST(pixval1, pixval2, center, sum);	\
       ENDFOR;						\
       if (npixels)					\
         output[y][x] = (PIXELTYPE)(((float)sum/(float)npixels) + 0.5);	\
       else output[y][x] = input[y][x];



/*F:snn*

________________________________________________________________

		snn
________________________________________________________________

Name:		snn - symmetric nearest neighbour noise reduction

Syntax:         | #include <xite/snn.h>
		|
                | int snn( IBAND input, IBAND output, int dx,
                |    int dy );

Description:    'snn' performs symmetric nearest neighbour noise
                reduction and edge sharpening. Uses rectangular
		processing window of size 'dx' * 'dy'.

Restrictions:	'input' and 'output' must have pixel type unsigned byte or
                unsigned short. The types must be identical.
		Even values of 'dx' and 'dy' are increased by one.

Return value:   | 0 => ok
                | 1 => Bands have different pixel types
                | 2 => Bad pixel types
                | 3 => Bad dx value
		| 4 => Bad dy value

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Id:             $Id$
________________________________________________________________

*/
#define PIXELTYPE UNS_BYTE

static int snn8(IBAND input, IBAND output, int dx, int dy)
{
   int x, y, xx1, yy1, xx2, yy2, xxstart, yystart, xxstop, yystop, sum,
       dxhalf, dyhalf, xsize, ysize, npixels, pixval1, pixval2, center;

   dxhalf = dx/2; dx = 2*dxhalf+1;
   dyhalf = dy/2; dy = 2*dyhalf+1;

   xsize = MIN(Ixsize(input),Ixsize(output));
   ysize = MIN(Iysize(input),Iysize(output));

   FOR (y=1; y LE dyhalf; y++)
     yystart=1; yystop=(y<<1)-1;
     xxstart=1;
     for (x=1; x LE dxhalf; x++)
       { xxstop=(x<<1)-1; npixels=(yystop*xxstop)>>1; SNN;}
     xxstart=x-dxhalf; xxstop=x+dxhalf; npixels = (yystop*dx)>>1;
     for (; x LE (xsize-dxhalf); x++)
       { SNN; ++ xxstart; ++ xxstop; }
     xxstop=xsize;
     FOR (; x LE xsize; x++)
       xxstart = (x<<1)-xsize;
       npixels=(yystop*(xsize-xxstart+1))>>1;
       SNN;
     ENDFOR;
   ENDFOR;

   yystart=y-dyhalf; yystop=y+dyhalf;
   FOR (; y LE (ysize-dyhalf); y++)
     xxstart=1;
     for (x=1; x LE dxhalf; x++)
       { xxstop=(x<<1)-1; npixels=(dy*xxstop)>>1; SNN;}
     xxstart=x-dxhalf; xxstop=x+dxhalf; npixels=(dy*dx)>>1;
     for (; x LE (xsize-dxhalf); x++)
       { SNN; ++ xxstart; ++ xxstop; }
     xxstop=xsize;
     FOR (; x LE xsize; x++)
       xxstart = (x<<1)-xsize;
       npixels=(dy*(xsize-xxstart+1))>>1;
       SNN;
     ENDFOR;
     ++ yystart; ++ yystop;
   ENDFOR;

   yystop=ysize;
   FOR (; y LE ysize; y++)
     yystart = (y<<1)-ysize;
     xxstart=1;
     for (x=1; x LE dxhalf; x++)
       { xxstop=(x<<1)-1; npixels=((ysize-yystart+1)*xxstop)>>1; SNN;}
     xxstart=x-dxhalf; xxstop=x+dxhalf; npixels=((ysize-yystart+1)*dx)>>1;
     for (; x LE (xsize-dxhalf); x++)
       { SNN; ++ xxstart; ++ xxstop; }
     xxstop=xsize;
     FOR (; x LE xsize; x++)
       xxstart = (x<<1)-xsize;
       npixels=((ysize-yystart+1)*(xsize-xxstart+1))>>1;
       SNN;
     ENDFOR;
   ENDFOR;
   return(0);
}   /* snn */
#undef PIXELTYPE


/*
_________________________________________________________________

		snn16
_________________________________________________________________
*/
#define PIXELTYPE UNS_SHORT

static int snn16(IUS_BAND input, IUS_BAND output, int dx, int dy)
/* should be static, keep it global for a while for histiorical reasons */
/* Tor L|nnestad 13/1 1992 */
{
   int x, y, xx1, yy1, xx2, yy2, xxstart, yystart, xxstop, yystop, sum,
       dxhalf, dyhalf, xsize, ysize, npixels, pixval1, pixval2, center;

   dxhalf = dx/2; dx = 2*dxhalf+1;
   dyhalf = dy/2; dy = 2*dyhalf+1;

   xsize = MIN(Ixsize((IBAND) input),Ixsize((IBAND) output));
   ysize = MIN(Iysize((IBAND) input),Iysize((IBAND) output));

   FOR (y=1; y LE dyhalf; y++)
     yystart=1; yystop=(y<<1)-1;
     xxstart=1;
     for (x=1; x LE dxhalf; x++)
       { xxstop=(x<<1)-1; npixels=(yystop*xxstop)>>1; SNN;}
     xxstart=x-dxhalf; xxstop=x+dxhalf; npixels = (yystop*dx)>>1;
     for (; x LE (xsize-dxhalf); x++)
       { SNN; ++ xxstart; ++ xxstop; }
     xxstop=xsize;
     FOR (; x LE xsize; x++)
       xxstart = (x<<1)-xsize;
       npixels=(yystop*(xsize-xxstart+1))>>1;
       SNN;
     ENDFOR;
   ENDFOR;

   yystart=y-dyhalf; yystop=y+dyhalf;
   FOR (; y LE (ysize-dyhalf); y++)
     xxstart=1;
     for (x=1; x LE dxhalf; x++)
       { xxstop=(x<<1)-1; npixels=(dy*xxstop)>>1; SNN;}
     xxstart=x-dxhalf; xxstop=x+dxhalf; npixels=(dy*dx)>>1;
     for (; x LE (xsize-dxhalf); x++)
       { SNN; ++ xxstart; ++ xxstop; }
     xxstop=xsize;
     FOR (; x LE xsize; x++)
       xxstart = (x<<1)-xsize;
       npixels=(dy*(xsize-xxstart+1))>>1;
       SNN;
     ENDFOR;
     ++ yystart; ++ yystop;
   ENDFOR;

   yystop=ysize;
   FOR (; y LE ysize; y++)
     yystart = (y<<1)-ysize;
     xxstart=1;
     for (x=1; x LE dxhalf; x++)
       { xxstop=(x<<1)-1; npixels=((ysize-yystart+1)*xxstop)>>1; SNN;}
     xxstart=x-dxhalf; xxstop=x+dxhalf; npixels=((ysize-yystart+1)*dx)>>1;
     for (; x LE (xsize-dxhalf); x++)
       { SNN; ++ xxstart; ++ xxstop; }
     xxstop=xsize;
     FOR (; x LE xsize; x++)
       xxstart = (x<<1)-xsize;
       npixels=((ysize-yystart+1)*(xsize-xxstart+1))>>1;
       SNN;
     ENDFOR;
   ENDFOR;
   return(0);
}   /* snn16 */
#undef PIXELTYPE


int snn(IBAND input, IBAND output, int dx, int dy)
{
   if (Ipixtyp(input) NE Ipixtyp(output))
     return(Error(1, "snn: Bands have different pixel types\n"));

   if (dx LT 0)
     return(Error(3, "snn: Bad value for horizontal window size.\n"));
   if (dy LT 0)
     return(Error(4, "snn: Bad value for vertical window size.\n"));

   if (Ipixtyp(input) == Iu_byte_typ)
     return(snn8(input, output, dx, dy));

   if (Ipixtyp(output) == Iu_short_typ)
     return(snn16((IUS_BAND)input, (IUS_BAND)output, dx, dy));

   return(Error(2,
   "Input and output pixel types must be unsigned byte or unsigned short.\n"));
}
