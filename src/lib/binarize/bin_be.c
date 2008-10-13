

/*

________________________________________________________________

        bin_be.c
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

/*F:bin_be*

________________________________________________________________


________________________________________________________________


Name:           bin_be - binarize a gray-scale band using 
                Bernsen`s method

Syntax:         | #include <xite/binarize.h>
                | int bin_be( int region_size,
                |    int contrast_limit, int set_doubt_to_low,
                |    IBAND inband, IBAND outband );

Description:    The input band 'inband' is binarized using 
                the method of Bernsen, the result returned
		in 'outband'.

		For each pixel, the threshold
		
		T(x,y) = (Z_low + Z_high)/2

		is used, where Z_low and Z_high are the lowest and highest
		gray level pixel values in a square neighborhood of size
		('region_size' x 'region_size') about the pixel (x,y).
		However, if the contrast measure

		C(x,y) = Z_high - Z_low < 'contrast_limit'
		
		the pixel is set to 255 if 'set_doubt_to_low' == 0, and to 0
		if 'set_doubt_to_low' >= 1.
		
Files:  	src/binarize/bin_be.c
See also:       binarize(1), thresBernsen(1), thresBernsen(3)

References:     &'John Bernsen'
                "Dynamic thresholding of grey-level images", 
		Proc. 8th International Conference on Pattern 
		Recognition (ICPR8), pp 1251-1255, Paris, France, 
		October 1986.
Return value:   | 0 : ok
                | 1 : Bad input pixel type
		| 2 : Bad output pixel type
Restrictions:   'inband' and 'outband' must have pixel type unsigned byte.
Author:         Oivind Due Trier
Examples:       
Id:             $Id$
________________________________________________________________

*/



#include <math.h>
#include <stdlib.h>
#include <xite/includes.h>
#include <xite/blab.h>
#include <xite/biff.h>
#include <xite/histo.h>
#include <xite/threshold.h>
#include <xite/binarize.h>
#include <xite/message.h>
#include "BinM.h"


int bin_be (int region_size, int contrast_limit, 
	    int set_doubt_to_low, 
	    IBAND inband, IBAND outband)
{

  if (region_size%2 == 0)
    region_size++;

  if (Ipixtyp(inband) != Iu_byte_typ)
    return(Error(1, "bin_be: Input pixel type must be unsigned byte.\n"));
  if (Ipixtyp(outband) != Iu_byte_typ)
    return(Error(2, "bin_be: Output pixel type must be unsigned byte.\n"));

  thresBernsen (inband, outband, contrast_limit, region_size, region_size, 
		BIN_FOREGROUND, BIN_BACKGROUND, set_doubt_to_low);

  
  return 0; /* Unix commands should return 0 */
}
