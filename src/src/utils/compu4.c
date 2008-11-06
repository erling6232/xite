

/*C*

________________________________________________________________

        compu4
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

/*F:compute_line=compute*/
/*F:compute_answer=compute*/
/*F:compute_define=compute*/
/*F:compute_error=compute*/
/*F:compute*

________________________________________________________________

		compute
________________________________________________________________

Name:           compute, compute_line, compute_answer, compute_define,
                compute_error - Simple calculator.

Syntax:         | #include <xite/compu4.h>
		|
                | int compute_line( char* inp_line,  ...);
		|
                | number *compute_answer( void  );
		|
                | char *compute_error( int message );
		|
                | int compute_define( char* name, func action );
Description:    Compute the value of an expression.
		The value can be of all types. The return value
		gives the type of the answer.

		| #define TY_UN -1  * Unknown, Error  in i_value  *
		| #define TY_BF 0   * Boolean, false
		| #define TY_BT 1   * Boolean, true
		| #define TY_CI 2   * int,     Answer in i_value  *
		| #define TY_CD 3   * double,  Answer in d_value  *
		| #define TY_CS 4   * *char,   Answer in p_value  *
		| #define TY_BO 5   * Boolean, Answer in i_value  *
		| #define TY_CP 6   * Predefined constant         *
		| #define TY_VR 7   * Variable                    *


		Answer is a 'number' record containing the result
                of the expression. Number is defined as:

                | typedef struct u_tag
                | {
                |   union
		|   {
		|     INTEG i_value;
                |     double d_value;
                |     char *p_value
		|   } u_value
                |   int u_type;
		| } number;


                Line is the string which is evaluated. The line is an
                arithmetic and/or a boolean expression. Special symbols
                introduced by the character % is converted to a string,
                or a constant from the argument list. The grammar is

                | S -> $
                | S -> E$
                | E -> un E
                | E -> E op E
		| E -> (E)
		| E -> id = E
                |
                | $ -  End of line
                | E -  Input constant.
		|        %S - The first argument in the list is
		|             replaced as a character string.
		|        %s - The next (or second) argument in the list is
		|             replaced as a character str ing.
		|        %L - The first argument in the list is
		|             replaced as a lowercase character string.
		|        %l - The next (or second) argument in the list is
		|             replaced as a lowercase character string.
		|        %U - The first argument in the list is
		|             replaced as a uppercase character string.
		|        %u - The next (or second) argument in the list is
		|             replaced as a uppercase character string.
                |        %D - The first argument in the list is
                |             replaced as an integer.
                |        %d - The next (or second) argument in the list is
                |             replaced as an integer.
                |        %F - The first argument in the list is
                |             replaced as a double precision.
                |        %f - The next (or second) argument in the list is
                |             replaced as a double precision.
                |      Predefined Constant.
                |        INTEG  - All integer strings and all integers
                |        REAL   - All floating-point strings and reals
                |        FILE   - All strings = name of existing file.
                |        EXE    - All strings = name of executable file.
                |        WRITE  - All strings = name of file with write
		|                 access.
                |        READ   - All strings = name of file with read
		|                 access.
		|        usrdef - Define your own constant
		|      Result from: un E, E op E, or (E)
                | un - [+,-]     int, float or string,
		|                String is converted to INTEG or float.
                |      [NOT]     Boolean
                | op - [*,/]     int, float or string.
		|                String is converted to INTEG or float.
		|                INTEG op float -> int.
		|                float op INTEG -> float.
                | op - [+,-]     int, float or string.
		|                String is converted to INTEG or float.
		|                 INTEG op float -> int.
		|                float op INTEG -> float.
                | op - [LT,<,LE,<=,EQ,==,NE,!=,GE,>=,GT,>]
		|                String is converted to INTEG or float.
		|                INTEG op float -> INTEG op INTEG ->
		|                Boolean.
		|                float op INTEG -> float op float ->
		|                Boolean.
                | op   [IS]      True is first argument match the regular
		|                expresion in arg2. All Predefined
		|                constants are valid regular expresions.
                | op - [AND]     Bool op Bool -> Bool. Second argument
                |                is not computed if first arg is FALSE.
                | op - [OR]      Bool op Bool -> Bool. Second argument
                |                is not computed if first arg is TRUE.
		| () -           Operators abow are listed with decreasing
		|                precedence.
		| =              Define variables. Defined variables may
		|                be used in other expressions.
		|                A = 4
		|                B = 2*A

Diagnostics:    The diagnostic code is listed below.

Return value:   If the evaluation of the expression was sucsessful the
		function returns a non-negative result.
		If the evaluation fails 'compute_error'  will return
		| E_TOK - Undefined token.
		| E_ARG - Missing argument.
		| E_OP  - Missing operand.
		| E_LP  - Too many )'s.
		| E_EOL - Unexpected end of line.
		| E_RP  - Expected )
		| E_TYP - Wrong arg type for op
		| E_LEN - Too long input line
		| E_EQ  - Variable expexted

Author:         Otto Milvang
Examples:
Id:             $Id$

---------------------------------------------------------------	*/

/*
---------------------------------------------------------------

       	compute - include files

---------------------------------------------------------------
*/

#include <ctype.h>
#include <errno.h>
#include <xite/includes.h>
#include <xite/compu4.h>
#ifdef HAVE_VARARGS_H
# include <varargs.h>
#else
# ifdef HAVE_STDARG_H
#  include <stdarg.h>
# endif
#endif
#include <stdlib.h>
#include "compu4_L.h"
#ifdef HAVE_FCNTL_H
# include <fcntl.h>
#endif
#ifdef HAVE_SYS_FILE_H
# include <sys/file.h>
#else
# ifdef HAVE_SYS_IO_H
#  include <sys/io.h>
# endif
#endif
#ifdef HAVE_MALLOC_H
# include <malloc.h>
#endif
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#ifdef HAVE_STRINGS_H
# include <strings.h>
#else
# ifdef HAVE_STRING_H
#  include <string.h>
# endif
#endif
#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif
#ifdef HAVE_CTYPE_H
# include <ctype.h>
#endif
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif


static char empty[] = "";
static number *global_answer = NULL;
static int global_error = E_OK;


