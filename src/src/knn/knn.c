
/*

________________________________________________________________

        knn
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
#include <xite/knn.h>
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/histo.h>
#include <xite/readarg.h>
#include <xite/message.h>

#ifndef MAX
# define MAX(a,b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef MIN
# define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef MAIN

static void hist(IBAND input, int hxstart, int hystart, int hxstop, int hystop, int *h)
{
   int x, y;
   for (x=0; x LE 255; x++) h[x] = 0;
   for (y=hystart; y LE hystop; y++)
     for (x=hxstart; x LE hxstop; x++)
       ++ h[input[y][x]];
}
 
/*F:knn*

________________________________________________________________

		knn
________________________________________________________________

Name:		knn - k nearest neighbour noise reduction

Syntax:         | #include <xite/knn.h>
		|
                | int knn( IBAND input, IBAND output, int n,
                |    int k );
Description:    'knn' performs 'k' nearest neighbour noise reduction,
                averaging over the center pixel and the 'k'
		nearest neighbours. If 'n' is even 'n'+1 is used.
                Uses histogram updating.

                'n' is the width of a square window. 'k' is the number of
                neighbours to average over.

Restrictions:   'input' and 'output' must have pixeltype unsigned byte.

Reference:      L. S. Davis & A. Rosenfeld:"Noise Cleaning by
                Iterated Local Averaging" IEEE trans. on syst.,
		man, and cyb., vol smc-8, no 9, 1978.

See also:       knn(1), mbknn(3), kncn(3), mbkncn(3)

Return value:   | 0 => ok
                | 1 => Bad input pixel type
                | 2 => Bad output pixel type
                | 3 => Bad n value (less than 1)
                | 4 => Bad k value (less than 1 or larger than n**2-1)

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Id:             $Id$
________________________________________________________________

*/

