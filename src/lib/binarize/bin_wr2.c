
/*

________________________________________________________________

        bin_wr2.c
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

/*F:bin_iwr=bin_wr2*/
/*F:bin_wr2*

________________________________________________________________

                bin_wr2
________________________________________________________________


Name:           bin_wr2, bin_iwr - binarize a gray-scale band using 
                White and Rohrer`s method

Syntax:         | #include <xite/binarize.h>
                |
                | int bin_wr2( int mode, int activity_limit,
                |    double distance_limit, int mean_size,
                |    int search_vec_len, int diagonal,
                |    int max_limit, int spot_size_limit,
                |    IBAND inband, IBAND outband,
                |    int intermediate_images );
                |
                | int bin_iwr( int mode, int activity_limit,
                |    int mean_size, int max_limit,
                |    int spot_size_limit, IBAND inband,
                |    IBAND outband, int intermediate_images );
                |
Description:    The input band 'inband' is binarized using the
                'Integrated function Algorithm' of White and Rohrer, or an
		improved version. The result is returned in 'outband'.

		Several improvements and variations of the method have been
		implemented; some of these are available in 'bin_wr2', but to
		get all the improvements, 'bin_iwr' must be used followed by
		'remove_ghost'.

		White and Rohrer`s method uses a gradient-like operator called
		the activity on the image. Pixels with activity below
		'activity_limit' are labeled '0'. The other pixels are further
		tested. If the Laplacian edge operator of the pixel is
		positive, the pixel is labeled '+', otherwise '-'. That way a
		three-level label-image is constructed, with legal pixel
		values '+', '0' and '-'. The idea is that in a sequence of
		labels, edges are identified as '-+' transitions or '+-'
		transitions. Object pixels are assumed to be '+' and '0'
		labeled pixels between a '-+' and '+-' pair. For each pixel to
		be classified, a search vector of length 'search_vec_len' for
		each search direction is centered on the pixel. The '-+' and
		'+-' must be inside the seach vector to be recognized. By
		using a low value on 'search_vec_len', only lines and small
		filled areas will be captured.

		If 'spot_size_limit' > 1, then small '+'-labeled regions
		having less than 'spot_size_limit' pixels are relabeled as
		'0'-regions.
		
		By specifying 'activity_limit' = 0, Kitler and Illingworth`s
		method is used to set the activity threshold. An upper bound
		for this value is specified by 'max_activity'.

		A lower limit on the distance between the '-+' and '+-' for
		pixels to be classified as object pixels is specified by
		'distance_limit'. This can be used to eliminate the thinnest
		lines in 'inband'.

		The first improvement suggested by Trier and Taxt [1] can be
		used in 'bin_wr2':

		The input image is smoothed by a mean filter of size
		('mean_size' x 'mean_size') to remove stochastic noise. To run
		the original White and Rohrer`s method, 'mean_size' = 1 must
		be used.

		Variations on how pixels are classified as 'print' or
		'background' are selected though the 'mode' parameter:

		If 'mode' = WR_TWO_BY_TWO, the original approach is used. Two
		times two pixels are classified at a time, requiring that all
		four pixels are inside either horizontal or vertical object
		pixel sequences.

		The reasons for classifying (2 x 2) pixels at a time in the
		original implementation was partly to save computational time,
		partly to reduce noise. With faster computers, a more
		time-consuming algorithm may be allowed.
		
		If 'mode' = WR_DOUBLE_VECTORS, then the idea is that a single
		pixel being part of an object pixel sequence is not enough. It
		has to have support from at least one of its neighbors. Either
		one of its vertical neighbors and the pixel itself have to be
		inside horizontal object pixel sequences or one of its
		horizontal neighbors and the pixel itself must be inside
		vertical object pixel sequences to be recognized as object
		pixels.

		If 'mode' = WR_SINGLE_VECTOR, only a single search vector is
		used in each direction.

		If 'diagonal' >= 1, then the horizontal and vertical search
		directions are complemented with two diagonal search
		directions. If 'diagonal' == 0, then only the horizontal and
		vertical search directions are used.

		If 'intermediate_images' > 0, then some intermediate images
		are saved during the execution. This is useful for analyzing
		the method.

		&'bin_iwr'

		'bin_iwr' is an improved version of 'bin_wr2'. The
		parameter list is somewhat different. The 'mode'
		parameter is kept for future improvements, but is
		currently ignored. The 'distance limit',
		'search_vec_len', and 'diagonal' parameters have been
		discarded from the parameter list, since
		the use of  search vectors to locate print pixels in
		the three-level label image is abandoned in 'bin_iwr'.

		Instead, the following approach is used. First, all
		"'+'"-marked regions are  labeled 'print' and "'-'"-marked
		regions are marked 'backgrond'. Then, for each
		"'0'"-marked region,  the number of "'-'" and "'+'"
		pixels that are 8-connected to the region is counted.
		If the number of "'+'" pixels is in majority, the
		"'0'" -marked region is labeled 'print', otherwise
		'background'. 

		This approach prodces a lot of noise which can be seen
		as spurious print components, which must be removed
		by using 'remove_ghost' afterwards.

Restrictions:   'inband' and 'outband' must have pixel type unsigned byte.

See also:       binarize(1)
References:     &[1] 'Øivind Due Trier & Torfinn Taxt' 
		"Improvement of `integrated function algorithm` for
		binarization of document images",
		Pattern Recognition Letters, to appear.

		&[2] 'J.M. White & G.D. Rohrer'
		"Image Thresholding for Optical Character Recognition and
		Other Applications Requiring Character Image Extraction", 
		IBM Journal of Research and Development, vol 27, no 4,
		pp 400-411, July 1983.
		
Return value:   | 0 : ok
                | 1 : Bad input pixel type
		| 2 : Bad output pixel type

Files:  	src/binarize/bin_wr2.c
Author:         Øivind Due Trier
Id:             $Id$
________________________________________________________________

*/


