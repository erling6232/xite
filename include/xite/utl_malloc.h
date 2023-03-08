
/*
  This header-file is produced automatically (at least in part) by
  makeHeader of BLAB, Ifi, UiO.
*/


/*

________________________________________________________________

        utl_malloc.hc
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

#ifndef _UTL_MALLOC_
#define _UTL_MALLOC_

_XITE_CPLUSPLUS_BEGIN

extern _XITE_WIN_DLL void *Mmatrix_3d  _XITE_PARAMS(( int i_x_min, int i_x_max, int i_y_min, int i_y_max, int i_z_min, int i_z_max, int i_element_size, int i_clear ));
extern _XITE_WIN_DLL void *Mmatrix_2d  _XITE_PARAMS(( int i_row_min, int i_row_max, int i_col_min, int i_col_max, int i_element_size, int i_clear ));
extern _XITE_WIN_DLL void *Mmatrix_1d  _XITE_PARAMS(( int i_first, int i_last, int i_element_size, int i_clear ));
extern _XITE_WIN_DLL void *Fmatrix_3d  _XITE_PARAMS(( char * i_cube, char *i_matrix, char *i_rows ));
extern _XITE_WIN_DLL void *Fmatrix_2d  _XITE_PARAMS(( char * i_matrix, char *i_rows ));
extern _XITE_WIN_DLL void *Fmatrix_1d  _XITE_PARAMS(( char *array ));

_XITE_CPLUSPLUS_END

#endif /* _UTL_MALLOC_ */
