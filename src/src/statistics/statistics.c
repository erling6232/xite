
/*

________________________________________________________________

        statistics
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
#include <xite/cdoc.h>
#include <math.h>
#include <xite/biff.h>
#include <stdlib.h>
#include <xite/statistics.h>
#include <xite/histo.h>
#include <xite/message.h>
#include <xite/utl_malloc.h>
#include <xite/readarg.h>
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#ifdef HAVE_STRINGS_H
# include <strings.h>
#else
# ifdef HAVE_STRING_H
#  include <string.h>
# endif
#endif



#define SHORT_LF_FORMAT "%11.6lg"
#define LONG_LF_FORMAT  "%22.16lg"
#define SHORT_LD_FORMAT "%11ld"
#define LONG_LD_FORMAT  "%22ld"
#define SHORT_D_FORMAT "%11d"
#define LONG_D_FORMAT  "%22d"

#ifndef MAX
# define MAX(a, b) ( (a) >= (b) ? (a) : (b) )
#endif /* MAX */

#ifndef MAIN

/*F:statisticMask=statistics*/
/*F:statisticMasks=statistics*/
/*F:hstatistics=statistics*/
/*F:statistic=statistics*/
/*F:cstatistic=statistics*/
/*F:statistics*

________________________________________________________________

		statistics
________________________________________________________________

Name:		statistics, statistic, cstatistic, hstatistics,
                statisticMask, statisticMasks - extract statistical information
                from a band

Syntax:         | #include <xite/statistics.h>
		|
                | int statistics( IBAND band, int* min, int* max,
                |    int* median, double* mean, double* stdev );
		|
                | int hstatistics( histogram histo, int* min,
                |    int* max, int* median, double* mean,
                |    double* stdev );
		|
                | int statistic( IBAND band, long* size,
                |    double* min, double* max, double* mean,
                |    double* stdev );
		|
                | int cstatistic( IBAND band, long* size,
                |    double* min, double* max, DCOMPLEX* mean,
                |    double* stdev );
		|
                | int statisticMask( IBAND band, IBAND mask,
                |    long* size, double* min, double* max,
                |    double* mean, double* stdev, int maskval );
		|
                | int statisticMasks( IBAND band, IBAND mask,
                |    long* size, double* min, double* max,
                |    double* mean, double* stdev );
		|
		|
Description:	'hstatistics' calculates 'min' (minimum value), 'max'
                (maximum value), 'median', 'mean' and 'stdev' (standard
		deviation) in a band from the histogram 'histo'. 

		'statistics' calculates 'min', 'max', 'median', 'mean' and
		standard deviation in 'band', by first producing the histogram
		and sending it to 'hstatistics'. 'band' must have pixel type
		unsigned byte.

		'statistic' calculates 'size' (number of elements), 'min',
                'max', 'mean' and 'stdev' (standard deviation) for 'band'.
		'band' must have a non-complex pixel type.

		'cstatistic' calculates the same statistics for a band with
		pixel type complex or double complex. In this case 'min'
		is the minimum absolute value, 'max' is the maximum absolute
		value, and 'mean' is complex.
 
		'statisticMask' calculates the same statistics for all pixels
		where mask[y,x] == maskval.

		'statisticMasks' calculates the same statistics for all pixels
		and all mask values. 'sizearr', 'minarr', 'maxarr', 'meanarr'
		and 'stdevarr' are arrays of 256 elements.
		
		One or more of the parameters 'min', 'minarr', 'max', 
		'maxarr', 'mean', 'meanarr', 'stdev' and 'stdevarr'
		may be NULL.

Restrictions:   'statistics' only works for bands with unsigned byte pixels.
                'statistic', 'statisticMask' and 'statisticMasks' only work
                for non-complex bands. 'cstatistic' only works for complex
		or double complex bands.

                'mask' must be of type unsigned byte and have the same size as
		'band'.

Return value:   |   0 - Ok.
                |   1 - Mask is not same size as band.
                |   2 - Wrong pixel type

See also:       statistics(1)

Author:		Tor Lønnestad, Otto Milvang and Svein Bøe, BLAB, Ifi, UiO

Id:             $Id$
________________________________________________________________

*/

int hstatistics(int *histo, int *min, int *max, int *median, double *mean, double *stdev)
{
  int n, size;
  double sum, diff;

  /* calc min */
  for(*min = 0;   histo[*min] == 0;  (*min)++);

  /* calc max */
  for(*max = 255; histo[*max] == 0; (*max)--);

  /* calc median */
  for (size=0, n=(*min); n <= (*max); n++) size += histo[n];

  for (sum=histo[n=(*min)]; sum < (size+1)/2; ) sum +=histo[++n];
  *median = n;

  /* calc mean */
  sum = 0.0;
  for (n = *min; n <= *max;  n++) sum += n*histo[n];
  *mean = (double)sum/(double)size;

  sum = 0.0;
  /* calc stdev */
  for(n = *min; n <= *max;  n++) {
    diff = (double)n - *mean;
    sum += (double)histo[n]*diff*diff;
  }
  *stdev = sqrt(sum/(double)size);

  return(0);

}  /*  hstatistics()  */

int statistics(IBAND b, int *min, int *max, int *median, double *mean, double *stdev)
{
  histogram histo;

  if (Ipixtyp(b) == Iu_byte_typ) {
    mkHisto(b, histo);
    return(hstatistics(histo, min, max, median, mean, stdev));
  }
  return(Error(2, "statistics(): Pixel type must be unsigned byte.\n"));

}  /* statistics() */


