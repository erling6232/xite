

/*

________________________________________________________________

        ihs.c
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
#include <xite/ihs.h>
#include XITE_STDIO_H

#ifdef DEBUG
#define PRINTF(a,b,c,d)  printf(a,b,c,d)
#else
#define PRINTF(a,b,c,d)
#endif

#define ONE_PI  3.141592654
#define TWO_PI  6.283185308

#define PI_0    0.000000000
#define PI_1    1.047197551
#define PI_2    2.094395103
#define PI_3    ONE_PI
#define PI_4    4.188790206
#define PI_5    5.235987757
#define PI_6    TWO_PI

#define A11     0.333333333
#define A12	A11
#define A13	A11
#define A21     -0.40824829
#define A22	A21
#define A23     0.81649658
#define A31     0.707106781
#define A32     -0.707106781
#define A33     0.0

#define F1      0.577350269  /* 1.0 / sqrt(3.0) */
#define F2      0.159154943  /* 1.0 / sqrt(2.0) */
#define F3      0.408248290  /* 1.0 / sqrt(6.0) */
#define F4      0.81649658   /* 2.0 / sqrt(6.0) */
#define F5      0.866025403  /* sin(60)         */

#define L1      0.577350269
#define L2      1.154700539
#define L3      1.732050808

#define scale(f, s) if (f<0.0)\
		      f=0.0; else {\
			f *= (s);\
			if (f >= 1.0) f=0.99999999;\
			}

#define iscale(f, s) f /= (s)

#define fscale(f)  if (f <= 0.0) f = 0.0; else\
			if (f >= 1.0) f=0.99999999;




/*F:rgb2ihs=ihs*/
/*F:ihs2rgb=ihs*/
/*F:rgb2ihs_byte=ihs*/
/*F:ihs2rgb_byte=ihs*/
/*F:rgb2ihs_word=ihs*/
/*F:ihs2rgb_word=ihs*/
/*F:ihs*

________________________________________________________________

		ihs
________________________________________________________________

Name:		ihs, rgb2ihs, ihs2rgb, rgb2ihs_byte, ihs2rgb_byte,
                rgb2ihs_word, ihs2rgb_word - Convert between RGB and IHS
Syntax:         | #include <xite/ihs.h>
		|
                | void rgb2ihs( double r, double g, double b,
                |    double* i, double* h, double* s, int norm );
		|
                | void ihs2rgb( double i, double h, double s,
                |    double* r, double* g, double* b, int norm );
		|
                | void rgb2ihs_byte( unsigned char r,
                |    unsigned char g, unsigned char b,
                |    unsigned char *i, unsigned char *h,
                |    unsigned char *s );
		|
                | void ihs2rgb_byte( unsigned char i,
                |    unsigned char h, unsigned char s,
                |    unsigned char *r, unsigned char *g,
                |    unsigned char *b );
		|
                | void rgb2ihs_word( unsigned short r,
                |    unsigned short g, unsigned short b,
                |    unsigned short *i, unsigned short *h,
                |    unsigned short *s );
		|
                | void ihs2rgb_word( unsigned short i,
                |    unsigned short h, unsigned short s,
                |    unsigned short *r, unsigned short *g,
                |    unsigned short *b );
Description:	'rgb2ihs' converts from RGB-space to IHS-space.
                'ihs2rgb' converts from IHS-space to RGB-space.
                rgb values are normalized 
		| 0.0 <= r <= 1.0, 0.0 <= b <= 1.0, 0.0 <= b <= 1.0, 
		If norm = TRUE the IHS values are  normalized.
		| 0.0 <= i <= 1.0, 0.0 <= h <= 1.0, 0.0 <= s <= 1.0, 
		If norm = FALSE:
		| 0.0 <= i <= 1.0
		| 0.0 <= h <  1.0
		| 0.0 <= s <= Smax(I,H)
		'rgb2ihs_xxxx' and 'ihs2rgb_xxxx' normalize the data
		in the range 0-255 for xxxx=byte and 0-65535 for xxxx=word
rgb2ihs:	| i = A11 * r + A12 * g + A13 * b;
		| u = A21 * r + A22 * g + A23 * b;
		| v = A31 * r + A32 * g + A33 * b;
		|
		| h = atan2(v, u) , 0 <= h < 2*pi
		| s = sqrt(u*u + v*v)
		|
ihs2rgb:	| u = s*cos(h)
		| v = s*sin(h)
		| 
		| r =  i + A21 * u + A31 * v;
		| g =  i + A22 * u + A32 * v;
		| b =  i + A23 * u + A33 * v;
		| 
Values:		| A11 = A12 = A13 =  1./3.
		| A21 = A22       = -1./sqrt(6.)
		| A23             =  2./sqrt(6.)
		| A31 = -A32      =  1./sqrt(2.)
		| A33             =  0.
		|
Normalization:	| i' = i / sqrt(3.0)
                | h' = h / 2*pi
		| s' = s / (Max s for (i,h)) 
See also:       ihs2rgb(1), ihs2rgb_img(3), rgb2ihs(1), rgb2ihs_img(3)
Author:		Otto Milvang
________________________________________________________________

*/

