/*

________________________________________________________________

        biffConvert
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
#include <xite/convert.h>
#include XITE_STDIO_H
#include XITE_TYPES_H
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/message.h>
#include XITE_LIMITS_H

#ifndef MIN
# define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif
#define FORALLYX for (y=1; y <= ysize; y++) {	\
        for (x=1; x <= xsize; x++) {
#define ENDFORALLYX } }



#ifndef MAIN

/*F:mkConvertBand=convertBand*/
/*F:convertBand*

________________________________________________________________

		convertBand
________________________________________________________________

Name:		convertBand, mkConvertBand - convert a band to another
                pixel type

Syntax:         | #include <xite/convert.h>
                |
                | int convertBand( IBAND b1, IBAND b2 );
		|
                | IBAND mkConvertBand( IBAND b, IPIXTYP pixtyp );
Description:    'convertBand' converts the data in <b1> into <b2>.
                Although typed as IBAND above, <b1> and <b2> may
		be of any combination of the following pixel types.

                |  Pixel Type   Band Type   IPIXTYP     IPIXTYP value
                | ----------------------------------------------------
		|  UNS_BYTE     IUB_BAND    Iu_byte_typ      3
		|  SIGN_BYTE    ISB_BAND    Is_byte_typ      4
		|  UNS_SHORT    IUS_BAND    Iu_short_typ     5
		|  SIGN_SHORT   ISS_BAND    Is_short_typ     6
		|  INTEGER      II_BAND     Integer_typ      7
		|  REAL         IR_BAND     Ireal_typ        8
		|  COMPLEX      IC_BAND     Icomplex_typ     9
		|  DOUBLE       ID_BAND     Idouble_typ     10
		|  DCOMPLEX     IDC_BAND    Id_complex_typ  11

		Only the overlapping part of the two bands is converted.
		Conversion is as simple as possible, no scaling is performed.
		If overflow, the result is set to the closest legal value.

		'mkConvertBand' creates a new band of the same size as
		the input band <b>, but with pixel type <pixtyp>,
                then converts the data in <b> into the new band,
                and returns the new band.
Bugs:
		There is a large number of combinations, the routines
		are NOT thoroughly tested!! Please report any problems.

Return value:   For convertBand:
                | 0 => ok
                | 1 => Can't handle that input pixel type
                | 2 => Can't handle that output pixel type

                For mkConvertBand:
                | The new band => ok
                | (IBAND) 0    => error

Author:		Tor Loennestad, BLAB, ifi, UiO
________________________________________________________________

*/

#ifndef FUNCPROTO
static int convFromUnsByte(b1, b2, ysize, xsize, outpixtyp)
IUB_BAND b1, b2;
long ysize, xsize;
IPIXTYP outpixtyp;
#else /* FUNCPROTO */
static int convFromUnsByte(IUB_BAND b1, IUB_BAND b2, long int ysize, long int xsize, IPIXTYP outpixtyp)
#endif /* FUNCPROTO */
{
  int x, y;

  switch ((int) outpixtyp) {
  case Iu_byte_typ:
    FORALLYX b2[y][x] = b1[y][x]; ENDFORALLYX;
    break;

  case Is_byte_typ: {
    ISB_BAND isb2 = (ISB_BAND)b2;

    FORALLYX
      isb2[y][x] =
	CLIP_MAX_CAST(b1[y][x], SIGN_BYTE_MAX, SIGN_BYTE);
    ENDFORALLYX;
  }
    break;

  case Iu_short_typ: {
    IUS_BAND ius2 = (IUS_BAND)b2;

    FORALLYX ius2[y][x] = (UNS_SHORT)b1[y][x]; ENDFORALLYX;
  }
    break;

  case Is_short_typ: {
    ISS_BAND iss2 = (ISS_BAND)b2;

    FORALLYX iss2[y][x] = (SIGN_SHORT)b1[y][x]; ENDFORALLYX;
  }
    break;

  case Integer_typ: {
    II_BAND ii2 = (II_BAND)b2;

    FORALLYX ii2[y][x] = (INTEGER)b1[y][x]; ENDFORALLYX;
  }
    break;

  case Ireal_typ: {
    IR_BAND ir2 = (IR_BAND)b2;

    FORALLYX ir2[y][x] = (REAL)b1[y][x]; ENDFORALLYX;
  }
    break;

  case Icomplex_typ: {
    IC_BAND ic2 = (IC_BAND)b2;

    FORALLYX
      ic2[y][x].re = (REAL)b1[y][x];
      ic2[y][x].im = 0.0;
    ENDFORALLYX;
  }
    break;

  case Idouble_typ: {
    ID_BAND id2 = (ID_BAND)b2;

    FORALLYX id2[y][x] = (DOUBLE)b1[y][x]; ENDFORALLYX;
  }
    break;

  case Id_complex_typ: {
    IDC_BAND idc2 = (IDC_BAND)b2;

    FORALLYX
      idc2[y][x].re = (DOUBLE)b1[y][x];
      idc2[y][x].im = 0.0;
    ENDFORALLYX;
  }
    break;

  default:
    return(2);
    /* break; */
  } /* outpixtyp */
  return(0);
} /* end of convFromUnsByte */

