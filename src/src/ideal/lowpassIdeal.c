

/*C*

________________________________________________________________

        lowpassIdeal
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
#include <xite/fft.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include <xite/ideal.h>
#include <xite/arithmetic.h>
#include <xite/convert.h>
#include "ideal_L.h"

#ifndef M_PI
#define M_PI    3.14159265358979323846
#endif



#ifndef MAIN

#ifndef FUNCPROTO
BiffStatus lowpassIdealf (band, cut_frequency)
IBAND band;
double  cut_frequency;
#else /* FUNCPROTO */
BiffStatus lowpassIdealf (IBAND band, double cut_frequency)
#endif /* FUNCPROTO */
{
  int x, y, xsize, ysize, xhalf, yhalf, feven, xeven, yeven, low;
  int filter_size, xhalf2, yhalf2;
  double max_rad, distx, disty, dist, sum = 0.0, fn = 0.0;
  IPIXTYP pt;

  xsize       = Ixsize(band);
  ysize       = Iysize(band);
  pt          = Ipixtyp(band);

  filter_size = cut_frequency * xsize;
  feven       = 1 - filter_size % 2;
  xeven       = 1 - xsize % 2;
  yeven       = 1 - ysize % 2;
  low         = feven ? 1 : 2;

  max_rad     = (float) (filter_size - 1) / (float) 2.0;
  xhalf       = (xsize + 1) / 2; /* Odd ysize: Includes center row. */
  yhalf       = (ysize + 1) / 2; /* Odd ysize: Includes center row. */
  xhalf2      = xhalf - feven * (!xeven);
  yhalf2      = yhalf - feven * (!yeven);

  switch((int) pt) {
    
  case Ireal_typ: {
    IR_BAND bnd = (IR_BAND) band;
    
    /* Basic quadrant (upper left). */
    
    for (y = 1; y <= yhalf; y++)
      for (x = 1; x <= xhalf; x++) {
        distx = abs(x - 1) + (double) feven / 2;
        disty = abs(y - 1) + (double) feven / 2;
        dist  = sqrt ((double) ((distx * distx) + (disty * disty)));
	
        if (dist <= max_rad) fn = 1.0;
        else fn = 0.0;
	
        bnd[y][x] = (float) fn;
        sum += bnd[y][x];
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
        bnd[ysize + low - y][xsize + low - x] = bnd[ysize + low - y][x];
        sum = sum + bnd[ysize + low - y][x];
      }
    
    /* Initialize zero-samples in cross-center of band. */
    
    if ((!feven && yeven) || (feven && !yeven))
      for (x = 1; x <= xsize; x++) {
        bnd[yhalf2 + 1][x] = 0.0;
      }
    if ((!feven && xeven) || (feven && !xeven))
      for (y = 1; y <= ysize; y++) {
        bnd[y][xhalf2 + 1] = 0.0;
      }
  }
    break;

  case Idouble_typ: {
    ID_BAND bnd = (ID_BAND) band;
    
    /* Basic quadrant (upper left). */
    
    for (y = 1; y <= yhalf; y++)
      for (x = 1; x <= xhalf; x++) {
        distx = abs(x - 1) + (double) feven / 2;
        disty = abs(y - 1) + (double) feven / 2;
        dist  = sqrt ((double) ((distx * distx) + (disty * disty)));
	
        if (dist <= max_rad) fn = 1.0;
        else fn = 0.0;
	
        bnd[y][x] = fn;
        sum += bnd[y][x];
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
        bnd[ysize + low - y][xsize + low - x] = bnd[ysize + low - y][x];
        sum = sum + bnd[ysize + low - y][x];
      }
    
    /* Initialize zero-samples in cross-center of band. */
    
    if ((!feven && yeven) || (feven && !yeven))
      for (x = 1; x <= xsize; x++) {
        bnd[yhalf2 + 1][x] = 0.0;
      }
    if ((!feven && xeven) || (feven && !xeven))
      for (y = 1; y <= ysize; y++) {
        bnd[y][xhalf2 + 1] = 0.0;
      }
  }
    break;

  case Icomplex_typ: {
    IC_BAND bnd = (IC_BAND) band;
    
    /* Basic quadrant (upper left). */
    
    for (y = 1; y <= yhalf; y++)
      for (x = 1; x <= xhalf; x++) {
        distx = abs(x - 1) + (double) feven / 2;
        disty = abs(y - 1) + (double) feven / 2;
        dist  = sqrt ((double) ((distx * distx) + (disty * disty)));
	
        if (dist <= max_rad) fn = 1.0;
        else fn = 0.0;
	
        bnd[y][x].re = (float) fn;
        bnd[y][x].im = 0.0;
        sum += bnd[y][x].re;
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
        bnd[ysize + low - y][xsize + low - x].re = bnd[ysize + low - y][x].re;
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
    
  case Id_complex_typ: {
    IDC_BAND bnd = (IDC_BAND) band;
    
    /* Basic quadrant (upper left). */
    
    for (y = 1; y <= yhalf; y++)
      for (x = 1; x <= xhalf; x++) {
        distx = abs(x - 1) + (double) feven / 2;
        disty = abs(y - 1) + (double) feven / 2;
        dist  = sqrt ((double) ((distx * distx) + (disty * disty)));
	
        if (dist <= max_rad) fn = 1.0;
        else fn = 0.0;
	
        bnd[y][x].re = fn;
        bnd[y][x].im = 0.0;
        sum += bnd[y][x].re;
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
        bnd[ysize + low - y][xsize + low - x].re = bnd[ysize + low - y][x].re;
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

  default: return(Error(E_PIXTYP, "%s\n", e_s[E_PIXTYP]));
    /* break; */
  } /* switch */

  return(0);

} /* lowpassIdealf() */

#endif /* not MAIN */

#ifdef MAIN

#ifndef FUNCPROTO
int main(argc, argv)
int argc;
char **argv;
#else /* FUNCPROTO */
int main(int argc, char **argv)
#endif /* FUNCPROTO */
{
  IMAGE   img;
  int     xsize, ysize, stat, spatial;
  double  cut;
  char    *arg, *title, *pta;
  IPIXTYP pt;

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [<option>...] <outimage>\n\
       where <option> is\n\
        -x  <xsize>         : Horizontal size of image\n\
        -y  <ysize>         : Vertical size of image\n\
        -c  <cut_frequency> : Cutoff-frequency (rel. to 0.5*sampl.freq)\n\
        -t  <image title>   : Title of filter image\n\
        -s                  : Return filter in spatial domain\n\
        -pt <pixtyp>        : Pixeltype for filter\n"));

  if (argc == 1) Usage(E_ARG, NULL);
  arg = argvOptions(argc, argv);

  xsize       = read_iswitch(&argc, argv, "-x", 512);
  ysize       = read_iswitch(&argc, argv, "-y", 512);
  cut         = read_dswitch(&argc, argv, "-c", 0.2);
  title       = read_switch(&argc, argv, "-t", 1, "Ideal LP filter");
  title       = (strcmp(title, "Lowpass filter") == 0) ?
                read_switch(&argc, argv, "-title", 1, "Lowpass filter") :
		title;  
  spatial     = read_bswitch(&argc, argv, "-s");
  pta         = spatial ? read_switch(&argc, argv, "-pt", 1, "r") :
                          read_switch(&argc, argv, "-pt", 1, "c");

  pt = IparsePixtyp(pta);
  if (pt < Iu_byte_typ) Usage(E_PIXTYP, "Illegal value for option '-pt'.\n");

  if (pt != Ireal_typ    && pt != Idouble_typ    &&
      pt != Icomplex_typ && pt != Id_complex_typ) {
    Usage(E_PIXTYP, "Illegal value for option '-pt'.\n");
    exit(1);
  }

  if (argc != 2) Usage(E_ARG, "%s\n", e_s[E_ARG]);

  if (spatial && pt != Icomplex_typ) {
    /* fft2d will be called later, and it will only accept Icomplex_typ. */

    img = Imake_image(1, title, Icomplex_typ, xsize, ysize);
  } else img = Imake_image(1, title, pt, xsize, ysize);

  if (!img) exit(Error(E_MALLOC, "%s\n", e_s[E_MALLOC]));

  if ( (stat = lowpassIdealf(img[1], cut)) )
    exit(Error(stat, "%s\n", e_s[stat]));

  if (spatial) {
    IBAND b;

    b = Imake_band(Icomplex_typ, Ixsize(img[1]), Iysize(img[1]));
    fft2d(img[1], b, 1, 0.0);

    if (Ipixtyp(b) != pt) img[1] = mkConvertBand(b, pt);
    else img[1] = b;
  }

  Ihistory(img, argv[0], arg);
  Iwrite_image(img, argv[1]);

  return(0);
}

#endif /* MAIN */