#define stat_non_compl(name, bandtyp, pixtyp)\
static void name(bandtyp b, long *size, double *minval, double *maxval,\
		 double *mean, double *stdev) \
{\
  int mode;\
  long y, x, xsize, ysize, tsize;\
  pixtyp mi, ma, *f;\
  double x1, x2;\
  double y1, y2;\
  \
  mode = ((minval || maxval) ? 1 : 0) + ((mean || stdev) ? 2 : 0);\
  if (mode == 0) return;\
  xsize = Ixsize((IBAND) b);\
  ysize = Iysize((IBAND) b);\
  tsize  = xsize*ysize;\
  mi = ma = b[1][1];\
  y1 = 0.0;\
  y2 = 0.0;\
  for (y=1; y<=ysize; y++)\
    {\
      f = &b[y][1];\
      x1 = 0;\
      x2 = 0;\
      x = xsize;\
      switch(mode)\
	{\
	case 1:\
          while (x--)\
	    {\
	      if (*f < mi) mi = *f;\
	      else if (*f > ma) ma = *f;\
	      f++;\
	    }\
	  break;\
	case 2:\
          while (x--)\
	    {\
	      x2 += (double) (*f) * (*f);\
	      x1 += *f++;\
	    }\
	  break;\
	case 3:\
          while (x--)\
	    {\
	      if (*f < mi) mi = *f;\
	      else if (*f > ma) ma = *f;\
	      x2 += (double) (*f) * (*f);\
	      x1 += *f++;\
	    }\
	  break;\
	}\
      y1 += x1;\
      y2 += x2;\
    }\
  if (size)   *size   = tsize;\
  if (minval) *minval = mi;\
  if (maxval) *maxval = ma;\
  if (mean)   *mean   = y1/ (double) tsize;\
  if (stdev)  *stdev  = sqrt(y2 /((double) tsize) -\
			     y1*y1/((double) tsize)/((double) tsize));\
}

#define stat_compl(name, bandtyp, pixtyp)\
static void name(bandtyp b, long *size, double *minval, double *maxval,\
		 DCOMPLEX *mean, double *stdev) \
{\
  int mode;\
  long y, x, xsize, ysize, tsize;\
  pixtyp *f;\
  double mi, ma, f_abs, f_abs2, f_re, f_im;\
  double x1_re, x1_im, x2;\
  double y1_re, y1_im, y2;\
  \
  mode = ((minval || maxval) ? 1 : 0) + ((mean || stdev) ? 2 : 0);\
  if (mode == 0) return;\
  xsize  = Ixsize((IBAND) b);\
  ysize  = Iysize((IBAND) b);\
  tsize  = xsize*ysize;\
  f_re   = b[1][1].re;\
  f_im   = b[1][1].im;\
  mi = ma = sqrt((double) f_re * f_re + f_im * f_im);\
  y1_re  = 0.0;\
  y1_im  = 0.0;\
  y2     = 0.0;\
  for (y=1; y<=ysize; y++)\
    {\
      f     = &b[y][1];\
      x1_re = 0.0;\
      x1_im = 0.0;\
      x2    = 0.0;\
      x     = xsize;\
      switch(mode)\
	{\
	case 1:\
          while (x--)\
	    {\
              f_re   = (*f).re;\
              f_im   = (*f).im;\
              f_abs  = sqrt( (double) f_re * f_re + f_im * f_im );\
	      if (f_abs < mi) mi = f_abs;\
	      else if (f_abs > ma) ma = f_abs;\
	      f++;\
	    }\
	  break;\
	case 2:\
          while (x--)\
	    {\
              f_re   = (*f).re;\
              f_im   = (*f).im;\
              f_abs2 = (double) f_re * f_re + f_im * f_im;\
	      x2    += f_abs2;\
	      x1_re += f_re;\
	      x1_im += f_im;\
              f++;\
	    }\
	  break;\
	case 3:\
          while (x--)\
	    {\
              f_re   = (*f).re;\
              f_im   = (*f).im;\
              f_abs2 = (double) f_re * f_re + f_im * f_im;\
              f_abs  = sqrt( f_abs2 );\
	      if (f_abs < mi) mi = f_abs;\
	      else if (f_abs > ma) ma = f_abs;\
	      x2    += f_abs2;\
	      x1_re += f_re;\
	      x1_im += f_im;\
              f++;\
	    }\
	  break;\
	}\
      y1_re += x1_re;\
      y1_im += x1_im;\
      y2    += x2;\
    }\
  if (size)   *size   = tsize;\
  if (minval) *minval = mi;\
  if (maxval) *maxval = ma;\
  if (mean)   {\
    mean->re = y1_re / (double) tsize;\
    mean->im = y1_im / (double) tsize;\
  }\
  if (stdev)  *stdev  = sqrt(y2 /((double) tsize) -\
			     (y1_re * y1_re + y1_im * y1_im) /\
                             ((double) tsize)/((double) tsize));\
}

stat_non_compl(byteStat, IBAND, UNS_BYTE)
stat_non_compl(signedByteStat, ISB_BAND, SIGN_BYTE)
stat_non_compl(shortStat, ISS_BAND, SIGN_SHORT)
stat_non_compl(unsignedStat, IUS_BAND, UNS_SHORT)
stat_non_compl(longStat, II_BAND, INTEGER)
stat_non_compl(floatStat, IR_BAND, REAL)
stat_non_compl(doubleStat, ID_BAND, DOUBLE)
stat_compl(complexStat, IC_BAND, COMPLEX)
stat_compl(doubleComplexStat, IDC_BAND, DCOMPLEX)

#define statMsk(name, bandtyp, pixtyp, min, max)\
static void name(bandtyp b, IBAND mask, long *size, double *minval,\
		 double *maxval, double *mean, double *stdev, int maskval)\
{\
  int mode;\
  long xsize, ysize, x, y, tsize;\
  pixtyp mi, ma, *f;\
  unsigned char *msk;\
  double x1, x2;\
  double y1, y2;\
  mode = ((minval || maxval) ? 1 : 0) + ((mean || stdev) ? 2 : 0);\
  if (mode == 0) return;\
  xsize = Ixsize((IBAND) b);\
  ysize = Iysize((IBAND) b);\
  tsize  = 0;\
  mi = max;\
  ma = min;\
  y1 = 0.0;\
  y2 = 0.0;\
  for (y=1; y<=ysize; y++)\
    {\
      msk = &mask[y][1];\
      f = &b[y][1];\
      x1 = 0;\
      x2 = 0;\
      x = xsize;\
      while (x--)\
       if (*msk++ == maskval)\
	{\
	  if (*f < mi) mi = *f;\
	  else if (*f > ma) ma = *f;\
	  x2 += (double) (*f) * (*f);\
	  x1 += *f++;\
	  tsize++;\
	} else f++;\
      y1 += x1;\
      y2 += x2;\
    }\
  if (size)   *size   = tsize;\
  if (minval) *minval = mi;\
  if (maxval) *maxval = ma;\
  if (mean)   *mean   = tsize ? y1 / ((double) tsize) : 0.0;\
  if (stdev)  *stdev  =\
     sqrt(tsize ? (y2/((double) tsize) -\
		   y1*y1/((double) tsize)/((double) tsize)) : 0);\
}

