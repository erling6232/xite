
/*

________________________________________________________________

        mkCumHisto
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

/*F:mkCumHisto*

________________________________________________________________

		mkCumHisto
________________________________________________________________

Name:		mkCumHisto - make a cumulative histogram

Syntax:         | #include <xite/histo.h>
                |
                | void mkCumHisto( histogram histo,
                |    histogram cumHisto );
Description:    make a cumulative histogram based on a histogram.
                The output histogram is not scaled. To scale it,
		call scaleHisto. Identical in and out param are
                legal.

                | histo    - input histogram
                | cumHisto - output cumulative histogram
Return value:

Author:		Tor L|nnestad
________________________________________________________________
*/

#ifndef FUNCPROTO
void mkCumHisto(histo, cumHisto)
  histogram histo, cumHisto;
#else /* FUNCPROTO */
void mkCumHisto(int *histo, int *cumHisto)
#endif /* FUNCPROTO */
{
  int n;
  cumHisto[0] = histo[0];
  for (n=1; n LE 255; n++)
    cumHisto[n] = cumHisto[n-1] + histo[n];
}

