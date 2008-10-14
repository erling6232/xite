

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
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#ifdef HAVE_STRINGS_H
 #include <strings.h>
#else
 #ifdef HAVE_STRING_H
  #include <string.h>
 #endif
#endif
#ifdef HAVE_MALLOC_H
 #include <malloc.h>
#endif
#include <xite/message.h>
#include <xite/readarg.h>
#include <xite/cdoc.h>



/*F:add_html_toc*

________________________________________________________________

		add_html_toc
________________________________________________________________

Name:		add_html_toc - 

Syntax:		| #include <xite/cdoc.h>
		| 
		| void add_html_toc( FILE *FILEin, char **headers,
		|    int num_headers, FILE *FILEout, int add_toc,
		|    int add_ref );
		|
Description:	Read a html file from 'FILEin'. Assume that 'headers' refers
                to a list of section titles. Add a table of contents for
		these section titles at the top of the html file in 'FILEout'
		if 'add_toc' is nonzero. Add links to the reference manual
		and the home of XITE if 'add_ref' is nonzero.
		Also make internal hypertext links work.

		'FILEin' and 'FILEout' must be open for reading and writing
		respectively.

See also:	'cdoc(1)', 'add_html_toc(1)'

Author:		Svein Bøe, Ifi, UiO
Id: 		$Id$
________________________________________________________________

*/

void add_html_toc(FILE *FILEin, char **headers, int num_headers, FILE *FILEout, int add_toc, int add_ref)
{
  char *start, buf[513], t[513], buf2[513];
  int j, found, line_num = 0;
  char *s;

  buf2[0] = '\0';

  /* Compare each line in infile with each header. */
  while (1) {
    if ((s = fgets(buf, 512, FILEin)) == NULL) break;
    /* fprintf(stderr, "%s", s); */
    line_num++;

    if ((start = strstr(buf, "<h1>")) == buf) {
      /* Found toplevel heading (manual page title). */

      fprintf(FILEout, "%s\n\n", buf);

      /* Link to Reference manual and home of XITE. */

      if (add_ref) {
	fprintf(FILEout, "%s%s%s%s\n",
		"[ <a href=\"Contents.html\">",
		"XITE Reference Manual</a> |",
		" <a href=\"http://www.ifi.uio.no/~blab/Software/Xite/\">",
		"XITE home</a> ]");
      }

      /* Search for first <h2> heading. */
      while (1) {
	if ((s = fgets(buf, 512, FILEin)) == NULL) break;
	line_num++;

	if ((start = strstr(buf, "<h2>")) == buf) break;
	fprintf(FILEout, "%s", buf);
      }

      if (add_toc) {
	/* Print table of contents. */

	fprintf(FILEout, "%s\n%s\n",
		"<h2>Contents</h2>", "<ul>");
	for (j = 0; j < num_headers; j++) {
	  fprintf(FILEout, "%s%s%s%s%s\n",
		  "  <li> <a href=\"#", headers[j], "\">", headers[j], "</a>");
	}
	fprintf(FILEout, "</ul>\n<hr>\n");
      }

      if (buf) fprintf(FILEout, "%s", buf);

      continue;
    } /* toc */

    found = 0;
    for (j = 0; j < num_headers; j++) {
      strcat(strcat(strcpy(t, "<h2>"), headers[j]), "</h2>");

      if (add_toc && (start = strstr(buf, t)) != NULL) {
	/* Make reference and print line. */

	found = 1;
	strncpy(t, buf, start-buf+4); t[start-buf+4] = '\0';
	strcat(t, "<a NAME=\"");
	strcat(t, headers[j]);
	strcat(t, "\">");
	strcat(t, headers[j]);
	strcat(t, "</a></h2>");
	fprintf(FILEout, "%s", t);

	break;
      } /* if */
    } /* for each header */

    if (!found && buf) fprintf(FILEout, "%s", buf);
  } /* while */

  return;

} /* add_html_toc() */