statMsk(byteStMsk, IBAND, UNS_BYTE, 0, UNS_BYTE_MAX)
statMsk(signedByteStMsk, ISB_BAND, SIGN_BYTE, SIGN_BYTE_MIN, SIGN_BYTE_MAX)
statMsk(shortStMsk, ISS_BAND, SIGN_SHORT, SIGN_SHORT_MIN, SIGN_SHORT_MAX)
statMsk(unsignedStMsk, IUS_BAND, UNS_SHORT, 0, UNS_SHORT_MAX)
statMsk(longStMsk, II_BAND, INTEGER, INTEGER_MIN, INTEGER_MAX)
statMsk(floatStMsk, IR_BAND, REAL, -REAL_MAX, REAL_MAX)
statMsk(doubleStMsk, ID_BAND, DOUBLE, -DOUBLE_MAX, DOUBLE_MAX)

#define statMsks(name, bandtyp, pixtyp)\
static void name(bandtyp b, IBAND mask, long *size, double *minval, \
		 double *maxval, double *mean, double *stdev) \
{\
  int mode, i, mark[256];\
  long tsize[256], xsize, ysize, x, y;\
  pixtyp mi[256], ma[256], mm1, mm2, *f;\
  unsigned char *msk;\
  double x1[256], x2[256];\
  double y1[256], y2[256];\
  mode = ((minval || maxval) ? 1 : 0) + ((mean || stdev) ? 2 : 0);\
  if (mode == 0) return;\
  xsize = Ixsize((IBAND) b);\
  ysize = Iysize((IBAND) b);\
  for(i=0; i<256; i++)\
    {\
      tsize[i]  = 0;\
      mark[i]  = 0;\
      mi[i] = ma[i] = 0.0;\
      x1[i] = 0;\
      x2[i] = 0;\
      y1[i] = 0.0;\
      y2[i] = 0.0;\
    }\
  for (y=1; y<=ysize; y++)\
    {\
      msk = &mask[y][1];\
      f = &b[y][1];\
      mm1 = mm2 = *msk;\
      x = xsize;\
      while (x--)\
	{\
	  if (tsize[*msk] == 0) mi[*msk] = ma[*msk] = *f;\
	  else if (*f < mi[*msk]) mi[*msk] = *f;\
	  else if (*f > ma[*msk]) ma[*msk] = *f;\
	  x2[*msk] += (double) (*f) * (*f);\
	  x1[*msk] += *f++;\
          if (*msk < mm1) mm1 = *msk;\
	  else if (*msk > mm2) mm2 = *msk;\
          mark[*msk] = 1;\
	  tsize[*msk++]++;\
	}\
      for(i=mm1; i<=mm2; i++)\
        if (mark[i])\
	{\
          y1[i] += x1[i];\
          y2[i] += x2[i];\
          x1[i] = 0;\
          x2[i] = 0;\
	  mark[i] = 0;\
	}\
    }\
  if (size)   for(i=0; i<256; i++) size[i]   = tsize[i];\
  if (minval) for(i=0; i<256; i++) minval[i] = mi[i];\
  if (maxval) for(i=0; i<256; i++) maxval[i] = ma[i];\
  if (mean)   for(i=0; i<256; i++) mean[i]   =\
   tsize[i] ? y1[i] / ((double) tsize[i]) : 0.0;\
  if (stdev)  for(i=0; i<256; i++) \
    stdev[i] =\
      sqrt(tsize[i] ? y2[i] / ((double) tsize[i]) -\
	   y1[i]*y1[i]/((double) tsize[i])/((double) tsize[i]) : 0.0);\
}

statMsks(byteStMsks, IBAND, UNS_BYTE)
statMsks(signedByteStMsks, ISB_BAND, SIGN_BYTE)
statMsks(shortStMsks, ISS_BAND, SIGN_SHORT)
statMsks(unsignedStMsks, IUS_BAND, UNS_SHORT)
statMsks(longStMsks, II_BAND, INTEGER)
statMsks(floatStMsks, IR_BAND, REAL)
statMsks(doubleStMsks, ID_BAND, DOUBLE)


int statistic(IBAND band, long *size, double *min, double *max, double *mean, double *stdev)
{
  switch((int) Ipixtyp(band))
    {
    case Iu_byte_typ:
      byteStat(band, size, min, max, mean, stdev); break;
    case Is_byte_typ:
      signedByteStat((ISB_BAND) band, size, min, max, mean, stdev); break;
    case Iu_short_typ:
      unsignedStat((IUS_BAND) band, size, min, max, mean, stdev); break;
    case Is_short_typ:
      shortStat((ISS_BAND) band, size, min, max, mean, stdev); break;
    case Integer_typ: 
      longStat((II_BAND) band, size, min, max, mean, stdev); break;
    case Ireal_typ:  
      floatStat((IR_BAND) band, size, min, max, mean, stdev); break;
    case Idouble_typ:
      doubleStat((ID_BAND) band, size, min, max, mean, stdev); break;
    default:
      return(Error(2, "statistic(): Pixel type must be non-complex.\n"));
      /* break; */
    }
  return(0);

} /* statistic() */