#include <math.h>
#include <stdlib.h>
#include <xite/includes.h>
#include <xite/biff.h>
#ifdef HAVE_STDIO_H
 #include <stdio.h>
#endif
#include <xite/blab.h>
#include <xite/histo.h>
#include <xite/stdiff.h>
#include <xite/binarize.h>
#include <xite/message.h>
#include "BinM.h"
#include "write_band.c"
#include "remove_small.c"

#define WR_NULL 148
#define WR_MINUS 255
#define WR_PLUS 0
#define WR_PRINT 64
#define WR_X_DIR 1
#define WR_Y_DIR 2
#define WR_D1_DIR 3
#define WR_D2_DIR 4


static int wr_activity(IBAND inband, IBAND outband)
{
  int x, y, i, j, dx, dy;
  int xsize, ysize;
  IBAND expanded;
  II_BAND tempband;
  int max_sum = 0;
  int sum;

  xsize = Ixsize(inband);
  ysize = Iysize(inband);
  
  expanded = Imake_band(Iu_byte_typ, xsize+4, ysize+4);
  expand_edge(inband, 0, 0, 2, expanded);
  
  tempband = (II_BAND)Imake_band(Integer_typ, xsize, ysize);

  for (x=3; x<=xsize+2; x++)
    for (y=3; y<=ysize+2; y++)
      {
	sum = 0;
	for (i= -1; i<=1; i++)
	  for (j= -1; j<=1; j++)
	    {
	      dx = expanded[y+j][x+i-1] - expanded[y+j][x+i+1];
	      dy = expanded[y+j-1][x+i] - expanded[y+j+1][x+i];
	      sum += abs(dx) + abs(dy);
	    }
	if (sum>max_sum)
	  max_sum = sum;
	tempband[y-2][x-2] = sum;
      }	
  /* factor = 1.0 * max_sum / 255; */
  for (x=1; x<=xsize; x++)
     for (y=1; y<=ysize; y++)
	if (tempband[y][x] > 255)
	   outband[y][x] = 255;
	else
	   outband[y][x] = (unsigned char)tempband[y][x];
       /* outband[y][x] = (unsigned char) ((double)tempband[y][x] / factor); */
  return 1;
}



static int wr_label(IBAND inband, IBAND activity, int a_lim, 
	     IBAND outband)
{
  int x, y, i, j, ddxy;
  int xsize, ysize;
  IBAND expanded;

  xsize = Ixsize(inband);
  ysize = Iysize(inband);
  
  expanded = Imake_band(Iu_byte_typ, xsize+4, ysize+4);
  expand_edge(inband, 0, 0, 2, expanded);

  for (x=1; x<=xsize; x++)
    for (y=1; y<=ysize; y++)
      {
	if (activity[y][x] < a_lim)
	  outband[y][x] = WR_NULL;
	else
	  {
	    i = x+2;
	    j = y+2;
	    ddxy = expanded[j][i+2] + expanded[j][i-2]
	      + expanded[j+2][i] + expanded[j-2][i]
		- 4 * expanded[j][i];
	    outband[y][x] = ((ddxy>=0) ? WR_PLUS : WR_MINUS );
	  }
      }	
  return 1;
}


static int wr_stroke(IBAND labels, int i, int j, 
		     int direction, int search_vec_len)
{
  int dx = 0;
  int dy = 0;
  int first_minus_x, first_minus_y;
  int second_minus_x, second_minus_y;
  int startx, starty, endx, endy;
  int distance;

  if (labels[j][i] == WR_MINUS)
    return FALSE;


  switch(direction)
    {
    case WR_X_DIR:
      dx = 1;
      dy = 0;
      break;
    case WR_Y_DIR:
      dx = 0;
      dy = 1;
      break;
    case WR_D1_DIR:
      dx = 1;
      dy = -1;
      break;
    case WR_D2_DIR:
      dx = 1;
      dy = 1;
      break;
    default:
      Warning(2, "[wr_stroke] ERROR! direction = %i\n",
	      direction);
      break;
    }

  startx = MIN(i - dx*search_vec_len, i + dx*search_vec_len);
  starty = MIN(j - dy*search_vec_len, j + dy*search_vec_len);
  endx   = MAX(i - dx*search_vec_len, i + dx*search_vec_len);
  endy   = MAX(j - dy*search_vec_len, j + dy*search_vec_len);
/*  
  Info(0, "[wr_stroke] start = (%3i,%3i), end = (%3i,%3i). dir = %i\n",
	  startx, starty, endx, endy, direction);
*/

  first_minus_x = i - dx;
  first_minus_y = j - dy;
  while (   (first_minus_x <= endx)   && (first_minus_y <= endy)
	 && (first_minus_x >= startx) && (first_minus_y >= starty)
	 && (labels[first_minus_y][first_minus_x] != WR_MINUS) )
    {	
      first_minus_x -= dx;
      first_minus_y -= dy;
    }
  if (labels[first_minus_y][first_minus_x] != WR_MINUS)
    return 0;

  second_minus_x = i + dx;
  second_minus_y = j + dy;
  while (   (second_minus_x <= endx)   && (second_minus_y <= endy)
	 && (second_minus_x >= startx) && (second_minus_y >= starty)
	 && (labels[second_minus_y][second_minus_x] != WR_MINUS) )
    {
      second_minus_x += dx;
      second_minus_y += dy;
    }
  if (labels[second_minus_y][second_minus_x] != WR_MINUS)
    return 0;

/*
  if (  (direction == WR_X_DIR) && (second_minus_x - i < dist_limit)
      &&(i - first_minus_x < dist_limit))
    return 0;
  if (  (direction == WR_Y_DIR) && (second_minus_y - j < dist_limit)
      &&(j - first_minus_y < dist_limit))
    return 0;
*/

  /* Absolute value of (dy * (second_minus_y-first_minus_y)), since
     the two factors will have the same sign. */
  distance = MAX ( (abs(second_minus_x-first_minus_x))
		  ,(abs(second_minus_y-first_minus_y)));

  if (  (labels[ first_minus_y+dy][ first_minus_x+dx] == WR_PLUS)
      &&(labels[second_minus_y-dy][second_minus_x-dx] == WR_PLUS))
    return distance;
  else
    return FALSE;
} /* END  wr_stroke */


