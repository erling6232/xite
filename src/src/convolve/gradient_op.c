

/*

________________________________________________________________

        gradient.c
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
#include XITE_STDIO_H
#include <xite/convert.h>
#include <xite/convolve.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include XITE_STRING_H

#ifdef GRADIENT
#define OP gradient
#endif

#ifdef SOBEL
#define OP sobel
#define NAME "sobel"
#define OP1 SOBEL1
#define OPI SOBELI
#define OPN SOBELN
#endif

#ifdef PREWITT
#define OP prewitt
#define NAME "prewitt"
#define OP1 PREWITT1
#define OPI PREWITTI
#define OPN PREWITTN
#endif

#ifdef ROBERTS
#define OP roberts
#define NAME "roberts"
#define OP1 ROBERTS1
#define OPI ROBERTSI
#define OPN ROBERTSN
#endif

#ifdef GRAD
#define OP grad
#define NAME "grad"
#define OP1 GRAD1
#define OPI GRADI
#define OPN GRADN
#endif

#ifndef MAIN

static unsigned char atg_s[257] = {   0,
     0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,
     2,  2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  4,  5,
     5,  5,  5,  5,  5,  6,  6,  6,  6,  6,  6,  6,  7,  7,  7,  7,
     7,  7,  8,  8,  8,  8,  8,  8,  8,  9,  9,  9,  9,  9,  9,  9,
    10, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11, 11, 11, 12, 12, 12,
    12, 12, 12, 12, 13, 13, 13, 13, 13, 13, 13, 14, 14, 14, 14, 14,
    14, 14, 15, 15, 15, 15, 15, 15, 15, 15, 16, 16, 16, 16, 16, 16,
    16, 17, 17, 17, 17, 17, 17, 17, 17, 18, 18, 18, 18, 18, 18, 18,
    19, 19, 19, 19, 19, 19, 19, 19, 20, 20, 20, 20, 20, 20, 20, 20,
    20, 21, 21, 21, 21, 21, 21, 21, 21, 22, 22, 22, 22, 22, 22, 22,
    22, 22, 23, 23, 23, 23, 23, 23, 23, 23, 23, 24, 24, 24, 24, 24,
    24, 24, 24, 24, 25, 25, 25, 25, 25, 25, 25, 25, 25, 26, 26, 26,
    26, 26, 26, 26, 26, 26, 26, 27, 27, 27, 27, 27, 27, 27, 27, 27,
    27, 27, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 29, 29, 29, 29,
    29, 29, 29, 29, 29, 29, 29, 29, 30, 30, 30, 30, 30, 30, 30, 30,
    30, 30, 30, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 32 };

static unsigned char pseudo_len_s[8] = { 4, 12, 20, 28, 36, 44, 52, 60};
static unsigned char pseudo_kum_s[8] = { 0, 4, 16, 36, 64, 100, 144, 192,};

static unsigned char pseudo_arr_s[256];

#define MAG(sum, sum1, sum2)\
      sum = (sum1 >= 0 ? sum1 : -sum1) + (sum2 >= 0 ? sum2 : -sum2);\
      if (sum > 255) sum = 255;

#define ANG(sum, sum1, sum2)\
      if (sum1 > 0 && sum2 >= 0)\
        sum = sum1>sum2 ? atg_s[ (sum2 << 8) / sum1 ] : \
          63 - atg_s[ (sum1 <<8) / sum2]; else\
      if (sum1 <= 0 && sum2 > 0)\
        sum = -sum1>sum2 ? 127 - atg_s[ (sum2 << 8) / (-sum1) ] :\
          64 + atg_s[ ((-sum1) <<8) / sum2]; else\
      if (sum1 < 0 && sum2 <= 0)\
        sum = -sum1>-sum2 ? 128 + atg_s[ ((-sum2) << 8) / (-sum1) ] :\
          191 - atg_s[ ((-sum1) <<8) / (-sum2)]; else\
      if (sum1 >= 0 && sum2 < 0) \
        sum = sum1>-sum2 ? 255 - atg_s[ ((-sum2) << 8) / sum1 ] :\
          192 + atg_s[ (sum1 <<8) / (-sum2)];

#define PSE(sum, lum, hue)\
      sum = pseudo_arr_s[lum]; \
      sum = pseudo_kum_s[sum] + hue * pseudo_len_s[sum] / 256;


#define SOBEL1(sum1, sum2)\
      sum1 = -*p1 -*p2 -*p2 -*p3 +  p1[1] + p2[1]+ p2[1] + p3[1];\
      sum2 = -3 * *p1 - p1[1] + 3 * *p3 + p3[1];

#define SOBELI(sum1, sum2)\
      sum1  = sum2  = - *p1++;\
      sum1 -= *p2++ << 1;\
      sum1 -= *p3;\
      sum2 += *p3++;\
      sum2 -= *p1++ << 1;\
      p2++;\
      sum2 += *p3++ << 1;\
      sum1 += *p1;\
      sum2 -= *p1--;\
      sum1 += *p2-- << 1;\
      sum1 += *p3;\
      sum2 += *p3--;

#define SOBELN(sum1, sum2)\
      sum1 = -p1[0] -p2[0] -p2[0] -p3[0] +  p1[1] + p2[1]+ p2[1] + p3[1];\
      sum2 = -p1[0] - 3 * p1[1] + p3[0] + 3 * p3[1];

#define PREWITT1(sum1, sum2)\
      sum1 = -*p1 -*p2 -*p3 +  p1[1] + p2[1] + p3[1];\
      sum2 = -2 * *p1 - p1[1] + 2 * *p3 + p3[1];

#define PREWITTI(sum1, sum2)\
      sum1  = sum2  = - *p1++;\
      sum1 -= *p2++;\
      sum1 -= *p3;\
      sum2 += *p3++;\
      sum2 -= *p1++;\
      p2++;\
      sum2 += *p3++;\
      sum1 += *p1;\
      sum2 -= *p1--;\
      sum1 += *p2--;\
      sum1 += *p3;\
      sum2 += *p3--;

#define PREWITTN(sum1, sum2)\
      sum1 = -p1[0] -p2[0] -p3[0] +  p1[1] + p2[1] + p3[1];\
      sum2 = -p1[0] - 2 * p1[1] + p3[0] + 2 * p3[1];

#define ROBERTS1(sum1, sum2)\
      sum1  = *p2++;\
      sum2  = -*p3++;\
      sum1 -= *p3;\
      sum2 += *p2;

#define ROBERTSI(sum1, sum2)\
      sum1  = *p2++;\
      sum2  = -*p3++;\
      sum1 -= *p3;\
      sum2 += *p2;

#define ROBERTSN(sum1, sum2)\
      sum1 = *p2 - *p3;\
      sum2 = -*p3 + *p2;

#define GRAD1(sum1, sum2)\
      sum1  = sum2 = -*p2++;\
      sum1 += *p2;\
      sum2 += *p3++;

#define GRADI(sum1, sum2)\
      sum1  = sum2 = -*p2++;\
      sum1 += *p2;\
      sum2 += *p3++;

#define GRADN(sum1, sum2)\
      sum1 = 0;\
      sum2 = -*p2 + *p3;



/*F:sobel=gradient*/
/*F:prewitt=gradient*/
/*F:roberts=gradient*/
/*F:grad=gradient*/
/*F:gradient*

________________________________________________________________

		gradient
________________________________________________________________

Name:		gradient, grad, prewitt, roberts, sobel - gradient
                operator for edge detection
Syntax:         | #include <xite/convolve.h>
		|
                | int gradient( IBAND input, ISS_BAND convx,
                |    ISS_BAND convy, double xscale,
                |    double yscale, IBAND magnitude, IBAND angle,
                |    IBAND pseudo, int verbose );
		|
                | int sobel( IBAND input, IBAND magnitude,
                |    IBAND angle, IBAND pseudo, int verbose );
		| 
                | int prewitt( IBAND input, IBAND magnitude,
                |    IBAND angle, IBAND pseudo, int verbose );
		|
                | int roberts( IBAND input, IBAND magnitude,
                |    IBAND angle, IBAND pseudo, int verbose );
		|
                | int grad( IBAND input, IBAND magnitude,
                |    IBAND angle, IBAND pseudo, int verbose );
		|
Description:    'gradient' performs a convolution of 'input' and 'convx',
		scaled by 'xscale' (or 0.0 for autoscaling), giving 'gx',
		and  a convolution of 'input' and 'convy', scaled by 'yscale' 
		((or 0.0 for autoscaling), giving 'gy'. 'gx' and 'gy' are 
		combined into magnitude, angle and a pseudo image.
		If 'verbose' is TRUE some messages are witten with Info().

		| magnitude = |gx| + |gy|
		| angle     =  atan(gy/gx);
		| pseudo    = combination of magnitude and angle.
		
		'sobel' calculates gx and gy as
                |
		|        -1 0 1             -1 -2 -1
		| gx =   -2 0 2       gy =   0  0  0  
		|        -1 0 1              1  2  1
		| 

		'prewitt' calculates gx and gy as
                |
		|        -1 0 1             -1 -1 -1
		| gx =   -1 0 1       gy =   0  0  0  
		|        -1 0 1              1  1  1
		| 

		'roberts' calculates gx and gy as
                |
		|        1  0              0  1
		| gx =               gy =
		|        0 -1             -1  0
		| 

		'grad' calculates gx and gy as
                |
		|       -1  1             -1  0
		| gx =               gy =
		|        0  0              1  0
		| 
Pseudo:		The pseudo comination of magnitude and pseudo:
                | mag level = magnitude / 32;
		|  Level  First Length
		|      0      0      4
		|      1      4     12
		|      2     16     20
		|      3     36     28
                |      4     64     36
                |      5    100     44
		|      6    144     52
		|      7    196     60
		| pseudo = first[mag_level] + angle/256* length[mag_level];
		The pseudo-image may be displayed with a "gradient"
		color-table.

See also:	convolve(3), conv3x3(3), ascii2biff(3)
Author:		Otto Milvang
Id: 		$Id$
________________________________________________________________

*/

