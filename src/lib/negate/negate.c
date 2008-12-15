
/*

________________________________________________________________

        negate
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
#include <xite/negate.h>
#include XITE_STDIO_H
#include <xite/blab.h>
#include <xite/message.h>

#ifndef MIN
# define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

/*F:negate*

________________________________________________________________

		negate
________________________________________________________________

Name:		negate - negate a band

Syntax:         | #include <xite/negate.h>
		|
                | int negate( IBAND b1, IBAND b2 );
Description:    Negates b1 into b2. Only largest common rectangle
                is negated. Accepts all pixel types. For pixel
		types including negative values, the result is
		| output = -input
		For pixel types NOT including negative values,
		the result is
		| output = MAX - input
		where MAX is the largest value of the actual pixel
		type. Both bands must have the same pixel type.
                Identical in- and out- parameters are legal.

Return value:   | 0 => ok
                | 1 => Bands have unknown pixel type
                | 2 => Bands have different pixel types

Author:		Tor L|nnestad, BLAB, IfI, UiO 

Id: 		$Id$
________________________________________________________________

*/

int negate(IBAND b1, IBAND b2)
{
  ISB_BAND sb1, sb2;
  IUS_BAND us1, us2;
  ISS_BAND ss1, ss2;
  II_BAND  i1,  i2;
  IR_BAND  r1,  r2;
  IC_BAND  c1,  c2;
  ID_BAND  d1,  d2;
  IDC_BAND dc1, dc2;
  int x, y, xsize, ysize, status = 0;

  if (Ipixtyp(b1) NE Ipixtyp(b2))
    return(Error(2, "negate: Bands have different pixel types\n"));

  xsize = MIN(Ixsize(b1),Ixsize(b2));
  ysize = MIN(Iysize(b1),Iysize(b2));

  switch ((int) Ipixtyp(b1)) {
  case Iu_byte_typ:
    for (y=1; y LE ysize; INC y)
      for (x=1; x LE xsize; INC x)
	b2[y][x] = UNS_BYTE_MAX - b1[y][x];
    break;
    
  case Is_byte_typ:
    sb1=(ISB_BAND)b1;
    sb2=(ISB_BAND)b2;
    for (y=1; y LE ysize; INC y)
      for (x=1; x LE xsize; INC x)
	sb2[y][x] = -sb1[y][x];
    break;
    
  case Iu_short_typ:
    us1=(IUS_BAND)b1;
    us2=(IUS_BAND)b2;
    for (y=1; y LE ysize; INC y)
      for (x=1; x LE xsize; INC x)
	us2[y][x] = UNS_SHORT_MAX - us1[y][x];
    break;
    
  case Is_short_typ:
    ss1=(ISS_BAND)b1;
    ss2=(ISS_BAND)b2;
    for (y=1; y LE ysize; INC y)
      for (x=1; x LE xsize; INC x)
	ss2[y][x] = -ss1[y][x];
    break;
    
  case Integer_typ:
    i1=(II_BAND)b1;
    i2=(II_BAND)b2;
    for (y=1; y LE ysize; INC y)
      for (x=1; x LE xsize; INC x)
	i2[y][x] = -i1[y][x];
    break;
    
  case Ireal_typ:
    r1=(IR_BAND)b1;
    r2=(IR_BAND)b2;
    for (y=1; y LE ysize; INC y)
      for (x=1; x LE xsize; INC x)
	r2[y][x] = -r1[y][x];
    break;
    
  case Icomplex_typ:
    c1=(IC_BAND)b1;
    c2=(IC_BAND)b2;
    for (y=1; y LE ysize; INC y)
      for (x=1; x LE xsize; INC x)
	{ c2[y][x].re = -c1[y][x].re; c2[y][x].im = -c1[y][x].im; }
    break;
    
  case Idouble_typ:
    d1=(ID_BAND)b1;
    d2=(ID_BAND)b2;
    for (y=1; y LE ysize; INC y)
      for (x=1; x LE xsize; INC x)
	d2[y][x] = -d1[y][x];
    break;
    
  case Id_complex_typ:
    dc1=(IDC_BAND)b1;
    dc2=(IDC_BAND)b2;
    for (y=1; y LE ysize; INC y)
      for (x=1; x LE xsize; INC x)
	{ dc2[y][x].re = -dc1[y][x].re; dc2[y][x].im = -dc1[y][x].im; }
    break;
    
  default: status = 1;
    break;
  } /* switch */
  
  if (!status) return(status);
  else return(Error(status, "negate: Bands have unknown pixel type\n"));

} /* negate() */
