

/*

________________________________________________________________

        remove_small.c
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

/* static char *Id = "$Id$, Blab, UiO"; */

/*L:check_size*

________________________________________________________________

                check_size
________________________________________________________________


Name:           check_size - count number of pixels in binary print component.
Syntax:         
Description:    
Files:		src/binarize/remove_small.c

See also:                       
Diagnostics:
Return value:   | 1 - ok
Restrictions:   Works on unsigned byte bands.
Author:         Oivind Due Trier
Examples:       
Id:             $Id$
________________________________________________________________

*/


#include <xite/includes.h>
#include <xite/biff.h>
#include XITE_STDIO_H

#include <xite/blab.h>
#include <xite/histo.h>
#include <math.h>
#include <xite/binarize.h>
#include "mark_object.c"


#ifdef FUNCPROTO
static int check_size(short x, short y, IBAND inband, IBAND visited, 
	       int max_size, int *size)
#else
static int check_size(x, y, inband, visited, 
	       max_size, size)
   short x;
   short y;
   IBAND inband;
   IBAND visited;
   int max_size;
   int *size;
#endif
{
  int xsize, ysize;
  int xstart, ystart;

  xstart = x;
  ystart = y;
  xsize = Ixsize(inband);
  ysize = Iysize(inband);
  size[0] = 0;

/*  while (size[0] <= (2 * max_size)) */
  while (TRUE)
    {    
      if (((visited[y][x] & BIN_DONE_MASK) == BIN_NOT_VISITED))
	{
	  /* Increment size counter: */
	  size[0]++; 
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
		    printf("[check_size] (xstart,ystart) = (%i,%i)", 
			   xstart, ystart);
		    printf("[check_size] (x,y) = (%i,%i)", 
			   x, y);
		  }
		  
		}
	      /* All points are visited: Return */
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



/*F:remove_small*

________________________________________________________________

                remove_small
________________________________________________________________


Name:           remove_small - remove small print objects in binary image

Syntax:         | #include <xite/binarize.h>
                |
                | int remove_small( IBAND inband, IBAND outband,
                |    int size_threshold );

Description:    All print (black) objects in a binary image are traversed, 
                and if the size is smaller than 'size_threshold', then
		the object is removed from the image. 

Restrictions:   'inband' and 'outband' must have pixel type unsigned byte.

Return value:   | 0 : ok
                | 1 : Bad input pixel type
		| 2 : Bad output pixel type

Author:         Øivind Due Trier
Files:		src/binarize/remove_small.c
Id:             $Id$
________________________________________________________________

*/


#ifdef FUNCPROTO
int remove_small(IBAND inband, IBAND outband, 
		 int size_threshold)
#else
int remove_small(inband, outband, 
		 size_threshold)
     IBAND inband;
     IBAND outband;
     int size_threshold;
#endif
{
  IBAND visited, deleted;
  int x, y, xsize, ysize;
  short i, j;
  int count;

  if (Ipixtyp(inband) != Iu_byte_typ)
    return(Error(1,
		 "remove_small: Input pixel type must be unsigned byte.\n"));
  if (Ipixtyp(outband) != Iu_byte_typ)
    return(Error(2,
		 "remove_small: Output pixel type must be unsigned byte.\n"));

  xsize = Ixsize(inband);
  ysize = Iysize(inband);
  
  visited   = Imake_band(Iu_byte_typ, xsize, ysize);
  deleted   = Imake_band(Iu_byte_typ, xsize, ysize);

  for (x=1; x<=xsize; x++)	
    for (y=1; y<=ysize; y++)
      {
	visited[y][x] = BIN_NOT_VISITED;
	deleted[y][x] = BIN_NOT_VISITED;
      }

  for (x=1; x<=xsize; x++)	
    for (y=1; y<=ysize; y++)
      outband[y][x] = inband[y][x];

  for (y=1; y<=ysize; y++)
  {
     for (x=1; x<=xsize; x++)	
     {	
	if (  (visited[y][x] == BIN_NOT_VISITED)
	    &&(outband[y][x] == BIN_FORGROUND))
	{
	   count = 0;
	   
	   /*	    printf ("  --- Recursively detecting one object\n"),  */
	   i = (short)x,
	   j = (short)y;
	   check_size(i, j, outband, visited, 
		      size_threshold, &count);
	   if (Verbose()) printf("   --- Object at (%i,%i): Count=%i\n", 
				 x, y, count); 
	   
	   if (count < size_threshold)
	   {
	      if (Verbose()) printf("  --- Removing object at (%i,%i)\n",
		      x, y);
	      
	      mark_object(x, y, outband, deleted, BIN_BACKGROUND);
	   }
	}
	
        else
	   visited [y][x] = BIN_VISITED;
     }					
  }   
  /*  write_byteband(visited, "BIN_visited.img");
   */
  for (x=1; x<=xsize; x++)	
     for (y=1; y<=ysize; y++)
	if (deleted[y][x] != 0)
	   deleted [y][x] = 255;
  /*  write_byteband(deleted, "BIN_deleted.img");
   */
  Idel_band(&visited);
  Idel_band(&deleted);
  
  return 0;

} /* END remove_small() */



