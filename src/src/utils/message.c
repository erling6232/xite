
/*

________________________________________________________________

        message.c
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




/*F:InitMessage=message*/
/*F:PushMessage=message*/
/*F:PopMessage=message*/
/*F:Info=message*/
/*F:Message=message*/
/*F:Warning=message*/
/*F:Error=message*/
/*F:Usage=message*/
/*F:Verbose=message*/
/*F:ExitOnError=message*/
/*F:LastMessage=message*/
/*F:MessageStream=message*/
/*F:DefaultMessage=message*/
/*F:DefaultNoMessage=message*/
/*F:xite_standard_options_usage_text=message*/
/*F:xite_app_std_usage_text=message*/
/*F:message*

________________________________________________________________

		message
________________________________________________________________

Name:		message, Info, Message, Warning, Error, Usage, InitMessage,
                PushMessage, PopMessage, Verbose, ExitOnError, LastMessage,
                MessageStream, DefaultMessage, DefaultNoMessage,
		xite_standard_options_usage_text, xite_app_std_usage_text -
		print error/warning/info/usage messages

Syntax:         | #include <xite/message.h>
		|
                | typedef int (*messagefunc)(int, char *);
                | typedef int (*Messagefunc)(int, char *, ...);
		|
                | int InitMessage( int* argc, char* argv[],
                |    char* usage );
                | int PushMessage( messagefunc info,
                |    messagefunc warning, messagefunc error,
                |    int exitonerror );
                | int PopMessage( void  );
		|
                | int Info( int id, char* format,  ...);
                | int Message( int id, char* format,  ...);
                | int Warning( int id, char* format,  ...);
                | int Error( int id, char* format,  ...);
                | int Usage( int id, char* format,  ...);
		|
                | int Verbose( void  );
                | int ExitOnError( void  );
                | char *LastMessage( void  );
                | FILE *MessageStream( void  );
		|
                | int DefaultMessage( int id, char* message );
                | int DefaultNoMessage( int id, char* message );
		|
		| char *xite_standard_options_usage_text( void );
		|
		| char *xite_app_std_usage_text( char *usage );
		|
Description:	This module defines a set of routines to 
                give information/warning/error/usage messages 
	        in a standard way.

		The system is initialized by 'InitMessage'. This routine will
		save the program name and usage text. All occurrences of %s in
		the usage text is replaced by the program name (max 5 times).
		Then it will search the command line for -help, -usage,
		-whatis, -man and -verbose switches. If
		-help/-usage/-whatis/-man is on the command line, the program
		will exit after printing out the usage text/man page. -verbose
		will set the verbose flag for the system. At last it will
		search for the environment variables VERBOSE and MESSAGEPROG.
		If the environment variable VERBOSE exists, the verbose flag
		is set. If the environment variable MESSAGEPROG exists, the
		program $MESSAGEPROG is started. All output from the message
		system is sent to $MESSAGEPROG.

		'Info', 'Message', 'Warning', 'Error', and 'Usage' are used
		to inform the user. The first parameter 'id'
		is the message number. This may be used as identification
		for installed action routines. A common way to use 'id'
		is to set id=0 for info, id=1 for warning, and id>=2 for
		error. The format on the rest of the parameters behaves
		exactly like the printf function. 'Info' will only be
		executed if the verbose flag is set. 'Message' behaves
		exactly like 'Info' except that the message is printed without
		regard to the 'verbose' flag. 'Usage' will print out a
		formatted error message followed by the usage text and
		then terminate the program with status 'id'.
		'Info', 'Warning' and 'Error' will invoke their action
		routines with the parameter 'id' and the formatted 
		text string. If the flag 'exitonerror' is set, the Error
		routine will terminate the program with status 'id'.
		When 'Error' is used from library routines it is
		important to keep in mind that 'exitonerror' may be FALSE.
	        An advice is to use: return(Error(...)) in library routines.

		'PushMessage' will install a new set of action routines
		and set a new status for the 'exitonerror' flag.
		Default action routine is 'DefaultMessage'. This routine
		prints the message to stderr. Another predefined 
		action routine is 'DefaultNoMessage' which does nothing.
		The installed action routine takes two arguments, 
		'int id' and 'char *message'. 
		If NULL is used as an action routine parameter, the
		action remains unchanged. To go back to the previous 
		state use the function 'PopMessage'. 

		'Verbose' and 'ExitOnError' will read the current settings
		of the flags 'verbose' and 'exitonerror'.

		'LastMessage' will return a pointer to the last message
		sent to the message system. 

		'MessageStream' returns a pointer to a stream
		for output messages.

		'xite_app_std_usage_text' returns the concatenation of
                'usage' and the standard usage text for
                XITE (given by 'xite_standard_options_usage_text').

		'xite_standard_options_usage_text' returns a string with
		a short description of the standard XITE options -help, -usage,
		-man, -whatis and -verbose.

Example:	| #include <xite/message.h>
		|
		| int my_error(int id, char *message)
		| {
		|   FILE *msg;
		|   msg = MessageStream();
		|   fprintf(msg, "**** Error no %d ****\\n", id);
		|   return(id);
		| }
		| 
		|
		| int lib_func(char *a, char *b)
		| {
		|   Info(0, "Library function(%s, %s)\\n", a,b);
		|   if (a==NULL || b == NULL) 
		|     return Error(2,
		|            "Argument a or b is NULL in lib_func\\n");
		|   / * Do something * /
                |   return(0); / * Status code 0 = OK * /
		| }
		|
		| main(int argc, char *argv[])
		| {
		|   int stat;
		|   InitMessage(&argc, argv,
		|               "Usage: %s <infile> <outfile>\\n");
		| 
		|   Info(0, "Test number of arguments\\n");
		|   if (argc != 3)
		|     Usage(2,"Wrong number of parameters\\n");
		| 
		|   Info(0, "Test Warning message\\n");
		|   Warning(0, "This is only a test version\\n");
		|   
		|   Info(0, "Test PushMessage and PopMessage\\n");
		|   PushMessage(NULL, NULL, my_error, 0);
		|   stat = Error(3, "This text is not printed\\n");
		|   PopMessage();
		| 
                |   lib_func(argv[1], argv[2]);
                |
		|   Info(0, "Test exit on Error\\n");
		|   Error(4, "Stat = %d\\n", stat);
		| 
		|   Warning(0,
		|     "The program will never reach this line\\n");
		| }
Restrictions:	The composed message string must not exceed 2047 bytes.
Return value:
Author:		Otto Milvang
________________________________________________________________

*/


