
/*
  This header-file is produced automatically (at least in part) by
  makeHeader of BLAB, Ifi, UiO.
*/


/*C*

________________________________________________________________

        binarize.h
        $Id$
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

/*************************************************************************
 *                                                                       *
 *  D E F I N E    C O N S T A N T S                                     *
 *                                                                       *
 *************************************************************************/


#ifndef BIN_C_H
#define BIN_C_H

#ifndef NULL
#define NULL 0
#endif

#define DEFAULT 0
#define ABUTALEB 1
#define BERNSEN 2
#define CHOW_KANEKO 3
#define NAKAGAWA_ROSENFELD 103
#define EIKVIL_TAXT_MOEN 4
#define DNM 104
#define DNM_NEW 1104
#define KAPUR_SAHOO_WONG 6
#define KITTLER_ILLINGWORTH 7
#define MARIDA_HAINSWORTH 8
#define NIBLACK 9
#define OTSU 10
#define PARKER 11
#define SHIO 12
#define TAXT_FLYNN_JAIN 13
#define TRIER_TAXT 14
#define WHITE_ROHRER_1 15
#define WHITE_ROHRER_2 115
#define IMPROVED_WHITE_ROHRER 215
#define WHITE_ROHRER_2_SSV 315
#define WHITE_ROHRER_2_DSV 415
#define YANOWITZ_BRUCKSTEIN 16
#define MANUAL  20

#define BIN_FORGROUND 0
#define BIN_FOREGROUND 0
#define BIN_BACKGROUND 255

#define BIN_INFINITY 1.223e10
#define BIN_TINY 1e-6

#define BIN_OK 0
#define BIN_ERROR -1
#define BIN_NO_MEMORY -10
#define BIN_NO_CLUSTER -2

#define BIN_NOT_VISITED 0
#define BIN_LEFT_DONE 1
#define BIN_DOWN_DONE 2
#define BIN_RIGHT_DONE 4
#define BIN_UP_DONE 8
#define BIN_FROM_LEFT 16
#define BIN_FROM_BELOW 32
#define BIN_FROM_RIGHT 64
#define BIN_FROM_ABOVE 128
#define BIN_DIR_MASK 240
#define BIN_DONE_MASK 15

#define BIN_VISITED_FIRST_TIME 255
#define BIN_VISITED 127
#define BIN_DELETED 63

#define WR_OLD 0
#define WR_TWO_BY_TWO 0
#define WR_SINGLE_VECTOR 1
#define WR_DOUBLE_VECTORS 2

#define WR_NULL_MASK 1
#define WR_PLUS_MASK 2

#endif /* BIN_C_H */

#ifndef _BINARIZE_H_
#define _BINARIZE_H_

_XITE_CPLUSPLUS_BEGIN

#include <xite/biff.h>

extern _XITE_WIN_DLL int bin_ab  _XITE_PARAMS(( IBAND inband, IBAND outband ));
extern _XITE_WIN_DLL int bin_be  _XITE_PARAMS(( int region_size, int contrast_limit, int set_doubt_to_low, IBAND inband, IBAND outband ));
extern _XITE_WIN_DLL int bin_etm  _XITE_PARAMS(( int region_size, int step_size, double dist_limit, double weight, int glob_min, int glob_max, int bin_method, IBAND inband, IBAND outband ));
extern _XITE_WIN_DLL int bin_iwr  _XITE_PARAMS(( int mode, int activity_limit, int mean_size, int max_limit, int spot_size_limit, IBAND inband, IBAND outband, int intermediate_images ));
extern _XITE_WIN_DLL int bin_ki  _XITE_PARAMS(( IBAND inband, IBAND outband ));
extern _XITE_WIN_DLL int bin_ksw  _XITE_PARAMS(( IBAND inband, IBAND outband ));
extern _XITE_WIN_DLL int bin_man  _XITE_PARAMS(( int threshold, IBAND inband, IBAND outband ));
extern _XITE_WIN_DLL int bin_mh  _XITE_PARAMS(( IBAND inband, IBAND outband, int use_sem_var, double convergence_limit ));
extern _XITE_WIN_DLL int bin_ni  _XITE_PARAMS(( int region_size, double weight, IBAND inband, IBAND outband ));
extern _XITE_WIN_DLL int bin_nr  _XITE_PARAMS(( double sdev_limit, double mean_limit, double min_ratio, double max_ratio, double valley_to_peak, int region_size, IBAND inband, IBAND outband ));
extern _XITE_WIN_DLL int bin_ot  _XITE_PARAMS(( IBAND inband, IBAND outband ));
extern _XITE_WIN_DLL int bin_pa  _XITE_PARAMS(( IBAND inband, IBAND outband, int region_size, double k, double mean_lim, double sdev_lim, int abs_value ));
extern _XITE_WIN_DLL int bin_tfj  _XITE_PARAMS(( IBAND inband, IBAND outband, int region_size, int step_size, int training_size ));
extern _XITE_WIN_DLL int bin_wr1  _XITE_PARAMS(( IBAND inband, IBAND outband, int x_lookahead, int y_lookahead, int bias_mode, int bias_factor, int f_factor, int g_factor, int intermediate_images ));
extern _XITE_WIN_DLL int bin_wr2  _XITE_PARAMS(( int mode, int activity_limit, double distance_limit, int mean_size, int search_vec_len, int diagonal, int max_limit, int spot_size_limit, IBAND inband, IBAND outband, int intermediate_images ));
extern _XITE_WIN_DLL int bin_yb  _XITE_PARAMS(( IBAND inband, IBAND outband, int threshold, double beta, int max_iterations ));
extern _XITE_WIN_DLL int expand_edge  _XITE_PARAMS(( IBAND inband, int zero_expand, int even_expand, int expand_depth, IBAND outband ));
extern _XITE_WIN_DLL int kapur_threshold  _XITE_PARAMS(( IBAND inband ));
extern _XITE_WIN_DLL int kittler_threshold  _XITE_PARAMS(( IBAND inband, double *mu_1, double *mu_2, double *mu ));
extern _XITE_WIN_DLL int mark_ghost  _XITE_PARAMS(( IBAND segmented, IBAND original, IBAND outband, int val_step_threshold ));
extern _XITE_WIN_DLL int mark_inv_ghost  _XITE_PARAMS(( IBAND segmented, IBAND original, IBAND outband, int val_step_threshold ));
extern _XITE_WIN_DLL int otsu_threshold  _XITE_PARAMS(( IBAND inband, double *mu_0, double *mu_1, double *mu ));
extern _XITE_WIN_DLL int remove_ghost  _XITE_PARAMS(( IBAND segmented, IBAND original, IBAND outband, int val_step_threshold ));
extern _XITE_WIN_DLL int remove_inv_ghost  _XITE_PARAMS(( IBAND segmented, IBAND original, IBAND outband, int val_step_threshold ));
extern _XITE_WIN_DLL int remove_small  _XITE_PARAMS(( IBAND inband, IBAND outband, int size_threshold ));

_XITE_CPLUSPLUS_END

#endif /* _BINARIZE_H_ */