/*
__________________________________________________________________
*/

#ifndef FUNCPROTO
static int convFromSignByte(b1, b2, ysize, xsize, outpixtyp)
ISB_BAND b1, b2;
long ysize, xsize;
IPIXTYP outpixtyp;
#else /* FUNCPROTO */
static int convFromSignByte(ISB_BAND b1, ISB_BAND b2, long int ysize, long int xsize, IPIXTYP outpixtyp)
#endif /* FUNCPROTO */
{
  int x, y;

  switch ((int) outpixtyp) {
  case Iu_byte_typ: {
    IUB_BAND iub2 = (IUB_BAND)b2;

    FORALLYX
      iub2[y][x] = CLIP_MIN_CAST(b1[y][x], 0, UNS_BYTE);
    ENDFORALLYX;
  }
    break;
    
  case Is_byte_typ: {
    ISB_BAND isb2 = (ISB_BAND)b2;

    FORALLYX isb2[y][x] = b1[y][x]; ENDFORALLYX;
  }
    break;
    
  case Iu_short_typ: {
    IUS_BAND ius2 = (IUS_BAND)b2;

    FORALLYX
      ius2[y][x] = CLIP_MIN_CAST(b1[y][x], 0, UNS_SHORT);
    ENDFORALLYX;
  }
    break;
    
  case Is_short_typ: {
    ISS_BAND iss2 = (ISS_BAND)b2;

    FORALLYX iss2[y][x] = (SIGN_SHORT)b1[y][x]; ENDFORALLYX;
  }
    break;
    
  case Integer_typ: {
    II_BAND ii2 = (II_BAND)b2;

    FORALLYX ii2[y][x] = (INTEGER)b1[y][x]; ENDFORALLYX;
  }
    break;
    
  case Ireal_typ: {
    IR_BAND ir2 = (IR_BAND)b2;

    FORALLYX ir2[y][x] = (REAL)b1[y][x]; ENDFORALLYX;
  }
    break;
    
  case Icomplex_typ: {
    IC_BAND ic2 = (IC_BAND)b2;

    FORALLYX
      ic2[y][x].re = (REAL)b1[y][x];
      ic2[y][x].im = 0.0;
    ENDFORALLYX;
  }
    break;
    
  case Idouble_typ: {
    ID_BAND id2 = (ID_BAND)b2;

    FORALLYX id2[y][x] = (DOUBLE)b1[y][x]; ENDFORALLYX;
  }
    break;
    
  case Id_complex_typ: {
    IDC_BAND idc2 = (IDC_BAND)b2;

    FORALLYX
      idc2[y][x].re = (DOUBLE)b1[y][x];
      idc2[y][x].im = 0.0;
    ENDFORALLYX;
  }
    break;
    
  default:
    return(2);
    /* break; */
  } /* outpixtyp */
  return(0);
}  /* end of convFromSignByte */

/*
__________________________________________________________________
*/