#include <stdlib.h>
#include <errno.h>
#include <xite/includes.h>
#include <xite/strings.h>
#include <xite/utils.h>
#include XITE_STDARG_H
#include XITE_FORK_H
#include XITE_MALLOC_H
#include XITE_STDIO_H
#include XITE_STRING_H
#include XITE_UNISTD_H
#include XITE_FCNTL_H

typedef struct messagestruct {
  int level;
  messagefunc info;
  messagefunc warning;
  messagefunc error;
  int exitonerror;
  struct messagestruct *prev;
  struct messagestruct *next;
} messagest, *messageptr;

#ifdef FUNCPROTO 
int DefaultMessage(int id, char *message);
#else /* FUNCPROTO */
int DefaultMessage(/* int id, char *message */);
#endif /* FUNCPROTO */

static messagest base =
{
  0,
  DefaultMessage,
  DefaultMessage,
  DefaultMessage,
  1,
  NULL,
  NULL,
};

static messageptr current = &base;

static char *progname  = "(untitled)";
static int prognamelen = 10;
static char *usagetxt  = "No usage";
static char last_message[2048];

int verbose = 0;

static char *message_prog = NULL;

/* Do not initialize msgfile to stderr because stderr is not a constant on
 * all platforms. Check instead, during runtime, prior to use, whether
 * msgfile is NULL and in that case set to stderr.
 */
