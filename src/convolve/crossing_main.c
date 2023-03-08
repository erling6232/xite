

/*C*

________________________________________________________________

        crossing
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
#include <xite/convolve.h>
#include XITE_STDIO_H
#include <xite/blab.h>
#include <xite/readarg.h>
#include <xite/message.h>

#ifndef MIN
# define MIN(a,b) (((a)<(b)) ? (a) : (b))
#endif



/*P:crossing*

________________________________________________________________

		crossing
________________________________________________________________

Name:		crossing - find zero crossing or any other crossing

Syntax:		crossing [-t <t>] <inimage> <outimage>

Description:	Can be used to find zero crossing of second derivative,
		laplace. Can also be used to determine any other kind
		of crossing. Pixels below or equal to 't' are set if the pixel
		to the right or below is above 't', pixels above 't' are
		set if the pixel to the right or below is below or equal to
		't'. Pixels that are "set" are set to the maximum absolute
		difference of the two neighbours, to indicate the strength
		of the edge.

		| IF (crossing t)
		|   out(x,y) = MAX(ABS(in(x,y)-in(x+1,y)), 
		|                  ABS(in(x,y)-in(x,y+1)))
		| ELSE
		|   out(x,y) = 0

Restrictions:   'inimage' must have bands with pixel type unsigned byte.

Options:        &-t t
                Crossing value. Default is 127.

See also:	crossing(3), contour(1)

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Examples:	crossing -t 100 mona.img monacross.img

Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE img;
  int t, nbands, bn;
  char *args;

  Iset_message(TRUE);
  Iset_abort(TRUE);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [<options>] <inimage> <outimage>\n"));

  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);

  t = read_iswitch(&argc, argv, "-t", 127);

  if (argc != 3) Usage(1, "Illegal number of arguments.\n");

  img = Iread_image(argv[1]);
  if (!img) Error(2, "Error in reading image.\n");
  nbands = Inbands(img);

  for (bn=1; bn LE nbands; bn++) {
    if (crossing(img[bn], img[bn], t))
      Error(2, "Error in crossing() while processing band %d.\n", bn);
  }

  Ihistory(img, argv[0], args);
  Iwrite_image(img, argv[2]);

  return(0);
}
