

/*C*

________________________________________________________________

        lapZeroCross
        $Id$
        Copyright 1991, Blab, UiO
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
#include <stdlib.h>
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#include <xite/blab.h>
#include <xite/convolve.h>
#include <xite/readarg.h>
#include <xite/message.h>



#ifndef MAIN

/*F:lapZeroCross*

________________________________________________________________

		lapZeroCross
________________________________________________________________

Name:		lapZeroCross - find zero crossing of lapacian

Syntax:         | #include <xite/convolve.h>
		|
                | BiffStatus lapZeroCross( IBAND iband,
                |    IBAND oband, int a, int b, int c,
                |    double scale, double offset, int v );

Description:	'lapZeroCross' finds the zero crossing of the laplacian
		of the input band. The routine starts by calling the
		laplace operator, followed by calling the crossing routine.

		The output is zero where the laplacian has no zero
		crossing, and proportional to the edge strength where
		the laplacian has a zero crossing.

		The arguments 'a', 'b', 'c', 'scale', 'offset' and 'v' are as
		described for the laplace operator. Typical values are
		'a' = 0, 'b' = -1, 'c' = 4, 'scale = 1.0', 'offset' = 127.0.

		The whole algorithm is:

		| <Calculate laplacian>;
		| FOR <every pixel (x,y)>:
		| IF <in(x,y) have zero crossing>
		|   out(x,y) = MAX(ABS(in(x,y)-in(x+1,y)), \\
		|                  ABS(in(x,y)-in(x,y+1)))
		| ELSE
		|   out(x,y) = 0

Restrictions:	'iband' and 'oband' must have pixel type unsigned byte.

See also:	lapZeroCross(1), laplace(3), gradient(3), grad(3), roberts(3),
                sobel(3), prewitt(3), contour(3), crossing(3)

Return value:	| 0 => ok
		| 1 => bad pixel type input band
		| 2 => bad pixel type output band
		| 3 => some error in crossing

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Id: 		$Id$
________________________________________________________________

*/

BiffStatus lapZeroCross(IBAND iband, IBAND oband, int a, int b, int c, double scale, double offset, int v)
{
  if (Ipixtyp(iband) NE Iu_byte_typ)
    return(Error(1,
		 "lapZeroCross: Input pixel type must be unsigned byte.\n"));
  if (Ipixtyp(oband) NE Iu_byte_typ)
    return(Error(2,
		 "lapZeroCross: Output pixel type must be unsigned byte.\n"));

  laplace(iband, oband, a, b, c, scale, offset, v);

  if (crossing(oband, oband, (int)offset))
    return(Error(3,
		 "lapZeroCross: Error in crossing().\n"));

  return(0);
}

#endif /* not MAIN */

/*P:lapZeroCross*

________________________________________________________________

		lapZeroCross
________________________________________________________________

Name:		lapZeroCross - find zero crossing of lapacian

Syntax:		lapZeroCross [<option>...] <inimage> <outimage>

Description:	'lapZeroCross' finds the zero crossing of the laplacian
		of the input image. The program starts by performing
		the laplace operator, followed by calling the crossing
		routine.

		The output is zero where the laplacian has no zero
		crossing, and proportional to the edge strength where
		the laplacian has a zero crossing.

		The whole algorithm is:

		| <Calculate laplacian>;
		| FOR <every pixel (x,y)>:
		| IF <in(x,y) have zero crossing>
		|   out(x,y) = MAX(ABS(in(x,y)-in(x+1,y)), \\
		|                  ABS(in(x,y)-in(x,y+1)))
		| ELSE
		|   out(x,y) = 0

Restrictions:   'inimage' must have bands with pixel type unsigned byte.

Options:        &-a a
                Laplace weight in 8-neighbors. Default 0.

		&-b b
		Laplace weight in 4-neighbors. Default -1.

		&-c c
		Laplace weight in center. Default 4.

		&-scale s
		Laplace output scaling factor. Default 1.0.

		&-offset o
		Laplace output offset and crossing level. Default 127.0.

See also:	lapZeroCross(3), laplace(1), gradient(1), grad(1), roberts(1),
                sobel(1), prewitt(1), contour(1), crossing(1)

Return value:	| 0 => ok
                | 1 => bad number of arguments
		| 2 => Can't open input file
		| 3 => Can't open output file

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Examples:	lapZeroCross -a -1 -b -2 -c 12 -scale 0.5 mona.img monazc.img

Id: 		$Id$
________________________________________________________________

*/

#ifdef MAIN

int main(int argc, char **argv)
{
  IMAGE img1, img2;
  double scale, offset;
  int nbands, bn, a, b, c;
  char *args;

  Iset_message(TRUE);
  Iset_abort(TRUE);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [<options>] <inimage> <outimage>\n"));

  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);

  a      = read_iswitch(&argc, argv, "-a", 0);
  b      = read_iswitch(&argc, argv, "-b", -1);
  c      = read_iswitch(&argc, argv, "-c", 4);
  scale  = read_dswitch(&argc, argv, "-scale", 1.0);
  scale  = read_dswitch(&argc, argv, "-s", scale);
  offset = read_dswitch(&argc, argv, "-offset", 127.0);
  offset = read_dswitch(&argc, argv, "-o", offset);

  if (argc != 3) Usage(1, "Illegal number of arguments.\n");

  img1   = Iread_image(argv[1]);
  nbands = Inbands(img1);
  if (NOT img1) Error(2, "Error in reading image.\n");
  img2 = Icopy_init(img1);
  if (NOT img2) Error(2, "Error in copying image.\n");

  for (bn=1; bn LE nbands; bn++)
    if (lapZeroCross(img1[bn], img2[bn], a, b, c, scale, offset, Verbose()))
      Error(2, "Error in lapZeroCross() for band %d.\n", bn);

  Ihistory(img2, argv[0], args);
  Iwrite_image(img2, argv[2]);

  return(0);
}

#endif /* MAIN */
