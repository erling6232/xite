
/*

________________________________________________________________

        remove_ghost.c
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

/*F:mark_ghost=remove_ghost*/
/*F:remove_inv_ghost=remove_ghost*/
/*F:mark_inv_ghost=remove_ghost*/
/*F:remove_ghost*

________________________________________________________________

                remove_ghost
________________________________________________________________


Name:           remove_ghost, remove_inv_ghost, mark_ghost, 
                mark_inv_ghost - remove (or mark) "ghost" objects from
                (or in) binary image

Syntax:         | #include <xite/binarize.h>
                |
                | int remove_ghost( IBAND segmented,
                |    IBAND original, IBAND outband,
                |    int val_step_threshold );
                | int remove_inv_ghost( IBAND segmented,
                |    IBAND original, IBAND outband,
                |    int val_step_threshold );
                | int mark_ghost( IBAND segmented,
                |    IBAND original, IBAND outband,
                |    int val_step_threshold );
                | int mark_inv_ghost( IBAND segmented,
                |    IBAND original, IBAND outband,
                |    int val_step_threshold );

Description:    The original gray scale image 'original' and a binarized
                version 'segmented' are input to the routine. 

		&'remove_ghost'
		First, 'inband' is copied to 'outband'. For all 4-connected
		black objects in 'outband', the average gradient magnitude
		value at the edge of the object is calculated. Objects having
		an average gradient below the threshold value
		'val_step_threshold' are removed from 'outband'.

		&'remove_inv_ghost'
		Works like 'remove_ghost' on 4-connected white objects.

		&'mark_ghost'
		Works like 'remove_ghost' , but instead of removing an object,
		it is colored (255 - 'avg_grad'), where 'avg_grad' is the
		average gradient value on the edge of the object. It can then
		be thresholded manually to give the final binary image.
		
		&'mark_inv_ghost'
		Combination of 'mark_ghost' and 'remove_inv_ghost'. Works like
		'remove_inv_ghost' , but instead of removing an object, it is
		colored 'avg_grad', where 'avg_grad' is the average gradient
		value on the edge of the object.

Restrictions:   Works on unsigned byte bands.

Return value:   | 0 : ok
                | 1 : Bad pixel type for segmented
		| 2 : Bad pixel type for original
		| 3 : Bad pixel type for outband

See also:       binarize(1), bin_yb(3), bin_iwr(3)

References:     &'S.D. Yanowitz & A.M. Bruckstein'
                "A New Method for Image Segmentation",
                Comput. Vision Graphics Image Proc., vol 46, no 1, pp 82-95,
                April 1989.
                
Files:		src/binarize/remove_ghost.c
Author:         Øivind Due Trier
Id:             $Id$
________________________________________________________________

*/


#include <math.h>
#include <xite/includes.h>
#include <xite/biff.h>
#include XITE_STDIO_H

#include <xite/blab.h>
#include <xite/stdiff.h>
#include <xite/convolve.h>
#include <xite/negate.h>
#include <xite/histo.h>
#include <xite/binarize.h>
#include <xite/message.h>
/* #include "write_band.c" */
#include "mark_object.c"

#ifdef FUNCPROTO
static int is_edge (IBAND inband, int x, int y)
#else
static int is_edge (inband, x, y)
   IBAND inband;
   int x;
   int y;
#endif

{
  int result;
  int xsize, ysize;
  
  xsize = Ixsize(inband);
  ysize = Iysize(inband);

  result = 0;
  if (inband[y][x] == BIN_BACKGROUND)
    result = 0;
  else if ((x==1)||(y==1)||(x==xsize)||(y==ysize))
    result = 1;
  else if (  (inband[y][x+1] == BIN_BACKGROUND)
	   ||(inband[y][x-1] == BIN_BACKGROUND)
	   ||(inband[y-1][x] == BIN_BACKGROUND)
	   ||(inband[y+1][x] == BIN_BACKGROUND))
    result = 1;

  return result ;
}




#ifdef FUNCPROTO
static int sum_edge_grad(short x, short y, IBAND inband, IBAND visited, 
		  IBAND magnitude, 
		  double *sum, int *edge_count)
#else
static int sum_edge_grad(x, y, inband, visited, 
		  magnitude, 
		  sum, edge_count)
     short x;
     short y;
     IBAND inband;
     IBAND visited;
     IBAND magnitude;
     double *sum;
     int *edge_count;
