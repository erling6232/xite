

/*C*

________________________________________________________________

        fork_xshow
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

#include <errno.h>
#include <xite/includes.h>
#include <xite/message.h>
#include <xite/readarg.h>
#include <xite/biff.h>
#include XITE_STDIO_H
#include XITE_FORK_H
#include XITE_UNISTD_H
#include <xite/arithmetic.h>
#include <xite/fork_xshow.h>



#ifndef MAIN

/*F:start_xshow*

________________________________________________________________

		start_xshow
________________________________________________________________

Name:		start_xshow - start xshow

Syntax:		| #include <xite/fork_xshow.h>
		| 
		| pid_t start_xshow( int *pipe_io );
		|
Description:	Start xshow in the background. Upon return, 'pipe_io'[1] will
                contain the file descriptor to use to send images to 'xshow'.
		Send images with 'Iwrite_image' and filename "-n", where 'n'
		equals 'pipe_io'[1]. Remember to close 'pipe_io'[1] when
		you don''t need it anymore.

		'xshow' will read from the filedescriptor in 'pipe_io[0]'.
		This filedescriptor is not open to the parent (calling)
		process.

Diagnostics:	Error messages will be issued if the pipe could not be
                created, if the child process could not be created, or if
		'xshow' could not be executed.

Return value:	| >0: Process ID for child process (xshow).
                | -2: Failed in creating pipes.
		| -1: Failed in creating child process.

See also:	xshow(1), fork_xshow(1), Iwrite_image(5)

Author:		Svein Bøe, BLAB, Ifi, UiO
Id: 		$Id$
________________________________________________________________

*/

#ifndef FUNCPROTO
pid_t start_xshow(pipe_io)
int *pipe_io;
#else /* FUNCPROTO */
pid_t start_xshow(int *pipe_io)
#endif /* FUNCPROTO */
{
  int status;
  pid_t child = 0;

  errno  = 0;
  status = pipe(pipe_io);

  if (status) {
    perror("start_xshow");
    Error(2, "start_xshow: Failed in creating pipe.\n");
    return((pid_t) -2);
  }

  errno  = 0;
  child  = fork();

  if (child == (pid_t) -1) {
    /* In parent process context. */
    close(pipe_io[0]); close(pipe_io[1]);
    perror("start_xshow");
    Error(2, "start_xshow: Failed in forking.\n");
    return(child);
  }

  if (child > 0) {
    /* In parent process context, 'child' is pid for child process. */

    close(pipe_io[0]);
    return(child);
  }

  if (child == 0) {
    /* In child process context. */

    char buf[20];

    close(pipe_io[1]);
    sprintf(buf, "%d", pipe_io[0]);

    errno = 0;
    execlp("xshow", "xshow", "-i", buf, "&", NULL);

    /* Here only if execlp() failed. */
    close(pipe_io[0]);
    perror("start_xshow");
    _exit(0);
  }
}

#endif /* not MAIN */

#ifdef MAIN

/*P:fork_xshow*

________________________________________________________________

		fork_xshow
________________________________________________________________

Name:		fork_xshow - an example of how to pass data to xshow

Syntax:		fork_xshow <inimage>

Description:	Start an 'xshow' child process and send an image to it.

Diagnostics:	Error messages will be issued if the pipe for communicating
                images could not be created, if the child process could not
		be created, or if 'xshow' could not be executed.

See also:       start_xshow(3), xshow(1)

Author:		Svein Bøe, BLAB, Ifi, UiO

Id: 		$Id$
________________________________________________________________

*/

#ifndef FUNCPROTO
int main(argc, argv)
int argc;
char **argv;
#else /* FUNCPROTO */
int main(int argc, char **argv)
#endif /* FUNCPROTO */
{
  char *args; /* To hold input arguments. */
  char buf[50];
  IMAGE img;
  int pipe_io[2], status;
  pid_t child;

  /* Automatic error message display in BIFF format routines: on  */
  Iset_message(1);

  /* Automatic job abortion on error in BIFF format routines: off */
  Iset_abort(0);

  /* Install usage message (end multiple lines with \n\)
   * xite_app_std_usage_text() will append a description of standard XITE
   * options. Replace xite_app_std_usage_text by ximage_app_usage_text
   * if the application uses the ximage toolkit.
   */
  InitMessage(&argc, argv, xite_app_std_usage_text("Usage: %s <inimage>\n"));

  /* Give usage message if no command-line arguments are given. */
  if (argc == 1) Usage(1, NULL);

  args = argvOptions(argc, argv);

  if (argc != 2) Usage(2, "Illegal number of arguments.\n");

  child = start_xshow(pipe_io);

  img = Iread_image(argv[1]);
  Iset_title(img, "From xx prog");
  sprintf(buf, "-%d\n", pipe_io[1]);
  status = Iwrite_image(img, buf);

  sleep(5); /* Seconds. */
  status = scale(img[1], img[1], 0.5, 40.0);
  status = Iwrite_image(img, buf);

  close(pipe_io[1]);

  return(0);
}

#endif /* MAIN */
