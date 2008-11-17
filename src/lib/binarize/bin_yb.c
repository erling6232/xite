

/*

________________________________________________________________

        bin_yb.c
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

/*F:bin_yb*

________________________________________________________________

                bin_yb
________________________________________________________________


Name:           bin_yb - binarize a gray-scale band using 
                Yanowitz and Bruckstein`s method

Syntax:         | #include <xite/binarize.h>
                |
                | int bin_yb( IBAND inband, IBAND outband,
                |    int threshold, double beta,
                |    int max_iterations );

Description:    The input band 'inband' is binarized using the method of
                Yanowitz and Bruckstein. The result is returned in 'outband'.

		A potential surface passing through the image at local maxima
		of the gradient image is used as a thresholding surface. The
		gradient magnitude image is computed, using for example
		Sobel`s edge operators. This image is thresholded using
		'threshold'. However, if 'threshold' == 0, Otsu`s method is
		used. The thresholded image is thinned to one-pixel wide
		lines, to identify edge pixels. The original image is smoothed
		by a (3x3) mean filter to remove stochastic noise. A potential
		surface is constructedby an iterative interpolation scheme.
		The interpolation surface is made to pass through the edge
		pixels. An interpolation residual R(x,y) is calculated for
		each non-edge pixel, and the new pixel value P(x,y) at
		iteration n is calculated as

		|  P[n](x,y) = P[n-1](x,y) + (beta * R[n](x,y)) / 4

		Yanowitz and Bruckstein recommended to use 1.0 < 'beta' < 2.0
		for fast convergence (over-relaxation). However, we found that
		by using 'beta' > 1.0, P(x,y) started to diverge after 5-10
		iterations. So 'beta' <= 1.0 must be used, and 'beat' = 1.0
		works well. A maximum number of 'max_iterations' is perfromed.
		
		Next, the image is binarized using the thresholding surface.
		In the original algorithm, so-called "ghost" objects are
		removed by a validation step. This validation step is in a
		separate library function named 'rem_ghost'.

Restrictions:   'inband' and 'outband' must have pixel type unsigned band.

See also:       binarize(1)

References:     &'S.D. Yanowitz & A.M. Bruckstein'
                "A New Method for Image Segmentation",
                Comput. Vision Graphics Image Proc., vol 46, no 1, pp 82-95,
                April 1989.
                
Return value:   | 0 : ok
                | 1 : Bad input pixel type
		| 2 : Bad output pixel type

Files:  	src/binarize/bin_yb.c
Author:         Øivind Due Trier
Id:             $Id$
________________________________________________________________

*/



#include <math.h>
#include <xite/includes.h>
#include XITE_MALLOC_H
#include <xite/blab.h>
#include <xite/biff.h>
#include <xite/histo.h>
#include <xite/stdiff.h>
#include <xite/convolve.h>
#include <xite/binarize.h>
#include <xite/thin.h>
#include <xite/message.h>
/* #include "write_band.c" */

#define YB_R_LIMIT 0.1

#define YB_MAX_SIZE 32768


static int init_pot_surf(IBAND inband, IR_BAND outband, IBAND fixed_points)
{
  int x, y;
  int x1, x2, y1, y2;
  int xsize, ysize;
  IR_BAND hor, ver;


  xsize = Ixsize(inband);
  ysize = Iysize(inband);

  hor = (IR_BAND) Imake_band(Ireal_typ, xsize, ysize);
  ver = (IR_BAND) Imake_band(Ireal_typ, xsize, ysize);

  /* The boundary points are held fixed. */
  for (x=1; x<=xsize; x++)
    {
      outband[1][x]     = inband[1][x];
      outband[ysize][x] = inband[ysize][x];
    }
  for (y=1; y<=ysize; y++)
    {
      outband[y][1]     = inband[y][1];
      outband[y][xsize] = inband[y][xsize];
    }

  for (y=2; y<ysize; y++)
    {
      x1 = 1;
      x2 = 2;
      while (x2 < xsize)
	{
	  while ((fixed_points[y][x2] != BIN_FORGROUND) && (x2 < xsize))
	    x2++;
	  hor[y][x1] = inband[y][x1];
	  hor[y][x2] = inband[y][x2];
	  for (x=x1+1; x<x2; x++)
	    hor[y][x] = ((x2-x)*hor[y][x1] + (x-x1)*hor[y][x2])/(x2-x1);
	  
	  x1 = x2;
	  x2++;
	}
    }

  for (x=2; x<xsize; x++)
    {
      y1 = 1;
      y2 = 2;
      while (y2 < ysize)
	{
	  while ((fixed_points[y2][x] != BIN_FORGROUND) && (y2 < ysize))
	    y2++;
	  ver[y1][x] = inband[y1][x];
	  ver[y2][x] = inband[y2][x];
	  for (y=y1+1; y<y2; y++)
	    ver[y][x] = ((y2-y)*ver[y1][x] + (y-y1)*ver[y2][x])/(y2-y1);
	  
	  y1 = y2;
	  y2++;
	}
    }

  for (x=2; x<xsize; x++)
    for (y=2; y<ysize; y++)
      outband[y][x] = (hor[y][x] + ver[y][x]) / 2.0;

  return BIN_OK;
}



