

/*

________________________________________________________________

        bin_pa.c
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

/*F:bin_pa*

________________________________________________________________

                bin_pa
________________________________________________________________


Name:           bin_pa - binarize a gray-scale band using Parker`s method

Syntax:         | #include <xite/binarize.h>
                |
                | int bin_pa( IBAND inband, IBAND outband,
                |    int region_size, double k, double mean_lim,
                |    double sdev_lim, int abs_value );

Description:    The input band 'inband' is binarized using the method of
                Parker. The result is returned in 'outband'.

Restrictions:   'inband' and 'outband' must have pixel type unsigned byte.

See also:       binarize(1)

References:     &'J.R. Parker'
                "Gray level thresholding in badly illuminated images",
		IEEE Transactions on Pattern Analysis and
		Machine Intelligence, vol T-PAMI 13, no 8, pp 813-819, 1991.
Return value:   | 0 : ok
                | 1 : Bad input pixel type
		| 2 : Bad output pixel type
Author:         Øivind Due Trier
Id:             $Id$
________________________________________________________________

*/


#include <xite/includes.h>
#include XITE_STDIO_H
#include <math.h>
#include <xite/blab.h>
#include <xite/histo.h>
#include <xite/binarize.h>
#include <xite/message.h>
#include "BinM.h"

#define PA_UNCLASS 255
#define PA_SMOOTH
#define REG_SQRT_05 0.70710678
#include "reg_smooth.c"


static int reg_mean_dev(IR_BAND inband, IR_BAND reg_sdev, IR_BAND reg_mean)
{
  int x, y, rx, ry;
  int xstart, ystart;
  int xend, yend;
  int xsize, ysize;
  int nof_reg_x, nof_reg_y;
  int n;
  float sum;
  float sum2;
  float mean;
  float variance;

  xsize = Ixsize((IBAND)inband);
  ysize = Iysize((IBAND)inband);
  nof_reg_x = Ixsize((IBAND)reg_sdev);
  nof_reg_y = Iysize((IBAND)reg_sdev);

  xend = 0;
 
  for (rx=1; rx<=nof_reg_x; rx++)
    {
      xstart = xend+1;
      xend = (xsize * rx) / nof_reg_x;
      yend = 0;	
      for (ry=1; ry<=nof_reg_y; ry++)
	{
	  ystart = yend+1;
	  yend = (ysize * ry) / nof_reg_y;
	  n = 0;
	  sum = 0;
	  sum2 = 0;
	  for (x=xstart; x<=xend; x++)
	    for (y=ystart; y<=yend; y++)
	      {
		n++;
		sum+= inband[y][x];
		sum2+= inband[y][x]*inband[y][x];
	      }
	  mean = sum * 1.0/n;
	  reg_mean[ry][rx] = mean;
	  variance = (sum2 * 1.0/n) - mean*mean;	
	  reg_sdev[ry][rx] = sqrt(variance);
	}
    }
  return 1;
}





static int pa_min_grad_old(IBAND inband, IR_BAND outband)
{
  int xsize, ysize;
  int x, y, i, j, a, b;
  IBAND I;
  float temp;

  xsize = Ixsize(inband);
  ysize = Iysize(inband);
  I = Imake_band(Iu_byte_typ, xsize+2, ysize+2);
  expand_edge(inband, 0, 0, 1, I);
  
  for (x = 1; x <= xsize; x++)
    for (y = 1; y <= ysize; y++)
      {
	i = x+1;
	j = y+1;
	/* Find largest-valued neigbour */
	temp = 0;
	for (a= -1; a<=1; a++)
	  for (b= -1; b<=1; b++)
	    if ((a!=0)||(b!=0))
	      if (temp < I[j+a][i+b])
		temp = I[j+a][i+b];
	/* miniumum difference between pixel and a neighbour 
	   is the difference between pixel and the largest neighbour */
	outband[y][x] = inband[y][x] - temp;
      }
  Idel_band(&I);
  return 1;
}

static int pa_min_grad(IBAND inband, IR_BAND outband)
{
  int xsize, ysize;
  int x, y, i, j, a, b;
  IBAND I;
  float temp, diff;

  xsize = Ixsize(inband);
  ysize = Iysize(inband);
  I = Imake_band(Iu_byte_typ, xsize+2, ysize+2);
  expand_edge(inband, 0, 0, 1, I);
  
  for (x = 1; x <= xsize; x++)
    for (y = 1; y <= ysize; y++)
      {
	i = x+1;
	j = y+1;
	/* Find closest neigbour */
	diff = 255.0;
	for (a= -1; a<=1; a++)
	  for (b= -1; b<=1; b++)
	    if ((a!=0)||(b!=0))
	      if (temp > (diff = fabs(I[j][i]-I[j+a][i+b])))
		temp = diff;
	outband[y][x] = temp;
      }
  Idel_band(&I);
  return 1;
}

