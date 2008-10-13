

#ifdef PA_SMOOTH
/* Weight matrix for 3x3 weighted mean smoothing, used by reg_interior_smooth
   for Parker's method.

         0.7 1.0 0.7
         1.0 1.5 1.0
         0.7 1.0 0.7
*/


static double reg_interior_smooth(double center, 
			 double edge1, double edge2, double edge3, double edge4,
			 double corner1, double corner2, 
			 double corner3, double corner4)
{
  return (1.5 * center + (edge1 + edge2+ edge3 + edge4)
	  + 0.7 * (corner1 + corner2 + corner3 + corner4)) / 8.3;
}


    
/* Reduced weight matrix for 3x3 weighted mean smoothing, 
   used by reg_corner_smooth:

             1.5 1.0
             1.0 0.7
*/

static double reg_corner_smooth(double center, double edge1, 
			       double edge2, double corner)
{
  return (1.5 * center + (edge1 + edge2) + 0.7 * corner) / 4.2;
}



/* Reduced weight matrix for 3x3 weighted mean smoothing, 
   used by reg_edge_smooth:

             1.0 0.7
             1.5 1.0
             1.0 0.7
*/

static double reg_edge_smooth(double center, 
		     double edge1, double edge2, double edge3, 
		     double corner1, double corner2)
{
  return (1.5 * center + (edge1 + edge2+ edge3)
	  + 0.7 * (corner1 + corner2)         ) / 5.9;
}

#endif




#ifdef NR_SMOOTH

/* Weight matrix for 3x3 weighted mean smoothing, used by reg_interior_smooth
   for Nakagawa and Rosenfelds's method.

         0.7 1.0 0.7
         1.0 2.0 1.0
         0.7 1.0 0.7

   0.7 is replaced by 1/sqrt(2). 
*/


static double reg_interior_smooth(double center, 
			 double edge1, double edge2, double edge3, double edge4,
			 double corner1, double corner2, 
			 double corner3, double corner4)
{
  return (2.0 * center + (edge1 + edge2+ edge3 + edge4)
	  + 0.7 * (corner1 + corner2 + corner3 + corner4)) / 8.8;
}


    
/* Reduced weight matrix for 3x3 weighted mean smoothing, 
   used by reg_corner_smooth:

             2.0 1.0
             1.0 0.7
*/

static double reg_corner_smooth(double center, double edge1, double edge2, double corner)
{
  return (2.0 * center + (edge1 + edge2) + 0.7 * corner) / 4.7;
}



/* Reduced weight matrix for 3x3 weighted mean smoothing, 
   used by reg_edge_smooth:

             1.0 0.7
             2.0 1.0
             1.0 0.7
*/

static double reg_edge_smooth(double center, 
		     double edge1, double edge2, double edge3, 
		     double corner1, double corner2)
{
  return (2.0 * center + (edge1 + edge2+ edge3)
	  + 0.7 * (corner1 + corner2)         ) / 6.4;
}
#endif



