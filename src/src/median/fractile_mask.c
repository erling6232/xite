
/*

________________________________________________________________

        fractile_mask
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



#include <xite/includes.h>
#include <xite/biff.h>
#include <xite/median.h>
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#include <stdlib.h>
#include <xite/message.h>
#ifdef HAVE_MALLOC_H
# include <malloc.h>
#endif
#ifdef HAVE_MEMORY_H
# include <memory.h>
#endif

#ifndef MAIN

typedef int (*func)( const void *, const void* );

static func compare; 

#define COMPARE(name, type) static int name(type **a, type **b) \
{ /* fprintf (stderr, "%8d %8d\n", **a, **b); */ \
  if (**a < **b) return(-1);\
  if (**a > **b) return(1);\
  return(0);\
}

COMPARE(comp_uns_byte, unsigned char)
COMPARE(comp_sign_byte, char)
COMPARE(comp_uns_short, unsigned short)
COMPARE(comp_sign_short, short)
COMPARE(comp_integer, int)
COMPARE(comp_real, float)
COMPARE(comp_double, double)


static int install_compare(IPIXTYP pixtyp)
{
  switch((int) pixtyp)
    {
    case Iu_byte_typ:  compare = (func) comp_uns_byte;      break;
    case Is_byte_typ:  compare = (func) comp_sign_byte;     break;
    case Iu_short_typ: compare = (func) comp_uns_short;     break;
    case Is_short_typ: compare = (func) comp_sign_short;    break;
    case Integer_typ:  compare = (func) comp_integer;       break;
    case Ireal_typ:    compare = (func) comp_real;          break;
    case Idouble_typ:  compare = (func) comp_double;        break;
    default: return(Error(2, "Wrong pixel type\n"));        /* break; */
    }
  return(0);
}


 
/*F:fractile_mask*

________________________________________________________________

		fractile_mask
________________________________________________________________

Name:		fractile_mask - noise reduction filtering

Syntax:         | #include <xite/median.h>
		|
                | int fractile_mask( IBAND inband, IBAND outband,
                |    ISS_BAND mask, int mirror, double fractile );

Description:    Filters the input band into the output band
                using a sorting filter using 'mask'. Sorts all
		elements in the inimage according to mask and
		selects the 'fractile' pixel.

		If 'fractile' = 0.5, this is a median filter. If 'fractile'
		is 0.0, this is a mean filter. If 'fractile' is 1.0, this is a
		max-filter. If 'mirror' is true, the mask is mirrored with
		respect to origo before it is applied.

Restrictions:   'inband' and 'outband' must have pixeltype unsigned byte.
                All non-complex pixeltypes are handled. The two bands
                must also have the same size.

Return value:   | 0 => ok
                | 2 => input band must be of same type as output band
		| 3 => input band must be of same size as output band

Author:		Otto Milvang, BLAB, Ifi, UiO

Id:             $Id$
________________________________________________________________

*/

int fractile_mask(IBAND inband, IBAND outband, ISS_BAND mask, int mirror, double fractile)
{
  char **sortarr;
  int nelem, numelem = 0;
  int xsize = Ixsize(inband); 
  int ysize = Iysize(inband); 
  int mxsize = Ixsize((IBAND) mask); 
  int mysize = Iysize((IBAND) mask);
  int mxstart = Ixstart((IBAND) mask); 
  int mystart = Iystart((IBAND) mask);
  int mxmin, mxmax, mymin, mymax;
  int x, y, mx, my, px, py, pixlen;
  int nrank, rank, mval;
  int stat;

  Info(0, "fractile_mask, %s, %d x %d, %s, %f\n",
       Ipixname(Ipixtyp(inband)), mxsize, mysize,
       mirror ? "T" : "F", fractile);

  if (Ipixtyp(inband) != Ipixtyp(outband))
    return
      (Error(2, "Input band must be of same pixel type as output band.\n"));
  if (Ixsize(outband) != xsize || Iysize(outband) != ysize)
    return(Error(3, "Input band must be of same size as output band.\n"));

  mxmin = mxstart;
  mxmax = mxmin + mxsize -1;
  mymin = mystart;
  mymax = mymin + mysize -1;

  
  for(y=1; y<=mysize; y++) 
    for(x=1; x<=mxsize; x++)
      {
	if (mask[y][x] < 0) 
	  Error(2, "fractile_mask: Negative values in mask not legal\n");
	numelem += mask[y][x];
      }
  sortarr = (char **) malloc(numelem * sizeof(char *));
  if (fractile < 0.0) fractile = 0.0;
  if (fractile >= 1.0) fractile = 1.0 - 0.1/numelem;

  nrank = fractile * numelem;

  if((stat = install_compare(Ipixtyp(inband)))) return(stat);
  pixlen = Ipixsize(Ipixtyp(inband))/8;
  
  for(y=1; y<=ysize; y++)
    for(x=1; x<=xsize; x++)
      {
	nelem = 0;
	for(my=1; my<=mysize; my++)
	  for(mx=1; mx<=mxsize; mx++)
	    {
	      if ( (mval = mask[my][mx]) == 0) continue;
	      if (mirror)
		{
		  py = y - my + mystart;
		  px = x - mx + mxstart;
		} else {
		  py = y + my - mystart;
		  px = x + mx - mxstart;
		}
	      if (py < 1 || px < 1 || py > ysize || px > xsize) continue;
	      while(mval--) sortarr[nelem++] = (char*) &inband[py][px*pixlen];
	    }
	if (nelem != 0)
	  { 
	    qsort(sortarr, nelem, sizeof(char *), compare);
	    rank = (nelem == numelem) ? (nrank) : (fractile * nelem); 
	    memcpy(&outband[y][x*pixlen], sortarr[rank], pixlen);
	  }
      }
  free(sortarr);
  return(0);
}  /* END of fractile_mask */

#endif /* not MAIN */