static int interp_pot_surf(IBAND inband, IBAND outband, IBAND fixed_points, 
		    double beta, int max_iterations)
{
  int x, y;
  int xsize, ysize;
  IR_BAND tempband1;
  IR_BAND tempband2;
  double R_max = 10.0;
  double R = 0.0;
  int iterations = 0;

  xsize = Ixsize(inband);
  ysize = Iysize(inband);

  tempband1 = (IR_BAND) Imake_band(Ireal_typ, xsize, ysize);
  tempband2 = (IR_BAND) Imake_band(Ireal_typ, xsize, ysize);


  /* Initate the temporary double-valued band. */
  init_pot_surf(inband, tempband1, fixed_points);

/*  write_floatband(tempband1, "YB_init_surf.img");*/

  iterations = 0;
  while ((fabs(R_max) > YB_R_LIMIT) && (iterations < max_iterations))
    {
      iterations ++;
/*
      if (iterations == 25)
	beta = beta * 0.7;
*/
      R_max = 0.0;

      /* The boundary points are held fixed */
      for (x=1; x<=xsize; x++)
	{
	  tempband2[1][x]     = tempband1[1][x];
	  tempband2[ysize][x] = tempband1[ysize][x];
	}
      for (y=1; y<=ysize; y++)
	{
	  tempband2[y][1]     = tempband1[y][1];
	  tempband2[y][xsize] = tempband1[y][xsize];
	}

      for (x=2; x<xsize; x++)
	for (y=2; y<ysize; y++)
	  {
	    if (fixed_points[y][x] != BIN_FORGROUND)
	      {
		R = tempband1[y+1][x] + tempband1[y-1][x] 
		  + tempband1[y][x+1] + tempband1[y][x-1] 
		    - 4.0 * tempband1[y][x];
		tempband2[y][x] = tempband1[y][x] + (beta * R)/4.0;
		if (fabs(R) > fabs(R_max))
		  R_max = R;
	      }
	    else
	      tempband2[y][x] = tempband1[y][x];
	  }
      if (Verbose())
	printf("  --- Iteration %2i, R_max = %6.2f\n", iterations, R_max);
      for (x=1; x<=xsize; x++)
	for (y=1; y<=ysize; y++)	
	  tempband1[y][x] = tempband2[y][x];
    }
  
  /* Copy the temporary double-valued band to the unsigned-byte-valued 
     output band. */

  if (Verbose()) printf("  --- Copy back\n");
  for (x=1; x<=xsize; x++)
    for (y=1; y<=ysize; y++)	
      outband[y][x] = (UNS_BYTE)tempband2[y][x];

  return 1;
}





