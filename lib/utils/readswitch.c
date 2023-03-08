

/*

________________________________________________________________

        readswitch.c
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



/*F:read_bswitch=read_switch*/
/*F:read_iswitch=read_switch*/
/*F:read_dswitch=read_switch*/
/*F:read_switch*

________________________________________________________________

		read_switch
________________________________________________________________

Name:		read_switch, read_bswitch, read_iswitch, read_dswitch -
                read switches from command line

Syntax:         | #include <xite/readarg.h>
                |
                | char *read_switch( int* argc, char** argv,
                |    char* name, int args, char* defreturn );
		|
                | int read_bswitch( int* argc, char** argv,
                |    char* name );
		|
                | int read_iswitch( int* argc, char** argv,
                |    char* name, int defval );
		|
                | double read_dswitch( int* argc, char** argv,
                |    char* name, double defval );
		|
Description:	'read_switch' reads a switch/option from the command line. The 
                routine searches for the switch 'name'. If 'name' is found 
		and args is zero, a pointer to the switch is returned.
		If 'name' is found and 'args' is nonzero, a string
		containing the switch argument(s) is returned.
		Otherwise, if 'name' does not occur in the
		command line, 'defreturn' is returned.

		'read_bswitch' returns TRUE if 'name' is a switch 
		in the command line, FALSE otherwise.

		'read_iswitch' searches for switch 'name' and returns
		its argument (converted to integer), otherwise it
		returns 'defval'.

		'read_dswitch' searches for switch 'name' and returns
		its argument (converted to double), otherwise it
		returns 'defval'.

		The switch and its argument are removed from
		'argv', and 'argc' is decremented.
Examples:	| #include <math.h>
		| #include <xite/readarg.h>
		| char *res, *fn;
                | double d;
		| int i, j, l;
                | d = atof(read_switch(&argc, argv, "-scale", 1, "0.5"));
                | sscanf(read_switch(&argc, argv, "-offset", 2, "128 128"),
		|        "%d%d", &i, &j);
                | l = read_switch(&argc, argv, "-log", 0, NULL) != NULL;
                | res = read_switch(&argc, argv, "-yes", 0, "-no");
		| fn = argv[1];
		|
		| or:
		|
		| d = read_dswitch(&argc, argv, "-scale", 0.5);
		| sscanf(read_switch(&argc, argv, "-offset", 2, "128 128"),
		|        "%d%d", &i, &j);
                | l = read_bswitch(&argc, argv, "-log");
                | res = read_switch(&argc, argv, "-yes", 0, "-no");
		| fn = argv[1];
		|
		| commandline: prog -offset 3 3 -scale 2 -log filename
		| d          : 2.0
		| i          : 3
		| j          : 3
		| l          : TRUE
		| res        : "-no"
		| fn         : "filename"
Author:		Otto Milvang
Id:             $Id$
________________________________________________________________

*/

#include <math.h>
#include <stdlib.h>
#include <xite/includes.h>
#include <xite/readarg.h>
#include <xite/strings.h>
#include XITE_STDIO_H
#include XITE_STRING_H
#include XITE_STRTOL_H
#include XITE_MALLOC_H

char *read_switch(int *argc, char **argv, char *name, int args, char *defreturn)
{
  int i, argn;
  char *ret;
  for(i=1; i<*argc; i++)
    {
      if (strcmp(name, argv[i]) == 0)
	{
	  argn = i+1;
	  if (i + args >= *argc)
	    {
	      fprintf(stderr,"read_switch: Missing argument after \"%s\"\n",
		      argv[i]);
	      exit(2);
	    }
	  ret = argv[args ? i+1 : i];
	  argn = i + 1 + args;
	  while(argn < *argc)
            argv[i++] = argv[argn++];
	  *argc = *argc - argn + i;
	  if (args-- <=1) return(ret);
	  while(args--) ret[strlen(ret)] = ' ';
	  return(ret);
	}
    }
  return(defreturn);
}

int read_bswitch(int *argc, char **argv, char *name)
{
  return(read_switch(argc, argv, name, 0, NULL) != NULL);
}

int read_iswitch(int *argc, char **argv, char *name, int defval)
{
  char *res, *ptr;
  int val;
  res = read_switch(argc, argv, name, 1, NULL);
  if (res == NULL) return(defval);
  val = strtol(res, &ptr, 0);
  if (*ptr)
    {
      fprintf(stderr, "read_iswitch: Integer argument to \"%s\" expected\n",
	      name);
      exit(2);
    }
  return(val);
}

double read_dswitch(int *argc, char **argv, char *name, double defval)
{
  char *res, *ptr;
  double val;
  res = read_switch(argc, argv, name, 1, NULL);
  if (res == NULL) return(defval);
  val = strtod(res, &ptr);
  if (*ptr)
    {
      fprintf(stderr, "read_dswitch: Numeric argument to \"%s\" expected\n",
	      name);
      exit(2);
    }
  return(val);
}



