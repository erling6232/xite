
/*

________________________________________________________________

        dither
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


#include <math.h>
#include <xite/includes.h>
#include <xite/biff.h>
#include <xite/dither.h>
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/message.h>

#ifndef MIN
# define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

/*F:dither*

________________________________________________________________

		dither
________________________________________________________________

Name:		dither - create a dithering band

Syntax:         | #include <xite/dither.h>
		|
                | int dither( IBAND b1, IBAND b2, int t );
Description:    'dither' dithers b1 into b2. Only largest common
                rectangle is ditherd. Accepts only UNS_BYTE pixles.
		t is used as a threshold-like argument.
                Identical in- and out- parameters are legal.

Return value:   | 0 => ok
                | 1 => Bad input pixel type
                | 2 => Bad output pixel type
                | 3 => Bad threshold value

Author:		Tor L|nnestad, BLAB, IfI, UiO 

Id:             $Id$
________________________________________________________________

*/

int dither(IBAND b1, IBAND b2, int t)
{
  int ysize;
  int x, y, xsize, rest;
  float scale = (t/(255.0-t));

  if (Ipixtyp(b1) NE Iu_byte_typ)
    return(Error(1, "dither: Bad input pixel type\n"));
  if (Ipixtyp(b2) NE Iu_byte_typ)
    return(Error(2, "dither: Bad output pixel type\n"));
  if ((t GT 255) OR (t LT 0))
    return(Error(3, "dither: Bad threshold value\n"));
  xsize = MIN(Ixsize(b1),Ixsize(b2));
  ysize = MIN(Iysize(b1),Iysize(b2));
  rest = 0; /* always in [-127,127] */ 

  FOR (y=1; y LT ysize; INC y)
    FOR (x=1; x LE xsize; INC x)
      IF ((int)(b1[y][x] + rest) GT t)
        rest -= (int)(((int)255 - (int)b1[y][x])*scale);
        b2[y][x] = (UNS_BYTE)255;
      ELSE
        rest += (int)b1[y][x];
        b2[y][x] = (UNS_BYTE)0;
      ENDIF;
    ENDFOR;
  ENDFOR;
  return(0);
}
