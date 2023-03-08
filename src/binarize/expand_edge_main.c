

/*

________________________________________________________________

        expand_edge.c
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
#include <xite/blab.h>
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/readarg.h>
#include <xite/binarize.h>
#include <xite/message.h>
/* #include "BinM.h" */
/* #include "write_band.c" */

/*P:expand_edge*

________________________________________________________________

		expand_edge
________________________________________________________________

Name:		expand_edge -  Expand image by edge mirroring.

Syntax:		expand_edge [-0] [-e] [-o] [-z <value>] [-p <size>]
                <inimage> <outimage>    

Description:    expand_edge is used on an image to aviod some types
                of edge difficulties.

                Expands 'inimage' by 'size' pixels on all four edges,
                and writes the result to 'outimage'. If the dimensions
		of 'inimage' is n x m, then the dimensions of outimage
		will be (n + 2*size) x (m + 2*size).

		Imagine placing mirrors on the edges of 'inimage'. These
		mirror images are mapped to outimage'. Using the default
		setting, which is equivalent to specifying '-e -p 3',
		the result will be like this:

		|       inimage                          outimage

       	                                      ihg|ghi               xyz|zyx
                                              fed|def     . . .     uvw|wvu
	                                      cba|abc               rst|tsr
		 +------- - - - -------+      ---+------- - - - -------+---
                 |abc               rst|      cba|abc               rst|tsr
                 |def     . . .     uvw|      fed|def     . . .     uvw|wvu
                 |ghi               xyz|      ihg|ghi               xyz|zyx
		 |                     |         |                     |

Options:	&-0
                The new pixels are set to 0 instead of being mirror images.

		&-z value
		The new pixels are set to -'value' instead of being mirror
		images.

		&-e
		Even expansion. 

		&-o
		Odd expansion. The pixels closest to the edge are not mirrored.

		&-p size
		'size' specifies the width in pixels of the expanded edge.

Restrictions:   'inimage' must have pixel type unsigned byte.
                'size' must not exceed the smallest of the X dimension
		and the Y dimension of 'inimage'.
		If both -e and -o options are specified, -e (the default)
		is chosen.

See also:       expand_edge(3)

Author:		Øivind Due Trier, BLAB, Ifi, UiO

Examples:       expand_edge  mona.img mona_expand.img 
                expand_edge -0 -o -p 2 mona.img mona_zeroexpand.img 
                expand_edge -z -255 -p 1 5.binary.img 5.binary_expand.img

Id:             $Id$
________________________________________________________________

*/

int main(int argc, char *argv[])
{
  
  IMAGE inimage, outimage;
  IBAND inband, outband;
  int xsize, ysize;
  char *infile;
  char *outfile;
  int zero_expand;
  int even_expand;
  int expand_depth;
  int error_code;


  Iset_message(TRUE);
  Iset_abort(TRUE);
  InitMessage(&argc, argv, xite_app_std_usage_text(
   "Usage: %s [-0] [-e] [-o] [-z <value>] [-p <size>] <infile> <outfile> \n"));
      
  /* Parse command line parameters */

  if (argc == 1) Usage(1, NULL);
  if (argc < 3) {
    Usage(2, "Wrong number of arguments.\n");
    exit(1);
  }
  zero_expand = read_bswitch(&argc, argv, "-0");
  zero_expand = read_iswitch(&argc, argv, "-z", zero_expand);
  even_expand = TRUE;
  if (read_bswitch(&argc, argv, "-o"))
    even_expand = FALSE;
  if (read_bswitch(&argc, argv, "-e"))
    even_expand = TRUE;

  expand_depth = atoi(read_switch(&argc, argv, "-p", 1, "3"));

  infile  = argv[argc-2];
  outfile = argv[argc-1];

  inimage = Iread_image(infile);
  inband = inimage[1];
  xsize = Ixsize(inband);
  ysize = Iysize(inband);
  outimage = Imake_image(1, "edge_expanded", Iu_byte_typ,
			 xsize + 2*expand_depth, ysize + 2*expand_depth);
  outband = outimage[1];

  /* if (Verbose()) printf("Initialization done\n"); */

  error_code = expand_edge(inband, zero_expand, even_expand, expand_depth,
			   outband);
/* 
  if (Verbose()) printf("Image is expanded, error code: %i\n", error_code);
*/
  
  if (error_code == Iok)
    error_code = Iwrite_image(outimage, outfile);

  /* if (Verbose()) printf("Image written, error code: %i\n", error_code); */

  return 0; /* Unix commands should return 0 */
}