/*
---------------------------------------------------------------

       	compute - typedef

        number  - is a general representation for a number
                  or string. First element represent the
		  element type. The second represent the value.
                  The type code is:

                  TY_UN - Unknown
                  TY_CI - int,         value in i_value.
                  TY_CD - double,      value in d_value.
                  TY_CS - *char,       value in p_value.
                  TY_BO - Boolean,     value in i_value.
                  TY_CP - Predefined,  value in i_value.
		  TY_CF - float,       value in f_value.
		  TY_VR - variable,    pointer in p_value.

        state   - is an internal type representing the parser
                  state and the assosiated  token.

---------------------------------------------------------------
*/
#define new_int(val)    new_token(TY_CI,(INTEG)(val),0.0,0);
#define new_double(val) new_token(TY_CD,INT_0,val,0);
#define new_string(val) new_token(TY_CS,INT_0,0.0,val);
#define new_bool(val)   new_token(TY_BO,(INTEG)(val),0.0,0);
#define new_pre(val)    new_token(TY_CP,(INTEG)(val),0.0,0);
#define new_var(val)    new_token(TY_VR,(INTEG)(val),0.0,0);
#ifdef DEBUG
#define DUMP(t,e)      printf("%s=%d\n",t,e)
#else
#define DUMP(t,e)
#endif
typedef struct u_state
{
  int pst;    /* Parser state   */
  int tok;    /* Token          */
} state;


/*
---------------------------------------------------------------


	compute - externals

---------------------------------------------------------------
*/
static void init_compute(int);
static int tok_type(int);
static int token_int(char*, char**, INTEG*, char*);
static int token_real(char *, char**, double*, char*);
static int token_op(char*, char**);
static int token_tok(char*, char**);
static int token_st(char*, char**, char**, char);
static int token_arg(char*, char**);
static int new_token(int, INTEG, double, char*);
static void next_token(int*, char**);
static void c_convert(number*);
static void d_convert(number*, number*);
static int redi1(number*, int);
static int redf1(number*, int);
static int redb1(number*, int);
static int redc1(number*, int);
static int red1(number*, int);
static int redb2(number*, number*, int);
static int redcp2(number*, number*, int);
static int redcc2(number*, number*, int);
static int redia2(number*, number*, int);
static int redfa2(number*, number*, int);
static int redir2(number*, number*, int);
static int redfr2(number*, number*, int);
static int red2(number*, number*, int);
static int next_action(int);
static void shift_symbol(int, int);
static int reduce_symbol(int);
static int parse(int);
static int compute_variable(char *, int);


/*
---------------------------------------------------------------

       	compute - global data structure

        state_stack - A stack of tokens and parse state
                      informations.
	stats       - Top of stack.

        arg_stack   - A stack of numbers.
        arg         - Top of stack.
        pre         - Last used predefined argument.

        line        - A local string buffer.
        line_ptr    - Pointer to first free character.

        skip        - Set TRUE if the first argument of
                      ARG1 OR  ARG2  is TRUE or
                      ARG1 AND ARG2  is FALSE.

---------------------------------------------------------------
*/

#define STACKLEN 32
static state state_stack[STACKLEN];
static int stats;
static number arg_stack[STACKLEN];
static int arg;
static int pre;
static char line[1024];
static char *line_ptr;
static int  skip;

#define TOK_USER 1200
#define MAX_USER 8
static funcarr user_def;
static char *user_tok[MAX_USER];
static int user_ptr = TOK_USER;

#define TOK_VAR 1600
#define MAX_VAR 64
static number *var_def[MAX_VAR];
static char *var_tok[MAX_VAR];
static int var_ptr = TOK_VAR;

/*
---------------------------------------------------------------

		compute - init_compute

                init_compute(predef)
                int predef

                Initialize global data structure
                predef is number of predefined variables

---------------------------------------------------------------
*/
static  void init_compute(int predef)
{
  skip = 0;
  stats = 1;
  arg  = predef-1;
  pre  = 0;
  line_ptr = line;
  state_stack[0].pst = 0;
  state_stack[0].tok = 0;
  state_stack[1].pst = 0;
  state_stack[1].tok = 1024+arg;
  new_bool(INT_1);                 /* Empty string gives true */
}



/* -------------------------------------------------------------

		compute - scanner

Name:		scanner
Syntax:		next_token(token,line_pointer)
                int *token;
                char **line_pointer;
Description:    Reads next token from input line. If the token
                is a constant the constant is put on the arg_stack.
                The token returned is TOK_E + arg.
                Token is returned in the variable token.
                The input line is scanned from the position
                line_pointer and the new position is returned
                by the same variable.

                1) Scan for integer.  Token = TOK_E.
                2) Scan for real.     Token = TOK_E.
                3) Scan for string.   Token = TOK_E.
                4) Scan for operator. Token =
                   *  TOK_MUL, /  TOK_DIV, +  TOK_ADD, -  TOK_SUB,
		   <  TOK_LT,  <= TOK_LE,  == TOK_EQ,  != TOK_NE,
		   >= TOK_GE,  >  TOK_GT,  (  TOK_LP,  )  TOK_RP,
		   =  TOK_SEQ
                5) Scan for reserved word.
                   LT, LE, EQ, NE, GE, GT, AND, OR, NOT, IS, ISNOT,
                   FILE, EXE, READ, WRITE. xxx --> TOK_xxx.
		6) Scan for userdefined FUNCTIONS
		7) Define new variable
Files:	
See also:	
Diagnostics:
Restrictions:
Return value:
Author:		Otto Milvang
Examples:
Id:             $Id$

---------------------------------------------------------------	*/

#define TOK_NUL 0
#define TOK_STR 1
#define TOK_ARI 2
#define TOK_ARB 16
#define TOK_MUL 16
#define TOK_DIV 17
#define TOK_ADD 32
#define TOK_SUB 33
#define TOK_ARE 47
#define TOK_REL 3
#define TOK_REB 48
#define TOK_LT  48
#define TOK_LE  49
#define TOK_EQ  50
#define TOK_NE  51
#define TOK_GE  52
#define TOK_GT  53
#define TOK_REE 63
#define TOK_STB 64
#define TOK_IS  64
#define TOK_ISNOT 65
#define TOK_IN  66
#define TOK_ELEM 68
#define TOK_STE 79
#define TOK_BOO 4
#define TOK_BOB 80
#define TOK_NOT 81
#define TOK_AND 96
#define TOK_ANDF 97
#define TOK_OR  112
#define TOK_ORT 113
#define TOK_SEQ 120
#define TOK_BOE 127
#define TOK_UNA 5
#define TOK_UNB 128
#define TOK_UNE 255
#define TOK_CTR 6
#define TOK_CTB 256
#define TOK_LP  256
#define TOK_RP  512
#define TOK_EOL 768
#define TOK_E   1024
#define TOK_PRE   1116
#define TOK_INT   1116
#define TOK_REAL  1117
#define TOK_FILE  1124
#define TOK_EXE   1125
#define TOK_WRITE 1126
#define TOK_READ  1128

/*
---------------------------------------------------------------

       	scanner - tok_type

        Returns the token type
        TOK_NUL - Unknown
        TOK_STR - String operator
        TOK_ARI - Aritmetic operator
        TOK_BOO - Boolean operator
        TOK_UNA - Unary operator
        TOK_CTR - Other known token.

---------------------------------------------------------------
*/

