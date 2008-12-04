
/*

________________________________________________________________

        closestMinMax
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
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/message.h>
#include <xite/minmax.h>
#include <xite/readarg.h>

#ifndef MAIN

#ifndef MIN
# define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif

/*F:closestMinMax*

________________________________________________________________

		closestMinMax
________________________________________________________________

Name:		closestMinMax - closest of minimum and maximum, noise reduction

Syntax:         | #include <xite/minmax.h>
		|
                | BiffStatus closestMinMax( IBAND in_band,
                |    IBAND out_band, int dx, int dy );

Description:	Compute minimum and maximum over a local region of size
                'dx' * 'dy'. New center pixel value is the one of the two
		that is	closest to the old center pixel value.

Restrictions:	'in_band' and 'out_band' must have pixel type unsigned byte.

Return value:	| 0 => ok
                | 1 => bad input pixel type
		| 2 => bad output pixel type
		| 3 => bad dx argument (less than 3)
		| 3 => bad dy argument (less than 3)

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Examples:	closestMinMax(b1, b2, 3, 5);

Id:             $Id$
________________________________________________________________

*/


BiffStatus closestMinMax(IBAND in_band, IBAND out_band, int dx, int dy)
{
   int x, y, xsize, ysize, diff1, diff2;
   IBAND min;

   if (Ipixtyp(in_band) NE Iu_byte_typ)
     return(Error(1,
		"closestMinMax: Input pixel type must be unsigned byte.\n"));
   if (Ipixtyp(out_band) NE Iu_byte_typ)
     return(Error(2,
		"closestMinMax: Output pixel type must be unsigned byte.\n"));
   if (dx LT 3) return(Error(3, "closestMinMax: bad dx argument\n"));
   if (dy LT 3) return(Error(4, "closestMinMax: bad dy argument\n"));

   xsize = MIN(Ixsize(in_band), Ixsize(out_band));
   ysize = MIN(Iysize(in_band), Iysize(out_band));
   min = Imake_band(Iu_byte_typ, xsize, ysize);

   minarea(in_band, min, dx, dy);
   maxarea(in_band, out_band, dx, dy);

   FOR (y=1; y LE ysize; INC y)
     FOR (x=1; x LE xsize; INC x)
       diff1 = in_band[y][x] - min[y][x];
       diff2 = out_band[y][x] - in_band[y][x];
       if (diff1 LT diff2) out_band[y][x] = min[y][x];
     ENDFOR;
   ENDFOR;
   Idel_band(&min);
   return(0);
}  /* minarea */

#endif /* not MAIN */



/*P:closestMinMax*

________________________________________________________________

		closestMinMax
________________________________________________________________

Name:		closestMinMax - closest of minimum and maximum, noise reduction

Syntax:		| closestMinMax <inimage> <outimage> [<dx> [<dy>]]

Description:	'closestMinMax' computes minimum and maximum over a local
                region of size 'dx' * 'dy'. New center pixel value
		is the one of the two that is closest to the old
		center pixel value. If 'dx' is given, but not 'dy',
		'dy' is set to 'dx'. If neither are given, both are
		set to 3.

See also:	

Restrictions:	'inimage' must have bands with pixel type unsigned byte.

Author:		Tor Lønnestad, BLAB, Ifi, UiO.

Examples:	| closestMinMax mona.img monamin.img 
                | closestMinMax mona.img monamin.img 7
                | closestMinMax mona.img monamin.img 3 5

Id:             $Id$
________________________________________________________________

*/

#ifdef MAIN

int main(int argc, char **argv)
{
   IMAGE img1,img2;
   int bn, dx, dy, stat;
   char *args;

   Iset_message(TRUE);
   Iset_abort(TRUE);
   InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <inimage> <outimage> [<dx> [<dy>]]\n"));

   if (argc == 1) Usage(1, NULL);
   args = argvOptions(argc, argv);
   if ((argc LT 3) OR (argc GT 5)) Usage(2, "Illegal number of arguments.\n");

   if (argc GE 4) dx = atoi(argv[3]); else dx = 3;
   if (argc GE 5) dy = atoi(argv[4]); else dy = dx;

   if ((img1 = Iread_image(argv[1]))) {
     img2 = Icopy_init(img1);
     for (bn=1; bn LE Inbands(img1); INC bn)
       if ((stat = closestMinMax(img1[bn], img2[bn], dx, dy)))
         Warning(stat, "Error in band %d\n", bn);

     Ihistory(img2, argv[0], args);
     Iwrite_image(img2, argv[2]);
   } else {
     Error(5, "Can't read input file %s\n", argv[1]);
   }

   return(0);

}  /* main */

#endif /* MAIN */