static int reg_smooth(IR_BAND inband, IR_BAND outband)
{
  int x, y, xsize, ysize;
  xsize = Ixsize((IBAND)inband);
  ysize = Iysize((IBAND)inband);

  
  /* Maa lage double-versjon av expand-edge! */

  outband[1][1] = reg_corner_smooth(inband[1][1], 
				   inband[1][2], inband[2][1], 
				   inband[2][2]);
  for (x=2; x<=xsize-1; x++)
    outband[1][x] = reg_edge_smooth(inband[1][x], 
				   inband[1][x-1], inband[1][x+1], 
				   inband[2][x],
				   inband[2][x-1], inband[2][x+1]);
  outband[1][xsize] = reg_corner_smooth(inband[1][xsize], 
				       inband[1][xsize-1], inband[2][xsize], 
				       inband[2][xsize-1]);
  for (y=2; y<=ysize-1; y++)
    {
      outband[y][1] 
	= reg_edge_smooth(inband[y][1], 
			 inband[y-1][1], inband[y+1][1], inband[y][2],
			 inband[y-1][2], inband[y+1][2]);

      for (x=2; x<=xsize-1; x++)
	outband[y][x] 
	  = reg_interior_smooth(inband[y][x],
			       inband[y-1][x], inband[y][x-1],
			       inband[y][x+1], inband[y+1][x],
			       inband[y-1][x-1], inband[y-1][x+1],
			       inband[y+1][x-1], inband[y+1][x+1]);

      outband[y][xsize] 
	= reg_edge_smooth(inband[y][xsize], 
			 inband[y-1][xsize], inband[y+1][xsize], 
			 inband[y][xsize-1],
			 inband[y-1][xsize-1],inband[y+1][xsize-1]);
    }
  outband[ysize][1] 
    = reg_corner_smooth(inband[ysize][1], 
		       inband[ysize][2], inband[ysize-1][1], 
		       inband[ysize-1][2]);
  for (x=2; x<=xsize-1; x++)
    outband[ysize][x] 
      = reg_edge_smooth(inband[ysize][x], 
		       inband[ysize][x-1], inband[ysize][x+1], 
		       inband[ysize-1][x],
		       inband[ysize-1][x-1], inband[ysize-1][x+1]);
  outband[ysize][xsize] 
    = reg_corner_smooth(inband[ysize][xsize], 
		       inband[ysize][xsize-1], inband[ysize-1][xsize], 
		       inband[ysize-1][xsize-1]);

  return 1;
}