#ifndef GRADIENT

#ifndef FUNCPROTO
int OP(input, magnitude, angle, pseudo, verbose)
IBAND input, magnitude, angle, pseudo;
int verbose;
#else /* FUNCPROTO */
int OP(IBAND input, IBAND magnitude, IBAND angle, IBAND pseudo, int verbose)
#endif /* FUNCPROTO */
{
  UNS_BYTE *p1, *p2, *p3, *rm, *ra, *rp;
  int lum, hue, sum, sum1, sum2, count;
  int i, j, k, xsize, ysize, y, mode;

  if (verbose)
    Info(0, "%s%s%s%s\n", NAME, 
	    magnitude ? " magnitude" : "", 
	    angle ? " angle" : "",
	    pseudo ? " pseudo" : "");
  if (Ipixtyp(input) != Iu_byte_typ)
    return(Error(2, "Input band must have pixel type unsigned byte.\n"));

  xsize = Ixsize(input);
  ysize = Iysize(input);
/*  if (Ixsize(magnitude) != xsize || Iysize(magnitude) != ysize ||
      Ixsize(angle)     != xsize || Iysize(angle) != ysize) return(3);
*/

  if (pseudo)
    {
      k = 0;
      for (i=0; i<8; i++)
	{
	  y = (i*2 + 1)*4;
	  for(j = 0; j<y; j++)
	    pseudo_arr_s[k++] = i;
	}
    }

  mode = 0;
  if (magnitude) mode += 1;
  if (angle)     mode += 2;
  if (pseudo)    mode += 4;

  for (y=1; y <= ysize; y++)
    { 
      p1 = &input[y > 1 ? y-1 : 1][1];
      p2 = &input[y][1];
      p3 = &input[y < ysize ? y+1 : ysize][1];
      if (magnitude) rm = &magnitude[y][1];
      if (angle)     ra = &angle[y][1];
      if (pseudo)    rp = &pseudo[y][1];

      OP1(sum1, sum2);
      MAG(lum, sum1, sum2);
      ANG(hue, sum1, sum2);
      PSE(sum, lum, hue);
      if (magnitude) *rm++ = lum;
      if (angle)     *ra++ = hue;
      if (pseudo)    *rp++ = sum;
      count = xsize -2;
      switch (mode)
	{	
	case 0:
	  break;
	case 1:
	  while(count--)
	    {
	      OPI(sum1, sum2);
	      MAG(lum, sum1, sum2);
	      *rm++ = lum;
	    }
	  break;
	case 2:
	  while(count--)
	    {
	      OPI(sum1, sum2);
	      ANG(hue, sum1, sum2);
	      *ra++ = hue;
	    }
	  break;
	case 3:
	  while(count--)
	    {
	      OPI(sum1, sum2);
	      MAG(lum, sum1, sum2);
	      ANG(hue, sum1, sum2);
	      *rm++ = lum;
	      *ra++ = hue;
	    }
	  break;
	case 4:
	case 5:
	case 6:
	case 7:
	  while(count--)
	    {
	      OPI(sum1, sum2);
	      MAG(lum, sum1, sum2);
	      ANG(hue, sum1, sum2);
	      PSE(sum, lum, hue);
	      if (magnitude) *rm++ = lum;
	      if (angle)     *ra++ = hue;
	      *rp++ = sum;
	    }
	  break;
	}
      OPN(sum1, sum2);
      MAG(lum, sum1, sum2);
      ANG(hue, sum1, sum2);
      PSE(sum, lum, hue);
      if (magnitude) *rm++ = lum;
      if (angle)     *ra++ = hue;
      if (pseudo)    *rp++ = sum;
    }
  return(0);
}

