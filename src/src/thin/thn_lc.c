

/*C*

________________________________________________________________

        thn_lc.c
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
#include <xite/thin.h>

#ifndef MAIN

#define FF 255

static UNS_BYTE lc_look_up[16][16]= 
/* 0 1 2 3 4 5 6 7 8 9 A B C D E F */
{{FF, FF, FF, 01, FF, 00, 01, 01, FF, FF, FF, FF, 01, 00, 01, 01}, /* 0 */ 
 {FF, FF, FF, FF, 00, FF, 00, 00, 01, FF, FF, FF, 01, 00, 01, 01}, /* 1 */ 
 {FF, FF, FF, FF, FF, FF, FF, FF, FF, FF, FF, FF, FF, FF, FF, FF}, /* 2 */ 
 {01, FF, FF, FF, 00, 00, 00, 00, 01, FF, FF, FF, 01, 00, 01, 01}, /* 3 */ 
 {FF, 00, FF, 00, FF, FF, FF, 00, FF, FF, FF, FF, FF, 00, FF, 00}, /* 4 */ 
 {00, FF, FF, 00, FF, FF, 00, FF, 00, 00, FF, 00, 00, FF, 00, FF}, /* 5 */ 
 {01, 00, FF, 00, FF, 00, FF, 00, FF, FF, FF, FF, FF, 00, FF, 00}, /* 6 */ 
 {01, 00, FF, 00, 00, FF, 00, FF, 01, 00, FF, 00, 01, FF, 01, FF}, /* 7 */ 
 {FF, 01, FF, 01, FF, 00, FF, 01, FF, FF, FF, FF, FF, 00, FF, 01}, /* 8 */ 
 {FF, FF, FF, FF, FF, 00, FF, 00, FF, FF, FF, FF, FF, 00, FF, 01}, /* 9 */ 
 {FF, FF, FF, FF, FF, FF, FF, FF, FF, FF, FF, FF, FF, FF, FF, FF}, /* A */ 
 {FF, FF, FF, FF, FF, 00, FF, 00, FF, FF, FF, FF, FF, 00, FF, 00}, /* B */ 
 {01, 01, FF, 01, FF, 00, FF, 01, FF, FF, FF, FF, FF, 00, FF, 01}, /* C */
 {00, 00, FF, 00, 00, FF, 00, FF, 00, 00, FF, 00, 00, FF, 00, FF}, /* D */ 
 {01, 01, FF, 01, FF, 00, FF, 01, FF, FF, FF, FF, FF, 00, FF, 00}, /* E */ 
 {01, 01, FF, 01, 00, FF, 00, FF, 01, 01, FF, 00, 01, FF, 00, FF}};/* F */




/*F:thn_lc*

________________________________________________________________

		thn_lc
________________________________________________________________

Name:		thn_lc - Lee and Chen''s thinning method.

Syntax:         | #include <xite/thin.h>
                |
                | int thn_lc( int verbose, IBAND inband,
                |    IBAND outband );

Description:    Thin a binary image with Lee and Chen''s method. Black (0) is
                foreground, and white (255) is background.

		This method is simply an extension of Zhang and Suen''s
		method. First, Zhang and Suen''s method is used. The skeleton
		thus obtained is not truly 8-connected, since some
		non-junction pixels have more than two neighbors, making the
		skeleton useless for algorithms that require this constraint.
		Therefore, some pixels have to be removed. The skeleton is
		inspected, and each pixel is tested using a lookup table. The
		result is a true 8-connected skeleton where only junction
		pixels have more than two 8-neighbors.

		If 'verbose' > 0, some messages are printed during execution. 

Restrictions:   'inband' must have pixel type unsigned byte. 'inband' is
                assumed to be binary valued, with foreground = 0,
		background = 255.

Author:		Øivind Due Trier, late one night at Michigan State University.

See also:       thn_lc(1), thn_har(3), thn_zs(3)

Reference:      &H.-J. Lee and B. Chen,
                "Recognition of handwritten chinese characters via short
		line segments,"
		Pattern Recognition,
		vol. 25, no. 5, pp. 543-552, 1992.

________________________________________________________________

*/

