

/*

________________________________________________________________

        copyBand
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
#include <xite/blab.h>
#include <xite/biff.h>
#include <xite/copy.h>
#include <xite/message.h>

#ifndef MIN
# define MIN(a,b) (((a) LT (b)) ? (a) : (b))
#endif

/*F:copyBand*

________________________________________________________________

		copyBand
________________________________________________________________

Name:		copyBand - copy a band

Syntax:         | #include <xite/copy.h>
		|
                | int copyBand( IBAND b1, IBAND b2 );
Description:    The largest rectangle common to b1 and b2 is copied
                from b1 to b2. A direct copy is performed, which will
                give strange effects if b1 and b2 are partly
                overlapping subbands. b1 and b2 should have the same
                pixel type, but all currently known pixel types are
                supported.

Return value:   | 0 => ok
                | 1 => b1 and b2 have different pixel types
                | 2 => unknown pixel type

Author:		Tor L|nnestad

Id:             $Id$
________________________________________________________________

*/

#ifndef FUNCPROTO
int copyBand(b1,b2)
IBAND b1,b2;
#else /* FUNCPROTO */
int copyBand(IBAND b1, IBAND b2)
#endif /* FUNCPROTO */
{
  int x, y, xsize, ysize;
  /* Declaire band pointers for all different pixel SIZES */
  
  if (Ipixtyp(b1) NE Ipixtyp(b2))
    return(Error(1, "copyBand: Bands have different pixel types\n"));

  xsize = MIN(Ixsize(b1),Ixsize(b2));
  ysize = MIN(Iysize(b1),Iysize(b2));

  switch (Ipixtyp(b1)) {

  case Iu_byte_typ:
  case Is_byte_typ: {
    IUB_BAND iub1 = (IUB_BAND) b1;
    IUB_BAND iub2 = (IUB_BAND) b2;
    for (y=1; y LE ysize; INC y)
      for (x=1; x LE xsize; INC x)
	iub2[y][x] = iub1[y][x];
  }
    break;
    
  case Iu_short_typ:
  case Is_short_typ: {
    IUS_BAND ius1 = (IUS_BAND) b1;
    IUS_BAND ius2 = (IUS_BAND) b2;
    for (y=1; y LE ysize; INC y)
      for (x=1; x LE xsize; INC x)
	ius2[y][x] = ius1[y][x];
  }
    break;
    
  case Integer_typ:
  case Ireal_typ: {
    II_BAND ii1 = (II_BAND) b1;
    II_BAND ii2 = (II_BAND) b2;
    for (y=1; y LE ysize; INC y)
      for (x=1; x LE xsize; INC x)
	ii2[y][x] = ii1[y][x];
  }
    break;
    
  case Icomplex_typ:
  case Idouble_typ: {
    ID_BAND id1 = (ID_BAND) b1;
    ID_BAND id2 = (ID_BAND) b2;
    for (y=1; y LE ysize; INC y)
      for (x=1; x LE xsize; INC x)
	id2[y][x] = id1[y][x];
  }
    break;
    
  case Id_complex_typ: {
    IDC_BAND idc1 = (IDC_BAND) b1;
    IDC_BAND idc2 = (IDC_BAND) b2;
    for (y=1; y LE ysize; INC y)
      for (x=1; x LE xsize; INC x)
	idc2[y][x] = idc1[y][x];
  }
    break;
    
  default:
    return(2);
  } /* switch */

  return(0);
}
