/****************************************************************************

	RegExp.h

	This file contains the C definitions and declarations for
	the regular expression matching code.

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

#ifndef _FWF_REGEXP_H_
#define	_FWF_REGEXP_H_

#include <xite/includes.h>
#include <X11/Xfuncproto.h>
#ifdef REGEX
# include XITE_TYPES_H
# include <regex.h>
#endif /* REGEX */
#include XITE_STDIO_H

#if (!NeedFunctionPrototypes)

void	RegExpCompile();
int	RegExpMatch();
void	_RegExpError();
void	RegExpPatternToRegExp();

#else

#ifdef REGEX
void    RegExpCompile(char *regexp, regex_t *fsm_ptr, int fsm_length);
int     RegExpMatch(char *string, regex_t *fsm_ptr);
void	_RegExpError(int val);
void	RegExpPatternToRegExp(char *pattern, char *reg_exp);
#else /* REGEX */
void	RegExpCompile(char *regexp, char *fsm_ptr, int fsm_length);
int	RegExpMatch(char *string, char *fsm_ptr);
void	_RegExpError(int val);
void	RegExpPatternToRegExp(char *pattern, char *reg_exp);
#endif /* REGEX */

#endif /* NeedFunctionPrototypes */

#ifndef TRUE
#define TRUE				1
#endif

#ifndef FALSE
#define	FALSE				0
#endif

#ifndef REGEX

#define	INIT		register char *sp = instring;
#define	GETC()		(*sp++)
#define	PEEKC()		(*sp)
#define	UNGETC(c)	-- sp
#define	RETURN(ptr)	return;
#define	ERROR(val)	_RegExpError(val)

#endif /* !REGEX */

#endif /* _FWF_REGEXP_H_ */
