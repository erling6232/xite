

/*C*

________________________________________________________________

        thn_lc.c
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
#include <stdlib.h>
#include <xite/includes.h>
#include XITE_STDIO_H
#include <xite/blab.h>
#include <xite/biff.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include <xite/thin.h>

/*P:thn_lc*

________________________________________________________________

		thn_lc
________________________________________________________________

Name:		thn_lc - Lee and Chen''s thinning method.

Syntax:		thn_lc <inimage> <outimage>

Description:	Thin a binary image with Lee and Chen''s method. Black (0) is
                foreground, and white (255) is background.

		This method is simply an extension of Zhang and Suen''s
		method. First, Zhang and Suen''s methhod is used. The skeleton
		thus obtained is not truly 8-connected, since some
		non-junction pixels have more than two neighbors, making the
		skeleton useless for algorithms that require this constraint.
		Therefore, some pixels have to be removed. The skeleton is
		inspected, and each pixel is tested using a lookup table. The
		result is a true 8-connected skeleton where only junction
		pixels have more than two 8-neighbors.

Restrictions:   'inimage' must have bands with pixel type unsigned byte.
                The input is assumed to be binary valued with foreground = 0,
		background = 255.

See also:	thn_lc(3), thn_har(1), thn_zs(1)

Reference:      &H.-J. Lee and B. Chen,
                "Recognition of handwritten chinese characters via short
		line segments,"
		Pattern Recognition,
		vol. 25, no. 5, pp. 543-552, 1992.

Files:		xite/src/thin/thn_lc.c
Author:		Øivind Due Trier, late one night at Michigan State University.
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
   
   if (thn_lc(Verbose(), inband, outband))
     return(Error(3, "Error in thn_lc.\n"));
   
   Ihistory(outimage, argv[0], args);
   Iwrite_image(outimage, outfilename);
   
   return 0; /* Unix commands should return 0 */

} /* END main() */