void rgb2ihs(double r, double g, double b, double *i, double *h, double *s, int norm)
{
  double fi, fh, fs, hh, v1, v2, cosh, sinh;
  double stop, sbot, sglob, imax, s1, s2, s3;
  if (r == g && r == b)
    {
      *i = r;
      *h = 0.0;
      *s = 0.0;
    } else {
      PRINTF("rgb: %10.4f %10.4f %10.4f\n", r, g, b);
      fi = A11 * r + A12 * g + A13 * b;
      v1 = A21 * r + A22 * g + A23 * b;
      v2 = A31 * r + A32 * g + A33 * b;
      PRINTF("ivw: %10.4f %10.4f %10.4f\n", fi, v1, v2);
      fh = atan2(v2, v1);
      if (fh<0.0) fh = fh + TWO_PI;
      hh = fh;
      scale(fh, F2);
      fs = sqrt(v1*v1 + v2*v2);
      PRINTF("ihs: %10.4f %10.4f %10.4f\n", fi, fh, fs);
      
      if (norm)
	{
	  cosh = v1/fs * F4;
	  sinh = v2/fs * F4;
	  s3 = cosh;
	  s1 = -cosh * 0.5 + sinh * F5;
	  s2 = -cosh * 0.5 - sinh * F5;
	  stop = s1;
          if (s2 < stop) stop = s2;
          if (s3 < stop) stop = s3;
	  sbot = s1;
          if (s2 > sbot) sbot = s2;
          if (s3 > sbot) sbot = s3;
	  sglob = 1.0/(sbot - stop);
	  imax = -stop*sglob;
	  if (fi <= imax)
	    fs = fs * imax/ fi / sglob; else
	    fs = fs * (1.0-imax)/ (1.0-fi) / sglob;
	}
      PRINTF("ihs: %10.4f %10.4f %10.4f\n", fi, fh, fs);
      
      *i = fi;
      *h = fh;
      *s = fs;
    }
}

void ihs2rgb(double i, double h, double s, double *r, double *g, double *b, int norm)
{
  double fr, fg, fb, cosh, sinh;
  double stop, sbot, sglob, imax, s1, s2, s3;

  if (i <= 0.0) *r = *g = *b = 0.0; else
  if (i >= 1.0) *r = *g = *b = 1.0; else
  if (s <= 0.0) *r = *g = *b = i; else 
    {
      if (s > 1.0) s = 1.0;
      PRINTF("ihs: %10.4f %10.4f %10.4f\n", i, h, s);
      h = TWO_PI * h;
      cosh = cos(h) * F4;
      sinh = sin(h) * F4;
      s3 = cosh;
      s1 = -cosh * 0.5 + sinh * F5;
      s2 = -cosh * 0.5 - sinh * F5;      
      if (norm)
	{
	  stop = s1;
          if (s2 < stop) stop = s2;
          if (s3 < stop) stop = s3;
	  sbot = s1;
          if (s2 > sbot) sbot = s2;
          if (s3 > sbot) sbot = s3;
	  sglob = 1.0/(sbot - stop);
	  imax = -stop*sglob;
	  if (i <= imax)
	    s = s / imax* i * sglob; else
	    s = s / (1.0-imax)* (1.0-i) * sglob;
	  
	  PRINTF("ihs: %10.4f %10.4f %10.4f\n", i, h, s);
	}
      fr = i + s*s1;
      fg = i + s*s2;
      fb = i + s*s3;
      fscale(fr);
      fscale(fg);
      fscale(fb);
      PRINTF("rgb: %10.4f %10.4f %10.4f\n", fr, fg, fb);
      *r = fr;
      *g = fg;
      *b = fb;
    }
}


