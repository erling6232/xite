
/*

________________________________________________________________

        rank
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

#ifndef MAIN

#ifndef MAX
# define MAX(a,b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef MIN
# define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif


static void hist(IBAND input, int hxstart, int hystart, int hxstop, int hystop, int *h)
{
   register int x, y;
   for (x=0; x LE 255; INC x) h[x] = 0;
   for (y=hystart; y LE hystop; INC y)
     for (x=hxstart; x LE hxstop; INC x)
       INC h[input[y][x]];
}
 
/*F:rank*

________________________________________________________________

		rank
________________________________________________________________

Name:		rank - noise reduction filter

Syntax:         | #include <xite/median.h>
		|
                | int rank( IBAND input, IBAND output, int rank,
                |    int dx, int dy );
Description:    'rank' filters the input band into the output band
                using a rank filter with window size 'dx'*'dy'.
		Even values of dx and dy are increased by one.
		Every input pixel is replaced by the 'rank'-th
		pixel in a sorted vector containg all pixels
		in the dx*dy window. rank=1 gives the min
		filter, rank=dx*dy/2 gives the median filter,
		and rank = dx*dy gives the max filter.

                Uses histogram updating when moving horizontally
		from pixel to pixel, like the method used in-

Reference:	Huang, Yang and Tang: "A Fast Two-Dimensional
		Median Filtering Algorithm" IEEE Trans. Ac., Speech, 
		and Signal Proc, Vol ASSP-27, No.1, Feb.1979. 

Return value:   | 0 => ok
                | 1 => bad pixel type input band
                | 2 => bad pixel type output band
		| 3 => bad rank value (outide 1..dx*dy)
		| 4 => bad dx value (less than 1 or greater than xsize)
		| 5 => bad dy value (less than 1 or greater than ysize)

Author:		Tor L|nnestad, BLAB, ifi, UiO

Id:             $Id$
________________________________________________________________

*/

int rank(IBAND input, IBAND output, int rank, int dx, int dy)
{
   int hy, hystart, hystop, pixval, hxstart, rankPixVal, sumLErpv,
       x, y, xsize, ysize, hxstop, hysize, localRank, dxhalf, dyhalf;
   histogram h;

   xsize  = MIN(Ixsize(input),Ixsize(output));
   ysize  = MIN(Iysize(input),Iysize(output));
   dxhalf = dx/2;
   dyhalf = dy/2;
   dx     = dxhalf*2+1;
   dy     = dyhalf*2+1;

   if (Ipixtyp(input) NE Iu_byte_typ)
     return(Error(1, "Input pixel type must be unsigned byte.\n"));
   if (Ipixtyp(output) NE Iu_byte_typ)
     return(Error(2, "Output pixel type must be unsigned byte.\n"));
   if ((dx LT 1) OR (dx GT xsize))
     return(Error(4, "Illegal value for horizontal window size.\n"));
   if ((dy LT 1) OR (dy GT ysize))
     return(Error(5, "Illegal value for vertical window size.\n"));
   if ((rank LT 1) OR (rank GT dx*dy))
     return(Error(3, "Illegal value for rank.\n"));

   FOR (y=1; y LE ysize; INC y)
     /* calc available area */
     hystart = MAX(1,y-dyhalf);
     hystop  = MIN(ysize,y+dyhalf);
     hysize  = hystop-hystart+1;

     /* calc. initial histogram */
     hxstart  = 1;
     hxstop   = dxhalf+1;
     localRank = MIN(rank, hxstop*hysize);
     hist(input, hxstart, hystart, hxstop, hystop, h);

     /* find first rank */
     sumLErpv=h[0]; rankPixVal=0;
     while (sumLErpv LT localRank) sumLErpv += h[INC rankPixVal];
     output[y][1] = rankPixVal;

     /* update along the line until window is dx wide */
     FOR (x=2; hxstop LT dx; INC x)
       INC hxstop;
       localRank = MIN(rank, hxstop*hysize);
       /* update histgr and sumLessEqualRank */
       FOR (hy=hystart; hy LE hystop; INC hy)
         pixval = input[hy][hxstop];
         INC h[pixval];
         if (pixval LE rankPixVal) INC sumLErpv; 
       ENDFOR;
       /* calc new rankPixVal */
       while (sumLErpv GE localRank) sumLErpv -= h[rankPixVal DEC];
       while (sumLErpv LT localRank) sumLErpv += h[INC rankPixVal];
       output[y][x] = rankPixVal;
     ENDFOR;

     /* update along the line while window is dx wide */
     FOR ( ; hxstop LT xsize; INC x)
       /* update histgr and sumLessEqualRank */
       INC hxstop;
       FOR (hy=hystart; hy LE hystop; INC hy)
         pixval = input[hy][hxstart];
         DEC h[pixval];
         if (pixval LE rankPixVal) DEC sumLErpv;
         pixval = input[hy][hxstop];
         INC h[pixval];
         if (pixval LE rankPixVal) INC sumLErpv;
       ENDFOR;
       INC hxstart;
       /* calc new rankPixVal */
       while (sumLErpv GE rank) sumLErpv -= h[rankPixVal DEC];
       while (sumLErpv LT rank) sumLErpv += h[INC rankPixVal];
       output[y][x] = rankPixVal;
     ENDFOR;

     /* update along the line until end of line */
     FOR ( ; x LE xsize; INC x)
       /* update histgr and sumLessEqualRank */
       FOR (hy=hystart; hy LE hystop; INC hy)
         pixval = input[hy][hxstart];
         DEC h[pixval];
         if (pixval LE rankPixVal) DEC sumLErpv; 
       ENDFOR;
       INC hxstart;
       localRank = MIN(rank, (hxstop-hxstart+1)*hysize);
       /* calc new rankPixVal */
       while (sumLErpv GE localRank) sumLErpv -= h[rankPixVal DEC];
       while (sumLErpv LT localRank) sumLErpv += h[INC rankPixVal];
       output[y][x] = rankPixVal;
     ENDFOR;
   ENDFOR;
   return(0);
}  /* END of rank */

