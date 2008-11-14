
/*

________________________________________________________________

        biff.h
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


/*====================================================================*/
/*                                                                    */
/*   MODULE-NAME    :  BIFF - Blab-UiO Image File Format              */
/*   PURPOSE        :  DEFINITIONS FOR HANDLING THE                   */
/*                     BLAB-UIO IMAGE FILE FORMAT.                    */
/*   WRITTEN BY     :  TOR LØNNESTAD                                  */
/*   VERSION        :  2.0                                            */
/*   LAST UPDATE    :  17-04-97                                       */
/*   REVISED BY     :  SVEIN BØE                                      */
/*   $Id$               */
/*                                                                    */
/*====================================================================*/

/*====================================================================*/
/*=========================   INCLUSIONS   ===========================*/
/*====================================================================*/

#ifndef _BIFF_H_
#define _BIFF_H_ 1

#define XITE_VERSION "3.44"

#ifdef _WIN32
# ifndef ANSI
#  define ANSI 1
# endif
#endif

#ifdef XITE_HEADERS
# include <xite/includes.h>
#endif

#ifndef _XITE_INCLUDES_H_
# if defined(ANSI) && !defined(__STDC__)
#   define __STDC__
# endif

# if defined(__STDC__) && !defined(ANSI)
#   define ANSI
# endif
#endif

#include <xite/xite_funcproto.h>

_XITE_CPLUSPLUS_BEGIN

#ifdef _XITE_INCLUDES_H_
# include XITE_STDIO_H
# include XITE_LIMITS_H    /* For pixel type definitions and limits */
#else
# include <stdio.h>
# include <limits.h>       /* For integral type limits */
#endif

/*====================================================================*/
/*====================   CONSTANT DEFINITIONS   ======================*/
/*====================================================================*/

#ifndef NULL
# define NULL           0
#endif

#ifndef TRUE
# define TRUE           1
#endif
#ifndef FALSE
# define FALSE          0
#endif

/*====================================================================*/
/*======================  TYPE DEFINITIONS  ==========================*/ 
/*====================================================================*/

typedef long IPIXTYP;              /* 0 <= abs(IPIXTYP) <= 11 */
typedef int  BiffStatus;

/* typedef BIT */
/* typedef BIT2 */
/* typedef NIBBLE */
typedef unsigned char UNS_BYTE;
#if CHAR_MAX == SCHAR_MAX
  typedef char SIGN_BYTE;
#else
  typedef signed char SIGN_BYTE;
#endif
typedef unsigned short UNS_SHORT; 
typedef short SIGN_SHORT;
#ifdef INTEGER_MAX
  /* XITE_LIMITS_H has been included. */
# if INTEGER_MAX == INT_MAX
    typedef int INTEGER;
#   define XITE_INTEGER_FORMAT "d"
#   define XITE_INT_IS_INT
# else
    typedef long INTEGER;
#   define XITE_INTEGER_FORMAT "ld"
# endif
#else /* INTEGER_MAX */
  /* XITE_LIMITS_H has not been included. */
# if LONG_MAX > 2147483647
    typedef int INTEGER;
#   define XITE_INTEGER_FORMAT "d"
#   define XITE_INT_IS_INT
# else
    typedef long INTEGER;
#   define XITE_INTEGER_FORMAT "ld"
# endif
#endif /* INTEGER_MAX */

typedef float REAL;
typedef struct { float re, im; } COMPLEX;
typedef struct { double re, im; } DCOMPLEX;
typedef double DOUBLE;

typedef UNS_BYTE**    IBAND;
typedef UNS_BYTE**    IUB_BAND;
typedef SIGN_BYTE**   ISB_BAND;
typedef UNS_SHORT**   IUS_BAND;
typedef SIGN_SHORT**  ISS_BAND;
typedef INTEGER**     II_BAND;
typedef REAL**        IR_BAND;
typedef COMPLEX**     IC_BAND;
typedef DOUBLE**      ID_BAND;
typedef DCOMPLEX**    IDC_BAND;

typedef UNS_BYTE***   IMAGE;
typedef UNS_BYTE***   IUB_IMAGE;
typedef SIGN_BYTE***  ISB_IMAGE;
typedef UNS_SHORT***  IUS_IMAGE;
typedef SIGN_SHORT*** ISS_IMAGE;
typedef INTEGER***    II_IMAGE;
typedef REAL***       IR_IMAGE;
typedef COMPLEX***    IC_IMAGE;
typedef DOUBLE***     ID_IMAGE;
typedef DCOMPLEX***   IDC_IMAGE;

