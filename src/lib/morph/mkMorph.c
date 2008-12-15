

/*C*

________________________________________________________________

        mkMorph.c
        $Id$
        Copyright 1994, Blab, UiO
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
#include <stdlib.h>
#include <xite/includes.h>
#include <xite/biff.h>
#include <xite/histo.h>
#include <xite/readarg.h>
#include <xite/message.h>
#include XITE_STDIO_H
#include <xite/morph.h>

#ifndef M_PI
# define M_PI    3.14159265358979323846
#endif
#ifndef M_PI_2
# define M_PI_2  1.57079632679489661923
#endif

#define MINVAL SIGN_SHORT_MIN

#ifndef MIN
# define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif
#ifndef MAX
# define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif
#define SQR(a) ((a) * (a))

/*
extern double trunc();
*/

#define trunc(a) ((int) (a))

/*L:ellipse*

________________________________________________________________

		ellipse
________________________________________________________________

Name:		ellipse

Syntax:		| static int ellipse( IBAND band, double b, double a,
                |    double theta );

Description:	Generates an ellipse with half axes a1='a'/2 and b1='b'/2,
                x^2/a1 + y^2/b1=1.
                The ellipse may be rotated by 'theta' radians.

Return value:	| 0 : OK
                | 1 : Bad size for a or b
		| 2 : Illegal pixel type

Author:		Martin Torpe Lie
Revised:        Svein Bøe
________________________________________________________________

*/

static int ellipse(IBAND band, double b, double a, double theta)
{
  int xsize, ysize, midx, midy, x, y;
  double val, A, B, C, D;
  ISS_BAND ss_band = (ISS_BAND) band;

  xsize = Ixsize(band);
  ysize = Iysize(band);
  if ((int)a > xsize || (int)b > ysize)
    return(Error(1, "ellipse(): Bad size for a or b.\n"));

  if (Ipixtyp(band) != Is_short_typ)
    return(Error(2, "ellipse(): Band must have pixel type signed short.\n"));

  midx = xsize/2 + 1;
  midy = ysize/2 + 1;
  a    = a/2.0;
  b    = b/2.0;

  A    = SQR(a*cos(theta)) + SQR(b*sin(theta));
  B    = 2*cos(theta)*sin(theta)*(SQR(a) - SQR(b));
  C    = SQR(a*sin(theta)) + SQR(b*cos(theta));
  D    = SQR(a*b);
  
  for(y = 1;y <= ysize; y++) {
    for(x = 1;x <= xsize; x++) {
      val = A * (double) SQR(x - midx)
	- (B * (double) ((x - midx)*(y - midy)))
	+ C * (double) SQR(y - midy) - D;

      if (val <= 0.0) ss_band[y][x] = 0;
      else ss_band[y][x] = MINVAL;
    }
  }

  return(0);

} /* ellipse() */



/*L:rectangle*

________________________________________________________________

		rectangle
________________________________________________________________

Name:		rectangle

Syntax:         | static int rectangle( IBAND band, double a,
                |    double b, double theta );

Description:	Generates a rectangle with sides 'a' and 'b'.
                The rectangle may be rotated by 'theta' radians.

Return value:	| 0 : OK
                | 1 : Bad size for a or b
		| 2 : Illegal pixel type

Author:		Martin Torpe Lie
Revised:        Svein Bøe
________________________________________________________________

*/