static FILE *msgfile = NULL;



#ifdef FUNCPROTO
int InitMessage(int *argc, char *argv[], char *usage)
#else /* FUNCPROTO */
int InitMessage(argc, argv, usage)
int *argc;
char *argv[];
char *usage;
#endif /* FUNCPROTO */
{
  char buf[256];
  char *ptr, *stp, *point;

  if (!msgfile) msgfile = stderr;

  progname = strcpy((char *) malloc(strlen(argv[0]) + 1), argv[0]);
  progname = FilenamePart(progname);

  point    = strrchr(progname, '.');
  if (point && !strcmp(point, ".bin")) *point = '\0';

  prognamelen = strlen(progname);
  ptr = usage;
  usagetxt = malloc(strlen(usage) + 5*strlen(progname));
  sprintf(usagetxt, ptr, progname, progname, progname, progname, progname);
  
  if (read_bswitch(argc, argv, "-help")) Usage(1, NULL);
  if (read_bswitch(argc, argv, "-usage")) Usage(1, NULL);
  if (read_bswitch(argc, argv, "-man")) {
#ifndef _WIN32
    sprintf(buf, "man -M $XITE_MAN %s", progname);
#else /* _WIN32 */
    sprintf(buf, "type %%XITE_MAN%%\\cat1\\%s.1 | more", progname);
#endif /* _WIN32 */
    system(buf);
    exit(1);
  }
  if (read_bswitch(argc, argv, "-whatis")) {
#ifndef _WIN32
    sprintf(buf, "man -M $XITE_MAN -f %s", progname);
#else /* _WIN32 */
    sprintf(buf, "find \"%s\" %%XITE_MAN%%\\windex", progname);
#endif /* _WIN32 */
    system(buf);
    exit(1);
  }
  verbose = read_bswitch(argc, argv, "-verbose");
  if (getenv("VERBOSE")) verbose = 1;
  message_prog = getenv("MESSAGEPROG");

  if (message_prog) {
    char *vec[132], *ptr;
    int fd[2], job, status = 0;
#ifdef _WIN32
    int parentStdin;
#endif
    enum {READ, WRITE};


    ptr = nextNonSpace(message_prog);
    if (!ptr || *ptr == '\0') return(-1);

    split_cmd_line(message_prog, vec);

#ifndef _WIN32
    errno = 0;
    if (pipe(fd) == -1) {
      perror("Error in InitMessage() pipe()");
      return(-2);
    }

    errno  = 0;
    if ((job = vfork()) == -1) {
      perror("Error in InitMessage() fork");

      /* Close both ends of pipe. */
      errno = 0;
      if (close(fd[READ]) == -1) {
	perror("Error in InitMessage() close(fd[READ])");
      }
      errno = 0;
      if (close(fd[WRITE]) == -1) {
	perror("Error in InitMessage() close(fd[WRITE])");
      }

      return(-2);
    } /* error */
    else if (job == 0) {
      /* Inside child process. */

      /* Close the writing end of pipe, for child. */
      errno = 0;
      if (close(fd[WRITE]) == -1) {
	perror("Error in InitMessage() close(fd[WRITE])");
	status = -2;
      }

      if (!status) {
	/* Close stdin of child. */
	errno = 0;
	if (close(0) == -1) {
	  perror("Error in InitMessage() close(0)");
	  status = -2;
	}
      }

      if (!status) {
	/* Make stdin of child refer to reading end of pipe. */
	errno = 0;
	if (dup2(fd[READ], 0) == -1) {
	  perror("Error in InitMessage() dup2()");
	  status = -2;
	}
      }

      if (!status) {
	/* Let child execute message program. */
	errno = 0;
	execvp(vec[0], vec);
      }

      /* We get here only if execvp() failed. */
      if (status || errno) {
	if (!status) perror("Error in InitMessage() execvp");

	status = -2;
	_exit(0); /* Leave child. */

	/* Remember to clean up for parent. */
      }

    } /* child */

    if (job > 0) { /*Not an "else" branch, in case execvp failed. */
      /* Inside parent. */

      /* Close reading end of pipe, for parent. */
      errno = 0;
      if (close(fd[READ]) == -1) {
	perror("Error in InitMessage() close(fd[READ]");
	status = -2;
      }

      if (status) {
	if (close(fd[WRITE]) == -1) {
	  perror("Error in InitMessage() close(fd[WRITE])");
	}
	return(status);
      }

      /* Initialize a stream for the open writing end of the pipe.
       * Send all future output from the message system into this pipe. */
      errno = 0;
      msgfile = fdopen(fd[WRITE], "w");
      if (!msgfile) {
	perror("Error in InitMessage() fdopen");
	if (close(fd[WRITE]) == -1) {
	  perror("Error in InitMessage() close(fd[WRITE])");
	}
	return(-2);
      }
    } /* parent */

#else

    /* Create pipe, but don't let child inherit it at this point. */
    errno = 0;
    if (_pipe(fd, 4096, _O_BINARY | _O_NOINHERIT) == -1) {
      perror("Error in InitMessage() _pipe()");
      return(-2);
    }

    /* Save stdin of parent. */
    errno = 0;
    if ((parentStdin = dup(_fileno(stdin))) == -1) {
      perror("Error in InitMessage() dup()");
      status = -2; /* Don't return until after clean-up. */
    }

    /* Make stdin refer to reading end of pipe. This will be inherited
     * by child. */
    errno = 0;
    if (dup2(fd[READ], _fileno(stdin)) == -1) {
      perror("Error in InitMessage() dup2()");
      status = -2; /* Don't return until after clean-up. */
    }

    /* Close the original reading end of the pipe. */
    errno = 0;
    if (close(fd[READ]) == -1) {
      perror("Error in InitMessage() close(fd[READ])");
      status = -2; /* Don't return until after clean-up. */
    }

    /* Start child process.
     * The parent now has the writing end of the pipe open, the child will
     * inherit the open reading end of the pipe which is attached to stdin. */

    errno  = 0;
    if (status || (job = spawnvp(_P_NOWAIT, vec[0], vec)) == -1) {
      if (job == -1) perror("Error in InitMessage() spawnvp()");

      /* Clean up, close writing end of pipe. */
      if (close(fd[WRITE]) == -1) {
	perror("Error in InitMessage() close(fd[WRITE])");
      }
      status = -2; /* Don't return until after clean-up. */
    }

    /* Set the original stdin for parent. */
    errno = 0;
    if (dup2(parentStdin, _fileno(stdin)) == -1) {
      perror("Error in InitMessage() dup2()");
      status = -2; /* Don't return until after clean-up. */
    }

    /* Close duplicate copy of original stdin. */
    errno = 0;
    if (close(parentStdin) == -1) {
      perror("Error in InitMessage() close(parentStdin)");
      status = -2; /* Don't return until after clean-up. */
    }

    if (status) {
      if (job != -1) {
	/* Clean up, close writing end of pipe. */
	if (close(fd[WRITE]) == -1) {
	  perror("Error in InitMessage() close(fd[WRITE])");
	}
      }
      return(status);
    }

    /* Initialize a stream for the open writing end of the pipe.
     * Send all future output from the message system into this pipe. */
    errno = 0;
    msgfile = fdopen(fd[WRITE], "w");
    if (!msgfile) {
      perror("Error in InitMessage() fdopen");
      close(fd[WRITE]);
      return(-2);
    }

#endif /* _WIN32 */

  } /* message_prog */
  
  return(0);
}

