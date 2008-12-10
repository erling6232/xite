/*

________________________________________________________________

        zernike.c
        $Id$
	Copyright 1996, Blab, UiO
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

static char *Id = "$Id$";

#include <xite/includes.h>
#include XITE_STDIO_H
#include XITE_TIME_H
#include <math.h>
#include <stdlib.h>
#include <signal.h>
#include <xite/blab.h>
#include <xite/biff.h>
#include <xite/histo.h>
#include <xite/readarg.h>
#include <xite/zernike.h>
#include <xite/message.h>

#ifndef MAIN

#ifndef M_PI
# define M_PI    3.14159265358979323846
#endif

/*
static void error_handler(value)
{
  printf ("ERROR, SIGFPE trigged\n");
  exit(0);
}

*/



static int zer_pol_R(int n, int m_in, double x, double y, double *res) 
{
  int i;
  int m;
  int s;
  int a; /* (n-s)! */
  int b; /*   s!   */
  int c; /* [(n+|m|)/2-s]! */
  int d; /* [(n-|m|)/2-s]! */
  int sign;

  m = abs(m_in);

  if ((n-m)%2!=0)
    Warning(1,
	    "zer_pol_R: Warning. R(%i,%i,%5.2f,%5.2f). n-|m| = %i is odd!\n", 
	    n, m, x, y, m);

  /* The code is optimized with respect to the faculty operations */

  res[0] = 0.0;
  if ((x*x + y*y) <= 1.0)
    {
      sign = 1;
      a = 1;
      for (i=2; i<=n; i++)
	a*=i;
      b=1;
      c = 1;
      for (i=2; i <= (n+m)/2; i++)
	c*=i;
      d = 1;
      for (i=2; i <= (n-m)/2; i++)
	d*=i;
      
      /* Before the loop is entered, all the integer variables (sign, a, */
      /* b, c, d) have their correct values for the s=0 case. */
      for (s=0; s<= (n-m)/2; s++)
	{
	  /*printf("zer_pol_R: s=%i, n=%i, m=%i, x=%6.3f, y=%6.3f, a=%i, */
	  /*b=%i, c=%i, d=%i, sign=%i\n", s,n,m,x,y,a,b,c,d,sign); */
	  res[0] += sign * (a*1.0/(b*c*d)) * pow((x*x + y*y),(n/2.0)-s);
	  
	  /* Now update the integer variables before the next iteration of */
	  /* the loop. */
	  
	  if (s < (n-m)/2)
	    {
	      sign = -sign;
	      a = a/(n-s);
	      b = b*(s+1);
	      c = c / ((n+m)/2 - s);
	      d = d / ((n-m)/2 - s);
	    }
	}      
    }
  return ZER_OK;
}



int zer_pol(int n, int m, double x, double y, DCOMPLEX *res) 
{
  double R;
  double arg;

  if ((x*x + y*y) > 1.0)
    {
      res->re = 0.0;
      res->im = 0.0;
    }
  else
    {
      zer_pol_R(n,m,x,y, &R);
      arg = m*atan2(y,x);
      res->re = R*cos(arg);
      res->im = R*sin(arg);
    }

  return ZER_OK;
}




/*F:zer_con=zer_mom*/
/*F:zer_rec=zer_mom*/
/*F:zer_pol=zer_mom*/
/*F:zer_mom*

________________________________________________________________

		zer_mom
________________________________________________________________

Name:		zer_mom, zer_pol, zer_rec, zer_con - Zernike moments

Syntax:		| #include <xite/zernike.h>
                |
                | int zer_mom( IBAND inband, int n, int m,
		|    DCOMPLEX *res );
		|
		| int zer_pol( int n, int m, double x, double y,
		|    DCOMPLEX *res );
		|
		| int zer_con( int n, IBAND inband, IBAND outband );
		|
		| int zer_rec( int order, IBAND inband, IBAND outband );
		|

Description:    The Zernike moment of order 'n' and repetition 'm' of
                an image f(x,y) is defined as follows:
		|          n+1                             *
                | A(n,m) = ---- Sum Sum f(x,y)[V(n,m, x,y)]  
		|           pi   x   y
		|
		| where x^2+y^2 <= 1
		|
		The image V(n,m, x,y) is the Zernike basis images of
		order 'n' and repetition 'm'. These basis images are
		complex and orthogonal. The Zernike moments are
		essentially the projections of the input image onto
		these basis images. 

		The original image can be reconstructed from the
		Zernike moments. The N-th order approximation is given
		by 
		|  ^         N
		|  f(x,y) = Sum Sum A(n,m) V(n,m, x,y)
		|           n=0  m
		|
		The contribution or information content of the n-th
		order moments is
		|
		|  I(x,y, n) = Sum A(n,m) V(n,m, x,y)
		|              m
		
		'zer_mom' computes A(n,m).

		'zer_pol' computes the zernike basis function
		V(n,m,x,y).

		'zer_con' computes the absolute	value of the contribution
		of the n-th order moments, i.e. the absolute value of
		I(x,y, n). 

		'zer_rec' demonstrates how the input image can be
		reconstructed from the Zernike basis functions and the
		Zernike moments. 

Return value:   | 0 : OK.

Author:		Øivind Due Trier, Dept. Informatics, Univ. Oslo
________________________________________________________________

*/



