

/*C*

________________________________________________________________

        butterworth
        $Id$
        Copyright 1991, Blab, UiO
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


#include <math.h>
#include <stdlib.h>
#include <xite/includes.h>
#include <xite/biff.h>
#include XITE_STDIO_H
#include XITE_STRING_H
#include XITE_MALLOC_H
#include <xite/fft.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include <xite/ideal.h>
#include <xite/arithmetic.h>
#include <xite/convert.h>
#include "ideal_L.h"

#ifndef M_PI
# define M_PI    3.14159265358979323846
#endif
#ifndef XITE_ABS
# define XITE_ABS(i) ((i) >= 0 ? (i) : (-(i)))
#endif



int butterworthf(IBAND band, double cut_frequency, int filter_order)
{
  int x, y, xsize, ysize, xhalf, yhalf, feven, xeven, yeven, low, filter_size;
  int xhalf2, yhalf2, status = 0;
  double max_rad, *distx, *disty, dist, fn, PPI, sum = 0.0;
  IPIXTYP pt;

  sum         = 0.0;
  xsize       = Ixsize (band);
  ysize       = Iysize (band);
  filter_size = xsize;

  pt      = Ipixtyp(band);
  PPI     = M_PI*M_PI;
  max_rad = (float) (filter_size - 1) / (float) 2.0;
  feven   = 1 - filter_size % 2;
  xeven   = 1 - xsize % 2;
  yeven   = 1 - ysize % 2;
  low     = feven ? 1 : 2;
  xhalf   = (xsize + 1) / 2; /* Odd ysize: Includes center row. */
  yhalf   = (ysize + 1) / 2; /* Odd ysize: Includes center row. */
  xhalf2  = xhalf - feven * (!xeven);
  yhalf2  = yhalf - feven * (!yeven);
  cut_frequency = cut_frequency * max_rad;

  if (xsize != ysize) {
    disty = (double *) malloc(sizeof(double) * (yhalf + xhalf));
    distx = &disty[yhalf];
  } else {
    disty = (double *) malloc(sizeof(double) * yhalf);
    distx = disty;
  }

  for (y = 1; y <= yhalf; y++) 
    disty[y-1] = XITE_ABS(y - 1) + (double) feven / 2.0;

  if (xsize != ysize) {
    for (x = 1; x <= xhalf; x++)
      distx[x-1] = XITE_ABS(x - 1) + (double) feven / 2.0;
  }

  switch ((int) pt) {

  case Ireal_typ: {
    IR_BAND bnd = (IR_BAND) band;

    /* Basic quadrant (upper left). */

    for (y = 1; y <= yhalf; y++) 
      for (x = 1; x <= xhalf; x++) {
	dist  = sqrt ((double) ((distx[x-1] * distx[x-1]) +
				(disty[y-1] * disty[y-1])));

	if (cut_frequency == 0.0) fn = 0.0;
	else if (dist <= max_rad) {
	  fn = 1 / (1 + pow((double) dist / cut_frequency,
			    (double) 2.0 * filter_order));
	} else fn = 0.0;

	bnd[y][x] = (float) fn;
	sum = sum + (float) fn;
      }

    /* Lower left. */

    for (y = low; y <= yhalf2; y++)
      for (x = 1; x <= xhalf; x++) {
	  bnd[ysize + low - y][x] = (float) bnd[y][x];
	  sum = sum + (float) bnd[y][x];
	}

    /* Upper right. */

    for (y = 1; y <= yhalf; y++)
      for (x = low; x <= xhalf2; x++) {
	bnd[y][xsize + low - x] = (float) bnd[y][x];
	sum = sum + (float) bnd[y][x];
      }

    /* Lower right. */

    for (y = low; y <= yhalf2; y++)
      for (x = low; x <= xhalf2; x++) {
	bnd[ysize + low - y][xsize + low - x] = 
	  (float) bnd[ysize + low - y][x];
	sum = sum + (float) bnd[ysize + low - y][x];
      }

    /* Initialize zero-samples in cross-center of band. */

    if ((!feven && yeven) || (feven && !yeven))
      for (x = 1; x <= xsize; x++)
	bnd[yhalf2 + 1][x] = 0.0;
    if ((!feven && xeven) || (feven && !xeven))
      for (y = 1; y <= ysize; y++)
	bnd[y][xhalf2 + 1] = 0.0;

  }
    break;

  case Idouble_typ: {
    ID_BAND bnd = (ID_BAND) band;

    /* Basic quadrant (upper left). */

    for (y = 1; y <= yhalf; y++) 
      for (x = 1; x <= xhalf; x++) {
        dist  = sqrt ((double) ((distx[x-1] * distx[x-1]) +
				(disty[y-1] * disty[y-1])));

	if (cut_frequency == 0.0) fn = 0.0;
	else if (dist <= max_rad) {
	  fn = 1 / (1 + pow((double) dist / cut_frequency,
			    (double) 2.0 * filter_order));
	} else fn = 0.0;

        bnd[y][x] = fn;
        sum = sum + fn;
      }

    /* Lower left. */

    for (y = low; y <= yhalf2; y++)
      for (x = 1; x <= xhalf; x++) {
          bnd[ysize + low - y][x] = bnd[y][x];
          sum = sum + bnd[y][x];
        }

    /* Upper right. */

    for (y = 1; y <= yhalf; y++)
      for (x = low; x <= xhalf2; x++) {
        bnd[y][xsize + low - x] = bnd[y][x];
        sum = sum + bnd[y][x];
      }

    /* Lower right. */

    for (y = low; y <= yhalf2; y++)
      for (x = low; x <= xhalf2; x++) {
        bnd[ysize + low - y][xsize + low - x] = 
          bnd[ysize + low - y][x];
        sum = sum + bnd[ysize + low - y][x];
      }

    /* Initialize zero-samples in cross-center of band. */

    if ((!feven && yeven) || (feven && !yeven))
      for (x = 1; x <= xsize; x++)
        bnd[yhalf2 + 1][x] = 0.0;
    if ((!feven && xeven) || (feven && !xeven))
      for (y = 1; y <= ysize; y++)
        bnd[y][xhalf2 + 1] = 0.0;

  }
    break;

  case Icomplex_typ: {
    IC_BAND bnd = (IC_BAND) band;

    /* Basic quadrant (upper left). */

    for (y = 1; y <= yhalf; y++) 
      for (x = 1; x <= xhalf; x++) {
        dist  = sqrt ((double) ((distx[x-1] * distx[x-1]) +
				(disty[y-1] * disty[y-1])));

	if (cut_frequency == 0.0) fn = 0.0;
	else if (dist <= max_rad) {
	  fn = 1 / (1 + pow((double) dist / cut_frequency,
			    (double) 2.0 * filter_order));
	} else fn = 0.0;

        bnd[y][x].re = (float) fn;
        bnd[y][x].im = (float) 0.0;
        sum = sum + (float) fn;
      }

    /* Lower left. */

    for (y = low; y <= yhalf2; y++)
      for (x = 1; x <= xhalf; x++) {
          bnd[ysize + low - y][x].re = (float) bnd[y][x].re;
          bnd[ysize + low - y][x].im = 0.0;
          sum = sum + (float) bnd[y][x].re;
        }

    /* Upper right. */

    for (y = 1; y <= yhalf; y++)
      for (x = low; x <= xhalf2; x++) {
        bnd[y][xsize + low - x].re = (float) bnd[y][x].re;
        bnd[y][xsize + low - x].im = 0.0;
        sum = sum + (float) bnd[y][x].re;
      }

    /* Lower right. */

    for (y = low; y <= yhalf2; y++)
      for (x = low; x <= xhalf2; x++) {
        bnd[ysize + low - y][xsize + low - x].re = 
          (float) bnd[ysize + low - y][x].re;
        bnd[ysize + low - y][xsize + low - x].im = 0.0;
        sum = sum + (float) bnd[ysize + low - y][x].re;
      }

    /* Initialize zero-samples in cross-center of band. */

    if ((!feven && yeven) || (feven && !yeven))
      for (x = 1; x <= xsize; x++) {
        bnd[yhalf2 + 1][x].re = 0.0;
        bnd[yhalf2 + 1][x].im = 0.0;
      }
    if ((!feven && xeven) || (feven && !xeven))
      for (y = 1; y <= ysize; y++) {
        bnd[y][xhalf2 + 1].re = 0.0;
        bnd[y][xhalf2 + 1].im = 0.0;
      }

  }
    break;

  case Id_complex_typ: {
    IDC_BAND bnd = (IDC_BAND) band;

    /* Basic quadrant (upper left). */

    for (y = 1; y <= yhalf; y++) 
      for (x = 1; x <= xhalf; x++) {
        dist  = sqrt ((double) ((distx[x-1] * distx[x-1]) +
				(disty[y-1] * disty[y-1])));

	if (cut_frequency == 0.0) fn = 0.0;
	else if (dist <= max_rad) {
	  fn = 1 / (1 + pow((double) dist / cut_frequency,
			    (double) 2.0 * filter_order));
	} else fn = 0.0;

        bnd[y][x].re = fn;
        bnd[y][x].im = 0.0;
        sum = sum + fn;
      }

    /* Lower left. */

    for (y = low; y <= yhalf2; y++)
      for (x = 1; x <= xhalf; x++) {
          bnd[ysize + low - y][x].re = bnd[y][x].re;
          bnd[ysize + low - y][x].im = 0.0;
          sum = sum + bnd[y][x].re;
        }

    /* Upper right. */

    for (y = 1; y <= yhalf; y++)
      for (x = low; x <= xhalf2; x++) {
        bnd[y][xsize + low - x].re = bnd[y][x].re;
        bnd[y][xsize + low - x].im = 0.0;
        sum = sum + bnd[y][x].re;
      }

    /* Lower right. */

    for (y = low; y <= yhalf2; y++)
      for (x = low; x <= xhalf2; x++) {
        bnd[ysize + low - y][xsize + low - x].re = 
          bnd[ysize + low - y][x].re;
        bnd[ysize + low - y][xsize + low - x].im = 0.0;
        sum = sum + bnd[ysize + low - y][x].re;
      }

    /* Initialize zero-samples in cross-center of band. */

    if ((!feven && yeven) || (feven && !yeven))
      for (x = 1; x <= xsize; x++) {
        bnd[yhalf2 + 1][x].re = 0.0;
        bnd[yhalf2 + 1][x].im = 0.0;
      }
    if ((!feven && xeven) || (feven && !xeven))
      for (y = 1; y <= ysize; y++) {
        bnd[y][xhalf2 + 1].re = 0.0;
        bnd[y][xhalf2 + 1].im = 0.0;
      }

  }
    break;

  default: status = 1;
    break;
    /* break; */

  } /* switch */

  free((char *) disty); disty = NULL;

  if (status) return(Error(E_PIXTYP, "%s\n", e_s[E_PIXTYP]));
  else return(0);

} /* butterworthf() */
