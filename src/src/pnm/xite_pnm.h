

/*C*

________________________________________________________________

        xite_pnm.h
        $Id$
        Copyright 1994, Blab, UiO
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

#ifndef _XITE_PNM_H_
#define _XITE_PNM_H_

_XITE_CPLUSPLUS_BEGIN

#include <xite/biff.h>
#include <pnm.h>

#ifdef PGM_BIGGRAYS

# define PNM_BIFF_BAND_TYPE IUS_BAND
# define PNM_BIFF_PIXVAL_TYPE Iu_short_typ

# else /* PGM_BIGGRAYS */

# define PNM_BIFF_BAND_TYPE IBAND
# define PNM_BIFF_PIXVAL_TYPE Iu_byte_typ

#endif /* PGM_BIGGRAYS */

extern _XITE_WIN_DLL int biff2pnmg _XITE_PARAMS(( IMAGE img, xel*** pnm, xelval *maxval, int *binary ));
extern _XITE_WIN_DLL int biff2pnmc _XITE_PARAMS(( IMAGE img, xel*** pnm, xelval *maxval ));
extern _XITE_WIN_DLL int biff2pnm _XITE_PARAMS(( IMAGE img, xel*** pnm, xelval *maxval, int *format ));
extern _XITE_WIN_DLL void writepnm _XITE_PARAMS(( xel **pnm, char *filename, int xsize, int ysize, xelval maxval, int format, int forceplain ));
extern _XITE_WIN_DLL int pnm2biff _XITE_PARAMS(( xel** pnm, int xsize, int ysize, IMAGE *img, int format, xelval maxval, int lpv, int hpv ));
extern _XITE_WIN_DLL int ppm2biff _XITE_PARAMS(( xel** pnm, int xsize, int ysize, IMAGE *img, xelval maxval ));
extern _XITE_WIN_DLL int pgm2biff _XITE_PARAMS(( xel** pnm, int xsize, int ysize, IMAGE *img, xelval maxval ));
extern _XITE_WIN_DLL int pbm2biff _XITE_PARAMS(( xel** pnm, int xsize, int ysize, IMAGE *img, xelval maxval, int lpv, int hpv ));

_XITE_CPLUSPLUS_END

#endif /* _XITE_PNM_H_ */