#endif
{
  int xsize, ysize;
  int xstart, ystart;

  xstart = x;
  ystart = y;
  xsize = Ixsize(inband);
  ysize = Iysize(inband);


  while (TRUE)
    {    
      if (   (is_edge(inband,x,y)) 
	  && ((visited[y][x] & BIN_DONE_MASK) == BIN_NOT_VISITED))
	{
	  sum[0] += magnitude[y][x];
	  edge_count[0]++;
/*	  printf("+"); */
	}

      if ((x>1) && ((visited[y][x] & BIN_LEFT_DONE) == 0) &&
	  (inband[y][x-1] == BIN_FORGROUND) && (visited[y][x-1] == 0))
	{
/*	  printf("<"); */
	  visited[y][x] = visited[y][x] | BIN_LEFT_DONE;
	  x = x - 1;
	  visited[y][x] = BIN_FROM_RIGHT;
	}
      else if ((x<xsize) && ((visited[y][x] & BIN_RIGHT_DONE) == 0) &&
	  (inband[y][x+1] == BIN_FORGROUND) && (visited[y][x+1] == 0))
	{
/*	  printf(">"); */
	  visited[y][x] = visited[y][x] | BIN_RIGHT_DONE;
	  x = x + 1;
	  visited[y][x] = BIN_FROM_LEFT;
	}
      else if ((y>1) && ((visited[y][x] & BIN_UP_DONE) == 0) &&
	  (inband[y-1][x] == BIN_FORGROUND) && (visited[y-1][x] == 0))
	{
/*	  printf("^"); */
	  visited[y][x] = visited[y][x] | BIN_UP_DONE;
	  y = y - 1;
	  visited[y][x] = BIN_FROM_BELOW;
	}
      else if ((y<ysize) && ((visited[y][x] & BIN_DOWN_DONE) == 0) &&
	  (inband[y+1][x] == BIN_FORGROUND) && (visited[y+1][x] == 0))
	{
/*	  printf("v"); */
	  visited[y][x] = visited[y][x] | BIN_DOWN_DONE;
	  y = y + 1;
	  visited[y][x] = BIN_FROM_ABOVE;
	}
      else /* Go back to preceding position. */
	{
/*	  printf("-"); */
	  switch (visited[y][x] & BIN_DIR_MASK)
	    {
	    case 0:
/*	      printf("o\n");	       */
	      if ((x != xstart) || (y != ystart))
		{
		  if (Verbose()) {
		    printf("[sum_edge_grad] (xstart,ystart) = (%i,%i)", 
			   xstart, ystart);
		    printf("[sum_edge_grad] (x,y) = (%i,%i)", 
			   x, y);
		  }
		  
		}
	      return 1;
	      /* break; */
	    case BIN_FROM_LEFT:
	      x=x-1;	
	      break;
	    case BIN_FROM_RIGHT:
	      x=x+1;
	      break;
	    case BIN_FROM_ABOVE:
	      y=y-1;
	      break;
	    case BIN_FROM_BELOW:
	      y=y+1;
	      break;
	    }
	}
    }
}



#ifdef FUNCPROTO
static int remove_object(int x, int y, IBAND ioband, IBAND visited)
#else
static int remove_object(x, y, ioband, visited)
     int x;
     int y;
     IBAND ioband;
     IBAND visited;