#define is_str(tok) ((tok) >= TOK_STB && (tok) <= TOK_STE)
#define is_ari(tok) ((tok) >= TOK_ARB && (tok) <= TOK_ARE)
#define is_rel(tok) ((tok) >= TOK_REB && (tok) <= TOK_REE)
#define is_boo(tok) ((tok) >= TOK_BOB && (tok) <= TOK_BOE)
#define is_una(tok) ((tok) >= TOK_UNB && (tok) <= TOK_UNE)
#define is_ctr(tok) ((tok) >= TOK_CTB)

static int tok_type(int tok)
{
  if(is_str(tok)) return(TOK_STR);
  if(is_ari(tok)) return(TOK_ARI);
  if(is_rel(tok)) return(TOK_REL);
  if(is_boo(tok)) return(TOK_BOO);
  if(is_una(tok)) return(TOK_UNA);
  if(is_ctr(tok)) return(TOK_CTR);
  return(TOK_NUL);
}



/*
---------------------------------------------------------------

       	scanner - token_int

        token_int(st,next,i,term)
        char *st,*term,**next;
	int *i;

        Return true if st is an integer followed by a term-char.
        st   - input string.
        next - returns a pointer to first character not in i.
        i    - returns the integer value.
        term - a string of legal terminators.

---------------------------------------------------------------
*/

static int token_int(char *st, char **next, INTEG *i, char *term)
{
  *i = strtol(st,next,0);
  if (*next == st) return(0);
  return((**next == 0) || (strchr(term,**next) != NULL));
}


/*
---------------------------------------------------------------

       	scanner - token_real

        token_real(st,next,i,term)
        char *st,*term,**next;
        double *f;

        Return true if st is a real followed by a term-char.
        st   - input string.
        next - returns a pointer to first character not in f.
        f    - returns the real value.
        term - a string of legal terminators.

---------------------------------------------------------------
*/

static int token_real(char *st, char **next, double *f, char *term)
{
#ifndef MSDOS
  char old,*oldptr;
  oldptr = st;
  *next = st;
  *f = 0.0;
  while(isspace(*oldptr)) oldptr++;
  if (*oldptr == '+' || *oldptr == '-') oldptr++;
  if(! isdigit(*oldptr)) return(0);
  while(isdigit(*oldptr)) oldptr++;
  if(*oldptr == '.') { 
    oldptr++;
    while(isdigit(*oldptr)) oldptr++;
  }
  if(*oldptr == 'e' || *oldptr == 'E') { 
    oldptr++;
    if (*oldptr == '+' || *oldptr == '-') oldptr++;
    if(isdigit(*oldptr)) { 
      while(isdigit(*oldptr)) oldptr++;
    } else {
       oldptr--;
       if (*oldptr == '+' || *oldptr == '-') oldptr--;
  }
  }
  old = *oldptr;
  *oldptr = 0;
  *f = strtod(st,next);
  *oldptr = old;
  *next = oldptr;
  if (*next == st) return(0);
  return((**next == 0) || (strchr(term,**next) != NULL));
#else
  *f = strtod(st,next);
  if (*next == st) return(0);
  return((**next == 0) || (strchr(term,**next) != NULL));
#endif
}

/*
---------------------------------------------------------------

       	scanner - token_tok

        token_tok(st,next,tok)
        char *st,**next;
        int *tok;

        Return token if reservwd word

---------------------------------------------------------------
*/
static int token_op(char *st, char **next)
{
  char h1,h2;
  *next = st;
  h1 = *st;
  h2 = *(st+1);
  if(h1 == 0)    return(TOK_EOL);
  *next = st+1;
  if (h1 == '(') return(TOK_LP);
  if (h1 == ')') return(TOK_RP);
  if (h1 == '+') return(TOK_ADD);
  if (h1 == '-') return(TOK_SUB);
  if (h1 == '/') return(TOK_DIV);
  if (h1 == '*') return(TOK_MUL);
  if (h1 == '<' && h2 != '=') return(TOK_LT);
  if (h1 == '>' && h2 != '=') return(TOK_GT);
  if (h1 == '=' && h2 != '=') return(TOK_SEQ);
  *next = st+2;
  if (h1 == '<' && h2 == '=') return(TOK_LE);
  if (h1 == '>' && h2 == '=') return(TOK_GE);
  if (h1 == '!' && h2 == '=') return(TOK_NE);
  if (h1 == '=' && h2 == '=') return(TOK_EQ);
  *next = st;
  return(TOK_NUL);
}


static int token_tok(char *st, char **next)
{
  static char reserved[20][6]=
  {
    "",
    "LT",     /*  1 */
    "LE",     /*  2 */
    "EQ",     /*  3 */
    "NE",     /*  4 */
    "GE",     /*  5 */
    "GT",     /*  6 */
    "OR",     /*  7 */
    "IS",     /*  8 */
    "IN",     /*  9 */
    "NOT",    /* 10 */
    "AND",    /* 11 */
    "INT",    /* 12 */
    "EXE",    /* 13 */
    "REAL",   /* 14 */
    "FILE",   /* 15 */
    "ELEM",   /* 16 */
    "READ",   /* 17 */
    "WRITE",  /* 18 */
    "ISNOT"   /* 19 */
   };
   static int tok[] = {TOK_NUL,TOK_LT,TOK_LE,TOK_EQ,TOK_NE,TOK_GE,TOK_GT,
               TOK_OR,TOK_IS,TOK_IN,TOK_NOT,TOK_AND,TOK_INT,TOK_EXE,
               TOK_REAL,TOK_FILE,TOK_ELEM,TOK_READ,TOK_WRITE,TOK_ISNOT};
  /*                      0  1  2  3  4  5  6  7  8  9     */
  static
  int hash[]= { /*  0 */  0,15, 0, 0,16,14,11, 0, 0, 0,
                /* 10 */  0, 0, 0, 0, 0, 0, 0, 5, 0, 0,
                /* 20 */  0,13, 2, 0, 4, 0,18, 3, 9,19,
                /* 30 */ 12, 0, 6, 8, 0, 0,10, 1, 7,17,
                /* 40 */  0 };
  char *help;
  int len, sum, i;
  sum = 0;
  help = st;
  while (isalpha(*help))
    {
      *help = toupper(*help);
      sum = sum+ *help;
      help++;
    }
  sum = sum - sum/41*41;
  len = help - st;
  if (isspace(*help)) help++;
  *next=help;
  while(hash[sum]) {
    if(strncmp(reserved[hash[sum]],st,len) == 0) { 
      return(tok[hash[sum]]);
    } else {
      sum=sum+1;
    };
  }
  for(i=TOK_USER; i<user_ptr; i++) {
    if (strlen(user_tok[i-TOK_USER]) == len)
    if (strncmp(user_tok[i-TOK_USER],st,len) == 0)
      return(i);
  }
  for(i=TOK_VAR; i<var_ptr; i++) {
    if (strlen(var_tok[i-TOK_VAR]) == len)
    if (strncmp(var_tok[i-TOK_VAR],st,len) == 0)
       return(i);
  }
  return(compute_variable(st,len));
}