#endif /* not GRADIENT */

#ifdef GRADIENT

#define OPI(sum1, sum2)\
	  sum1 = *s1++;\
	  sum2 = *s2++;

#ifndef FUNCPROTO
int gradient(input, convx, convy, xscale, yscale, magnitude, angle, pseudo, verbose)
IBAND input, magnitude, angle, pseudo;
ISS_BAND convx, convy;
double xscale, yscale; 
int verbose;
#else /* FUNCPROTO */
int gradient(IBAND input, ISS_BAND convx, ISS_BAND convy, double xscale, double yscale, IBAND magnitude, IBAND angle, IBAND pseudo, int verbose)
#endif /* FUNCPROTO */
{
  ISS_BAND gx, gy;
  UNS_BYTE *rm, *ra, *rp;
  short *s1, *s2;
  int lum, hue, sum, sum1, sum2, count;
  int i, j, k, xsize, ysize, y, mode, stat;

  if (verbose)
    Info(0, "%s%s%s%s\n", "gradient",
	    magnitude ? " magnitude" : "", 
	    angle ? " angle" : "",
	    pseudo ? " pseudo" : ""); 
  xsize = Ixsize(input);
  ysize = Iysize(input);

  if (pseudo)
    {
      k = 0;
      for (i=0; i<8; i++)
	{
	  y = (i*2 + 1)*4;
	  for(j = 0; j<y; j++)
	    pseudo_arr_s[k++] = i;
	}
    }

  gx = (ISS_BAND) Imake_band(Is_short_typ, xsize, ysize);
  gy = (ISS_BAND) Imake_band(Is_short_typ, xsize, ysize);

  if (gx == NULL || gy == NULL)  return(Error(3, "Cannot allocate memory.\n"));
  stat = convolve(input, (IBAND) gx, convx, xscale, 0.0, verbose);
  stat = convolve(input, (IBAND) gy, convy, yscale, 0.0, verbose);

  mode = 0;
  if (magnitude) mode += 1;
  if (angle)     mode += 2;
  if (pseudo)    mode += 4;

  for (y=1; y <= ysize; y++)
    { 
      s1 = &gx[y][1];
      s2 = &gy[y][1];
      if (magnitude) rm = &magnitude[y][1];
      if (angle)     ra = &angle[y][1];
      if (pseudo)    rp = &pseudo[y][1];
      count = xsize;
      switch (mode)
	{	
	case 0:
	  break;
	case 1:
	  while(count--)
	    {
	      OPI(sum1, sum2);
	      MAG(lum, sum1, sum2);
	      *rm++ = lum;
	    }
	  break;
	case 2:
	  while(count--)
	    {
	      OPI(sum1, sum2);
	      ANG(hue, sum1, sum2);
	      *ra++ = hue;
	    }
	  break;
	case 3:
	  while(count--)
	    {
	      OPI(sum1, sum2);
	      MAG(lum, sum1, sum2);
	      ANG(hue, sum1, sum2);
	      *rm++ = lum;
	      *ra++ = hue;
	    }
	  break;
	case 4:
	case 5:
	case 6:
	case 7:
	  while(count--)
	    {
	      OPI(sum1, sum2);
	      MAG(lum, sum1, sum2);
	      ANG(hue, sum1, sum2);
	      PSE(sum, lum, hue); 
	      if (magnitude) *rm++ = lum;
	      if (angle)     *ra++ = hue;
	      *rp++ = sum;
	    }
	  break;
	}
    }
  Idel_band((IBAND *) &gx);
  Idel_band((IBAND *) &gy);
  return(0);
}

