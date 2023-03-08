/*

________________________________________________________________

        thresBernsen
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
#include <xite/threshold.h>
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/minmax.h>
#include <xite/readarg.h>
#include <xite/message.h>

#ifndef MIN
# define MIN(a,b) (((a)<(b)) ? (a) : (b))
#endif

/*F:thresBernsen*

________________________________________________________________

		thresBernsen
________________________________________________________________

Name:		thresBernsen - local treshold by the method of Bernsen.

Syntax:         | #include <xite/threshold.h>
		|
                | int thresBernsen( IBAND inband, IBAND outband,
                |    int cmin, int dx, int dy, int lpv, int hpv,
                |    int low );

Description:	If the difference between the largest and the smallest
		pixel value within the 'dx'*'dy' window is greater than
		or equal to 'cmin' (local contrast threshold), the average
		of the two values is used as threshold. Pixels below this
		threshold are set to 'lpv', and pixels above are set to 'hpv'.

		If 'low' is TRUE, pixels in homogenous areas (difference
		below 'cmin') are assumed to be below the threshold and are
		set to 'lpv', and if 'low' is FALSE these pixels are set
		to 'hpv'.

Restrictions:   'inband' and 'outband' must have pixel type unsigned byte.

Reference:	Bernsen, J: "Dynamic thresholding of grey-level images"
		Proc. of the 8th ICPR, Paris, Oct 1986, 1251-1255.

Return value:   | 0 => ok
                | 1 => bad pixel type inband
                | 2 => bad pixel type outband
		| 3 => bad cmin value
		| 4 => bad dx or dy value
		| 5 => bad lpv or hpv value
		| 6 => not enough memory

Author:		Olav Borgli and Tor Lønnestad, BLAB, Ifi, UiO

Id:		$Id$
________________________________________________________________

*/


int thresBernsen(IBAND inband, IBAND outband, int cmin, int dx, int dy, int lpv, int hpv, int low)
{
  IBAND maxband, minband;
  int xsize, ysize, x, y, t, c, bpv;

  xsize = MIN(Ixsize(inband), Ixsize(outband));
  ysize = MIN(Iysize(inband), Iysize(outband));

  if (Ipixtyp(inband) NE Iu_byte_typ)
    return(Error(1,
		 "thresBernsen: Input pixel type must be unsigned byte.\n"));
  if (Ipixtyp(outband) NE Iu_byte_typ)
    return(Error(2,
		 "thresBernsen: Output pixel type must be unsigned byte.\n"));
  if ((cmin LT 0) OR (cmin GT 256))
    return(Error(3, "thresBernsen: Bad cmin value.\n"));
  if ((dx LT 1) OR (dy LT 1) OR (dx GT xsize) OR (dy GT ysize))
    return(Error(4, "thresBernsen: Bad dx or dy value.\n"));
  if ((lpv LT 0) OR (hpv LT 0) OR (lpv GT 255) OR (hpv GT 255))
    return(Error(5, "thresBernsen: Bad lpv or hpv value.\n"));

  minband = Imake_band(3, xsize, ysize);
  maxband = Imake_band(3, xsize, ysize);
  if (NOT minband OR NOT maxband)
    return(Error(6, "Not enough memory\n"));
  bpv = (low) ? lpv : hpv;

  minarea(inband, minband, dx, dy);
  maxarea(inband, maxband, dx, dy);

  for (y=1; y<=ysize; y++)
    for (x=1; x<=xsize; x++)
      {
	c = maxband[y][x] - minband[y][x];
	if (c < cmin) outband[y][x] = bpv;
        else
	{ 
          t = (maxband[y][x] + minband[y][x]) / 2;
          if (inband[y][x] >= t)
	    outband[y][x] = hpv;
	  else
	    outband[y][x] = lpv;
        }
      }

  Idel_band(&minband);
  Idel_band(&maxband);
  return(0);
}
