

#ifdef FUNCPROTO
static int mark_object(int x, int y, IBAND ioband, IBAND visited, 
		       int mark_value)
#else
static int mark_object(x, y, ioband, visited, mark_value)
     int x;
     int y;
     IBAND ioband;
     IBAND visited;
     int mark_value;
#endif
{
  int xsize, ysize;
  int xstart, ystart;
  UNS_BYTE object_value;
  
  object_value = ioband[y][x];
  xstart = x;
  ystart = y;
  xsize = Ixsize(ioband);
  ysize = Iysize(ioband);


  while (TRUE)
    {    
      ioband[y][x] = mark_value;

      if ((x>1) && ((visited[y][x] & BIN_LEFT_DONE) == 0) &&
	  (ioband[y][x-1] == object_value) && (visited[y][x-1] == 0))
	{
	  visited[y][x] = visited[y][x] | BIN_LEFT_DONE;
	  x = x - 1;
	  visited[y][x] = BIN_FROM_RIGHT;
	}
      else if ((x<xsize) && ((visited[y][x] & BIN_RIGHT_DONE) == 0) &&
	       (ioband[y][x+1] == object_value) && (visited[y][x+1] == 0))
	{
	  visited[y][x] = visited[y][x] | BIN_RIGHT_DONE;
	  x = x + 1;
	  visited[y][x] = BIN_FROM_LEFT;
	}
      else if ((y>1) && ((visited[y][x] & BIN_UP_DONE) == 0) &&
	       (ioband[y-1][x] == object_value) && (visited[y-1][x] == 0))
	{
	  visited[y][x] = visited[y][x] | BIN_UP_DONE;
	  y = y - 1;
	  visited[y][x] = BIN_FROM_BELOW;
	}
      else if ((y<ysize) && ((visited[y][x] & BIN_DOWN_DONE) == 0) &&
	       (ioband[y+1][x] == object_value) && (visited[y+1][x] == 0))
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
		    printf("[mark_object] (xstart,ystart) = (%i,%i)", 
			   xstart, ystart);
		    printf("[mark_object] (x,y) = (%i,%i)", 
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
	      if (Verbose()) {
		printf("[mark_object] Illegal value ");
		printf("of visited[%i][%i] = %3i + %2i\n",
		       x, y, visited[y][x] & BIN_DIR_MASK, 
		       visited[y][x] - (visited[y][x] & BIN_DIR_MASK));
	      }
	    } /* switch */
	} /* else */
    } /* while */

} /* END mark_object */

