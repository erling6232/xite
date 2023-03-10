
/*

________________________________________________________________

        transpose_band
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
#include <xite/geometry.h>
#include XITE_STDIO_H
#include <xite/blab.h>
#include <xite/message.h>
#include <xite/readarg.h>



#define transpose_macro(NAME, TYPE, BAND) \
 \
static void NAME(BAND I) \
{\
  int x1, x2, y1, y2, xsize, ysize, xstop, ystop;\
  TYPE h;\
  xsize = Ixsize( (IBAND) I);\
  ysize = Ixsize( (IBAND) I);\
  for (y1=1; y1<=ysize; y1 += 32)\
    {\
      ystop = y1+31 > ysize ? ysize : y1+31;\
      xstop = y1+31 > xsize ? xsize : y1+31;\
      for (y2=y1; y2<=ystop; y2++)\
        for (x2=y2+1; x2<=xstop; x2++)\
	  {\
	    h = I[y2][x2];\
	    I[y2][x2] = I[x2][y2];\
	    I[x2][y2] = h;\
	  }\
      for (x1=y1+32; x1<=xsize; x1 += 32)\
	{\
	  ystop = y1+31 > ysize ? ysize : y1+31;\
	  xstop = x1+31 > xsize ? xsize : x1+31;\
	  for (y2=y1; y2<=ystop; y2++)\
	    for (x2=x1; x2<=xstop; x2++)\
	      {\
		h = I[y2][x2];\
		I[y2][x2] = I[x2][y2];\
		I[x2][y2] = h;\
	      }\
	}\
    }\
}

transpose_macro(transpose_I1,   UNS_BYTE,  IBAND)
transpose_macro(transpose_I2,   UNS_SHORT, IUS_BAND)
transpose_macro(transpose_I4,   INTEGER,   II_BAND)
transpose_macro(transpose_I8,   DOUBLE,    ID_BAND)
transpose_macro(transpose_I8c,  COMPLEX,   IC_BAND)
transpose_macro(transpose_I16c, DCOMPLEX,  IDC_BAND)




/*F:transpose_band*

________________________________________________________________

		transpose_band
________________________________________________________________

Name:		transpose_band - transpose a BIFF image band

Syntax:         | #include <xite/geometry.h>
		|
                | int transpose_band( IBAND I );

Description:    Transpose an image band. Works on all pixel types.

Restrictions:   xsize == ysize

Return value:   | 0 => ok
                | 2 => Bad pixel type
		| 3 => Bad size, xsize <> ysize

See also:       transpose_img(1), shift_band(3), affine(3), quadratic(3),
                rotate(3)

Author:		Otto Milvang
Id: 		$Id$
________________________________________________________________

*/


BiffStatus transpose_band(IBAND I)
{
  int pixtyp;
  pixtyp = Ipixtyp(I);
  if (Ixsize(I) != Iysize(I))
    return(Error(3, "transpose_band: Bad size, xsize <> ysize\n"));

  switch(pixtyp)
    {
    case Iu_byte_typ:
    case Is_byte_typ: transpose_I1(I); break;
    case Iu_short_typ:
    case Is_short_typ: transpose_I2((IUS_BAND) I); break;
    case Integer_typ:
    case Ireal_typ: transpose_I4((II_BAND) I); break;
    case Icomplex_typ: transpose_I8c((IC_BAND) I); break;
    case Idouble_typ: transpose_I8((ID_BAND) I); break;
    case Id_complex_typ: transpose_I16c((IDC_BAND) I); break;
    default: return(Error(2, "transpose_band: Bad pixel type\n")); /* break; */
    }

  return(0);
}