int cstatistic(IBAND band, long *size, double *min, double *max, DCOMPLEX *mean, double *stdev)
{
  switch((int) Ipixtyp(band))
    {
    case Icomplex_typ:
      complexStat((IC_BAND) band, size, min, max, mean, stdev); break;
    case Id_complex_typ:
      doubleComplexStat((IDC_BAND) band, size, min, max, mean, stdev); break;
    default:
      return(Error(2, "%s%s\n",
		   "cstatistic(): Pixel type must be complex or double ",
		   "complex."));
      /* break; */
    }

  return(0);

} /* cstatistic() */

int statisticMask(IBAND band, IBAND mask, long *size, double *min, double *max, double *mean, double *stdev, int maskval)
{

  if (Ixsize(band) != Ixsize(mask) || Iysize(band) != Iysize(mask))
    return(Error(1, "statisticMask(): Mask must be same size as band.\n"));

  switch((int) Ipixtyp(band))
    {
    case Iu_byte_typ: 
      byteStMsk(band, mask, size, min, max, mean, stdev, maskval);
      break;
    case Is_byte_typ:
      signedByteStMsk((ISB_BAND) band, mask, size, min, max, mean, stdev,
		    maskval);
      break;
    case Iu_short_typ: 
      unsignedStMsk((IUS_BAND) band, mask, size, min, max, mean, stdev,
		    maskval);
      break;
    case Is_short_typ:
      shortStMsk((ISS_BAND) band, mask, size, min, max, mean, stdev, maskval);
      break;
    case Integer_typ:
      longStMsk((II_BAND) band, mask, size, min, max, mean, stdev, maskval);
      break;
    case Ireal_typ: 
      floatStMsk((IR_BAND) band, mask, size, min, max, mean, stdev, maskval);
      break;
    case Idouble_typ:
      doubleStMsk((ID_BAND) band, mask, size, min, max, mean, stdev, maskval);
      break;
    default:
      return(Error(2, "statisticMask(): Pixel type must be non-complex.\n"));
      /* break; */
    }
  return(0);
}


int statisticMasks(IBAND band, IBAND mask, long *size, double *min, double *max, double *mean, double *stdev)
{
  if (Ixsize(band) != Ixsize(mask) || Iysize(band) != Iysize(mask))
    return(Error(1, "statisticMasks(): Mask must be same size as band.\n"));

  switch((int) Ipixtyp(band))
    {
    case Iu_byte_typ: 
      byteStMsks(band, mask, size, min, max, mean, stdev);
      break;
    case Is_byte_typ: 
      signedByteStMsks((ISB_BAND) band, mask, size, min, max, mean, stdev);
      break;
    case Iu_short_typ: 
      unsignedStMsks((IUS_BAND) band, mask, size, min, max, mean, stdev);
      break;
    case Is_short_typ:
      shortStMsks((ISS_BAND) band, mask, size, min, max, mean, stdev);
      break;
    case Integer_typ:
      longStMsks((II_BAND) band, mask, size, min, max, mean, stdev); break;
    case Ireal_typ:
      floatStMsks((IR_BAND) band, mask, size, min, max, mean, stdev); break;
    case Idouble_typ:
      doubleStMsks((ID_BAND) band, mask, size, min, max, mean, stdev);
      break;
    default:
      return(Error(2, "statisticMasks(): Pixel type must be non-complex.\n"));
      /* break; */
    }
  return(0);
}



/*:covarianceMask=covariance*/
/*:covarianceMasks=covariance*/
/*:covariance*

________________________________________________________________

		covariance
________________________________________________________________

Name:		covariance - ???

Syntax: 	| #include <xite/statistics.h>
		|
                | int covariance( IBAND band, int *size, double *min,
                |    double *max, double *mean, double *stdev );
Description:	'covariance' searches through the 'band' and returns 
                'size' (number of elements), 'mean' (mean value) 
		and 'covar' (covariance) from the band.

		One or both of the parameters 'mean' and 'covar'
		may be NULL.
Return value:	| 0 - Ok
                | 2 - Wrong Pixeltype
Restrictions:	Does not work on complex band.
Author:		Otto Milvang
________________________________________________________________

*/

/*
		|
                | int covarianceMask( IBAND band, IBAND mask, int *size,
                |    double *min, double *max, double *mean, double *stdev,
                |    int maskval );
		|
		| int covarianceMasks(band, mask, 
		|                sizearr, minarr, maxarr, meanarr, stdevarr)
                | IBAND band;
		| IBAND mask;
		| int *sizearr;
		| double *minarr, *maxarr, *meanarr, *stdevarr;


		'covarianceMask' searchess through the 'band' and calculates
		the same covariances for all pixels where mask == maskval.

		'covarianceMasks' searches through the 'band' and calculates
		the same covariances for all pixels and all mask values.
		'sizearr', 'meanarr' and 'covararr' are arrays of length
		256 elements.
		
		One or more of the parameters 'mean', 'meanarr', 'covar'
		and 'covarvarr' may be NULL.
*/