int knn(IBAND input, IBAND output, int n, int k)
{
   int hxstart, hystart, hystop, hy, pixval;
   int x, y, xsize, ysize, hxstop, hysize, fullarea, nhalf, localk;
   int sum, sumpixval, min, max, nmax, nmin;
   histogram h;

   xsize = MIN(Ixsize(input),Ixsize(output));
   ysize = MIN(Iysize(input),Iysize(output));

   if (Ipixtyp(input) NE Iu_byte_typ)
     return(Error(1, "knn: Input pixel type must be unsigned byte.\n"));
   if (Ipixtyp(output) NE Iu_byte_typ)
     return(Error(2, "knn: Input pixel type must be unsigned byte.\n"));
   if (n LT 1) return(Error(3, "knn: Bad n value\n"));
   if ((k LT 1) OR (k GT n*n-1))
     return(Error(4, "knn: Bad k value\n"));

   nhalf = (n/2);
   n = nhalf*2+1;
   fullarea = n*n;
   ++k; /* now includes center pixel */

   FOR (y=1; y LE ysize; y++)
     /* calc available area */
     hystart = MAX(1,y-nhalf);
     hystop  = MIN(ysize,y+nhalf);
     hysize  = hystop-hystart+1;

     /* calc. initial histogram */
     hxstart  = 1;
     hxstop   = nhalf+1;
     localk = k * hxstop * hysize / fullarea;
     hist(input,hxstart,hystart,hxstop,hystop,h);

     /* find first neighbour average */
     pixval=input[y][1];
     sum = h[pixval]; sumpixval = sum*pixval;
     min = pixval-1; max = pixval+1;
     nmax = (max GT 255) ? 0 : h[max];
     nmin = (min LT   0) ? 0 : h[min];

     IF (sum LT localk)
       LOOP
         if (nmax)
           { sumpixval += max; nmax--; EXITIF (++sum GE localk); };
         if (nmin)
           { sumpixval += min; nmin--; EXITIF (++sum GE localk); };
         WHILE (NOT (nmax OR nmin))
           nmax = (++max GT 255) ? 0 : h[max];
           nmin = (--min LT   0) ? 0 : h[min];
         ENDWHILE;
       ENDLOOP;
     ENDIF;
     output[y][1] = sumpixval/sum; /* avrundingsfeil */

     /* update along the line until window is n*n */
     FOR (x=2; hxstop LT n; x++)
       ++ hxstop;
       localk = k * hxstop * hysize / fullarea;
       /* update histgr */
       for (hy=hystart; hy LE hystop; hy++)
         ++ h[input[hy][hxstop]];

       /* calc new average */
       pixval=input[y][x];
       sum = h[pixval]; sumpixval = sum*pixval;
       min = pixval-1; max = pixval+1;
       nmax = (max GT 255) ? 0 : h[max];
       nmin = (min LT   0) ? 0 : h[min];
       IF (sum LT localk)
         LOOP
           if (nmax)
             { sumpixval += max; nmax--; EXITIF (++sum GE localk); };
           if (nmin)
             { sumpixval += min; nmin--; EXITIF (++sum GE localk); };
           WHILE (NOT (nmax OR nmin))
             nmax = (++max GT 255) ? 0 : h[max];
             nmin = (--min LT   0) ? 0 : h[min];
           ENDWHILE;
         ENDLOOP;
       ENDIF;
       output[y][x] = sumpixval/sum; /* avrundingsfeil */
     ENDFOR;

     /* update along the line while window has width n */
     localk = k * n * hysize / fullarea;
     FOR ( ; hxstop LT xsize; x++)
       /* update histgr */
       ++ hxstop;
       FOR (hy=hystart; hy LE hystop; hy++)
         -- h[input[hy][hxstart]];
         ++ h[input[hy][hxstop]];
       ENDFOR;
       ++ hxstart;

       /* calc new average */
       pixval=input[y][x];
       sum = h[pixval]; sumpixval = sum*pixval;
       min = pixval-1; max = pixval+1;
       nmax = (max GT 255) ? 0 : h[max];
       nmin = (min LT   0) ? 0 : h[min];

       IF (sum LT localk)
         LOOP
           if (nmax)
             { sumpixval += max; nmax--; EXITIF (++sum GE localk); };
           if (nmin)
             { sumpixval += min; nmin--; EXITIF (++sum GE localk); };
           WHILE (NOT (nmax OR nmin))
             nmax = (++max GT 255) ? 0 : h[max];
             nmin = (--min LT   0) ? 0 : h[min];
           ENDWHILE;
         ENDLOOP;
       ENDIF;
       output[y][x] = sumpixval/sum; /* avrundingsfeil */
     ENDFOR;

     /* update along the line until end of line */
     FOR ( ; x LE xsize; x++)
       /* update histgr */
       for (hy=hystart; hy LE hystop; hy++)
         -- h[input[hy][hxstart]];
       ++ hxstart;
       localk = (hxstop-hxstart+1)*hysize * k / fullarea;

       /* calc new average */
       pixval=input[y][x];
       sum = h[pixval]; sumpixval = sum*pixval;
       min = pixval-1; max = pixval+1;
       nmax = (max GT 255) ? 0 : h[max];
       nmin = (min LT   0) ? 0 : h[min];
       IF (sum LT localk)
         LOOP
           if (nmax)
             { sumpixval += max; nmax--; EXITIF (++sum GE localk); };
           if (nmin)
             { sumpixval += min; nmin--; EXITIF (++sum GE localk); };
           WHILE (NOT (nmax OR nmin))
             nmax = (++max GT 255) ? 0 : h[max];
             nmin = (--min LT   0) ? 0 : h[min];
           ENDWHILE;
         ENDLOOP;
       ENDIF;
       output[y][x] = sumpixval/sum; /* avrundingsfeil */
     ENDFOR;
   ENDFOR; /* all y */
   return(0);
}  /* END of knn */

#endif /* not MAIN */



/*P:knn*

________________________________________________________________

		knn
________________________________________________________________

Name:		knn - k nearest neighbour noise reduction

Syntax:		knn <input image> <output image> <n> <k>

Description:    Performs k nearest neighbour noise removal.
                n x n is window size.
		Averages over center pixel and k neighbours,
		using histogram updating technique.

Restrictions:   The input image must have pixel type unsigned byte.

Reference:      L. S. Davis & A. Rosenfeld:"Noise Cleaning by
                Iterated Local Averaging" IEEE trans. on syst.,
		man, and cyb., vol smc-8, no 9, 1978.
See also:       knn(3), mbknn(1), kncn(1), mbkncn(1)

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Examples:       knn mona.img monaknn.img 5 9

Id:             $Id$
________________________________________________________________

*/

#ifdef MAIN

int main(int argc, char **argv)
{
   IMAGE i1, i2;
   int n, k, bn, stat;
   char *args;

   InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <inimage> <outimage> <n> <k>\n"));
   Iset_message(TRUE);
   Iset_abort(TRUE);

   if (argc == 1) Usage(1, NULL);
   args = argvOptions(argc, argv);
   if (argc != 5) Usage(2, "Illegal number of arguments.\n");

   n = atoi(argv[3]);
   k = atoi(argv[4]);

   i1 = Iread_image(argv[1]);
   i2 = Icopy_init(i1);

   for (bn=1; bn LE Inbands(i1); bn++)
     if ((stat = knn(i1[bn], i2[bn], n, k)))
       Warning(stat, "Error in band %d\n", bn);

   Ihistory(i2, argv[0], args);
   Iwrite_image(i2, argv[2]);

   return(0);
}

#endif /* MAIN */