static int rectangle(IBAND band, double a, double b, double theta)
{
  int xsize, ysize, midx, midy, x, y;
  double thp, p, xm, ym;
  ISS_BAND ss_band = (ISS_BAND) band;
  
  xsize = Ixsize(band);
  ysize = Iysize(band);
  if (((int)a > xsize) || ((int)b > ysize))
    return(Error(1, "rectangle(): Bad size for a or b.\n"));

  if (Ipixtyp(band) != Is_short_typ)
    return(Error(2, "rectangle(): Band must have pixel type signed short.\n"));

  midx = xsize/2 + 1;
  midy = ysize/2 + 1;
  a    = a/2.0 + 0.001;
  b    = b/2.0 + 0.001;
  
  for(y = 1;y <= ysize; y++) {
    for(x = 1;x <= xsize; x++) {
      xm  = (double) (x - midx);
      ym  = (double) (y - midy);
      p   = sqrt(SQR(xm) + SQR(ym));
      thp = (x == midx) ? (ym > 0.0) ? M_PI_2 : -M_PI_2 : atan(ym/xm);

      if ((ym < 0.0) && (xm < 0.0))
	thp -= M_PI;
      if ((ym > 0.0) && (xm < 0.0))
	thp += M_PI;
      if ((ym == 0.0) && (xm < 0.0))
	thp = -M_PI;

      xm = p*cos(thp + theta);
      if (xm < 0.0) xm = trunc(xm*100000.0 - 0.5)/100000.0;
      else xm = trunc(xm*100000.0 + 0.5)/100000.0;

      ym = p*sin(thp + theta);
      if (ym < 0.0) ym = trunc(ym*100000.0 - 0.5)/100000.0;
      else ym = trunc(ym*100000.0 + 0.5)/100000.0;
      
      if ((xm <= a) && (xm >= - a) && (ym <= b) && (ym >= - b))
	ss_band[y][x]= 0;
      else
	ss_band[y][x]= MINVAL;
    }
  }

  return(0);

} /* rectangle() */



/*F:mkMorphForm*

________________________________________________________________

		mkMorphForm
________________________________________________________________

Name:		mkMorphForm - make a flat morphological structuring element

Syntax:		| #include <xite/morph.h>
                |
		| IBAND mkMorphForm( morph_height_type type,
		|    double xsize, double ysize, double theta );

Description:    Generates and returns a band with an object as defined
                by 'type', 'xsize' and 'ysize'. The resulting band is
		a square, with horizontal and vertical sizes equal to the
		length of the rectangle''s diagonal,
		| sqrt(2) * MAX(xsize, ysize),
		rounded up to the nearest odd integer.

		The object pixels are set equal to zero, the background
		pixels are set equal to the minimum for a signed short pixel,
		typically -32768.

		Use one of the constants SQUARE, ELLIPSE, LINE, CIRCLE,
		RECTANGLE for 'type'. These constants are of type
		'morph_height_type'.

		For 'type' equal to LINE, 'ysize' is not used. It is
		hardcoded to a width of 1.

Return value:   Band pointer.

See also:       mkMorphHeight(3), MorphScale(3), mkMorph(1)

Author:		Martin Torpe Lie
Revised:        Svein Bøe
Doc:            Martin Torpe Lie and Svein Bøe
________________________________________________________________

*/

IBAND mkMorphForm(morph_height_type type, double xsize, double ysize, double theta)
{
  int dia;
  IBAND band;
  
  dia  = (int) (sqrt(2.0) * MAX(xsize, ysize));
  dia += ((dia % 2) ? 0 : 1);
  band = (IBAND) Imake_band(Is_short_typ, dia, dia);
  
  switch (type)
    {
    case CIRCLE:
      ellipse(band, xsize, xsize, theta);
      break;
      
    case ELLIPSE:
      if (xsize == ysize) ysize = ysize/5.0*3.0;
      ellipse(band, xsize, ysize, theta);
      break;
      
    case LINE:
      rectangle(band, xsize, 1.0, theta);
      break;
      
    case SQUARE:
      rectangle(band, xsize, xsize, theta);
      break;
      
    case RECTANGLE:
      if (xsize == ysize) ysize = ysize/5.0*3.0;
      rectangle(band, xsize, ysize, theta);
      break;
    }

  return(band);

} /* mkMorphForm() */

/*L:zellipse*

________________________________________________________________

		zellipse
________________________________________________________________

Name:		zellipse

Syntax:		| static int zellipse( IBAND band, double a, double b,
                |    int c, double theta );

Description:	Generates an elliptic height profile with half axes
                a1='a'/2, b1='b'/2 and c1='c'/2 with the formula
                x^2/a1 + y^2/b1 + z^2/c1=1. The ellipse may be rotated by
		'theta' radians.

Return value:	| 0 : OK
                | 1 : Bad size for a or b
		| 2 : Illegal pixel type

Author:		Martin Torpe Lie
Revised:        Svein Bøe
________________________________________________________________

*/

