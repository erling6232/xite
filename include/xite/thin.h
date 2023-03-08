
/*
  This header-file is produced automatically (at least in part) by
  makeHeader of BLAB, Ifi, UiO.
*/


/*C*

________________________________________________________________

        thin.h
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

#ifndef _THIN_H_
#define _THIN_H_

_XITE_CPLUSPLUS_BEGIN

#ifndef NULL
#define NULL 0
#endif

/* Status values returned in calls to routines */
#define THN_OK 0
#define THN_ERROR -1
#define THN_NO_MEMORY -10;

#define THN_NO_DIR -256;

#define THN_UNKNOWN_INT -32768

#define THN_FOREGROUND 0
#define THN_BACKGROUND 255


extern _XITE_WIN_DLL int thn_har  _XITE_PARAMS(( IBAND inband ));
extern _XITE_WIN_DLL int thn_lc  _XITE_PARAMS(( int verbose, IBAND inband, IBAND outband ));
extern _XITE_WIN_DLL int thn_zs  _XITE_PARAMS(( int verbose, IBAND inband, IBAND outband ));

_XITE_CPLUSPLUS_END

#endif /* _THIN_H_ */
