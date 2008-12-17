

/*

________________________________________________________________

        stvar.c
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
#include <xite/stdiff.h>
#include XITE_STDIO_H
#include <stdlib.h>
#include XITE_UNISTD_H
#include <xite/blab.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include XITE_MALLOC_H
#include XITE_STRING_H

static int sqrtarr_s[17000];
#define isqrt(i) sqrtarr_s[i]

/*L*

________________________________________________________________

		update_line
________________________________________________________________

Name:		update_line
Syntax:		
Description:	Update sum 'x' and square sum 'x2' for a 'line'
                of length 'width'. 
Return value:
Author:		Otto Milvang
________________________________________________________________

*/


static void update_line(UNS_BYTE *line, int *x, int *x2, int width, int add)
{
  int val;
  
  line++;
  if (add) {
    while(width--) {
      val = *line++;
      *x  += val; x++;
      val  = val*val;
      *x2 += val; x2++;
    }
  } else {
    while(width--) {
      val = *line++;
      *x  -= val; x++;
      val  = val*val;
      *x2 -= val; x2++;
    }
  }
}



/*L*

________________________________________________________________

		stvar_line
________________________________________________________________

Name:		stvar_line
Syntax:		
Description:
Return value:
Author:		Otto Milvang
________________________________________________________________

*/

#define stvar_pixel {\
 	  mid  = sumx / N;\
          val  = *input++;\
	  stdev = isqrt( (sqsumx - sumx*mid)/N);\
	  val = val - (stdev * k) / 256 - mid + 128;\
          if (val & ~0x0ff)\
	    {\
              if (val < 0) val = 0; else val = 0xff;\
	    }\
	  *result++ = val;\
	  }

static void stvar_line(UNS_BYTE *result, UNS_BYTE *input, int *x, int *x2, int width, int deltax, int deltay, int k)
{
  int sumx, sqsumx, N, mid, stdev, i, val;
  int *xx, *xx2;

  result++; input++;
  sumx = 0; sqsumx = 0; N = 0;
  xx = x; xx2 = x2;
  if (deltax > width) deltax = width-1;
  if ((deltax & 1) == 0) deltax++; 
  for (i=1; i<=deltax; i++) {
    N      += deltay;
    sumx   += *xx++;
    sqsumx += *xx2++;
    if (i & 1) stvar_pixel;
  }
  width -= deltax;
  while (width--) {
    sumx   += *xx++;
    sumx   -= *x++;
    sqsumx += *xx2++;
    sqsumx -= *x2++;
    stvar_pixel;
  }
  while (deltax--) {
    N      -= deltay;
    sumx   -= *x++;
    sqsumx -= *x2++;
    if (deltax & 1) stvar_pixel;
  }
}




/*L*

________________________________________________________________

		stther_line
________________________________________________________________

Name:		stther_line
Syntax:		
Description:
Return value:
Author:		Otto Milvang
________________________________________________________________

*/

#define stther_pixel {\
 	  mid   = sumx / N;\
          val  = *input++;\
	  stdev = isqrt( (sqsumx - sumx*mid)/N);\
	  val = (val - (stdev * k) / 256 - mid >= 0) ? 255 : 0;\
	  *result++ = val;\
	  }

static void stther_line(UNS_BYTE *result, UNS_BYTE *input, int *x, int *x2, int width, int deltax, int deltay, int k)
{
  int sumx, sqsumx, N, mid, stdev, i, val;
  int *xx, *xx2;

  result++; input++;
  sumx = 0; sqsumx = 0; N = 0;
  xx = x; xx2 = x2;

  if (deltax > width) deltax = width-1;
  if ((deltax & 1) == 0) deltax++; 
  
  for (i=1; i<=deltax; i++) {
    N      += deltay;
    sumx   += *xx++;
    sqsumx += *xx2++;
    if (i & 1) stther_pixel;
  }
  width -= deltax;
  while (width--) {
    sumx   += *xx++;
    sumx   -= *x++;
    sqsumx += *xx2++;
    sqsumx -= *x2++;
    stther_pixel;
  }
  while (deltax--) {
    N      -= deltay;
    sumx   -= *x++;
    sqsumx -= *x2++;
    if (deltax & 1) stther_pixel;
  }
}




/*F:stvar*

________________________________________________________________

		stvar
________________________________________________________________

Name:		stvar - local variance

Syntax:         | #include <xite/stdiff.h>
		|
                | int stvar( IBAND input, IBAND output,
		|    int deltax, int deltay, double kt, int t );

Description:	Local variable threshold.

                | t(x,y) = k*s(x,y) + u(x,y)
		|      r(x,y) = b(x,y) - t(x,y) + 128
		| [-t] r(x,y) = if( b(x,y) >= t(x,y)) then 255 else 0 
		|
		| r(x,y)  = output image
		| u(x,y)  = mean component
		| s(x,y)  = st. dev component
		| b(b,y)  = pixel value in (x,y)

		'deltax' is local area width, 'deltay' is local area height.

Restrictions:   'input' and 'output' must have pixel type unsigned byte.

Return value:   | 0 : Ok
                | 1 : Bad input pixel type
		| 2 : Bad output pixel type

Reference:	Wayne Niblack: An introduction to Digital Image Processing

Author:		Otto Milvang
________________________________________________________________

*/


int stvar(IBAND input, IBAND output, int deltax, int deltay, double kt, int t)
{
  int *x, *x2, i, j, k, l, ik;
  UNS_BYTE **first, **last, **inptr, **outptr;
  int width, height;

  if (Ipixtyp(input) != Iu_byte_typ)
    return(Error(1, "Input pixel type must be unsigned byte.\n"));
  if (Ipixtyp(output) != Iu_byte_typ)
    return(Error(2, "Output pixel type must be unsigned byte.\n"));

  width  = Ixsize(input);
  height = Iysize(input);

  for (i=0; i<=128; i++) {  
    j = i*i; k = (i+1)*(i+1);
    for (l=j; l<k; l++) sqrtarr_s[l] = i;
  }
  
  inptr = &input[1]; outptr = &output[1];
  ik = kt * 256.0;
  first = last = inptr; 
  x  = (int *) malloc(width*sizeof(int));
  x2 = (int *) malloc(width*sizeof(int));
  
  for(i=0; i<width; i++) x[i] = x2[i] = 0;
  
  for (i=1; i<= deltay; i++) {
    update_line(*first++, x, x2, width, 1);
    if ( i & 1) { 
      if (t) stther_line(*outptr++, *inptr++, x, x2, width, deltax, i, ik);
      else stvar_line(*outptr++, *inptr++, x, x2, width, deltax, i, ik);
    }
  }
  height -= deltay;
  while(height--) {
    update_line(*first++, x, x2, width, 1);
    update_line(*last++, x, x2, width, 0);
    if (t) stther_line(*outptr++, *inptr++, x, x2, width,
		       deltax, deltay, ik);
    else stvar_line(*outptr++, *inptr++, x, x2, width,
		      deltax, deltay, ik);
  }
  while(deltay--) {
    update_line(*last++, x, x2, width, 0);
    if (deltay & 1) { 
      if (t) stther_line(*outptr++, *inptr++, x, x2, width,
			 deltax, deltay, ik);
      else stvar_line(*outptr++, *inptr++, x, x2, width, deltax, deltay, ik);
    }
  }
  free(x);
  free(x2);

  return(0);
}