static int zellipse(IBAND band, double a, double b, int c, double theta)
{
  int xsize, ysize, midx, midy, x, y;
  double val, p, thp, xm, ym;
  ISS_BAND ss_band = (ISS_BAND) band;
  
  xsize = Ixsize(band);
  ysize = Iysize(band);
  if ((int)a > xsize || (int)b > ysize)
    return(Error(1, "zellipse(): Bad size for a or b.\n"));

  if (Ipixtyp(band) != Is_short_typ)
    return(Error(2, "zellipse(): Band must have pixel type signed short.\n"));
  
  midx = xsize/2 + 1;
  midy = ysize/2 + 1;
  a    = a/2.0;
  b    = b/2.0;
  
  for(y = 1;y <= ysize; y++) {
    for(x = 1;x <= xsize; x++) {
      if (ss_band[y][x] == 0) {
	xm  = (double) (x - midx);
	ym  = (double) (y - midy);
	p   = sqrt(SQR(xm) + SQR(ym));
	thp = (x == midx) ? (ym > 0.0) ? M_PI_2 : -M_PI_2 : atan(ym/xm);

	if ((ym < 0.0) && (xm < 0.0))
	  thp -= M_PI;
	if ((ym > 0.0) && (xm < 0.0))
	  thp += M_PI;
	if ((ym == 0.0) && (xm < 0.0))
	  thp = -M_PI;

	xm = p*cos(thp + theta);
	if (xm < 0.0) xm = trunc(xm*100000.0 - 0.5)/100000.0;
	else xm = trunc(xm*100000.0 + 0.5)/100000.0;

	ym = p*sin(thp + theta);
	if (ym < 0.0) ym = trunc(ym*100000.0 - 0.5)/100000.0;
	else ym = trunc(ym*100000.0 + 0.5)/100000.0;

	val = (double)c*c*(1.0 - (xm*xm)/(a*a) - (ym*ym)/(b*b));
	
	if (val >= 0.0)
	  ss_band[y][x] = (int)sqrt(val);
      }
    }
  }

  return(0);

} /* zellipse() */



/*L:zlinear*

________________________________________________________________

		zlinear
________________________________________________________________

Name:		zlinear
Syntax:		| static int zlinear( IBAND band, double a, double b,
                |    int c, double theta );

Description:	Generates a linear height profile with 'a' and 'b'.
                The height profile may be rotated by 'theta' radians.

Return value:	| 0 : OK
                | 1 : Bad size for a or b
		| 2 : Illegal pixel type

Author:		Martin Torpe Lie
Revised:        Svein Bøe
________________________________________________________________

*/


