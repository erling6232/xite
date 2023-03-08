
/*

________________________________________________________________

        sigma
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
#include <xite/sigma.h>
#include XITE_STDIO_H
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
#define isqrt(n) isqrtarr[n]

static int isqrtarr[16400];

/* generates histogram for rectangular subarea */
static void hist(IBAND input, int hxstart, int hystart, int hxstop, int hystop, int *h)
{
    int x,y;
   for (x=0; x LE 255; INC x) h[x] = 0;
   for (y=hystart; y LE hystop; INC y)
     for (x=hxstart; x LE hxstop; INC x)
       INC h[input[y][x]];
}



/*F:sigmat=sigma*/
/*F:noiseStDev=sigma*/
/*F:sigma*

________________________________________________________________

		sigma
________________________________________________________________

Name:		sigma, sigmat, noiseStDev - noise reduction filtering

Syntax:         | #include <xite/sigma.h>
		|
                | void sigma( IBAND input, IBAND output, int n,
                |    float s );
                | void sigmat( IBAND input, IBAND output, int n,
                |    int t );
                | float noiseStDev( IBAND input, int n );

Description:	'sigma' filters the 'input' band into the 'output'
                band using a sigma filter with window size 'n' x 'n'.
                The center pixel is replaced with the average of all
		pixels in the window whose value is within 's'
                standard deviations from the center pixel. The implementation
		calls 'noiseStDev' to obtain an estimate of the noise
		standard deviation, multiplies with 's' to obtain the
		parameter 't', and finally calls 'sigmat'.

		'sigmat' filters the input band into the output
                band using a sigma filter with window size 'n' x 'n'.
                The center pixel is replaced with the average of all
		pixels in the window whose value is within 't' counts
		from the center pixel. Uses histogram updating when
		moving horizontally from pixel to pixel.

                'noiseStDev' estimates the noise standard deviation
		by measuring the standard deviation within every
                possible 'n' x 'n' window, and averaging over the
		10% lowest values. Returns the standard deviation.

Restrictions:   'input' and 'output' must both have pixel type unsigned byte.

Return value:   &For sigmat
                | 0 : OK
                | 1 : Bad input pixel type
		| 2 : Bad output pixel type

		&For noiseStDev
		| -1   : Bad input pixel type
		| >= 0 : Standard deviation

		&For sigma
		| 0 : Ok
		| 1 : Bad input pixel type
		| >1 : Error in sigmat

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Id:             $Id$
________________________________________________________________

*/

int sigmat(IBAND input, IBAND output, int n, int t)
{
  int hy, hystart, hystop, pixval, hxstart, hxstop;
  int sum, num, i, start, stop;
  int x, y, xsize, ysize, hysize, nhalf;
  histogram h;

  xsize = MIN(Ixsize(input),Ixsize(output));
  ysize = MIN(Iysize(input),Iysize(output));
  nhalf = (n/2);
  n     = nhalf*2+1;

  if (Ipixtyp(input) != Iu_byte_typ)
    return(Error(1, "Input pixel type must be unsigned byte.\n"));
  if (Ipixtyp(output) != Iu_byte_typ)
    return(Error(2, "Output pixel type must be unsigned byte.\n"));

  FOR (y=1; y LE ysize; INC y)
    /* calc available area */
    hystart = MAX(1,y-nhalf);
    hystop  = MIN(ysize,y+nhalf);
    hysize  = hystop-hystart+1;

    /* calc. initial histogram */
    hxstart  = 1;
    hxstop   = nhalf+1;
    hist(input,hxstart,hystart,hxstop,hystop,h);

    /* find first sigma value */
    sum=0; num=0; pixval=input[y][1];
    start=MAX(pixval-t,0); stop=MIN(pixval+t,255);
    for (i=start; i LE stop; INC i)
      { sum += i*h[i]; num += h[i]; }
    output[y][1] = (UNS_BYTE)(sum/num); /* PASS AVRUNDINGSFEIL! */

    /* update along the line until window is n*n */
    FOR (x=2; hxstop LT n; INC x)
      INC hxstop;
      /* update histgr */
      for (hy=hystart; hy LE hystop; INC hy)
	INC h[input[hy][hxstop]];
      /* calc new output */
      sum=0; num=0; pixval=input[y][x]; 
      start=MAX(pixval-t,0); stop=MIN(pixval+t,255);
      for (i=start; i LE stop; INC i)
	{ sum += i*h[i]; num += h[i]; }
      output[y][x] = (UNS_BYTE)(sum/num); /* PASS AVRUNDINGSFEIL! */
    ENDFOR;

    /* update along the line while window is n*n */
    FOR ( ; hxstop LT xsize; INC x)
      /* update histogr */
      INC hxstop;
      FOR (hy=hystart; hy LE hystop; INC hy)
	DEC h[input[hy][hxstart]];
	INC h[input[hy][hxstop]];
      ENDFOR;
      INC hxstart;
      /* calc new output */
      sum=0; num=0; pixval=input[y][x]; 
      start=MAX(pixval-t,0); stop=MIN(pixval+t,255);
      for (i=start; i LE stop; INC i)
	{ sum += i*h[i]; num += h[i]; }
      output[y][x] = (UNS_BYTE)(sum/num); /* PASS AVRUNDINGSFEIL! */
    ENDFOR;

    /* update along the line until end of line */
    FOR ( ; x LE xsize; INC x)
      /* update histgr and sumLessEqualSigma */
      for (hy=hystart; hy LE hystop; INC hy)
	DEC h[input[hy][hxstart]];
      INC hxstart;
      /* calc new output */
      sum=0; num=0; pixval=input[y][x]; 
      start=MAX(pixval-t,0); stop=MIN(pixval+t,255);
      for (i=start; i LE stop; INC i)
	{ sum += i*h[i]; num += h[i]; }
      output[y][x] = (UNS_BYTE)(sum/num); /* PASS AVRUNDINGSFEIL! */
    ENDFOR;

  ENDFOR;

  return(0);

}  /* END of sigmat */