int zer_mom(IBAND inband, int n, int m, DCOMPLEX *res) 
{
  int i,j;
  double i_0, j_0;
  double i_scale, j_scale;
  double x,y;
  DCOMPLEX v;
  int isize, jsize;

  if (Ipixtyp(inband) != Iu_byte_typ)
    Error(1, "zer_mom: Input pixel type must be unsigned byte.\n");

  if ((n<0) || (abs(m) > n) || ((n-abs(m))%2!=0))
    Error(2, "zer_mom: n=%i, m=%i, n-|m|=%i\n", n, m, (n-abs(m)));

  isize = Ixsize(inband);
  jsize = Iysize(inband);

  i_0 = (isize+1)/2.0;
  j_0 = (jsize+1)/2.0;
  i_scale = isize/2.0;
  j_scale = jsize/2.0;

  res->re = 0.0;
  res->im = 0.0;

  for (i=1; i<=isize; i++)
    for (j=1; j<= jsize; j++)
      {
	x = (i-i_0)/i_scale;
	y = (j-j_0)/j_scale;
/*	printf ("zer_mom: x=%6.3f y=%6.3f i=%i j=%i\n", x,y,i,j);
*/
	if ((x*x + y*y) <= 1.0)
	  {
	    zer_pol(n,m,x,y, &v);
	    res->re += inband[j][i]*v.re;
	    res->im += inband[j][i]*(-v.im);
	  }
      }
  res->re = res->re*(n+1)/M_PI;
  res->im = res->im*(n+1)/M_PI;

  return ZER_OK;

} /* END zer_mom */



int zer_con(int n, IBAND inband, IBAND outband)
{
  IDC_BAND transformed;
  int m;
  int i, j;
  double i_0, j_0;
  double i_scale, j_scale;
  int isize, jsize;
  double x, y;
  double min, max, scale;
  DCOMPLEX a, v;

  if (Ipixtyp(inband) != Iu_byte_typ)
    Error(1, "zer_con: Input pixel type must be unsigned byte.\n");
  if (Ipixtyp(outband) != Iu_byte_typ)
    Error(2, "zer_con: Output pixel type must be unsigned byte.\n");

  isize = Ixsize(inband);
  jsize = Iysize(inband);

  i_0 = (isize+1)/2.0;
  j_0 = (jsize+1)/2.0;
  i_scale = isize/2.0;
  j_scale = jsize/2.0;

  transformed = (IDC_BAND) Imake_band(Id_complex_typ, isize, jsize);
  
  for (i=1; i<=isize; i++)
    for (j=1; j<=jsize; j++)
      {
	transformed[j][i].re = 0.0;
	transformed[j][i].im = 0.0;
      }
  for (m=-n; m<=n; m++)
    {
      if ((n-abs(m)) %2 == 0)
	{
	  zer_mom(inband, n,m, &a);
	  for (i=1; i<=isize; i++)
	    for (j=1; j<=jsize; j++)
	      {
		x = (i-i_0)/i_scale;
		y = (j-j_0)/j_scale;
/*		if (x*x + y*y <= 1.0) */
/* 		printf ("zer_con: x=%6.3f y=%6.3f i=%i j=%i\n", x,y,i,j);
*/
		zer_pol(n,m,x,y, &v);
		transformed[j][i].re += a.re*v.re - a.im*v.im;
		transformed[j][i].im += a.re*v.im + a.im*v.re;
/*		printf ("zer_con: x,y=(%6.3f,%6.3f), contr=(%9.6f,%9.6f),",
			x,y,transformed[j][i].re,
			transformed[j][i].im);*/
/*		printf (" a=(%9.6f,%9.6f), v=(%9.6f,%9.6f)\n",
			a.re, a.im, v.re, v.im);*/
	      }
	}
    }
  for (i=1; i<=isize; i++)
    for (j=1; j<=jsize; j++)
      {
	transformed[j][i].re = sqrt
	  (  transformed[j][i].re*transformed[j][i].re 
	   + transformed[j][i].im*transformed[j][i].im);
	transformed[j][i].im = 0.0;
      }
  min = max = transformed[1][1].re;
  for (i=1; i<=isize; i++)
    for (j=1; j<=jsize; j++)
      {
	if (transformed[j][i].re > max)
	  max = transformed[j][i].re;
	if  (transformed[j][i].re < min)
	  min = transformed[j][i].re;
      }
  if (max == min)
    scale = 0.0;
  else
    scale = 255.0/(max-min);

  Info(0, "zer_con: min=%9.6f, max=%9.6f, sacle=%f\n", min, max, scale);

  for (i=1; i<=isize; i++)
    for (j=1; j<=jsize; j++)
      {
	outband[j][i] = (UNS_BYTE) (transformed[j][i].re * scale);
/*	printf ("zer_con: outband[%3i][%3i]=%3i\n", j, i, outband[j][i]);*/
      }
  return ZER_OK;
} /* END zer_con */



