

/*C*

________________________________________________________________

        fhtPower
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


#include <xite/includes.h>
#include <xite/biff.h>
#include <stdlib.h>
#include <xite/fht.h>
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#include <xite/blab.h>
#include <xite/readarg.h>
#include <xite/message.h>



/*F:fhtPower*

________________________________________________________________

		fhtPower
________________________________________________________________

Name:		fhtPower - calculate power specter from Hartley transform

Syntax:         | #include <xite/fht.h>
		|
                | int fhtPower( IR_BAND inband, IR_BAND outband );

Description:    'fhtPower' calculates the power spectrum from a
                Hartley transform. The power spectrum is identical
		to the power spectrum calculated from the Fourier
		transform, and the Hartley transform is significantly
		faster.

		The power spectrum is
		|
		| P(u,v) = [Re{F(u,v)}]^2 + [Im{F(u,v)}]^2
		|        = H_e(u,v)^2 + H_o(u,v)^2
		|        = (H(u,v)^2 + H(-u,-v)^2)/2

		For images with horizontal and vertical even symmetry,
		the power spectrum simplifies to
		|
		| P(u,v) = ( H(u,v)^2 + H(-u,-v)^2 ) / 2
		|        = H(u,v)^2

Restrictions:	Bands should have equal size. Currently, only single
		precision float values are accepted.

See also:	fhtPower(1), fht2d(3), fft2d(3), ft2ht(3), ht2ft(3),
                fhtPhase(3)

Return value:   | 0 => ok
                | 1 => bad pixel type input
		| 2 => bad pixel type output
		| 3 => bad sizes

Author:		Tor L�nnestad, BLAB, Ifi, UiO

Id: 		$Id$
________________________________________________________________

*/


int fhtPower(IR_BAND inband, IR_BAND outband)
{
  int x, y, xsize, ysize, xsizep2, ysizep2;

  if (Ipixtyp((IBAND) inband) NE Ireal_typ) return(1);
  if (Ipixtyp((IBAND) outband) NE Ireal_typ) return(2);
  xsize = Ixsize((IBAND) inband);
  ysize = Iysize((IBAND) inband);
  if (xsize NE Ixsize((IBAND) outband)) return(3);
  if (ysize NE Iysize((IBAND) outband)) return(3);
  xsizep2 = xsize+2;
  ysizep2 = ysize+2;

  /* dc component */
  outband[1][1] = inband[1][1]*inband[1][1];

  /* first line */
  for (x=2; x LE xsize; x++)
    outband[1][x] = (inband[1][x]*inband[1][x] 
      + inband[1][xsizep2-x]*inband[1][xsizep2-x])/2.0;

  /* general line loop */
  FOR (y=2; y LE ysize; y++)
    /* first sample */
    outband[y][1] = (inband[y][1]*inband[y][1] 
      + inband[ysizep2-y][1]*inband[ysizep2-y][1])/2.0;

    /* general sample loop */
    for (x=2; x LE xsize; x++)
      outband[y][x] = (inband[y][x]*inband[y][x] 
        + inband[ysizep2-y][xsizep2-x]*inband[ysizep2-y][xsizep2-x])/2.0;
  ENDFOR
  return(0);
}