/*
---------------------------------------------------------------

       	scanner - token_st

	token_st(st,next,c, term)
	char *st,**next, **c, term;

        Return string in c.

---------------------------------------------------------------
*/

static int token_st(char* st, char** next, char** c, char term)
{
  char *ptr;
  st++;
  ptr = st;
  while(*ptr != term && *ptr != 0) ptr++;
  *c = (char*) malloc(ptr-st+1);
  strncpy(*c, st, ptr-st);
  (*c)[ptr-st] = 0;
  if (*ptr) ptr++;
  *next = ptr;
  return(1);
}

/*
---------------------------------------------------------------

	scanner - token_arg

	token_arg(st,next)
	char *st,**next;

	Returns token.

---------------------------------------------------------------
*/
static int token_arg(char *st, char **next)
{
  st++;
  pre = pre+1;
  if (strchr("SDFLU",*st)) arg_stack[pre] = arg_stack[0];
  if(*st) st++;
  *next = st;
  return(TOK_E + pre);
}

/* ---------------------------------------------------------------

       	scanner - new_arg

        new_arg(token,type,l,f,c)
        int *token, type;
	int *l;
        double *f;
        char *c;

        Allocate a new stack_element for arg.


--------------------------------------------------------------- */
static int new_token(int type, INTEG l, double f, char *c)
{
  number *arg1;
  arg = arg + 1;
  arg1 = &arg_stack[arg];
  i_set(arg1, 0);
  t_set(arg1, type);
  switch(type) {
    case TY_UN: i_set(arg1, l); break;
    case TY_CI: i_set(arg1, l); break;
    case TY_CD: d_set(arg1, f); break;
    case TY_CS: p_set(arg1, c); break;
    case TY_BO: i_set(arg1, l); break;
    case TY_CP: i_set(arg1, l); break;
    case TY_VR: i_set(arg1, l); break;
  }
  return(arg);
}

/*
---------------------------------------------------------------

       	scanner - next_token

        next_token(token, next_pointer);
        *token;
        char ** next_pointer;

        Read next token.


---------------------------------------------------------------
*/
static void next_token(int *token, char **next_pointer)
{
  char *c, *c1, *c2;
  INTEG l;
  int tok;
  double f;
  c2 = *next_pointer;
  while(isspace(*c2)) c2++;
  if(isdigit(*c2) && token_int(c2,&c1,&l," ()+-*/<=>!")) { 
    *token = TOK_E + new_int(l);
  } else if(isdigit(*c2) && token_real(c2,&c1,&f," ()+-*/<=>!")) {
    *token = TOK_E + new_double(f);
  } else if(*c2 == '"') {
    token_st(c2,&c1,&c,'"');
    *token = TOK_E + new_string(c);
  } else if(*c2 == 0x27) {
    token_st(c2,&c1,&c,0x27);
    *token = TOK_E + new_string(c);
  } else if(*c2 == '%') {
    *token = token_arg(c2,&c1);
  } else if((tok = token_op(c2,&c1))) {
    *token = tok;
  } else if((tok = token_tok(c2,&c1))) {
    *token = tok;
    if (tok>=TOK_VAR)
    {
      tok = *token-TOK_VAR;
      *token = TOK_E + new_var(tok);
    } else if(*token >= TOK_PRE) {
      tok = *token-TOK_PRE;
      *token = TOK_E + new_pre(tok);
    }
  } else {
    *token = TOK_NUL;
  }
  *next_pointer = c1;
#ifdef DEBUG
  fprintf(stderr, "Next: %s\n",c1);
#endif
}

/* -------------------------------------------------------------

		compute - convert

Name:		c_convert, d_convert
Syntax:		c_convert(arg)
                number *arg;
                d_convert(arg1,arg2);
                number *arg1, *arg2;
Description:    c_convert converts a string to INTEG or double.
                d_convert converts arg1 and arg2 to the same type
Author: 	Otto Milvang
Examples:
Id:             $Id$

---------------------------------------------------------------	*/
static void c_convert(number *arg)
{
  INTEG i;
  double f;
  char *st,*t1,*t2;
  if(t_get(arg) != TY_CS) return;
  st = p_get(arg);
  token_int(st,&t1,&i,NULL);
  token_real(st,&t2,&f,NULL);
  i_set(arg, i);
  d_set(arg, f);
  if(t2 > st) { 
  if(t2 > t1) { 
    d_set(arg, f);
    t_set(arg, TY_CD);
  } else {
    i_set(arg, i);
    t_set(arg, TY_CI);
  };
  }
}

static void d_convert(number *arg1, number *arg2)
{
    c_convert(arg1);
    c_convert(arg2);
    if(t_get(arg1) == TY_CI && t_get(arg2) == TY_CD) { 
      i_set(arg2, (INTEG) d_get(arg2));
      t_set(arg2, TY_CI);
      return;
    }
    if(t_get(arg1) == TY_CD && t_get(arg2) == TY_CI) { 
      d_set(arg2, (double) i_get(arg2));
      t_set(arg2, TY_CD);
      return;
    }
}

/* -------------------------------------------------------------

		compute - unary op

Name:		red1, redc1, redi1, redf1, redb1
Syntax:		redx1(arg1,op)
                number *arg1;
                int op;
Description:    Compute: op arg1
Files:	
See also:	
Return value:   Return value = 0  -> OK
                Return value = 7  -> Not legal operation
Return value:
Author:         Otto Milvang		
Examples:
Id:             $Id$

---------------------------------------------------------------	*/

static int redi1(number *arg1, int op)
{
  INTEG i1;
  if(skip) return(E_OK);
  i1 = i_get(arg1);
  switch(op) {
    case TOK_ADD:             break;
    case TOK_SUB: i1 = - i1;  break;
    default: return(E_TYP);   /* break; */
  }
  i_set(arg1, i1);
  return(E_OK);
}
static int redf1(number *arg1, int op)
{
  double f1;
  if(skip) return(E_OK);
  f1 = d_get(arg1);
  switch(op) {
    case TOK_ADD:                break;
    case TOK_SUB: f1 = -f1;      break;
    default:      return(E_TYP); /* break; */
  }
  d_set(arg1, f1);
  return(E_OK);
}