typedef struct
{
  /* When a band is saved to file from a little-endian computer, an extra
     bit of pixtyp is set (pixtyp | Ilittle_endian_mask).
     If the band is a colortable, an extra bit is set (pixtyp | Icolor_mask).
     Only the bits not concerned with byte-sex or colortable are used in
     determining pixel size. Colortables are also handled when determining
     pixel type and name. */

   IPIXTYP pixtyp;
   long xsize;
   long ysize;
   long xstart;
   long ystart;
   long xmag;
   long ymag;
   long status;
   long roi_xstart;
   long roi_ystart;
   long roi_xsize;
   long roi_ysize;
   long* this_struct;
} _Iband_info;

typedef struct
{
   long nbands;
   char *filename;
   char title[36]; /* 33 + alignment */
   long status; 
   long param[8];
   long nchars;
   long nblocks;
   long pos;
   char *text;
   long textbufblks;
   long ninfoblks;
   long nbandblks;
   long ntextblks;
   int fd;
   int maybe_pipe;
} _Info;

/*====================================================================*/
/*==================  EXTERN DATA, DEFINED IN BIFF.C =================*/
/*====================================================================*/

extern _XITE_WIN_DLL unsigned char Ibitmask[];
extern _XITE_WIN_DLL unsigned char Inegbitmask[];
extern _XITE_WIN_DLL IPIXTYP Icolor_mask;

/*====================================================================*/
/*======================   MACRO FUNCTIONS   =========================*/
/*====================================================================*/

#define Igetbit(band,y,x,b)   ((band[y][x] & Ibitmask[b]) ? 1 : 0)
#define Isetbit(band,y,x,b)   (band[y][x] |= Ibitmask[b])
#define Iclearbit(band,y,x,b) (band[y][x] &= Inegbitmask[b])
#define Inegbit(band,y,x,b)   (band[y][x] ^= Ibitmask[b])

#define xiteFree(p) {if (p) {free((p)); (p) = NULL;} \
                     else Warning(2, "xiteFree: NULL-pointer.\n");}

#define CLIP_RANGE_CAST(x, min, max, type) \
  ((x) > (max) ? (type) (max) : \
    ((x) < (min) ? (type) (min) : \
      (type) (x)))

#define CLIP_RANGE_ROUND_CAST(x, min, max, type) \
  ((x) > (max) ? (type) (max) : \
    ((x) < (min) ? (type) (min) : \
      ((x) > 0.0 ? (type) (x + 0.5) : \
        (type) (x - 0.5))))

#define CLIP_MIN_CAST(x, min, type) \
  ((x) < (min) ? (type) (min) : (type) (x))

#define CLIP_MAX_CAST(x, max, type) \
  ((x) > (max) ? (type) (max) : (type) (x))

#define CLIP_MIN_ROUND_CAST(x, min, type) \
  ((x) < (min) ? (type) (min) : \
      ((x) > 0.0 ? (type) (x + 0.5) : \
        (type) (x - 0.5)))

#define CLIP_MAX_ROUND_CAST(x, max, type) \
  ((x) > (max) ? (type) (max) : \
    ((x) > 0.0 ? (type) (x + 0.5) : \
      (type) (x - 0.5)))

/*=====   PIXEL TYPE NUMBERS   =====*/
/* When a band is saved to file from a little-endian computer, an extra
   bit is set (pixtyp | Ilittle_endian_mask) */
   
#define Iunknown_typ   -1
#define Ibit_typ        0
#define Ibit2_typ       1
#define Inibble_typ     2
#define Iu_byte_typ     3
#define Is_byte_typ     4
#define Iu_short_typ    5
#define Is_short_typ    6
#define Integer_typ     7
#define Ireal_typ       8
#define Icomplex_typ    9
#define Idouble_typ    10
#define Id_complex_typ 11
#define Iu_color_typ   (Icolor_mask | Iu_byte_typ)

/*=====   IMAGE STATUS   =====*/
#define Ireadonly      0
#define Ireadwrite     1
#define Iwrite         2
#define Ilocal         3

/*=====   BAND STATUS   =====*/
#define Inormal        0
#define Isubband       1
#define Iabsent        2

/*=====   ERROR NUMBERS   =====*/
#define Iok            0
#define Ibifferr       1
#define Iopenerr       2
#define Icloserr       3
#define Inotoperr      4
#define Ibandnrerr     5
#define Isizerr        6
#define Istrtmagerr    7
#define Ireaderr       8
#define Iwriterr       9
#define Itexterr      10
#define Iblockerr     11
#define Ipixtyperr    12
#define Ilockerr      13
#define Istatuserr    14
#define Iblocknrerr   15
#define Isetposerr    16
#define Igetcharerr   17
#define Infoerr       18
#define Iparnrerr     19
#define Imkarrerr     20
#define Itmpfilerr    21
#define Iroierr       22
#define Imageerr      23
#define Ibanderr      24
#define Imallocerr    25
#define Icolpixerr    26
#define Iexecerr      27
#define Ibandspecerr  28
#define Inotimplerr   99