#ifndef FUNCPROTO
static int convFromUnsShort(b1, b2, ysize, xsize, outpixtyp)
IUS_BAND b1, b2;
long ysize, xsize;
IPIXTYP outpixtyp;
#else /* FUNCPROTO */
static int convFromUnsShort(IUS_BAND b1, IUS_BAND b2, long int ysize, long int xsize, IPIXTYP outpixtyp)
#endif /* FUNCPROTO */
{
  int x, y;

  switch ((int) outpixtyp) {
  case Iu_byte_typ: {
    IUB_BAND iub2 = (IUB_BAND)b2;

    FORALLYX
      iub2[y][x] = CLIP_MAX_CAST(b1[y][x], UNS_BYTE_MAX, UNS_BYTE);
    ENDFORALLYX;
  }
    break;
    
  case Is_byte_typ: {
    ISB_BAND isb2 = (ISB_BAND)b2;

    FORALLYX
      isb2[y][x] = CLIP_MAX_CAST(b1[y][x], SIGN_BYTE_MAX, SIGN_BYTE);
    ENDFORALLYX;
  }
    break;
    
  case Iu_short_typ: {
    IUS_BAND ius2 = (IUS_BAND)b2;

    FORALLYX ius2[y][x] = b1[y][x]; ENDFORALLYX;
  }
    break;
    
  case Is_short_typ: {
    ISS_BAND iss2 = (ISS_BAND)b2;

    FORALLYX
      iss2[y][x] = CLIP_MAX_CAST(b1[y][x], SIGN_SHORT_MAX, SIGN_SHORT);
    ENDFORALLYX;
  }
    break;
    
  case Integer_typ: {
    II_BAND ii2 = (II_BAND)b2;

    FORALLYX ii2[y][x] = (INTEGER)b1[y][x]; ENDFORALLYX;
  }
    break;
    
  case Ireal_typ: {
    IR_BAND ir2 = (IR_BAND)b2;

    FORALLYX ir2[y][x] = (REAL)b1[y][x]; ENDFORALLYX;
  }
    break;
    
  case Icomplex_typ: {
    IC_BAND ic2 = (IC_BAND)b2;

    FORALLYX
      ic2[y][x].re = (REAL)b1[y][x];
      ic2[y][x].im = 0.0;
    ENDFORALLYX;
  }
    break;
    
  case Idouble_typ: {
    ID_BAND id2 = (ID_BAND)b2;

    FORALLYX id2[y][x] = (DOUBLE)b1[y][x]; ENDFORALLYX;
  }
    break;
    
  case Id_complex_typ: {
    IDC_BAND idc2 = (IDC_BAND)b2;

    FORALLYX
      idc2[y][x].re = (DOUBLE)b1[y][x];
      idc2[y][x].im = 0.0;
    ENDFORALLYX;
  }
    break;
    
  default:
    return(2);
    /* break; */
  } /* outpixtyp */
  return(0);
}  /* end of convFromUnsShort */


/*
__________________________________________________________________
*/

#ifndef FUNCPROTO
static int convFromSignShort(b1, b2, ysize, xsize, outpixtyp)
ISS_BAND b1, b2;
long ysize, xsize;
IPIXTYP outpixtyp;
#else /* FUNCPROTO */
static int convFromSignShort(ISS_BAND b1, ISS_BAND b2, long int ysize, long int xsize, IPIXTYP outpixtyp)
#endif /* FUNCPROTO */
{
  int x, y;
  
  switch ((int) outpixtyp) {
  case Iu_byte_typ: {
    IUB_BAND iub2 = (IUB_BAND)b2;

    FORALLYX
      iub2[y][x] = CLIP_RANGE_CAST(b1[y][x], 0, UNS_BYTE_MAX, UNS_BYTE);
    ENDFORALLYX;
  }
    break;
    
  case Is_byte_typ: {
    ISB_BAND isb2 = (ISB_BAND)b2;

    FORALLYX
      isb2[y][x] =
	CLIP_RANGE_CAST(b1[y][x], SIGN_BYTE_MIN, SIGN_BYTE_MAX, SIGN_BYTE);
    ENDFORALLYX;
  }
    break;
    
  case Iu_short_typ: {
    IUS_BAND ius2 = (IUS_BAND)b2;

    FORALLYX
      ius2[y][x] = CLIP_MIN_CAST(b1[y][x], 0, UNS_SHORT);
    ENDFORALLYX;
  }
    break;
    
  case Is_short_typ: {
    ISS_BAND iss2 = (ISS_BAND)b2;

    FORALLYX iss2[y][x] = b1[y][x]; ENDFORALLYX;
  }
    break;
    
  case Integer_typ: {
    II_BAND ii2 = (II_BAND)b2;

    FORALLYX ii2[y][x] = (INTEGER)b1[y][x]; ENDFORALLYX;
  }
    break;
    
  case Ireal_typ: {
    IR_BAND ir2 = (IR_BAND)b2;

    FORALLYX ir2[y][x] = (REAL)b1[y][x]; ENDFORALLYX;
  }
    break;
    
  case Icomplex_typ: {
    IC_BAND ic2 = (IC_BAND)b2;

    FORALLYX
      ic2[y][x].re = (REAL)b1[y][x];
      ic2[y][x].im = 0.0;
    ENDFORALLYX;
  }
    break;
    
  case Idouble_typ: {
    ID_BAND id2 = (ID_BAND)b2;

    FORALLYX id2[y][x] = (DOUBLE)b1[y][x]; ENDFORALLYX;
  }
    break;
    
  case Id_complex_typ: {
    IDC_BAND idc2 = (IDC_BAND)b2;

    FORALLYX
      idc2[y][x].re = (DOUBLE)b1[y][x];
      idc2[y][x].im = 0.0;
    ENDFORALLYX;
  }
    break;
    
  default:
    return(2);
    /* break; */
  } /* outpixtyp */
  return(0);
}  /* end of convFromSignhort */

