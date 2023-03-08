
/*

________________________________________________________________

        logarithm.c
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


#include <stdlib.h>
#include <math.h>
#include <xite/includes.h>
#include <xite/biff.h>
#include <xite/arithmetic.h>
#include XITE_STDIO_H
#include <xite/blab.h>
#include <xite/convert.h>
#include <xite/geometry.h>
#include <xite/message.h>
#include <xite/readarg.h>

#ifndef MAX
# define MAX(a, b) ((a) >= (b) ? (a) : (b))
#endif

#ifndef XITE_ABS
# define XITE_ABS(i) ((i) >= 0 ? (i) : (-(i)))
#endif



/*F:fftDisplay=logarithm*/
/*F:logarithm*

________________________________________________________________

		logarithm
________________________________________________________________

Name:		logarithm, fftDisplay - Take logarithm of BIFF band (compress
                dynamic range)

Syntax:         | #include <xite/arithmetic.h>
		|
                | int logarithm( IBAND in_band, IBAND out_band,
		|    double eps, double offset, double scale,
		|    double minimum );
		|
		| #include <xite/fft.h>
		|
                | int fftDisplay( IBAND in_band, IBAND out_band,
                |    double scale );
		|
Description:    Takes the logarithm of the magnitude of the pixelvalues of a
                real or complex band according to the algorithm

		| out_pix = (log10(eps + |in_pix|) + offset) * scale
		| out_pix = (out_pix >= minimum) ? out_pix : minimum

		This mapping will compress the dynamic range of an image and
		make small pixelvalues better visible (e.g. used to display
		Fourier	transform magnitude).

		'eps' equal to DOUBLE_MIN gives the default value
		| eps  = 1 / (10 * xsize * ysize)

		'offset' equal to DOUBLE_MAX gives the default value
		| offset = -log10(eps)

		'scale' equal to 0.0 gives the default value 20.0.

		'minimum' equal to DOUBLE_MAX gives the default value
		-10.0 * 'scale'.

		For an integral output pixeltype, the resulting pixel-values
		are rounded. The pixelvalues are limited to fit within the
		range of the output pixeltype.

		'fftDisplay' is retained for compatibility with previous XITE
		versions. It is a macro call to 'logarithm' with 'eps' equal
		to DOUBLE_MIN, 'offset' equal to DOUBLE_MAX and 'minimum' equal
		to -DOUBLE_MAX. This will give the default values for 'eps' and
		'offset' mentioned above. The call to 'logarithm' is followed
		by a call to 'shift_band'.

See also:	logarithm(1), power(3), phase(3), real(3), imag(3),
                absValue(3)

Restrictions:   Input band may be any pixeltype, output may be double, real or
                unsigned byte.

Return value:   | 1 - bands have different xsize
                | 2 - bands have different ysize
                | 3 - out_band has wrong pixeltype
                | 4 - in_band has unknown pixeltype

Author:		Trond Sørensen (wrote fftDisplay).

Modified:       Svein Bøe (added output pixeltype real and double, fixed bug
                for input pixeltype real, added lots of more input pixeltypes)
		when converting from 'fftDisplay' to 'logarithm'.

Id: 		$Id$
________________________________________________________________

*/

#define FUNC_LOG10_ROUND(NAME, OUT_TYPE, IN_MINI, IN_MAXI, OUT_MAXI) \
static void NAME(OUT_TYPE *tab, double eps, double offset, double scale, OUT_TYPE minimum) \
{ \
  int i; \
  double d; \
  \
  if (eps != 0.0 && offset != 0.0 && scale != 1.0) { \
    for (i = IN_MINI; i <= IN_MAXI; i++) { \
      d      = (log10(i + eps) + offset) * scale; \
      *tab++ = CLIP_RANGE_ROUND_CAST(d, minimum, OUT_MAXI, OUT_TYPE); \
    } \
    return; \
  } \
  if (eps != 0.0 && offset != 0.0) { \
    for (i = IN_MINI; i <= IN_MAXI; i++) { \
      d      = log10(i + eps) + offset; \
      *tab++ = CLIP_RANGE_ROUND_CAST(d, minimum, OUT_MAXI, OUT_TYPE); \
    } \
    return; \
  } \
  if (eps != 0.0 && scale != 1.0) { \
    for (i = IN_MINI; i <= IN_MAXI; i++) { \
      d      = log10(i + eps) * scale; \
      *tab++ = CLIP_RANGE_ROUND_CAST(d, minimum, OUT_MAXI, OUT_TYPE); \
    } \
    return; \
  } \
  if (offset != 0.0 && scale != 1.0) { \
    for (i = IN_MINI; i <= IN_MAXI; i++) { \
      d      = (log10((double) i) + offset) * scale; \
      *tab++ = CLIP_RANGE_ROUND_CAST(d, minimum, OUT_MAXI, OUT_TYPE); \
    } \
    return; \
  } \
  if (eps != 0.0) { \
    for (i = IN_MINI; i <= IN_MAXI; i++) { \
      d      = log10(i + eps); \
      *tab++ = CLIP_RANGE_ROUND_CAST(d, minimum, OUT_MAXI, OUT_TYPE); \
    } \
    return; \
  } \
  if (offset != 0.0) { \
    for (i = IN_MINI; i <= IN_MAXI; i++) { \
      d      = log10((double) i) + offset; \
      *tab++ = CLIP_RANGE_ROUND_CAST(d, minimum, OUT_MAXI, OUT_TYPE); \
    } \
    return; \
  } \
  if (scale != 1.0) { \
    for (i = IN_MINI; i <= IN_MAXI; i++) { \
      d      = log10((double) i) * scale; \
      *tab++ = CLIP_RANGE_ROUND_CAST(d, minimum, OUT_MAXI, OUT_TYPE); \
    } \
    return; \
  } \
 \
  for (i = IN_MINI; i <= IN_MAXI; i++) { \
    d      = log10((double) i); \
    *tab++ = CLIP_RANGE_ROUND_CAST(d, minimum, OUT_MAXI, OUT_TYPE); \
  } \
  return; \
 \
}

