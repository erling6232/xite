
/*C*

________________________________________________________________

        thn_har.c
        $Id$
        Copyright 1994, Blab, UiO
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
#include <stdlib.h>
#include <xite/includes.h>
#include XITE_STDIO_H
#include XITE_STRING_H
#include XITE_FCNTL_H
#include <xite/blab.h>
#include <xite/biff.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include <xite/negate.h>
#include <xite/thin.h>



/*P:thn_har*

________________________________________________________________

		thn_har
________________________________________________________________

Name:		thn_har - thinning of binary image using Haralick and 
                Shapiro''s morphological thinning.

Syntax:		thn_har <inimage> <outimage>

Description:    Morphological thinning according to Haralick and Shapiro.

Restrictions:   'inimage' must have bands with pixel type unsigned byte.
                The input is assumed to be binary valued with foreground = 0,
		background = 255.

See also:       thn_har(3), thn_lc(1), thn_zs(1)

Reference:	&R. M. Haralick and L. G. Shapiro,
                "Computer and Robot Vision",
                Vol. 1, Chapter 5 (especially 5.10.1),
		Addison-Wesley, Reading, Massachusetts, 1992,

Author:		Qian Huang (Michigan State University), 
                Øivind Due Trier (BLAB, Ifi, University of Oslo).
Id: 		$Id$
________________________________________________________________

*/

int main (int argc, char *argv[])
{
  int xsize, ysize;
  IBAND inband;
  IBAND outband;
  char *infilename, *outfilename, *args;
  IMAGE inimage;
  IMAGE outimage;

  Iset_message(TRUE);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <inimage> <outimage>\n"));

  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);
  if (argc < 3) Usage(2, "Illegal number of arguments.\n");

  infilename  = argv[argc-2];
  outfilename = argv[argc-1];
  inimage     = Iread_image(infilename);
  inband      = (IBAND)inimage[1]; /* First band only is used. */
  xsize       = Ixsize(inband);
  ysize       = Iysize(inband);
  outimage    = Imake_image(1, outfilename, Ipixtyp(inband), xsize, ysize);
  outband     = (IBAND) outimage[1];

  Icopy_band(inband, outband);
  if (thn_har(outband) != 0) Error(3, "Error in thn_har.\n");

  Ihistory(outimage, argv[0], args);
  Iwrite_image(outimage, outfilename);

  return 0; /* Unix commands should return 0 */

} /* END main() */
