
/*

________________________________________________________________

        histoSpecify
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
#include <xite/blab.h>
#include <xite/histo.h>
#include <xite/message.h>

#ifndef MIN
# define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif


/*F:histoSpecify*

________________________________________________________________

		histoSpecify
________________________________________________________________

Name:		histoSpecify - histo transformation to specified function

Syntax:         | #include <xite/histo.h>
                |
                | int histoSpecify( IBAND b1, IBAND b2,
                |    histogram histo );
Description:    Perform  a  histogram  transformation generating
                a band with histogram  approximating  the  given
                histogram as good as possible.  Input and output
                band may be identical. If b1 and b2 have different
                sizes, the largest common rectangle is transformed.

                | b1    - input band
                | b2    - output histogram
                | histo - histogram to be approximated

Return value:   | 0 => ok
                | 1 => bad input pixel type
                | 2 => bad output pixel type


Author:		Tor L|nnestad

Id:             $Id$
________________________________________________________________
*/

#ifndef FUNCPROTO
int histoSpecify(b1, b2, histo)
  IBAND b1, b2;
  histogram histo;
#else /* FUNCPROTO */
int histoSpecify(IBAND b1, IBAND b2, int *histo)
#endif /* FUNCPROTO */
{
  int i,j, s1, ant, old, min;
  double m;
  histogram h1, h2, h3;

  if (Ipixtyp(b1) != Iu_byte_typ)
    return(Error(1, "histoNorm: Bad input pixel type\n"));
  if (Ipixtyp(b2) != Iu_byte_typ)
    return(Error(2, "histoNorm: Bad output pixel type\n"));

  mkHisto(b1, h1);
  mkCumHisto(h1, h1);
  mkCumHisto(histo, h2);
  m = (double) h1[255]/ (double) h2[255];
  for (i=0; i<=255; i++)
    h2[i] = (int) ( (double) h2[i] * m);
  h2[255] = h1[255];
  for (i=255; i>0; i--) {
    h1[i] -= h1[i-1];
    h2[i] -= h2[i-1];
  }
  j=0;
  old = 0;
  for (i=0; i<=255; i++) {
    if (h1[i] == 0) {
      h3[i] = j;
      continue;
    }
    s1 = 0;
    old = ant = h1[i];
    while (ant) {
      min = MIN(ant, h2[j]);
      ant -= min;
      s1 += ( j * min);
      h2[j] -= min;
      if (h2[j] == 0) j++;
    }
    h3[i] = s1/old;
  }
  histoTransf(b1, b2, h3);
  return(0);
}