static int redb1(number *arg1, int op)
{
  INTEG i1;
  if(skip) return(E_OK);
  i1 = i_get(arg1);
  switch(op) {
    case TOK_NOT: i1 = ! i1;     break;
    default:      return(E_TYP); /* break; */
  }
  i_set(arg1, i1);
  return(E_OK);
}
static int redc1(number *arg1, int op)
{
  return(E_TYP);
}

static int red1(number *arg1, int op)
{
  int tok_class;
  if (t_get(arg1) == TY_VR) *arg1 =  *(var_def[i_get(arg1)]);
  tok_class = tok_type(op);
  if(tok_class == TOK_UNA) { 
    switch(t_get(arg1)) {
      case TY_UN: return(E_VAR);
      case TY_CI: return(redi1(arg1,op-128)); /* break; */
      case TY_CD: return(redf1(arg1,op-128)); /* break; */
      case TY_BO: return(redb1(arg1,op-128)); /* break; */
      default:    return(E_TYP);              /* break; */
    }
  }
  return(E_TYP);
}

/* -------------------------------------------------------------

		compute - binary op

Name:		red2, redcp2, redia2, redfa2, redir2, redfr2, redb2
Syntax:		redxx2(arg1, arg2, op)
                number *arg1, *arg2;
                int op;
Description:    Compute: arg1 op arg2
Files:	
See also:	
Return value:   Return value = 0  -> OK
                Return value = 7  -> Not legal operation
Return value:
Author:         Otto Milvang		
Examples:
Id:             $Id$

---------------------------------------------------------------	*/
static int redb2(number *arg1, number *arg2, int op)
{
  INTEG i0,i2;
  i2 = i_get(arg2);
  switch(op) {
    case TOK_AND:  i0 = i2;        break;
    case TOK_ANDF: i0 = 0; skip=0; break;
    case TOK_OR:   i0 = i2;        break;
    case TOK_ORT:  i0 = 1; skip=0; break;
  }
  i_set(arg1, i0);
  return(E_OK);
}

static int redcp2(number *arg1, number *arg2, int op)
{
  INTEG i0, i2, it;
  char c, *c1, *c2, *ct;
  double ft;
  number *arg0;
  arg0 = &arg_stack[0];
  if(skip) return(E_OK);
  if (t_get(arg2) != TY_CP) return(E_TYP);
  if (op > TOK_ISNOT) return(E_TYP);
  c1 = p_get(arg1);
  i2 = i_get(arg2) + TOK_PRE;
  if(i2 >= TOK_USER) { 
    c_convert(arg1);
    i0 = user_def[i2-TOK_USER](arg1);
  } else {
  switch(t_get(arg1)) {
    case TY_CI:
      switch((int) i2) {
	case TOK_INT:  i0 = 1;        break;
	case TOK_REAL: i0 = 0;        break;
	default:       return(E_TYP); /* break; */
      }                                         break;
    case TY_CD:
      switch((int)i2) {
	case TOK_INT:  i0 = 0;        break;
	case TOK_REAL: i0 = 1;        break;
	default:       return(E_TYP); /* break; */
      }                                         break;
    case TY_CS:
      switch((int)i2) {
	case TOK_INT:
	  token_int(c1,&ct,&it,NULL);
          while(isspace(*ct)) ct++;
	  i0 = (*ct == 0 && c1 != ct);
	  if(i0) { 
	    i_set(arg0, it);
	    t_set(arg0, TY_CI);
	  }                           break;
	case TOK_REAL:
	  token_real(c1,&ct,&ft,NULL);
	  while(isspace(*ct)) ct++;
	  i0 = (*ct == 0 && c1 != ct);
	  if(i0) { 
	    d_set(arg0, ft);
	    t_set(arg0, TY_CD);
	  }                           break;
	case TOK_FILE:
	  i0 = 0 == access(c1,0);     break;
	case TOK_EXE:
	  i0 = 0 == access(c1,1);     break;
	case TOK_WRITE:
	  i0 = 1; ct = c1;
	  while ((c = *ct++)) i0 = i0 && (c > ' ' && c <= 'z');
	  if(i0) { 
	   i0 = 0 == access(c1,2);
	   if(i0 == 0 && errno == 2) { 
	    i0 = creat(c1,-1);
	    if(i0 >= INT_0) { 
	      close((int) i0);
	      unlink(c1);
	      i0 = INT_1;
	    } else {
	      i0 = INT_0;
	    }
	  }
	  }                           break;
	case TOK_READ:
	  i0 = 0 == access(c1,4);     break;
	default:
	  if(t_get(arg2) == TY_CS) { 
	    c2 = p_get(arg2);
	    return(E_TYP);
	  } else {
	    return(E_TYP);
	  }                           /* break; */
      }                                         break;
    }
  }
  if (op == TOK_ISNOT) i0 = ! i0;
  i_set(arg1, i0);
  return(E_OK);
}

static int redcc2(number *arg1, number *arg2, int op)
{
  INTEG i0, i1, i2;
  int il;
  char *c1, *c2, *ct;
  if(skip) return(E_OK);
  if (t_get(arg1) != TY_CS) return(E_TYP);
  if (t_get(arg2) != TY_CS) return(E_TYP);
  c1 = p_get(arg1);
  c2 = (char*) malloc(strlen(p_get(arg2))+1);
  strcpy(c2, p_get(arg2));
  i0 = 0;
  i1 = 0;
  i2 = 1;
  il = strlen(c1);
  ct = strtok(c2," ");
  while(1) {
    if(ct == NULL) break;
    if(op == TOK_IN) { 
      if(strlen(ct) >= il) { 
	if(strncmp(ct,c1,il) == 0) { 
	   i0++;
	   i1 = i2;
	}
      }
    } else {
      if(strcmp(ct,c1) == 0) { 
	i0++;
	i1 = i2;
      }
    }
    ct = strtok(NULL," ");
    i2 ++;
  }
  i2 = i1;
  if(i0 != 1) i2 = 0;
  i_set(arg1, i2);
  free(c2);
  return(0);
}


static int redia2(number *arg1, number *arg2, int op)
{
  INTEG i0,i1,i2;
  if(skip) return(E_OK);
  i1 = i_get(arg1);
  i2 = i_get(arg2);
  switch(op) {
    case TOK_ADD: i0 = i1 + i2; break;
    case TOK_SUB: i0 = i1 - i2; break;
    case TOK_MUL: i0 = i1 * i2; break;
    case TOK_DIV: i0 = i1 / i2; break;
  };
  i_set(arg1, i0);
  return(E_OK);
}
static int redfa2(number *arg1, number *arg2, int op)
{
  double f0,f1,f2;
  if(skip) return(E_OK);
  f1 = d_get(arg1);
  f2 = d_get(arg2);
  switch(op) {
    case TOK_ADD: f0 = f1 + f2; break;
    case TOK_SUB: f0 = f1 - f2; break;
    case TOK_MUL: f0 = f1 * f2; break;
    case TOK_DIV: f0 = f1 / f2; break;
  };
  d_set(arg1, f0);
  return(E_OK);
}