/*
 __________________________________________________________________
*/

#ifndef FUNCPROTO
static int convFromInt(b1, b2, ysize, xsize, outpixtyp)
II_BAND b1, b2;
long ysize, xsize;
IPIXTYP outpixtyp;
#else /* FUNCPROTO */
static int convFromInt(II_BAND b1, II_BAND b2, long int ysize, long int xsize, IPIXTYP outpixtyp)
#endif /* FUNCPROTO */
{
  int x, y;
  
  switch ((int) outpixtyp) {
  case Iu_byte_typ: {
    IUB_BAND iub2 = (IUB_BAND)b2;

    FORALLYX
      iub2[y][x] = CLIP_RANGE_CAST(b1[y][x], 0, UNS_BYTE_MAX, UNS_BYTE);
    ENDFORALLYX;
  }
    break;
    
  case Is_byte_typ: {
    ISB_BAND isb2 = (ISB_BAND)b2;

    FORALLYX
      isb2[y][x] =
	CLIP_RANGE_CAST(b1[y][x], SIGN_BYTE_MIN, SIGN_BYTE_MAX, SIGN_BYTE);
    ENDFORALLYX;
  }
    break;
    
  case Iu_short_typ: {
    IUS_BAND ius2 = (IUS_BAND)b2;

    FORALLYX
      ius2[y][x] = CLIP_RANGE_CAST(b1[y][x], 0, UNS_SHORT_MAX, UNS_SHORT);
    ENDFORALLYX;
  }
    break;
    
  case Is_short_typ: {
    ISS_BAND iss2 = (ISS_BAND)b2;

    FORALLYX
      iss2[y][x] =
	CLIP_RANGE_CAST(b1[y][x], SIGN_SHORT_MIN, SIGN_SHORT_MAX, SIGN_SHORT);
    ENDFORALLYX;
  }
    break;
    
  case Integer_typ: {
    II_BAND ii2 = (II_BAND)b2;

    FORALLYX ii2[y][x] = b1[y][x]; ENDFORALLYX;
  }
    break;
    
  case Ireal_typ: {
    IR_BAND ir2 = (IR_BAND)b2;

    FORALLYX ir2[y][x] = (REAL)b1[y][x]; ENDFORALLYX;
  }
    break;
    
  case Icomplex_typ: {
    IC_BAND ic2 = (IC_BAND)b2;

    FORALLYX
      ic2[y][x].re = (REAL)b1[y][x];
      ic2[y][x].im = 0.0;
    ENDFORALLYX;
  }
    break;
    
  case Idouble_typ: {
    ID_BAND id2 = (ID_BAND)b2;

    FORALLYX id2[y][x] = (DOUBLE)b1[y][x]; ENDFORALLYX;
  }
    break;
    
  case Id_complex_typ: {
    IDC_BAND idc2 = (IDC_BAND)b2;

    FORALLYX
      idc2[y][x].re = (DOUBLE)b1[y][x];
      idc2[y][x].im = 0.0;
    ENDFORALLYX;
  }
    break;
    
  default:
    return(2);
    /* break; */
  } /* outpixtyp */
  return(0);
}  /* end of convFromInt */


/*
__________________________________________________________________
*/

