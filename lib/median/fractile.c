
/*

________________________________________________________________

        fractile
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
#include <xite/median.h>
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/histo.h>
#include <xite/message.h>

#ifndef MAX
# define MAX(a,b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef MIN
# define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif


static void hist(IBAND input, int hxstart, int hystart, int hxstop, int hystop, int *h)
{
   register int x,y;
   for (x=0; x LE 255; INC x) h[x] = 0;
   for (y=hystart; y LE hystop; INC y)
     for (x=hxstart; x LE hxstop; INC x)
       INC h[input[y][x]];
}
 
/*F:fractile*

________________________________________________________________

		fractile
________________________________________________________________

Name:		fractile - noise reduction filtering

Syntax:         | #include <xite/median.h>
		|
                | int fractile( IBAND input, IBAND output,
                |    int dx, int dy, double frac );

Description:    Filters the input band into the output band
                using a fractile filter with window size 'dx' x 'dy'.
		The filter sorts the pixels inside the window and
		choses the ('dx' * 'dy' * 'frac') pixel.

		'frac = 1.0/dx/dy' gives the minfilter, 'frac = 0.5'
		gives the median filter and 'frac = 1.0-1/dx/dy' gives 
		the max filter.

		Even values of 'dx' and 'dy' are increased by one. Uses
                histogram updating when moving horizontally from pixel to
                pixel.

Restrictions:   Accepts only pixel type unsigned byte.

Reference:      Huang, Yang and Tang:
		"A Fast Two-Dimensional Median Filtering Algorithm"
		IEEE Trans. Ac., Speech, and Signal Proc, Vol ASSP-27,
		No.1, Feb.1979. 

Return value:   | 0 => ok
                | 1 => bad pixel value input band
                | 2 => bad pixel value output band
		| 3 => bad dx value (less than 1 or greater than xsize)
		| 4 => bad dy value (less than 1 or greater than ysize)

Author:		Tor Lønnestad, BLAB, Ifi, UiO
Modified:	Otto Milvang, BLAB, Ifi, UiO
Id:             $Id$
________________________________________________________________

*/

int fractile(IBAND input, IBAND output, int dx, int dy, double frac)
{
   int hy, hystart, hystop, pixval, hxstart, med, sumLEMed;
   int x, y, xsize, ysize, hxstop, hysize, halfarea, dxhalf, dyhalf;
   histogram h;

   xsize = MIN(Ixsize(input),Ixsize(output));
   ysize = MIN(Iysize(input),Iysize(output));
   dxhalf = dx/2;
   dyhalf = dy/2;
   dx     = dxhalf*2+1;
   dy     = dyhalf*2+1;

   if (Ipixtyp(input) NE Iu_byte_typ) 
       return(Error(1, "Input band must have pixel type unsigned byte.\n"));
   if (Ipixtyp(output) NE Iu_byte_typ) 
       return(Error(1, "Output band must have pixel type unsigned byte.\n"));
   if ((dx LT 1) OR (dx GT xsize)) 
       return(Error(1, "Bad horizontal window xsize.\n"));
   if ((dy LT 1) OR (dy GT ysize)) 
       return(Error(1, "Bad vertical window ysize.\n"));
   if ((frac < 0.0) OR (frac > 1.0))
       return(Error(1, "Bad fractile argument.\n"));

   FOR (y=1; y LE ysize; INC y)
     /* calc available area */
     hystart = MAX(1,y-dyhalf);
     hystop  = MIN(ysize,y+dyhalf);
     hysize  = hystop-hystart+1;

     /* calc. initial histogram */
     hxstart  = 1;
     hxstop   = dxhalf+1;
     halfarea = (hxstop*hysize+1)*frac;
     hist(input,hxstart,hystart,hxstop,hystop,h);

     /* find first fractile */
     sumLEMed=h[0]; med=0;
     while (sumLEMed LT halfarea) sumLEMed += h[INC med];
     output[y][1] = med;

     /* update along the line until window is n*n */
     FOR (x=2; hxstop LT dx; INC x)
       INC hxstop;
       halfarea = (hxstop*hysize+1)*frac;
       /* update histgr and sumLessEqualFractile */
       FOR (hy=hystart; hy LE hystop; INC hy)
         pixval = input[hy][hxstop];
         INC h[pixval];
         if (pixval LE med) INC sumLEMed; 
       ENDFOR;
       /* calc new med */
       while (sumLEMed GE halfarea) sumLEMed -= h[med DEC];
       while (sumLEMed LT halfarea) sumLEMed += h[INC med];
       output[y][x] = med;
     ENDFOR;

     /* update along the line while window is n*n */
     FOR ( ; hxstop LT xsize; INC x)
       /* update histgr and sumLessEqualFractile */
       INC hxstop;
       FOR (hy=hystart; hy LE hystop; INC hy)
         pixval = input[hy][hxstart];
         DEC h[pixval];
         if (pixval LE med) DEC sumLEMed;
         pixval = input[hy][hxstop];
         INC h[pixval];
         if (pixval LE med) INC sumLEMed;
       ENDFOR;
       INC hxstart;
       /* calc new med */
       while (sumLEMed GE halfarea) sumLEMed -= h[med DEC];
       while (sumLEMed LT halfarea) sumLEMed += h[INC med];
       output[y][x] = med;
     ENDFOR;

     /* update along the line until end of line */
     FOR ( ; x LE xsize; INC x)
       /* update histgr and sumLessEqualFractile */
       FOR (hy=hystart; hy LE hystop; INC hy)
         pixval = input[hy][hxstart];
         DEC h[pixval];
         if (pixval LE med) DEC sumLEMed; 
       ENDFOR;
       INC hxstart;
       halfarea = ((hxstop-hxstart+1)*hysize+1)*frac;
       /* calc new med */
       while (sumLEMed GE halfarea) sumLEMed -= h[med DEC];
       while (sumLEMed LT halfarea) sumLEMed += h[INC med];
       output[y][x] = med;
     ENDFOR;
   ENDFOR;
   return(0);
}  /* END of fractile */