static int wr_dstroke(IBAND labels, int i, int j  , int direction, int search_vec_len)
{
  if (labels[j][i] != WR_MINUS)
    return wr_stroke(labels, i, j, direction, search_vec_len);
  else if (direction == WR_D1_DIR)
    return wr_stroke(labels, i+1, j-1, direction, search_vec_len);
  else
    return wr_stroke(labels, i-1, j-1, direction, search_vec_len);
}



static int wr_segment(IBAND labels, IBAND outband, int dist_limit, 
		      int search_vec_len, int diagonal)
{
  int x, y, i, j;
  int xsize, ysize;
  IBAND expanded;

  xsize = Ixsize(labels);
  ysize = Iysize(labels);
  
  expanded = Imake_band(Iu_byte_typ, 
			xsize+2*search_vec_len+2, ysize+2*search_vec_len+2);
  expand_edge(labels, 0, 0, search_vec_len+1, expanded);
  
  for (x=1; x<=xsize; x++)
    for (y=1; y<=ysize; y++)
      {
	i = x+search_vec_len;
	j = y+search_vec_len;
	
	if (   (wr_stroke(expanded, i, j  , WR_X_DIR, search_vec_len)
		>= dist_limit)
	    && (   (wr_stroke(expanded, i, j+1, WR_X_DIR, search_vec_len)
		    >= dist_limit)
		|| (wr_stroke(expanded, i, j-1, WR_X_DIR, search_vec_len)
		    >= dist_limit)))
	  {
	    outband[y][x] = 0;
	  }
	else if (   (wr_stroke(expanded, i  , j, WR_Y_DIR, search_vec_len)
		     >= dist_limit)
		 && (   (wr_stroke(expanded, i-1, j, WR_Y_DIR, search_vec_len)
			 >= dist_limit)
		     || (wr_stroke(expanded, i+1, j, WR_Y_DIR, search_vec_len)
			 >= dist_limit)))
	  {
	    outband[y][x] = 0;
	  }
	else if (diagonal)
	  {
/*
	    Info(0, "[wr_segment] diagonal = %i\n", diagonal);
*/
	    if (   (wr_stroke(expanded, i,   j, WR_D1_DIR, search_vec_len)
		    >= dist_limit)
		&& (   (wr_dstroke(expanded, i, j+1, WR_D1_DIR, search_vec_len)
			>= dist_limit)
		    || (wr_dstroke(expanded, i-1, j, WR_D1_DIR, search_vec_len)
			>= dist_limit)))
	      {
		outband[y][x] = 0;
	      }
	    else if (   (wr_stroke(expanded, i,   j, WR_D2_DIR, search_vec_len)
			 >= dist_limit)
		     && (   (wr_dstroke(expanded, i, j+1, WR_D2_DIR, 
					search_vec_len) >= dist_limit)
			 || (wr_dstroke(expanded, i+1, j, WR_D2_DIR, 
					search_vec_len) >= dist_limit)))
	      {
		outband [y][x] = 0;
	      }
	    else
	      {
		outband[y][x] = 255;
	      }
	  }
	else
	  outband[y][x] = 255;
      }
  return 1;
} /* END wr_segment */




static int wr_segment_6(IBAND labels, IBAND outband, int dist_limit, int
		 search_vec_len, int diagonal)
{
  int x, y, i, j;
  int xsize, ysize;
  IBAND expanded;

  xsize = Ixsize(labels);
  ysize = Iysize(labels);
  
  expanded = Imake_band(Iu_byte_typ, 
			xsize+2*search_vec_len+2, ysize+2*search_vec_len+2);
  expand_edge(labels, 0, 0, search_vec_len+1, expanded);
  
  for (x=1; x<=xsize; x++)
    for (y=1; y<=ysize; y++)
      {
	i = x+search_vec_len;
	j = y+search_vec_len;

	outband[y][x] = 255;

	if (wr_stroke(expanded, i, j  , WR_X_DIR, search_vec_len) >= dist_limit)
	  {
	    outband[y][x] = 0;
	  }
	else if (wr_stroke(expanded, i, j, WR_Y_DIR, search_vec_len) >= dist_limit)
	  {
	    outband[y][x] = 0;
	  }
	else if (diagonal)
	  {
	    if (wr_stroke(expanded, i, j, WR_D1_DIR, search_vec_len) >= dist_limit)
	      {
		outband[y][x] = 0;
	      }
	    else if (wr_stroke(expanded, i,   j, WR_D2_DIR, search_vec_len)
		     >= dist_limit)
	      {
		outband [y][x] = 0;
	      }
	  }	
      }
  return 1;
} /* END wr_segment_6 */

