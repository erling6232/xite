

/*

________________________________________________________________

        maxHisto
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


#include <stdlib.h>
#include <xite/includes.h>
#include <xite/biff.h>
#include <xite/median.h>
#include <xite/histo.h>
#include <xite/message.h>
#include <xite/readarg.h>

#ifndef MAIN

#ifndef MAX
# define MAX(a,b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef MIN
# define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif


static void hist(IBAND input, int hxstart, int hystart, int hxstop, int hystop, int *h)
{
   register int x,y;
   for (x=0; x <= 255; ++ x) h[x] = 0;
   for (y=hystart; y <= hystop; ++ y)
     for (x=hxstart; x <= hxstop; ++ x)
       ++ h[input[y][x]];
}
 
/*F:maxHisto*

________________________________________________________________

		maxHisto
________________________________________________________________

Name:		maxHisto - texture measure

Syntax:         | #include <xite/median.h>
		|
                | int maxHisto( IBAND input, IBAND output,
                |    int nx, int ny );

Description:    Filters the 'input' band into the 'output' band
                using a local filter with window size 'nx' x 'ny'.
                The number of occurrences of the pixel value
		occurring most frequently in the window becomes
		the new center value. Even values of 'nx' or 'ny'
		are increased by one. Uses histogram updating
		when moving horizontally from pixel to pixel.
		If histogram value is larger than 255, 255 is used.

Return value:   | 0 => ok
                | 1 => bad pixel type input band
                | 2 => bad pixel type output band
                | 3 => bad nx value (less than 1 or greater than xsize)
                | 4 => bad ny value (less than 1 or greater than ysize)

Reference:	Lowitz, Gabriel E.: "Can a Local Histogram
                Really Map Texture Information?" Pattern Rec.,
		Vol. 16, pp141-147, 1983.

Author:		Tor Lønnestad, BLAB, Ifi, UiO
Id:             $Id$
________________________________________________________________

*/

int maxHisto(IBAND input, IBAND output, int nx, int ny)
{
  int i, j, hxstart, hxstop, hystart, hystop, hy, pixval, Number;
  int x, y, xsize, ysize, hysize, nxhalf, nyhalf, Pixval;
  histogram h,       /* histogram of local window area */
	    pixvals, /* list of frequent occurring pixel values */
	    many;    /* boolean, many[i]=TRUE if i is in pixvals */

  xsize  = MIN(Ixsize(input),Ixsize(output));
  ysize  = MIN(Iysize(input),Iysize(output));
  nxhalf = nx/2;
  nyhalf = ny/2;
  nx     = nxhalf*2+1;
  ny     = nyhalf*2+1;

  if (Ipixtyp(input) != Iu_byte_typ)
    return(Error(1, "Input pixel type must be unsigned byte.\n"));
  if (Ipixtyp(output) != Iu_byte_typ)
    return(Error(2, "Output pixel type must be unsigned byte.\n"));
  if ((nx < 1) || (nx > xsize))
    return(Error(3, "Horizontal window size illegal.\n"));
  if ((ny < 1) || (ny > ysize))
    return(Error(4, "Vertical window size illegal.\n"));
  
  for (y=1; y <= ysize; ++ y) {
    /* calc available area */
    hystart = MAX(1,y-nyhalf);
    hystop  = MIN(ysize,y+nyhalf);
    hysize  = hystop-hystart+1;
    
    /* calc. initial histogram */
    hxstart  = 1;
    hxstop   = nxhalf+1;
    hist(input,hxstart,hystart,hxstop,hystop,h);
    
    /* find all "frequent" pixel values */
    j=0;
    for (i=0; i <= 255; ++ i) many[i]=FALSE;
    for (i=0; i <= 255; ++ i)
      if (h[i] >= 2) {pixvals[j++]=i; many[i]=TRUE;}

    /* find the most frequent one */
    if (j > 0) {
      Pixval = pixvals[0]; Number = h[Pixval];
      for (i=1; i < j; ++ i)
	if (h[pixvals[i]] > Number)
	  {Pixval=pixvals[i]; Number=h[Pixval];}
      output[y][1] = MIN(255, Number);
    } else output[y][1] = 1;

    /* update along the line until window has grown to nx*ny */
    for (x=2; hxstop < nx; ++ x) {
      ++ hxstop;
      /* update histgr */
      for (hy=hystart; hy <= hystop; ++ hy) {
	pixval = input[hy][hxstop];
	if ((++ h[pixval]) == 2)
	  {pixvals[j++]=pixval; many[pixval]=TRUE;}
      }
      /* calc most freq. */
      if (j > 0) {
	Pixval = pixvals[0]; Number = h[Pixval];
	for (i=1; i < j; ++ i)
	  if (h[pixvals[i]] > Number)
	    {Pixval=pixvals[i]; Number=h[Pixval];}
	output[y][x] = MIN(255, Number);
      } else output[y][x] = 1;
    }

    /* update along the line while window is nx*ny */
    for ( ; hxstop < xsize; ++ x) {
      /* update histgr */
      ++ hxstop;
      for (hy=hystart; hy <= hystop; ++ hy) {
	-- h[input[hy][hxstart]];
	pixval = input[hy][hxstop];
	if (((++ h[pixval]) == 2) && (! many[pixval]))
	  {pixvals[j++]=pixval; many[pixval]=TRUE;}
      }
      ++ hxstart;
      /* calc the most frequent */
      Pixval = input[y][x]; Number = 1;
      for (i=0; i < j; )
	if (h[pixvals[i]] > Number) /* new record */
	  {Pixval=pixvals[i++]; Number=h[Pixval];}
	else if (h[pixvals[i]] < 2) /* hasn't got many */
	  {many[pixvals[i]]=FALSE; pixvals[i]=pixvals[-- j];}
	else ++ i;
      output[y][x] = MIN(255, Number);
    }

    /* update along the line until end of line */
    for ( ; x <= xsize; ++ x) {
      /* update histgr */
      for (hy=hystart; hy <= hystop; ++ hy)
	-- h[input[hy][hxstart]];
      ++ hxstart;
      /* calc most frequent */
      Pixval = input[y][x]; Number = 1;
      for (i=0; i < j; )
	if (h[pixvals[i]] > Number) /* new record */
	  {Pixval=pixvals[i++]; Number=h[Pixval];}
	else if (h[pixvals[i]] < 2) /* hasn't got many */
	  {many[pixvals[i]]=FALSE; pixvals[i]=pixvals[-- j];}
	else ++ i;
      output[y][x] = MIN(255, Number);
    }
  } /* all y... */

  return(0);

}  /* maxHisto() */