static int redir2(number *arg1, number *arg2, int op)
{
  INTEG i0,i1,i2;
  if(skip) return(E_OK);
  i1 = i_get(arg1);
  i2 = i_get(arg2);
  switch(op) {
    case TOK_LT: i0 = i1 <  i2; break;
    case TOK_LE: i0 = i1 <= i2; break;
    case TOK_EQ: i0 = i1 == i2; break;
    case TOK_NE: i0 = i1 != i2; break;
    case TOK_GE: i0 = i1 >= i2; break;
    case TOK_GT: i0 = i1 >  i2; break;
  };
  i_set(arg1, i0);
  return(E_OK);
}
static int redfr2(number *arg1, number *arg2, int op)
{
  INTEG i0;
  double f1,f2;
  if(skip) return(E_OK);
  f1 = d_get(arg1);
  f2 = d_get(arg2);
  switch(op) {
    case TOK_LT: i0 = f1 <  f2; break;
    case TOK_LE: i0 = f1 <= f2; break;
    case TOK_EQ: i0 = f1 == f2; break;
    case TOK_NE: i0 = f1 != f2; break;
    case TOK_GE: i0 = f1 >= f2; break;
    case TOK_GT: i0 = f1 >  f2; break;
  };
  i_set(arg1, i0);
  return(E_OK);
}

static int red2(number *arg1, number *arg2, int op)
{
  int tok_class, stat;
  if (t_get(arg2) == TY_VR) *arg2 =  *(var_def[i_get(arg2)]);
  if (t_get(arg2) == TY_UN) return(E_VAR);
  if (op == TOK_SEQ)
  {
    if (t_get(arg1) == TY_VR)
    {
      *(var_def[i_get(arg1)]) = *arg2;
      return(E_OK);
    }
    return(E_EQ);
  }
  tok_class = tok_type(op);
  if (t_get(arg1) == TY_VR) *arg1 =  *(var_def[i_get(arg1)]);
  if (t_get(arg1) == TY_UN) return(E_VAR);

  switch(tok_class) {
    case TOK_STR:
      if((op == TOK_IN) || (op == TOK_ELEM)) { 
	stat = redcc2(arg1,arg2,op);
        arg1 -> u_type = TY_CI;
      } else {
	stat = redcp2(arg1,arg2,op);
	t_set(arg1,  TY_BO);
      }                                                 break;
    case TOK_ARI:
	d_convert(arg1,arg2);
	switch(t_get(arg2)) {
	  case TY_CI: stat = redia2(arg1, arg2, op); break;
	  case TY_CD: stat =redfa2(arg1, arg2, op); break;
	  default:    return(E_TYP);          /* break; */
	}                                               break;
    case TOK_REL:
        d_convert(arg1,arg2);
	t_set(arg1, TY_BO);
	switch(t_get(arg2)) {
	  case TY_CI: stat = redir2(arg1, arg2, op); break;
	  case TY_CD: stat = redfr2(arg1, arg2, op); break;
	  default:    return(E_TYP);          /* break; */
	}                                               break;
    case TOK_BOO:
	/* if (t_get(arg1) EQ TY_CI) t_set(arg1, TY_BO);
	if (t_get(arg2) EQ TY_CI) t_set(arg2, TY_BO);    */
	if(t_get(arg1) == TY_BO && t_get(arg2) == TY_BO) { 
	  stat = redb2(arg1,arg2,op);
	} else {
	  return(E_TYP);
	}                                               break;
    default: return(E_TOK);                            /* break; */
  }
  return(E_OK);
}

/* -------------------------------------------------------------

                compute - parser

Name:		parse, next_symbol, shift_symbol, reduce_symbol
Grammar:
                0) S -> E$
                   S -> $
		1) E -> op E
		2) E -> E op E
		3) E -> (E)
States:
                0) * S          1) op * E       2) E op * E
                   * $             * op E          * op E
		   * E$            * E op E        * E op E
		   * op E          * (E)           * (E)
		   * E op E
		   * (E)

		3) ( * E)       4) (E) *        5) E * $
		   * op E                          E * op E
		   * E op E
		   * (E)

		6) op E *       7) E op E *     8) (E * )
		   E * op E        E * op E        E * op E

Parse table:
                State   op      (       )       $       E
		--------------------------------------------
		s0      s1      s3      e1      Acc-1   s5
		s1      s1/e1   s3      e1      e4      s6
		s2      s1/e1   s3      e1      e4      s7
                s3      s1      s3      e1      e4      s8
                s4      r3      e2      r3      r3      e2
                s5      s1      e2      e3      Acc-2   e2
                s6      r1      e2      r1      r1      e2
                s7      s2/r2   e2      r2      r2      e2
                s8      s2      e2      s4      e5      e2
Conflicts:
                c1 = s1/e1 - Return error.
                c2 = s2/e2 - Skift if previous op > this op.
Errors:
                e1 - Missing argument
                e2 - Missing operand
                e3 - Too many )
                e4 - Unexpected end of line
                e5 - Missing )

Author:         Otto Milvang		
Examples:
Id:             $Id$

---------------------------------------------------------------	*/

#define PA_S0 0
#define PA_S1 1
#define PA_S2 2
#define PA_S3 3
#define PA_S4 4
#define PA_S5 5
#define PA_S6 6
#define PA_S7 7
#define PA_S8 8
#define PA_R0 10
#define PA_R1 11
#define PA_R2 12
#define PA_R3 13
#define PA_C0 20
#define PA_C1 21
#define PA_C2 22
#define PA_E0 30
#define PA_E1 (PA_E0 + E_ARG)
#define PA_E2 (PA_E0 + E_OP)
#define PA_E3 (PA_E0 + E_LP)
#define PA_E4 (PA_E0 + E_EOL)
#define PA_E5 (PA_E0 + E_RP)
#define PA_NL 60
#define PA_A0 70
#define PA_A1 70
#define PA_A2 71

