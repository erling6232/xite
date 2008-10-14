

/*

________________________________________________________________

        biffswap
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
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#include <xite/message.h>



/*F:biff_swap*

________________________________________________________________

		biff_swap
________________________________________________________________

Name:		biff_swap - swap byte order in biff-band
Syntax:         | #include <xite/convert.h>
		|
                | int biff_swap( IBAND band );
Description:	Swap byte order. Works for all pixel types, including complex
                and double complex.

		There is usually no need to call this function, because XITE
		takes care of swapping bytes automatically.
Return value:	0
See also:       biffswap(1)
Author:		Otto Milvang and Svein Bøe
Bug fixes:      Svein Bøe
________________________________________________________________

*/

int biff_swap(IBAND band)
{
  IUS_BAND ius_band;
  II_BAND ii_band;
  ID_BAND id_band;
  IC_BAND ic_band;
  IDC_BAND idc_band;
  int xsize, ysize, x, y;
  IPIXTYP pt;
  int ptl;
  unsigned char *ptr, c;

  pt = Ipixtyp(band);
  ptl = Ipixsize(pt)/8;
  xsize = Ixsize(band);
  ysize = Iysize(band);

  switch(ptl)
    {
      case 2:
	ius_band = (IUS_BAND) band;
	for(y=1; y<=ysize; y++)
	  for(x=1; x<=xsize; x++)
	    { 
              ptr = (unsigned char *) &ius_band[y][x];
	      c = *ptr;
	      *ptr = ptr[1];
	      ptr[1] = c;
	  }
	break;
      case 4:
	ii_band = (II_BAND) band;
	for(y=1; y<=ysize; y++)
	  for(x=1; x<=xsize; x++)
	    { 
              ptr = (unsigned char *) &ii_band[y][x];
	      c = *ptr;
	      *ptr = ptr[3];
	      ptr[3] = c;
	      c = ptr[1];
	      ptr[1] = ptr[2];
	      ptr[2] = c;
	  }
	break;
      case 8:
	switch ((int) pt)
	  {
	  case Idouble_typ:
	    id_band = (ID_BAND) band;
	    for(y=1; y<=ysize; y++)
	      for(x=1; x<=xsize; x++)
		{ 
		  ptr = (unsigned char *) &id_band[y][x];
		  c = ptr[0];
		  ptr[0] = ptr[7];
		  ptr[7] = c;
		  c = ptr[1];
		  ptr[1] = ptr[6];
		  ptr[6] = c;
		  c = ptr[2];
		  ptr[2] = ptr[5];
		  ptr[5] = c;
		  c = ptr[3];
		  ptr[3] = ptr[4];
		  ptr[4] = c;
		}
	    break;
	  case Icomplex_typ:
	    ic_band = (IC_BAND) band;
	    for(y=1; y<=ysize; y++)
	      for(x=1; x<=xsize; x++)
		{ 
		  ptr = (unsigned char *) &ic_band[y][x].re;
		  c = ptr[0];
		  ptr[0] = ptr[3];
		  ptr[3] = c;
		  c = ptr[1];
		  ptr[1] = ptr[2];
		  ptr[2] = c;

		  ptr = (unsigned char *) &ic_band[y][x].im;
		  c = ptr[0];
		  ptr[0] = ptr[3];
		  ptr[3] = c;
		  c = ptr[1];
		  ptr[1] = ptr[2];
		  ptr[2] = c;
		}
	    break;
	  default:
	    Warning(0, "Can't convert %s band\n", Ipixname(pt));
	    break;
	  }
	break;
      case 16:
	idc_band = (IDC_BAND) band;
	for(y=1; y<=ysize; y++)
	  for(x=1; x<=xsize; x++)
	    { 
	      ptr = (unsigned char *) &idc_band[y][x].re;
	      c = ptr[0];
	      ptr[0] = ptr[7];
	      ptr[7] = c;
	      c = ptr[1];
	      ptr[1] = ptr[6];
	      ptr[6] = c;
	      c = ptr[2];
	      ptr[2] = ptr[5];
	      ptr[5] = c;
	      c = ptr[3];
	      ptr[3] = ptr[4];
	      ptr[4] = c;

	      ptr = (unsigned char *) &idc_band[y][x].im;
	      c = ptr[0];
	      ptr[0] = ptr[7];
	      ptr[7] = c;
	      c = ptr[1];
	      ptr[1] = ptr[6];
	      ptr[6] = c;
	      c = ptr[2];
	      ptr[2] = ptr[5];
	      ptr[5] = c;
	      c = ptr[3];
	      ptr[3] = ptr[4];
	      ptr[4] = c;
	    }
	break;
      default:
	Warning(0, "Can't convert %s band\n", Ipixname(pt));
	break;
    }
  return(0);
}