#ifndef FUNCPROTO
static int convFromReal(b1, b2, ysize, xsize, outpixtyp)
IR_BAND b1, b2;
long ysize, xsize;
IPIXTYP outpixtyp;
#else /* FUNCPROTO */
static int convFromReal(IR_BAND b1, IR_BAND b2, long int ysize, long int xsize, IPIXTYP outpixtyp)
#endif /* FUNCPROTO */
{
  int x, y;
  
  switch ((int) outpixtyp) {
  case Iu_byte_typ: {
    IUB_BAND iub2 = (IUB_BAND)b2;

    FORALLYX
      iub2[y][x] = CLIP_RANGE_ROUND_CAST(b1[y][x], 0, UNS_BYTE_MAX, UNS_BYTE);
    ENDFORALLYX;
  }
    break;
    
  case Is_byte_typ: {
    ISB_BAND isb2 = (ISB_BAND)b2;

    FORALLYX
      isb2[y][x] = CLIP_RANGE_ROUND_CAST(b1[y][x], SIGN_BYTE_MIN,
					 SIGN_BYTE_MAX, SIGN_BYTE); 
    ENDFORALLYX;
  }
    break;
    
  case Iu_short_typ: {
    IUS_BAND ius2 = (IUS_BAND)b2;

    FORALLYX
      ius2[y][x] = CLIP_RANGE_ROUND_CAST(b1[y][x], 0, UNS_SHORT_MAX,
					 UNS_SHORT);
    ENDFORALLYX;
  }
    break;
    
  case Is_short_typ: {
    ISS_BAND iss2 = (ISS_BAND)b2;

    FORALLYX
      iss2[y][x] = CLIP_RANGE_ROUND_CAST(b1[y][x], SIGN_SHORT_MIN,
					 SIGN_SHORT_MAX, SIGN_SHORT); 
    ENDFORALLYX;
  }
    break;
    
  case Integer_typ: {
    II_BAND ii2 = (II_BAND)b2;

    FORALLYX
      ii2[y][x] = CLIP_RANGE_ROUND_CAST(b1[y][x], INTEGER_MIN, INTEGER_MAX,
					INTEGER);
    ENDFORALLYX;
  }
    break;
    
  case Ireal_typ: {
    IR_BAND ir2 = (IR_BAND)b2;

    FORALLYX ir2[y][x] = b1[y][x]; ENDFORALLYX;
  }
    break;
    
  case Icomplex_typ: {
    IC_BAND ic2 = (IC_BAND)b2;

    FORALLYX
      ic2[y][x].re = b1[y][x];
      ic2[y][x].im = 0.0;
    ENDFORALLYX;
  }
    break;
    
  case Idouble_typ: {
    ID_BAND id2 = (ID_BAND)b2;

    FORALLYX id2[y][x] = (DOUBLE)b1[y][x]; ENDFORALLYX;
  }
    break;
    
  case Id_complex_typ: {
    IDC_BAND idc2 = (IDC_BAND)b2;

    FORALLYX
      idc2[y][x].re = (DOUBLE)b1[y][x];
      idc2[y][x].im = 0.0;
    ENDFORALLYX;
  }
    break;
    
  default:
    return(2);
    /* break; */
  } /* outpixtyp */
  return(0);
}  /* end of convFromReal */


/*
__________________________________________________________________
*/

#ifndef FUNCPROTO
static int convFromComplex(b1, b2, ysize, xsize, outpixtyp)
IC_BAND b1, b2;
long ysize, xsize;
IPIXTYP outpixtyp;
#else /* FUNCPROTO */
static int convFromComplex(IC_BAND b1, IC_BAND b2, long int ysize, long int xsize, IPIXTYP outpixtyp)
#endif /* FUNCPROTO */
{
  int x, y;
  
  switch ((int) outpixtyp) {
  case Iu_byte_typ: {
    IUB_BAND iub2 = (IUB_BAND)b2;

    FORALLYX
      iub2[y][x] =
	CLIP_RANGE_ROUND_CAST(b1[y][x].re, 0, UNS_BYTE_MAX, UNS_BYTE);
    ENDFORALLYX;
  }
    break;
    
  case Is_byte_typ: {
    ISB_BAND isb2 = (ISB_BAND)b2;

    FORALLYX
      isb2[y][x] = CLIP_RANGE_ROUND_CAST(b1[y][x].re, SIGN_BYTE_MIN,
					 SIGN_BYTE_MAX, SIGN_BYTE); 
    ENDFORALLYX;
  }
    break;
    
  case Iu_short_typ: {
    IUS_BAND ius2 = (IUS_BAND)b2;

    FORALLYX
      ius2[y][x] = CLIP_RANGE_ROUND_CAST(b1[y][x].re, 0, UNS_SHORT_MAX,
					 UNS_SHORT);
    ENDFORALLYX;
  }
    break;
    
  case Is_short_typ: {
    ISS_BAND iss2 = (ISS_BAND)b2;

    FORALLYX
      iss2[y][x] = CLIP_RANGE_ROUND_CAST(b1[y][x].re, SIGN_SHORT_MIN,
					 SIGN_SHORT_MAX, SIGN_SHORT); 
    ENDFORALLYX;
  }
    break;
    
  case Integer_typ: {
    II_BAND ii2 = (II_BAND)b2;

    FORALLYX
      ii2[y][x] = CLIP_RANGE_ROUND_CAST(b1[y][x].re, INTEGER_MIN,
					INTEGER_MAX, INTEGER);
    ENDFORALLYX;
  }
    break;
    
  case Ireal_typ: {
    IR_BAND ir2 = (IR_BAND)b2;

    FORALLYX ir2[y][x] = b1[y][x].re; ENDFORALLYX;
  }
    break;
    
  case Icomplex_typ: {
    IC_BAND ic2 = (IC_BAND)b2;

    FORALLYX
      ic2[y][x].re = b1[y][x].re;
      ic2[y][x].im = b1[y][x].im;
    ENDFORALLYX;
  }
    break;
    
  case Idouble_typ: {
    ID_BAND id2 = (ID_BAND)b2;

    FORALLYX id2[y][x] = (DOUBLE)b1[y][x].re; ENDFORALLYX;
  }
    break;
    
  case Id_complex_typ: {
    IDC_BAND idc2 = (IDC_BAND)b2;

    FORALLYX
      idc2[y][x].re = (DOUBLE)b1[y][x].re;
      idc2[y][x].im = (DOUBLE)b1[y][x].im;
    ENDFORALLYX;
  }
    break;
    
  default:
    return(2);
    /* break; */
  } /* outpixtyp */
  return(0);
}  /* end of convFromComplex */


