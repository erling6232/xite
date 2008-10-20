
/*

________________________________________________________________

        scatter
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
#include <stdlib.h>
#include <xite/scatter.h>
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#include <xite/blab.h>
#include <xite/message.h>
#include <xite/readarg.h>

#ifndef MAIN

#ifndef MIN
# define MIN(a,b) (((a)<(b)) ? (a) : (b))
#endif



/*F:scatter*

________________________________________________________________

		scatter
________________________________________________________________

Name:		scatter - make a scatterplot

Syntax:         | #include <xite/scatter.h>
		|
                | int scatter( IBAND input1, IBAND input2,
                |    IBAND output, int *overflows );
		|
Description:	Creates a scatterplot (a two dimensional
                histogram) of the two input bands. Every
		scatter update that would increase the
		scatter value beyond 255 increases the
		parameter 'overflows' instead.

Restrictions:   All bands must have pixel type unsigned byte.

Return value:	| 0 => ok
                | 1 => Bad pixel type in input1
                | 2 => Bad pixel type in input2
                | 3 => Bad pixel type in output
		| 4 => Output has xsize less than 256
		| 5 => Output has ysize less than 256

Author:		Tor Lønnestad, BLAB, ifi, UiO

Id:             $Id$
________________________________________________________________

*/



int scatter(IBAND input1, IBAND input2, IBAND output, int *overflows)
{
   int x, y, val1, val2, xsize, ysize;

   *overflows = 0;
   if (Ipixtyp(input1) NE Iu_byte_typ)
     return(Error(1, "%s\n",
		  "scatter(): input1 must have pixel type unsigned byte."));
   if (Ipixtyp(input2) NE Iu_byte_typ)
     return(Error(2, "%s\n",
		  "scatter(): input2 must have pixel type unsigned byte."));
   if (Ipixtyp(output) NE Iu_byte_typ)
     return(Error(3, "%s\n",
		  "scatter(): output must have pixel type unsigned byte."));
   if (Ixsize(output) LT 256)
     return(Error(4, "scatter(): Output has xsize less than 256.\n"));
   if (Iysize(output) LT 256)
     return(Error(5, "scatter(): Output has ysize less than 256.\n"));

   xsize = MIN(Ixsize(input1),Ixsize(input2));
   ysize = MIN(Iysize(input1),Iysize(input2));

   for (y=1; y LE 256; y++)
     for (x=1; x LE 256; x++)
       output[y][x] = 0;

   FOR (y=1; y LE ysize; y++)
     FOR (x=1; x LE xsize; x++)
       val1 = input1[y][x]+1;
       val2 = input2[y][x]+1;
       if (output[val1][val2] LT 255)
         ++ output[val1][val2];
       else ++ (*overflows);
     ENDFOR;
   ENDFOR;
   return 0;

}  /*  scatter() */

#endif /* not MAIN */



/*P:scatter*

________________________________________________________________

		scatter
________________________________________________________________

Name:		scatter - make a scatterplot

Syntax:		scatter [<option>...] <inimage1> <inimage2> <outimage>

Description:	Creates a scatterplot (a two dimensional
                histogram) of band 1 of the two input images.
		The scatterplot is returned as an unsigned byte
		image.

Options:        &-t title, -title title
                'title' of 'outimage'.

Restrictions:	Accepts only UNS_BYTE pixels. Every scatter update that
                would increase the scatter value beyond 255 increases an
		overflow counter, which is eventually written to stderr.

Author:		Tor Lønnestad, BLAB, Ifi, UiO

Examples:	| scatter image1.img image2.img scatter.img
		| scatter image.img:1 image.img:2 scatter.img

Id:             $Id$
________________________________________________________________

*/

#ifdef MAIN

int main(int argc, char **argv)
{
   IMAGE i1, i2, i3;
   char* title, *args;
   int stat, overflows = 0;

   InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <oldfile1> <oldfile2> <newfile>\n"));
   Iset_message(TRUE);
   Iset_abort(TRUE);

   if (argc == 1) Usage(1, NULL);

   args = argvOptions(argc, argv);
   title = read_switch(&argc, argv, "-t", 1, "SCATTERPLOT");
   title = read_switch(&argc, argv, "-title", 1, title);

   if (argc != 4) Usage(2, "Illegal number of arguments.\n");

   i1 = Iread_image(argv[1]);
   i2 = Iread_image(argv[2]);
   i3 = Imake_image(1, title, Iu_byte_typ, 256, 256);
   Iset_start(i3[1], 0, 0);

   if ((stat = scatter(i1[1], i2[1], i3[1], &overflows))) exit(stat);
   if (overflows)
     Warning(6, "Scatter overflows: %d.\n", overflows);

   Ihistory(i3, argv[0], args);
   Iwrite_image(i3, argv[3]);

   return(0);

} /* main */

#endif /* MAIN */