int zer_rec(int order, IBAND inband, IBAND outband)
{
  IDC_BAND transformed;
  int n, m;
  int i, j;
  double i_0, j_0;
  double i_scale, j_scale;
  int isize, jsize;
  double x, y;
  DCOMPLEX a, v;

  if (Ipixtyp(inband) != Iu_byte_typ)
    Error(1, "zer_rec: Input pixel type must be unsigned byte.\n");
  if (Ipixtyp(outband) != Iu_byte_typ)
    Error(2, "zer_rec: Output pixel type must be unsigned byte.\n");

  isize = Ixsize(inband);
  jsize = Iysize(inband);

  i_0 = (isize+1)/2.0;
  j_0 = (jsize+1)/2.0;
  i_scale = isize/2.0;
  j_scale = jsize/2.0;

  transformed = (IDC_BAND) Imake_band(Id_complex_typ, isize, jsize);
  
  for (i=1; i<=isize; i++)
    for (j=1; j<=jsize; j++)
      {
	transformed[j][i].re = 0.0;
	transformed[j][i].im = 0.0;
      }
  for (n=1; n<=order; n++)
    {
      for (m=-n; m<=n; m++)
	{
	  if ((n-abs(m)) %2 == 0)
	    {
	      Info(0, "zer_rec: n=%3i, m=%3i.\n", n, m);
	      zer_mom(inband, n,m, &a);
	      for (i=1; i<=isize; i++)
		for (j=1; j<=jsize; j++)
		  {
		    x = (i-i_0)/i_scale;
		    y = (j-j_0)/j_scale;
		    /*if (x*x + y*y <= 1.0) */
		    /*printf ("zer_rec: x=%6.3f y=%6.3f i=%i j=%i\n", x,y,i,j);
		     */
		    zer_pol(n,m,x,y, &v);
		    transformed[j][i].re += a.re*v.re - a.im*v.im;
		    transformed[j][i].im += a.re*v.im + a.im*v.re;
		    /*printf ("zer_con: x,y=(%6.3f,%6.3f), contr=(%9.6f,%9.6f),",
		      x,y,transformed[j][i].re,
		      transformed[j][i].im);*/
		    /*printf (" a=(%9.6f,%9.6f), v=(%9.6f,%9.6f)\n",
		      a.re, a.im, v.re, v.im);*/
		  }
	    }
	}
    }
/*
  for (i=1; i<=isize; i++)
    for (j=1; j<=jsize; j++)
      {
	transformed[j][i].re = sqrt
	  (  transformed[j][i].re*transformed[j][i].re 
	   + transformed[j][i].im*transformed[j][i].im);
	transformed[j][i].im = 0.0;
      }
  min = max = transformed[1][1].re;
  for (i=1; i<=isize; i++)
    for (j=1; j<=jsize; j++)
      {
	if (transformed[j][i].re > max)
	  max = transformed[j][i].re;
	if  (transformed[j][i].re < min)
	  min = transformed[j][i].re;
      }
  if (max == min)
    scale = 0.0;
  else
    scale = 255.0/(max-min);
  printf ("zer_rec: min=%9.6f, max=%9.6f, sacle=%f\n", min, max, scale);
*/


  for (i=1; i<=isize; i++)
    for (j=1; j<=jsize; j++)
      {
	if (transformed[j][i].re < 0.0)
	  outband[j][i] = 0;
	else if (transformed[j][i].re > 255.0)
	  outband[j][i] = 255;
	else
	  outband[j][i] = (UNS_BYTE) (transformed[j][i].re);
/*	printf ("zer_con: outband[%3i][%3i]=%3i\n", j, i, outband[j][i]);*/
      }
  return ZER_OK;
} /* zer_rec */

