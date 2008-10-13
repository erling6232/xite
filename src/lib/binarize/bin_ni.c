

/*

________________________________________________________________

        bin_ni.c
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

/*F:bin_ni*

________________________________________________________________

                bin_ni
________________________________________________________________


Name:           bin_ni - binarize a gray-scale band using 
                Niblack`s method

Syntax:         | #include <xite/binarize.h>
                |
                | int bin_ni( int region_size, double weight,
                |    IBAND inband, IBAND outband );

Description:    The input band 'inband' is binarized using the method of
                Niblack. The result is returned in 'outband'.

		The idea of this method is to vary the threshold over the
		image, based on the local mean and local standard deviation.
		The threshold at pixel (x,y) is calculated as

		T(x,y) = 'weight' * sigma(x,y) + mu(x,y)

		where sigma(x,y) and mu(x,y) are the estimated standard
		deviation and mean value in a local neighborhood of (x,y) of
		size ('region_size' x 'region_size'). The value of 'weight' is
		used to adjust how much of the foreground object edges that
		are taken as a part of the object. A recommended value for
		'weight' is -0.2 for images with black foreground objects, and
		'weight' = +0.2 for images with white foreground objects.

Restrictions:   'inband' and 'outband' must have pixel type unsigned byte.

See also:       binarize(1), stvar(3)

References:     &'Wayne Niblack'
                "An introduction to Digital image processing",
		pp 115-116, Prentice Hall, 1986.
Return value:   | 0 : ok
                | 1 : Bad input pixel type
		| 2 : Bad output pixel type
Files:  	src/binarize/bin_ni.c
Author:         Øivind Due Trier
Id:             $Id$
________________________________________________________________

*/


#include <math.h>
#include <stdlib.h>
#include <xite/includes.h>
#include <xite/blab.h>
#include <xite/biff.h>
#include <xite/histo.h>
#include <xite/stdiff.h>
#include <xite/binarize.h>
#include <xite/message.h>

/*
#include "BinM.h"
*/


int bin_ni (int region_size, double weight,
	     IBAND inband, IBAND outband)
{
  int xsize, ysize;

  if (region_size%2 == 0)
    region_size++;

  xsize = Ixsize(inband);
  ysize = Iysize(inband);
  
  if (Ipixtyp(inband) != Iu_byte_typ)
    return(Error(1, "bin_ni: Input pixel type must be unsigned byte.\n"));
  if (Ipixtyp(outband) != Iu_byte_typ)
    return(Error(2, "bin_ni: Output pixel type must be unsigned byte.\n"));

  stvar (inband, outband, region_size, region_size, weight, TRUE);

  return 0; /* Unix commands should return 0 */
}