/*
__________________________________________________________________
*/

#ifndef FUNCPROTO
static int convFromDouble(b1, b2, ysize, xsize, outpixtyp)
ID_BAND b1, b2;
long ysize, xsize;
IPIXTYP outpixtyp;
#else /* FUNCPROTO */
static int convFromDouble(ID_BAND b1, ID_BAND b2, long int ysize, long int xsize, IPIXTYP outpixtyp)
#endif /* FUNCPROTO */
{
  int x, y;
  
  switch ((int) outpixtyp) {
  case Iu_byte_typ: {
    IUB_BAND iub2 = (IUB_BAND)b2;

    FORALLYX
      iub2[y][x] = CLIP_RANGE_ROUND_CAST(b1[y][x], 0, UNS_BYTE_MAX, UNS_BYTE);
    ENDFORALLYX;
  }
    break;
    
  case Is_byte_typ: {
    ISB_BAND isb2 = (ISB_BAND)b2;

    FORALLYX
      isb2[y][x] = CLIP_RANGE_ROUND_CAST(b1[y][x], SIGN_BYTE_MIN,
					 SIGN_BYTE_MAX, SIGN_BYTE); 
    ENDFORALLYX;
  }
    break;
    
  case Iu_short_typ: {
    IUS_BAND ius2 = (IUS_BAND)b2;

    FORALLYX
      ius2[y][x] =
	CLIP_RANGE_ROUND_CAST(b1[y][x], 0, UNS_SHORT_MAX, UNS_SHORT);
    ENDFORALLYX;
  }
    break;
    
  case Is_short_typ: {
    ISS_BAND iss2 = (ISS_BAND)b2;

    FORALLYX
      iss2[y][x] = CLIP_RANGE_ROUND_CAST(b1[y][x], SIGN_SHORT_MIN,
					 SIGN_SHORT_MAX, SIGN_SHORT); 
    ENDFORALLYX;
  }
    break;
    
  case Integer_typ: {
    II_BAND ii2 = (II_BAND)b2;

    FORALLYX
      ii2[y][x] = CLIP_RANGE_ROUND_CAST(b1[y][x], INTEGER_MIN,
					INTEGER_MAX, INTEGER);
    ENDFORALLYX;
  }
    break;
    
  case Ireal_typ: {
    IR_BAND ir2 = (IR_BAND)b2;

    FORALLYX
      ir2[y][x] = CLIP_RANGE_CAST(b1[y][x], -REAL_MAX, REAL_MAX, REAL);
    ENDFORALLYX;
  }
    break;
    
  case Icomplex_typ: {
    IC_BAND ic2 = (IC_BAND)b2;

    FORALLYX
      ic2[y][x].re = CLIP_RANGE_CAST(b1[y][x], -REAL_MAX, REAL_MAX, REAL);
      ic2[y][x].im = 0.0;
    ENDFORALLYX;
  }
    break;
    
  case Idouble_typ: {
    ID_BAND id2 = (ID_BAND)b2;

    FORALLYX id2[y][x] = b1[y][x]; ENDFORALLYX;
  }
    break;
    
  case Id_complex_typ: {
    IDC_BAND idc2 = (IDC_BAND)b2;

    FORALLYX
      idc2[y][x].re = b1[y][x];
      idc2[y][x].im = 0.0;
    ENDFORALLYX;
  }
    break;
    
  default:
    return(2);
    /* break; */
  } /* outpixtyp */
  return(0);
}  /* end of convFromDouble */


/*
__________________________________________________________________
*/

