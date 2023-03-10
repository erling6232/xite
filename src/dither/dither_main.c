
/*

________________________________________________________________

        dither
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
#include <xite/dither.h>
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/message.h>

#ifndef MIN
# define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

/*P:dither*

________________________________________________________________

		dither
________________________________________________________________

Name:		dither - create a dithering image

Syntax:		dither <input image> <output image> [threshold]

Description:    'dither' dithers an image for better display on BW
                screens. <threshold> is a threshold-like argument,
		default is 127. Larger (smaller) value gives darker
		(brighter) image.

Files:	
See also:	
Diagnostics:

Restrictions:   Accepts only UNS_BYTE pixels.

Return value:

Author:		Tor L|nnestad, BLAB, IfI, UiO

Examples:       | dither mona.img monadith.img 
                | dither mona.img monadith.img 100

Id:             $Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE i;
  int bn, t;
  char arg[30];

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <oldfile> <newfile> [threshold]\n"));
  Iset_message(TRUE);
  Iset_abort(TRUE);
  if (argc == 1) Usage(1, (char*)0);
  if ((argc LT 3) OR (argc GT 4)) Usage(2, "Bad number of arguments\n");

  i = Iread_image(argv[1]);
  if (argc GE 4) t = atoi(argv[3]); else t = 127;
  for (bn=1; bn LE Inbands(i); bn++)
    if(dither(i[bn], i[bn], t))
      Warning(3, "Bad arguments in band %d\n", bn);

  sprintf(arg, "threshold : %d", t);
  Ihistory(i, argv[0], arg);
  Iwrite_image(i, argv[2]);
   return(0);
}