#ifndef FUNCPROTO
char *xite_standard_options_usage_text()
#else /* FUNCPROTO */
char *xite_standard_options_usage_text(void)
#endif /* FUNCPROTO */
{
  static char t[] =
"  Standard XITE options: \n\
    -help    : Print usage text for program and exit.\n\
    -usage   : Print usage text for program and exit.\n\
    -man     : Print man page for program and exit.\n\
    -whatis  : Print one-line man page description for program and exit.\n\
    -verbose : Set the verbose flag.\n";

  return(t);

} /* xite_standard_options_usage_text() */

#ifndef FUNCPROTO
char *xite_app_std_usage_text(usage)
char *usage;
#else /* FUNCPROTO */
char *xite_app_std_usage_text(char *usage)
#endif /* FUNCPROTO */
{
  char *standard_usage, *full_usage;

  standard_usage = xite_standard_options_usage_text();
  full_usage     = (char *) malloc(strlen(standard_usage) + strlen(usage) + 1);

  (void) strcpy(full_usage, usage);
  (void) strcat(full_usage, standard_usage);

  return(full_usage);

} /* xite_app_std_usage_text() */




#ifdef FUNCPROTO
int PushMessage(messagefunc info, messagefunc warning,
	    messagefunc error, int exitonerror)
#else /* FUNCPROTO */
int PushMessage(info, warning, error, exitonerror)
messagefunc info, warning, error;
int exitonerror;
#endif /* FUNCPROTO */
{
  messageptr new;
  if (current->next) new = current->next; else
  {
    new = (messageptr) malloc(sizeof(messagest));
    if (new == NULL)
      return(Error(-1, "Not enough memory (malloc(%d))\n", sizeof(messagest)));
    new->level = current->level + 1;
    new->prev  = current;
    current->next = new;
  }
  new -> info        = info ? info : current->info;
  new -> warning     = warning ? warning : current->warning;
  new -> error       = error ? error : current->error;
  new -> exitonerror = exitonerror;
  current = new;
  return(current->level);
}




