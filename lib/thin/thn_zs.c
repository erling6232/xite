

/*C*

________________________________________________________________

        thn_zs.c
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

#include <math.h>
#include <stdlib.h>
#include <xite/includes.h>
#include XITE_STDIO_H
#include <xite/blab.h>
#include <xite/biff.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include <xite/binarize.h>
#include <xite/thin.h>

/*F:thn_zs*

________________________________________________________________
 
                thn_zs
________________________________________________________________


Name:           thn_zs - Thin a binary image. Algorithm by Zhang and Suen.

Syntax:         | #include <xite/thin.h>
                |
                | int thn_zs( int verbose, IBAND inband,
                |    IBAND outband );

Description:    Algorithm by Zhang and Suen.

Restrictions:   'inband' and 'outband' must have pixel type unsigned byte.
                'inband' is assumed to be binary valued, with foreground = 0,
		background = 255.

See also:       thn_zs(1), thn_har(3), thn_lc(3)

References:     &[1] T. Y. Zhang and C. Y. Suen,
                "A Fast Parallel Algorithm for Thinning Digital Patterns.",
		Comm. ACM, vol. 27, no. 3, pp 236-239, 1984.

                &[2] Rafael C. Gonzalez and Paul Wintz,
		"Digital Image Processing.",
		2. edition, 1987, pp. 398-402.

Return value:   | 0 - ok
                | 1 - Bad input pixel type
		| 2 - Bad output pixel type

Author:         Øivind Due Trier, Ifi, UiO
Files:          src/binarization/thn_zs.c
Id:             $Id$
________________________________________________________________  
*/

static int zs_get_ngb (int x, int y, IBAND inband, UNS_BYTE *p)
{
  p[1] = (inband[y][x]     == THN_FOREGROUND);
  p[2] = (inband[y-1][x]   == THN_FOREGROUND);
  p[3] = (inband[y-1][x+1] == THN_FOREGROUND);
  p[4] = (inband[y][x+1]   == THN_FOREGROUND);
  p[5] = (inband[y+1][x+1] == THN_FOREGROUND);
  p[6] = (inband[y+1][x]   == THN_FOREGROUND);
  p[7] = (inband[y+1][x-1] == THN_FOREGROUND);
  p[8] = (inband[y][x-1]   == THN_FOREGROUND);
  p[9] = (inband[y-1][x-1] == THN_FOREGROUND);

  return 1;
}


static int zs_nze_ngb (UNS_BYTE *p)
{
  int i;
  int sum = 0;
  for (i=2; i<=9; i++)
    sum += p[i];
  return sum;
}


static int zs_pos_tra (UNS_BYTE *p)
{
  int prev;
  int i;
  int result = 0;

  prev = p[9];
  for (i=2; i<=9; i++)
    {
      if ((prev == 0) && (p[i] == 1))
	result ++;
      prev = p[i];
    }
  return result;
}

static int zs_flag_bp1 (IBAND inband, IBAND flagband)
{
  int x, y;
  int xsize, ysize;
  int N, S;
  UNS_BYTE p[10];

  xsize = Ixsize(inband);
  ysize = Iysize(inband);
  
  for (x=2; x<xsize; x++)
    for (y=2; y<ysize; y++)
      {
	zs_get_ngb(x, y, inband, p);
	N = zs_nze_ngb(p);
	S = zs_pos_tra(p);
	if (   (N <= 6) && (N >= 2)
	    && (S == 1)
	    && (p[2] * p[4] * p[6] == 0)
	    && (p[4] * p[6] * p[8] == 0))
/*	    && (p[2] && p[4] && p[6] == 0)
	    && (p[4] && p[6] && p[8] == 0)) */
	  flagband[y][x] = 1;
	else
	  flagband[y][x] = 0;
	  
      }
  return 1;
}

static int zs_flag_bp2 (IBAND inband, IBAND flagband)
{
  int x, y;
  int xsize, ysize;
  int N, S;
  UNS_BYTE p[10];

  xsize = Ixsize(inband);
  ysize = Iysize(inband);
  
  for (x=2; x<xsize; x++)
    for (y=2; y<ysize; y++)
      {
	zs_get_ngb(x, y, inband, p);
	N = zs_nze_ngb(p);
	S = zs_pos_tra(p);
	if (   (N <= 6) && (N >= 2)
	    && (S == 1)
	    && (p[2] * p[4] * p[8] == 0)
	    && (p[2] * p[6] * p[8] == 0))
/*	    && (p[2] && p[4] && p[8] == 0)
	    && (p[2] && p[6] && p[8] == 0)) */
	  flagband[y][x] = 1;
	else
	  flagband[y][x] = 0;
	  
      }
  return 1;
}

static int zs_del_fbp (IBAND ioband, IBAND flagband)
{
  int delete = 0;
  int x, y;
  int xsize, ysize;

  xsize = Ixsize(ioband);
  ysize = Iysize(ioband);
  
  for (x=2; x<xsize; x++)
    for (y=2; y<ysize; y++)
      {
	if ((flagband[y][x]) && (ioband[y][x] != THN_BACKGROUND))
	  {
	    ioband[y][x] = THN_BACKGROUND;
	    delete++;
	  }
      }

  return delete;
}
 
int thn_zs(int verbose, IBAND inband, IBAND outband)
{
  int xsize, ysize;
  int x, y;
  int deleted = 1;
  IBAND thinband, flagband;

  if (Ipixtyp(inband) != Iu_byte_typ)
    return(Error(1, "Input pixel type must be unsigned byte.\n"));
  if (Ipixtyp(outband) != Iu_byte_typ)
    return(Error(2, "Output pixel type must be unsigned byte.\n"));

  xsize = Ixsize(inband);
  ysize = Iysize(inband);

  for (x=1; x<= xsize; x++)
    for (y=1; y<=ysize; y++)
      if ((inband[y][x] != THN_BACKGROUND) &&
	  (inband[y][x] != THN_FOREGROUND))
	Warning(1, "thn_zs: wrong value in pixel[%i][%i]: %i.\n",
		y, x, inband[y][x]);
  
  thinband = (IBAND)Imake_band(Iu_byte_typ, (xsize + 2), (ysize + 2));
  flagband = (IBAND)Imake_band(Iu_byte_typ, (xsize + 2), (ysize + 2));

  if (verbose)
     printf ("  --- edge expanding\n");
  expand_edge (inband, 0, 0, 1, thinband);

  if (verbose)
     printf ("  --- delete border points\n");
  deleted = 1;
  while (deleted > 0)
    {
      zs_flag_bp1 (thinband, flagband);
      deleted = zs_del_fbp (thinband, flagband);
      zs_flag_bp2 (thinband, flagband);
      deleted += zs_del_fbp (thinband, flagband);
      if (verbose)
	 printf ("     # deleted = %i\n", deleted);
    }

  for (x=1; x<=xsize; x++)
    for (y=1; y<=ysize; y++)
      outband [y][x] = thinband[y+1][x+1];

  Idel_band(&thinband);
  Idel_band(&flagband);

  return THN_OK;
}
