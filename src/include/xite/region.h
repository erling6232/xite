
/*
  This header-file is produced automatically (at least in part) by
  makeHeader of BLAB, Ifi, UiO.
*/


/*

________________________________________________________________

        region.h
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

#ifndef _REGION_H_
#define _REGION_H_

_XITE_CPLUSPLUS_BEGIN

#include <xite/biff.h>
#include <xite/histo.h>

#define NORTH 0
#define NORTHWEST 1
#define WEST 2
#define SOUTHWEST 3
#define SOUTH 4
#define SOUTHEAST 5
#define EAST 6
#define NORTHEAST 7

#define nilr (region*)0
#define nily (yline*)0

#define YLXSIZE(yl) ((yl)->xmax - (yl)->xmin + 1)
#define YLXSTOP(yl) (((yl)->xmax)

typedef void *ImageDataReg;
typedef int (*intfunc) _XITE_PARAMS((ImageDataReg data, int x, int y, ImageDataReg reldata));

typedef struct _yline
{
  int xmin, xmax;
  struct _yline *next;
} yline;

/* ymin - ymax = region size    */
/* amin - amax = allocated size */

typedef struct _region
{
  int objectnr, regionnr, ymin, ymax, amin, amax, sons, nc;
  struct _region *next, *previous;
  void *userdata;
  yline **ylines;
} region; 


typedef int (*regionfunc) _XITE_PARAMS((void *data1, int x, int y, void *data2));
extern _XITE_WIN_DLL int regionArea  _XITE_PARAMS(( region *reg, int loop, int *area ));
extern _XITE_WIN_DLL region* regionConvexHull  _XITE_PARAMS(( region * reg ));
extern _XITE_WIN_DLL region* regionCreate  _XITE_PARAMS(( int objnr, int regnr, int nc, int ymin, int ymax ));
extern _XITE_WIN_DLL int regionDraw  _XITE_PARAMS(( IBAND band, region * reg, int loop, int pv ));
extern _XITE_WIN_DLL void regionFree  _XITE_PARAMS(( region ** reg, int loop ));
extern _XITE_WIN_DLL int regionHistoUpdate  _XITE_PARAMS(( IBAND band, region * reg, int loop, histogram histo ));
extern _XITE_WIN_DLL int regionHisto  _XITE_PARAMS(( IBAND band, region * reg, int loop, histogram histo ));
extern _XITE_WIN_DLL int regionImoment  _XITE_PARAMS(( IBAND band, region *reg, int loop, double *moments ));
extern _XITE_WIN_DLL yline* regionInsertYline  _XITE_PARAMS(( region * reg, int y, int xmin, int xmax ));
extern _XITE_WIN_DLL yline* regionDeleteYline  _XITE_PARAMS(( region * reg, int y, int xmin, int xmax ));
extern _XITE_WIN_DLL BiffStatus regionEndInsert  _XITE_PARAMS(( region * reg ));
extern _XITE_WIN_DLL int regionXmean  _XITE_PARAMS(( IBAND band, region *reg, int loop, double *xmean ));
extern _XITE_WIN_DLL int regionYmean  _XITE_PARAMS(( IBAND band, region *reg, int loop, double *ymean ));
extern _XITE_WIN_DLL int regionMean  _XITE_PARAMS(( IBAND band, region *reg, int loop, double *xmean, double *ymean ));
extern _XITE_WIN_DLL int regionXmin  _XITE_PARAMS(( region * reg, int loop, int *xmin ));
extern _XITE_WIN_DLL int regionXmax  _XITE_PARAMS(( region * reg, int loop, int *xmax ));
extern _XITE_WIN_DLL int regionYmin  _XITE_PARAMS(( region * reg, int loop, int *ymin ));
extern _XITE_WIN_DLL int regionYmax  _XITE_PARAMS(( region * reg, int loop, int *ymax ));
extern _XITE_WIN_DLL int regionMinMax  _XITE_PARAMS(( region * reg, int loop, int *xmin, int *ymin, int *xmax, int *ymax ));
extern _XITE_WIN_DLL int regionCMoment  _XITE_PARAMS(( IBAND band, region *reg, int p, int q, int loop, double * moment ));
extern _XITE_WIN_DLL int regionPMoment  _XITE_PARAMS(( IBAND band, region *reg, int p, int q, double xc, double yc, int loop, double *moment ));
extern _XITE_WIN_DLL int regionMoment  _XITE_PARAMS(( IBAND band, region *reg, int p, int q, int loop, double * moment ));
extern _XITE_WIN_DLL BiffStatus regionPerform  _XITE_PARAMS(( void *data, region * reg, int loop, regionfunc performproc, void *performdata ));
extern _XITE_WIN_DLL int regionPerimeter  _XITE_PARAMS(( region *reg, int nc, int loop, int * perimeter ));
extern _XITE_WIN_DLL int regionIsPoint  _XITE_PARAMS(( region * reg, int loop, int x, int y ));
extern _XITE_WIN_DLL int regionPoint  _XITE_PARAMS(( region * reg, int *x, int *y ));
extern _XITE_WIN_DLL region * regionSearch  _XITE_PARAMS(( ImageDataReg data, int x, int y, int xmin, int xmax, int ymin, int ymax, int nc, intfunc relfunc, ImageDataReg reldata ));
extern _XITE_WIN_DLL int regionSort  _XITE_PARAMS(( region **regArr, region ***objArr, int rsize, int *osize ));
extern _XITE_WIN_DLL region* regionCopy  _XITE_PARAMS(( region * reg, int loop ));
extern _XITE_WIN_DLL region* regionUnion  _XITE_PARAMS(( region *reg1, region *reg2, int nc ));
extern _XITE_WIN_DLL int regionOverlap  _XITE_PARAMS(( region *r1, region *r2 ));
extern _XITE_WIN_DLL int regionNeighbor  _XITE_PARAMS(( region *reg1, region *reg2, int *overlap, int *adjacent, int nc ));
extern _XITE_WIN_DLL int regionVolume  _XITE_PARAMS(( IBAND band, region *reg, int loop, int *volume ));
extern _XITE_WIN_DLL BiffStatus regionYline  _XITE_PARAMS(( IBAND input, region ***regions, int nc, int sort, int split, int *size ));

_XITE_CPLUSPLUS_END

#endif /* _REGION_H_ */