#define FUNC_LOG10_ROUND_ABS(NAME, OUT_TYPE, IN_MINI, IN_MAXI, OUT_MAXI) \
static void NAME(OUT_TYPE *tab, double eps, double offset, double scale, OUT_TYPE minimum) \
{ \
  int i, abs_val; \
  double d; \
  \
  if (eps != 0.0 && offset != 0.0 && scale != 1.0) { \
    for (i = IN_MINI; i <= IN_MAXI; i++) { \
      abs_val = i >= 0 ? i : -i; \
      d      = (log10(abs_val + eps) + offset) * scale; \
      *tab++ = CLIP_RANGE_ROUND_CAST(d, minimum, OUT_MAXI, OUT_TYPE); \
    } \
    return; \
  } \
  if (eps != 0.0 && offset != 0.0) { \
    for (i = IN_MINI; i <= IN_MAXI; i++) { \
      abs_val = i >= 0 ? i : -i; \
      d      = log10(abs_val + eps) + offset; \
      *tab++ = CLIP_RANGE_ROUND_CAST(d, minimum, OUT_MAXI, OUT_TYPE); \
    } \
    return; \
  } \
  if (eps != 0.0 && scale != 1.0) { \
    for (i = IN_MINI; i <= IN_MAXI; i++) { \
      abs_val = i >= 0 ? i : -i; \
      d      = log10(abs_val + eps) * scale; \
      *tab++ = CLIP_RANGE_ROUND_CAST(d, minimum, OUT_MAXI, OUT_TYPE); \
    } \
    return; \
  } \
  if (offset != 0.0 && scale != 1.0) { \
    for (i = IN_MINI; i <= IN_MAXI; i++) { \
      abs_val = i >= 0 ? i : -i; \
      d      = (log10((double) abs_val) + offset) * scale; \
      *tab++ = CLIP_RANGE_ROUND_CAST(d, minimum, OUT_MAXI, OUT_TYPE); \
    } \
    return; \
  } \
  if (eps != 0.0) { \
    for (i = IN_MINI; i <= IN_MAXI; i++) { \
      abs_val = i >= 0 ? i : -i; \
      d      = log10(abs_val + eps); \
      *tab++ = CLIP_RANGE_ROUND_CAST(d, minimum, OUT_MAXI, OUT_TYPE); \
    } \
    return; \
  } \
  if (offset != 0.0) { \
    for (i = IN_MINI; i <= IN_MAXI; i++) { \
      abs_val = i >= 0 ? i : -i; \
      d      = log10((double) abs_val) + offset; \
      *tab++ = CLIP_RANGE_ROUND_CAST(d, minimum, OUT_MAXI, OUT_TYPE); \
    } \
    return; \
  } \
  if (scale != 1.0) { \
    for (i = IN_MINI; i <= IN_MAXI; i++) { \
      abs_val = i >= 0 ? i : -i; \
      d      = log10((double) abs_val) * scale; \
      *tab++ = CLIP_RANGE_ROUND_CAST(d, minimum, OUT_MAXI, OUT_TYPE); \
    } \
    return; \
  } \
 \
  for (i = IN_MINI; i <= IN_MAXI; i++) { \
      abs_val = i >= 0 ? i : -i; \
    d      = log10((double) abs_val); \
    *tab++ = CLIP_RANGE_ROUND_CAST(d, minimum, OUT_MAXI, OUT_TYPE); \
    } \
  return; \
 \
}