static int zlinear(IBAND band, double a, double b, int c, double theta)
{
  int xsize, ysize, midx, midy, x, y;
  double thp, p, xm, ym, f1, f2, tmp;
  ISS_BAND ss_band = (ISS_BAND) band;
  
  xsize = Ixsize(band);
  ysize = Iysize(band);
  if ((int)a > xsize || (int)b > ysize)
    return(Error(1, "zlinear(): Bad size for a or b.\n"));

  if (Ipixtyp(band) != Is_short_typ)
    return(Error(2, "zlinear(): Band must have pixel type signed short.\n"));
  
  midx = xsize/2 + 1;
  midy = ysize/2 + 1;
  a    = a / 2.0;
  b    = b / 2.0;
  
  for(y = 1;y <= ysize; y++) {
    for(x = 1;x <= xsize; x++) {
      if (ss_band[y][x] == 0) {
	xm  = (double) (x - midx);
	ym  = (double) (y - midy);
	p   = sqrt(SQR(xm) + SQR(ym));
	thp = (x == midx) ? (ym > 0.0) ? M_PI_2 : -M_PI_2 : atan(ym/xm);

	if ((ym < 0.0) && (xm < 0.0))
	  thp -= M_PI;
	if ((ym > 0.0) && (xm < 0.0))
	  thp += M_PI;
	if ((ym == 0.0) && (xm < 0.0))
	  thp = -M_PI;

	xm = p*cos(thp + theta);
	if (xm < 0.0) xm = trunc(xm*100000.0 - 0.5)/100000.0;
	else xm = trunc(xm*100000.0 + 0.5)/100000.0;

	ym = p*sin(thp + theta);
	if (ym < 0.0) ym = trunc(ym*100000.0 - 0.5)/100000.0;
	else ym = trunc(ym*100000.0 + 0.5)/100000.0;
	
	f1 = b*xm - a*ym;
	f2 = b*xm + a*ym;
	
	if (f1 < 0.0) {
	  if (f2 > 0.0)
	    ss_band[y][x]= (int)(((tmp = c*(1 - ym/b)) >= 0.0) ? tmp : 0.0);
	  else
	    ss_band[y][x]= (int)(((tmp = c*(1 + xm/a)) >= 0.0) ? tmp : 0.0);
	} else {
	  if (f2 > 0.0)
	    ss_band[y][x]= (int)(((tmp = c*(1 - xm/a)) >= 0.0) ? tmp : 0.0);
	  else
	    ss_band[y][x]= (int)(((tmp = c*(1 + ym/b)) >= 0.0) ? tmp : 0.0);
	}
      }
    }
  }

  return(0);

} /* zlinear() */



/*L:zparabola*

________________________________________________________________

		zparabola
________________________________________________________________

Name:		zparabola
Syntax:		| static int zparabola( IBAND band, double a,
                |    double b, int c, double theta );

Description:	Generates a parabolic height profile with 'a' and 'b'.
                The parabola may be rotated by 'theta' radians.

Return value:	| 0 : OK
                | 1 : Bad size for a or b
		| 2 : Illegal pixel type

Author:		Martin Torpe Lie
Revised:        Svein Bøe
________________________________________________________________

*/

static int zparabola(IBAND band, double a, double b, int c, double theta)
{
  int xsize, ysize, midx, midy, x, y;
  double thp, p, xm, ym, f1, a2, b2;
  ISS_BAND ss_band = (ISS_BAND) band;
  
  xsize = Ixsize(band);
  ysize = Iysize(band);
  if ((int)a > xsize || (int)b > ysize)
    return(Error(1, "zparabola(): Bad size for a or b.\n"));

  if (Ipixtyp(band) != Is_short_typ)
    return(Error(2, "zparabola(): Band must have pixel type signed short.\n"));
  
  midx = xsize/2 + 1;
  midy = ysize/2 + 1;
  a    = a/2.0;
  b    = b/2.0;
  a2   = (double)c/(a*a);
  b2   = (double)c/(b*b);
  
  for(y = 1;y <= ysize; y++) {
    for(x = 1;x <= xsize; x++) {
      if (ss_band[y][x] == 0) {
	xm  = (double) (x - midx);
	ym  = (double) (y - midy);
	p   = sqrt(SQR(xm) + SQR(ym));
	thp = (x == midx) ? (ym > 0.0) ? M_PI_2 : -M_PI_2 : atan(ym/xm);

	if ((ym < 0.0) && (xm < 0.0))
	  thp -= M_PI;
	if ((ym > 0.0) && (xm < 0.0))
	  thp += M_PI;
	if ((ym == 0.0) && (xm < 0.0))
	  thp = -M_PI;

	xm = p*cos(thp + theta);
	if (xm < 0.0) xm = trunc(xm*100000.0 - 0.5)/100000.0;
	else xm = trunc(xm*100000.0 + 0.5)/100000.0;

	ym = p*sin(thp + theta);
	if (ym < 0.0) ym = trunc(ym*100000.0 - 0.5)/100000.0;
	else ym = trunc(ym*100000.0 + 0.5)/100000.0;
	
	f1 = -a2*xm*xm - b2*ym*ym + c;
	
	if (f1 > 0)
	  ss_band[y][x] = (int)f1;
      }
    }
  }

  return(0);

} /* zparabola() */