/*====================================================================*/
/*=======================  FUNCTIONDEFINITIONS  ======================*/
/*====================================================================*/

/*=====   IMAGE HANDLING   =====*/

extern _XITE_WIN_DLL IMAGE Imake_image  _XITE_PARAMS(( int nbands, char *title, IPIXTYP pt, long xsize, long ysize ));
extern _XITE_WIN_DLL IMAGE Init_image  _XITE_PARAMS(( int nbands, char *title ));
extern _XITE_WIN_DLL IMAGE Iread_image  _XITE_PARAMS(( char *filename ));
extern _XITE_WIN_DLL IMAGE Iread_image_bands _XITE_PARAMS(( char *filename, int *bandarr, int num_bands ));
extern _XITE_WIN_DLL IMAGE Icopy_init  _XITE_PARAMS(( IMAGE i ));
extern _XITE_WIN_DLL BiffStatus Iwrite_image  _XITE_PARAMS(( IMAGE i, char *filename ));
extern _XITE_WIN_DLL BiffStatus Iopen_image  _XITE_PARAMS(( IMAGE *i, char *filename, int acc ));
extern _XITE_WIN_DLL BiffStatus Iclose_image  _XITE_PARAMS(( IMAGE i ));
extern _XITE_WIN_DLL long Istatus  _XITE_PARAMS(( IMAGE i ));
extern _XITE_WIN_DLL long Inbands  _XITE_PARAMS(( IMAGE i ));
extern _XITE_WIN_DLL long Inchars  _XITE_PARAMS(( IMAGE i ));
extern _XITE_WIN_DLL char * Ititle  _XITE_PARAMS(( IMAGE i ));
extern _XITE_WIN_DLL BiffStatus Iset_title  _XITE_PARAMS(( IMAGE i, char *title ));
extern _XITE_WIN_DLL BiffStatus Iset_nbands  _XITE_PARAMS(( IMAGE *i, int nbands ));
extern _XITE_WIN_DLL BiffStatus Iget_param  _XITE_PARAMS(( IMAGE i, int parmnr, long *parmval ));
extern _XITE_WIN_DLL BiffStatus Iset_param  _XITE_PARAMS(( IMAGE i, int parmnr, long parmval ));
extern _XITE_WIN_DLL BiffStatus Init_bands  _XITE_PARAMS(( IMAGE i, IPIXTYP pt, long xsize, long ysize ));
extern _XITE_WIN_DLL void Idel_image  _XITE_PARAMS(( IMAGE *ip ));

/*=====   BAND HANDLING   =====*/

extern _XITE_WIN_DLL IBAND Imake_band  _XITE_PARAMS(( IPIXTYP pt, long xsize, long ysize ));
extern _XITE_WIN_DLL BiffStatus Imake_bands  _XITE_PARAMS(( IMAGE i, IPIXTYP pt, long xsize, long ysize ));
extern _XITE_WIN_DLL IBAND Imake_subband  _XITE_PARAMS(( IBAND band, long xstart, long ystart, long xsize, long ysize ));
extern _XITE_WIN_DLL IBAND Init_band  _XITE_PARAMS(( IPIXTYP pt, long xsize, long ysize ));
extern _XITE_WIN_DLL void Idel_band  _XITE_PARAMS(( IBAND *band ));
extern _XITE_WIN_DLL void Idel_band_pix  _XITE_PARAMS(( IBAND *band ));
extern _XITE_WIN_DLL BiffStatus Iread_band  _XITE_PARAMS(( IMAGE i, int bandnr ));
extern _XITE_WIN_DLL BiffStatus Iwrite_band  _XITE_PARAMS(( IMAGE i, int bandnr ));
extern _XITE_WIN_DLL long Iband_status  _XITE_PARAMS(( IBAND band ));
extern _XITE_WIN_DLL long Ixmag  _XITE_PARAMS(( IBAND band ));
extern _XITE_WIN_DLL long Ixsize  _XITE_PARAMS(( IBAND band ));
extern _XITE_WIN_DLL long Ixstart  _XITE_PARAMS(( IBAND band ));
extern _XITE_WIN_DLL long Iymag  _XITE_PARAMS(( IBAND band ));
extern _XITE_WIN_DLL long Iysize  _XITE_PARAMS(( IBAND band ));
extern _XITE_WIN_DLL long Iystart  _XITE_PARAMS(( IBAND band ));
extern _XITE_WIN_DLL BiffStatus Iset_start  _XITE_PARAMS(( IBAND band, long xstart, long ystart ));
extern _XITE_WIN_DLL BiffStatus Iset_mag  _XITE_PARAMS(( IBAND band, long xmag, long ymag ));
extern _XITE_WIN_DLL BiffStatus Iget_roi  _XITE_PARAMS(( IBAND band, long *roi_xstart, long *roi_ystart, long *roi_xsize, long *roi_ysize ));
extern _XITE_WIN_DLL BiffStatus Iset_roi  _XITE_PARAMS(( IBAND band, long roi_xstart, long roi_ystart, long roi_xsize, long roi_ysize ));
extern _XITE_WIN_DLL long Ibandsize  _XITE_PARAMS(( IBAND band ));
extern _XITE_WIN_DLL IPIXTYP Ipixtyp  _XITE_PARAMS(( IBAND band ));
extern _XITE_WIN_DLL char * Ipixname  _XITE_PARAMS(( IPIXTYP pixtyp ));
extern _XITE_WIN_DLL char* IpixPrintFormat  _XITE_PARAMS(( IPIXTYP pixtyp ));
extern _XITE_WIN_DLL IPIXTYP IparsePixtyp _XITE_PARAMS(( char *pixtyp ));
extern _XITE_WIN_DLL long Ipixsize  _XITE_PARAMS(( IPIXTYP pity ));
extern _XITE_WIN_DLL BiffStatus Icopy_band  _XITE_PARAMS(( IBAND b1, IBAND b2 ));