#define FUNC_LOG10_ABS(NAME, OUT_TYPE, IN_MINI, IN_MAXI, OUT_MAXI) \
static void NAME(OUT_TYPE *tab, double eps, double offset, double scale, OUT_TYPE minimum) \
{ \
  int i, abs_val; \
  double d; \
  \
  if (eps != 0.0 && offset != 0.0 && scale != 1.0) { \
    for (i = IN_MINI; i <= IN_MAXI; i++) { \
      abs_val = i >= 0 ? i : -i; \
      d      = (log10(abs_val + eps) + offset) * scale; \
      *tab++ = CLIP_RANGE_CAST(d, minimum, OUT_MAXI, OUT_TYPE); \
    } \
    return; \
  } \
  if (eps != 0.0 && offset != 0.0) { \
    for (i = IN_MINI; i <= IN_MAXI; i++) { \
      abs_val = i >= 0 ? i : -i; \
      d      = log10(abs_val + eps) + offset; \
      *tab++ = CLIP_RANGE_CAST(d, minimum, OUT_MAXI, OUT_TYPE); \
    } \
    return; \
  } \
  if (eps != 0.0 && scale != 1.0) { \
    for (i = IN_MINI; i <= IN_MAXI; i++) { \
      abs_val = i >= 0 ? i : -i; \
      d      = log10(abs_val + eps) * scale; \
      *tab++ = CLIP_RANGE_CAST(d, minimum, OUT_MAXI, OUT_TYPE); \
    } \
    return; \
  } \
  if (offset != 0.0 && scale != 1.0) { \
    for (i = IN_MINI; i <= IN_MAXI; i++) { \
      abs_val = i >= 0 ? i : -i; \
      d      = (log10((double) abs_val) + offset) * scale; \
      *tab++ = CLIP_RANGE_CAST(d, minimum, OUT_MAXI, OUT_TYPE); \
    } \
    return; \
  } \
  if (eps != 0.0) { \
    for (i = IN_MINI; i <= IN_MAXI; i++) { \
      abs_val = i >= 0 ? i : -i; \
      d      = log10(abs_val + eps); \
      *tab++ = CLIP_RANGE_CAST(d, minimum, OUT_MAXI, OUT_TYPE); \
    } \
    return; \
  } \
  if (offset != 0.0) { \
    for (i = IN_MINI; i <= IN_MAXI; i++) { \
      abs_val = i >= 0 ? i : -i; \
      d      = log10((double) abs_val) + offset; \
      *tab++ = CLIP_RANGE_CAST(d, minimum, OUT_MAXI, OUT_TYPE); \
    } \
    return; \
  } \
  if (scale != 1.0) { \
    for (i = IN_MINI; i <= IN_MAXI; i++) { \
      abs_val = i >= 0 ? i : -i; \
      d      = log10((double) abs_val) * scale; \
      *tab++ = CLIP_RANGE_CAST(d, minimum, OUT_MAXI, OUT_TYPE); \
    } \
    return; \
  } \
 \
  for (i = IN_MINI; i <= IN_MAXI; i++) { \
    abs_val = i >= 0 ? i : -i; \
    d      = log10((double) abs_val); \
    *tab++ = CLIP_RANGE_CAST(d, minimum, OUT_MAXI, OUT_TYPE); \
  } \
  return; \
 \
}

#define FUNC_LOG10(NAME, OUT_TYPE, IN_MINI, IN_MAXI, OUT_MAXI) \
static void NAME(OUT_TYPE *tab, double eps, double offset, double scale, OUT_TYPE minimum) \
{ \
  int i; \
  double d; \
  \
  if (eps != 0.0 && offset != 0.0 && scale != 1.0) { \
    for (i = IN_MINI; i <= IN_MAXI; i++) { \
      d      = (log10(i + eps) + offset) * scale; \
      *tab++ = CLIP_RANGE_CAST(d, minimum, OUT_MAXI, OUT_TYPE); \
    } \
    return; \
  } \
  if (eps != 0.0 && offset != 0.0) { \
    for (i = IN_MINI; i <= IN_MAXI; i++) { \
      d      = log10(i + eps) + offset; \
      *tab++ = CLIP_RANGE_CAST(d, minimum, OUT_MAXI, OUT_TYPE); \
    } \
    return; \
  } \
  if (eps != 0.0 && scale != 1.0) { \
    for (i = IN_MINI; i <= IN_MAXI; i++) { \
      d      = log10(i + eps) * scale; \
      *tab++ = CLIP_RANGE_CAST(d, minimum, OUT_MAXI, OUT_TYPE); \
    } \
    return; \
  } \
  if (offset != 0.0 && scale != 1.0) { \
    for (i = IN_MINI; i <= IN_MAXI; i++) { \
      d      = (log10((double) i) + offset) * scale; \
      *tab++ = CLIP_RANGE_CAST(d, minimum, OUT_MAXI, OUT_TYPE); \
    } \
    return; \
  } \
  if (eps != 0.0) { \
    for (i = IN_MINI; i <= IN_MAXI; i++) { \
      d      = log10(i + eps); \
      *tab++ = CLIP_RANGE_CAST(d, minimum, OUT_MAXI, OUT_TYPE); \
    } \
    return; \
  } \
  if (offset != 0.0) { \
    for (i = IN_MINI; i <= IN_MAXI; i++) { \
      d      = log10((double) i) + offset; \
      *tab++ = CLIP_RANGE_CAST(d, minimum, OUT_MAXI, OUT_TYPE); \
    } \
    return; \
  } \
  if (scale != 1.0) { \
    for (i = IN_MINI; i <= IN_MAXI; i++) { \
      d      = log10((double) i) * scale; \
      *tab++ = CLIP_RANGE_CAST(d, minimum, OUT_MAXI, OUT_TYPE); \
    } \
    return; \
  } \
 \
  for (i = IN_MINI; i <= IN_MAXI; i++) { \
    d      = log10((double) i); \
    *tab++ = CLIP_RANGE_CAST(d, minimum, OUT_MAXI, OUT_TYPE); \
  } \
  return; \
 \
}

