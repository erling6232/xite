 

/*

________________________________________________________________

        cdoc.c
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


/*P:cdoc*

________________________________________________________________

		cdoc
________________________________________________________________


Name:           cdoc - Extract documentation from comments in c-programs.

Syntax:         | cdoc [<option>...] <input_file>
                | cdoc [<option>...] <input_file> <output_file>
                | cdoc [<option>...] <input_file> <output_dir>

Description:    Extract comments from c-files and generate documentation.

		'cdoc' is a tool for automatic documentation of c-programs.
		cdoc will search for the pattern "/*?*" (? is any letter),
		"/*?:subject*" or "/*?:subject=manpage* /" 
		in the c-code and format the comments (which end with * /).
		The latter kind of specification links the description of
		'subject' to the description of 'manpage' (see 'Format 3'
		below).

		The comment section must end with a line starting with the
		two characters '*' and '/'.

                'input_file' can be given as "-" or "-0" (for stdin), and
		'output_file' can be given as "-" or "-1" (for stdout). If
		only one filename is specified, this is interpreted as
		'input_file'. Default input extension is ".c" (see option
		'-s').

		The result is written to a file with the same main name and
		extension according to document type. If two file names are
		specified, the first name is 'input_file', and the second name
		is 'output_file'. If 'output_file' is a directory, i.e.
		'output_dir', the output filename becomes 'output_dir/subject'
		with some suitable extension.
 
Options:        &-e spec
                You may specify which letters or subject 'cdoc' should 
		search for by specifying -e <letter> or -e <letter:subject>.
		Default search letters are "DFP". Multiple '-e' options may
		be specified.

		If you want to create a Unix manual page or hypertext
		reference manual page, the 'man' section number is derived
		from 'letter':
		&&P
		section number 1
		&&S 
		section number 2
		&&F
		section number 3
		&&E
		section number 4
		&&D
		section number 5
		&&G
		section number 6
		&&M
		section number 7
		&&A
		section number 8

		&-m sec
		If you want to create a Unix manual page or hypertext reference
		manual page, you must either use the switch -m 'sec' (where
		'sec' is the 'man' section number), or specify the section
		number through 'letter' in an '-e' option.

		&-h
		Produce html code (-> _1.html, _3.html or _5.html)
		A table of contents list is written at the top of the document
		if the document consists of more than a number of lines given
		by the '-k' option.

                &-l
		Produce LaTeX code (default) (-> .tex)

                &-n
		Produce nLaTeX code (-> .tex)

                &-t
		Produce troff code (-> .1, .3 or .5)

                &-x
		Extract comments without formatting (-> .txt)

		&-c
		Do not add table of contents to top of hypertext reference
		manual page.

		&-k num
		If option '-h' is in use and the hypertext reference manual
		page exceeds approximately 'num' lines, a table of contents
		will be added at the top of the page. Default: 80.

		&-s extension
		Use 'extension' as default extension for the input source
		file.

Format 1:       | Line  1: /*?:subject*
		| Line  2:
		| Line  3: _______________________________________
		| Line  4:
		| Line  5: 		modulename
		| Line  6: _______________________________________
		| Line  7:
		| Line  8: Name:	modulename - one line description
		| Line  9: Syntax:	| modulename [ <option>... ] args
		| Line 10:              | more syntax 
		| Line 11: Keyword 1:   text
		| Line 12:              text may go over more than one line
		| Line 13: Keyword 2:   You may underline a word by
		| Line 14:              putting it between 'apostrophes'
		| Line 15:              (apostrophes may be printed by
		| Line 16:              using two apostrophes '').
		| Line 17:
		| Line 18:		Lines will be aligned if you
		| Line 19:		prefix them with a '|'.
		| Line 20:
		| Line 21:              &An ampersand starts an item.
		| Line 22:              The line with the ampersand will
		| Line 23:              be the name of the item.
		| Line 24:              The following lines will be
		| Line 25:              automatically indented.
		| Line 26:              The indentation can be cancelled
		| Line 27:              -&like this. 
		| Line 28:              &&A double ampersand starts a
		| Line 29:              second level of item indentation.
		| Line 30:              It can be cancelled by -&&.
		| Line 31:              See the 'Options:' section above
		| Line 32:              and src/xshow/xshow.c for
		| Line 33:              examples.
		| Line 34:
		|       etc.
		| Line n: * /
		
		Only one dash ("-") is allowed in the Name section above.
                If several items have the same description, they must appear
                in a comma-separated list before the dash.

                Leading blanks are skipped. If the first non-blank symbol
                is a "|", this symbol is skipped, and the rest of the line
                is written unformatted.

Format 2:	| Line  1: /*?*
		| Line  2:
		| Line  3: _______________________________________
		| Line  4:
		| Line  5: 		modulename
		| Line  6: _______________________________________
		| Line  7:
		| etc.
		Same as above.

Format 3:	| Line  1: /*?:subject=manpage* /
		Make a man page which is a link to another manpage.

		In this case, the comma-separated list of names in the
		Name section must start with 'manpage'. This is to ensure
		correct links for the hypertext reference manual.

Files:          $XITE_HOME/etc/emacs-header - Auto header generator.
Examples:	| cdoc -t -e P:cdoc cdoc.c $XITE_MAN/man1/cdoc
                | cdoc -t -e F:func -e P:func func.c $XITE_MAN/
Author:         Otto Milvang
Id: 		$Id$
________________________________________________________________

*/


