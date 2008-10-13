

/*

________________________________________________________________

        bin_man.c
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


/*F:bin_man*

________________________________________________________________
 
                bin_man
________________________________________________________________


Name:           bin_man - binarize a gray-scale band using a manual threshold

Syntax:         | #include <xite/binarize.h>
                |
                | int bin_man( int threshold, IBAND inband,
                |    IBAND outband );

Description:    The input band 'inband' is binarized using 
                'threshold', the result returned in 'outband'.

See also:       binarize(1)

Return value:   | 0 : ok
                | 1 : Bad input pixel type
		| 2 : Bad output pixel type

Restrictions:   'inband' and 'outband' must have pixel type unsigned byte.

Author:         Øivind Due Trier
Files:          src/binarize/bin_man.c
Id:             $Id$
________________________________________________________________

*/

#include <xite/includes.h>
/* #include XITE_STDIO_H */
#include <math.h>
#include <xite/blab.h>
#include <xite/histo.h>
#include <xite/binarize.h>
#include <xite/message.h>


int bin_man (int threshold, IBAND inband, IBAND outband)
{
  int xsize, ysize;
  int x, y;

  if (Ipixtyp(inband) != Iu_byte_typ)
    return(Error(1, "bin_man: Input pixel type must be unsigned byte.\n"));
  if (Ipixtyp(outband) != Iu_byte_typ)
    return(Error(2, "bin_man: Output pixel type must be unsigned byte.\n"));

  xsize = Ixsize(inband);
  ysize = Iysize(inband);
  
  for (x = 1; x <= xsize; x++)
    for (y = 1; y <= ysize; y++)
      if (inband[y][x] < threshold)
	outband[y][x] = 0;
      else	
	outband[y][x] = 255;

  return 0;
}
