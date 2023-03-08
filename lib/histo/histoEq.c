
/*

________________________________________________________________

        histoEq
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
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/histo.h>
#include <xite/message.h>
#include <xite/readarg.h>

/*F:histoEq*

________________________________________________________________

		histoEq
________________________________________________________________

Name:		histoEq - histogram equalization

Syntax:         | #include <xite/histo.h>
                |
                | BiffStatus histoEq( IBAND input, IBAND output,
                |    int graylevels );

Description:    Perform histogram equalization. 'input' and 'output'
                may be identical. The whole 'input' band is used
                to generate the histogram transformation, but
                only the largest rectangle common to 'input' and 'output'
                is transformed.

                'graylevels' is the maximum number of levels in 'output'.

Restrictions:   'input' and 'output' must have pixel type unsigned byte.

Return value:   | 0 => ok
                | 1 => bad pixel type b1
                | 2 => bad pixel type b2
                | 3 => bad value graylevels

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Id:             $Id$
________________________________________________________________
*/

BiffStatus histoEq(IBAND b1, IBAND b2, int graylevels)
{
  histogram histo;

  if (Ipixtyp(b1) NE Iu_byte_typ)
    return(Error(1, "histoEq: Input pixel type must be unsigned byte.\n"));
  if (Ipixtyp(b2) NE Iu_byte_typ)
    return(Error(2, "histoEq: Output pixel type must be unsigned byte.\n"));
  if ((graylevels LT 2) OR (graylevels GT 256))
    return(Error(3, "histoEq: Bad value for graylevels.\n"));

  mkHisto(b1, histo);
  mkCumHisto(histo, histo);
  scaleHisto(histo, histo, graylevels);
  histoTransf(b1, b2, histo);
  return(0);
}