#include <ctype.h>
#include <xite/includes.h>
#include <xite/biff.h>
#include <stdlib.h>
#include <xite/cdoc.h>
#include <xite/strings.h>
#include <xite/message.h>
#include <errno.h>
#include XITE_STAT_H
#include XITE_STDIO_H
#include XITE_STRING_H
#include XITE_TIME_H
#include XITE_TOUPPER_H
#include XITE_UNISTD_H
#include XITE_MKTEMP_H

#ifdef MAIN

static FILE *inp_s, *outp_s;
static char man_s[20]      ="1";
static char symb_s[80]     = "DFP";
static int hypertextLink_s = 0;
static int items=0;
static int outp_line_num_s;

/* Standard white-space characters. */
#define TAB_CHAR          '\t'  /* Horizontal tab */
#define VTAB_CHAR         '\v'  /* Vertical tab */
#define NEWLINE_CHAR      '\n'  /* Newline */
#define CR_CHAR           '\r'  /* Carriage return */
#define FORM_FEED_CHAR    '\f'  /* Form-feed */
#define SINGLE_QUOTE_CHAR '\''

/* Built-in isspace() will return 1 for e.g. 'ÿ'. */
#define is_space(c) \
  ((c) == ' ' || (c) == TAB_CHAR || (c) == VTAB_CHAR || \
   (c) == NEWLINE_CHAR || (c) == CR_CHAR || (c) == FORM_FEED_CHAR)

#define INDENT_TAB    7

typedef enum {
  LATEX,
  NLATEX,
  TROFF,
  TEXT,
  HTML
} doc_type;

#ifndef MIN
# define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif /* MIN */

/* Read from inp_s into line until next EOF or newline. 
 * Skip '#' as first character on line (to allow comment-headers in
 * scripts).
 * Substitute TAB by 8 blanks.
 * Substitute NEWLINE by null character.
 * Return EOF or first character after NEWLINE. */

#ifndef FUNCPROTO
static int getline(line)
char *line;
#else /* FUNCPROTO */
static int getline(char *line)
#endif /* FUNCPROTO */
{
  int cnr, intch;

  cnr = 0;
  for(;;) {
    intch = getc(inp_s);
    if (cnr == 0 && intch == '#') intch = getc(inp_s);
    if (intch == EOF) return(EOF);
    *line = (char) intch;
    cnr ++;
    
    /* Replace TAB character by 8 blanks. */
    if (*line == TAB_CHAR) {
      *line = ' ';
      while((cnr & 7) != 0) {
	line++;
	cnr++;
	*line = ' ';
      }
    }
    if (*line == NEWLINE_CHAR) *line = '\0';
    if (*line == '\0') break;
    line ++;
  }
  return(*line);
}

static char newpage[][30]    = { 
  "\\newpage\n",        
  "/newpage\n",        
  "",
  "",
  "", };
static char subsection[][30] = { 
  "\n\\subsection*{",
  "\n/subsection*<", 
  ".TH ",
  "",
  "<H1>", };
static char subsubsec[][30]  = { 
  "\n\\subsubsection*{",
  "\n/subsubsection*<",
  ".SH ",
  "",
  "<H2>", };
static char beginVerb[][30]  = {
  "\\begin{verbatim}\n",
  "/begin<verbatim>\n",
  "", 
  "",
  "<PRE>\n", };
static char endVerb[][30]    = { 
  "\\end{verbatim}\n", 
  "/end<verbatim>\n",
  "\n", 
  "",
  "</PRE>\n", };
static char beginList[][60]  = { 
  "\\begin{list}{}{\\item[]\\setlength{\\parsep}{1ex}}\n",
  "/begin<list><></item[]>\n",
  "", 
  "",
  "", };
static char beginDescr[][60]  = { 
  "\\begin{list}{}{\\item[]\\setlength{\\parsep}{1ex}}\n",
  "/begin<list><></item[]>\n",
  "", 
  "",
  "<DL>\n", };
static char endList[][30]    = { 
  "\\end{list}\n\n",     
  "/end<list>\n\n",
  "\n", 
  "",
  "", };
static char endDescr[][30]    = { 
  "\\end{list}\n\n",     
  "/end<list>\n\n", 
  "\n", 
  "",
  "</DL>\n", };
static char endSubsec[][30]     = {
  "}\n",
  ">\n",
  "\n",
  "\n",
  "</H1>\n", };
static char endSubsubsec[][30]     = {
  "}\n",
  ">\n",
  "\n",
  "\n",
  "</H2>\n", };
static char begBold[][30]    = { 
  "{\\bf ",
  "</bf ", 
  ".B ",
  "'",
  "<B>", };
static char endBold[][30]    = { 
  "}",
  ">", 
  "",
  "'",
  "</B>", };
