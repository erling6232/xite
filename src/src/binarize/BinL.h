

/*

________________________________________________________________

        BinL.h
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

#ifndef BIN_L_H
#define BIN_L_H

_XITE_CPLUSPLUS_BEGIN

extern int get_parameter_string _XITE_PARAMS(( int *argc, char **argv[] ));
extern int mh_threshold _XITE_PARAMS(( IBAND inband, IBAND labels, double threshold ));
extern int mh_class_means _XITE_PARAMS(( double *mean0, double *mean1, int *n0, int *n1 ));
extern double mh_tot_variance _XITE_PARAMS(( IBAND inband, double tot_mean ));
extern int mh_iso_corr _XITE_PARAMS(( double *rho_sq2, double *rho_2, double *rho_sq5, double *rho_2sq2 ));
extern int mh_segment _XITE_PARAMS(( double gamma_0, double gamma_1, double gamma_5 ));
extern int mh_count_diff _XITE_PARAMS(( IBAND A, IBAND B ));
extern int nr_mean_dev _XITE_PARAMS(( IBAND inband, IR_BAND reg_sdev, IR_BAND reg_mean ));
extern void thin _XITE_PARAMS(( IBAND input, IBAND output ));
extern int write_byteband _XITE_PARAMS(( IBAND inband, char *filename ));
extern int write_intband _XITE_PARAMS(( II_BAND inband, char *filename ));
extern int write_log_intband _XITE_PARAMS(( II_BAND inband, char *filename ));
extern int write_floatband _XITE_PARAMS(( IR_BAND inband, char *filename ));
extern int write_doubleband _XITE_PARAMS(( ID_BAND inband, char *filename ));
extern double gauss _XITE_PARAMS(( double x, double mu, double sigma ));
extern int k_initial_part _XITE_PARAMS(( int* center ));
extern int k_means _XITE_PARAMS(( histogram H, int nof_clusters, int *thresholds ));
extern int k_new_part _XITE_PARAMS(( int* center, histogram partition ));
extern int remove_object _XITE_PARAMS(( int x, int y, IBAND ioband, IBAND deleted ));
extern int mark_object _XITE_PARAMS(( int x, int y, IBAND ioband, IBAND deleted, int value ));
extern int sum_edge_grad _XITE_PARAMS(( IBAND magnitude, double *sum, int *edge_count ));
extern int wr2_is_edge  _XITE_PARAMS(( IBAND inband, int x, int y, UNS_BYTE pixel_value ));
extern int wr2_cnt_ngb _XITE_PARAMS(( II_BAND backg_visited, int backg_flag, UNS_BYTE pixel_value, double *fraction ));
extern int wr2_chk_pix  _XITE_PARAMS(( II_BAND backg_visited, int backg_flag, int *plusses, int *minuses, int *nulls ));
extern int wr2_chk_obj _XITE_PARAMS(( UNS_BYTE mark_value, double threshold ));
extern int wr2_seg_lab _XITE_PARAMS(( double null_threshold, double plus_threshold, int intermediate_images ));

_XITE_CPLUSPLUS_END

#endif BIN_L_H