#ifdef FUNCPROTO
int PopMessage(void)
#else /* FUNCPROTO */
int PopMessage()
#endif /* FUNCPROTO */
{
  if (current->level == 0)
    Warning(0, "Message level already 0 in PopMessage\n");
  else current = current->prev;
  return(current->level);
}




#ifdef FUNCPROTO
int Info(int id, char *format, ...)
{
  va_list ap;
  if (!verbose) return id;
  va_start(ap, format);
#else /* FUNCPROTO */
int Info(va_alist)
va_dcl
{
  va_list ap;
  int id;
  char *format;
  if (!verbose) return id;
  va_start(ap);
  id     = va_arg(ap, int);
  format = va_arg(ap, char *);
#endif /* FUNCPROTO */
  vsprintf(last_message, format, ap);
  va_end(ap);
  current->info(id, last_message);
  return id;
}




#ifdef FUNCPROTO
int Message(int id, char *format, ...)
{
  va_list ap;
  va_start(ap, format);
#else /* FUNCPROTO */
int Message(va_alist)
va_dcl
{
  va_list ap;
  int id;
  char *format;
  va_start(ap);
  id     = va_arg(ap, int);
  format = va_arg(ap, char *);
#endif /* FUNCPROTO */
  vsprintf(last_message, format, ap);
  va_end(ap);
  current->info(id, last_message);
  return id;
}




#ifdef FUNCPROTO
int Warning(int id, char *format, ...)
{
  va_list ap;
  va_start(ap, format);
#else /* FUNCPROTO */
int Warning(va_alist)
va_dcl
{
  va_list ap;
  int id;
  char *format;
  va_start(ap);
  id     = va_arg(ap, int);
  format = va_arg(ap, char *);
#endif /* FUNCPROTO */
  sprintf(last_message, "%s warning: ", progname);
  vsprintf(&last_message[10+prognamelen], format, ap);
  va_end(ap);
  current->warning(id, last_message);
  return id;
}




#ifdef FUNCPROTO
int Error(int id, char *format, ...)
{
  va_list ap;
  va_start(ap, format);
#else /* FUNCPROTO */
int Error(va_alist)
va_dcl
{
  va_list ap;
  int id;
  char *format;
  va_start(ap);
  id     = va_arg(ap, int);
  format = va_arg(ap, char *);
#endif /* FUNCPROTO */
  sprintf(last_message, "%s error: ", progname);
  vsprintf(&last_message[8+prognamelen], format, ap);
  va_end(ap);
  current->error(id, last_message);
  if (current->exitonerror) { 
    sleep(1);
    exit(id);
  }
  return id;
}




#ifdef FUNCPROTO
int Usage(int id, char *format, ...)
{
  va_list ap;
  va_start(ap, format);
#else /* FUNCPROTO */
int Usage(va_alist)
va_dcl
{
  va_list ap;
  int id;
  char *format;
  va_start(ap);
  id     = va_arg(ap, int);
  format = va_arg(ap, char *);
#endif /* FUNCPROTO */
  if (format)
    {
      sprintf(last_message, "%s error: ", progname);
      vsprintf(&last_message[8+prognamelen], format, ap);
    }
  else last_message[0] = 0;
  va_end(ap);
  if (strlen(last_message)) current->error(id, last_message);
  verbose = 1;
  current->info(id, usagetxt);
  sleep(1);
  exit(id);
  return id;
}




#ifdef FUNCPROTO 
int Verbose(void)
#else /* FUNCPROTO */
int Verbose()
#endif /* FUNCPROTO */
{
  return(verbose);
}




#ifdef FUNCPROTO 
int ExitOnError(void)
#else /* FUNCPROTO */
int ExitOnError()
#endif /* FUNCPROTO */
{
  return(current->exitonerror);
}




#ifdef FUNCPROTO 
char *LastMessage(void)
#else /* FUNCPROTO */
char *LastMessage()
#endif /* FUNCPROTO */
{
  return(last_message);
}




#ifdef FUNCPROTO 
int DefaultMessage(int id, char *message)
#else /* FUNCPROTO */
int DefaultMessage(id, message)
int id;
char *message;
#endif /* FUNCPROTO */
{
  if (!msgfile) msgfile = stderr;

  fprintf(msgfile, "%s", message);
  fflush(msgfile);
  return(id);
}



#ifdef FUNCPROTO 
int DefaultNoMessage(int id, char *message)
#else /* FUNCPROTO */
int DefaultNoMessage(id, message)
int id;
char *message;
#endif /* FUNCPROTO */
{
  return(id);
}



#ifdef FUNCPROTO 
FILE *MessageStream(void)
#else /* FUNCPROTO */
FILE *MessageStream()
#endif /* FUNCPROTO */
{
  if (!msgfile) msgfile = stderr;

  return(msgfile);
}

#ifdef TEST

#ifdef FUNCPROTO
int my_error(int id, char *message)
#else /* FUNCPROTO */
int my_error(id, message)
int id; 
char *message;
#endif /* FUNCPROTO */
{
  FILE *msg;
  msg = MessageStream();
  fprintf(msg, "**** Error no %d ****\n", id);
  return(id);
}

#ifdef FUNCPROTO
main(int argc, char *argv[])
#else /* FUNCPROTO */
main(argc, argv)
int argc;
char *argv[];
#endif /* FUNCPROTO */
{
  int stat;
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <infile> <outfile>\n"));

  Info(0, "Test number of arguments\n");
  if (argc != 3) Usage(2,"Wrong number of parameters\n");

  Info(0, "Test Warning message\n");
  Warning(0, "This is only a test version\n");
  
  Info(0, "Test PushMessage and PopMessage\n");
  PushMessage(NULL, NULL, my_error, 0);
  stat = Error(3, "This text is not printed\n");
  PopMessage();

  Info(0, "Test exit on Error\n");
  Error(4, "Stat = %d\n", stat);

  Warning(0, "The program will never reach this line\n");
}

#endif