#endif /* not MAIN */



/*P:rank*

________________________________________________________________

		rank
________________________________________________________________

Name:		rank - noise reduction filter

Syntax:		rank <inimage> <outimage> <rank> [<dx> [<dy>]]

Description:    'rank' noise filtering with window 'dx'*'dy'
                using the histogram update technique.
		Every input pixel is replaced by the rank-th
		pixel in a sorted vector containg all pixels
		in the dx*dy window. rank=1 gives the min
		filter, rank=dx*dy/2 gives the median filter,
		and rank = dx*dy gives the max filter.

                | inimage - input image
		| outimage - output image
		| rank - the rank of the output pixel
		| dx - horizontal window size. Default value 3
		| dy - vertical window size. Default value dx

Files:	
See also:	
Diagnostics:

Restrictions:   Accepts only UNS_BYTE pixels. dx, dy must be
                larger than 0. rank should be in [1..dx*dy]

Author:		Tor L|nnestad, BLAB, ifi, UiO

Examples:       | rank mona.img monaRank.img 1
                | rank mona.img monaRank.img 3 7
		| rank mona.img monaRank.img 35 7 5

Id:             $Id$
________________________________________________________________

*/

#ifdef MAIN

int main(int argc, char **argv)
{
   IMAGE i1,i2;
   int bn, dx, dy, rnk;
   char arg[40];

   Iset_message(TRUE); 
   Iset_abort(TRUE);
   InitMessage(&argc, argv, xite_app_std_usage_text(
     "Usage: %s <inimage> <outimage> <rank> [<dx> [<dy>]]\n"));

   if (argc == 1) Usage(1, NULL);
   if ((argc < 4) OR (argc GT 6)) Usage(1, "Illegal number of arguments.\n");

   rnk = atoi(argv[3]);
   if (argc GE 5) dx = atoi(argv[4]); else dx = 3;
   if (argc GE 6) dy = atoi(argv[5]); else dy = dx;
   if ((dx LT 1) OR (dy LT 1))
     Error(2, "Window size should be greater than 0.\n");

   if ((rnk GT dx*dy) OR (rnk LT 1))
     Error(2, "Rank value should be in 1..dx*dy\n");
  
   i1 = Iread_image(argv[1]);
   i2 = Icopy_init(i1);

   for (bn=1; bn LE Inbands(i1); INC bn)
     if (rank(i1[bn], i2[bn], rnk, dx, dy))
       Error(2, "Error in band %d\n", bn);

   sprintf(arg, " rank = %d, dx = %d, dy = %d", rnk, dx, dy);
   Ihistory(i2, argv[0], arg);
   Iwrite_image(i2,argv[2]);
   return(0);
}

#endif /* MAIN */