#define stat_4(name, bandtyp, pixtyp)\
static void name(IMAGE img, long *size, double *mean, double **covar) \
{\
  int nbands, n, m, xsize, ysize, x, y, tsize;\
  pixtyp **f;\
  double *x1, **x2;\
  bandtyp b;\
  double *y1, **y2;\
  xsize = Ixsize((IBAND) img[1]);\
  ysize = Iysize((IBAND) img[1]);\
  tsize  = xsize*ysize;\
  nbands = Inbands(img);\
  f  = (pixtyp **)    Mmatrix_1d(1, nbands, sizeof(pixtyp *), 1);\
  x1 = (double *)    Mmatrix_1d(1, nbands, sizeof(double), 1);\
  y1 = (double *)  Mmatrix_1d(1, nbands, sizeof(double), 1);\
  x2 = (double **)   Mmatrix_2d(1, nbands, 1, nbands, sizeof(double), 1);\
  y2 = (double **) Mmatrix_2d(1, nbands, 1, nbands, sizeof(double), 1);\
  for (y=1; y<=ysize; y++)\
    {\
      for (n=1; n<= nbands; n++)\
	{\
	   b = (bandtyp) img[n];\
           f[n] = &b[y][1];\
	   x1[n] = 0;\
           for (m=1; m<=nbands; m++)\
  	     x2[n][m] = 0;\
	}\
      x = xsize;\
      while (x--)\
        {\
	  for(n=1; n<=nbands; n++)\
	    {\
              x1[n] += *(f[n]);\
	      for (m=n; m<=nbands; m++)\
		x2[n][m] += (double) (*(f[n])) * (*(f[m]));\
            }\
        }\
      for(n=1; n<=nbands; n++)\
	{\
          y1[n] += x1[n];\
          for (m=n; m<=nbands; m++)\
	    y2[n][m] += (x2[n][m]);\
        }\
    }\
  for(n=1; n<=nbands; n++)\
    for (m=1; m<n; m++)\
      y2[n][m] = y2[n][m];\
  if (size)   *size   = tsize;\
  if (mean)\
    for(n=1; n<=nbands; n++)\
      mean[n] = y1[n]/tsize;\
  if (covar)\
    for(n=1; n<=nbands; n++)\
      for (m=1; m<=nbands; m++)\
        covar[n][m] = y2[n][m]/tsize - y1[n]*y1[m]/tsize/tsize;\
}

/*
stat_4(byte_4, IBAND, unsigned char)
stat_4(short_4, ISS_BAND, short)
stat_4(unsigned_4, IUS_BAND, unsigned short)
stat_4(long_4, II_BAND, long)
stat_4(float_4, IR_BAND, float)
stat_4(double_4, ID_BAND, double)
*/

#define stat_5(name, bandtyp, pixtyp)\
static void name(bandtyp b, IBAND mask, int *long, double *minval, \
		 double *maxval, double *mean, double *stdev, int maskval) \
{\
  int mode, xsize, ysize, x, y, tsize;\
  pixtyp mi, ma, *f;\
  unsigned char *msk;\
  double x1, x2;\
  double y1, y2;\
  mode = ((minval || maxval) ? 1 : 0) + ((mean || stdev) ? 2 : 0);\
  if (mode == 0) return;\
  xsize = Ixsize((IBAND) b);\
  ysize = Iysize((IBAND) b);\
  tsize  = 0;\
  mi = ma = b[1][1];\
  y1 = 0.0;\
  y2 = 0.0;\
  for (y=1; y<=ysize; y++)\
    {\
      msk = &mask[y][1];\
      f = &b[y][1];\
      x1 = 0;\
      x2 = 0;\
      x = xsize;\
      while (x--)\
        if (*msk++ == maskval)\
	{\
	  if (*f < mi) mi = *f;\
	  else if (*f > ma) ma = *f;\
	  x2 += (double) (*f) * (*f);\
	  x1 += *f++;\
	  tsize++;\
	} else f++;\
      y1 += x1;\
      y2 += x2;\
    }\
  if (size)   *size   = tsize;\
  if (minval) *minval = mi;\
  if (maxval) *maxval = ma;\
  if (mean)   *mean = tsize ? y1/tsize : 0;\
  if (stdev)  *stdev = sqrt(tsize ? y2/tsize - y1*y1/tsize/tsize : 0);\
}

/*
stat_5(byte_5, IBAND, unsigned char)
stat_5(short_5, ISS_BAND, short)
stat_5(unsigned_5, IUS_BAND, unsigned short)
stat_5(long_5, II_BAND, long)
stat_5(float_5, IR_BAND, float)
stat_5(double_5, ID_BAND, double)
*/

#define stat_6(name, bandtyp, pixtyp)\
static void name(bandtyp b, IBAND mask, long *size, double *minval, \
		 double *maxval, double *mean, double *stdev) \
{\
  int mode, i, xsize, ysize, x, y, tsize[256], mark[256];\
  pixtyp mi[256], ma[256], mm1, mm2, *f;\
  unsigned char *msk;\
  double x1[256], x2[256];\
  double y1[256], y2[256];\
  mode = ((minval || maxval) ? 1 : 0) + ((mean || stdev) ? 2 : 0);\
  if (mode == 0) return;\
  xsize = Ixsize((IBAND) b);\
  ysize = Iysize((IBAND) b);\
  for(i=0; i<256; i++)\
    {\
      tsize[i]  = 0;\
      mark[i]  = 0;\
      mi[i] = ma[i] = 0.0;\
      x1[i] = 0;\
      x2[i] = 0;\
      y1[i] = 0.0;\
      y2[i] = 0.0;\
    }\
  for (y=1; y<=ysize; y++)\
    {\
      msk = &mask[y][1];\
      f = &b[y][1];\
      mm1 = mm2 = *msk;\
      x = xsize;\
      while (x--)\
	{\
	  if (tsize[*msk] == 0) mi[*msk] = ma[*msk] = *f;\
	  else if (*f < mi[*msk]) mi[*msk] = *f;\
	  else if (*f > ma[*msk]) ma[*msk] = *f;\
	  x2[*msk] += (double) (*f) * (*f);\
	  x1[*msk] += *f++;\
          if (*msk < mm1) mm1 = *msk;\
	  else if (*msk > mm2) mm2 = *msk;\
          mark[*msk] = 1;\
	  tsize[*msk++]++;\
	}\
      for(i=mm1; i<=mm2; i++)\
        if (mark[i])\
	{\
          y1[i] += x1[i];\
          y2[i] += x2[i];\
          x1[i] = 0;\
          x2[i] = 0;\
	  mark[i] = 0;\
	}\
    }\
  if (size)   for(i=0; i<256; i++) size[i]   = tsize[i];\
  if (minval) for(i=0; i<256; i++) minval[i] = mi[i];\
  if (maxval) for(i=0; i<256; i++) maxval[i] = ma[i];\
  if (mean)   for(i=0; i<256; i++) mean[i] = tsize[i] ? y1[i]/tsize[i] : 0;\
  if (stdev)  for(i=0; i<256; i++) \
    stdev[i] = \
      sqrt(tsize[i] ? y2[i]/tsize[i] - y1[i]*y1[i]/tsize[i]/tsize[i] : 0.0);\
}

