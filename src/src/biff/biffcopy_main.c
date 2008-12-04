

/*

________________________________________________________________

        biffcopy.c
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


/*P:biffcopy*

________________________________________________________________

		biffcopy
________________________________________________________________

Name:		biffcopy - copy a biff file

Syntax:		| biffcopy [-band <bandspec> [-band <bandspec>]...]
                |     <infile> [<outfile>...] 

Description:	Copy an image file from 'infile' to 'outfile'.
                If you specify "*" as 'infile', the program will
		prompt for an infile name. If you don''t specify
		'outfile', the program will promt for an outfile name.
		If more than one 'outfile' is specified, the image
	        is duplicated to all outfiles.

		This program is useful when you want to extract
		one or more bands from an image. It is handy to use
		in pipes or to execute from a program.

		Storage is allocated only for the bands requested.

Options:        '-band <bandspec>' is used when only some
		of the bands should be copied. The bandspec 
		is a list of bands or band-intervals separated
		by ":". A band-interval is specified by 'start-stop'.

Author:		Otto Milvang

Examples:	| biffcopy mona.img -                ; pipe to stdout
		| biffcopy - cp.img -                ; copy stdin to stdout
		|                                    ;  and to file cp.img
                | biffcopy monacol.img:2 monab2.img  ; copy band 2
                | biffcopy -band 2:3-1:3 - -         ; copy band 2,3,2,1,3
		|                                    ; from stdin to stdout

Id: 		$Id$
________________________________________________________________

*/

#include <xite/includes.h>
#include <xite/biff.h>
#include <xite/readarg.h>
#include <xite/message.h>
#include XITE_STDIO_H
#include XITE_STRING_H
#include XITE_MALLOC_H

int main(int argc, char **argv)
{
  char filename[132];
  char *bandspec[32], *bspec, *args;
  int *bands = NULL, stat, bnr = 0, i, num_specs = 0;
  IMAGE img;

  Iset_message(1);
  Iset_abort(0);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [<option>...] <infile> <outfile>...]\n\
       where <option> is \n\
       -band <bandspec> : Band-specification\n"));

  if (argc == 1) Usage(0, NULL);
  args = argvOptions(argc, argv);

  while (1) {
    bspec = read_switch(&argc, argv, "-b", 1, NULL);
    if (!bspec) bspec = read_switch(&argc, argv, "-band", 1, bspec);
    if (!bspec) bspec = read_switch(&argc, argv, "-bands", 1, bspec);
    if (bspec) bandspec[num_specs++] =
      strcpy((char *) malloc((size_t) (strlen(bspec) + 1) * sizeof(char)),
	     bspec);
    else break;
  }

  if (argc == 3 && argv[1][0] == '*') {
    printf("Filename: ");
    scanf("%s",filename);
  } else {
    strcpy(filename, argv[1]);
  }

  if (num_specs != 0) {
    bnr = 0;

    for (i = 0; i < num_specs; i++) {
      stat = Iparse_band_spec(bandspec[i], &bands, &bnr);
      if (stat) Error(4, "Bad band specification.\n");
    }
  }

  if (num_specs != 0)
    img = Iread_image_bands(filename, bands, bnr);
  else
    img = Iread_image(filename);

  if (img == 0) Error(3, "Error in reading %s.\n", filename);

  if (argc == 2) {
    printf("Filename: ");
    scanf("%s", filename);

    stat = Iwrite_image(img, filename);

    if (stat != 0) Error(6, "Can't write %s.\n", filename);
  } else {

    for(i=2; i<argc; i++) {
      strcpy(filename, argv[i]);

      stat = Iwrite_image(img, filename);

      if (stat != 0) Error(6, "Can't write %s.\n", filename);
    } /* for */
  }

  return(0);
}
