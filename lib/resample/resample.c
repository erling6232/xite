
/*

________________________________________________________________

        resample
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
#include <xite/resample.h>
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/message.h>

/*F:resample*

________________________________________________________________

		resample
________________________________________________________________

Name:		resample - stretch or compress a band

Syntax:         | #include <xite/resample.h>
		|
                | int resample( IBAND b1, IBAND b2 );
Description:    The data in input band b1 is resampled into output
                band b2, using nearest neighbour resampling. Accepts
		only unsigned byte pixels.

Return value:   | 0 => ok
                | 1 => Bad input pixel type
                | 2 => Bad output pixel type

Author:		Tor L|nnestad, BLAB, ifi, UiO

Id:             $Id$
________________________________________________________________

*/

int resample(IBAND b1, IBAND b2)
{
  int x, y, x1, y1, xsize2, ysize2;
  float xfactor, yfactor, xoffset, yoffset;

  if (Ipixtyp(b1) NE Iu_byte_typ)
    return(Error(1, "resample: Bad input pixel type\n"));
  if (Ipixtyp(b2) NE Iu_byte_typ)
    return(Error(2, "resample: Bad output pixel type\n"));


  xfactor = ((float)Ixsize(b1))/(float)Ixsize(b2);
  yfactor = ((float)Iysize(b1))/(float)Iysize(b2);
  xoffset = 1.0 - xfactor/2;
  yoffset = 1.0 - yfactor/2;
  xsize2 = Ixsize(b2);
  ysize2 = Iysize(b2);

  FOR (y=1; y LE ysize2; y++)
    FOR (x=1; x LE xsize2; x++)
      x1 = (int)(xoffset + xfactor*x);
      y1 = (int)(yoffset + yfactor*y);
      b2[y][x] = b1[y1][x1];
    ENDFOR;
  ENDFOR;
  return 0;
}