/*F:argvOptions*

________________________________________________________________

		argvOptions
________________________________________________________________

Name:		argvOptions - Concatenate all the command-line options and
                arguments in one string.
Syntax:         | #include <xite/readarg.h>
		| 
                | char *argvOptions( int argc, char** argv );
Description:	Return a string consisting of all the options in the argv
                string array, i. e. all the string elements argv[i],
		0 < i < argc, separated by spaces.
		May typically be used as the third argument of the
		'Ihistory(5)' routine.
See also:	'Ihistory(5)'
Return value:	Pointer to the string, or NULL if argc == 1.
Author:		Svein Bøe, Ifi, UiO
Id: 		$Id$
________________________________________________________________

*/

char *argvOptions(int argc, char **argv)
{
  char *options;
  int num = 0, b;

  /* Concatenize options in the 'argv' string array. */

  if (argc == 1) return(NULL);

  for (b = 1; b < argc; b++) {
    num += strlen(argv[b]) + 1; /* +1 for space between each option. */
  }
  /* No need to increase 'num' by one for the terminating null character,
     because the space in front of the first option is not needed (it is added
     by Ihistory). */
  
  if (num == 0) return(NULL);

  options = (char *) malloc((size_t) num);
  options[0] = '\0';
  for (b = 1; b < argc; b++) {
    (void) strcat(options, argv[b]);
    if (b != argc - 1) (void) strcat(options, " ");
  }
  
  return(options);

} /* argvOptions */




/*F:split_cmd_line*

________________________________________________________________

		split_cmd_line
________________________________________________________________

Name:		split_cmd_line - split command line into words

Syntax:		| #include <xite/readarg.h>
		| 
		| void split_cmd_line( char *cmd_line, char **vec );
		|
Description:	Split 'cmd_line' into words. Words are separated by
                blanks. A list of words surrounded by a pair of quotes (")
		is interpreted as one word.

		'cmd_line' will be changed on return, in that the word
		delimiters are replaced by null-characters.

		The words are returned in 'vec' which is terminated
		by a NULL-element.

Restrictions:	'vec' must refer to already allocated memory.

See also:	read_switch(3)
Author:		Svein Bøe, Ifi, UiO
Id: 		$Id$
________________________________________________________________

*/

void split_cmd_line(char *cmd_line, char **vec)
{
  char *ptr;
  int in_string = 0, vnr = 0;

  ptr = cmd_line;

  while (*ptr) {

    ptr = nextNonSpace(ptr); /* No allocation. */
    if (*ptr) {
      /* Not yet end of command-line, new word. */

      if (*ptr == '\"' && !in_string) { /* " (help emacs highlights) */
        /* Start of multi-word string. */
        in_string = 1;

        vec[vnr++] = ++ptr; /* Skip '"' */
      } else if (!in_string) vec[vnr++] = ptr;

    } else {
      /* No more words in command-line, terminate the vector. */
      vec[vnr++] = NULL;
    }

    ptr = nextSpace(ptr);
    if (in_string && ptr > cmd_line && *(ptr - 1) == '\"') { /* " */
      /* End of multi-word string. */

      in_string = 0;
      *(ptr - 1) = '\0'; /* Remove '"' */
      if (*ptr != '\0') ptr++;
    } else if (!in_string && *ptr) {
      /* End of simple string. */
      *ptr++ = '\0';
    }

  } /* while (*ptr) */

  vec[vnr] = NULL;

  return;

} /* split_cmd_line() */

#ifdef MAIN

main(int argc, char **argv)
{
  int i, j;
  char *res;
  res = read_switch(&argc, argv, "-scale",1,"0.5");
  fprintf(stderr,"-scale = |%s|\n", res);
  sscanf(read_switch(&argc, argv, "-offset2", 2, "128 128"), "%d%d", &i, &j);
  res = read_switch(&argc, argv, "-offset",2,"128 128");
  fprintf(stderr,"-offset = |%s| %d %d \n", res, i, j);
  res = read_switch(&argc, argv, "-log",0, NULL);
  fprintf(stderr,"-log = |%s|\n", res);
  res = read_switch(&argc, argv, "-yes",0,"-no");
  fprintf(stderr,"-yes = |%s|\n", res);
  fprintf(stderr, "-d = %10.2f\n", read_dswitch(&argc, argv, "-d", 30.0)); 
  fprintf(stderr, "-i = %10d\n", read_iswitch(&argc, argv, "-i", 20));
  fprintf(stderr, "-b = %10d\n", read_bswitch(&argc, argv, "-b"));
 
  fprintf(stderr, "argc = %d\n", argc);
  return(0);
}

#endif