#endif /* GRADIENT */

#endif /* not MAIN */



#ifdef MAIN

/*P:sobel=gradient*/
/*P:prewitt=gradient*/
/*P:roberts=gradient*/
/*P:grad=gradient*/
/*P:gradient*

________________________________________________________________

		gradient
________________________________________________________________

Name:		gradient, sobel, prewitt, roberts, grad - gradient like
                operators

Syntax:         | gradient [-angle] [-magnitude] [-pseudo]
                |          [-xscale <xscale>] [-yscale <yscale>]
                |          [-title  <title>] [-verbose] 
                |          <inimage> <outimage> <xconvfile> <yconvfile>
                | 
		| sobel    [-angle] [-magnitude] [-pseudo]
		|          [-title  <title>] <inimage> <outimage>  
		|
		| prewitt  [-angle] [-magnitude] [-pseudo]
		|          [-title  <title>] <inimage> <outimage>  
		|
		| roberts  [-angle] [-magnitude] [-pseudo]
		|          [-title  <title>] <inimage> <outimage>  
		|
		| grad     [-angle] [-magnitude] [-pseudo]
		|          [-title  <title>] <inimage> <outimage>  

Description:    'gradient' performs a gradient operator on the input image
                'inimage' and writes the result to 'outimage'.
		
		The convolution kernels are specified by 'xconvfile' and
		'yconvfile'.

		'sobel', 'roberts', 'prewitt' and 'grad' use standard
		convolution kernels (see below).

Options:        &-magnitude
                Combine horizontal and vertical contributions by adding
		the absolute values.
		| magnitude = |convx| + |convy|

		&-angel
		Combine horizontal and vertical contributions by calculating
		the gradient angle.
		| angle = atan(convy/convx)

		&-pseudo
		Combine horizontal and vertical contributions by combining
		the magnitude and angle into a pseudo image.
		| pseudo = pseudo combination of magnitude and angle

		The pseudo comination of magnitude and pseudo:
                | mag level = magnitude / 32;
		|  Level  First Length
		|      0      0      4
		|      1      4     12
		|      2     16     20
		|      3     36     28
                |      4     64     36
                |      5    100     44
		|      6    144     52
		|      7    196     60
		| pseudo = first[level] + angle/256*length[level]

		The pseudo-image may be displayed with a "gradient"
		color-table.

		&-xscale xscale
		Scale magnitude data. Default is 'xscale'=0, which gives
		autoscaling.
                | xpix = (xconv. result) * xscale

		&-yscale yscale
		Scale magnitude data. Default is 'yscale'=0, which gives
		autoscaling.
                | ypix = (yconv. result) * yscale;

		&-title title
		Set title of output image to 'title'.

		&-verbose
		Write a message	if the convolution sum is less than 0 or
		greater than 255.

Standard convolution kernels:

		&sobel:
                |
		|            -1 0 1                 -1 -2 -1
		| xconv =    -2 0 2        yconv =   0  0  0  
		|            -1 0 1                  1  2  1
		| 

		&prewitt
                |
		|            -1 0 1                 -1 -1 -1
		| xconv =    -1 0 1        yconv =   0  0  0  
		|            -1 0 1                  1  1  1
		| 

		&roberts
                |
		|            1  0                  0  1
		| xconv =                 yconv =
		|            0 -1                 -1  0
		| 

		&grad
                |
		|           -1  1                 -1  0
		| xconv =                 yconv =
		|            0  0                  1  0
		| 

See also:	convolve(1), conv3x3(1), ascii2biff(1)
Author:		Otto Milvang
Id: 		$Id$
________________________________________________________________

*/

