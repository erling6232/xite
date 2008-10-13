

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
#ifdef HAVE_STDIO_H
 #include <stdio.h>
#endif
#include <stdlib.h>
#include <xite/readarg.h>
#include <xite/binarize.h>
#include <xite/message.h>
#include "BinM.h"
/* #include "write_band.c" */

/*F:expand_edge*

________________________________________________________________

		expand_edge
________________________________________________________________

Name:		expand_edge - Expand image by edge mirroring.

Syntax:         | #include <xite/binarize.h>
                |
                | int expand_edge( IBAND inband, int zero_expand,
                |    int even_expand, int expand_depth,
                |    IBAND outband );

Description:    'expand_edge' is used on a band to avoid some types
                of edge difficulties.

                Expands 'inband' by 'expand_depth' pixels on all four edges,
                and writes the result to 'outband'. If the dimensions
		of 'inband' are n x m, then the dimensions of 'outband'
		will be (n + 2*size) x (m + 2*size).

		Imagine placing mirrors on the edges of 'inband'. These
		mirror bands are mapped to 'outband'. Using 'expand_depth'
		= 3 and 'even_expand' == 1, the result will be like this:

		|        inband:                          outband:
		|
       	        |                            ihg|ghi               xyz|zyx
                |                            fed|def     . . .     uvw|wvu
	        |                            cba|abc               rst|tsr
		| +------- - - - -------+    ---+------- - - - -------+---
                | |abc               rst|    cba|abc               rst|tsr
                | |def     . . .     uvw|    fed|def     . . .     uvw|wvu
                | |ghi               xyz|    ihg|ghi               xyz|zyx
		| |                     |       |                     |
                | .                     .       .                     .
                | .                     .       .                     .
                | .                     .       .                     .
		|

		 If 'zero_expand' == 1, the new pixels are set to 0 instead.
		 Else, if 'zero_expand' <= -1, then the new pixels are set to
		 '-zero_expand'. To expand a binary image with a white
		 border, use 'zero_expand' = -255.
		 If 'even_expand' == 0, the pixels closest to the edge are
		 not mirrored/repeated.
		 `expand_depth' specifies the width in pixels of the
		 sections of 'inband' to be mirrored.

Restrictions:   'inband' and 'outband' must have pixel type unsigned byte.
                `expand_depth' must not exceed the smallest of the X dimension
		and the Y dimension of 'inband'.

Return value:   | 0  => ok
                | 1  => Bad pixel type
		| 2  => parameter 'expand_depth' too large.
		| 3  => outband too small.

See also:       expand_edge(1)

Files:		src/binarize/expand_edge.c
Author:		Øivind Due Trier, BLAB, Ifi, UiO.

Id:             $Id$
________________________________________________________________

*/

int expand_edge(IBAND inband, int zero_expand, int even_expand, int expand_depth, IBAND outband)
{
  int x, y;
  int xsize, ysize;
  int low_x_offset;
  int low_y_offset;
  int high_x_offset;
  int high_y_offset;

  if (Ipixtyp(inband) != Iu_byte_typ)
    return(Error(1, "expand_edge: Input pixel type must be unsigned byte.\n"));
  if (Ipixtyp(outband) != Iu_byte_typ)
    return(Error(1,
		 "expand_edge: Output pixel type must be unsigned byte.\n"));

  xsize = Ixsize(inband);
  ysize = Iysize(inband);

  if (expand_depth > xsize || expand_depth > ysize)
    return(Error(2, "expand_edge: expand_edge too large.\n"));

  if (Ixsize(outband) < xsize + 2*expand_depth ||
      Iysize(outband) < ysize + 2*expand_depth)
    return(Error(3, "expand_edge: outband too small.\n"));

  if (zero_expand == 1) {
    for (x=1 - expand_depth; x<=xsize + expand_depth; x++)
      for (y=1 - expand_depth; y<=ysize + expand_depth; y++)
	if ((x >= 1) AND (x <= xsize) AND (y >= 1) AND (y <= ysize))
	  outband[y + expand_depth][x + expand_depth]
	    = inband[y][x];
	else
	  outband[y + expand_depth][x + expand_depth] = 0;
  } else if (zero_expand <= -1) {
    for (x=1 - expand_depth; x<=xsize + expand_depth; x++)
      for (y=1 - expand_depth; y<=ysize + expand_depth; y++)
	if ((x >= 1) AND (x <= xsize) AND (y >= 1) AND (y <= ysize))
	  outband[y + expand_depth][x + expand_depth]
	    = inband[y][x];
	else
	  outband[y + expand_depth][x + expand_depth] = -zero_expand;
  } else {
    if (even_expand) {
      low_y_offset  = expand_depth + 1;
      high_y_offset = ysize + 1;
      low_x_offset  = 2*expand_depth + 1;
      high_x_offset = expand_depth + xsize + 1;
    } else {
      low_y_offset  = expand_depth + 2;
      high_y_offset = ysize;
      low_x_offset  = 2*expand_depth + 2;
      high_x_offset = expand_depth + xsize;
    }

    for (x=1; x<=xsize; x++) {
      for (y=1; y<=expand_depth; y++) 
	outband[y][x + expand_depth] 
	  = inband[low_y_offset-y][x];
      for (y=1; y<=ysize; y++)
	outband[y+expand_depth][x + expand_depth]
	  = inband[y][x];
      for (y=1; y<=expand_depth; y++)
	outband[expand_depth+ysize+y][x + expand_depth]
	  = inband[high_y_offset-y][x];
    }	
    for (y=1; y<=expand_depth + ysize + expand_depth; y++) {
      for (x=1; x<=expand_depth; x++)
	outband[y][x] 
	  = outband[y][low_x_offset - x];
      for (x=1; x<=expand_depth; x++)
	outband[y][expand_depth + xsize + x]
	  = outband[y][high_x_offset - x];
    }	
  }

  return Iok;

} /* expand_edge() */
