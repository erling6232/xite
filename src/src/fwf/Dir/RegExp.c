/****************************************************************************

	RegExp.c

	This file contains the C code for the regular expression
	matching code.

	The routines supported act as a more friendly, user level
	interface to the regexp regular expression matching system.

 ****************************************************************************/

/*
 * Author:
 * 	Brian Totty
 * 	Department of Computer Science
 * 	University Of Illinois at Urbana-Champaign
 *	1304 West Springfield Avenue
 * 	Urbana, IL 61801
 * 
 * 	totty@cs.uiuc.edu
 * 	
 */ 

#include <xite/includes.h>
#include <stdlib.h>
#include <Xfwf/RegExp.h>
#ifdef REGEX
# include XITE_TYPES_H
# include <regex.h>
#else /* REGEX */
# include <regexp.h>
#endif /* !REGEX */
#include <xite/message.h>
#include XITE_STDIO_H

#ifndef FUNCPROTO
# ifdef REGEX
void RegExpCompile(regexp, fsm_ptr, fsm_length)
char *regexp;
regex_t *fsm_ptr;
int fsm_length;
# else /* REGEX */
void RegExpCompile(regexp, fsm_ptr, fsm_length)
char *regexp, *fsm_ptr;
int fsm_length;
# endif /* REGEX */
#else /* FUNCPROTO */
# ifdef REGEX
void RegExpCompile(char *regexp, regex_t *fsm_ptr, int fsm_length)
# else /* REGEX */
void RegExpCompile(char *regexp, char *fsm_ptr, int fsm_length)
# endif /* REGEX */
#endif /* FUNCPROTO */
{
#ifndef NO_REGEXP
# ifdef REGEX
        int status = 0, nbytes = 0;
	char *err_string = NULL;

        status = regcomp(fsm_ptr, regexp, REG_NOSUB);
	if (status) {
	  nbytes = regerror(status,  fsm_ptr,  NULL, 0);
	  err_string = (char *) malloc(nbytes + 1);
	  (void) regerror(status,  fsm_ptr,  err_string, nbytes + 1);
	  Warning(1, "%s\n", err_string);
	}
# else
	compile(regexp, fsm_ptr, &(fsm_ptr[fsm_length]), '\0');
# endif
#endif
} /* End RegExpCompile */


#ifndef FUNCPROTO
# ifdef REGEX
int RegExpMatch(string, fsm_ptr)
char *string;
regex_t *fsm_ptr;
# else /* REGEX */
int RegExpMatch(string, fsm_ptr)
char *string, *fsm_ptr;
# endif /* REGEX */
#else /* FUNCPROTO */
# ifdef REGEX
int RegExpMatch(char *string, regex_t *fsm_ptr)
# else /* REGEX */
int RegExpMatch(char *string, char *fsm_ptr)
# endif /* REGEX */
#endif /* FUNCPROTO */
{
#ifndef NO_REGEXP
# ifdef REGEX
        int status = 0, nbytes = 0;
	char *err_string = NULL;

        status = regexec(fsm_ptr, string, (size_t) 0, NULL, 0);

	if (!status) return(TRUE);
	else if (status == REG_NOMATCH) return(FALSE);
	else {
	  nbytes = regerror(status,  fsm_ptr,  NULL, 0);
	  err_string = (char *) malloc(nbytes + 1);
	  (void) regerror(status,  fsm_ptr,  err_string, nbytes + 1);
	  Warning(1, "%s\n", err_string);
	  return(FALSE);
	}
# else /* REGEX */
	if (advance(string, fsm_ptr) != 0)
		return(TRUE);
	    else
		return(FALSE);
# endif /* REGEX */
#else
	return(TRUE);
#endif
} /* End RegExpMatch */


#ifndef FUNCPROTO
void _RegExpError(val)
int val;
#else /* FUNCPROTO */
void _RegExpError(int val)
#endif /* FUNCPROTO */
{
	fprintf(stderr,"Regular Expression Error %d\n",val);
	exit(-1);
} /* End _RegExpError */


#ifndef FUNCPROTO
void RegExpPatternToRegExp(pattern,reg_exp)
char *pattern,*reg_exp;
#else /* FUNCPROTO */
void RegExpPatternToRegExp(char *pattern, char *reg_exp)
#endif /* FUNCPROTO */
{
	int in_bracket;

	in_bracket = 0;
	while (*pattern != '\0')
	{
		if (in_bracket)
		{
			if (*pattern == ']') in_bracket = 0;
			*reg_exp++ = *pattern++;
		}
		    else
		{
			switch (*pattern)
			{
			    case '[':
				in_bracket = 1;
				*reg_exp++ = '[';
				break;
			    case '?':
				*reg_exp++ = '.';
				break;
			    case '*':
				*reg_exp++ = '.';
				*reg_exp++ = '*';
				break;
			    case '.':
				*reg_exp++ = '\\';
				*reg_exp++ = '.';
				break;
			    default:
				*reg_exp++ = *pattern;
				break;
			}
			++ pattern;
		}
	}
	*reg_exp++ = '$';
	*reg_exp++ = '\0';
} /* End RegExpPatternToRegExp */