/*
---------------------------------------------------------------

		parse - next_action

                next_action(token)
                int token;

                Get next_action from parse_tab

---------------------------------------------------------------
*/
static
short parse_tab[9][5] = { {PA_S1, PA_S3, PA_E1, PA_A1, PA_S5},
			  {PA_C1, PA_S3, PA_E1, PA_E4, PA_S6},
			  {PA_C1, PA_S3, PA_E1, PA_E4, PA_S7},
			  {PA_S1, PA_S3, PA_E1, PA_E4, PA_S8},
			  {PA_R3, PA_E2, PA_R3, PA_R3, PA_E2},
			  {PA_S2, PA_E2, PA_E3, PA_A2, PA_E2},
			  {PA_R1, PA_E2, PA_R1, PA_R1, PA_E2},
			  {PA_C2, PA_E2, PA_R2, PA_R2, PA_E2},
			  {PA_S2, PA_E2, PA_S4, PA_E5, PA_E2} };
static int next_action(int token)
{
  int next,tok;
  next=parse_tab[state_stack[stats].pst][token/256];
  if(next == PA_C1) { 
    if(state_stack[stats].tok < TOK_UNB ||  state_stack[stats].tok >= (TOK_NOT+TOK_UNB)) { 
      next = PA_S1;
    } else {
      next = PA_E1;
    }
  } else if(next == PA_C2) {
    if(token/8 < state_stack[stats-1].tok/8) { 
      next = PA_S2;
    } else if((tok_type(token) == TOK_REL) &&  (tok_type(state_stack[stats-1].tok) == TOK_REL) ) {
      tok = state_stack[stats].tok;
      parse(TOK_AND);
      parse(tok);
      next = PA_S2;
    } else {
      next = PA_R2;
    }
  }
  return(next);
}

/*
---------------------------------------------------------------

		parse - shift symbol

                shift_symbol(next, token)
                int next, token

                Input next symbol on stack.
                Exeptions:
                 IS NOT    --> ISNOT
                TRUE OR   --> TRUE ORT
                FALSE AND --> FALSE ANDF
                op E      --> (op+128) E

---------------------------------------------------------------
*/
static void shift_symbol(int next, int token)
{
  int *tok = &state_stack[stats].tok;
  number *arg1 = &arg_stack[*tok-TOK_E];
  if(*tok == TOK_IS && token == TOK_NOT) { 
    *tok = TOK_ISNOT;
  } else {
    if(token == TOK_AND && t_get(arg1) == TY_BO) { 
       skip = ! i_get(arg1);
       if(skip)token++;
    }
    if(token == TOK_OR && t_get(arg1) == TY_BO) { 
       skip =  (int) i_get(arg1);
       if(skip)token++;
    }
    stats = stats + 1;
    if(next == PA_S1 && token) token = token+128;
    state_stack[stats].pst = next;
    state_stack[stats].tok= token;
  }
}

/*
---------------------------------------------------------------

		parse - reduce_symbol

                reduce_symbol(next)
                int next;

                1) E -> op E
                2) E -> E op E
                3) E -> (E)

---------------------------------------------------------------
*/
static int reduce_symbol(int next)
{
  int red, arg1, arg2, st;
  red = next-PA_R0;
  switch(red) {
    case 1:
#ifdef DEBUG
	    if(state_stack[stats].tok < TOK_E)              return(E_ARG);
	    if(tok_type(state_stack[stats-1].tok) != TOK_UNA) return(E_OP);
#endif
            arg1 = state_stack[stats].tok - TOK_E;
            st   = red1(&arg_stack[arg1],
                        state_stack[stats - 1].tok);
            if(st) return(st);
            state_stack[stats - 1].tok = state_stack[stats].tok;
	    stats = stats - 1;
	    break;
    case 2:
#ifdef DEBUG
	    if(state_stack[stats].tok < TOK_E)               return(E_ARG);
	    if(tok_type(state_stack[stats-1].tok) >= TOK_UNA) return(E_OP);
	    if(state_stack[stats-2].tok < TOK_E)             return(E_ARG);
#endif
            arg1 = state_stack[stats-2].tok - TOK_E;
            arg2 = state_stack[stats].tok   - TOK_E;
            st   = red2(&arg_stack[arg1],
                        &arg_stack[arg2],
                        state_stack[stats-1].tok);
            if(st) return(st);
	    stats = stats - 2;
	    break;
    case 3:
#ifdef DEBUG
	    if(state_stack[stats].tok != TOK_RP)    return(E_RP);
	    if(state_stack[stats-1].tok < TOK_E)   return(E_OP);
	    if(state_stack[stats-2].tok != TOK_LP) return(E_LP);
#endif
            state_stack[stats-2].tok = state_stack[stats-1].tok;
            stats = stats - 2;
	    st = 0;
	    break;
  };
  state_stack[stats].pst= parse_tab[state_stack[stats-1].pst][4];
  return(st);
}

/*
---------------------------------------------------------------

		parse

                parse(token)
                int token;

                Find next_action.
                Reduce if possible.
                Shift symbol.

---------------------------------------------------------------
*/
static int parse(int token)
{
  int next, exit, err;
  exit=0;
  while(1) {
    next = next_action(token);
    if(next == PA_NL) { 
    } else if(next < PA_R0) {
      DUMP("shift",token);
      shift_symbol(next, token);
      exit = 1;
    } else if(next < PA_C0) {
      DUMP("reduce",next-PA_R0);
      if((err=reduce_symbol(next)))
	return(err);
    } else if(next < PA_E0) {
    } else if(next < PA_A0) {
      DUMP("error",next);
      return(next-PA_E0);
    } else {
      DUMP("accept",0);
      exit=16;
    }
    if(exit) break;
  }
  return(exit);
}

/* -------------------------------------------------------------

		compute_line

Name:           compute_line
Syntax:         compute_line(inp_line, ...)
		char *inp_line;
Description:    parse a line. Compute answer.
Files:
See also: 
Diagnostics:
Restrictions:
Return value:
Author:
Examples:
Id:             $Id$

--------------------------------------------------------------- */