#ifndef FUNCPROTO
int main(argc,argv)
int argc;
char *argv[];
#else /* FUNCPROTO */
int main(int argc, char **argv)
#endif /* FUNCPROTO */
{
  IMAGE img, img2;
  IBAND bm, ba, bp;
#ifdef GRADIENT
  ISS_BAND c1, c2;
#endif /* GRADIENT */
  int n, stat, bn, angle, mag, pseudo, verbose, num;
  double xscale, yscale;
  char *title, *args;
  int xstart, ystart;

  InitMessage(&argc, argv, xite_app_std_usage_text(
#ifdef GRADIENT
      "Usage: %s [-angle] [-magnitude] [-pseudo]\n\
      [-xscale <xscale>] [-yscale <yscale>]\n\
      [-title  <title>] \n\
      <inimage> <outimage> <xconvfile> <yconvfile>\n"));
#else /* GRADIENT */
      "Usage: %s [-angle] [-magnitude] [-pseudo]\n\
      [-title  <title>] <inimage> <outimage>\n"));
#endif /* GRADIENT */

  Iset_message(1);
  Iset_abort(1);

  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);

  angle   = read_bswitch(&argc, argv, "-a") ||  
            read_bswitch(&argc, argv, "-angle");
  mag     = read_bswitch(&argc, argv, "-m") ||  
            read_bswitch(&argc, argv, "-magnitude");
  pseudo  = read_bswitch(&argc, argv, "-p") ||  
            read_bswitch(&argc, argv, "-pseudo");
  title   = read_switch (&argc, argv, "-title", 1,
	    read_switch (&argc, argv, "-t", 1, NULL));
  xscale  = read_dswitch(&argc, argv, "-xscale",
	    read_dswitch(&argc, argv, "-x", 0.0));
  yscale  = read_dswitch(&argc, argv, "-yscale",
	    read_dswitch(&argc, argv, "-y", 0.0));
  verbose = Verbose();

  num = (angle ? 1 : 0) + (mag ? 1 : 0) + (pseudo ? 1 : 0);
  if (num == 0) {
    mag = 1;
    num = 1;
  }

  if (argc > 1) {
    img = Iread_image(argv[1]);
    if (img == NULL) Error(2, "Can't read input file %s.\n", argv[1]);
    n = Inbands(img);

    img2 = Init_image(n*num, title ? title : Ititle(img));
    if (img2 == NULL) Error(2, "Malloc error (new image).\n");

    if (Icopy_text(img, img2) != Iok) {
      Error(2, "Error in Icopy_text().\n");
    }
  }
  n = 1;

