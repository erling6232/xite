

/*

________________________________________________________________

        stvar.c
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
#include XITE_UNISTD_H
#include <xite/blab.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include XITE_MALLOC_H
#include XITE_STRING_H

/*P:stvar*

________________________________________________________________

		stvar
________________________________________________________________

Name:		stvar - local variance

Syntax:		stvar [-t] <inimage> <outimage> <deltax> <deltay> <k>

Description:	Local variable threshold.

                | t(x,y) = k*s(x,y) + u(x,y)
		|      r(x,y) = b(x,y) - t(x,y) + 128
		| [-t] r(x,y) = if( b(x,y) >= t(x,y)) then 255 else 0 
		|
		| r(x,y)  = output image
		| u(x,y)  = mean component
		| s(x,y)  = st. dev component
		| b(b,y)  = pixel value in (x,y)
		|
		| deltax  = Local area width
		| deltay  = Local area height

Options:        &-t
                Threshold the result.

Restrictions:   'input' must have bands with pixel type unsigned byte.

Reference:	Wayne Niblack: An introduction to Digital Image Processing

Author:		Otto Milvang

Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE input, output;
  int i, t;
  char *args;

  Iset_message(TRUE);
  Iset_abort(TRUE);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-t] <inimage> <outimage> <deltax> <deltay> <k>\n"));

  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);
  if (argc != 7 && argc !=6) Usage(1, "Illegal number of arguments.\n");

  if (strcmp(argv[1],"-t") == 0) t = 1; else t = 0;
  input  = Iread_image(argv[1+t]);
  output = Icopy_init(input);  

  for (i=1; i<=Inbands(input); i++)
    stvar(input[i], output[i],
            atoi(argv[3+t]), atoi(argv[4+t]), atof(argv[5+t]), t);

  Ihistory(output, argv[0], args);
  Iwrite_image(output, argv[2+t]);

  return(0);
}
