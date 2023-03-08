
/*

________________________________________________________________

        histoNorm
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
#include <stdlib.h>
#include XITE_STDIO_H
#include <xite/blab.h>
#include <xite/histo.h>
#include <xite/message.h>

/*F:histoNorm*

________________________________________________________________

		histoNorm
________________________________________________________________

Name:		histoNorm - histogram normalization

Syntax:         | #include <xite/histo.h>
                |
                | int histoNorm( IBAND b1, IBAND b2, double my,
                |    double sigma );
Description:    Perform histogram normalization; approximate hi-
                stogram of output image to the normal (gaussian)
                distribution with mean value my and st.dev.=sigma.
                Input and output may be identical. Only the largest
                rectangle common to b1 and b2 is transformed.

                | b1 - input band
                | b2 - output band
                | my - desired mean value
                | sigma - desired standard deviation

Return value:   | 0 => ok
                | 1 => bad input pixel type
                | 2 => bad output pixel type

Author:		Tor L|nnestad

Id:             $Id$
________________________________________________________________
*/

int histoNorm(IBAND b1, IBAND b2, double my, double sigma)
{
  int n;
  histogram normHisto;
  double sigma2_2 = 2*sigma*sigma;

  if (Ipixtyp(b1) != Iu_byte_typ)
    return(Error(1, "histoNorm: Bad input pixel type\n"));
  if (Ipixtyp(b2) != Iu_byte_typ)
    return(Error(2, "histoNorm: Bad output pixel type\n"));

  /* make normal distribution */
  FOR (n=0; n LE 255; INC n)
    normHisto[n] =
      (int)(exp(-((double)n-my)*((double)n-my)/sigma2_2)*100000.0+0.5);
  ENDFOR;
  histoSpecify(b1, b2, normHisto);
  return(0);
}