#endif
{
  int xsize, ysize;
  int xstart, ystart;

  xstart = x;
  ystart = y;
  xsize = Ixsize(ioband);
  ysize = Iysize(ioband);


  while (TRUE)
    {    
      ioband[y][x] = BIN_BACKGROUND;

      if ((x>1) && ((visited[y][x] & BIN_LEFT_DONE) == 0) &&
	  (ioband[y][x-1] == BIN_FORGROUND) && (visited[y][x-1] == 0))
	{
	  visited[y][x] = visited[y][x] | BIN_LEFT_DONE;
	  x = x - 1;
	  visited[y][x] = BIN_FROM_RIGHT;
	}
      else if ((x<xsize) && ((visited[y][x] & BIN_RIGHT_DONE) == 0) &&
	       (ioband[y][x+1] == BIN_FORGROUND) && (visited[y][x+1] == 0))
	{
	  visited[y][x] = visited[y][x] | BIN_RIGHT_DONE;
	  x = x + 1;
	  visited[y][x] = BIN_FROM_LEFT;
	}
      else if ((y>1) && ((visited[y][x] & BIN_UP_DONE) == 0) &&
	       (ioband[y-1][x] == BIN_FORGROUND) && (visited[y-1][x] == 0))
	{
	  visited[y][x] = visited[y][x] | BIN_UP_DONE;
	  y = y - 1;
	  visited[y][x] = BIN_FROM_BELOW;
	}
      else if ((y<ysize) && ((visited[y][x] & BIN_DOWN_DONE) == 0) &&
	       (ioband[y+1][x] == BIN_FORGROUND) && (visited[y+1][x] == 0))
	{
	  visited[y][x] = visited[y][x] | BIN_DOWN_DONE;
	  y = y + 1;
	  visited[y][x] = BIN_FROM_ABOVE;
	}
      else /* Go back to preceding position. */
	{
	  switch (visited[y][x] & BIN_DIR_MASK)
	    {
	    case 0: 
	      if ((x != xstart) || (y != ystart))
		{
		  if (Verbose()) {
		    printf("[remove_object] (xstart,ystart) = (%i,%i)", 
			   xstart, ystart);
		    printf("[remove_object] (x,y) = (%i,%i)", 
			   x, y);
		  }
		}
	      return 1;
	      /* break; */
	    case BIN_FROM_LEFT:
	      x=x-1;	
	      break;
	    case BIN_FROM_RIGHT:
	      x=x+1;
	      break;
	    case BIN_FROM_ABOVE:
	      y=y-1;
	      break;
	    case BIN_FROM_BELOW:
	      y=y+1;
	      break;
	    default:
	      Warning(1, "Illegal value of visited[%i][%i] = %3i + %2i\n",
		      x, y, visited[y][x] & BIN_DIR_MASK, 
		      visited[y][x] - (visited[y][x] & BIN_DIR_MASK));
	    }
	}
    }
} /* END remove_object */



#ifdef FUNCPROTO
int remove_ghost(IBAND segmented, IBAND original, IBAND outband, 
		 int val_step_threshold)
#else
int remove_ghost(segmented, original, outband, 
		 val_step_threshold)
     IBAND segmented;
     IBAND original;
     IBAND outband;
     int val_step_threshold;
#endif
{
  IBAND smoothed;
  IBAND visited, deleted;
  IBAND magnitude, angle, pseudo;
  int x, y, xsize, ysize;
  short i, j;
  int count;
/*   int tot_count; */
  double score;

  if (Ipixtyp(segmented) != Iu_byte_typ)
    return(Error(1,
	      "remove_ghost: segmented pixel type must be unsigned byte.\n"));
  if (Ipixtyp(original) != Iu_byte_typ)
    return(Error(2,
	      "remove_ghost: original pixel type must be unsigned byte.\n"));
  if (Ipixtyp(outband) != Iu_byte_typ)
    return(Error(3,
	      "remove_ghost: Output pixel type must be unsigned byte.\n"));

  xsize = Ixsize(segmented);
  ysize = Iysize(segmented);
  
  smoothed  = Imake_band(Iu_byte_typ, xsize, ysize);
  visited   = Imake_band(Iu_byte_typ, xsize, ysize);
  deleted   = Imake_band(Iu_byte_typ, xsize, ysize);

  magnitude = Imake_band(Iu_byte_typ, xsize, ysize);
  angle     = Imake_band(Iu_byte_typ, xsize, ysize);
  pseudo    = Imake_band(Iu_byte_typ, xsize, ysize);
  
  mean (original, smoothed, 3, 3);
  sobel (smoothed, magnitude, angle, pseudo, 0);
  
  for (x=1; x<=xsize; x++)	
    for (y=1; y<=ysize; y++)
      {
	visited[y][x] = BIN_NOT_VISITED;
	deleted[y][x] = BIN_NOT_VISITED;
      }

  for (x=1; x<=xsize; x++)	
    for (y=1; y<=ysize; y++)
      outband[y][x] = segmented[y][x];

  for (x=1; x<=xsize; x++)	
    for (y=1; y<=ysize; y++)
      {
	if (  (visited[y][x] == BIN_NOT_VISITED)
	    &&(outband[y][x] == BIN_FORGROUND))
	  {
	    count = 0;
	    score = 0.0;
	    
/*	    printf ("  --- Recursively detecting one object\n"),  */
	    i = (short)x,
	    j = (short)y;
	    sum_edge_grad(i, j, outband, visited, 
			  magnitude, &score, &count);
/*	    printf ("   --- Object at (%i,%i): Count=%i, Score=%9.3f\n", 
	            x, y, count, score); 
*/	
	    if ((count == 0) || ((score/count) < val_step_threshold))
	      {
/*	        printf ("  --- Removing ghost object at (%i,%i)\n",
	                x, y);
*/
		remove_object(x, y, outband, deleted);
	      }
	  }

        else
          visited [y][x] = BIN_VISITED;
      }					

/*  write_byteband(visited, "BIN_visited.img");
*/
  for (x=1; x<=xsize; x++)	
    for (y=1; y<=ysize; y++)
      if (deleted[y][x] != 0)
	deleted [y][x] = 255;
/*  write_byteband(deleted, "BIN_deleted.img");
*/
  Idel_band(&smoothed);
  Idel_band(&visited);
  Idel_band(&deleted);

  Idel_band(&magnitude);
  Idel_band(&angle);
  Idel_band(&pseudo);

  return 0;
} /* END remove_ghost() */