#ifndef FUNCPROTO
static int convFromDComplex(b1, b2, ysize, xsize, outpixtyp)
IDC_BAND b1, b2;
long ysize, xsize;
IPIXTYP outpixtyp;
#else /* FUNCPROTO */
static int convFromDComplex(IDC_BAND b1, IDC_BAND b2, long int ysize, long int xsize, IPIXTYP outpixtyp)
#endif /* FUNCPROTO */
{
  int x, y;
  
  switch ((int) outpixtyp) {
  case Iu_byte_typ: {
    IUB_BAND iub2 = (IUB_BAND)b2;

    FORALLYX
      iub2[y][x] =
	CLIP_RANGE_ROUND_CAST(b1[y][x].re, 0, UNS_BYTE_MAX, UNS_BYTE);
    ENDFORALLYX;
  }
    break;
    
  case Is_byte_typ: {
    ISB_BAND isb2 = (ISB_BAND)b2;

    FORALLYX
      isb2[y][x] = CLIP_RANGE_ROUND_CAST(b1[y][x].re, SIGN_BYTE_MIN,
					 SIGN_BYTE_MAX, SIGN_BYTE); 
    ENDFORALLYX;
  }
    break;
    
  case Iu_short_typ: {
    IUS_BAND ius2 = (IUS_BAND)b2;

    FORALLYX
      ius2[y][x] =
	CLIP_RANGE_ROUND_CAST(b1[y][x].re, 0, UNS_SHORT_MAX, UNS_SHORT);
    ENDFORALLYX;
  }
    break;
    
  case Is_short_typ: {
    ISS_BAND iss2 = (ISS_BAND)b2;

    FORALLYX
      iss2[y][x] = CLIP_RANGE_ROUND_CAST(b1[y][x].re, SIGN_SHORT_MIN,
					 SIGN_SHORT_MAX, SIGN_SHORT); 
    ENDFORALLYX;
  }
    break;
    
  case Integer_typ: {
    II_BAND ii2 = (II_BAND)b2;

    FORALLYX
      ii2[y][x] = CLIP_RANGE_ROUND_CAST(b1[y][x].re, INTEGER_MIN,
					INTEGER_MAX, INTEGER);
    ENDFORALLYX;
  }
    break;
    
  case Ireal_typ: {
    IR_BAND ir2 = (IR_BAND)b2;

    FORALLYX
      ir2[y][x] = CLIP_RANGE_CAST(b1[y][x].re, -REAL_MAX, REAL_MAX, REAL);
    ENDFORALLYX;
  }
    break;
    
  case Icomplex_typ: {
    IC_BAND ic2 = (IC_BAND)b2;

    FORALLYX
      ic2[y][x].re = CLIP_RANGE_CAST(b1[y][x].re, -REAL_MAX, REAL_MAX, REAL);
      ic2[y][x].im = CLIP_RANGE_CAST(b1[y][x].im, -REAL_MAX, REAL_MAX, REAL);
    ENDFORALLYX;
  }
    break;
    
  case Idouble_typ: {
    ID_BAND id2 = (ID_BAND)b2;

    FORALLYX id2[y][x] = b1[y][x].re; ENDFORALLYX;
  }
    break;
    
  case Id_complex_typ: {
    IDC_BAND idc2 = (IDC_BAND)b2;

    FORALLYX
      idc2[y][x].re = b1[y][x].re;
      idc2[y][x].im = b1[y][x].im;
    ENDFORALLYX;
  }
    break;
    
  default:
    return(2);
    /* break; */
  } /* outpixtyp */
  return(0);
}  /* end of convFromDComplex */

/*
__________________________________________________________________
*/

#ifndef FUNCPROTO
int convertBand(b1, b2)
IBAND b1, b2;
#else /* FUNCPROTO */
int convertBand(IBAND b1, IBAND b2)
#endif /* FUNCPROTO */
{
  long xsize, ysize;
  IPIXTYP inpixtyp, outpixtyp;

  xsize = MIN(Ixsize(b1), Ixsize(b2));
  ysize = MIN(Iysize(b1), Iysize(b2));
  inpixtyp = Ipixtyp(b1);
  outpixtyp = Ipixtyp(b2);

  switch((int) inpixtyp) {
  case Iu_byte_typ:
    return(convFromUnsByte(b1, b2, ysize, xsize, outpixtyp));
  case Is_byte_typ:
    return(convFromSignByte((ISB_BAND)b1, (ISB_BAND)b2, ysize, xsize,
			    outpixtyp));
  case Iu_short_typ:
    return(convFromUnsShort((IUS_BAND)b1, (IUS_BAND)b2, ysize, xsize,
			    outpixtyp));
  case Is_short_typ:
    return(convFromSignShort((ISS_BAND)b1, (ISS_BAND)b2, ysize, xsize,
			     outpixtyp));
  case Integer_typ:
    return(convFromInt((II_BAND)b1, (II_BAND)b2, ysize, xsize,
		       outpixtyp));
  case Ireal_typ:
    return(convFromReal((IR_BAND)b1, (IR_BAND)b2, ysize, xsize, 
			outpixtyp));
  case Icomplex_typ:
    return(convFromComplex((IC_BAND)b1, (IC_BAND)b2, ysize, xsize,
			   outpixtyp));
  case Idouble_typ:
    return(convFromDouble((ID_BAND)b1, (ID_BAND)b2, ysize, xsize,
			  outpixtyp));
  case Id_complex_typ:
    return(convFromDComplex((IDC_BAND)b1, (IDC_BAND)b2, ysize, xsize,
			    outpixtyp));
  }
  return(1);
}


