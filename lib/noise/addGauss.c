
/*

________________________________________________________________

        addGauss
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


#include <stdlib.h>
#include <xite/includes.h>
#include <xite/biff.h>
#include <xite/noise.h>
#include <xite/blab.h>
#include <xite/message.h>
#include <xite/utils.h>
#include XITE_RANDOM_H
#include XITE_STDIO_H
#include XITE_TIME_H

#ifndef MIN
# define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif


/*F:addGauss*

________________________________________________________________

		addGauss
________________________________________________________________

Name:		addGauss - add gaussian random noise to a band

Syntax:         | #include <xite/noise.h>
		|
                | BiffStatus addGauss( IBAND b1, IBAND b2,
                |    double my, double sigma );
Description:    Adds random noise with Gaussian distribution to
                band b1, places the result in band b2. The
                distribution has mean value my and standard
                deviation sigma. If resulting value is larger
                than 255 output is set to 255, and if resulting
                value is less than 0 output is set to 0.

Return value:   | 0 => ok
                | 1 => Bad input pixel type
                | 2 => Bad output pixel type

Author:		Tor L|nnestad, BLAB, IfI, UiO

Id:             $Id$
________________________________________________________________

*/

BiffStatus addGauss(IBAND b1, IBAND b2, double my, double sigma)
{
  int x, y, xsize, ysize, tmp;

  if (Ipixtyp(b1) NE Iu_byte_typ)
    return(Error(1, "addGauss: Bad input pixel type\n"));
  if (Ipixtyp(b2) NE Iu_byte_typ)
    return(Error(2, "addGauss: Bad output pixel type\n"));
  xsize = MIN(Ixsize(b1),Ixsize(b2));
  ysize = MIN(Iysize(b1),Iysize(b2));
  my += 0.5; /* due to conversion to int later on */

  srandom(time(0)); /* random initialization of random generator */
  FOR (y=1; y LE ysize; INC y)
    FOR (x=1; x LE xsize; INC x)
      tmp = (int)((double)b1[y][x] + gaussRandom(my,sigma));
      if (tmp LT 0) b2[y][x] = 0;
      else if (tmp GT 255) b2[y][x] = 255;
      else b2[y][x] = (UNS_BYTE)tmp;
    ENDFOR;
  ENDFOR;
  return(0);
} /* int addGauss */
