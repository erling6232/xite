
/*

________________________________________________________________

        scaleHisto
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
#include <xite/histo.h>

#ifndef MAX
# define MAX(a,b) (((a) > (b)) ? (a) : (b))
#endif


/*F:scaleHisto*

________________________________________________________________

		scaleHisto
________________________________________________________________

Name:		scaleHisto - scale a cumulative histogram

Syntax:         | #include <xite/histo.h>
                |
                | void scaleHisto( histogram cumHisto,
                |    histogram scHisto, int range );
Description:    Scale a cumulative histgram for use as histogram
		transformation. 'scale' implies that all values
		are linearly scaled to the interval [0..<range>].
		Identical in- and out-parameter is legal.

                | cumHisto - input cumulative histogram
                | scHisto  - output scaled cumulative histogram
		| range    - Number of levels in scHisto, typically 256
Return value:
Author:		Tor L|nnestad & Otto Milvang
Id:             $Id$
________________________________________________________________
*/

#ifndef FUNCPROTO
void scaleHisto(cumHisto, scHisto, range)
  histogram cumHisto, scHisto;
  int range;
#else /* FUNCPROTO */
void scaleHisto(int *cumHisto, int *scHisto, int range)
#endif /* FUNCPROTO */
{
  int old, new, h, n=0;
  double mult, frange;
  frange = range;
  while (cumHisto[n] EQ 0) scHisto[n++] = 0;
  old = cumHisto[n];
  mult =  (double)(MAX(cumHisto[255]*2,2));

  FOR ( ; n LE 255; INC n)
    new = cumHisto[n];
    h = (int)((double)(old+new) * frange / mult);
    old = new;
    if (h>=range) h = range-1;
    scHisto[n] = h;
  ENDFOR
}




