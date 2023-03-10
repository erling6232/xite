/*C*


________________________________________________________________

        glcmParameter
        $Id$
        Copyright 1991, Blab, UiO
        Image processing lab, Department of Informatics
        University of Oslo
        E-mail: blab@uio.no
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
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/arithmetic.h>
#include <xite/histo.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include <xite/texture.h>
#include <xite/draw.h>
#include XITE_MALLOC_H



/*F:glcmAsm=glcmParameter*/
/*F:glcmCont=glcmParameter*/
/*F:glcmCorr=glcmParameter*/
/*F:glcmIdm=glcmParameter*/
/*F:glcmEntropy=glcmParameter*/
/*F:glcmVar=glcmParameter*/
/*F:glcmProm=glcmParameter*/
/*F:glcmShade=glcmParameter*/
/*F:glcmInertia=glcmParameter*/
/*F:glcmDiagmoment=glcmParameter*/
/*F:glcmParameter*

________________________________________________________________

		glcmParameter
________________________________________________________________

Name:		glcmParameter, glcmAsm, glcmCont, glcmCorr, glcmIdm,
                glcmEntropy, glcmVar, glcmProm, glcmShade, glcmInertia,
                glcmDiagmoment - Computes features from the Grey Level
                Co-occurrence Matrix (GLCM)

Syntax:         | #include <xite/texture.h>
		|
                | double glcmAsm( ID_BAND inband );
		|
                | double glcmCont( ID_BAND inband );
		|
                | double glcmCorr( ID_BAND inband );
		|
                | double glcmIdm( ID_BAND inband );
		|
                | double glcmEntropy( ID_BAND inband );
		|
                | double glcmVar( ID_BAND inband );
		|
                | double glcmProm( ID_BAND inband );
		|
                | double glcmShade( ID_BAND inband );
		|
                | double glcmInertia( ID_BAND inband );
		|
                | double glcmDiagmoment( ID_BAND inband );

Description:    The input parameters are normalized bands.

                &glcmAsm
		Calculates Angular Second Moment of a GLCM. See reference 1.

                &glcmCont
		Calculates Contrast of a GLCM. See reference 3.

		&glcmCorr
		Calculates Correlation of a GLCM. See reference 1.

		&glcmIdm
		Calculates Inverse Difference Moment of a GLCM. See
		reference 1.

		&glcmEntropy
		Calculates Entropy of a GLCM. See reference 1.

                &glcmVar
		Calculates Variance of a GLCM. See reference 1.

		&glcmProm
		Calculates Cluster Prominence of a GLCM. See reference 2.

		&glcmShade
		Calculates Cluster Shade of a GLCM. See reference 2.

		&glcmInertia
		Calculates Inertia of a GLCM. See reference 2.

                &glcmDiagmoment
		Calculates Diagonal moment of a GLCM.

Restrictions:   'inband' must have pixel type double.

See also:       glcm(3)

Reference: 	&R. M. Haralick et al.
                "Textural Features for Image Classification",
                IEEE Trans. on Systems, Man and Cybernetics,
                Vol. SMC-3, pp. 610-621, 1973.

                &R. W. Conners et al.
		"Segmentation of a High-Resolution Urban Scene Using 
                Texture Operators",
		Computer Vision, Graphics and Image Processing,
		Vol. 25, pp. 273-310, 1984.

		&C. C. Gotlieb et al.
		"Texture Descriptors Based on Co-ooccurrence Matrices",
		Computer Vision, Graphics and Image Processing,
		Vol. 51, pp. 70-86, 1990.

Return value:	GLCM-Parameter value (double)

Author:		Yogesan, BLAB, Ifi, UiO

Id: 		$Id$
________________________________________________________________

*/

double glcmAsm(ID_BAND inband)
{
  int x, y, xsize, ysize;
  double Asm = 0.0;

  if (Ipixtyp((IBAND) inband) != Idouble_typ)
    return(Error(-1, "Input band must have pixel type double.\n"));

  xsize = Ixsize((IBAND) inband);
  ysize = Iysize((IBAND) inband);

  for (y=1; y<=ysize; y++)
    for (x=1; x<=xsize; x++)
      Asm += (inband[y][x] * inband[y][x]);

  return(Asm);
}