#ifdef FUNCPROTO
int mark_ghost(IBAND segmented, IBAND original, IBAND outband, 
	       int val_step_threshold)
#else
int mark_ghost(segmented, original, outband, 
	       val_step_threshold)
     IBAND segmented;
     IBAND original;
     IBAND outband;
     int val_step_threshold;
#endif
{
  IBAND edges;
  IBAND visited, deleted;
  IBAND magnitude, angle, pseudo;
  IBAND smoothed;
  int x, y, xsize, ysize;
  short i, j;
  int count;
/*   int tot_count; */
  double score;
  int avg_grad;

  if (Ipixtyp(segmented) != Iu_byte_typ)
    return(Error(1,
	      "mark_ghost: segmented pixel type must be unsigned byte.\n"));
  if (Ipixtyp(original) != Iu_byte_typ)
    return(Error(2,
	      "mark_ghost: original pixel type must be unsigned byte.\n"));
  if (Ipixtyp(outband) != Iu_byte_typ)
    return(Error(3,
	      "mark_ghost: Output pixel type must be unsigned byte.\n"));

  xsize = Ixsize(segmented);
  ysize = Iysize(segmented);
  
  edges    = Imake_band(Iu_byte_typ, xsize, ysize);
  visited  = Imake_band(Iu_byte_typ, xsize, ysize);
  deleted  = Imake_band(Iu_byte_typ, xsize, ysize);

  smoothed  = Imake_band(Iu_byte_typ, xsize, ysize);
  magnitude = Imake_band(Iu_byte_typ, xsize, ysize);
  angle     = Imake_band(Iu_byte_typ, xsize, ysize);
  pseudo    = Imake_band(Iu_byte_typ, xsize, ysize);
  
  mean (original, smoothed, 3, 3);
  sobel (smoothed, magnitude, angle, pseudo, 0);
  sobel (segmented, edges, angle, pseudo, 0);

/*  write_byteband (edges, "BIN_edges.img");
*/

  for (x=1; x<=xsize; x++)	
    for (y=1; y<=ysize; y++)
      {
	visited[y][x] = BIN_NOT_VISITED;
	deleted[y][x] = BIN_NOT_VISITED;
      }

  for (x=1; x<=xsize; x++)	
    for (y=1; y<=ysize; y++)
      outband[y][x] = segmented[y][x];

  for (x=1; x<=xsize; x++)	
    for (y=1; y<=ysize; y++)
      {
	if (  (visited[y][x] == BIN_NOT_VISITED)
	    &&(outband[y][x] == BIN_FORGROUND))
	  {
	    count = 0;
	    score = 0.0;
	    
/*	    printf ("  --- Recursively detecting one object\n"),  */
	    i = (short)x,
	    j = (short)y;
	    sum_edge_grad(i, j, outband, visited,
			  magnitude, &score, &count);
/*	    printf ("   --- Object at (%i,%i): Count=%i, Score=%9.3f\n", 
	            x, y, count, score); 
*/	
	    avg_grad = (int)(score/count);
/*	    if ((count == 0) || (avg_grad < val_step_threshold))
	      {
	        printf ("  --- Removing ghost object at (%i,%i)\n",
	                x, y);
*/
		mark_object(x, y, outband, deleted, (255-avg_grad));
/*
	      }
*/
	  }

        else
          visited [y][x] = BIN_VISITED;
      }					

/*  write_byteband(visited, "BIN_visited.img");*/
  for (x=1; x<=xsize; x++)	
    for (y=1; y<=ysize; y++)
      if (deleted[y][x] != 0)
	deleted [y][x] = 255;
/*  write_byteband(deleted, "BIN_deleted.img");*/
  Idel_band(&edges);
  Idel_band(&smoothed);
  Idel_band(&visited);
  Idel_band(&deleted);

  Idel_band(&magnitude);
  Idel_band(&angle);
  Idel_band(&pseudo);


  return 0;
} /* END mark_ghost() */