static int reg_interpolate(IR_BAND inband, IR_BAND outband)
{
  int x, y, rx, ry;
  int xstart, ystart;
  int xend, yend;
  int xsize, ysize;
  int nof_reg_x, nof_reg_y;


  xsize = Ixsize((IBAND)outband);
  ysize = Iysize((IBAND)outband);
  nof_reg_x = Ixsize((IBAND)inband);
  nof_reg_y = Iysize((IBAND)inband);

  if (Verbose()) {
    printf("reg_interpolate\n");
    printf("   size of inband: (%i,%i), size of outband : (%i,%i)\n",
	   nof_reg_x, nof_reg_y, xsize, ysize);
  }

  xend = (xsize) / nof_reg_x / 2 + 1;

  if (Verbose()) printf("   Interior pixels\n");
  for (rx=1; rx<nof_reg_x; rx++)
    {
      xstart = xend;
      xend = (xsize * (2*rx+1)) / nof_reg_x / 2 + 1;
      yend = (ysize) / nof_reg_y / 2 + 1;
      for (ry=1; ry<nof_reg_y; ry++)
	{
	  ystart = yend;
	  yend = (ysize * (2*ry+1)) / nof_reg_y / 2 + 1;
	  for (x=xstart; x<=xend; x++)
	    {
/*	      outband[ystart][x] 
		= (  inband[ry][rx] 
		   + inband[ry][rx+1]  ) / 2.0;
	      outband[yend][x] 
		= (  inband[ry+1][rx] 
		   + inband[ry+1][rx+1]  ) / 2.0;
	      for (y=ystart+1; y<=yend-1; y++)
		{	
		  outband[y][x]
		    = (  (yend-y)   * outband[ystart][x]
		       + (y-ystart) * outband[yend][x]  )* 1.0 / (yend-ystart);
		}
*/
	      outband[ystart][x] 
		= (  (xend-x)   * inband[ry][rx] 
		   + (x-xstart) * inband[ry][rx+1]  ) / (xend-xstart);
	      outband[yend][x] 
		= (  (xend-x)   * inband[ry+1][rx] 
		   + (x-xstart) * inband[ry+1][rx+1]  ) / (xend-xstart);
	      for (y=ystart+1; y<=yend-1; y++)
		{	
		  outband[y][x]
		    = (  (yend-y)   * outband[ystart][x]
		       + (y-ystart) * outband[yend][x]  ) / (yend-ystart);
		}

	    }
	}
    }

  if (Verbose()) printf("   Edge pixels\n");
  xstart = (xsize                  ) / nof_reg_x / 2 + 1;
  ystart = (ysize                  ) / nof_reg_y / 2 + 1;
  xend   = (xsize * (2*nof_reg_x-1)) / nof_reg_x / 2 + 1;
  yend   = (ysize * (2*nof_reg_y-1)) / nof_reg_y / 2 + 1;
  for (x=xstart; x<=xend; x++)
    {
      for (y=1; y<ystart; y++)
	outband[y][x] = 2*outband[ystart][x] - outband[2*ystart-y][x];
      for (y=yend+1; y<=ysize; y++)
	outband[y][x] = 2*outband[yend][x] - outband[2*yend-y][x];
    }
  for (y=ystart; y<=yend; y++)
    {
      for (x=1; x<xstart; x++)
	outband[y][x] = 2*outband[y][xstart] - outband[y][2*xstart-x];
      for (x=xend+1; x<=xsize; x++)
	outband[y][x] = 2*outband[y][xend] - outband[y][2*xend-x];
    }
  outband[1][1] 
    = (  ystart * (2*outband[1][xstart] - outband[1][2*xstart-1])
       + xstart * (2*outband[ystart][1] - outband[2*ystart-1][1]))
      /(xstart+ystart); 
  outband[1][xsize] 
    = (  ystart       * (2*outband[1][xend]      -outband[1][2*xend-xsize])
       + (xsize-xend) * (2*outband[ystart][xsize]-outband[2*ystart-1][xsize]))
      /((xsize-xend)+ystart); 
  outband[ysize][1] 
    = ( (ysize-yend)* (2*outband[ysize][xstart]- outband[ysize][2*xstart-1])
       + xstart     * (2*outband[yend][1]      - outband[2*yend-ysize][1]))
      /(xstart+(ysize-yend)); 
  outband[ysize][xsize] 
    = ( (ysize-yend)* (2*outband[ysize][xend] - outband[ysize][2*xend-xsize])
       +(xsize-xend)* (2*outband[yend][xsize] - outband[2*yend-ysize][xsize]))
      /((xsize-xend)+(ysize-yend)); 

  if (Verbose()) printf("   Corner pixels\n");
  for (x=1; x<xstart; x++)
    {
      outband[1][x] 
	= (  (xstart-x) * outband[1][1]
	   + (x-1)      * outband[1][xstart]) / (xstart-1);
      outband[ysize][x] 
	= (  (xstart-x) * outband[ysize][1]
	   + (x-1)      * outband[ysize][xstart]) / (xstart-1);
      for (y=2; y<ystart; y++)
	outband[y][x] 
	  = (  (ystart-y) * outband[1][x]
	     + (y-1)      * outband[ystart][x]) / (ystart-1);
      for (y=yend+1; y<ysize; y++)
	outband[y][x] 
	  = (  (ysize-y)  * outband[yend][x]
	     + (y-yend)   * outband[ysize][x]) / (ysize-yend);
    }

  for (x=xend+1; x<=xsize; x++)
    {
      outband[1][x] 
	= (  (x-xend)    * outband[1][xsize]
	   + (xsize-x)   * outband[1][xend]) / (xsize-xend);
      outband[ysize][x] 
	= (  (x-xend)    * outband[ysize][xsize]
	   + (xsize-x)   * outband[ysize][xend]) / (xsize-xend);
      for (y=2; y<ystart; y++)
	outband[y][x] 
	  = (  (ystart-y) * outband[1][x]
	     + (y-1)      * outband[ystart][x]) / (ystart-1);
      for (y=yend+1; y<ysize; y++)
	outband[y][x] 
	  = (  (ysize-y)  * outband[yend][x]
	     + (y-yend)   * outband[ysize][x]) / (ysize-yend);
    }

  return 1;
}
