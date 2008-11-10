

/*

________________________________________________________________

        fft2d.c
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
#include <stdlib.h>
#include <xite/fft.h>
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#include <xite/blab.h>
#include <xite/convert.h>
#include "fft_L.h"
#include <xite/geometry.h>
#include <xite/message.h>
#include <xite/readarg.h>
#ifdef HAVE_MALLOC_H
# include <malloc.h>
#endif



/*L*

________________________________________________________________

		fft_ft_hor, fft_ft_vert
________________________________________________________________

Name:		fft_ft_hor
Syntax:		HIDDEN
Description:
Return value:
Author:		Otto Milvang
________________________________________________________________

*/


#define f_hor(hor, I_BAND, complex, fft)                       	\
static void hor(I_BAND in_band, I_BAND out_band, complex in[],  \
		complex out[], int xsize, int ysize)            \
{								\
  int x, y;   						        \
  complex *ptr1, *ptr2;						\
  for (y=1; y <= ysize; y++)					\
    {								\
      ptr1 = &in[0];						\
      ptr2 = &in_band[y][1];					\
      for (x = 1; x <= xsize; x++)				\
	*ptr1++ = *ptr2++;					\
      fft((float *) in, xsize);					\
      ptr1 = &in[0]; 						\
      ptr2 = &out_band[y][1];					\
      for (x = 1; x <= xsize; x++)				\
	*ptr2++  = *ptr1++;					\
    }								\
}


#define f_ver(ver, I_BAND, complex, fft)                        \
static void ver(I_BAND in_band, I_BAND out_band, complex in[],  \
		complex out[], int xsize, int ysize)		\
{								\
  int x, y;						        \
  complex *ptr1;						\
  for (x=1; x <= xsize; x++)					\
    {								\
      ptr1 = &in[0];						\
      for (y = 1; y <= ysize; y++)				\
	*ptr1++ = in_band[y][x];				\
      fft((float *) in, ysize);					\
      ptr1 = &in[0];						\
      for (y = 1; y <= ysize; y++)				\
	out_band[y][x] = *ptr1++;				\
    }								\
}

#define f_copy(copy, I_BAND)					\
static void copy(I_BAND in_band, I_BAND out_band, int inverse)  \
{								\
  int x, y, xsize, ysize;					\
  xsize = Ixsize((IBAND) in_band);				\
  ysize = Iysize((IBAND) in_band);				\
  if (in_band == out_band)					\
    {								\
      if(inverse)						\
	for(y=1; y <= ysize; y++)				\
	  for(x=1; x <= xsize; x++)				\
	    in_band[y][x].im = -in_band[y][x].im;		\
    } else {							\
      if(inverse)						\
	for(y=1; y <= ysize; y++)				\
	  for(x=1; x <= xsize; x++)				\
	    {							\
	      out_band[y][x].re =  in_band[y][x].re;		\
	      out_band[y][x].im = -in_band[y][x].im;		\
	    }							\
      else							\
	for(y=1; y <= ysize; y++)				\
	  for(x=1; x <= xsize; x++)				\
	    out_band[y][x] = in_band[y][x];			\
    }								\
}  

#define f_scale(scal, I_BAND)					\
static void scal(I_BAND in_band, double scale)			\
{								\
  int x, y, xsize, ysize;					\
  xsize = Ixsize((IBAND) in_band);				\
  ysize = Iysize((IBAND) in_band);				\
  if (scale != 1.0)						\
    for(y=1; y <= ysize; y++)					\
      for(x=1; x <= xsize; x++)					\
	{							\
	  in_band[y][x].re = in_band[y][x].re * scale;		\
	  in_band[y][x].im = in_band[y][x].im * scale;		\
	}							\
}  

f_hor(fft_f_hor, IC_BAND,  COMPLEX,  fft)
f_ver(fft_f_ver, IC_BAND,  COMPLEX,  fft)
f_copy(fft_f_copy, IC_BAND)
f_scale(fft_f_scale, IC_BAND)