int thn_lc(int verbose, IBAND inband, IBAND outband)
{
  int x, y, xsize, ysize;
  int P2, P3, P4, P5;
  int P6, P7, P8, P9;
  int i, j, status;
  
  xsize = Ixsize(inband);
  ysize = Iysize(inband);
  
  if ((status = thn_zs(verbose, inband, outband)) != 0)
    return(Error(status, "Error in thn_zs.\n"));
  
  for (y=2; y<ysize; y++) {
    for (x=2; x<xsize; x++) {
      if (outband[y][x] == THN_FOREGROUND) {
	P2 = (outband[y-1][x  ] == THN_FOREGROUND) << 0;
	P3 = (outband[y-1][x+1] == THN_FOREGROUND) << 1;
	P4 = (outband[y  ][x+1] == THN_FOREGROUND) << 2;
	P5 = (outband[y+1][x+1] == THN_FOREGROUND) << 3;
	P6 = (outband[y+1][x  ] == THN_FOREGROUND) << 0;
	P7 = (outband[y+1][x-1] == THN_FOREGROUND) << 1;
	P8 = (outband[y  ][x-1] == THN_FOREGROUND) << 2;
	P9 = (outband[y-1][x-1] == THN_FOREGROUND) << 3;
	
	i = P9+P8+P7+P6;
	j = P5+P4+P3+P2;
	if (lc_look_up[i][j] == 00)
	  outband[y][x] = THN_BACKGROUND;
	else if (lc_look_up[i][j] == 01) {
	  Warning(1, "%s%i%s%i%s\n",
		  "thn_lc: Pixel (", x, ",", y,
		  ") should have been removed by thn_zs()");
	  outband[y][x] = THN_BACKGROUND;
	}
      } /* END if  */
    } /* END for (x=1; ... */
  } /* END for (y=1; ... */
  
  return THN_OK;
}

#endif /* not MAIN */

/*P:thn_lc*

________________________________________________________________

		thn_lc
________________________________________________________________

Name:		thn_lc - Lee and Chen''s thinning method.

Syntax:		thn_lc <inimage> <outimage>

Description:	Thin a binary image with Lee and Chen''s method. Black (0) is
                foreground, and white (255) is background.

		This method is simply an extension of Zhang and Suen''s
		method. First, Zhang and Suen''s methhod is used. The skeleton
		thus obtained is not truly 8-connected, since some
		non-junction pixels have more than two neighbors, making the
		skeleton useless for algorithms that require this constraint.
		Therefore, some pixels have to be removed. The skeleton is
		inspected, and each pixel is tested using a lookup table. The
		result is a true 8-connected skeleton where only junction
		pixels have more than two 8-neighbors.

Restrictions:   'inimage' must have bands with pixel type unsigned byte.
                The input is assumed to be binary valued with foreground = 0,
		background = 255.

See also:	thn_lc(3), thn_har(1), thn_zs(1)

Reference:      &H.-J. Lee and B. Chen,
                "Recognition of handwritten chinese characters via short
		line segments,"
		Pattern Recognition,
		vol. 25, no. 5, pp. 543-552, 1992.

Files:		xite/src/thin/thn_lc.c
Author:		Øivind Due Trier, late one night at Michigan State University.
Id: 		$Id$
________________________________________________________________

*/

#ifdef MAIN

int main (int argc, char *argv[])
{
   int xsize, ysize;
   IBAND inband;
   IBAND outband;
   char *infilename, *outfilename, *args;
   IMAGE inimage;
   IMAGE outimage;
   
   Iset_message(TRUE);
   InitMessage(&argc, argv, xite_app_std_usage_text(
     "Usage: %s <inimage> <outimage>\n"));

   if (argc == 1) Usage(1, NULL);
   args = argvOptions(argc, argv);
   if (argc < 3) Usage(2, "Illegal number of arguments.\n");

   infilename  = argv[argc-2];
   outfilename = argv[argc-1];
   inimage     = Iread_image(infilename);
   inband      = (IBAND)inimage[1]; /* First band only is used. */
   xsize       = Ixsize(inband);
   ysize       = Iysize(inband);
   outimage    = Imake_image(1, outfilename, Ipixtyp(inband), xsize, ysize);
   outband     = (IBAND) outimage[1];
   
   if (thn_lc(Verbose(), inband, outband))
     return(Error(3, "Error in thn_lc.\n"));
   
   Ihistory(outimage, argv[0], args);
   Iwrite_image(outimage, outfilename);
   
   return 0; /* Unix commands should return 0 */

} /* END main() */

#endif /* MAIN */