#endif /* not MAIN */

#ifdef MAIN

/*P:zernike*

________________________________________________________________

		zernike
________________________________________________________________

Name:		zernike - zernike moment image of a gray scale 
                or binary image

Syntax:		zernike [-n <n>] [-r] [-t <title>] <inimage> <outimage>

Description:	The Zernike moment of order 'n' and repetition 'm' of
                an image f(x,y) is defined as follows:
		|          n+1                             *
                | A(n,m) = ---- Sum Sum f(x,y)[V(n,m, x,y)]  
		|           pi   x   y
		|
		| where x^2+y^2 <= 1
		|
		The image V(n,m, x,y) is the Zernike basis images of
		order 'n' and repetition 'm'. These basis images are
		complex and orthogonal. The Zernike moments are
		essentially the projections of the input image onto
		these basis images. 

		The original image can be reconstructed from the
		Zernike moments. The N-th order approximation is given
		by 
		|  ^         N
		|  f(x,y) = Sum Sum A(n,m) V(n,m, x,y)
		|           n=0  m
		|
		The contribution or information content of the n-th
		order moments is
		|
		|  I(x,y, n) = Sum A(n,m) V(n,m, x,y)
		|              m
		
Restrictions:   'inimage' must be single-band with pixel type unsigned byte.

Options:	&-n n
                Use moment order 'n'. Default 0.

                &-r
                Reconstruct image from moments. Default: Compute the absolute
		value of I(x,y, n).

		&-t title
		Use 'title' for 'outimage'.

See also:       zer_mom(3), zer_con(3), zer_rec(3), zer_pol(3)

References:     &[1] 'A. Khotanzad and Y.H. Hong'
                "Rotation invariant image recognition using features
		selected via a systematic method",
		Pattern Recognition, vol.23, no.10, pp.1089-1101, 1990. 

		&[2] 'Thomas H. Reiss'
		"Recognizing Planar Objects Using Invariant Image Features",
		Lecture Notes in Computer Science, volume 676, pp. 17-20,
		Springer-Verlag, 1993.

Return value:   0 : OK
Files:          | xite/src/zernike/zernike.c
Author:		Øivind Due Trier, Ifi, UiO.
________________________________________________________________

*/

int main(int argc, char *argv[])
{
  int order;
  char *infile, *outfile, *args, *title;
  IBAND inband, outband, tempband;
  IMAGE inimage, outimage;
  int xsize, ysize;
  long start_time, elapsed_time;
  int reconstruct;
  int status;

  Iset_message(TRUE);
  Iset_abort(TRUE);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-n <n>] [-r] [-t <title>] <inimage> <outimage>\n"));

  if (argc == 1) Usage(1, NULL);

  args = argvOptions(argc, argv); /* Save command-line arguments. */

  order       = read_iswitch(&argc, argv, "-n", 0);
  reconstruct = read_bswitch(&argc, argv, "-r");
  title       = read_switch(&argc, argv, "-t", 1,
			    reconstruct ? "zernike reconstructed" : "zernike");

  if (argc < 3) Usage(2, "Illegal number of arguments.\n");

  infile  = argv[argc-2];
  outfile = argv[argc-1];

  /* Start the clock */

  start_time = clock();

  /* Read input image from file */

  inimage = Iread_image(infile);
  inband  = (IBAND)inimage[1]; /* First band only is used. */

  xsize = Ixsize(inband);
  ysize = Iysize(inband);

  /* Make data structure for output image */

  outimage = Icopy_init(inimage);
  Iset_title(outimage, title);
  outband  = (IBAND) outimage[1];

  if (reconstruct) {
    zer_rec (order, inband, outband);
    /* histoEq(tempband, outband, 256);*/
  } else {
    tempband = Imake_band(Iu_byte_typ, xsize, ysize);
    zer_con (order, inband, tempband);
    histoEq(tempband, outband, 256);
    Idel_band (&tempband);
  }

  elapsed_time = clock() - start_time;
  Info(0, "Time used (CPU) : %fs\n", elapsed_time / 1e6);

  Ihistory(outimage, argv[0], args);
  status = Iwrite_image(outimage, outfile);

  return 0; /* Unix commands should return 0 */

} /* main() */

#endif /* MAIN */