/*L:zcone*

________________________________________________________________

		zcone
________________________________________________________________

Name:		zcone
Syntax:		| static int zcone( IBAND band, double a, double b,
                |    int c, double theta );

Description:	Generates an elliptic conic height profile with 'a' and 'b'.
                The cone may be rotated by 'theta' radians.

Return value:	| 0 : OK
                | 1 : Bad size for a or b
		| 2 : Illegal pixel type

Author:		Martin Torpe Lie
________________________________________________________________

*/


static int zcone(IBAND band, double a, double b, int c, double theta)
{
  int xsize, ysize, midx, midy, x, y;
  double thp, p, xm, ym, f1, a2, b2;
  ISS_BAND ss_band = (ISS_BAND) band;
  
  xsize = Ixsize(band);
  ysize = Iysize(band);
  if ((int)a > xsize || (int)b > ysize)
    return(Error(1, "zcone(): Bad size for a or b.\n"));

  if (Ipixtyp(band) != Is_short_typ)
    return(Error(2, "zcone(): Band must have pixel type signed short.\n"));
  
  midx = xsize/2 + 1;
  midy = ysize/2 + 1;
  a    = a/2.0;
  b    = b/2.0;
  a2   = (double)c/(a*a);
  b2   = (double)c/(b*b);
  
  for(y = 1;y <= ysize; y++) {
    for(x = 1;x <= xsize; x++) {
      if (ss_band[y][x] == 0) {
	xm  = (double) (x - midx);
	ym  = (double) (y - midy);
	p   = sqrt(SQR(xm) + SQR(ym));
	thp = (x == midx) ? (ym > 0.0) ? M_PI_2 : -M_PI_2 : atan(ym/xm);

	if ((ym < 0.0) && (xm < 0.0))
	  thp -= M_PI;
	if ((ym > 0.0) && (xm < 0.0))
	  thp += M_PI;
	if ((ym == 0.0) && (xm < 0.0))
	  thp = -M_PI;

	xm = p*cos(thp + theta);
	if (xm < 0.0) xm = trunc(xm*100000.0 - 0.5)/100000.0;
	else xm = trunc(xm*100000.0 + 0.5)/100000.0;

	ym = p*sin(thp + theta);
	if (ym < 0.0) ym = trunc(ym*100000.0 - 0.5)/100000.0;
	else ym = trunc(ym*100000.0 + 0.5)/100000.0;
	
	f1 = (double)c*(1 - sqrt((xm*xm)/(a*a) + (ym*ym)/(b*b)));
	
	if (f1 > 0.0)
	  ss_band[y][x] = (int)f1;
      }
    }
  }

  return(0);

} /* zcone() */



/*F:mkMorphHeight*

________________________________________________________________

		mkMorphHeight
________________________________________________________________

Name:		mkMorphHeight - make a height on a morphological
                structuring element

Syntax:		| #include <xite/morph.h>
                |
		| int mkMorphHeight( IBAND band,
		|    int type, double xsize, double ysize,
		|    int zsize, double theta );

Description:    Modifies the height of a structuring element in 'band', using
                a shape as defined by 'type'. Only the zero-valued pixels
		in 'band' are modified, they are assumed to be inside the
		structuring element.

		Use one of the constants ELLIPSE, CIRCLE, PARABOL, CONE,
		LINEAR for 'type'. These constants are of type
		'morph_height_type'.

Restrictions:   'band' must be of pixel type signed short.

Return value:   | 0 : Ok
                | 1 : Illegal pixel type

See also:       mkMorphForm(3), MorphScale(3), mkMorph(1)

Author:		Martin Torpe Lie
Revised:        Svein Bøe
Doc:            Martin Torpe Lie and Svein Bøe
________________________________________________________________

*/

