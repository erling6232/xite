

/*C*

________________________________________________________________

        getfiles
	$Id$
        Copyright 1994, Blab, UiO
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



/*F:open_files=getfiles*/
/*F:close_files=getfiles*/
/*F:getfiles*

________________________________________________________________

		getfiles
________________________________________________________________

Name:		getfiles, open_files, close_files  - open or close input and
                output files

Syntax:         | #include <xite/cdoc.h>
		|
		| int open_files( int argc, char **argv,
		|    FILE **inp, FILE **outp, char *definp,
		|    char *defoutp, int owerwrite,
		|    char **infilename, char **outfilename );
		|
                | void close_files( FILE* inp, FILE* outp );
		|

Description:    'open_files' reads program commandline 'argv' which is
                expected to have the form

                | cmd [ <option>... ] [ <infile> [ <outfile> ] ]
                
                'argc' and 'argv' are parameters to main. 'inp' and 'outp' are
		returned streams. 'definp' and 'defoutp' are wild card
		filenames with syntax <pre>*<post>. The filenames 'infilename'
		and 'outfilename' must match this expression. If they don''t
		match, <pre> or <post> is added. An empty string matches all
		filenames. 'owerwrite' is true if 'outfile' is allowed to
		owerwrite 'infile'.

                &0 file-names specified
                'inp' is set to standard input and 'outp' is set to standard
		output.

                &1 file-name specified  
                Make sure 'infile' has prefix and postfix given by 'definp'
		(add if necessary). Return 'inp' as a reference to this
		(possibly pre/appended) filename.

                'outp' is returned as a reference to a file with name given
		by the stem of 'infile' with prefix and postfix added from
		'defoutp'.
                
                &2 file-names specified
		Make sure 'infile' has prefix and postfix given by 'definp'
		and 'outfile' has prefix and postfix given by 'defoutp'.
		Adjust filenames if necessary.

                If (the possibly adjusted) filenames 'infile' and 'outfile'
		have the same name and 'owerwrite' is true, make a temporary
		output file. Rename this file to 'outfile' in 'close_files'.
		If 'owerwrite' is false, write a message and return an error.

                -&Open the files. If 'infile' does not exist or an
                'outfile' is not opened, write a message and return an error.

		'close_files' closes the files (if not stdin or stdout)
		and renames the temporary output file to 'outfile' (if
		'infile' and 'outfile' were the same).

Restrictions:   'option' must be processed by the 'attgetopt(3)' subroutine
                before 'open_files' is called.

Return value:   0 - Inp and outp are successfully opened.
                1 - Can not open input file.
                2 - Can not open output file.
                3 - Can not owerwrite input file.
                4 - Too many arguments.
                5 - Bad options.
                6 - Default does not contain a '*'

Author:		Otto Milvang
Id: 		$Id$

________________________________________________________________

*/

#include <xite/includes.h>
#include <xite/cdoc.h>     /* Must be included prior to <stdlib.h> */
#include <stdlib.h>
#include <xite/strings.h>
#include XITE_MKTEMP_H
#include XITE_STDIO_H
#include XITE_STRING_H
#include XITE_UNISTD_H

#ifndef MAIN

static char input_name_s[MAXPATHLEN], output_name_s[MAXPATHLEN],
  temp_name_s[MAXPATHLEN];



#ifndef FUNCPROTO
static void open_message(name,file,err)
char *name, *file;
int err;
#else /* FUNCPROTO */
static void open_message(char *name, char *file, int err)
#endif /* FUNCPROTO */
{
  switch(err) {
    case 0: break;
    case 1: fprintf(stderr, "%s: Can not open input-file: %s\n", name, file);
      break;
    case 2: fprintf(stderr, "%s: Can not open output-file: %s\n", name, file);
      break;
    case 3: fprintf(stderr, "%s: Can not owerwrite iutput-file %s\n",
		    name, file);
      break;
    case 4: fprintf(stderr,
		    "Usage: %s %s[ -flag ] [<input-file> [<output-file>]]\n",
		    name, file);
      break;
    case 5: fprintf(stderr, "%s: Bad options %s\n", name, file);
      break;
    case 6: fprintf(stderr, "%s: Default does not contain a '*'%s\n",
		    name, file);
      break;
  }
  exit(err);
}

#ifndef FUNCPROTO
static void match_length(template, left, right)
char *template;
int *left, *right;
#else /* FUNCPROTO */
static void match_length(char *template, int *left, int *right)
#endif /* FUNCPROTO */
{
  /* Return 'left' as number of characters to the left of '*',
   * 'right' as number of characters to the right of '*'.
   */

  char *pos;
  int len;

  *left = *right = 0;

  if((len = strlen(template))) {
    pos = iindex(template, '*');

    if(pos == NULL || pos != rrindex(template,'*'))
      open_message("Wrong use of openfiles:","",6);

    *left  = pos - template;
    *right = len - *left - 1;
  }

} /* match_length() */

#ifndef FUNCPROTO
static int filename_match(filename, template)
char *filename, *template;
#else /* FUNCPROTO */
static int filename_match(char *filename, char *template)
#endif /* FUNCPROTO */
{
  int llen, rlen;
  char tmpNam[80], *pos, *tmp;

  match_length(template, &llen, &rlen);

  if(llen && strncmp(filename, template, llen)) {
    /* 'filename' does not start with the left part of 'template'.
     * Prepend this part to 'filename'. */

    strncpy(tmpNam, template, llen);
    tmpNam[llen] = '\0';
    strcat(tmpNam, filename);
    strcpy(filename, tmpNam);
  }

  pos = filename + strlen(filename) - rlen;
  tmp = template + llen + 1;

  if(rlen && strcmp(pos,tmp)) {
    /* Add right part of 'template' to 'filename'. */

    strcat(filename,tmp);
  }

  return(0);

} /* filename_match() */