/*________________________________________________________________*/


float noiseStDev(IBAND input, int n)
{
   int x, y, nhalf, xmax, ymax, N, pixval,
       sum, sumx, sumxx, xx, yy, st_dev, i, j, k, l;
   histogram h;

   /* initialize */
   nhalf = n/2;
   n     = 2*nhalf+1;
   N     = n*n;
   xmax  = Ixsize(input)-n+1;
   ymax  = Iysize(input)-n+1;

   if (Ipixtyp(input) != Iu_byte_typ)
     return(Error(-1, "Input pixel type must be unsigned byte.\n"));

   for (x=0; x LE 255; INC x) h[x] = 0;
   for (i=0; i<=128; i++) {  
     j = i*i; k = (i+1)*(i+1);
     for (l=j; l<k; l++) isqrtarr[l] = i;
   }
   
   FOR (y=1; y LE ymax; INC y)
     /* calc first stdev */
     sumx = 0; sumxx = 0;
     FOR (yy=y; yy LT y+n; INC yy)
       FOR (xx=1; xx LE n; INC xx)
         pixval = input[yy][xx];
         sumx  += pixval;
         sumxx += pixval*pixval;
       ENDFOR;
     ENDFOR;
     st_dev = isqrt( (sumxx - sumx*sumx/N)/N );
     INC h[st_dev];

     /* update along the line */
     FOR (x=1; x LT xmax; INC x)
       FOR (yy=y; yy LT y+n; INC yy)
         pixval = input[yy][x];
         sumx  -= pixval;
         sumxx -= pixval*pixval;
         pixval = input[yy][x+n];
         sumx  += pixval;
         sumxx += pixval*pixval;
       ENDFOR;
       st_dev = isqrt( (sumxx - sumx*sumx/N)/N );
       INC h[st_dev];
     ENDFOR;
   ENDFOR; /* histogram h of standard deviations is now generated */
 
   sum = 0; sumx = 0; sumxx = 0;
   for (x=0; x LE 255; INC x) sumxx += h[x];
   sumxx = sumxx/10; /* use the mean of the 10% lowest standard dev.s */
   x = 0;
   while (sum LT sumxx)
     if (h[x]--)  {sumx += x; INC sum; } else INC x;
     
   return((float)sumx/sumxx);
} 

/*________________________________________________________________*/


int sigma(IBAND input, IBAND output, int n, float s)
{
   float st_dev;
   int  t;

   st_dev = noiseStDev(input, n);

   if (st_dev < 0)
     return(Error(1, "Input pixel type must be unsigned byte.\n"));

   t = (int) (st_dev*s);

   Info(0, "St.dev is estimated to %.2f, t is thus set to %d \n", st_dev, t);

   if ((t = sigmat(input, output, n, t)) != 0)
     return(Error(t, "Error in sigmat().\n"));

   return(0);

} /* END of sigma  */