double glcmCont(ID_BAND inband)
{
  int x, y, xsize, ysize, n;
  double cont=0.0, *sum;

  if (Ipixtyp((IBAND) inband) != Idouble_typ)
    return(Error(-1, "Input band must have pixel type double.\n"));

  xsize = Ixsize((IBAND) inband);
  ysize = Iysize((IBAND) inband);
  sum = (double*)calloc(xsize, sizeof(double));
 
  for (n=0; n<xsize; n++) sum[n] = 0.0;

  for (y=1 ; y <= ysize;y++)
    for (x=1; x <= xsize;x++)
      sum[abs(y-x)] += inband[y][x]; 

  for (n=0; n<xsize; n++) cont += n * n * sum[n];

  free(sum);

  return(cont);
}

double glcmCorr(ID_BAND inband)
{
  int x, y, xsize, ysize;
  double corr = 0.0, mux, muy, muxx, muyy, sigmax, sigmay;

  if (Ipixtyp((IBAND) inband) != Idouble_typ)
    return(Error(-1, "Input band must have pixel type double.\n"));

  xsize = Ixsize((IBAND) inband);
  ysize = Iysize((IBAND) inband);

  muy =  mux = 0.0;
  for (y=1 ; y <= ysize; y++)
    { 
      muyy = muxx = 0.0;
      for (x=1; x <= xsize; x++)
	{
	  muyy += inband[y][x];
	  muxx += inband[x][y];
	}
      muy += muyy * (y-1);
      mux += muxx * (y-1);
    }

  sigmay  = sigmax = 0.0;
  for (y=1; y <= ysize; y++)
    { 
      muxx = muyy = 0.0;
      for (x=1; x <= xsize; x++) 
	{
	  muyy += inband[y][x];
          muxx += inband[x][y];
         }
      sigmay += (y - 1 - muy) * (y - 1 - muy) * muyy;
      sigmax += (y - 1 - mux) * (y - 1 - mux) * muxx;
    }

  for (y= 1; y <= ysize; y++)
    for (x=1; x <= xsize; x++)
      corr += ((x-1)*(y-1))*inband[y][x] ;

  sigmax=pow(sigmax,0.5);
  sigmay=pow(sigmay,0.5);
  corr  = ( corr - (mux*muy) ) / (sigmax * sigmay);


  return(corr);
}

double glcmIdm(ID_BAND inband)
{
  int x, y, xsize, ysize;
  double idm = 0.0;

  if (Ipixtyp((IBAND) inband) != Idouble_typ)
    return(Error(-1, "Input band must have pixel type double.\n"));

  xsize = Ixsize((IBAND) inband);
  ysize = Iysize((IBAND) inband);

  for (y= 1; y <= ysize; y++)
    for (x=1; x <= xsize; x++) 
      idm += inband[y][x] / (1 + pow((double)(y - x),2.0)) ;
  
  return(idm);
}

double glcmEntropy(ID_BAND inband)
{
  int x, y, xsize, ysize;
  double entropy = 0.0;

  if (Ipixtyp((IBAND) inband) != Idouble_typ)
    return(Error(-1, "Input band must have pixel type double.\n"));

  xsize = Ixsize((IBAND) inband);
  ysize = Iysize((IBAND) inband);

  for (y=1; y <=ysize; y++)
    for (x=1; x <= xsize; x++) 
      if (inband[y][x] != 0.0)
        entropy += inband[y][x] * ( log10(inband[y][x]) / log10(2.0) );

  entropy = -1.0 * entropy;

  return(entropy);
}