FUNC_LOG10_ROUND(    log10_ub_ub, UNS_BYTE, 0,              UNS_BYTE_MAX,
		     UNS_BYTE_MAX)
FUNC_LOG10(          log10_ub_r,  REAL,     0,              UNS_BYTE_MAX,
		     REAL_MAX)
FUNC_LOG10(          log10_ub_d,  DOUBLE,   0,              UNS_BYTE_MAX,
		     DOUBLE_MAX)
FUNC_LOG10_ROUND_ABS(log10_sb_ub, UNS_BYTE, SIGN_BYTE_MIN,  SIGN_BYTE_MAX,
		     UNS_BYTE_MAX)
FUNC_LOG10_ABS(      log10_sb_r,  REAL,     SIGN_BYTE_MIN,  SIGN_BYTE_MAX,
		     REAL_MAX)
FUNC_LOG10_ABS(      log10_sb_d,  DOUBLE,   SIGN_BYTE_MIN,  SIGN_BYTE_MAX,
		     DOUBLE_MAX)
FUNC_LOG10_ROUND(    log10_us_ub, UNS_BYTE, 0,              UNS_SHORT_MAX,
		     UNS_BYTE_MAX)
FUNC_LOG10(          log10_us_r,  REAL,     0,              UNS_SHORT_MAX,
		     REAL_MAX)
FUNC_LOG10(          log10_us_d,  DOUBLE,   0,              UNS_SHORT_MAX,
		     DOUBLE_MAX)
FUNC_LOG10_ROUND_ABS(log10_ss_ub, UNS_BYTE, SIGN_SHORT_MIN, SIGN_SHORT_MAX,
		     UNS_BYTE_MAX)
FUNC_LOG10_ABS(      log10_ss_r,  REAL,     SIGN_SHORT_MIN, SIGN_SHORT_MAX,
		     REAL_MAX)
FUNC_LOG10_ABS(      log10_ss_d,  DOUBLE,   SIGN_SHORT_MIN, SIGN_SHORT_MAX,
		     DOUBLE_MAX)