/*
stat_6(byte_6, IBAND, unsigned char)
stat_6(short_6, ISS_BAND, short)
stat_6(unsigned_6, IUS_BAND, unsigned short)
stat_6(long_6, II_BAND, long)
stat_6(float_6, IR_BAND, float)
stat_6(double_6, ID_BAND, double)
*/

/*
int covariance(IBAND band, int *size, double *min, double *max,
	       double *mean, double *stdev)
{
  switch((int) Ipixtyp(band))
    {
    case 3:  byte_4(band, size, min, max, mean, stdev); break;
    case 5:  unsigned_4((IUS_BAND) band, size, min, max, mean, stdev); break;
    case 6:  short_4((ISS_BAND) band, size, min, max, mean, stdev); break;
    case 7:  long_4((II_BAND) band, size, min, max, mean, stdev); break;
    case 8:  float_4((IR_BAND) band, size, min, max, mean, stdev); break;
    case 10: double_4((ID_BAND) band, size, min, max, mean, stdev); break;
    default: return(2); break;
    }
  return(0);
} 


int covarianceMask(IBAND band, IBAND mask, int *size, double *min,
		   double *max, double *mean, double *stdev, int maskval)
{
  switch((int) Ipixtyp(band))
    {
    case 3:  byte_5(band, mask, size, min, max, mean, stdev, maskval); break;
    case 5:  unsigned_5((IUS_BAND) band, mask, size, min, max, mean, stdev,
			maskval); break;
    case 6:  short_5((ISS_BAND) band, mask, size, min, max, mean, stdev,
		     maskval); break;
    case 7:  long_5((II_BAND) band, mask, size, min, max, mean, stdev,
		    maskval); break;
    case 8:  float_5((IR_BAND) band, mask, size, min, max, mean, stdev,
		     maskval); break;
    case 10: double_5((ID_BAND) band, mask, size, min, max, mean, stdev,
		      maskval); break;
    default: return(2); break;
    }
  return(0);
}


int covarianceMasks(IBAND band, IBAND mask, int *size, double *min,
		    double *max, double *mean, double *stdev)
{
  switch((int) Ipixtyp(band))
    {
    case 3:  byte_6(band, mask, size, min, max, mean, stdev); break;
    case 5:  unsigned_6((IUS_BAND) band, mask, size, min, max, mean, stdev);
      break;
    case 6:  short_6((ISS_BAND) band, mask, size, min, max, mean, stdev);
      break;
    case 7:  long_6((II_BAND) band, mask, size, min, max, mean, stdev); break;
    case 8:  float_6((IR_BAND) band, mask, size, min, max, mean, stdev); break;
    case 10: double_6((ID_BAND) band, mask, size, min, max, mean, stdev);
      break;
    default: return(2); break;
    }
  return(0);
}
*/

#endif /* not MAIN */

#ifdef MAIN

/*P:statistics*

________________________________________________________________

		statistics
________________________________________________________________

Name:		statistics - extract statistical information from an image

Syntax:		| statistics [<option>...] <image> [<image>...]

Description:	Calculates minimum, maximum, median, mean and standard
                deviation for each band in an image. All values
                are written to stdout. For complex-valued image bands, minimum
		and maximum values are calculated as minimum and maximum
		absolute values.
		
		The calculations may be done on the whole image,
                or a mask may be specified.

Options:        &-r file
                Use region mask 'file' which must be a BIFF unsigned byte image
		file. For all the mask pixels with a certain value, calculate
		the statistics for image pixels at the same coordinates.
		Repeat for all mask values.

		&-p value
		Calculate and print statistics only for the mask value 'value'
		(i.e. only for those image pixels with the same location as
		the mask pixels with value equal to 'value'). This option
		requires the '-r' option.

		&-a
		Use long output format. Default is "11.6lg"
		(6 significant digits) for floating-point, "11ld" for
		integers. Long format is "22.16lg" (16 significant digits)
		for floating-point, "22ld" for integers. The exact meaning
		of the format specification is the same as for the C
		function printf(3).

		-&If one of the options [flumns] is used, the
		output is unformatted (unless option '-h' is used), and only
		the indicated statistic is calculated.

		&-h
		Horizontal format information, even if one of the options
		[flumns] is used.
		
		&-f
		Print filename and bandnumber (and masknumber if -r is used).

		&-l
		Print minimum value.

		&-m
		Print mean.

		&-n
		Print median (Unsigned byte image only).

                &-s
		Print standard deviation.

                &-u
		Print maximum value.

Restrictions:	Median works for unsigned byte images only.
		Mask image must have pixel type unsigned byte.

See also:       statistics(3)

Author:		Tor Lønnestad and Otto Milvang, BLAB, Ifi, UiO

Examples:	statistics mona.img

Id:             $Id$
________________________________________________________________

*/

static void readsw(int *argc, char *argv[], char **region, int *format, int *name, int *n, int *ca, int *l, int *u, int *m, int *s, int *p, int *h, int *a)
{
  *a      = read_bswitch(argc, argv, "-a");
  *ca     = read_bswitch(argc, argv, "-c");
  *name   = read_bswitch(argc, argv, "-f");
  *h      = read_bswitch(argc, argv, "-h");
  *l      = read_bswitch(argc, argv, "-l");
  *m      = read_bswitch(argc, argv, "-m");
  *n      = read_bswitch(argc, argv, "-n");
  *s      = read_bswitch(argc, argv, "-s");
  *u      = read_bswitch(argc, argv, "-u");
  *p      = read_iswitch(argc, argv, "-p", -1);
  *region = read_switch (argc, argv, "-r", 1, NULL);

  if ((*ca) || (*name) || (*h) || (*l) || (*m) || (*n) || (*s) || (*u))
    *format = FALSE;

  if (!(*ca || *name || *l || *m || *n || *s || *u))
    *l = *u = *m = *n = *s = *ca = TRUE;
}

