
/*

________________________________________________________________

        absValue
        $Id$
        Copyright 1991, Blab, UiO
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
#include <stdlib.h>
#include <xite/negate.h>
#include <math.h>
#include XITE_STDIO_H
#include <xite/message.h>
#include <xite/readarg.h>
#include XITE_UNISTD_H

#ifndef MIN
# define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif


/*F:fftAmp=absValue*/
/*F:absValue*

________________________________________________________________

		absValue
________________________________________________________________

Name:		absValue, fftAmp - take the absolute value of a band

Syntax:         | #include <xite/negate.h>
		|
                | int absValue( IBAND b1, IBAND b2 );
		|
		| #include <xite/fft.h>
		|
		| int fftAmp( IC_BAND in_band, IR_BAND out_band );
		|
Description:    'absValue' takes the absolute value of b1 into b2. Only largest
		common rectangle is handled.

		Accepts all pixel types. Both bands must have the same pixel
		type, except that if 'b1' is complex, then 'b2' may be either
		complex or real, and if 'b1' is double complex, then 'b2'
		may be either double complex or double.

                Identical in- and out- parameters are legal.

		Absolute value of a complex pixel is
		| sqrt(re * re + im * im)

		'fftAmp' is retained for compatibility with previous XITE
		versions. It is a macro call to 'absValue'.

Return value:   | 0 => ok
                | 1 => Bands have unknown pixel type
                | 2 => Illegal combination of pixel types

Author:		Tor L?nnestad, BLAB, IfI, UiO 
Modified:       Svein B?e

Id: 		$Id$
________________________________________________________________

*/

#define ABS(a,b) (((a) < (b)) ? (-(a)) : (a))

int absValue(IBAND band1, IBAND band2)
{
  int x, y, xsize, ysize;
  IPIXTYP ipt, opt;
  double val;

  ipt = Ipixtyp(band1);
  opt = Ipixtyp(band2);

  if (ipt != opt) {
    if (!((ipt == Icomplex_typ   && opt == Ireal_typ) ||
	  (ipt == Id_complex_typ && opt == Idouble_typ)))
      return(Error(2, "absValue: Illegal combination of pixel types\n"));
  }

  xsize = MIN(Ixsize(band1), Ixsize(band2));
  ysize = MIN(Iysize(band1), Iysize(band2));

  switch ((int) ipt) {
  case Iu_byte_typ: {
    IUB_BAND b1 = (IUB_BAND) band1;
    IUB_BAND b2 = (IUB_BAND) band2;
    for (y=1; y <= ysize; ++y)
      for (x=1; x <= xsize; ++x)
	b2[y][x] = b1[y][x];
    return(0);
  }
    
  case Is_byte_typ: {
    ISB_BAND b1 = (ISB_BAND) band1;
    ISB_BAND b2 = (ISB_BAND) band2;
    for (y=1; y <= ysize; ++y)
      for (x=1; x <= xsize; ++x)
	b2[y][x] = ABS(b1[y][x], 0);
    return(0);
  }
    
  case Iu_short_typ: {
    IUS_BAND b1 = (IUS_BAND) band1;
    IUS_BAND b2 = (IUS_BAND) band2;
    for (y=1; y <= ysize; ++y)
      for (x=1; x <= xsize; ++x)
	b2[y][x] = b1[y][x];
    return(0);
  }
    
  case Is_short_typ: {
    ISS_BAND b1 = (ISS_BAND) band1;
    ISS_BAND b2 = (ISS_BAND) band2;
    for (y=1; y <= ysize; ++y)
      for (x=1; x <= xsize; ++x)
	b2[y][x] = ABS(b1[y][x], 0);
    return(0);
  }
    
  case Integer_typ: {
    II_BAND b1 = (II_BAND) band1;
    II_BAND b2 = (II_BAND) band2;
    for (y=1; y <= ysize; ++y)
      for (x=1; x <= xsize; ++x)
	b2[y][x] = ABS(b1[y][x], 0);
    return(0);
  }
    
  case Ireal_typ: {
    IR_BAND b1 = (IR_BAND) band1;
    IR_BAND b2 = (IR_BAND) band2;
    for (y=1; y <= ysize; ++y)
      for (x=1; x <= xsize; ++x)
	b2[y][x] = ABS(b1[y][x], 0.0);
    return(0);
  }
    
  case Icomplex_typ: {
    IC_BAND b1 = (IC_BAND) band1;

    if (opt == ipt) {
      IC_BAND b2 = (IC_BAND) band2;
      for (y=1; y <= ysize; ++y)
	for (x=1; x <= xsize; ++x) {
	  val = sqrt((double) b1[y][x].re * b1[y][x].re +
		     b1[y][x].im * b1[y][x].im);
	  b2[y][x].re = CLIP_RANGE_CAST(val, -REAL_MAX, REAL_MAX, REAL);
	  b2[y][x].im = 0.0;
	}
    } else {
      IR_BAND b2 = (IR_BAND) band2;
      for (y=1; y <= ysize; ++y)
	for (x=1; x <= xsize; ++x) {
	  val = sqrt((double) b1[y][x].re * b1[y][x].re +
		     b1[y][x].im * b1[y][x].im);
	  b2[y][x] = CLIP_RANGE_CAST(val, -REAL_MAX, REAL_MAX, REAL);
	}
    }
    return(0);
  }

  case Idouble_typ: {
    ID_BAND b1 = (ID_BAND) band1;
    ID_BAND b2 = (ID_BAND) band2;
    for (y=1; y <= ysize; ++y)
      for (x=1; x <= xsize; ++x)
	b2[y][x] = ABS(b1[y][x], 0.0);
    return(0);
  }
    
  case Id_complex_typ: {
    IDC_BAND b1 = (IDC_BAND) band1;

    if (opt == ipt) {
      IDC_BAND b2 = (IDC_BAND) band2;
      for (y=1; y <= ysize; ++y)
	for (x=1; x <= xsize; ++x) {
	  val = sqrt((double) b1[y][x].re * b1[y][x].re +
		     b1[y][x].im * b1[y][x].im);
	  b2[y][x].re = val;
	  b2[y][x].im = 0.0;
	}
    } else {
      ID_BAND b2 = (ID_BAND) band2;
      for (y=1; y <= ysize; ++y)
	for (x=1; x <= xsize; ++x) {
	  val = sqrt((double) b1[y][x].re * b1[y][x].re +
		     b1[y][x].im * b1[y][x].im);
	  b2[y][x] = val;
	}
    }
    return(0);
  }
  } /* switch */

  return(Error(1, "absValue: Bands have unknown pixel type\n"));
}