static int Interp_edge (IBAND thinned, IBAND smoothed)
{
  int x, y, i, j, k;
  double delta;
  int xsize, ysize;
  UNS_BYTE *edge;
  UNS_BYTE *temp;
  UNS_BYTE *flag;
  int start = 0;
  int length;
  
  xsize = Ixsize(smoothed);
  ysize = Iysize(smoothed);

  if (xsize != Ixsize(thinned))
    Warning(1, "Warning: smoothed.xsize = %i, thinned.xsize = %li\n", 
	    xsize, Ixsize(thinned));
  if (ysize != Iysize(thinned))
    Warning(1, "Warning: smoothed.ysize = %i, thinned.ysize = %li\n", 
	    ysize, Iysize(thinned));
      
  length = 2*xsize + 2*ysize - 4;

  if ((edge = (UNS_BYTE*)malloc(sizeof(UNS_BYTE) * (length+1)))==NULL)
    return -1;
  if ((flag = (UNS_BYTE*)malloc(sizeof(UNS_BYTE) * (length+1)))==NULL)
    return -1;
  if ((temp = (UNS_BYTE*)malloc(sizeof(UNS_BYTE) * (length+1)))==NULL)
    return -1;
  
  i = 0;
  for (x=1; x<=xsize; x++)
    {
      edge[i] = smoothed[1][x];
      flag[i] = thinned [1][x];
      i++;
    }
  for (y=2; y<ysize; y++)
    {
      edge[i] = smoothed[y][xsize];
      flag[i] = thinned [y][xsize];
      i++;
    }
  for (x=xsize; x>=1; x--)
    {
      edge[i] = smoothed[ysize][x];
      flag[i] = thinned [ysize][x];
      i++;
    }
  for (y=ysize-1; y>1; y--)
    {
      edge[i] = smoothed[y][1];
      flag[i] = thinned [y][1];
      i++;
    }

  if (i != length)
    Warning(1, "Warning! Index i = %i, length = %i\n", i, length);

/*
  printf ("Input edge \n");
  printf ("\n%i:", 0);
  for (i=0; i<=length; i++)
    {
      if (flag[i]== BIN_FORGROUND)
	printf (">%3i", edge[i]);
      else
	printf (" %3i", edge[i]);
      if (i%15 == 14)
	printf ("\n%i:", i+1);
    }
  printf ("\n");
*/

  start = 0;
  while ((flag[start]!=BIN_FORGROUND) && (start < length))
    start++;

  if (start == length)
    {
      /* No edges on border --- */
      ;
    }
  else
    {
      /* Adjust edge indexing so that a flagged point is at index 0 */
      /* Also, append a copy of this flagged point at the end. */
      j = 0;
      for (i=start; i<length; i++)
	{
	  temp[j] = edge[i];
	  j++;
	}
      for (i=0; i<start; i++)
	{
	  temp[j] = edge[i];
	  j++;
	}
      for (i=0; i<length; i++)
	edge[i] = temp[i];
      edge[length] = edge[0];
      j = 0;
      for (i=start; i<length; i++)
	{
	  temp[j] = flag[i];
	  j++;
	}
      for (i=0; i<start; i++)
	{
	  temp[j] = flag[i];
	  j++;
	}
      for (i=0; i<length; i++)
	flag[i] = temp[i];
      flag[length] = flag[0];

      /* Interpolate */
      i = 0;
      j = 1;
      while((flag[j] != BIN_FORGROUND) && (j <length))
	j++;
      
      while (i < length)
	{	
	  delta = 1.0 * ((int) edge[j] - (int)edge[i]) / (j-i);
	  for (k=1; k<j-i; k++)
	    edge[i+k] = (UNS_BYTE)((double)edge[i] + (k * delta));
	  i = j;
	  j++;
	  while((flag[j] != BIN_FORGROUND) && (j <length))
	    j++;
	}
      
      /* Adjust edge back to original addressing */
      j = 0;
      for (i=length-start; i<length; i++)
	{
	  temp[j] = edge[i];
	  j++;
	}
      for (i=0; i<length-start; i++)
	{
	  temp[j] = edge[i];
	  j++;
	}
      for (i=0; i<length; i++)
	edge[i] = temp[i];
      edge[length] = edge[0];

      /* Adjust flag back to original addressing */
      j = 0;
      for (i=length-start; i<length; i++)
	{
	  temp[j] = flag[i];
	  j++;
	}
      for (i=0; i<length-start; i++)
	{
	  temp[j] = flag[i];
	  j++;
	}
      for (i=0; i<length; i++)
	flag[i] = temp[i];
      flag[length] = flag[0];

/*
      printf ("Output edge : \n");
      printf ("\n%i:", 0);
      for (i=0; i<=length; i++)
	{
	  if (flag[i]== BIN_FORGROUND)
	    printf (">%3i", edge[i]);
	  else
	    printf (" %3i", edge[i]);
	  if (i%15 == 14)
	    printf ("\n%i:", i+1);

	}
      printf ("\n");
*/
      
      /* Copy the interpolated edge back to the image */
      i = 0;
      for (x=1; x<=xsize; x++)
	{
	  smoothed[1][x] = edge[i];
	  i++;
	}
      for (y=2; y<ysize; y++)
	{
	  smoothed[y][xsize] = edge[i];
	  i++;
	}
      for (x=xsize; x>=1; x--)
	{
	  smoothed[ysize][x] = edge[i];
	  i++;
	}	
      for (y=ysize-1; y>1; y--)
	{
	  smoothed[y][1] = edge[i];
	  i++;
	}
      
    }

  return 1;
}

