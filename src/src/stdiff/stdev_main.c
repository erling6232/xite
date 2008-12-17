
/*

________________________________________________________________

        stdev.c
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
#include <xite/stdiff.h>
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/histo.h>
#include <xite/message.h>
#include <xite/readarg.h>

/*P:stdev*

________________________________________________________________

		stdev
________________________________________________________________

Name:		stdev - calculation of local standard deviation

Syntax:		| stdev <inimage> <outimage> [<dx> [<dy>]]

Description:    'stdev' calculates and returns the standard
                deviation in a local window of size 'dx' * 'dy'.
		The border is also processed, calculating the
		standard deviation over the part of the window
		that is defined.

		'dx' is the horizontal window size, default value 3.
		'dy' is the vertical window size, default value 'dx'.

Restrictions:   'inimage' must have bands with pixel type unsigned byte.
                If 'dx' or 'dy' are even, they are increased by one.
		'dx' and 'dy' should be positive.

See also:	stdev(3)

Author:		Tor Lønnestad, BLAB, Ifi, UiO.

Examples:       | stdev mona.img monaStd.img
                | stdev mona.img monaStd.img 15
                | stdev mona.img monaStd.img 5 9

Id:             $Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
   IMAGE i1, i2;
   int dx, dy, bn;
   char *args;

   Iset_message(TRUE); 
   Iset_abort(TRUE); 
   InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <inimage> <outimage> [<dx> [<dy>] ] \n"));

   if (argc == 1) Usage(0, NULL);
   args = argvOptions(argc, argv);
   if ((argc < 3) || (argc > 5)) Usage(2, "Illegal number of arguments.\n");

   if (argc >= 4) dx = atoi(argv[3]); else dx = 3; 
   if (argc >= 5) dy = atoi(argv[4]); else dy = dx;
   if ((dx < 0) || (dy < 0))
     exit(Error(3, " Window size should be greater than 0 \n"));
   i1 = Iread_image(argv[1]);
   i2 = Icopy_init(i1);

   for (bn = 1; bn <= Inbands(i1); ++ bn)
     if (stdev(i1[bn], i2[bn], dx, dy))
       Warning(4, " Error in band %d\n", bn);

   Ihistory(i2, argv[0], args);
   Iwrite_image(i2,argv[2]);

   return(0);
}
