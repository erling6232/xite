

/*

________________________________________________________________

        shift_img.c
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
#include <xite/blab.h>
#include <xite/biff.h>
#include <xite/geometry.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include XITE_MALLOC_H
#include XITE_STDIO_H

/*P:shift_img*

________________________________________________________________

                shift_img
________________________________________________________________

Name:           shift_img - Set new origo on a BIFF image

Syntax:         | shift_img <infile> <outfile> <xshift> <yshift>

Description:    Set new origo on a BIFF image. Pixels outside the
                new coordinate system are wrapped around.
                Works on all pixel types. 

Return value:   0 if success, nonzero otherwise

See also:       shift_band(3), transpose_img(1), affine(1), quadratic(1),
                reorganize(1), rotate(1), warp(1)

Author:         Otto Milvang

Id:             $Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE img;
  int bn;
  char *args;

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <infile> <outfile> <xshift> <yshift>\n"));

  Iset_abort(TRUE);
  Iset_message(TRUE);

  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);

  if (argc != 5) Usage(2, "Illegal number of arguments.\n");

  img = Iread_image(argv[1]);

  for (bn=1; bn LE Inbands(img); bn++)
    shift_band(img[bn], img[bn], atoi(argv[3]), atoi(argv[4]));

  Ihistory(img, argv[0], args);
  Iwrite_image(img, argv[2]);

  return(0);
}
