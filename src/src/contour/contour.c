
/*

________________________________________________________________

        contour
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
#include <xite/contour.h>
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/message.h>
#include <xite/readarg.h>

#ifndef MIN
# define MIN(a,b) ((a)<(b) ? a : b)
#endif

#ifndef MAIN

/*F:contour*

________________________________________________________________

		contour
________________________________________________________________

Name:		contour - create a contour map

Syntax:         | #include <xite/contour.h>
                |
                | int contour( IBAND input, IBAND output,
                |    int eqd, int cpv, int bpv );

Description:    Creates a contour map of the input image, with
                equidistance 'eqd'. Contour pixels receive the value 'cpv',
		contour pixel value, and background pixels receive the value
		'bpv', background pixel value. Contours should
		really be drawn 'between' the pixels, but are
		shifted half a pixel to the left or half a pixel
		upward to hit a pixel.

Restrictions:   'input' and 'output' must have pixel type unsigned byte.
                'eqd' must be in [2, 255].

Return value:   | 0 => ok
                | 2 => bad pixel type input
		| 3 => bad pixel type output
		| 4 => bad eqd value (less than 2 or greater than 255)

See also:       contour(1), crossing(3)

Author:		Tor Lønnestad, BLAB, Ifi, UiO.

Id:             $Id$
________________________________________________________________

*/

#ifndef FUNCPROTO
int contour(input, output, eqd, cpv, bpv)
IBAND input, output;
int eqd, cpv, bpv;
#else /* FUNCPROTO */
int contour(IBAND input, IBAND output, int eqd, int cpv, int bpv)
#endif /* FUNCPROTO */
{
   int x, y, xsize, ysize, i, mapval;
   UNS_BYTE map[256];

   if (Ipixtyp(input) NE Iu_byte_typ)
     return(Error(2, "contour: Input pixel type must be unsigned byte.\n"));
   if (Ipixtyp(output) NE Iu_byte_typ)
     return(Error(3, "contour: Output pixel type must be unsigned byte.\n"));
   if ((eqd LT 2) OR (eqd GT 255))
     return(Error(4, "contour: bad eqd argument value.\n"));

   if (cpv LT 0)   cpv = 0;
   if (cpv GT 255) cpv = 255;
   if (bpv LT 0)   bpv = 0;
   if (bpv GT 255) bpv = 255;

   for (i=0; i LE 255; ++ i) map[i] = i/eqd;

   xsize = MIN(Ixsize(input), Ixsize(output));
   ysize = MIN(Iysize(input), Iysize(output));

   FOR (y=1; y LT ysize; ++ y)
     FOR (x=1; x LT xsize; ++ x)
       mapval = map[input[y][x]];
       if ((mapval NE map[input[y][x+1]])
         OR (mapval NE map[input[y+1][x]]))
         output[y][x] = cpv; /* contour */
       else output[y][x] = bpv; /* background */
     ENDFOR;  /* all x */
     output[y][xsize] =
       (map[input[y][x]] NE map[input[y+1][x]]) ? cpv : bpv;
   ENDFOR;  /* all y */

   for (x=1; x LT xsize; ++ x)
     output[ysize][x] = 
       (map[input[ysize][x]] NE map[input[ysize][x+1]]) ? cpv : bpv;

   output[ysize][xsize] = bpv;

   return(0);

}  /* contour */

#endif /* MAIN */



/*P:contour*

________________________________________________________________

		contour
________________________________________________________________

Name:		contour - make a contour map

Syntax:		| contour [-cpv <cpv>] [-bpv <bpv>] <inimage> <outimage>
                |         [<equidistance>]

Description:    Creates a contour map of the input image.
                Default 'equidistance' is 20.

		If the result is noisy, smooth the input image before
		contouring, or use larger equidistance.

Restrictions:   'inimage' must have bands with pixel type unsigned byte.

Options:        &-cpv cpv
                Contour	pixel value, default 0.

		&-bpv bpv
		Background pixel value, default 255.

See also:       contour(3), crossing(1)

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Examples:       contour mona.img monaContour.img 40

Id:             $Id$
________________________________________________________________

*/

#ifdef MAIN

#ifndef FUNCPROTO
int main(argc,argv)
int argc;
char *argv[];
#else /* FUNCPROTO */
int main(int argc, char **argv)
#endif /* FUNCPROTO */
{
   IMAGE i1, i2;
   int bn, eqd, cpv, bpv;
   char *args;

   Iset_message(TRUE);
   Iset_abort(TRUE);
   InitMessage(&argc, argv, xite_app_std_usage_text(
       "Usage: %s  [-cpv <cpv>] [-bpv <bpv>] <input image> \n\
             <output image> [<equidistance>]\n"));

   if (argc == 1) Usage(1, NULL);
   args = argvOptions(argc, argv);

   cpv = read_iswitch(&argc, argv, "-cpv", 0);
   bpv = read_iswitch(&argc, argv, "-bpv", 255);

   if ((argc LT 3) OR (argc GT 4))
     exit(Usage(2, "Illegal number of arguments.\n"));

   if (argc EQ 3) eqd = 20; else eqd = atoi(argv[3]);

   i1 = Iread_image(argv[1]);
   i2 = Icopy_init(i1);

   for (bn=1; bn LE Inbands(i1); ++ bn)
     if (contour(i1[bn], i2[bn], eqd, cpv, bpv))
       Warning(3, "Error in countour() for band %d.\n", bn);

   Ihistory(i2, argv[0], args);
   Iwrite_image(i2, argv[2]);

   return(0);
}

#endif /* MAIN */