#ifndef FUNCPROTO
int open_files(argc, argv, inp, outp, definp, defoutp, owerwrite, infilename, outfilename)
int  argc;
char *argv[];
FILE **inp, **outp;
char *definp, *defoutp;
int  owerwrite;
char **infilename, **outfilename;
#else /* FUNCPROTO */
int open_files(int argc, char **argv, FILE **inp, FILE **outp, char *definp, char *defoutp, int owerwrite, char **infilename, char **outfilename)
#endif /* FUNCPROTO */
{
  int opt, err, args, temp_len1, temp_len2;
  
/* ===== Skip options ===== */

  err = 0;
  temp_name_s[0] = 0;
  if (infilename)  *infilename  = NULL;
  if (outfilename) *outfilename = NULL;
  if(att_optind == 1)
  while((opt = attgetopt(argc,argv,"")) != EOF) err=5;
  if(err) open_message(argv[0],"",err);

  args = argc-att_optind;

  switch(args)
    {
    case 0:
      *inp  = stdin;
      *outp = stdout;

      return(0);
      /* break; */

    case 1:
      /* Only one filename given. Assume that it denotes 'infile'. */

      if (!strcmp(argv[att_optind], "-") || !strcmp(argv[att_optind], "-0")) {
	*inp = stdin;
	return(0);
	/* break; */
      }

      strcpy(input_name_s, argv[att_optind]);
      /* Make sure 'input_name_s' has prefix and postfix given by 'definp'. */
      filename_match(input_name_s, definp);

      /* Find lengths of prefix and postfix in 'definp'. */
      match_length(definp, &temp_len1, &temp_len2);

      /* Copy the stem of 'input_name_s' to 'temp_name_s'. */
      strncpy(temp_name_s, input_name_s + temp_len1,
              strlen(input_name_s) - temp_len1 - temp_len2);
      temp_name_s[strlen(input_name_s) - temp_len1 - temp_len2] = '\0';

      /* Make sure 'output_name_s' consists of the stem from 'input_name_s'
       * with prefix and postfix from 'defoutp'. */
      strcpy(output_name_s, temp_name_s);
      filename_match(output_name_s, defoutp);

      *infilename  = input_name_s;
      *outfilename = output_name_s;
      break;

    case 2:
      if (!strcmp(argv[att_optind], "-") || !strcmp(argv[att_optind], "-0")) {
	*inp = stdin;
      } else {
	strcpy(input_name_s, argv[att_optind]);
	filename_match(input_name_s, definp);
      }
      *infilename = input_name_s;

      if (!strcmp(argv[att_optind+1], "-") ||
	  !strcmp(argv[att_optind+1], "-1")) {
	*outp = stdout;
	return(0);
	/* break; */
      } else {
	strcpy(output_name_s, argv[att_optind+1]);
	filename_match(output_name_s, defoutp);
	*outfilename = output_name_s;
      }
      break;

    default:
      open_message(argv[0], "", 4);
      break;
  };

  if(*inp != stdin && (access(input_name_s, 4)) != 0)
    open_message(argv[0], input_name_s, 1);

  if (*inp != stdin && *outp != stdout &&
      !strcmp(input_name_s, output_name_s)) {
    /* Use temporary output file. Remember the correct output filename
     * so that 'close_files' can rename the output file. */

    if(!owerwrite) open_message(argv[0], "", 3);
    if(access(output_name_s, 2) != 0) open_message(argv[0], output_name_s, 2);
    strcpy(temp_name_s, output_name_s);
    strcpy(output_name_s, getenv("HOME"));
    strcat(output_name_s, "/.");
    strcat(output_name_s, argv[0]);
    strcat(output_name_s, "-XXXXXX");
    mktemp(output_name_s);
    *outfilename = output_name_s;
  } else if (*inp != stdin && *outp != stdout) {
    strcpy(temp_name_s, "");
  }

/* ===== open files ====== */

  if (*inp  != stdin)  *inp = fopen(input_name_s, "r");
  if (*inp  == NULL)   open_message(argv[0], input_name_s, 1);
  if (*outp != stdout) *outp = fopen(output_name_s, "w");
  if (*outp == NULL) {
    fclose(*inp);
    open_message(argv[0], output_name_s, 2);
  }

  return(0);

} /* open_files() */

#ifndef FUNCPROTO
void close_files(inp, outp)
FILE *inp, *outp;
#else /* FUNCPROTO */
void close_files(FILE *inp, FILE *outp)
#endif /* FUNCPROTO */
{
  /* Close the files and rename the outputfile if it was temporary. */

  if (inp  != stdin)  fclose(inp);
  if (outp != stdout) fclose(outp);

  if(strlen(temp_name_s)) {
    /* Rename the file 'output_name_s' to 'temp_name_s'. */

    unlink(temp_name_s);
#ifndef _WIN32
    link(output_name_s, temp_name_s);
#else
    {
      char *buf;

      buf = malloc((strlen("copy ") +
		    strlen(output_name_s) +
		    strlen(temp_name_s) + 1)
		   * sizeof(char));

      system(strcat(strcat(strcat(strcpy(buf, "copy "),
				  output_name_s),
			   " "),
		    temp_name_s));
      free(buf);
  }
#endif
    unlink(output_name_s);
  }

  return;

} /* close_files() */

#endif /* not MAIN */