static int readfile(char *name, char **filename, IMAGE *img)
{
  if (name[0] == '-') {
    /* Reading from a pipe. */

    *img = Iread_image(name);
    *filename = NULL;
    if (*img == 0) exit(0);
  } else {
    *filename = name;
    *img = Iread_image(name);
    if (*img == 0) return(2);
  } 
  return(Iok);
}
      
int main(int argc, char **argv)
{
  IMAGE img, mask=NULL;
  int i, bn, mn, stat;
  int rstat, ps, pe, room;
  size_t fname_len, sp_len;
  int n=FALSE, l = FALSE, u = FALSE;
  int name = FALSE, format = TRUE, long_format = FALSE;
  int he = FALSE, c=FALSE, m=FALSE, s=FALSE, p = -1;
  char *filename = NULL, *regionfile = NULL, *fname = NULL, *form_lf_string;
  char *form_ld_string, *form_d_string, formStr[512];
  int imin, imax, imedian, argnum;
  long size[256];
  double min[256], max[256], mean[256], stdev[256];
  DCOMPLEX c_mean;
  int compl = 0;

  Iset_message(TRUE);
  Iset_abort(TRUE);

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [<option>...] <imagefile>... \n\
       where <option> is chosen from\n\
       -r <file>  : Use region mask file.\n\
       -p <value> : Calculate statistics only for the mask value <value>.\n\
       -a         : Use long floating-point and integer output format.\n\
       -f         : Print filename and bandnumber (and masknumber with -r).\n\
       -h         : Horizontal output format (including text), even if an\n\
                    option [flunms] is used.\n\
       -l         : Print min value.\n\
       -m         : Print mean.\n\
       -n         : Print median (unsigned byte image only).\n\
       -s         : Print standard deviation.\n\
       -u         : Print max value.\n"));

  if (argc == 1) Usage(1, NULL);

  readsw(&argc, argv, &regionfile, &format, &name, &n, &c, 
	 &l, &u, &m, &s, &p, &he, &long_format);

  if (he) format = FALSE;
  if (!long_format) {
    form_lf_string = SHORT_LF_FORMAT;
    form_ld_string = SHORT_LD_FORMAT;
    form_d_string  = SHORT_D_FORMAT;
  } else {
    form_lf_string = LONG_LF_FORMAT;
    form_ld_string = LONG_LD_FORMAT;
    form_d_string  = LONG_D_FORMAT;
  }

  if (regionfile) mask = Iread_image(regionfile);
  if (mask && Ipixtyp(mask[1]) != Iu_byte_typ)
    Error(2, "Mask image must have pixeltype unsigned byte.\n");
      
  /* Treat remaining command-line arguments as image filenames. */

  argnum = 1;
  while (argc - argnum > 0) {
    rstat = readfile(argv[argnum++], &filename, &img);
    if (rstat) Error(2, "Error reading file %s\n",argv[--argnum]);

    fname = filename == NULL ? "stdin" : filename;
    fname_len = strlen(fname);

    mn = 1;
    for (bn=1; bn <= Inbands(img);  bn++) {
      imedian = -1; /* Changed only if unsigned-byte and no mask. */
      compl   = FALSE;

      if (mask && (p < 0))
	stat = statisticMasks
	  (img[bn], mask[mn], size, min, max, mean, stdev);
      else if (mask && (p>=0)) {
	stat = statisticMask
	  (img[bn], mask[mn], &size[p], &min[p], &max[p],
	   &mean[p], &stdev[p], p);
      }
      else if (Ipixtyp(img[bn]) == Iu_byte_typ)
	stat = statistics(img[bn], &imin, &imax, &imedian, mean, stdev);
      else if (Ipixtyp(img[bn]) == Icomplex_typ ||
	       Ipixtyp(img[bn]) == Id_complex_typ) {
	compl = TRUE;
	stat = cstatistic(img[bn], size, min, max, &c_mean, stdev);
      }
      else
	stat = statistic(img[bn], size, min, max, mean, stdev);

      if (stat == 2) {
	if (filename)
	  Warning(3, "Bad pixeltype for band number %d in %s.\n",
		  bn, filename);
	else
	  Warning(3, "Bad pixeltype for band number %d.\n", bn);
	continue;
      } else if (stat == 1) {
	if (filename)
	  Warning(3, "Mask and band number %d not same size in %s.\n",
		  bn, filename);
	else
	  Warning(3, "Mask and band number %d not same size.\n", bn);
	continue;
      }
      
      /* size has not been computed in the no-mask unsigned-byte case. */
      if (imedian >=0) size[0] = Ixsize(img[bn]) * Iysize(img[bn]);
      
      ps = p>=0 ? p : 0;
      pe = p>=0 ? p : 255;
      if (!mask) ps = pe = 0;

      if (format) {
	/* Print all statistics, median only if no-mask unsigned-byte. */
	for (i=ps; i<=pe; i++) {
	  if (size[i] == 0) continue; /* No mask pixels with this values. */ 
	  if (mask)
	    printf("\n Band %d, Plane %d : \n\n", bn, i);
	  else
	    printf("\n Band %d : \n\n",bn);
	  
	  sprintf(formStr, "%s%s\n%s%s\n%s%s\n", "   Size  : ",
		  form_ld_string,
		  "   Xsize : ", form_ld_string, "   Ysize : ",
		  form_ld_string);
	  printf(formStr, size[i], Ixsize(img[bn]), Iysize(img[bn]));
	  
	  if (imedian >= 0) {
	    sprintf(formStr, "%s%s\n%s%s\n", "   Min   : ", form_d_string,
		    "   Max   : ", form_d_string);
	    printf(formStr, imin, imax);
	  }
	  else {
	    sprintf(formStr, "%s%s\n%s%s\n", "   Min   : ", form_lf_string,
		    "   Max   : ", form_lf_string);
	    printf(formStr, min[i], max[i]);
	  }
	  if (imedian >= 0) {
	    sprintf(formStr, "%s%s\n", "   Median: ", form_d_string);
	    printf(formStr, imedian);
	  }
	  
	  if (!compl) {
	    sprintf(formStr, "%s%s\n%s%s\n", "   Mean  : ", form_lf_string,
		    "   St.dev: ", form_lf_string);
	    printf(formStr, mean[i], stdev[i]);
	  } else {
	    char *sgn = "+";

	    if (c_mean.im < 0) sgn[0] = '\0';

	    sprintf(formStr, "%s%s%s%sj\n%s%s\n", "   Mean  : ",
		    form_lf_string, sgn, form_lf_string,
		    "   St.dev: ", form_lf_string);
	    printf(formStr, c_mean.re, c_mean.im, stdev[i]);
	  }
	  
	}
      } else if (he) {
	/* Print only selected statistics with headers,
	 * median only if no-mask unsigned-byte. */
	
	if (mask)
	  sp_len = MAX(fname_len, strlen("Filename")) + strlen(" band") +
	    strlen(" mask");
	else
	  sp_len = MAX(fname_len, strlen("Filename")) + strlen(" band");
	
	if (name) {
	  /* Write filename etc. */
	  if (mask) {
	    /* Include mask value. */
	    room = sp_len - strlen("Filename") - strlen(" band") -
	      strlen(" mask");
	    if (room > 0) {
	      printf("%*c%s%s%s", room, ' ',
		     "Filename", " band", " mask");
	    } else {
	      printf("%s%s%s", "Filename", " band", " mask");
	    }
	  } else {
	    /* No mask value. */
	    room = sp_len - strlen("Filename") - strlen(" band");
	    if (room > 0) {
	      printf("%*c%s%s", room, ' ', "Filename", " band");
	    } else {
	      printf("%s%s", "Filename", " band");
	    }
	  }
	} /* name */
	
	if (!long_format) {
	  if (l) printf("         Min");
	  if (u) printf("         Max");
	  if (n && imedian >= 0) printf("      Median");
	  if (m) printf("        Mean");
	  if (s) printf("     St.dev.");
	} else {
	  if (l) printf("                    Min");
	  if (u) printf("                    Max");
	  if (n && imedian >= 0) printf("                 Median");
	  if (m) printf("                   Mean");
	  if (s) printf("                St.dev.");
	}
	if (l || u || (n && imedian >= 0) || m || s || name) printf("\n");
	
	for (i=ps; i<=pe; i++) {
	  if (size[i] == 0) continue;
	  
	  if (name) {
	    if (mask) {
	      /* Include mask value. */
	      room = sp_len - fname_len - strlen(" band") - strlen(" mask");
	      if (room > 0) {
		printf("%*c%s %4d %4d", room, ' ', fname, bn, i);
	      } else {
		printf("%s %4d %4d", fname, bn, i);
	      }
	    } else {
	      /* No mask value. */
	      room = sp_len - fname_len - strlen(" band");
	      if (room > 0) {
		printf("%*c%s %4d", room, ' ', fname, bn);
	      } else {
		printf("%s %4d", fname, bn);
	      }
	    }
	  }
	  
	  if (imedian >= 0) {
	    sprintf(formStr, " %s", form_d_string);
	    if (l) printf(formStr, imin);
	    if (u) printf(formStr, imax);
	    if (n) printf(formStr, imedian); 
	  }
	  else {
	    sprintf(formStr, " %s", form_lf_string);
	    if (l) printf(formStr, min[i]);
	    if (u) printf(formStr, max[i]);
	  }
	  
	  if (!compl) {
	    if (m) {
	      sprintf(formStr, " %s", form_lf_string);
	      printf(formStr, mean[i]);
	    }
	  } else {
	    if (m) {
	      char *sgn = "+";

	      if (c_mean.im < 0) sgn[0] = '\0';

	      sprintf(formStr, " %s%s%sj", form_lf_string, sgn,
		      form_lf_string);
	      printf(formStr, c_mean.re, c_mean.im);
	    }
	  }
	  sprintf(formStr, " %s", form_lf_string);
	  if (s) printf(formStr, stdev[i]);
	  if (l || u || (n && imedian >= 0) || m || s || name) printf("\n");
	}
      }
      else {
	/* Print only selected statistics with no headers,
	 * median only if no-mask unsigned-byte. */
	for (i=ps; i<=pe; i++) {
	  if (size[i] == 0) continue;
	  if (name) { 
	    if (mask) printf("%s %3d %3d", fname, bn, i); 
	    else printf("%s %3d ", fname, bn);
	  }
	  if (imedian >= 0) {
	    sprintf(formStr, " %s", form_d_string);
	    if (l) printf(formStr, imin);
	    if (u) printf(formStr, imax);
	    if (n) printf(formStr, imedian);
	  } else {
	    sprintf(formStr, " %s", form_lf_string);
	    if (l) printf(formStr, min[i]);
	    if (u) printf(formStr, max[i]);
	  }
	  
	  if (!compl) {
	    if (m) {
	      sprintf(formStr, " %s", form_lf_string);
	      printf(formStr, mean[i]);
	    }
	  } else {
	    if (m) {
	      char *sgn = "+";

	      if (c_mean.im < 0) sgn[0] = '\0';

	      sprintf(formStr, " %s%s%sj", form_lf_string, sgn,
		      form_lf_string);
	      printf(formStr, c_mean.re, c_mean.im);
	    }
	  }
	  sprintf(formStr, " %s", form_lf_string);
	  if (s) printf(formStr, stdev[i]);
	  if (l || u || (n && imedian >= 0) || m || s || name) printf("\n");
	}
      } /* else */
      mn++;
      if (mask && (mn > Inbands(mask))) mn = 1;
    } /* for each band */
    if (format) printf("\n");
    Idel_image(&img);
  } /* while arguments */

  return(0);
}  /* main */

#endif /* MAIN */