/*F:fft=fft2d*/
/*F:fft2d*

________________________________________________________________

		fft2d
________________________________________________________________

Name:		fft2d, fft - Two dimensional Fourier Transform

Syntax:         | #include <xite/fft.h>
		|
                | int fft2d( IBAND in_band, IBAND out_band,
                |    int inverse, double scale );

Description:    Discrete Fourier Transform.

		If 'inverse' = FALSE (0), a forward transform is
		performed, otherwise an inverse transform is performed.

		'scale' is used to scale the output image. Set 'scale'
		to '1.0' if you don''t want scaling. Set 'scale' to
		'0.0' if you want standard scaling (1/(xsize*ysize) in
		forward transform and no scaling in inverse transform).

		The forward and inverse discrete Fourier transforms
		with standard scaling are given respectively as:

		|                                       
		|           1    M   N          -j*2*pi*(ux/M + vy/N)
		| F(u,v) = ---  Sum Sum f(x,y) e
		|          M*N  x=0 y=0
		|
		|           M   N          j*2*pi*(ux/M + vy/N)
		| f(x,y) = Sum Sum F(u,v) e
		|          u=0 v=0
		|
		where M=Ixsize(f), N=Iysize(f).

Restrictions:   'in_band' and 'out_band' must be of the same size and type
                Icomplex_typ. Horizontal and vertical band size must be
		powers of two.

Return value:	| 0 - ok
                | 1 - Bad pixel type
		| 2 - Bad size

See also:       fft2d(1), fftMagPhase(1), absValue(3), power(3), phase(3),
                real(3), imag(3), multiply(3)

Author:		Otto Milvang
Doc:            Otto Milvang and Svein Bøe
________________________________________________________________

*/

static int fft_fsincos(int length)
{
  int i, pow;

  i = 1; pow=0;
  while (i < length)
    {
      i <<= 1;
      pow++;
    }
  if (i != length) pow=0;
  return(pow);
}

BiffStatus fft2d(IBAND in_band, IBAND out_band, int inverse, double scale)
{
  int xsize, ysize, pixtyp;
  COMPLEX *in, *out;

  /* Get info */
  xsize =  Ixsize (in_band);
  ysize =  Iysize (in_band);
  pixtyp = Ipixtyp(in_band);

  if (pixtyp != Icomplex_typ)
    return(Error(1, "fft2d: Input is not pixeltype complex.\n"));
  if (Ipixtyp(out_band) != pixtyp)
    return(Error(1, "fft2d: Output is not pixeltype complex.\n"));
  if (xsize != Ixsize(out_band))
    return(Error(2,
		 "fft2d: Input and output have different horizontal size.\n"));
  if (ysize != Iysize(out_band))
    return(Error(2,
		 "fft2d: Input and output have different vertical size.\n"));
  if (fft_fsincos(xsize) <= 0)
    return(Error(2, "fft2d: Horizontal size is not power of 2.\n"));
  if (fft_fsincos(ysize) <= 0)
    return(Error(2, "fft2d: Vertical size is not power of 2.\n"));

  in  = (COMPLEX*) malloc(Ipixsize(pixtyp)*(xsize > ysize ? xsize : ysize));
  out = (COMPLEX*) malloc(Ipixsize(pixtyp)*(xsize > ysize ? xsize : ysize));

  if (scale == 0.0) 
    scale = (inverse ? 1.0 : 1.0 / ( (double) xsize * (double) ysize));

  fft_f_copy((IC_BAND) in_band, (IC_BAND) out_band, inverse);
  fft_f_hor((IC_BAND) out_band, (IC_BAND) out_band, in, out, xsize, ysize);

  if (xsize != ysize) 
    {
      fft_f_ver((IC_BAND) out_band, (IC_BAND)out_band, in, out, xsize, ysize); 
    } else {
      transpose_band(out_band);
      fft_f_hor((IC_BAND) out_band, (IC_BAND) out_band, in, out, xsize, ysize);
      transpose_band(out_band);
    }
  fft_f_scale((IC_BAND) out_band, scale);
  free(in);
  free(out);
  return(0);
}
