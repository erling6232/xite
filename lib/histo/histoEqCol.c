
/*

________________________________________________________________

        histoEqCol
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



#include <stdlib.h>
#include <xite/includes.h>
#include <xite/biff.h>
#include <xite/blab.h>
#include <xite/color.h>
#include <xite/histo.h>
#include <xite/message.h>

/*F:histoEqCol*

________________________________________________________________

		histoEqCol
________________________________________________________________

Name:		histoEqCol - histogram equalization color table

Syntax:         | #include <xite/histo.h>
                |
                | void histoEqCol( IBAND b, Color_tab ct,
                |    int graylevels );
Description:    Make a color table doing histogram  equalization
                of a band. Assuming byte pixels.

                | b  - input band
                | ct - output color table
                | graylevels : max number of levels in color table

Return value:
Author:		Tor L|nnestad
Id:             $Id$
________________________________________________________________
*/

void histoEqCol(IBAND b, Color_cell *ct, int graylevels)
{
  histogram histo;
  mkHisto(b,histo);
  mkCumHisto(histo,histo);
  scaleHisto(histo,histo, graylevels);
  scaled2col(histo,ct);
}