#endif /* not MAIN */



/*P:maxHisto*

________________________________________________________________

		maxHisto
________________________________________________________________

Name:		maxHisto - texture measure

Syntax:		| maxHisto <inimage> <outimage> [<nx> [<ny>]]

Description:    Filters the input image into the output image
                using a local filter with window size 'nx' * 'ny'.
                The number of occurrences of the pixel value
		occurring most frequently in the window becomes
		the new center value.

		Even values of 'nx' or 'ny' are increased by one. Uses
		histogram updating when moving horizontally from pixel to
		pixel. If histogram value is larger than 255, 255 is used.

		Default value for 'nx' is 3. Default value for 'ny' is 'nx'.

Restrictions:   'inimage' must have bands with pixel type unsigned byte.

References:	Lowitz, Gabriel E.: "Can a Local Histogram
                Really Map Texture Information?" Pattern Rec.,
		Vol. 16, pp141-147, 1983.

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Examples:       | maxHisto mona.img monaMH.img
                | maxHisto mona.img monaMH.img 5
		| maxHisto mona.img monaMH.img 5 7
________________________________________________________________

*/

#ifdef MAIN

int main(int argc, char **argv)
{
   IMAGE i1, i2;
   int bn, nx, ny;
   char *args;

   Iset_message(TRUE); 
   Iset_abort(TRUE);
   InitMessage(&argc, argv, xite_app_std_usage_text(
	       "Usage: %s <oldfile> <newfile> [<nx> [<ny>]]\n"));

   if (argc == 1) Usage(1, NULL);
   args = argvOptions(argc, argv);

   if (argc < 3) Usage(1, "Illegal number of arguments.\n");
   if (argc >= 4) nx = atoi(argv[3]); else nx = 3;
   if (argc >= 5) ny = atoi(argv[4]); else ny = nx;
   if ((nx < 1) || (ny < 1))
     Usage(2, "Window sizes should be greater than 0.\n");
 
   i1 = Iread_image(argv[1]);
   i2 = Icopy_init(i1);

   for (bn=1; bn <= Inbands(i1); ++ bn)
     if (maxHisto(i1[bn], i2[bn], nx, ny))
       Message(2, "%s: Error in band %d\n", argv[0], bn);

   Ihistory(i2, argv[0], args);
   Iwrite_image(i2,argv[2]);

   return(0);
}

#endif /* MAIN */