double glcmVar(ID_BAND inband)
{
    int x, y, xsize, ysize;
    double var = 0.0, mean = 0.0;
    double mux=0.0, muy=0.0, muyy = 0.0;

    if (Ipixtyp((IBAND) inband) != Idouble_typ)
    return(Error(-1, "Input band must have pixel type double.\n"));

    xsize = Ixsize((IBAND) inband);
    ysize = Iysize((IBAND) inband);

    for (y=1; y <= ysize; y++)
      for (x=1; x <= xsize; x++) 
        mean += ( (double)(x-1) * (double)(y-1) * inband[y][x]);
    
    for (y=1; y<=ysize; y++)
      for (x=1; x<=xsize; x++)
	 var += ((double)(y-1) - mean) * ((double)(y-1) - mean) * inband[y][x];

    return(var);
}


double glcmShade(ID_BAND inband)
{
  int x, y, xsize, ysize;
  double shade = 0.0, mux = 0.0, muy = 0.0;
  double muxy = 0.0, muyy = 0.0, muxx = 0.0;

  if (Ipixtyp((IBAND) inband) != Idouble_typ)
    return(Error(-1, "Input band must have pixel type double.\n"));

  xsize = Ixsize((IBAND) inband);
  ysize = Iysize((IBAND) inband);

  for (y=1 ; y <= ysize; y++)
    {
      muyy = muxx = 0.0;
      for (x=1; x <= xsize; x++)
	{
	  muyy += inband[y][x];
	  muxx += inband[x][y];
	}
      muy += muyy * (y-1);
      mux += muxx * (y-1);
    }

  muxy = mux + muy;
  for (y=1; y <= ysize; y++)
    for (x=1; x <= xsize; x++) 
      shade += inband[y][x] * pow(((double)x + (double)y - 2.0 - muxy),3.0);
      
  return(shade);
}

double glcmProm(ID_BAND inband)
{
  int x, y, xsize, ysize;
  double prom = 0.0, mux = 0.0, muy = 0.0;
  double muxy = 0.0, muyy = 0.0;

  if (Ipixtyp((IBAND) inband) != Idouble_typ)
    return(Error(-1, "Input band must have pixel type double.\n"));

  xsize = Ixsize((IBAND) inband);
  ysize = Iysize((IBAND) inband);

  for (y=1; y <= ysize; y++)
    {
      muyy = 0.0;
      for (x=1; x <= xsize; x++) 
	{
	  muyy += inband[y][x];
	  mux += (double)(x-1) * inband[y][x]; 
	}
      muy += (double)(y-1) * muyy; 
    }

  muxy = mux + muy;
  for (y=1; y <= ysize; y++)
    for (x=1; x <= xsize; x++) 
      prom += inband[y][x] * pow(((double)x + (double)y - 2.0 - muxy),4.0);
      
  return(prom);
}

double glcmInertia(ID_BAND inband)
{
  int x, y, xsize, ysize;
  double inert = 0.0;

  if (Ipixtyp((IBAND) inband) != Idouble_typ)
    return(Error(-1, "Input band must have pixel type double.\n"));

  xsize = Ixsize((IBAND) inband);
  ysize = Iysize((IBAND) inband);

  for (y=1; y <= ysize; y++)
    for (x=1; x <= xsize; x++) 
      inert += inband[y][x] * ((y-x) * (y-x));
 
  return(inert);
}



double glcmDiagmoment(ID_BAND inband)
{	
   int x, y, xsize, ysize;
   double diagm = 0.0, mean= 0.0;
   double muy=0.0, mux=0.0, muyy=0.0;

   if (Ipixtyp((IBAND) inband) != Idouble_typ)
    return(Error(-1, "Input band must have pixel type double.\n"));

   xsize = Ixsize((IBAND) inband);
   ysize = Iysize((IBAND) inband);
    
   for (y=1; y <= ysize; y++)
     {
       muyy = 0.0;
       for (x=1; x <= xsize; x++)
         {
	   muyy += inband[y][x];
	   mux += (double)(x-1) * inband[y][x];
         }
       muy += (double)(y-1) * muyy;
     }
    
   /* mean (below) is a better approximation for real mean of the
      original image */
   /* mean = (mux + muy) / 2; */
    
   diagm = 0.0;
   for (y=1; y <= ysize; y++)
     for (x=1; x <= xsize; x++)
       diagm  += (double) (abs(y-x)*( (y-1) + (x-1) - mux - muy )*inband[y][x]);
    
   return(diagm);
}