static char beginIt[][30]    = { 
  "{\\em ",
  "</em ", 
  ".I ",
  "'",
  "<EM>", };
static char endIt[][30]      = { 
  "}",
  ">",    
  "", 
  "'",
  "</EM>", };
static char item[][30]     = { 
  "\\item[",
  "/item[",
  ".TP ",
  "",
  "<LI>", };
static char descr_item[][30]     = { 
  "\\item[",
  "/item[",
  ".TP ",
  "",
  "<DT>", };
static char descr_item2[][30]     = { 
  "\\item[",
  "/item[",
  ".sp",
  "",
  "<DT>", };
static char descr_item3[][30]     = { 
  "\\item[",
  "/item[",
  ".in -.5i\n.sp",
  "",
  "<DT>", };
static char endItem[][30]     = { 
  "]",
  "]",
  "",
  "",
  "", };
static char end_descr_item[][30]     = { 
  "]",
  "]",
  "",
  "",
  "\n<DD>", };
static char end_descr_item2[][30]     = { 
  "]",
  "]",
  ".in +.5i\n",
  "",
  "\n<DD>", };
static char end_indent[][30]     = { 
  "",
  "",
  ".P \n",
  "",
  "", };
static char end_indent2[][30]     = { 
  "\\end{list}\n\n",
  "/end<list>\n\n",
  ".in -.5i\n",
  "",
  "", };
#define endIndent(mode) \
  if (mode != TROFF && items < 2) fprintf(outp_s, end_indent[mode]); \
  else fprintf(outp_s, end_indent2[mode]);

static char slash[][30]      = {
  "\\slash ",
  "/slash",
  "",
  "/",
  "/", };
static char newline[][30]      = {
  "\\mbox{} \n",
  "/mbox<> \n",
  "",
  "",
  "<BR>\n", };
static char ext[][30]        = { 
  "*.tex",
  "*.tex", 
  "*.1",
  "*.txt",
  "*_1.html", };

/* Transform special characters in line and print to outp_s. */

#ifndef FUNCPROTO
static void putlinel(line, heading, cr, quote)
char *line;
int heading, cr, quote;
#else /* FUNCPROTO */
static void putlinel(char *line, int heading, int cr, int quote)
#endif /* FUNCPROTO */
{
  char c, oldc = 0;
  int italic;
  italic = 0;
  while(is_space(*line)) line++;
  for(;;) { 
    if(*line == SINGLE_QUOTE_CHAR) {
      if(*(line+1) != SINGLE_QUOTE_CHAR) {
	if(italic)  fprintf(outp_s, "%s", endIt[LATEX]);
	else  fprintf(outp_s, "%s", beginIt[LATEX]);
	italic = 1 - italic;
      }
      line++;
    }
    if ( (c = *line) == 0) break;
    
    if (isalnum(c)) putc(c, outp_s);
    else if (iindex(",;.:[]{}|'-()@?\"", c) != NULL)  putc(c, outp_s);
    else if (c == '/') fprintf(outp_s, slash[LATEX]);
    else if(iindex("*+=<>", c) != NULL) fprintf(outp_s, "\\mbox{$%c$}", c);
    else {
      if (c != ' ') {
	putc('\\', outp_s); putc(c, outp_s);
      } else putc(' ', outp_s);
    } 

    oldc = c;
    line++;
  }
  if(italic) fprintf(outp_s, "%s", endIt[LATEX]);
  if(cr) putc('\n', outp_s);
} /* putlinel() */

#ifndef FUNCPROTO
static void putlinen(line, heading, cr, quote)
char *line;
int heading, cr, quote;
#else /* FUNCPROTO */
static void putlinen(char *line, int heading, int cr, int quote)
#endif /* FUNCPROTO */
{
  char c;
  int italic;
  italic = 0;
  while(is_space(*line)) line++;
  for(;;) { 
    if(*line == SINGLE_QUOTE_CHAR) {
      if(*(line+1) != SINGLE_QUOTE_CHAR) {
	if(italic)  fprintf(outp_s, "%s", endIt[NLATEX]);
	else  fprintf(outp_s, "%s", beginIt[NLATEX]);
	italic = 1 - italic;
      }
      line++;
    }
    if ( (c = *line) == 0) break;
    
    if (isalnum(c)) putc(c, outp_s);
    else if(iindex("{|}[\\]", c) != NULL) putc(c, outp_s);
    else if(iindex(",;.:", c) != NULL) putc(c, outp_s);
    else if(c == '/') fprintf(outp_s, slash[NLATEX]);
    else if(iindex("*+-=<>", c) != NULL) fprintf(outp_s, "/mbox<$%c$>", c);
    else { putc('/', outp_s); putc(c, outp_s); } 
    
    line++;
    
  }
  if(italic) fprintf(outp_s, "%s", endIt[NLATEX]);
  if(cr) putc('\n', outp_s);
} /* putlinen() */



