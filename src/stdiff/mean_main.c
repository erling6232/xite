
/*

________________________________________________________________

        mean.c
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
#include <xite/includes.h>
#include <xite/biff.h>
#include <xite/stdiff.h>
#include XITE_STDIO_H
#include <stdlib.h>
#include <xite/blab.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include XITE_MALLOC_H



/*P:mean*

________________________________________________________________

		mean
________________________________________________________________

Name:		mean - compute local mean

Syntax:		mean <infile> <outfile> [deltax [deltay]] 

Description:	| Local Mean 
		| deltax  = Local area width (default = 3)
		| deltay  = Local area height (default = deltax)

Restrictions:   Only non-complex pixel-types.

Author:		Otto Milvang, Ifi, UiO
Revised:        Svein Bøe, Ifi, UiO (to allow all non-complex pixel types)

Examples:	| mean mona.img monamin.img
                | mean mona.img monamin.img 5
                | mean mona.img monamin.img 3 9

Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE input, output;
  int i, dx, dy;
  char *args;

  Iset_message(TRUE);
  Iset_abort(TRUE);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <input> <output> [<dx> [<dy>]]\n"));

  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);
  if ((argc < 3) || (argc > 5)) Usage(1, "Illegal number of arguments.\n");

  if (argc >= 4) dx = atoi(argv[3]); else dx = 3;
  if (argc == 5) dy = atoi(argv[4]); else dy = dx;

  input  = Iread_image(argv[1]);
  output = Icopy_init(input);  

  for (i=1; i<=Inbands(input); i++)
    mean(input[i], output[i], dx, dy);

  Ihistory(output, argv[0], args); 
  Iwrite_image(output, argv[2]);

  return(0);
}
