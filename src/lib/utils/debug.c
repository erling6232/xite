

/*C*

________________________________________________________________

        debug.c
        debug.c,v 1.2 1995/08/23 14:58:38 svein Exp
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

static char *Id = "debug.c,v 1.2 1995/08/23 14:58:38 svein Exp, Blab, UiO";


#include <xite/includes.h>
#include XITE_MALLOC_H
#include XITE_STDIO_H

#ifndef DEBUG
# define DEBUG
#endif
#include <xite/debug.h> /* To get cross-checking of prototypes. */

#define ENTER_TEXT " entered."
#define LEAVE_TEXT " leaving."
#define FUNCTION_SKIP 2

static int debug_blanks = -FUNCTION_SKIP;



/*F:prepend_debug_blanks=enter_function_debug*/
/*F:leave_function_debug=enter_function_debug*/
/*F:enter_function_debug*

________________________________________________________________

		enter_function_debug
________________________________________________________________

Name:		enter_function_debug, leave_function_debug,
                prepend_debug_blanks - Handle debug printouts in XITE
Syntax:         | #include <xite/debug.h>
		| 
                | char *prepend_debug_blanks( char* t );
                | void enter_function_debug( char* func_name );
                | void leave_function_debug( char* func_name );
Description:	
Author:		Svein Bøe, Ifi, UiO
Id: 		debug.c,v 1.2 1995/08/23 14:58:38 svein Exp
________________________________________________________________

*/

char *prepend_debug_blanks(char *t)
{
  char *tt;

  if (debug_blanks < 0) {
    fprintf(stderr, "prepend_debug_blanks: non-positive margin.\n");
    exit(1);
  }
  tt = (char *) malloc(debug_blanks + strlen(t) + 1);

  if (debug_blanks > 0) sprintf(tt, "%*c%s", debug_blanks, ' ', t);
  else sprintf(tt, "%s", t);

  return(tt);

} /* prepend_debug_blanks() */

void enter_function_debug(char *func_name)
{
  char *t, *tt;

  debug_blanks += FUNCTION_SKIP;
  t = (char *) malloc(strlen(func_name) + strlen(ENTER_TEXT) + 2);
  sprintf(t, "%s%s\n", func_name, ENTER_TEXT);

  tt = prepend_debug_blanks(t);
  free(t);

  fprintf(stderr, tt);
  free(tt);

  return;

} /* enter_function_debug() */

void leave_function_debug(char *func_name)
{
  char *t, *tt;

  t = (char *) malloc(strlen(func_name) + strlen(LEAVE_TEXT) + 2);
  sprintf(t, "%s%s\n", func_name, LEAVE_TEXT);

  tt = prepend_debug_blanks(t);
  free(t);

  fprintf(stderr, tt);
  free(tt);

  debug_blanks -= FUNCTION_SKIP;

  return;

} /* leave_function_debug() */