static int wr_segment_5(IBAND labels, IBAND outband, int dist_limit, 
		 int search_vec_len) 
{
  int x, y, i, j;
  int xsize, ysize;
  int xstroke, ystroke;
  IBAND expanded;


  xsize = Ixsize(labels);
  ysize = Iysize(labels);
  
  expanded = Imake_band(Iu_byte_typ, xsize+2*search_vec_len, 
			ysize+2*search_vec_len);
  expand_edge(labels, 1, 0, search_vec_len, expanded);
  
  for (x=1; x<xsize; x+=2)
     for (y=1; y<ysize; y+=2)
     {
	i = x+search_vec_len;
	j = y+search_vec_len; 
	if (  (expanded[j  ][i  ] == WR_MINUS) 
	    ||(expanded[j+1][i  ] == WR_MINUS)
	    ||(expanded[j  ][i+1] == WR_MINUS) 
	    ||(expanded[j+1][i+1] == WR_MINUS))
	{
	   outband[y][x] = outband[y+1][x] 
	      = outband[y][x+1] = outband[y+1][x+1] = 255;
	}
	else
	{
	   xstroke 
              = (  (wr_stroke(expanded, i  , j  , WR_X_DIR, search_vec_len)
                    >= dist_limit)
                 &&(wr_stroke(expanded, i+1, j+1, WR_X_DIR, search_vec_len)
                    >= dist_limit));
	   ystroke 
              = (  (wr_stroke(expanded, i  , j  , WR_Y_DIR, search_vec_len)
                    >= dist_limit)
                 &&(wr_stroke(expanded, i+1, j+1, WR_Y_DIR, search_vec_len)
                    >= dist_limit));
            if (xstroke || ystroke)
	       outband[y][x] = outband[y+1][x] 
		  = outband[y][x+1] = outband[y+1][x+1] =  0;
            else 
	       outband[y][x] = outband[y+1][x] 
		  = outband[y][x+1] = outband[y+1][x+1] = 255;
	}
     }
  return 1;
} /* END wr_segment_5 */



static int wr2_is_edge (IBAND inband, int x, int y, UNS_BYTE pixel_value)
{
  int result;
  int xsize, ysize;
  
  xsize = Ixsize(inband);
  ysize = Iysize(inband);

  result = 0;
  if (inband[y][x] != pixel_value)
    result = 0;
  else if ((x==1)||(y==1)||(x==xsize)||(y==ysize))
    result = 1;
  else if (  (inband[y][x+1] != pixel_value)
	   ||(inband[y][x-1] != pixel_value)
	   ||(inband[y-1][x] != pixel_value)
	   ||(inband[y+1][x] != pixel_value))
    result = 1;

  return result ;
}



static int wr2_chk_pix (int x, int y, IBAND inband, 
		 II_BAND backg_visited, int backg_flag,
		 int *plusses, int *minuses, int *nulls)
{
   int startx, starty, endx, endy;
   int i, j;
   int xsize, ysize;

   plusses[0] = minuses[0] = nulls[0] = 0;

   xsize = Ixsize(inband);
   ysize = Iysize(inband);

   startx = ((x==1)? x : x-1);
   endx   = ((x==xsize)? x : x+1);
   starty = ((y==1)? y : y-1);
   endy   = ((y==ysize)? y : y+1);

   for (i=startx; i<=endx; i++)
      for (j=starty; j<=endy; j++)
      {
	 if ( ((i!=x)||(j!=y)) && (backg_visited[j][i] != backg_flag))
	 {
	    backg_visited[j][i] = backg_flag;
	    switch (inband[j][i])
	    {
	    case WR_PLUS:
	    case WR_PRINT:
	       plusses[0]++;
	       break;
	    case WR_MINUS:
	       minuses[0]++;
	       break;
	    case WR_NULL:
	       nulls[0]++;
	       break;
	    default:
	       Warning(2, "wr2_chk_pix: Error! inband[%i][%i]=%i\n",
		       j, i, inband[j][i]);
	       break;
	    }
	 }
      }
   
   return BIN_OK;
} /* END wr2_chk_pix */