int mkMorphHeight(IBAND band, int type, double xsize, double ysize, int zsize, double theta)
{
  int status = 0;

  if (Ipixtyp(band) != Is_short_typ)
    return(Error(1, "%s%s\n",
		 "mkMorphHeight(): Band must have pixel type ",
		 "signed short."));

  switch (type)
    {
    case CIRCLE:
      xsize = (int)MIN((double)xsize, (double)ysize);
      status = zellipse(band, xsize, xsize, zsize, theta);
      break;
      
    case ELLIPSE:
      if (xsize == ysize) ysize = ysize/5.0*3.0;
      status = zellipse(band, xsize, ysize, zsize, theta);
      break;
      
    case PARABOL:
      status = zparabola(band, xsize, ysize, zsize, theta);
      break;
      
    case CONE:
      status = zcone(band, xsize, ysize, zsize, theta);
      break;
      
    case LINEAR:
      status = zlinear(band, xsize, ysize, zsize, theta);
      break;
      
    case RECTANGLE:
    case SQUARE:
      break;
    }
  
  return(status);

} /* mkMorphHeight() */

/*F:MorphScale*

________________________________________________________________

		MorphScale
________________________________________________________________

Name:		MorphScale - raise or lower morphological structuring element
                to get maximum or minimum equal to zero

Syntax:		| #include <xite/morph.h>
                |
		| void MorphScale( IBAND band, int neg );

Description:    On input, it is assumed that the structuring element in 'band'
                consists of all the non-negative pixels.

		With 'neg' equal to zero, subtract a constant value from the
		structuring element pixels so that the minimum value inside
		the structuring element becomes equal to zero. This means that
		the height profile will rise from zero.

		With nonzero 'neg', add a constant value to the structuring
		element pixels in 'band' so that the maximum value inside the
		structuring becomes zero. This means that the top of the height
		profile touches zero from below. This is to prevent too large
		or small values when eroding or dilating an image.

Return value:   | 0 : Ok
		| 1 : Illegal pixel type

See also:       mkMorphHeight(3), MorphScale(3), mkMorph(1)

Author:		Martin Torpe Lie
Revised:        Svein Bøe
Doc:            Svein Bøe
________________________________________________________________

*/


int MorphScale(IBAND band, int neg)
{
  int x, y, xsi, ysi, min, max, found_str_element = 0;
  ISS_BAND ss_band = (ISS_BAND) band;

  if (Ipixtyp(band) != Is_short_typ)
    return(Error(1, "%s%s\n", 
		 "MorphScale(): Band must have pixel type ",
		 "signed short."));
  
  xsi = Ixsize(band);
  ysi = Iysize(band);
  min = SIGN_SHORT_MAX;
  max = SIGN_SHORT_MIN;

  /* Find the extreme values inside the object. */
  for (x=1; x <= xsi; x++)
    for (y=1; y <= ysi; y++)
      if (ss_band[y][x] >= 0) {
	/* Inside the object */
	found_str_element = 1;

	/* max and min can never be negative when found_str_element is TRUE,
	   because the structuring element pixels are non-negative. */
        min = (int) MIN(min, ss_band[y][x]);
        max = (int) MAX(max, ss_band[y][x]);
      }
  
  if (neg && found_str_element && max > 0) {
    if (Verbose())
      fprintf(stderr, "Reducing all pixel values by %d.\n", max);

    for (y = 1; y <= ysi; y++) {
      for (x = 1; x <= xsi; x++) {
	if (ss_band[y][x] >= 0) {
	  ss_band[y][x] -= max;
	  if (Verbose())
	    fprintf(stderr, "%5d", ss_band[y][x]);
	} else
	  if (Verbose())
	    fprintf(stderr, "    -");
      }
      if (Verbose())
	fprintf(stderr, "\n");
    }
  } else if (!neg && found_str_element && min > 0) {
    if (Verbose())
      fprintf(stderr, "Reducing all pixel values by %d.\n", min);

    for (y = 1; y <= ysi; y++) {
      for (x = 1; x <= xsi; x++) {
	if (ss_band[y][x] >= 0) {
	  ss_band[y][x] -= min;
	  if (Verbose())
	    fprintf(stderr, "%5d", ss_band[y][x]);
	} else
	  if (Verbose())
	    fprintf(stderr, "    -");
      }
      if (Verbose())
	fprintf(stderr, "\n");
    }
  }

  return(0);

} /* mkMorphScale() */
