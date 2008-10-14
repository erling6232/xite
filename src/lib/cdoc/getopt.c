/*
**	@(#)getopt.c	2.5 (smail) 9/15/87
*/

/*
 * Here's something you've all been waiting for:  the AT&T public domain
 * source for getopt(3).  It is the code which was given out at the 1985
 * UNIFORUM conference in Dallas.  I obtained it by electronic mail
 * directly from AT&T.  The people there assure me that it is indeed
 * in the public domain.
 * 
 * There is no manual page.  That is because the one they gave out at
 * UNIFORUM was slightly different from the current System V Release 2
 * manual page.  The difference apparently involved a note about the
 * famous rules 5 and 6, recommending using white space between an option
 * and its first argument, and not grouping options that have arguments.
 * Getopt itself is currently lenient about both of these things White
 * space is allowed, but not mandatory, and the last option in a group can
 * have an argument.  That particular version of the man page evidently
 * has no official existence, and my source at AT&T did not send a copy.
 * The current SVR2 man page reflects the actual behavor of this getopt.
 * However, I am not about to post a copy of anything licensed by AT&T.
 */

/* Changed by Otto Milvang/Tor Lønnestad and Svein Bøe to fit XITE */

#include <xite/includes.h>
#include <xite/cdoc.h>
#include <xite/strings.h>
#ifdef HAVE_STRINGS_H
 #include <strings.h>
#else
 #ifdef HAVE_STRING_H
   #include <string.h>
  #endif
#endif
#ifdef HAVE_SYS_TYPES_H    /* For ptrdiff_t. */
#  include <sys/types.h>
#endif

#ifndef NULL
# define NULL	0
#endif
#define EOF	(-1)
#ifdef HAVE_UNISTD_H        /* To get declaration of write() */
#  include <unistd.h>
#endif
#define ERR(s, c)	if(att_opterr){\
	char errbuf[2];\
	errbuf[0] = c; errbuf[1] = '\n';\
	(void) write(2, argv[0], (unsigned)strlen(argv[0]));\
	(void) write(2, s, (unsigned)strlen(s));\
	(void) write(2, errbuf, 2);}

int	att_opterr = 1;
int	att_optind = 1;
int	att_optopt;
char	*att_optarg;



/*F:getopt=attgetopt*/
/*F:attgetopt*

________________________________________________________________

		attgetopt
________________________________________________________________

Name:		attgetopt, getopt - Get options
Syntax:         | #include <xite/cdoc.h>
                | int attgetopt( int argc, char** argv,
                |    char* opts );
Description:    The name of this routine is attgetopt, to eliminate conflicts
                with the system supplied getopt routine.

		White space between an option and its first argument is
                allowed, but not mandatory, and the last option in a group can
		have an argument. See the manual page for getopt, outside
		of the XITE environment.
Return value:
Author:		AT&T
________________________________________________________________

*/

int attgetopt(int argc, char **argv, char *opts)
{
  static int sp = 1;
  int c;
  char *cp;

  if(sp == 1) {
    if(att_optind >= argc ||
       argv[att_optind][0] != '-' || argv[att_optind][1] == '\0'
       || (argv[att_optind][1] >= '0' && argv[att_optind][1] <= '9')) {
      return(EOF);
    } else if(strcmp(argv[att_optind], "--") == (ptrdiff_t) NULL) {
      att_optind++;
      return(EOF);
    }
  }
  att_optopt = c = argv[att_optind][sp];
  if(c == ':' || (cp=iindex(opts, c)) == NULL) {
    ERR(": illegal option -- ", c);
    if(argv[att_optind][++sp] == '\0') {
      att_optind++;
      sp = 1;
    }
    return('?');
  }
  if(*++cp == ':') {
    if(argv[att_optind][sp+1] != '\0')
      att_optarg = &argv[att_optind++][sp+1];
    else if(++att_optind >= argc) {
      ERR(": option requires an argument -- ", c);
      sp = 1;
      return('?');
    } else
      att_optarg = argv[att_optind++];
    sp = 1;
  } else {
    if(argv[att_optind][++sp] == '\0') {
      sp = 1;
      att_optind++;
    }
    att_optarg = NULL;
  }
  return(c);
}
