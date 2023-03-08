

/*C*

________________________________________________________________

        thn_zs.c
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
#include <xite/binarize.h>
#include <xite/thin.h>

/*P:thn_zs*

________________________________________________________________

		thn_zs
________________________________________________________________

Name:		thn_zs -  thin a binary image using algorithm by Zhang
                and Suen.

Syntax:		thn_zs <inimage> <outimage>

Description:	Thinning a binary image with algorithm by Zhang and Suen[1]

Restrictions:	'inimage' must have bands with pixel type unsigned byte.
                The input is assumed to be binary valued with foreground = 0,
		background = 255.

See also:       thn_zs(3), thn_har(1), thn_lc(1)

References:     &[1] T. Y. Zhang and C. Y. Suen,
                "A Fast Parallel Algorithm for Thinning Digital Patterns.",
		Comm. ACM, vol. 27, no. 3, pp 236-239, 1984.

                &[2] Rafael C. Gonzalez and Paul Wintz,
		"Digital Image Processing.",
		2. edition, 1987, pp. 398-402.

Files:		src/binarization/thn_zs.c
Id: 		$Id$
________________________________________________________________

*/

int main (int argc, char *argv[])
{
   char  *infile;
   char  *outfile, *args;
   int   xsize, ysize;
   IMAGE inimage, outimage;
   IBAND inband, outband;
   
   Iset_message(TRUE);
   Iset_abort(TRUE);

   InitMessage(&argc, argv, xite_app_std_usage_text(
   "Usage: %s <inimage> <outimage>\n"));
   
   /* Parse command line parameters */
   
   if (argc == 1) Usage(1, NULL);
   args = argvOptions(argc, argv);
   if (argc < 3) Usage(2, "Illegal number of arguments.\n");
   
   infile  = argv[argc-2];
   outfile = argv[argc-1]; 
   
   inimage = Iread_image(infile);
   inband  = (IBAND)inimage[1]; /* First band only is used. */
   
   xsize   = Ixsize(inband);
   ysize   = Iysize(inband);
   
   /* Make data structure for output image */
   
   outimage = Imake_image(1, outfile, Iu_byte_typ, xsize, ysize);
   outband  = (IBAND) outimage[1];
   
   if (thn_zs(Verbose(), inband, outband))
     return(Error(3, "Error in thn_zs.\n"));

   Ihistory(outimage, argv[0], args);
   Iwrite_image(outimage, outfile);

   return 0; /* Unix commands should return 0 */

} /* END main() */