int bin_yb (IBAND inband, IBAND outband, int threshold,
	    double beta, 
	    int max_iterations)
{
  IBAND magnitude, thresholded;
  IBAND thinned, thr_surface;
  IBAND smoothed, angle, pseudo;

  int xsize, ysize;
  int x, y;
  double dummy1, dummy2, dummy3;
  
  if (Ipixtyp(inband) != Iu_byte_typ)
    return(Error(1, "bin_yb: Input pixel type must be unsigned byte.\n"));
  if (Ipixtyp(outband) != Iu_byte_typ)
    return(Error(2, "bin_yb: Output pixel type must be unsigned byte.\n"));

  xsize = Ixsize(inband);
  ysize = Iysize(inband);
  
  magnitude   = (IBAND)Imake_band(Iu_byte_typ, xsize, ysize);
  thresholded = (IBAND)Imake_band(Iu_byte_typ, xsize, ysize);
  thinned     = (IBAND)Imake_band(Iu_byte_typ, xsize, ysize);
  thr_surface = (IBAND)Imake_band(Iu_byte_typ, xsize, ysize);
  
  smoothed = Imake_band(Iu_byte_typ, xsize, ysize);
  angle    = Imake_band(Iu_byte_typ, xsize, ysize);
  pseudo   = Imake_band(Iu_byte_typ, xsize, ysize);
  
  if (Verbose()) printf("mean (inband) -> smoothed\n");
  mean (inband, smoothed, 3, 3);
  if (Verbose()) printf("Sobel (smoothed) -> magnitude\n");
  sobel (smoothed, magnitude, angle, pseudo, 0);
  Idel_band (&angle);
  Idel_band (&pseudo);
  
/*  write_byteband(magnitude, "YB_edge.img");
*/
  
  if (threshold == 0)
    threshold = otsu_threshold(magnitude, &dummy1, &dummy2, &dummy3);
  
  if (Verbose())
    printf("Threshold (magnitude,%3i) -> thresholded\n", threshold);
  for (x = 1; x <= xsize; x++)
    for (y = 1; y <= ysize; y++)
      if (magnitude[y][x] < threshold)
	thresholded[y][x] = 255;
      else	
	thresholded[y][x] = 0;

/*  write_byteband(thresholded, "YB_thres.img");
*/

  if (Verbose()) printf("thn_lc(thresholded) -> thinned\n");
  thn_lc(0, thresholded, thinned); 
  
/*  write_byteband (thinned, "YB_thinned.img");
*/  
  if (Verbose()) printf("interp_edge(thinned) -> smoothed\n");
  Interp_edge (thinned, smoothed);

  if (Verbose()) printf("interp_pot_surf(smoothed, thinned) -> thr_surface\n");
  interp_pot_surf (smoothed, thr_surface, thinned, beta, max_iterations);
  Idel_band (&smoothed);

  if (Verbose()) printf("Output image YB_thr_surface.img\n"); 
/*  write_byteband (thr_surface, "YB_thr_surface.img");
*/

  if (Verbose()) printf("Threshold image using thresholding surface.\n"); 
  for (x = 1; x <= xsize; x++)
    for (y = 1; y <= ysize; y++)
      {
	if (inband[y][x] <= thr_surface[y][x])
	  outband[y][x] = 0;
	else	
	  outband[y][x] = 255;	  
      }
  if (Verbose()) printf("Returning.\n"); 

  Idel_band (&magnitude);
  Idel_band (&thresholded);
  Idel_band (&thinned);
  Idel_band (&thr_surface);

  return 0;
}