#ifndef FUNCPROTO
static void putlinet(line, heading, cr, quote)
char *line;
int heading, cr, quote;
#else /* FUNCPROTO */
static void putlinet(char *line, int heading, int cr, int quote)
#endif /* FUNCPROTO */
{
  char c;
  time_t msec;
  int pos;
  char *dat; 
  int italic, CR;

  italic = 0;
  pos = 0;
  CR  = 0;

  /* Capitalize section-heading characters. */
  if (heading) { 
    dat = line;
    while(*dat) { 
      if (islower(*dat)) *dat = toupper(*dat);
      dat++;
    }
  }
  
  while(is_space(*line)) line++;
  
  for(;;) {
    if (*line == SINGLE_QUOTE_CHAR) {
      if (*(line+1) != SINGLE_QUOTE_CHAR) {
	while (*(line+1) && italic && iindex(",;.:]})?", *(line+1)) != NULL)
	  fprintf(outp_s, "%c",  *(++line)); 
	if(pos && !italic && !CR) putc('\n', outp_s);
	if (italic) fprintf(outp_s, "%s", endIt[TROFF]); 
	else fprintf(outp_s, "%s", beginIt[TROFF]);
	italic = 1 - italic;

	line++;
	while(is_space(*line)) line++;
	if(*line && !italic) {
	  CR = 1;
	  putc('\n', outp_s);
	} else CR = 0;

      } else { /* Double quote, print single quote. */
	fprintf(outp_s, "'");

	line += 2;
	while(is_space(*line)) line++;
	CR = 0;
      }
    } /* Single quote. */
    
    if((c = *line) == 0) break;
    if (c != SINGLE_QUOTE_CHAR) {
      putc(c, outp_s);
      pos++;
      line++;
      CR = 0;
    }
  } /* for */
  
  if (heading == 1) {
    msec = 0;
    msec = time(&msec);
    dat = ctime(&msec);
    dat[7] = 0; dat[10] = 0; dat[24] = 0;
    fprintf(outp_s, " %s \"%s %s %s\"", man_s, &dat[8], &dat[4], &dat[20]);
  } 
  if (cr) putc('\n', outp_s);
}

#ifndef FUNCPROTO
static void putlinex(line, heading, cr, quote)
char *line;
int heading, cr, quote;
#else /* FUNCPROTO */
static void putlinex(char *line, int heading, int cr, int quote)
#endif /* FUNCPROTO */
{
  fprintf(outp_s, "%s", line);
  if(cr) putc('\n', outp_s);
}

#ifndef FUNCPROTO
static char *putlineh(line, heading, cr, quote)
char *line;
int heading, cr, quote;
#else /* FUNCPROTO */
static char *putlineh(char *line, int heading, int cr, int quote)
#endif /* FUNCPROTO */
{
  char c, oldc = 0, olderc = 0;
  char *outbuf, *ptr;
  int italic = 0;

  ptr = outbuf = (char *) malloc(512);

  while(is_space(*line)) line++;
  for(;;) { 
    if(*line == SINGLE_QUOTE_CHAR) {
      if(*(line+1) != SINGLE_QUOTE_CHAR) {
	if(italic)  {
	  sprintf(ptr, "%s", endIt[HTML]);
	  ptr += strlen(endIt[HTML]);
	} else {
	  sprintf(ptr, "%s", beginIt[HTML]);
	  ptr += strlen(beginIt[HTML]);
	}
	italic = 1 - italic;
      }
      line++;
    }
    if ( (c = *line) == 0) break;
    
    if (isalnum(c)) *ptr++ = c;
    else if (hypertextLink_s && c == ')') {
      /* Could this be the end of a manual-page reference to another XITE
       * symbol ?
       */
      if (isdigit(oldc) && olderc == '(' && (oldc >= '1' && oldc <= '8')) {
	/* Yes, it is fairly safe to assume so. */
	char *line2, *name, *ptr2;
	int len;

	/* Look backwards to the start of the symbol name. */
	line2 = line - 3;
	while (isalnum(*line2) || *line2 == '_')
	  line2--;
	if (!(isalnum(*line2) || *line2 == '_')) line2++;

	/* name will refer to the symbol for which there
	 * should be an XITE man-page (and thus also an html file).
	 * Replace this (including the following "(n)") in outbuf by a URL.
	 */
	len = line - line2 - 2;
	name = (char *) malloc(len+1);
	(void) strncpy(name, line2, len);
	*(name + len) = '\0';
	
	ptr2 = ptr - len - 2;
	sprintf(ptr2, "%s%s%s%c%s%s%s%c%s%s",
		"<A HREF=\"", name, "_", oldc, ".html\">",
		name, "(", oldc, ")", "</A>");
	ptr = ptr2 +
	  strlen("<A HREF=\"") + 2 * len +
	    strlen("_1.html\">") + strlen("(1)") + strlen("</A>");
	free(name);
      } /* Reference to other XITE symbol. */
    }
    else if (c == '/') {
      sprintf(ptr, slash[HTML]);
      ptr += strlen(slash[HTML]);
    }
    else if (iindex("<", c) != NULL) {
      sprintf(ptr, "&lt;");
      ptr += 4;
    }
    else if (iindex(">", c) != NULL) {
      sprintf(ptr, "&gt;");
      ptr += 4;
    }
    else if (iindex("&", c) != NULL) {
      sprintf(ptr, "&amp;");
      ptr += 5;
    }
    else if (iindex("\"", c) != NULL) {
      sprintf(ptr, "&quot;");
      ptr += 6;
    }
    /* else if (iindex("$_,;.:[]{}()|'-@?*+=Ê¯Â∆ÿ≈!%", c) != NULL) *ptr++ = c;
     */
    else {
      /*
	if (c != ' ') {
	*ptr++ = '\\'; *ptr++ = c;
      } else *ptr++ = ' ';
      */
      *ptr++ = c;
    } 

    olderc = oldc;
    oldc = c;
    line++;
  }
  if(italic) {
    sprintf(ptr, "%s", endIt[HTML]);
    ptr += strlen(endIt[HTML]);
  }
  if (oldc == 0 && olderc == 0) {
    /* Empty line. */

    sprintf(ptr, "%s", "</P>");
    ptr += strlen("</P>");
  }
  if(cr) *ptr++ = '\n';

  *ptr = '\0';
  fprintf(outp_s, "%s", outbuf);

  return(outbuf);

} /* putlineh() */

