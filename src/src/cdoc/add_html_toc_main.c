

/*C*

________________________________________________________________

        add_html_toc
        $Id$
        Copyright 1997, Blab, UiO
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
#include XITE_STDIO_H
#include XITE_STRING_H
#include XITE_MALLOC_H
#include <xite/message.h>
#include <xite/readarg.h>
#include <xite/cdoc.h>

/*P:add_html_toc*

________________________________________________________________

		add_html_toc
________________________________________________________________

Name:		add_html_toc - Add a table of contents to an html file

Syntax:		add_html_toc <html-infile> <headers-file> <html-outfile>

Description:	'headers-file' must contain a list of section titles.
                A table of contents is added at the top of the contents
		of 'html-infile' and returned in 'html-outfile'.

See also:	add_html_toc(3)

Author:		Svein Bøe, Ifi, UiO

Id: 		$Id$
________________________________________________________________

*/

int main(int argc, char **argv)
{
  char *args; /* To hold input arguments. */
  char *infile, *headfile, *outfile, buf[512], *header[512];
  FILE *FILEin, *FILEhead, *FILEout;
  int num_heads = 0;

  /* Install usage message (end multiple lines with \n\)
   * xite_app_std_usage_text() will append a description of standard XITE
   * options. Replace xite_app_std_usage_text by ximage_app_usage_text
   * if the application uses the ximage toolkit.
   */
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <html-file> <headers-file> <outfile>\n"));

  /* Give usage message if no command-line arguments are given. */
  if (argc == 1) Usage(1, NULL);

  /* Save command-line arguments before processing the command-line with
   * read_switch() function calls.
   */
  args = argvOptions(argc, argv);

  /* Process input options */
  /* Boolean: b=read_bswitch(&argc, argv, "option-name"); */
  /* Integer: i=read_iswitch(&argc, argv, "option-name", default-value); */
  /* Double:  d=read_dswitch(&argc, argv, "option-name", default-value); */
  /* Text:    t=read_switch (&argc, argv, "option-name",1, "default-value"); */

  /* Give usage message if arguments are missing */
  if (argc != 4) Usage(2, "Illegal number of arguments.\n");

  infile   = argv[1];
  headfile = argv[2];
  outfile  = argv[3];

  if (!strcmp(infile, "-")  || !strcmp(infile, "-0"))   FILEin   = stdin;
  else FILEin = fopen(infile, "r");
  if (!strcmp(headfile, "-")|| !strcmp(headfile, "-0")) FILEhead = stdin;
  else FILEhead = fopen(headfile, "r");
  if (!strcmp(outfile, "-") || !strcmp(outfile, "-1"))  FILEout  = stdout;
  else FILEout = fopen(outfile, "w");

  /* Read file with section headers. */
  while (1) {
    if (!fgets(buf, 512, FILEhead)) {
      break;
    }
    buf[strlen(buf)-1] = '\0';

    header[num_heads] = strcpy((char *) malloc(strlen(buf) + 1), buf);
    num_heads++;
  }
  if (FILEhead != stdin) fclose(FILEhead);

  add_html_toc(FILEin, header, num_heads, FILEout, 1, 1);

  if (FILEin  != stdin)  fclose(FILEin);
  if (FILEout != stdout) fclose(FILEout);

  return(0);

} /* main() */