/*-------------------------------------------------------------
*/

#ifndef FUNCPROTO
IBAND mkConvertBand(b, pixtyp)
IBAND b;
IPIXTYP pixtyp;
#else /* FUNCPROTO */
IBAND mkConvertBand(IBAND b, IPIXTYP pixtyp)
#endif /* FUNCPROTO */
{
  IBAND b2;
  int status;
  b2 = Imake_band(pixtyp, Ixsize(b), Iysize(b));
  if (NOT b2) return((IBAND)0);
  status = convertBand(b, b2);
  if (status) {
    Idel_band(&b2);
  return((IBAND)0);
  }
  return(b2);
}

#endif /* not MAIN */

/*P:biffConvert*

________________________________________________________________

		biffConvert
________________________________________________________________

Name:		biffConvert - convert image between different pixel types

Syntax:		| biffConvert <inimage> <outimage> <pixeltype>
                |    [<pixeltype>...]

Description:    'biffConvert' reads the input image <inimage>, converts
                every band to specified pixel type, and writes output
		image to <outimage>. First pixel type specification is
		applied to first band, second to second,.. If more bands,
		the last pixel type specification is applied to the rest
		of the bands. Pixel types are specified by a pixel type
		number:

		| ub or  3 => convert to unsigned byte
		| sb or  4 => convert to signed byte
		| us or  5 => convert to unsigned short
		| ss or  6 => convert to signed short
		| i  or  7 => convert to integer
		| r  or  8 => convert to real
		| c  or  9 => convert to complex
		| d  or 10 => convert to double
		| dc or 11 => convert to double complex

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Examples:       | # convert to real: 
                | biffConvert mona.img monaReal.img 8
                |
                | # convert to byte:
                | biffConvert spot.img spotByte.img 3
                |
                | # convert 1st to byte, 2nd to integer, 3rd to real:
                | biffConvert spot.img spotBIR.img 3 7 8 
                |
                | # convert first band to byte and short:
                | biffConvert mona.img:1:1 monaBS.img 3 5 

Id: 		$Id$
________________________________________________________________

*/

#ifdef MAIN

#ifndef FUNCPROTO
int main(argc, argv)
int argc;
char **argv;
#else /* FUNCPROTO */
int main(int argc, char **argv)
#endif /* FUNCPROTO */
{
  IMAGE i1, i2;
  int bn;
  IPIXTYP pt;
  
  Iset_message(TRUE);
  Iset_abort(TRUE);
  InitMessage(&argc, argv, xite_app_std_usage_text(
     "Usage: %s <inimage> <outimage> <pixtyp> [<pixtyp>...]\n\
        where pixtyp may be \n\
          ub or 3 => unsigned byte,    sb or  4 => signed byte, \n\
          us or 5 => unsigned short,   ss or  6 => signed short, \n\
          i  or 7 => integer,          r  or  8 => real, \n\
          c  or 9 => complex,          d  or 10 => double, \n\
          dc or 11 => double complex\n"));

  if (argc == 1) Usage(1, NULL);
  if (argc < 4) Usage(2, "Illegal number of arguments.\n");
  
  i1 = Iread_image(argv[1]);
  if (NOT i1) exit(Error(3, "Couldn't read %s\n", argv[1]));
  i2 = Init_image(Inbands(i1), Ititle(i1));
  if (NOT (size_t)i2) exit(Error(4, "Malloc failed\n"));
  Icopy_text(i1, i2);
  for (bn=1; bn LE Inbands(i1); bn++) {
    pt = (argc GE bn+3) ? IparsePixtyp(argv[bn+2]) : pt;
    i2[bn] = mkConvertBand(i1[bn], pt);
    if (NOT i2[bn]) {
      Warning(5, "Couldn't convert band %d\n", bn);
      i2[bn] = i1[bn];
    }
  }
  
  Ihistory(i2, argv[0], "");
  Iwrite_image(i2, argv[2]);
  return(0);
}

#endif /* MAIN */