/* Transform special characters in line and print to outp_s. */

#ifndef FUNCPROTO
static char *putline(mode, line, header, cr, quote)
doc_type mode;
char *line;
int header, cr, quote;
#else /* FUNCPROTO */
static char *putline(doc_type mode, char *line, int header, int cr, int quote)
#endif /* FUNCPROTO */
{
  char *t = NULL;

  outp_line_num_s++;

  switch(mode)
    {
    case LATEX:  putlinel(line, header, cr, quote); break;
    case NLATEX: putlinen(line, header, cr, quote); break;
    case TROFF:  putlinet(line, header, cr, quote); break;
    case TEXT:   putlinex(line, header, cr, quote); break;
    case HTML:   t = putlineh(line, header, cr, quote); break;
    }

  return(t);
}

#ifndef FUNCPROTO
static void descrItem(mode, line, header, cr, quote)
doc_type mode, header, cr, quote;
char *line;
#else /* FUNCPROTO */
static void descrItem(doc_type mode, char *line, int header, int cr, int quote)
#endif /* FUNCPROTO */
{
  if (mode != TROFF || items < 2)
    fprintf(outp_s, "%s\n", descr_item[mode]);
  else if (items > 2) fprintf(outp_s, "%s\n", descr_item3[mode]);
  else fprintf(outp_s, "%s\n", descr_item2[mode]);

  fprintf(outp_s, begBold[mode]);
  putline(mode, line, header, cr, quote);
  fprintf(outp_s, endBold[mode]);

  if (mode != TROFF || items < 2)
    fprintf(outp_s, "%s", end_descr_item[mode]);
  else fprintf(outp_s, "%s", end_descr_item2[mode]);

  return;
} /* descrItem() */

#ifndef FUNCPROTO
static void strip(txt)
char *txt;
#else /* FUNCPROTO */
static void strip(char *txt)
#endif /* FUNCPROTO */
{
  char *p, *t;

  t = p = &(txt[strlen(txt)]); /* Referring to '\0'. */
  if (p > txt) p--;  /* Last character in txt. */

  while (p >= txt && *p == ' ') p--;

  if (p < t) {
    p++;
    *p = '\0';
  }

  return;

} /* strip() */

#ifndef FUNCPROTO
int main(argc, argv)
int argc;
char *argv[];
#else /* FUNCPROTO */
int main(int argc, char **argv)
#endif /* FUNCPROTO */
{
  int err=0;
  int comment = 0, written = 0, linked = 0;
  int empty = 1, quote = 0;
  int itembegin = 0;
  int linelen, opt;
  int symbnr=0, antsymb = 0;
  int lineNum = 0;
  int indent_tab = 0;
  int num_subsub_header = 0;
  int toc = 1;
  int ntoc = 80;
  char *symbarr[200];
  char *subsub_header[500];
  char line[512], *lin, *orgarg;
  char outbuf[512], *ptr, extension[100];
  char *infilename, *outfilename;
  doc_type mode = LATEX;
#ifndef _WIN32
  mode_t umask_old = 0, umask_new = S_IWGRP | S_IWOTH;
#endif

  Iset_message(1);
  Iset_abort(0);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-lntxh] [-m <sec>] [-e <expr>] [-c] [-k <num>] [-s <ext>] \n\
       <input_file> [<output_file> | <output_dir>]\n"));

/*
      Test command line and open files, write error message
*/
  symbarr[0] = "PFD";
  (void) strcpy(extension, "*.c");
  while((opt = attgetopt(argc, argv, "hlnxtcm:e:k:s:")) != EOF)
    switch((char) opt)
      {
      case 'c': toc  = 0;                    break;
      case 'k': sscanf(att_optarg, "%d", &ntoc); break;
      case 'l': mode = LATEX;                break;
      case 'n': mode = NLATEX;               break;
      case 't': mode = TROFF;                break;
      case 'x': mode = TEXT;                 break;
      case 'h': mode = HTML;                 break;
      case 'm': strcpy(man_s, att_optarg);       break;
      case 's': strcpy(extension, att_optarg);   break;
      case 'e': strcpy(symb_s, att_optarg);
                symbarr[antsymb++] = att_optarg;
                break;
      default: 	err =  5;                    break;
    }

  if (argc - att_optind <= 0) Usage(1, NULL);

  if(err) Usage(1, "Illegal option.\n");