/*
static int pa_mean_dev(IR_BAND gradient, IR_BAND qdev, IR_BAND qmean)
{
  int x, y, rx, ry;
  int xstart, ystart;
  int xend, yend;
  int xsize, ysize;
  int nof_reg_x, nof_reg_y;
  int n;
  float sum;
  float sum_dev;
  float mean;
  float mean_dev;

  xsize = Ixsize(gradient);
  ysize = Iysize(gradient);
  nof_reg_x = Ixsize(qdev);
  nof_reg_y = Iysize(qdev);

  xend = 0;
 
  for (rx=1; rx<=nof_reg_x; rx++)
    {
      xstart = xend+1;
      xend = (xsize * rx) / nof_reg_x;
      yend = 0;	
      for (ry=1; ry<=nof_reg_y; ry++)
	{
	  ystart = yend+1;
	  yend = (ysize * ry) / nof_reg_y;
	  n = 0;
	  sum = 0;
	  sum_dev = 0;
	  for (x=xstart; x<=xend; x++)
	    for (y=ystart; y<=yend; y++)
	      {
		n++;
		sum+= gradient[y][x];
	      }
	  for (x=xstart; x<=xend; x++)
	    for (y=ystart; y<=yend; y++)
		sum_dev+= gradient[y][x];
	  mean = sum * 1.0/n;
	  qmean[ry][rx] = mean;
	  variance = (sum2 * 1.0/n) - mean*mean;	
	  qdev[ry][rx] = sqrt(variance);
	}
    }
  return 1;
}
*/



static int pa_pix_aggreg(IBAND grey, IBAND binary)
{
  int count = 0;
  int x, y, xsize, ysize;
  int a, b, i, j;
  int threshold;
  IBAND tempbin;
  IBAND tempgrey;

  xsize = Ixsize(grey);
  ysize = Iysize(grey);

  tempbin = Imake_band(Iu_byte_typ, xsize+2, ysize+2);
  tempgrey = Imake_band(Iu_byte_typ, xsize+2, ysize+2);
  expand_edge(binary, 0, 0, 1, tempbin);
  expand_edge(grey, 0, 0, 1, tempgrey);

  for (x=1; x<=xsize; x++)
    for (y=1; y<=ysize; y++)
      {
	i=x+1;
	j=y+1;
	if(tempbin[j][i] == PA_UNCLASS)
	  {
	    threshold = 256;
	    for (a= -1; a<=1; a++)
	      for (b= -1; b<=1; b++)
		if ((a!=0)||(b!=0))
		  if(tempbin[j+a][i+b] == BIN_FORGROUND)
		    threshold = MIN(threshold, tempgrey[j+a][i+b]);
	    if ((threshold < 256) && (tempgrey[j][i] < threshold))
	      {
		binary[y][x] = BIN_FORGROUND;
		count++;
	      }
	  }
      }
  
  Idel_band (&tempbin);
  Idel_band (&tempgrey);
  return count;
}

static int pa_reg_grow_mean(IBAND grey, IBAND binary)
{
  int count = 0;
  int x, y, xsize, ysize;
  int a, b, i, j;
  IBAND tempbin;
  IBAND tempgrey;

  int n;
  int sum;
  
  xsize = Ixsize(grey);
  ysize = Iysize(grey);

  tempbin = Imake_band(Iu_byte_typ, xsize+2, ysize+2);
  tempgrey = Imake_band(Iu_byte_typ, xsize+2, ysize+2);
  expand_edge(grey, 0, 0, 1, tempgrey);
  expand_edge(binary, 0, 0, 1, tempbin);

  for (x=1; x<=xsize; x++)
    for (y=1; y<=ysize; y++)
      {
	i=x+1;
	j=y+1;
	if(tempbin[j][i] == PA_UNCLASS)
	  {
	    n = 0;
	    sum = 0;
	    for (a= -1; a<=1; a++)
	      for (b= -1; b<=1; b++)
		if ((a!=0)||(b!=0))
		  if(tempbin[j+a][i+b] == BIN_FORGROUND)
		    {
		      n++;
		      sum += tempgrey[j+a][i+b];
		    }
	    if ((n >= 4) && (tempgrey[j][i] < (sum*1.0/n)))
	      {
		binary[y][x] = BIN_FORGROUND;
		count++;
	      }
	  }
      }

  Idel_band (&tempbin);
  Idel_band (&tempgrey);
  return count;
}

static int pa_reg_grow_max(IBAND grey, IBAND binary)
{
  int count = 0;
  int x, y, xsize, ysize;
  int a, b, i, j;
  IBAND tempbin;
  IBAND tempgrey;

  int n;
  int threshold;
  
  xsize = Ixsize(grey);
  ysize = Iysize(grey);

  tempbin = Imake_band(Iu_byte_typ, xsize+2, ysize+2);
  tempgrey = Imake_band(Iu_byte_typ, xsize+2, ysize+2);
  expand_edge(grey, 0, 0, 1, tempgrey);
  expand_edge(binary, 0, 0, 1, tempbin);

  for (x=1; x<=xsize; x++)
    for (y=1; y<=ysize; y++)
      {
	i=x+1;
	j=y+1;
	if(tempbin[j][i] == PA_UNCLASS)
	  {
	    n = 0;
	    threshold = 0;
	    for (a= -1; a<=1; a++)
	      for (b= -1; b<=1; b++)
		if ((a!=0)||(b!=0))
		  if(tempbin[j+a][i+b] == BIN_FORGROUND)
		    {
		      n++;
		      threshold = MAX (threshold, tempgrey[j+a][i+b]);
		    }
	    if ((n >= 6) && (tempgrey[j][i] < threshold))
	      {
		binary[y][x] = BIN_FORGROUND;
		count++;
	      }
	  }
      }

  Idel_band (&tempbin);
  Idel_band (&tempgrey);
  return count;
}


