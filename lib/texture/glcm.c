/*C*


________________________________________________________________

        glcm
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


#include <math.h>
#include <stdlib.h>
#include <xite/includes.h>
#include <xite/biff.h>
#include <xite/texture.h>
#include <xite/blab.h>
#include <xite/histo.h>
#include <xite/draw.h>
#include <xite/readarg.h>
#include <xite/message.h>
#include XITE_STDIO_H

/*F:glcm*

________________________________________________________________

		glcm
________________________________________________________________

Name:		glcm - Computes Grey Level Cooccurrence Matrix 

Syntax:         | #include <xite/texture.h>
		|
                | int glcm( IBAND input, ID_BAND output, int dx,
                |    int dy, int num_greylevel );

Description:    Computes the Grey Level Cooccurence Matrix of an image. The
		angle and the displacement will be computed from the given
		'dx' and 'dy'. 'dx' and 'dy' can be negative.

		All the pixel values in the input band will be increased by
		one prior to computing the GLCM (though not changing the
		contents of the input band). This means that if there is a
		greylevel combination (0,p) in the input band, then the value
		at (1,p+1) in the output band will be increased.

		Note that the number of gray levels is not set to
		'num_graylevel'. This parameter is merely used to check
		whether 'input' contains too large pixel values, and a
		warning is given to this effect. If a reduction of the
		number of gray levels is desired, this should be taken care
		of prior to calling this function.

Restrictions:   'input' must have pixel type unsigned byte. 'output' must
                have pixel type double.

Return value:   | 0 => ok
                | Warning:
                |   1 => there are grey levels > "num_greylevel" in 
                |        the input band
		| Errors:
		|   2 => input is not unsigned byte
                |   3 => output is not double type
                |   4 => horizontal size of output is less than
		|        "num_greylevel"
                |   5 => vertical size of output is less than
		|        "num_greylevel"

Reference:      &R. M. Haralick, K. Shanmugam, and I. Dinstein
                "Textural Features for Image Classification",
                IEEE Trans. on Systems, Man and Cybernetics,
                Vol. SMC-3, pp. 610-621, 1973.

Author:		Yogesan K., BLAB, Ifi, UiO

Id: 		$Id$
________________________________________________________________

*/

int glcm(IBAND input, ID_BAND output, int dx, int dy, int num_greylevel)
{
  int x, y, pv1, pv2, stat=0;
  int xstart, ystart, xstop, ystop;
  
  if (Ipixtyp(input) != Iu_byte_typ)
    return(Error(2, "Input pixel type must be unsigned byte.\n"));
  if (Ipixtyp((IBAND) output) != Idouble_typ)
    return(Error(3, "Output pixel type must be double.\n"));
  if (Ixsize((IBAND) output) < num_greylevel)
    return(Error(4, "Horizontal output size smaller than num_greylevel.\n"));
  if (Iysize((IBAND) output) < num_greylevel)
    return(Error(5, "Vertical output size smaller than num_greylevel.\n"));

  xstart = (dx>=0) ? 1 : (-dx + 1);
  ystart = (dy>=0) ? 1 : (-dy + 1);
  xstop  = (dx>=0) ? Ixsize(input)-dx : Ixsize(input);
  ystop  = (dy>=0) ? Iysize(input)-dy : Iysize(input);
 
  for (y=ystart; y <= ystop; y++)
    for (x=xstart; x <= xstop; x++) {
      pv1 = input[y][x] + 1;
      pv2 = input[y+dy][x+dx] + 1;
      if ((pv1 > num_greylevel) || (pv2 > num_greylevel)) stat = 1;
      else output[pv1][pv2]++; 
    }

  if (!stat) return(stat);
  else return(Warning(stat, "%s%d%s\n%s\n",
		      "There are grey levels > ", num_greylevel,
		      " in input band.",
		      "The GLCM is not updated for these levels."));
}
