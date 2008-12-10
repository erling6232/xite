

/*

________________________________________________________________

        stdiff.c
        $Id$
        Copyright 1990, Blab, UiO
        Image processing lab, Department of Informatics
        University of Oslo
        E-mail: blab@ifi.uio.no
________________________________________________________________
  
  Permission to use, copy, modify and distribute this software and it
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
#include XITE_MALLOC_H
#include <xite/message.h>
#include <xite/readarg.h>

#ifndef MAIN

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

		stdiff_line
________________________________________________________________

Name:		stdiff_line
Syntax:		
Description:
Return value:
Author:		Otto Milvang
________________________________________________________________

*/

#define stdiff_pixel {\
 	  mid   = sumx / N;\
          val  = *input++;\
	  stdev = isqrt( (sqsumx - sumx*mid)/N);\
	  val = alpha * m0 + (256-alpha) * mid + \
                (val - mid) * beta * s0 / (s0 + beta * stdev);\
          if (val & ~0x0ffff)\
	    {\
              if (val < 0) val = 0; else val = 0x0ffff;\
	    }\
	  *result++ = val / 256;\
	  }

static void stdiff_line(UNS_BYTE *result, UNS_BYTE *input, int *x, int *x2, int width, int deltax, int deltay, int alpha, int beta, int m0, int s0)
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
    if (i & 1) stdiff_pixel;
  }
  width -= deltax;
  while (width--) {
    sumx   += *xx++;
    sumx   -= *x++;
    sqsumx += *xx2++;
    sqsumx -= *x2++;
    stdiff_pixel;
  }
  while (deltax--) {
    N      -= deltay;
    sumx   -= *x++;
    sqsumx -= *x2++;
    if (deltax & 1) stdiff_pixel;
  }
}



/*F:stdiff*

________________________________________________________________

		stdiff
________________________________________________________________

Name:		stdiff - local statistical differencing

Syntax:         | #include <xite/stdiff.h>
		|
                | void stdiff( IBAND input,
                |    IBAND output, int deltax,
                |    int deltay, double alpha, double beta,
                |    double m0, double s0 );

Description:	Local Pixel Value Mapping.

                | r(x,y) = u(x,y) + s(x,y)
		| u(x,y) = alpha * m0 + (1-alpha) * bm(x,y)
		| s(x,y) = (b(x,y)-bm(x,y)) * (beta*s0 / (s0+bs(x,y)))
		|
		| r(x,y)  = result image
		| u(x,y)  = mean component
		| s(x,y)  = st. dev component
		| m0      = The mean to force locally
		| alpha   = fraction m0 <--> local mean
		| b(b,y)  = pixel value in (x,y)
		| bm(x,y) = local mean
		| bs(x,y) = local st. dev
		| s0	  = new st. dev
		| beta    = parameter to control s0 <--> local st.dev

		'deltax' is local area width, 'deltay' is local area height.

Restrictions:   'input' and 'output' must have pixel type unsigned byte.

Return value:   | 0 : Ok
                | 1 : Bad input pixel type
		| 2 : Bad output pixel type

Reference:	Wayne Niblack: An introduction to Digital Image Processing

Author:		Otto Milvang
________________________________________________________________

*/


int stdiff(IBAND input, IBAND output, int deltax, int deltay, double alpha, double beta, double m0, double s0)
{
  int *x, *x2, i, j, k, l, ialpha,ibeta, im0, is0;
  UNS_BYTE **first, **last;
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
  
  input++; output++;
  ialpha = alpha * 256.0;
  ibeta = beta * 256.0;
  is0   = s0 * 256.0;
  im0   = m0;
  first = last = input; 
  x  = (int *) malloc(width*sizeof(int));
  x2 = (int *) malloc(width*sizeof(int));
  
  for(i=0; i<width; i++) x[i] = x2[i] = 0;
  
  for (i=1; i<= deltay; i++) {
    update_line(*first++, x, x2, width, 1);
    if ( i & 1) stdiff_line(*output++, *input++, x, x2, 
			    width, deltax, i, ialpha, ibeta, im0, is0);
  }
  height -= deltay;
  while(height--) {
    update_line(*first++, x, x2, width, 1);
    update_line(*last++, x, x2, width, 0);
    stdiff_line(*output++, *input++, x, x2, 
		width, deltax, deltay, ialpha, ibeta, im0, is0);
  }
  while(deltay--) {
    update_line(*last++, x, x2, width, 0);
    if (deltay & 1) stdiff_line(*output++, *input++, x, x2, 
				width, deltax, deltay, ialpha, ibeta,
				im0, is0);
    
  }
  free(x);
  free(x2);

  return(0);
}

#endif /* not MAIN */

/*P:stdiff*

________________________________________________________________

		stdiff
________________________________________________________________

Name:		stdiff - local statistical differencing

Syntax:		stdiff <inimage> <outimage> <deltax> <deltay>
                       <alpha> <beta> <m0> <s0>

Description:	Local Pixel Value Mapping.

                | r(x,y) = u(x,y) + s(x,y)
		| u(x,y) = alpha * m0 + (1-alpha) * bm(x,y)
		| s(x,y) = (b(x,y)-bm(x,y)) * (beta*s0 / (s0+bs(x,y)))
		|
		| r(x,y)  = output image
		| u(x,y)  = mean component
		| s(x,y)  = st. dev component
		| m0      = The mean to force locally
		| alpha   = fraction m0 <--> local mean
		| b(b,y)  = pixel value in (x,y)
		| bm(x,y) = local mean
		| bs(x,y) = local st. dev
		| s0	  = new st. dev
		| beta    = parameter to control s0 <--> local st.dev

		'deltax' is local area width, 'deltay' is local area height.

Se also:	Wayne Niblack: An introduction to Digital Image Processing

Restrictions:   'inimage' must have bands with pixel type unsigned byte.

Author:		Otto Milvang

Id: 		$Id$
________________________________________________________________

*/

#ifdef MAIN

int main(int argc, char **argv)
{
  IMAGE input, output;
  int i;
  char *args;

  Iset_message(TRUE);
  Iset_abort(TRUE);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <inimage> <outimage> <deltax> <deltay>\n\
       <alpha> <beta> <m0> <s0>\n"));

  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);

  if (argc != 9) Usage(1, "Illegal number of arguments.\n");

  input  = Iread_image(argv[1]);
  output = Icopy_init(input);  

  for (i=1; i<=Inbands(input); i++)
    stdiff(input[i], output[i],
            atoi(argv[3]), atoi(argv[4]), atof(argv[5]), 
            atof(argv[6]), atof(argv[7]), atof(argv[8]));

  Ihistory(output, argv[0], args);
  Iwrite_image(output, argv[2]);

  return(0);
}
     
#endif /* MAIN */