BiffStatus logarithm(IBAND in_band, IBAND out_band, double eps, double offset, double scale, double minimum)
{
  long    xsize, ysize, y, x;
  IPIXTYP ipixtyp, opixtyp;
  double  res, re, img;
  int status = 0, int_abs_val = 0;
  static double old_eps_s, old_offset_s, old_scale_s, old_minimum_s;
  
  xsize   = Ixsize (in_band);
  ysize   = Iysize (in_band);
  ipixtyp = Ipixtyp(in_band);
  opixtyp = Ipixtyp(out_band);

  if (xsize != Ixsize(out_band))
    return(Error(1, "logarithm: bands have different xsize\n"));
  if (ysize != Iysize(out_band))
    return(Error(2, "logarithm: bands have different ysize\n"));

  if (opixtyp != Iu_byte_typ && opixtyp != Ireal_typ && opixtyp != Idouble_typ)
    return(Error(3, "logarithm: out_band has wrong pixtyp\n"));

  if (eps     == DOUBLE_MIN) eps = 0.1 / (double) xsize / (double) ysize;
  if (offset  == DOUBLE_MAX) {
    if (eps > 0.0) offset = -log10(eps);
    else offset = DOUBLE_MIN;
  }
  if (scale   == 0.0)       scale   = 20.0;
  if (minimum == DOUBLE_MAX) minimum = -10.0 * scale;

  switch ((int) ipixtyp) {
  case Iu_byte_typ: {
    IUB_BAND ibnd = (IUB_BAND) in_band;
    
    switch ((int) opixtyp) {

    case Iu_byte_typ: {
      IUB_BAND obnd = (IUB_BAND) out_band;
      UNS_BYTE table[UNS_BYTE_MAX];

      log10_ub_ub(table, eps, offset, scale, MAX(0, minimum));

      for (y = 1; y <= ysize; y++) {
	for (x = 1; x <= xsize; x++) {
	  obnd[y][x] = table[ibnd[y][x]];
	}
      }
    }
      break;

    case Ireal_typ: {
      IR_BAND obnd = (IR_BAND) out_band;
      REAL table[UNS_BYTE_MAX];

      log10_ub_r(table, eps, offset, scale, MAX(-REAL_MAX, minimum));

      for (y = 1; y <= ysize; y++) {
	for (x = 1; x <= xsize; x++) {
	  obnd[y][x] = table[ibnd[y][x]];
	}
      }
    }
      break;
    
    case Idouble_typ: {
      ID_BAND obnd = (ID_BAND) out_band;
      DOUBLE table[UNS_BYTE_MAX];

      log10_ub_d(table, eps, offset, scale, MAX(-DOUBLE_MAX, minimum));

      for (y = 1; y <= ysize; y++) {
	for (x = 1; x <= xsize; x++) {
	  obnd[y][x] = table[ibnd[y][x]];
	}
      }
    }
      break;

    } /* switch */
  } /* Iu_byte_typ */
    break;

  case Is_byte_typ: {
    ISB_BAND ibnd = (ISB_BAND) in_band;

    switch ((int) opixtyp) {

    case Iu_byte_typ: {
      IUB_BAND obnd = (IUB_BAND) out_band;
      UNS_BYTE table[SIGN_BYTE_MAX - SIGN_BYTE_MIN + 1];

      log10_sb_ub(table, eps, offset, scale, MAX(0, minimum));

      for (y = 1; y <= ysize; y++) {
	for (x = 1; x <= xsize; x++) {
	  obnd[y][x] = table[ibnd[y][x] - SIGN_BYTE_MIN];
	}
      }
    }
      break;

    case Ireal_typ: {
      IR_BAND obnd = (IR_BAND) out_band;
      REAL table[SIGN_BYTE_MAX - SIGN_BYTE_MIN + 1];

      log10_sb_r(table, eps, offset, scale, MAX(-REAL_MAX, minimum));

      for (y = 1; y <= ysize; y++) {
	for (x = 1; x <= xsize; x++) {
	  obnd[y][x] = table[ibnd[y][x] - SIGN_BYTE_MIN];
	}
      }
    }
      break;
    
    case Idouble_typ: {
      ID_BAND obnd = (ID_BAND) out_band;
      DOUBLE table[SIGN_BYTE_MAX - SIGN_BYTE_MIN + 1];

      log10_sb_d(table, eps, offset, scale, MAX(-DOUBLE_MAX, minimum));

      for (y = 1; y <= ysize; y++) {
	for (x = 1; x <= xsize; x++) {
	  obnd[y][x] = table[ibnd[y][x] - SIGN_BYTE_MIN];
	}
      }
    }
      break;

    } /* switch */
    
  }
    break;

  case Iu_short_typ: {
    IUS_BAND ibnd = (IUS_BAND) in_band;

    switch ((int) opixtyp) {

    case Iu_byte_typ: {
      IUB_BAND obnd = (IUB_BAND) out_band;

      if (xsize * ysize > UNS_SHORT_MAX) {
	static UNS_BYTE *table_us_ub_s;

	if (!table_us_ub_s)
	  table_us_ub_s = (UNS_BYTE *)
	    malloc(sizeof(UNS_BYTE) * (UNS_SHORT_MAX + 1));

	if (eps   != old_eps_s   || offset  != old_offset_s || 
	    scale != old_scale_s || minimum != old_minimum_s)
	  log10_us_ub(table_us_ub_s, eps, offset, scale, MAX(0, minimum));

	for (y = 1; y <= ysize; y++) {
	  for (x = 1; x <= xsize; x++) {
	    obnd[y][x] = table_us_ub_s[ibnd[y][x]];
	  }
	}
      } else {
	for (y = 1; y <= ysize; y++) {
	  for (x = 1; x <= xsize; x++) {
	    res = (log10(eps + (int) ibnd[y][x]) + offset) * scale;
	    obnd[y][x] = CLIP_RANGE_ROUND_CAST(res, MAX(0, minimum),
					       UNS_BYTE_MAX, UNS_BYTE);
	  }
	}
      } /* else */
    }
      break;

    case Ireal_typ: {
      IR_BAND obnd = (IR_BAND) out_band;

      if (xsize * ysize > UNS_SHORT_MAX) {
	static REAL *table_us_r_s;

	if (!table_us_r_s)
	  table_us_r_s = (REAL *)
	    malloc(sizeof(REAL) * (UNS_SHORT_MAX + 1));

	if (eps   != old_eps_s   || offset  != old_offset_s ||
	    scale != old_scale_s || minimum != old_minimum_s)
	  log10_us_r(table_us_r_s, eps, offset, scale,
		     MAX(-REAL_MAX, minimum));

	for (y = 1; y <= ysize; y++) {
	  for (x = 1; x <= xsize; x++) {
	    obnd[y][x] = table_us_r_s[ibnd[y][x]];
	  }
	}
      } else {
	for (y = 1; y <= ysize; y++) {
	  for (x = 1; x <= xsize; x++) {
	    res = (log10(eps + (int) ibnd[y][x]) + offset) * scale;
	    obnd[y][x] =
	      CLIP_RANGE_CAST(res, MAX(-REAL_MAX, minimum), REAL_MAX, REAL);
	  }
	}
      } /* else */
    }
      break;
    
    case Idouble_typ: {
      ID_BAND obnd = (ID_BAND) out_band;

      if (xsize * ysize > UNS_SHORT_MAX) {
	static DOUBLE *table_us_d_s;

	if (!table_us_d_s)
	  table_us_d_s = (DOUBLE *)
	    malloc(sizeof(DOUBLE) * (UNS_SHORT_MAX + 1));

	if (eps   != old_eps_s   || offset  != old_offset_s ||
	    scale != old_scale_s || minimum != old_minimum_s)
	  log10_us_d(table_us_d_s, eps, offset, scale,
		     MAX(-DOUBLE_MAX, minimum));

	for (y = 1; y <= ysize; y++) {
	  for (x = 1; x <= xsize; x++) {
	    obnd[y][x] = table_us_d_s[ibnd[y][x]];
	  }
	}
      } else {
	for (y = 1; y <= ysize; y++) {
	  for (x = 1; x <= xsize; x++) {
	    res = (log10(eps + (int) ibnd[y][x]) + offset) * scale;
	    obnd[y][x] = MAX(res, minimum);
	  }
	}
      }
    }
      break;

    } /* switch */
    
  }
    break;

  case Is_short_typ: {
    ISS_BAND ibnd = (ISS_BAND) in_band;

    switch ((int) opixtyp) {

    case Iu_byte_typ: {
      IUB_BAND obnd = (IUB_BAND) out_band;

      if (xsize * ysize > SIGN_SHORT_MAX - SIGN_SHORT_MIN) {
	static UNS_BYTE *table_ss_ub_s;

	if (!table_ss_ub_s)
	  table_ss_ub_s = (UNS_BYTE *)
	    malloc(sizeof(UNS_BYTE) * (SIGN_SHORT_MAX - SIGN_SHORT_MIN + 1));

	if (eps   != old_eps_s   || offset  != old_offset_s ||
	    scale != old_scale_s || minimum != old_minimum_s)
	  log10_ss_ub(table_ss_ub_s, eps, offset, scale, MAX(0, minimum));

	for (y = 1; y <= ysize; y++) {
	  for (x = 1; x <= xsize; x++) {
	    obnd[y][x] = table_ss_ub_s[ibnd[y][x] - SIGN_SHORT_MIN];
	  }
	}
      } else {
	for (y = 1; y <= ysize; y++) {
	  for (x = 1; x <= xsize; x++) {
	    res = (log10(eps + XITE_ABS(ibnd[y][x])) + offset) * scale;
	    obnd[y][x] = CLIP_RANGE_ROUND_CAST(res, MAX(0, minimum),
					       UNS_BYTE_MAX, UNS_BYTE);
	  }
	}
      } /* else */
    }
      break;

    case Ireal_typ: {
      IR_BAND obnd = (IR_BAND) out_band;

      if (xsize * ysize > SIGN_SHORT_MAX - SIGN_SHORT_MIN) {
	static REAL *table_ss_r_s;

	if (!table_ss_r_s)
	  table_ss_r_s = (REAL *)
	    malloc(sizeof(REAL) * (SIGN_SHORT_MAX - SIGN_SHORT_MIN + 1));

	if (eps   != old_eps_s   || offset  != old_offset_s ||
	    scale != old_scale_s || minimum != old_minimum_s)
	  log10_ss_r(table_ss_r_s, eps, offset, scale,
		     MAX(-REAL_MAX, minimum));

	for (y = 1; y <= ysize; y++) {
	  for (x = 1; x <= xsize; x++) {
	    obnd[y][x] = table_ss_r_s[ibnd[y][x] - SIGN_SHORT_MIN];
	  }
	}
      } else {
	for (y = 1; y <= ysize; y++) {
	  for (x = 1; x <= xsize; x++) {
	    res = (log10(eps + XITE_ABS(ibnd[y][x])) + offset) * scale;
	    obnd[y][x] =
	      CLIP_RANGE_CAST(res, MAX(-REAL_MAX, minimum), REAL_MAX, REAL);
	  }
	}
      } /* else */
    }
      break;
    
    case Idouble_typ: {
      ID_BAND obnd = (ID_BAND) out_band;

      if (xsize * ysize > SIGN_SHORT_MAX - SIGN_SHORT_MIN) {
	static DOUBLE *table_ss_d_s;

	if (!table_ss_d_s)
	  table_ss_d_s = (DOUBLE *)
	    malloc(sizeof(DOUBLE) * (SIGN_SHORT_MAX - SIGN_SHORT_MIN + 1));

	if (eps   != old_eps_s   || offset  != old_offset_s ||
	    scale != old_scale_s || minimum != old_minimum_s)
	  log10_ss_d(table_ss_d_s, eps, offset, scale,
		     MAX(-DOUBLE_MAX, minimum));

	for (y = 1; y <= ysize; y++) {
	  for (x = 1; x <= xsize; x++) {
	    obnd[y][x] = table_ss_d_s[ibnd[y][x] - SIGN_SHORT_MIN];
	  }
	}
      } else {
	for (y = 1; y <= ysize; y++) {
	  for (x = 1; x <= xsize; x++) {
	    res = (log10(eps + XITE_ABS(ibnd[y][x])) + offset) * scale;
	    obnd[y][x] = MAX(res, minimum);
	  }
	}
      } /* else */
    }
      break;

    } /* switch */
  } /* Is_short_typ */
    break;

  case Integer_typ: {
    II_BAND ibnd = (II_BAND) in_band;

    switch ((int) opixtyp) {

    case Iu_byte_typ: {
      IUB_BAND obnd = (IUB_BAND) out_band;

      for (y = 1; y <= ysize; y++) {
	for (x = 1; x <= xsize; x++) {
	  int_abs_val = XITE_ABS(ibnd[y][x]);
	  res = (log10(eps + int_abs_val) + offset) * scale;
	  obnd[y][x] = CLIP_RANGE_ROUND_CAST(res, MAX(0, minimum),
					     UNS_BYTE_MAX, UNS_BYTE);
	}
      }
    }
      break;

    case Ireal_typ: {
      IR_BAND obnd = (IR_BAND) out_band;

      for (y = 1; y <= ysize; y++) {
	for (x = 1; x <= xsize; x++) {
	  int_abs_val = XITE_ABS(ibnd[y][x]);
	  res = (log10(eps + int_abs_val) + offset) * scale;
	  obnd[y][x] =
	    CLIP_RANGE_CAST(res, MAX(-REAL_MAX, minimum), REAL_MAX, REAL);
	}
      }
    }
      break;
    
    case Idouble_typ: {
      ID_BAND obnd = (ID_BAND) out_band;

      for (y = 1; y <= ysize; y++) {
	for (x = 1; x <= xsize; x++) {
	  int_abs_val = XITE_ABS(ibnd[y][x]);
	  res = (log10(eps + int_abs_val) + offset) * scale;
	  obnd[y][x] = MAX(res, minimum);
	}
      }
    }
      break;
    } /* switch */
  } /* Integer_typ */
    break;

  case Ireal_typ: {
    IR_BAND ibnd = (IR_BAND) in_band;

    switch ((int) opixtyp) {

    case Iu_byte_typ: {
      IUB_BAND obnd = (IUB_BAND) out_band;

      for (y = 1; y <= ysize; y++) {
	for (x = 1; x <= xsize; x++) {
	  res = (log10(eps + fabs((double) ibnd[y][x])) + offset) * scale;
	  obnd[y][x] = CLIP_RANGE_ROUND_CAST(res, MAX(0, minimum),
					     UNS_BYTE_MAX, UNS_BYTE);
	}
      }
    }
      break;

    case Ireal_typ: {
      IR_BAND obnd = (IR_BAND) out_band;

      for (y = 1; y <= ysize; y++) {
	for (x = 1; x <= xsize; x++) {
	  res = (log10(eps + fabs((double) ibnd[y][x])) + offset) * scale;
	  obnd[y][x] =
	    CLIP_RANGE_CAST(res, MAX(-REAL_MAX, minimum), REAL_MAX, REAL);
	}
      }
    }
      break;
    
    case Idouble_typ: {
      ID_BAND obnd = (ID_BAND) out_band;

      for (y = 1; y <= ysize; y++) {
	for (x = 1; x <= xsize; x++) {
	  res = (log10(eps + fabs((double) ibnd[y][x])) + offset) * scale;
	  obnd[y][x] = MAX(res, minimum);
	}
      }
    }
      break;
    } /* switch */
  } /* Ireal_typ */
    break;

  case Idouble_typ: {
    ID_BAND ibnd = (ID_BAND) in_band;

    switch ((int) opixtyp) {

    case Iu_byte_typ: {
      IUB_BAND obnd = (IUB_BAND) out_band;

      for (y = 1; y <= ysize; y++) {
	for (x = 1; x <= xsize; x++) {
	  res = (log10(eps + fabs((double) ibnd[y][x])) + offset) * scale;
	  obnd[y][x] = CLIP_RANGE_ROUND_CAST(res, MAX(0, minimum),
					     UNS_BYTE_MAX, UNS_BYTE);
	}
      }
    }
      break;

    case Ireal_typ: {
      IR_BAND obnd = (IR_BAND) out_band;

      for (y = 1; y <= ysize; y++) {
	for (x = 1; x <= xsize; x++) {
	  res = (log10(eps + fabs((double) ibnd[y][x])) + offset) * scale;
	  obnd[y][x] =
	    CLIP_RANGE_CAST(res, MAX(-REAL_MAX, minimum), REAL_MAX, REAL);
	}
      }
    }
      break;
    
    case Idouble_typ: {
      ID_BAND obnd = (ID_BAND) out_band;

      for (y = 1; y <= ysize; y++) {
	for (x = 1; x <= xsize; x++) {
	  res = (log10(eps + fabs((double) ibnd[y][x])) + offset) * scale;
	  obnd[y][x] = MAX(res, minimum);
	}
      }
    }
      break;
    } /* switch */
  } /* Idouble_typ */
    break;

  case Icomplex_typ: {
    IC_BAND ibnd = (IC_BAND) in_band;

    switch ((int) opixtyp) {

    case Iu_byte_typ: {
      IUB_BAND obnd = (IUB_BAND) out_band;

      for (y = 1; y <= ysize; y++) {
	for (x = 1; x <= xsize; x++) {
	  re  = ibnd[y][x].re;
	  img = ibnd[y][x].im;
	  res = sqrt((double) (re * re + img * img));
	  res = (log10(eps + (double) res) + offset) * scale;
	  obnd[y][x] = CLIP_RANGE_ROUND_CAST(res, MAX(0, minimum),
					     UNS_BYTE_MAX, UNS_BYTE);
	}
      }
    }
      break;

    case Ireal_typ: {
      IR_BAND obnd = (IR_BAND) out_band;

      for (y = 1; y <= ysize; y++) {
	for (x = 1; x <= xsize; x++) {
	  re  = ibnd[y][x].re;
	  img = ibnd[y][x].im;
	  res = sqrt((double) (re * re + img * img));
	  res = (log10(eps + (double) res) + offset) * scale;
	  obnd[y][x] =
	    CLIP_RANGE_CAST(res, MAX(-REAL_MAX, minimum), REAL_MAX, REAL);
	}
      }
    }
      break;

    case Idouble_typ: {
      ID_BAND obnd = (ID_BAND) out_band;

      for (y = 1; y <= ysize; y++) {
	for (x = 1; x <= xsize; x++) {
	  re  = ibnd[y][x].re;
	  img = ibnd[y][x].im;
	  res = sqrt((double) (re * re + img * img));
	  res = (log10(eps + (double) res) + offset) * scale;
	  obnd[y][x] = MAX(res, minimum);
	}
      }
    }
      break;
    } /* switch */
  } /* Icomplex_typ */
    break;

  case Id_complex_typ: {
    IDC_BAND ibnd = (IDC_BAND) in_band;

    switch ((int) opixtyp) {

    case Iu_byte_typ: {
      IUB_BAND obnd = (IUB_BAND) out_band;

      for (y = 1; y <= ysize; y++) {
	for (x = 1; x <= xsize; x++) {
	  re  = ibnd[y][x].re;
	  img = ibnd[y][x].im;
	  res = sqrt((double) (re * re + img * img));
	  res = (log10(eps + (double) res) + offset) * scale;
	  obnd[y][x] = CLIP_RANGE_ROUND_CAST(res, MAX(0, minimum),
					     UNS_BYTE_MAX, UNS_BYTE);
	}
      }
    }
      break;

    case Ireal_typ: {
      IR_BAND obnd = (IR_BAND) out_band;

      for (y = 1; y <= ysize; y++) {
	for (x = 1; x <= xsize; x++) {
	  re  = ibnd[y][x].re;
	  img = ibnd[y][x].im;
	  res = sqrt((double) (re * re + img * img));
	  res = (log10(eps + (double) res) + offset) * scale;
	  obnd[y][x] =
	    CLIP_RANGE_CAST(res, MAX(-REAL_MAX, minimum), REAL_MAX, REAL);
	}
      }
    }
      break;

    case Idouble_typ: {
      ID_BAND obnd = (ID_BAND) out_band;

      for (y = 1; y <= ysize; y++) {
	for (x = 1; x <= xsize; x++) {
	  re  = ibnd[y][x].re;
	  img = ibnd[y][x].im;
	  res = sqrt((double) (re * re + img * img));
	  res = (log10(eps + (double) res) + offset) * scale;
	  obnd[y][x] = MAX(res, minimum);
	}
      }
    }
      break;
    } /* switch */
  } /* Id_complex_typ */
    break;

  default: status = 4;
    break;

  } /* switch */

  old_eps_s     = eps;
  old_offset_s  = offset;
  old_scale_s   = scale;
  old_minimum_s = minimum;

  if (!status) return(status);
  else return(Error(status, "logarithm: Unknown pixeltype for in_band.\n"));

} /* logarithm() */
