
/*

________________________________________________________________

        scaled2col
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
#include <xite/color.h>
#include <xite/histo.h>

/*F:scaled2col*

________________________________________________________________

		scaled2col
________________________________________________________________

Name:		scaled2col - convert scaled cumulative histogram

Syntax:         | #include <xite/histo.h>
                |
                | void scaled2col( histogram scHisto,
                |    Color_tab colTab );
Description:    Make a color table according to a scaled
                cumulative histogram.

                | scHisto - input scaled cumulative histogram
                | colTab - output color table

Return value:

Author:		Tor L|nnestad

Id:             $Id$
________________________________________________________________
*/

#ifndef FUNCPROTO
void scaled2col(scHisto, colTab)
  histogram scHisto;
  Color_tab colTab;
#else /* FUNCPROTO */
void scaled2col(int *scHisto, Color_cell *colTab)
#endif /* FUNCPROTO */
{
  int n;
  FOR (n=0; n LE 255; n++)
    colTab[n].pixel = n;
    colTab[n].red   = scHisto[n]*256;
    colTab[n].green = scHisto[n]*256;
    colTab[n].blue  = scHisto[n]*256;
  ENDFOR;
}