#ifdef FUNCPROTO
int  remove_inv_ghost(IBAND segmented, IBAND original, IBAND outband, 
		      int val_step_threshold)
#else
int  remove_inv_ghost(segmented, original, outband, 
		      val_step_threshold)
     IBAND segmented;
     IBAND original;
     IBAND outband;
     int val_step_threshold;
#endif
{
  IBAND inverse, inverse_out;
  int xsize, ysize;

  if (Ipixtyp(segmented) != Iu_byte_typ)
    return(Error(1,
      "remove_inv_ghost: segmented pixel type must be unsigned byte.\n"));
  if (Ipixtyp(original) != Iu_byte_typ)
    return(Error(2,
      "remove_inv_ghost: original pixel type must be unsigned byte.\n"));
  if (Ipixtyp(outband) != Iu_byte_typ)
    return(Error(3,
      "remove_inv_ghost: Output pixel type must be unsigned byte.\n"));

  xsize = Ixsize(segmented);
  ysize = Iysize(segmented);
  inverse     = Imake_band(Iu_byte_typ, xsize, ysize); 
  inverse_out = Imake_band(Iu_byte_typ, xsize, ysize); 
  negate (segmented, inverse);
/*
  printf ("[remove_inv_ghost] Calling remove_gost\n");
*/
  remove_ghost(inverse, original, inverse_out, val_step_threshold);
/*
  printf ("[remove_inv_ghost] Returned from remove_gost\n");
*/
  negate (inverse_out, outband);
  Idel_band(&inverse);
  Idel_band(&inverse_out);

  return 0;

} /* END remove_inv_ghost */

#ifdef FUNCPROTO
int  mark_inv_ghost(IBAND segmented, IBAND original, IBAND outband, 
		    int val_step_threshold)
#else
int  mark_inv_ghost(segmented, original, outband, 
		    val_step_threshold)
     IBAND segmented;
     IBAND original;
     IBAND outband;
     int val_step_threshold;
#endif
{
  IBAND inverse, inverse_out;
  int xsize, ysize;

  if (Ipixtyp(segmented) != Iu_byte_typ)
    return(Error(1,
      "mark_inv_ghost: segmented pixel type must be unsigned byte.\n"));
  if (Ipixtyp(original) != Iu_byte_typ)
    return(Error(2,
      "mark_inv_ghost: original pixel type must be unsigned byte.\n"));
  if (Ipixtyp(outband) != Iu_byte_typ)
    return(Error(3,
      "mark_inv_ghost: Output pixel type must be unsigned byte.\n"));

  xsize = Ixsize(segmented);
  ysize = Iysize(segmented);
  inverse     = Imake_band(Iu_byte_typ, xsize, ysize); 
  inverse_out = Imake_band(Iu_byte_typ, xsize, ysize); 
  negate (segmented, inverse);
/*
  printf ("[mark_inv_ghost] Calling mark_gost\n");
*/ 
  mark_ghost(inverse, original, inverse_out, val_step_threshold);
/*
  printf ("[mark_inv_ghost] Returned from mark_gost\n");
*/
  negate (inverse_out, outband);
  Idel_band(&inverse);
  Idel_band(&inverse_out);

  return 0;

} /* END mark_inv_ghost */