/*=====   TEXT HANDLING   =====*/

extern _XITE_WIN_DLL BiffStatus Iread_text  _XITE_PARAMS(( IMAGE i ));
extern _XITE_WIN_DLL BiffStatus Iwrite_text  _XITE_PARAMS(( IMAGE i ));
extern _XITE_WIN_DLL long Igetpos  _XITE_PARAMS(( IMAGE i ));
extern _XITE_WIN_DLL BiffStatus Isetpos  _XITE_PARAMS(( IMAGE i, long pos ));
extern _XITE_WIN_DLL BiffStatus Igetchar  _XITE_PARAMS(( IMAGE i, char *c ));
extern _XITE_WIN_DLL BiffStatus Iputchar  _XITE_PARAMS(( IMAGE i, int c ));
extern _XITE_WIN_DLL BiffStatus Iput_text  _XITE_PARAMS(( IMAGE i, char *s ));
extern _XITE_WIN_DLL BiffStatus Iget_line  _XITE_PARAMS(( IMAGE i, char *s ));
extern _XITE_WIN_DLL BiffStatus Iappend_line  _XITE_PARAMS(( IMAGE i, char *s ));
extern _XITE_WIN_DLL BiffStatus Itype_text  _XITE_PARAMS(( IMAGE i, FILE *fp ));
extern _XITE_WIN_DLL BiffStatus Iedit_text  _XITE_PARAMS(( IMAGE i ));
extern _XITE_WIN_DLL BiffStatus Idel_text  _XITE_PARAMS(( IMAGE i ));
extern _XITE_WIN_DLL BiffStatus Icopy_text  _XITE_PARAMS(( IMAGE fromimg, IMAGE toimg ));
extern _XITE_WIN_DLL BiffStatus Iend_of_text  _XITE_PARAMS(( IMAGE i ));
extern _XITE_WIN_DLL BiffStatus Ihistory  _XITE_PARAMS(( IMAGE i, char *progname, char *comment ));

/*=====   BLOCK HANDLING   =====*/

extern _XITE_WIN_DLL BiffStatus Iread_block  _XITE_PARAMS(( IMAGE i, unsigned char *blk, long blnr ));
extern _XITE_WIN_DLL BiffStatus Iwrite_block  _XITE_PARAMS(( IMAGE i, unsigned char *blk, long blnr ));

/*=====   ERROR HANDLING   =====*/

extern _XITE_WIN_DLL char * Ierr_message  _XITE_PARAMS(( BiffStatus ernr ));
extern _XITE_WIN_DLL BiffStatus Ierr_action  _XITE_PARAMS(( BiffStatus ernr ));
extern _XITE_WIN_DLL int Iset_message  _XITE_PARAMS(( int on ));
extern _XITE_WIN_DLL int Iset_abort  _XITE_PARAMS(( int on ));
extern _XITE_WIN_DLL int Iset_overwrite  _XITE_PARAMS(( int on ));

/*=====   OTHER =====*/

extern _XITE_WIN_DLL int host_byte_order _XITE_PARAMS(( void ));
extern _XITE_WIN_DLL BiffStatus Iparse_band_spec _XITE_PARAMS(( char *spec, int **bandarray, int *num_bands ));
extern _XITE_WIN_DLL char *Ifilename_part _XITE_PARAMS(( char *filename ));
extern _XITE_WIN_DLL BiffStatus Iparse_filename _XITE_PARAMS(( char **filename, int **bandarray, int *num_bands ));

_XITE_CPLUSPLUS_END

#endif /* _BIFF_H_ */