int compute_line(char *inp_line, ...)
{
  va_list ap;
  number *tos, *bos;
  int predef[16];
  int argno, first, last;
  int token, exit;
  char *c,*c1, *c2, *cp;

  va_start(ap, inp_line);

  if (inp_line == NULL) inp_line = empty;
  c = inp_line;
  first = 0;
  argno = 0;
  while(*c)
    if(*c == '%') { 
      c++;
      argno++;
      switch(*c) {
	case 'D': first = 1; predef[0]     = TY_CI ; break;
	case 'F': first = 1; predef[0]     = TY_CD ; break;
	case 'L': first = 1; predef[0]     = TY_CL ; break;
	case 'S': first = 1; predef[0]     = TY_CS ; break;
	case 'U': first = 1; predef[0]     = TY_CU ; break;
	case 'd':            predef[argno] = TY_CI ; break;
	case 'f':            predef[argno] = TY_CD ; break;
	case 'l':            predef[argno] = TY_CS ; break;
	case 's':            predef[argno] = TY_CS ; break;
	case 'u':            predef[argno] = TY_CS ; break;
	default:                                     break;
      }
    } else {
      c++;
    }
  pre = 1 - first;
  while(pre <= argno)
    {
      arg = pre-1;
      arg_stack[pre].u_type = predef[pre];
      switch(predef[pre]) {
	case TY_CI: new_int(va_arg(ap,INTEG))       break;
	case TY_CD: new_double(va_arg(ap,double)) break;
	case TY_CS: new_string(va_arg(ap,char *)) break;
	case TY_CL: c1 = va_arg(ap, char *);
		    c2 = cp = (char *) malloc(strlen(c1)+1);
		    while(*c1)
		      {
			*c2++ = tolower(*c1);
			c1++;
		      }
		    *c2 = 0;
		    arg_stack[pre].u_type = predef[pre] = TY_CS;
		    new_string(cp);
		    break;
	case TY_CU: c1 = va_arg(ap, char *);
		    c2 = cp = (char *) malloc(strlen(c1)+1);
		    while(*c1)
		      {
			*c2++ = toupper(*c1);
			c1++;
		      }
		    *c2 = 0;
		    arg_stack[pre].u_type = predef[pre] = TY_CS;
		    new_string(cp);
		    break;
      }
      pre++;
    };
  va_end(ap);
  init_compute(pre);
  last = arg;
  global_error = E_OK;
  global_answer = NULL;
  if(strlen(inp_line) >= 1023)
  {
    global_error = E_LEN;
    return(TY_UN);
  }
  strcpy(line,inp_line);
  c1 = line;
  while(1) {
    next_token(&token,&c1);
    if (token == 0)
    {
      global_error = 1;
      return(-1);
    }
    exit = parse(token);
    if (exit >= 2 && exit != 16)
    {
      global_error = exit;
      return(-1);
    }
    if(exit == 16) break;
  };
  tos = &arg_stack[state_stack[stats].tok-TOK_E];
  bos = &arg_stack[0];
  if (tos == bos) tos = &arg_stack[last];
  global_answer = tos;
  if (t_get(global_answer) == TY_VR)
  {
    *global_answer =  *(var_def[i_get(global_answer)]);
    if (t_get(global_answer) == TY_UN)
    {
      global_error = E_VAR;
      return(-1);
    }
  }

  if (t_get(tos) == TY_BO) return( (int) i_get(tos));
  return(t_get(tos));
}


static char *err[] = {
  "E_OK  - No errors",
  "E_TOK - Undefined token",
  "E_ARG - Missing argument.",
  "E_OP  - Missing operand.",
  "E_LP  - Too many )'s.",
  "E_EOL - Unexpected end of line.",
  "E_RP  - Expected )",
  "E_TYP - Wrong arg type for op",
  "E_??? - ",
  "E_LEN - Too long input line",
  "E_EQ  - Variable expected",
  "E_VAR - Undefined variable",
};

char *compute_error(int message)
{
  if (message) printf("\nCompute error: %s\n\n",err[global_error]);
  if (message == 2) abort();
  return(err[global_error]);
}

number *compute_answer(void)
{
  return(global_answer);
}

/* -------------------------------------------------------------

		compute_define

Name:           compute_define
Syntax:         compute_define(token, action)
		char *token;
		func action;
Description:    Define a new token.
Files:
See also:
Diagnostics:
Restrictions:
Return value:
Author:
Examples:
Id:             $Id$

--------------------------------------------------------------- */

int compute_define(char *name, func action)
{
  char *tmp;
  tmp = name;
  while(*tmp)
  {
    (void) toupper(*tmp);
    tmp++;
  }
  if(user_ptr == TOK_USER+MAX_USER) return(TOK_NUL);
  tmp = (char *) malloc(strlen(name)+1);
  strcpy(tmp,name);
  user_tok[user_ptr-TOK_USER] = tmp;
  user_def[user_ptr-TOK_USER] = action;
  return(user_ptr ++);
}

static int compute_variable(char *name, int len)
{
  char *tmp;
  number *num;
  if(var_ptr == TOK_VAR+MAX_VAR) return(TOK_NUL);
  if (len == 0) len = strlen(name);
  tmp = (char *) malloc(len+1);
  strncpy(tmp,name, len);
  tmp[len] = 0;
  var_tok[var_ptr-TOK_VAR] = tmp;
  num = (number*) malloc(sizeof(number));
  num -> u_type = TY_UN;
  var_def[var_ptr-TOK_VAR] = num;
  return(var_ptr ++);
}

#ifdef TEST
#define MAIN 1

/* -------------------------------------------------------------

		compute_line
Author:         Otto Milvang
Examples:
Id:             $Id$

--------------------------------------------------------------- */

static int lowprime(arg)
number *arg;
{
  int num;
  if (t_get(arg) != TY_CI) return(0);
  num = (int) i_get(arg);
  if (num == 2) return(1);
  if (num == 3) return(1);
  if (num == 5) return(1);
  if (num == 7) return(1);
  if (num == 11) return(1);
  if (num == 13) return(1);
  if (num == 17) return(1);
  if (num == 23) return(1);
  if (num == 29) return(1);
  if (num == 31) return(1);
  if (num == 37) return(1);
  if (num == 41) return(1);
  if (num == 43) return(1);
  return(0);
}
#endif

#ifdef MAIN
void main(int argc, char **argv)
{
  char c[1024],*cp;
  number answer;
  int i;
#ifdef TEST
  compute_define("PRIME", lowprime);
#endif
  do
  {
    cp = c;
    if(argc > 1)
      {
	for(i=1; i < argc; i++)
	  {
	    strcpy(cp,argv[i]);
	    cp = cp + strlen(argv[i]);
	    *cp = ' ';
	    cp ++;
	  }
      } else {
	printf(">> ");
	while((*cp++ = (char) getchar()) != (char) 10);
	if (cp == c+1) break;
      }
    cp--;
    *cp = 0;
#ifdef TEST
    i = compute_line(c, "12","y");
#else
    i = compute_line(c);
#endif
    switch(i) {
      case TY_UN: compute_error(1);                        break;
      case TY_BF: printf("FALSE\n");                       break;
      case TY_BT: printf("TRUE\n");                        break;
      case TY_CI: printf("%ld\n",i_get(compute_answer())); break;
      case TY_CD: printf("%f\n", d_get(compute_answer())); break;
      case TY_CS: printf("%s\n", p_get(compute_answer())); break;
      case TY_BO: if(i_get(compute_answer())) {
		    printf("True\n");
		  } else {
		    printf("False\n");
		  }
		  break;
      case TY_VR: printf("Variable %s defined\n",
			 var_tok[i_get(compute_answer())]); break;
      default: printf("Unexpected return value %d\n",i); break;
    }
  } while(argc == 1);
  exit(0);
}

#endif
