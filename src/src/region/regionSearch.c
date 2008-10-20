

/*

________________________________________________________________

        regionSearch.c
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




/*F:regionSearch*

________________________________________________________________

		regionSearch
________________________________________________________________

Name:		regionSearch - search through and create a region

Syntax:         | #include <xite/region.h>
                |
                | typedef int 
		|    (*intfunc)(ImageDataReg data, int x, int y,
		|       ImageDataReg searchdata);
		|
                | region *regionSearch( ImageDataReg data, int x,
                |    int y, int xmin, int xmax, int ymin,
                |    int ymax, int nc, intfunc relfunc,
                |    ImageDataReg reldata );

Description:	Search through an area in 'data' starting in point
                'x', 'y' and limited by 'xmin','xmax', 'ymin', 'ymax',
		find all pixels in a 'nc' connected area (nc = 4/8)
		satisfying 'relfunc'. 'searchdata' is a pointer to
		the data used by 'searchfunc'.
		
Return value:   The region if ok, otherwise nil.

See also:       regionAnalyse(1), regionArea(3),
		regionConvexHull(3), regionCreate(3), regionDraw(3), 
		regionFree(3), regionHisto(3), regionHistoUpdate(3), 
		regionImoment(3), regionInsertYline(3), regionDeleteYline(3)
		regionEndInsert(3), regionXmean(3), regionYmean(3),
		regionMean(3), regionXmin(3), regionXmax(3), regionYmin(3),
		regionYmax(3), regionMinMax(3), regionCMoment(3),
		regionPMoment(3), regionMoment(3),
		regionPerform(3), regionPerimeter(3), regionIsPoint(3),
		regionPoint(3), regionSort(3), regionCopy(3), regionUnion(3),
		regionOverlap(3), regionNeighbor(3), regionVolume(3),
		regionYline(3)

Author:		Otto Milvang
________________________________________________________________

*/

#include <xite/includes.h>
#include <xite/biff.h>
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#include <xite/message.h>
#include <xite/region.h>
#ifdef HAVE_MALLOC_H
# include <malloc.h>
#endif

typedef struct
{
  ImageDataReg data, reldata, searchdata;
  int xs, xe, ys, ye;
  intfunc relfunc;
  int neighb, regionsize;
  int xmin, xmax, ymin, ymax;
  int error;
  region *reg;
} searchtype;




static int ok(region *reg, int y, int x)
{
  yline *yl;

  if (!(yl = reg->ylines[y])) return(1);
  while(yl)
    {
      if (x<yl->xmin) return(1);
      if (x<=yl->xmax) return(0);
      yl = yl->next;
    }
  return(1);
}

static int segment(searchtype *info, int x, int y, int prev_xb, int prev_xe, int down)
{  							
    							
  int lx, x1, x2, r, y2, la, lb, xs, xe;
  intfunc relfunc;
  ImageDataReg data, reldata; 						
  region *reg;
  yline  * yl;

  data = info->data;		
  relfunc = info->relfunc;
  reldata = info->reldata;
  reg = info->reg;

  if (!relfunc(data, x, y, reldata)) return(0);

  xs = info-> xs;
  xe = info-> xe;

  for(x1=x-1; x1 >= xs && relfunc(data, x1, y, reldata); x1--);
  for(x2=x+1; x2 <= xe && relfunc(data, x2, y, reldata); x2++);
  x1 ++; x2--;
  
  yl = regionInsertYline(reg, y, x1, x2);
  if (yl == NULL)
    {
      info->error = 1;
      return(Error(2, "Error in creating yline object\n"), 0);
    }
							
  info->regionsize += (x2-x1+1);	
  if ( x1 < info->xmin ) info->xmin = x1;
  if ( x2 > info->xmax ) info->xmax = x2;
  if ( y  > info->ymax ) info->ymax = y;
  if ( y  < info->ymin ) info->ymin = y;

  if ( info->neighb && (x1 > xs ) ) x1--;     		
  if ( info->neighb && (x2 < xe) ) x2++;

  for (r = -1; r<=1; r++)
    {
      y2 = (r ==  1) ? y+down : y-down;
      la = (r ==  0) ? prev_xe : x1;
      lb = (r == -1) ? prev_xb : x2;  
      if ( y2 >= info->ys &&  y2 <= info->ye)
	{
	  for ( lx = la; lx <= lb; lx++ )
	    if (ok(reg, y2, lx))
	    lx += segment(info, lx, y2, x1-1, x2+1, y2-y); 	
	}
    }
  return(x2-x);					
}						       

region * regionSearch(ImageDataReg data, int x, int y, int xmin, int xmax, int ymin, int ymax, int nc, intfunc relfunc, ImageDataReg reldata)
{
  searchtype info;
  region *reg;

  info.data = data;
  info.reldata = reldata;
  info.xs = xmin;
  info.ys = ymin;
  info.xe = xmax;
  info.ye = ymax;
  info.neighb = ( nc == 8 ) ? 1 : 0;        
  info.relfunc = relfunc;

  info.regionsize  = 0;
  info.xmin = x;
  info.xmax = x;
  info.ymin = y;
  info.ymax = y;
  info.reg = reg = regionCreate(0, 0, nc, ymin, ymax);
  if (reg == NULL) return(NULL);
  info.error = 0;

  segment(&info, x, y, x, x, -1);
  if (info.error) return(NULL);

  regionEndInsert(reg);
  return(reg);
}





