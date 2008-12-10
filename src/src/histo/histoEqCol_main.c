
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

/*P:histoEqCol*

________________________________________________________________

		histoEqCol
________________________________________________________________

Name:		histoEqCol - histogram equalization color table

Syntax:		histoEqCol <input image> <output color table>

Description:    histoEqCol makes a color table based on the
                cumulative histogram of THE FIRST BAND of the
                input image. If you display the first band with
                the new color table, the effect will be histogram
                equalization of that band.

Files:	        biff.h histo.h color.h
See also:	
Diagnostics:

Restrictions:   first band of input image should have byte pixels.
Return value:

Author:		Tor Lønnestad

Examples:       histoEqCol mona.img monaEq.col
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE i;
  int k;
  Color_tab ct;

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <inimage> <output color table> [<graylevels>]\n"));
  Iset_message(TRUE);
  if(argc == 1) Usage(1, (char*)0);
  if ((argc LT 3) OR (argc GT 4))
    Usage(2, "Bad number of arguments\n");
  if (argc GE 4) k = atoi(argv[3]); else k = 256;

  i = Iread_image(argv[1]);
  histoEqCol(i[1],ct, k); /* only using band 1 !!! */
  color_write(argv[2], ct, "HistoEq", 256);
   return(0);
}