int bin_pa (IBAND inband, IBAND outband, int region_size, double k,
	    double mean_lim, double sdev_lim, int abs_value)
{
  int count;
  int xsize, ysize;
  int x, y;
  int nof_regions_x;
  int nof_regions_y;
  IR_BAND min_grad;
  IR_BAND qdev;
  IR_BAND qmean;
  IR_BAND qdev1;
  IR_BAND qmean1;
  IR_BAND qdev2;
  IR_BAND qmean2;

  if (Ipixtyp(inband) != Iu_byte_typ)
    return(Error(1, "bin_pa: Input pixel type must be unsigned byte.\n"));
  if (Ipixtyp(outband) != Iu_byte_typ)
    return(Error(2, "bin_pa: Output pixel type must be unsigned byte.\n"));

  xsize = Ixsize(inband);
  ysize = Iysize(inband);
  nof_regions_x = xsize / region_size ;
  nof_regions_y = ysize / region_size ;


  if (Verbose()) {
    printf("\nParker's method\n");
    printf("Reccognized parameters:\n");
    printf("[-r]   Region size                 = %3i\n", region_size);
    printf("[-k]   weight                      = %6.2f\n", k);
    printf("[-m]   mean limit                  = %6.2f\n", mean_lim);
    printf("[-s]   standard deviation limit    = %6.2f\n", sdev_lim);
    printf("[-abs] Use abs.value in LIG        = %3i\n", abs_value);

    printf("Initializing all bands...\n");
  }

  min_grad = (IR_BAND) Imake_band(Ireal_typ, xsize, ysize);
  qdev   = (IR_BAND) Imake_band(Ireal_typ, xsize, ysize);
  qmean  = (IR_BAND) Imake_band(Ireal_typ, xsize, ysize);
  qdev1  = (IR_BAND) Imake_band(Ireal_typ, nof_regions_x, nof_regions_y);
  qmean1 = (IR_BAND) Imake_band(Ireal_typ, nof_regions_x, nof_regions_y);
  qdev2  = (IR_BAND) Imake_band(Ireal_typ, nof_regions_x, nof_regions_y);
  qmean2 = (IR_BAND) Imake_band(Ireal_typ, nof_regions_x, nof_regions_y);

  if (abs_value)
    pa_min_grad(inband, min_grad);
  else
    pa_min_grad_old(inband, min_grad);
/*  write_floatband(min_grad, "Pa_grad.img");*/

  reg_mean_dev(min_grad, qdev1, qmean1);
/*  write_floatband(qdev1, "Pa_qdev1.img");*/
/*  write_floatband(qmean1, "Pa_qmean1.img");*/

  reg_smooth(qdev1, qdev2);
  reg_smooth(qmean1, qmean2);
/*  write_floatband(qdev2, "Pa_qdev2.img");*/
/*  write_floatband(qmean2, "Pa_qmean2.img");*/
  Idel_band((IBAND *)&qdev1);
  Idel_band((IBAND *)&qmean1);
  
  reg_interpolate(qmean2, qmean);
  reg_interpolate(qdev2, qdev);
/*  write_floatband(qdev, "Pa_qdev.img");*/
/*  write_floatband(qmean, "Pa_qmean.img");*/
  Idel_band((IBAND*)&qdev2);
  Idel_band((IBAND*)&qmean2);


  for (x = 1; x <= xsize; x++)
    for (y = 1; y <= ysize; y++)
      if ((qmean[y][x] >= mean_lim) || (qdev[y][x] <= sdev_lim))
	outband[y][x] = PA_UNCLASS;
      else if (min_grad[y][x] <= qmean[y][x] + k * qdev[y][x])
	outband[y][x] = BIN_FORGROUND;
      else
	outband[y][x] = PA_UNCLASS;
/*  write_byteband(outband, "segmented_1.img");  */

  if (Verbose()) {
    while ((count = pa_pix_aggreg(inband, outband)))
      printf("%i pixels aggregated.\n", count);
    /*  write_byteband(outband, "pix_aggreg.img");  */

    while ((count = pa_reg_grow_mean(inband, outband)))
      printf("%i pixels in reg.grow (mean of 4).\n", count);
    /*  write_byteband(outband, "reg_grow_mean.img");  */

    while ((count = pa_reg_grow_max(inband, outband)))
      printf("%i pixels in reg.grow (max of 6).\n", count);
  }

  Idel_band((IBAND*)&min_grad);
  Idel_band((IBAND*)&qdev);
  Idel_band((IBAND*)&qmean);

  return 0;
}