void rgb2ihs_byte(unsigned char r, unsigned char g, unsigned char b, unsigned char *i, unsigned char *h, unsigned char *s)
{
  double fi, fh, fs;
  int ii, ih, is;
  rgb2ihs( ((double) r + 0.5) / 256.0, ((double) g + 0.5) / 256.0, 
	   ((double) b + 0.5) / 256.0, &fi, &fh, &fs, 1);

  ii = fi * 255.99999;
  ih = fh * 255.99999;
  is = fs * 255.99999;
  if (ii < 0) ii = 0; else if (ii>255) ii = 255;
  if (ih < 0) ih = 0; else if (ih>255) ih = 255;
  if (is < 0) is = 0; else if (is>255) is = 255;
  *i = ii;
  *h = ih;
  *s = is;
}

void ihs2rgb_byte(unsigned char i, unsigned char h, unsigned char s, unsigned char *r, unsigned char *g, unsigned char *b)
{
  double fr, fg, fb;
  int ir, ig, ib;
  ihs2rgb( ((double) i + 0.5) / 256.0, ((double) h + 0.5) / 256.0, 
	   ((double) s + 0.5) / 256.0, &fr, &fg, &fb, 1);
  ir = fr * 255.99999;
  ig = fg * 255.99999;
  ib = fb * 255.99999;
  if (ir < 0) ir = 0; else if (ir>255) ir = 255;
  if (ig < 0) ig = 0; else if (ig>255) ig = 255;
  if (ib < 0) ib = 0; else if (ib>255) ib = 255;
  *r = ir;
  *g = ig;
  *b = ib;
}

void rgb2ihs_word(short unsigned int r, short unsigned int g, short unsigned int b, short unsigned int *i, short unsigned int *h, short unsigned int *s)
{
  double fi, fh, fs;
  int ii, ih, is;
  rgb2ihs( ((double) r + 0.5) / 65536.0, ((double) g + 0.5) / 65536.0, 
	   ((double) b + 0.5) / 65526.0, &fi, &fh, &fs, 1);
  ii = fi * 65535.99; 
  ih = fh * 65535.99;
  is = fs * 65535.99;
  if (ii < 0) ii = 0; else if (ii>65535) ii = 65535;
  if (ih < 0) ih = 0; else if (ih>65535) ih = 65535;
  if (is < 0) is = 0; else if (is>65535) is = 65535;
  *i = ii;
  *h = ih;
  *s = is;
}

void ihs2rgb_word(short unsigned int i, short unsigned int h, short unsigned int s, short unsigned int *r, short unsigned int *g, short unsigned int *b)
{
  double fr, fg, fb;
  int ir, ig, ib;
  ihs2rgb( ((double) i + 0.5) / 65536.0, ((double) h + 0.5) / 65536.0, 
	   ((double) s + 0.5) / 65536.0, &fr, &fg, &fb, 1);
  ir = fr * 65535.99;
  ig = fg * 65535.99;
  ib = fb * 65535.99;
  if (ir < 0) ir = 0; else if (ir>65535) ir = 65535;
  if (ig < 0) ig = 0; else if (ig>65535) ig = 65535;
  if (ib < 0) ib = 0; else if (ib>65535) ib = 65535;
  *r = ir;
  *g = ig;
  *b = ib;
}