#ifdef GRADIENT
  if (argc != 5) Usage(2, NULL);
  stat = ascii2biff((IBAND *) &c1, argv[3], Is_short_typ);
  stat = ascii2biff((IBAND *) &c2, argv[4], Is_short_typ);

  for (bn=1; bn <= Inbands(img); bn++)
    {
      bm = ba = bp = NULL; 
      xstart = Ixstart(img[bn]);
      ystart = Iystart(img[bn]);
      if (mag)
	{
	  bm =  Imake_band(Iu_byte_typ, Ixsize(img[bn]), Iysize(img[bn]));
	  if (bm == NULL) 
	      Error(2, "Malloc error (New image, band %d)\n", bn);
	  Iset_start(bm, xstart, ystart);
	  img2[n++] = bm;
	}
      if (angle)
	{
	  ba =  Imake_band(Iu_byte_typ, Ixsize(img[bn]), Iysize(img[bn]));
	  if (ba == NULL) 
	      Error(2, "Malloc error (New image, band %d)\n", bn);
	  Iset_start(ba, xstart, ystart);
	  img2[n++] = ba;
	}
      if (pseudo)
	{
          bp =  Imake_band(Iu_byte_typ, Ixsize(img[bn]), Iysize(img[bn]));
          if (bp == NULL)
	    Error(2, "Malloc error (New image, band %d)\n", bn);
	  Iset_start(bp, xstart, ystart);
	  img2[n++] = bp;
        }
      stat = gradient(img[bn], c1, c2, xscale, yscale, bm, ba, bp, verbose);
      if (stat) Error(2, "Error in gradient \n");
    }

#else /* GRADIENT */

  if (argc != 3) Usage(2, NULL);
  for (bn=1; bn <= Inbands(img); bn++)
    {
      bm = ba = bp = NULL; 
      xstart = Ixstart(img[bn]);
      ystart = Iystart(img[bn]);
      if (mag)
	{
	  bm =  Imake_band(Iu_byte_typ, Ixsize(img[bn]), Iysize(img[bn]));
	  if (bm == NULL)	  
            Error(2, "Malloc error (New image, band %d)\n", bn);
	  Iset_start(bm, xstart, ystart);
	  img2[n++] = bm;
	}
      if (angle)
	{
	  ba =  Imake_band(Iu_byte_typ, Ixsize(img[bn]), Iysize(img[bn]));
	  if (ba == NULL)
	    Error(2, "Malloc error (New image, band %d)\n", bn);
	  Iset_start(ba, xstart, ystart);
	  img2[n++] = ba;
	}
      if (pseudo)
	{
	  bp =  Imake_band(Iu_byte_typ, Ixsize(img[bn]), Iysize(img[bn]));
	  if (bp == NULL)
	      Error(2, "Malloc error (New image, band %d)\n", bn);
	  Iset_start(bp, xstart, ystart);
	  img2[n++] = bp;
	}
      stat = OP(img[bn], bm, ba, bp, verbose);
      if (stat) Error(2, "Error in gradient \n");
    }

#endif /* GRADIENT */

    Ihistory(img2, argv[0], args); 
    Iwrite_image(img2, argv[2]);

    return(0);
}

#endif /* MAIN */