#ifndef _WIN32
  umask_old = umask(umask_new);
#endif

  if (antsymb==0) antsymb=1;
  ext[TROFF][2] = man_s[0];
  orgarg = argv[att_optind+1];

  for (symbnr=0; symbnr<antsymb; symbnr++) {
    num_subsub_header = 0;
    outp_line_num_s   = 0;
    written           = 0;
    linked            = 0;

    strcpy(symb_s, symbarr[symbnr]);
    /* fprintf(stderr, "-%s-\n", symb_s); */
    if (symb_s[1] == ':' && (mode == TROFF || mode == HTML)) {
      /* troff or html output with section-number and subject specified.
       * Test section-number.
       */
      switch(symb_s[0])
	{
	case 'P': man_s[0] = ext[mode][2] = '1'; break;
	case 'S': man_s[0] = ext[mode][2] = '2'; break;
	case 'F': man_s[0] = ext[mode][2] = '3'; break;
	case 'E': man_s[0] = ext[mode][2] = '4'; break;
	case 'D': man_s[0] = ext[mode][2] = '5'; break;
	case 'G': man_s[0] = ext[mode][2] = '6'; break;
	case 'M': man_s[0] = ext[mode][2] = '7'; break;
	case 'A': man_s[0] = ext[mode][2] = '8'; break;
	}
    }

    if ((mode == TROFF || mode == HTML) &&
	argc-att_optind == 2 && orgarg[strlen(orgarg)-1] == '/') {
      /* Output directory given on command-line.
       * Construct man- or html-filename.
       */
      strcpy(line, orgarg);
      if (mode == TROFF) {
	strcat(line, "man");         /* Add man section directory. */
	strcat(line, &ext[mode][2]);
	strcat(line, "/");
      }
      strcat(line, &symb_s[2]);      /* Subject name. */
      strcat(line, &ext[mode][1]);   /* Denoting program, routine or format. */
      argv[att_optind+1] = line;	
      /* fprintf(stderr, "-%s-\n", line);  */
      open_files(argc, argv, &inp_s, &outp_s, extension, ext[mode], 0,
		 &infilename, &outfilename);
    } else if (symbnr == 0) {
      open_files(argc, argv, &inp_s, &outp_s, extension, ext[mode], 0,
		 &infilename, &outfilename);
    }
    
    lineNum = 0;
    while (getline(line) != EOF) {
      strip(line);
      lineNum++;
      linelen = strlen(line);
      
      /* ---- Test comment on / off */
      
      if (linelen) {
	/* Not an empty (only newline) line. */
	if (comment) {
	  /* Inside comment. */
	  if (strncmp(line, "*/", 2) == 0) {
	    /* Comment ends. */
	    comment=0;
	    if (quote) fprintf(outp_s, endVerb[mode]);
	    if (items > 0) {
	      items = MIN(2, items);
	      while (items--) {
		fprintf(outp_s, endDescr[mode]);
		endIndent(mode);
	      }
	    }
	    fprintf(outp_s, endList[mode]);
	    quote = 0;
	  }
	} /* comment */
	else {
	  /* Not inside comment. Does one start? */
	  if (strlen(line) > 3 && 
	      line[0] == '/' && 
	      line[1] == '*' &&
	      ( (line[3] == '*' && symb_s[1] != ':' &&
		 strchr(symb_s, line[2]) != NULL) ||
	       ((strncmp(symb_s, &line[2], strlen(symb_s)) == 0) &&
		(line[2 + strlen(symb_s)] == '=' || 
		 line[2 + strlen(symb_s)] == '*') ))) {
	    /* Comment starts. */
	    if (line[2 + strlen(symb_s)] == '=') {
	      /* Link to other manual page. */
	      if (mode == TROFF) {
		written = 1;
		line[strlen(line)-2] = 0;
		fprintf(outp_s, ".so man%c/%s.%c\n", ext[mode][2], 
		      &line[3+strlen(symb_s)], ext[mode][2]);
	      } else if (mode == HTML) {
		char refname[MAXPATHLEN];

		linked = 1;
		close_files(inp_s, outp_s);
		if (outp_s != stdout) unlink(outfilename);

		line[strlen(line)-2] = 0;
		sprintf(refname, "%s%s", &line[3+strlen(symb_s)],
			&ext[mode][1]);
		errno = 0;
#ifndef _WIN32
		if (symlink(refname, outfilename)) perror("cdoc link");
#else
		/* Copy file. */
		{
		  char *buf;

		  buf = malloc((strlen("copy ") +
				strlen(refname) +
				strlen(outfilename) + 1)
			       * sizeof(char));

		  system(strcat(strcat(strcat(strcpy(buf, "copy "),
					      refname),
				       " "),
				outfilename));
		  free(buf);
		}
#endif
	      }
	    } else {
	      /* New manual page comment. */
	      comment=1; written = 1;

	      /* Expecting an empty line */
	      getline(line); lineNum++;
	      if(mode == TEXT) putline(TEXT, line, 1, 1, quote);
	      strip(line);

	      if (strlen(line) == 0) {
		/* Line was empty. */

		/* Expecting an underscore line. */
		getline(line);  lineNum++;
		if(mode == TEXT) putline(TEXT, line, 1, 1, quote);
		strip(line);
	      } else Warning(1, "Found no empty line at line %d.\n", lineNum);

	      if (strlen(line) > 0 && *line == '_') {
		/* Probably an underscore line. */

		/* Expecting an empty line. */
		getline(line);  lineNum++;
		if(mode == TEXT) putline(TEXT, line, 1, 1, quote);
		strip(line);
	      } else Warning(1, "Found no underscore line at line %d.\n",
			     lineNum);

	      if (strlen(line) == 0) {
		/* Line was empty. */

		/* Expecting a modulename line. */
		getline(line);  lineNum++;
		strip(line);
		if (mode == TEXT) fprintf(outp_s, newpage[mode]);
	      } else Warning(1, "Found no empty line at line %d.\n", lineNum);

	      if (mode == HTML) {
		char *s;

		fprintf(outp_s, "%s\n%s",
			"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2//EN\">",
			"<HTML>\n<HEAD>\n  <TITLE>");
		s = putline(mode, line, 1, 0, quote);
		fprintf(outp_s, "%s\n%s\n%s%s%s\n%s%s%s%s\n%s%s\n%s\n%s\n",
			"</TITLE>",
			"  <META NAME=\"generator\" CONTENT=\"XITE cdoc\">",
			"  <META NAME=\"keywords\" CONTENT=\"XITE ", s, "\">",
			"  <META NAME=\"description\"",
			" CONTENT=\"XITE Reference Manual - ", s, "\">",
			"  <LINK REV=\"made\"",
			" HREF=\"mailto:xite-bugs@ifi.uio.no\">",
			"  <LINK REL=\"up\" HREF=\"Contents.html\">",
			"</HEAD>");

		fprintf(outp_s, "\n%s\n\n",
			"<BODY BACKGROUND=\"../mona_canny.jpg\">");
	      }
	      fprintf(outp_s, "%s", subsection[mode]);
	      putline(mode, line, 1, 0, quote);
	      fprintf(outp_s, "%s", endSubsec[mode]);

	      if (mode == HTML) fprintf(outp_s, "%s\n", "<HR>");

	      empty = 1;

	      /* Expecting an underscore line */
	      getline(line);  lineNum++;
	      strip(line);
	      if (!(strlen(line) > 0 && *line == '_'))
		Warning(1, "Found no underscore line at line %d.\n", lineNum);

	    } /* New manual page comment. */
	  } /* New or link to manual page. */
	}/* Check for comment */
      } /* Non-empty line. */
      if (! comment) continue;
      
      /* ---- Process comment line ---- */
      
      if (mode == TEXT) {
	/* Text file output, don't format. */
	putline(mode, line, 1, 1, quote); continue;
      }

      if (line[0] == '_') continue; /* Ending underscore line. */

      /* ---- Header ---- */

      if (line[0] != ' ' && 
	  line[0] != (char) TAB_CHAR &&  
	  (lin = (char*) iindex(line, ':')  ) != NULL) {
	/* Found start of new section. */

	/* Terminate quoting and previous stuff. */
	if (quote)  fprintf(outp_s, endVerb[mode]);
	if (!empty) fprintf(outp_s, endList[mode]);
	if (items > 0) {
	  items = MIN(2, items);
	  while (items--) {
	    fprintf(outp_s, endDescr[mode]);
	    endIndent(mode);
	  }
	}
	*lin++ = 0; /* Remove ':' from section title. */
	fprintf(outp_s, subsubsec[mode]);

	subsub_header[num_subsub_header++] =
	  putline(mode, line, 2, 0, quote);

	fprintf(outp_s, endSubsubsec[mode]);
	empty = 1;
	quote = 0;

	/* If section is "See also:" and mode==HTML, then make hypertext
	 * links to the referred manual pages (indicated by trailing "(n)",
	 * where  1<=n<=8). Assume that the manual pages are in XITE.
	 */

	if (mode == HTML && strcmp(line, "See also") == 0) {
	  hypertextLink_s = 1;
	} else hypertextLink_s = 0;
      } else lin = line;
      
      /* ---- Text line ---- */
      
      while(is_space(*lin)) lin++;

      if (*lin == '&') {
	/* New item. */

	if (empty)  fprintf(outp_s, beginList[mode]);
	if (items < 1) fprintf(outp_s, beginDescr[mode]);
	if (quote)  fprintf(outp_s, endVerb[mode]);

	lin++;
	if (*lin == '&') {
	  if (items < 2) fprintf(outp_s, beginDescr[mode]);
	  items++;
	  lin++;
	} else {
	  if (items > 1) {
	    items = MIN(2, items);
	    while (items-- > 1) fprintf(outp_s, endDescr[mode]);
	  }
	  items = 1;
	}

	descrItem(mode,lin,0,1,quote);
	
	empty     = 0;
	quote     = 0;
	itembegin = 1;
      } else if (*lin == '-' && *(lin+1) == '&') {
	/* Items ended. */

	if (items > 0) {
	  fprintf(outp_s, endDescr[mode]);
	  endIndent(mode);
	}
	if (quote) fprintf(outp_s, endVerb[mode]);

	if (*(lin+2) == '&') {
	  lin +=3;
	  items = 1;
	} else {
	  lin += 2;
	  items = 0;
	}

	putline(mode, lin, 0, 1, quote);

	empty     = 0;
	quote     = 0;
	itembegin = 0;
      } else if (*lin == '|') {
	/* Copy output verbatim. */
	lin++;
	if (empty)     fprintf(outp_s, beginList[mode]);
	if (itembegin) fprintf(outp_s, newline[mode]);
	if (! quote)   fprintf(outp_s, beginVerb[mode]);

	if (mode == HTML) {
	  ptr = outbuf;

	  for (;;) {
	    if (iindex("<", *lin) != NULL) {
	      sprintf(ptr, "&lt;");
	      ptr += 4;
	    }
	    else if (iindex(">", *lin) != NULL) {
	      sprintf(ptr, "&gt;");
	      ptr += 4;
	    }
	    else if (iindex("&", *lin) != NULL) {
	      sprintf(ptr, "&amp;");
	      ptr += 5;
	    }
	    else if (iindex("\"", *lin) != NULL) {
	      sprintf(ptr, "&quot;");
	      ptr += 6;
	    }
	    else *ptr++ = *lin;

	    if (*lin++ == 0) break;
	  }

	  fprintf(outp_s, "%s\n", outbuf);
	}
	else if (mode != TROFF) fprintf(outp_s, "%s\n", lin);
	else {
	  char c, oldc = 0, *t;
	  
	  /* Replace space-delimited single backslash by double backslash. */
	  t = lin;
	  for(;;) { 
	    if ((c = *t) == 0) break;
	    
	    if (c != '\\') putc(c, outp_s);
	    else if (c == '\\' && oldc != '\\') {
	      putc(c, outp_s); putc(c, outp_s);
	    }
	    
	    oldc = c;
	    t++;
	  }
          putc('\n', outp_s);
        }
	
	indent_tab = items <= 1 ? items * INDENT_TAB : INDENT_TAB + 4;
	if (mode == TROFF && 7 + indent_tab + strlen(lin) > 65) {
	  Warning(1, "Manual page line (%d) will probably be broken.\n",
		  lineNum);
	}
	empty     = 0;
	quote     = 1;
	itembegin = 0;
	outp_line_num_s++;
      } else if(strlen(lin) > 0 && empty) {
	if (quote) fprintf(outp_s, endVerb[mode]);
	fprintf(outp_s, beginList[mode]);
	putline(mode, lin, 0, 1, quote);
	empty     = 0;
	quote     = 0;
	itembegin = 0;
      } else if (strlen(lin)) {
	if (quote) fprintf(outp_s, endVerb[mode]);
	putline(mode, lin, 0, 1, quote);
	empty     = 0;
	quote     = 0;
	itembegin = 0;
      } else {
	if (quote) fprintf(outp_s, endVerb[mode]);
	putline(mode, lin, 0, 1, quote);
	quote     = 0;
	itembegin = 0;
      }
    } /* Found new line. */
    
    if (written && mode == HTML) fprintf(outp_s, "\n</BODY>\n</HTML>\n");

    if (written) {
      /* Close file - rename temporary outputfile - exit */

      close_files(inp_s, outp_s);
    }

    if (!written && !linked && outp_s != stdout) unlink(outfilename);

    if (written && mode == HTML && outp_s != stdout) {
      char *tmpfilename, dir[100], buf[512];
      FILE *tmpfp;

      /*
      fprintf(stderr, "%s: outp_line_num: %d, num_headers: %d\n",
              outfilename, outp_line_num_s, num_subsub_header);
      */
      strcpy(dir, getenv("HOME"));
      tmpfilename = tempnam(dir, "/.doc");
      tmpfp       = fopen(tmpfilename, "w");
      outp_s      = fopen(outfilename, "r");

      add_html_toc(outp_s, subsub_header, num_subsub_header, tmpfp,
		   toc && outp_line_num_s > ntoc, 1);

      fclose(tmpfp);
      fclose(outp_s);

      unlink(outfilename);

      /* Copy from tmpfilename to outfilename. */

      tmpfp  = fopen(tmpfilename, "r");
      outp_s = fopen(outfilename, "w");

      while (fgets(buf, 512, tmpfp)) fputs(buf, outp_s);

      fclose(tmpfp); fclose(outp_s);
      unlink(tmpfilename);
    }
  } /* for symbnr */

#ifndef _WIN32
  (void) umask(umask_old);
#endif

  return(0);

} /* main() */

#endif /* MAIN */
