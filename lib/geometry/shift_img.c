

/*

________________________________________________________________

        shift_img.c
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



#include <stdlib.h>
#include <xite/includes.h>
#include <xite/blab.h>
#include <xite/biff.h>
#include <xite/geometry.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include XITE_MALLOC_H
#include XITE_STDIO_H

static int gcd(int a, int b)
{
  int r;

  if (a == 0) return(b);
  if (b == 0) return(a);

  while (b != 0) {
    r = a % b;
    a = b;
    b = r;
  }

  return(a);
}



#define shift_macro(NAME, TYPE, BAND)			\
                                                        \
static void NAME(BAND I, BAND O, int xshift, int yshift) \
{							\
  int x1, x2, y1, y2, xsize, ysize;			\
  int xgcd, xlen, ygcd, ylen, yp, yn;			\
  TYPE *h, *s, *d;					\
                                                        \
  xsize = Ixsize( (IBAND) I);				\
  ysize = Iysize( (IBAND) I);				\
  xshift %= xsize;					\
  yshift %= ysize;					\
  if (xshift < 0) xshift += xsize;			\
  if (yshift < 0) yshift += ysize;			\
  xgcd = gcd(xshift, xsize);				\
  ygcd = gcd(yshift, ysize);				\
  ylen = ysize/ygcd;					\
  xlen = xsize-xshift;                                  \
  h = (TYPE *) malloc((xsize+1) *			\
      Ipixsize(Ipixtyp((IBAND) I))/8);			\
                                                        \
  for (y1=1; y1<=ygcd; y1 ++)				\
    {							\
      for(x1=1; x1<=xsize; x1++) h[x1] = I[y1][x1];	\
      yp = yn = y1;					\
      for(y2=1; y2<=ylen; y2++)				\
        {						\
	  yn = (( yp - 1 + yshift) % ysize) + 1;	\
	  s = (yn == y1) ? &h[1] : &I[yn][1];		\
          d = &O[yp][1];			      	\
	  x2 = xshift;	       				\
	  for (x1=0; x1<xlen; )     d[x1++] = s[x2++];	\
	  x2 = 0;					\
	  for (x1=xlen; x1<xsize; ) d[x1++] = s[x2++];	\
	  yp = yn;					\
	}						\
    }	     						\
  free(h);						\
}

shift_macro(shift_I1,  UNS_BYTE,  IBAND)
shift_macro(shift_I2,  UNS_SHORT, IUS_BAND)
shift_macro(shift_I4,  INTEGER,   II_BAND)
shift_macro(shift_I8,  DOUBLE,    ID_BAND)
shift_macro(shift_I16, DCOMPLEX,  IDC_BAND)






/*F:shift_band*

________________________________________________________________

		shift_band
________________________________________________________________

Name:		shift_band - Set new origo on a band.

Syntax:         | #include <xite/geometry.h>
		|
                | int shift_band( IBAND in_band, IBAND out_band,
                |    int shiftx, int shifty );

Description:    shift an image band. Works on all pixel types.
                'in_band' and 'out_band' may be the same band.

Restrictions:   'in_band' and 'outband' must be of same type and size

Return value:   | 0 => ok
                | 2 => Bands have different pixel type
		| 3 => Bands have different size

See also:       shift_img(1), transpose_band(3), affine(3), quadratic(3),
                rotate(3)

Author:		Otto Milvang

Id: 		$Id$ 
________________________________________________________________

*/

BiffStatus shift_band(IBAND in_band, IBAND out_band, int shiftx, int shifty)
{
  int pixtyp;
  pixtyp = Ipixtyp(in_band);

  if (Ipixtyp(out_band) != pixtyp) 
    return(Error(2, "shift_band: Bands have different pixel types\n"));

  if (Ixsize(in_band) != Ixsize(out_band) || 
     (Ixsize(in_band) != Ixsize(out_band)))
    return(Error(3, "shift_band: Bands have different size\n"));

  switch(pixtyp)
    {
    case Iu_byte_typ:
    case Is_byte_typ: shift_I1(in_band, out_band, shiftx, shifty); break;
    case Iu_short_typ:
    case Is_short_typ: shift_I2((IUS_BAND) in_band, (IUS_BAND) out_band,
				shiftx, shifty); break;
    case Integer_typ:
    case Ireal_typ: shift_I4((II_BAND) in_band, (II_BAND) out_band,
			     shiftx, shifty); break;
    case Icomplex_typ: 
    case Idouble_typ: shift_I8((ID_BAND) in_band, (ID_BAND) out_band,
			       shiftx, shifty); break;
    case Id_complex_typ: shift_I16((IDC_BAND) in_band, (IDC_BAND) out_band,
				   shiftx, shifty); break;
    default: Ierr_action(Ipixtyperr); return(Ipixtyperr); /* break; */
    }
  return(0);
}