static int wr2_cnt_ngb(int x, int y, IBAND inband, IBAND visited, 
		II_BAND backg_visited, int backg_flag,
		UNS_BYTE pixel_value,
		double *fraction)
{
  int xsize, ysize;
  int xstart, ystart;
  int size;
  int plusses, minuses, nulls, sum_minus, sum_plus, sum_null;
  int finished;
  
  finished = 0;
  
  sum_minus = 0;
  sum_plus = 0;
  sum_null = 0;
  
  xstart = x;
  ystart = y;
  xsize = Ixsize(inband);
  ysize = Iysize(inband);
  size = 0;
  
  
  /*  while (size <= (2 * max_size)) */
  while (!finished) {    
    if (((visited[y][x] & BIN_DONE_MASK) == BIN_NOT_VISITED)) {
      size++;
      /*	  printf("+"); */
      if (wr2_is_edge(inband, x, y, pixel_value)) {
	wr2_chk_pix (x, y, inband, backg_visited, backg_flag,
		     &plusses, &minuses, &nulls);
	sum_plus += plusses;
	sum_minus += minuses;
	sum_null += nulls;
      }
    }
    
    if ((x>1) && ((visited[y][x] & BIN_LEFT_DONE) == 0) &&
	(inband[y][x-1] == pixel_value) && (visited[y][x-1] == 0)) {
      /*	  printf("<"); */
      visited[y][x] = visited[y][x] | BIN_LEFT_DONE;
      x = x - 1;
      visited[y][x] = BIN_FROM_RIGHT;
    }
    else if ((x<xsize) && ((visited[y][x] & BIN_RIGHT_DONE) == 0) &&
	     (inband[y][x+1] == pixel_value) && (visited[y][x+1] == 0)) {
      /*	  printf(">"); */
      visited[y][x] = visited[y][x] | BIN_RIGHT_DONE;
      x = x + 1;
      visited[y][x] = BIN_FROM_LEFT;
    }
    else if ((y>1) && ((visited[y][x] & BIN_UP_DONE) == 0) &&
	     (inband[y-1][x] == pixel_value) && (visited[y-1][x] == 0)) {
      /*	  printf("^"); */
      visited[y][x] = visited[y][x] | BIN_UP_DONE;
      y = y - 1;
      visited[y][x] = BIN_FROM_BELOW;
    }
    else if ((y<ysize) && ((visited[y][x] & BIN_DOWN_DONE) == 0) &&
	     (inband[y+1][x] == pixel_value) && (visited[y+1][x] == 0)) {
      /*	  printf("v"); */
      visited[y][x] = visited[y][x] | BIN_DOWN_DONE;
      y = y + 1;
      visited[y][x] = BIN_FROM_ABOVE;
    }
    else { /* Go back to preceding position. */
      /*	  printf("-"); */
      switch (visited[y][x] & BIN_DIR_MASK) {
      case 0:
	/*	      printf("o\n");	       */
	if ((x != xstart) || (y != ystart)) {
	  if (Verbose()) printf(
        "[check_size] (xstart,ystart) = (%i,%i), [check_size] (x,y) = (%i,%i)",
	       xstart, ystart, x, y);
	  
	}
	/* All points are visited: Return */
	finished = 1;
	break;
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
  switch(pixel_value) {
  case WR_NULL:
    fraction[0] = (1.0*sum_plus)/(1.0*(sum_plus+sum_minus));
    break;
  case WR_PLUS:
    fraction[0] = (1.0*sum_minus)/(1.0*(sum_null+sum_minus));
    break;
  case WR_MINUS:
    fraction[0] = (1.0*sum_null)/(1.0*(sum_plus+sum_null));
    if (Verbose()) printf("[wr2_cnt_ngb] Warning: pixel_value = WR_MINUS.\n");
    break;
    
  default:
    Error(2, "wr2_cnt_ngb: Wrong pixel_value = %i\n", pixel_value);
    exit (-1);
    break;
  }
  return BIN_OK;
} /* END wr2_cnt_ngb */



static int wr2_chk_obj(IBAND inband, IBAND outband, UNS_BYTE pixel_value, 
		UNS_BYTE mark_value,
		double threshold)
{
  int x, y, xsize, ysize;
  double fraction;
  IBAND Visited;
  IBAND deleted;
  II_BAND backg_visited;
  int backg_flag;

  xsize = Ixsize(inband);
  ysize = Iysize(inband);
  
  deleted   = (IBAND) Imake_band(Iu_byte_typ, xsize, ysize);
  Visited   = (IBAND) Imake_band(Iu_byte_typ, xsize, ysize);
  backg_visited   = (II_BAND) Imake_band(Integer_typ, xsize, ysize);

  for (x=1; x<=xsize; x++)	
    for (y=1; y<=ysize; y++)
      {
	Visited[y][x] = BIN_NOT_VISITED;
	deleted[y][x] = BIN_NOT_VISITED;
	backg_visited[y][x] = 0;
      }

  for (x=1; x<=xsize; x++)	
    for (y=1; y<=ysize; y++)
      outband[y][x] = inband[y][x];

  write_byteband (Visited, "Visited.img");

  backg_flag = 1;
  for (y=1; y<=ysize; y++)
  {
     for (x=1; x<=xsize; x++)	
     {	
	if (  (Visited[y][x] == BIN_NOT_VISITED)
	    &&(outband[y][x] == pixel_value))
	{
	   
	   /* Info(0, "[wr2_chk_obj] Detecting one object\n"); */
	   wr2_cnt_ngb(x, y, outband, Visited, backg_visited, backg_flag,
		       pixel_value, &fraction);
	   backg_flag++;
	   /* Info(0, "[wr2_chk_obj] Object at (%i,%i): fraction=%f\n", 
	      x, y, fraction); */
	   
	   if (fraction > threshold)
	   {
	      /* printf ("[wr2_chk_obj] Marking object (type %i [=%i?]) ",
		 outband[y][x], pixel_value);
		 printf ("at (%i,%i) as %i\n",
		 x, y, mark_value);
		 */
	      mark_object(x, y, outband, deleted, mark_value);
	      /* write_byteband(outband, "WR_partly_marked.img"); */
	   }
	}
	
        else
	   Visited[y][x] = BIN_VISITED;
     }					
  }   
  /*  write_byteband(Visited, "BIN_visited.img");
   */
  for (x=1; x<=xsize; x++)	
     for (y=1; y<=ysize; y++)
	if (deleted[y][x] != 0)
	   deleted [y][x] = 255;
  /*  write_byteband(deleted, "BIN_deleted.img");
   */
  Idel_band((IBAND*)&backg_visited);
  Idel_band(&Visited);
  Idel_band(&deleted);
  
  return 1;
} /* END wr2_chk_obj() */



static int wr2_seg_lab(int mode, IBAND labels, IBAND outband, 
		double null_threshold, double plus_threshold, 
		int intermediate_images)
{
   IBAND tempband1;
   IBAND tempband2;
   int xsize, ysize;
   int x, y;
   
   xsize = Ixsize(labels);
   ysize = Iysize(labels);
   tempband1 = (IBAND) Imake_band(Iu_byte_typ, xsize, ysize);
   tempband2 = (IBAND) Imake_band(Iu_byte_typ, xsize, ysize);

   if ((mode & WR_NULL_MASK) == WR_NULL_MASK)
   {
      if (Verbose()) printf("[wr2_seg_lab] Checking WR_NULL-marked objects\n");
      wr2_chk_obj(labels, tempband1, WR_NULL, WR_PLUS, null_threshold);
      if (intermediate_images)
	 write_byteband (tempband1, "WR_label_null_check.img");
   }
   else
      Icopy_band(labels, tempband1);

   if ((mode & WR_PLUS_MASK) == WR_PLUS_MASK)
   {
      if (Verbose()) printf("[wr2_seg_lab] Checking WR_PLUS-marked objects\n");
      wr2_chk_obj(tempband1, tempband2, WR_PLUS, WR_PRINT, plus_threshold);
      if (intermediate_images)
	 write_byteband (tempband2, "WR_label_plus_check.img");

      for (y=1; y<=ysize; y++)
	 for (x=1; x<=xsize; x++)
	    if (tempband2[y][x] == WR_PRINT)
	       outband[y][x] = BIN_FOREGROUND;
	    else
	       outband[y][x] = BIN_BACKGROUND;
   }
   else
   {
      for (y=1; y<=ysize; y++)
	 for (x=1; x<=xsize; x++)
	    if (tempband1[y][x] == WR_PLUS)
	       outband[y][x] = BIN_FOREGROUND;
	    else
	       outband[y][x] = BIN_BACKGROUND;
   }
   Idel_band(&tempband1);
   Idel_band(&tempband2);
   return BIN_OK;
}



/****************************************************************
 *
 *   int  bin_wr2
 *
 ****************************************************************/

int bin_wr2 (int mode, int activity_limit, 
	     double distance_limit, int mean_size,
	     int search_vec_len, int diagonal, int max_limit,
	     int spot_size_limit,
	     IBAND inband, IBAND outband, int intermediate_images)
{
   int xsize, ysize;
   int x, y;
   int dist_lim;
   
   
   IBAND activity;
   IBAND label;
   IBAND smoothed;
   IBAND segmented;
   IBAND segmented_2;
   IBAND tempband;
   double dummy_0, dummy_1, dummy_T;

   if (Ipixtyp(inband) != Iu_byte_typ)
     return(Error(1, "bin_wr2: Input pixel type must be unsigned byte.\n"));
   if (Ipixtyp(outband) != Iu_byte_typ)
     return(Error(2, "bin_wr2: Output pixel type must be unsigned byte.\n"));

   search_vec_len = search_vec_len / 2;
   dist_lim = (int) distance_limit;
   
   xsize = Ixsize(inband);
   ysize = Iysize(inband);
   
   activity     = Imake_band(Iu_byte_typ, xsize, ysize);
   label        = Imake_band(Iu_byte_typ, xsize, ysize);
   smoothed     = Imake_band(Iu_byte_typ, xsize, ysize);
   tempband     = Imake_band(Iu_byte_typ, xsize, ysize);
   segmented    = Imake_band(Iu_byte_typ, xsize, ysize);
   segmented_2  = Imake_band(Iu_byte_typ, xsize, ysize);
   
   mean(inband, smoothed, mean_size, mean_size);
   if (intermediate_images) 
      write_byteband(smoothed, "WR_smoothed.img");

   wr_activity(smoothed, activity);
   if (intermediate_images) write_byteband (activity, "WR_activity.img");
   
   if (activity_limit < 0)
   {
      activity_limit = kittler_threshold(activity, 
					 &dummy_0, &dummy_1, &dummy_T);
      if (Verbose()) printf(
 "Automatically selected activity limit = %i. (Kittler/Illingworth's threshold)\n",
	       activity_limit);
      if (activity_limit > max_limit)
      {
	 activity_limit = max_limit;
	 if (Verbose())
	   printf("Selected activity limit > max_limit, new value = %i.\n",
		  activity_limit);
      }
   }
   else
      if (Verbose()) printf("Manually selected activity limit=%i.\n", activity_limit);
   
   wr_label(smoothed, activity, activity_limit, label);
   write_byteband(label, "WR_label.img"); 
   
   if (spot_size_limit > 1)
   {
      bin_man (WR_PLUS+1, label, segmented);
      if (intermediate_images) 
	 write_byteband(segmented, "WR_binary_plus_labels.img");
      remove_small (segmented, segmented_2, spot_size_limit);
      if (intermediate_images) 
	 write_byteband(segmented_2, "WR_rem_small_negated.img");
      for (x=1; x<=xsize; x++)
	 for (y=1; y<=ysize; y++)
	    if (segmented[y][x] != segmented_2[y][x])
	       label[y][x] = WR_NULL;
      if (intermediate_images) 
	 write_byteband(label, "WR_spot_size.img");
   }
   
   if (mode == WR_OLD)
      wr_segment_5(label, outband, dist_lim, search_vec_len);
   else if (mode == WR_DOUBLE_VECTORS)
      wr_segment(label, outband, dist_lim, search_vec_len, diagonal);
   else /* mode == WR_SINGLE_VECTOR */
      wr_segment_6(label, outband, dist_lim, 
		   search_vec_len, diagonal);
   Idel_band (&activity);
   Idel_band (&label);
   Idel_band (&smoothed);
   Idel_band (&tempband);
   Idel_band (&segmented);
   Idel_band (&segmented_2);
   return 0;
}


/****************************************************************
 *
 *   int  bin_iwr
 *
 ****************************************************************/

int bin_iwr (int mode, int activity_limit, 
	     int mean_size,
	     int max_limit,
	     int spot_size_limit,
	     IBAND inband, IBAND outband, int intermediate_images)
{
   int xsize, ysize;
   int x, y;
/*    int dist_lim; */
   
   
   IBAND activity;
   IBAND label;
   IBAND smoothed;
   IBAND segmented;
   IBAND segmented_2;
/*   IBAND smallband; */
   IBAND tempband;
   double dummy_0, dummy_1, dummy_T;
   
   if (Ipixtyp(inband) != Iu_byte_typ)
     return(Error(1, "bin_iwr: Input pixel type must be unsigned byte.\n"));
   if (Ipixtyp(outband) != Iu_byte_typ)
     return(Error(2, "bin_iwr: Output pixel type must be unsigned byte.\n"));
   
   xsize = Ixsize(inband);
   ysize = Iysize(inband);
   
   activity     = Imake_band(Iu_byte_typ, xsize, ysize);
   label        = Imake_band(Iu_byte_typ, xsize, ysize);
   smoothed     = Imake_band(Iu_byte_typ, xsize, ysize);
   tempband     = Imake_band(Iu_byte_typ, xsize, ysize);
   segmented    = Imake_band(Iu_byte_typ, xsize, ysize);
   segmented_2  = Imake_band(Iu_byte_typ, xsize, ysize);
   
   mean(inband, smoothed, mean_size, mean_size);
   if (intermediate_images) 
      write_byteband(smoothed, "WR_smoothed.img");

   wr_activity(smoothed, activity);
   if (intermediate_images) write_byteband (activity, "WR_activity.img");
   
   if (activity_limit < 0)
   {
      activity_limit = kittler_threshold(activity, 
					 &dummy_0, &dummy_1, &dummy_T);
      if (Verbose())
	printf("%s%i%s\n",
	       "Automatically selected activity limit = ",
	       activity_limit,
	       ". (Kittler/Illingworth's threshold).");
      if (activity_limit > max_limit)
      {
	 activity_limit = max_limit;
	 if (Verbose())
	   printf("Selected activity limit > max_limit, new value = %i.\n",
		  activity_limit);
      }
   }
   else
      if (Verbose()) printf("Manually selected activity limit=%i.\n",
			    activity_limit);
   
   wr_label(smoothed, activity, activity_limit, label);
   write_byteband(label, "WR_label.img"); 
   
   if (spot_size_limit > 1)
   {
      bin_man (WR_PLUS+1, label, segmented);
      if (intermediate_images) 
	 write_byteband(segmented, "WR_binary_plus_labels.img");
      remove_small (segmented, segmented_2, spot_size_limit);
      if (intermediate_images) 
	 write_byteband(segmented_2, "WR_rem_small_negated.img");
      for (x=1; x<=xsize; x++)
	 for (y=1; y<=ysize; y++)
	    if (segmented[y][x] != segmented_2[y][x])
	       label[y][x] = WR_NULL;
      if (intermediate_images) 
	 write_byteband(label, "WR_spot_size.img");
   }
   
   wr2_seg_lab(mode, label, outband, 0.5, 0.5, intermediate_images);

   Idel_band (&activity);
   Idel_band (&label);
   Idel_band (&smoothed);
   Idel_band (&tempband);
   Idel_band (&segmented);
   Idel_band (&segmented_2);
   return BIN_OK;
} /* END bin_iwr() */



/************************************************/
/*                                              */
/*                                              */
/* some old functions that are not used anymore */
/*                                              */
/*                                              */
/************************************************/



/* 
static int wr_stroke_x(IBAND labels, int i, int j, int search_vec_len)
{
  int first_minus;
  int second_minus;
  int stroke_found = FALSE;
  int start, end;

  if (labels[j][i] == WR_MINUS)
    return FALSE;

  start = i - search_vec_len;
  end   = i + search_vec_len;
  
  first_minus = start;
  
  while (!stroke_found)
    {
  
      while (   (first_minus < i)
	     && (labels[j][first_minus] != WR_MINUS) )
	first_minus ++;
      if (first_minus >= i-1)
	return FALSE;

      if (labels[j][first_minus] == WR_PLUS)
	{
	  second_minus= first_minus + 2;
	  while (   (second_minus < end) 
		 && (labels[j][second_minus] != WR_MINUS) )
	    second_minus ++;
	  if (   (second_minus > i+1) 
	      && (labels[j][second_minus-1] == WR_PLUS))
	    stroke_found = TRUE;
	}
      first_minus ++;
    }
  return stroke_found;
}




static int wr_stroke_y(IBAND labels, int i, int j, int search_vec_len)
{
  int first_minus;
  int second_minus;
  int stroke_found = FALSE;
  int start, end;

  if (labels[j][i] == WR_MINUS)
    return FALSE;

  start = j - search_vec_len;
  end   = j + search_vec_len;
  
  first_minus = start;
  
  while (!stroke_found)
    {	  
      while (   (first_minus < j)
	     && (labels[first_minus][i] != WR_MINUS) )
	first_minus ++;
      if (first_minus >= j-1)
	return FALSE;

      if (labels[first_minus][i] == WR_PLUS)
	{
	  second_minus= first_minus + 2;
	  while (   (second_minus < end) 
		 && (labels[second_minus][i] != WR_MINUS) )
	    second_minus ++;
	  if (   (second_minus > j+1) 
	      && (labels[second_minus-1][i] == WR_PLUS))
	    stroke_found = TRUE;
	}
      first_minus ++;
    }
  return stroke_found;
}
*/






/* 
static int wr_segment_3(IBAND labels, IBAND outband, int dist_limit, 
			int search_vec_len)
{
  int x, y, i, j;
  int xsize, ysize;
  int xstroke, ystroke;


  IBAND expanded;


  xsize = Ixsize(labels);
  ysize = Iysize(labels);
  
  expanded = Imake_band(Iu_byte_typ, xsize+2*search_vec_len, ysize+2*search_vec_len);
  expand_edge(labels, 0, 0, search_vec_len, expanded);
  
  for (x=1; x<=xsize; x++)
    for (y=1; y<=ysize; y++)
      {
	i = x+search_vec_len;
	j = y+search_vec_len;
	xstroke = wr_stroke(expanded, i, j  , WR_X_DIR, search_vec_len);
	ystroke = wr_stroke(expanded, i  , j, WR_Y_DIR, search_vec_len);
	if ((xstroke>=dist_limit) || (ystroke>=dist_limit))
	  outband[y][x] = 0;
	else 
	  outband[y][x] = 255;
      }
  return 1;
} \* END wr_segment_3 *\





static int wr_segment_1(IBAND labels, IBAND outband, int dist_limit, 
			int search_vec_len) 
{
  int x, y, i, j;
  int xsize, ysize;
  int xstroke, ystroke;
  IBAND expanded;


  xsize = Ixsize(labels);
  ysize = Iysize(labels);
  
  expanded = Imake_band(Iu_byte_typ, xsize+2*search_vec_len, ysize+2*search_vec_len);
  expand_edge(labels, 0, 0, search_vec_len, expanded);
  
  for (x=1; x<=xsize; x++)
    for (y=1; y<=ysize; y++)
      {
	i = x+search_vec_len;
	j = y+search_vec_len;
	xstroke = (wr_stroke(expanded, i, j, WR_X_DIR, search_vec_len) 
		   >= dist_limit);
	ystroke = (wr_stroke(expanded, i, j, WR_Y_DIR, search_vec_len)
		   >= dist_limit);
	if (xstroke != ystroke)
	  outband[y][x] = 0;
	else 
	  outband[y][x] = 255;
      }
  return 1;
} \* END wr_segment_1 *\





static int wr_segment_2(IBAND labels, IBAND outband, int dist_limit, 
			int search_vec_len)
{
  int x, y, i, j;
  int xsize, ysize;
  int xstroke, ystroke;
  IBAND expanded;


  xsize = Ixsize(labels);
  ysize = Iysize(labels);
  
  expanded = Imake_band(Iu_byte_typ, xsize+2*search_vec_len, ysize+2*search_vec_len);
  expand_edge(labels, 0, 0, search_vec_len, expanded);
  
  for (x=1; x<xsize; x+=2)
    for (y=1; y<ysize; y+=2)
      {
	i = x+search_vec_len;
	j = y+search_vec_len;
	if (  (expanded[j  ][i  ] == WR_MINUS) 
	    ||(expanded[j+1][i  ] == WR_MINUS)
	    ||(expanded[j  ][i+1] == WR_MINUS) 
	    ||(expanded[j+1][i+1] == WR_MINUS))
	  {
	    outband[y][x] = outband[y+1][x] 
	      = outband[y][x+1] = outband[y+1][x+1] = 255;
	  }
	else
	  {
	    xstroke 
	      = (  (wr_stroke(expanded, i  , j  , WR_X_DIR, search_vec_len)
		    >= dist_limit)
		 &&(wr_stroke(expanded, i+1, j+1, WR_X_DIR, search_vec_len) 
		    >= dist_limit));
	    ystroke 
	      = (  (wr_stroke(expanded, i  , j  , WR_Y_DIR, search_vec_len) 
		    >= dist_limit)
		 &&(wr_stroke(expanded, i+1, j+1, WR_Y_DIR, search_vec_len) 
		    >= dist_limit));
	    if (xstroke != ystroke)
	      outband[y][x] = outband[y+1][x] 
		= outband[y][x+1] = outband[y+1][x+1] =  0;	
	    else 
	      outband[y][x] = outband[y+1][x] 
		= outband[y][x+1] = outband[y+1][x+1] = 255;
	  }
      }	
  return 1;
} \* END wr_segment_2 *\

*/
