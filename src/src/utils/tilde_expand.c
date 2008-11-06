

/*

________________________________________________________________

        tilde_expand.c
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



/*F:xite_pipe_filedes=tilde_expand*/
/*F:xiteFilenameType=tilde_expand*/
/*F:tilde_expand*

________________________________________________________________

		tilde_expand
________________________________________________________________

Name:		tilde_expand, xiteFilenameType, xite_pipe_filedes - handle
                filenames in XITE

Syntax:         | #include <xite/utils.h>
		|
                | char *tilde_expand( char* filename );
		|
		| xite_filename_type xiteFilenameType( char *name );
		|
		| int xite_pipe_filedes( char *filename, int reading );
		|

Description:    'tilde_expand' expands "~" (tilde) and environment variables
                at the front of 'filename'. The interpretation of tilde is the
                same as for the command shells "csh", "tcsh" and "bash" on a
                UNIX system. If the expansion fails, 'filename' is returned.

		The pattern "~/name1" (or "~\\name1" on Windows NT) is
		interpreted as the file "name1" in the current user''s home
		directory. The current user''s home directory is determined
		from the environment variable HOME, if set. Otherwise, on a
		Unix system, the current user''s entry in the password
		database is enquired to find the home directory, while on a
		Windows NT system, the environment variable HOMEPATH is
		used.

		The pattern "~name1" is interpreted as the home directory of
		user "name1", the pattern "~name1/name2" is interpreted as the
		file "name2" in the home directory of user "name1". On a
		Unix system, the user home directory is looked up in the
		password database. On a Windows NT system, these patterns
		are returned unchanged.

		An environment variable at the front of 'filename' is assumed
		to start with a "$" on a UNIX system, like this:
		$SOME_DIR/file. Under Windows NT it is assumed to be enclosed
		in a pair of "%", like this: %SOME_DIR%\file.

		'xiteFilenameType' checks to see if 'name' is a special XITE
		filename. Special (or standard) filenames are those which are
		to be interpreted as pipe channels. These include "-0"
		(stdin), "-1" (stdout) "-" (stdin or stdout depending on the
		occasion), "-n" (where 2 <= n <= 9), "" (same as "-") and NULL
		(same as "-"). Channel number "n" may be larger than 9.

		'xite_pipe_filedes' returns a file descriptor corresponding to
		any of the special XITE filenames recognized by
		'xiteFilenameType'. 'reading' decides whether to open stdin
		or stdout if 'filename' is ambiguous.

Return value:   For 'tilde_expand', the expanded name of the file (or NULL if
                'filename' is NULL or a zero-length string). The required
                memory is allocated by this function and should be freed (with
                'free(3)') when no longer needed.

		For 'xiteFilenameType' one of the values of type
		xite_filename_type given in the file
		| $XITE_HOME/include/utils.h.

Authors:	Ellen Sentovich, Rick Spickelmier

Modifications:  Otto Milvang, Svein Bøe

Id:             $Id$
________________________________________________________________

*/

#include <xite/includes.h>
#include <xite/utils.h>
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
#include <stdlib.h>
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif

#ifdef HAVE_SYS_PARAM_H
#  include <sys/param.h>
#endif
#ifdef HAVE_PWD_H
#  include <pwd.h>
#endif
#if !defined(MSDOS) && !defined(_WIN32)
  static char dir_sep = '/', env_start = '$';
#elif defined(_WIN32)
  static char dir_sep = '\\', env_start = '%', env_end = '%';
#endif

#define USER_NAME_SIZE 32

char *tilde_expand(char *filename)
{
#ifdef MSDOS
  return(filename);
#else /* MSDOS */
  int current_path_len = MAXPATHLEN;
  char username[USER_NAME_SIZE], *loc;
# ifdef HAVE_STRUCT_PASSWD_PW_DIR
  struct passwd *pw;
# endif
  char *path = NULL;
  
  if ((filename == NULL) || strcmp(filename, "") == 0) {
    return(NULL);
  }
  
  path = (char *) calloc(current_path_len+1, sizeof(char)); /* All zeros. */
  if (path == NULL) {
    Warning(1, "tilde_expand(): Cannot allocate space.\n");
    return(filename);
  }
  
  if (filename[0] != '~' && filename[0] != env_start) {
    /* No tilde or environment variable at the beginning of filename. */
    (void) strcpy(path, filename);
    return(path);
  }
  
  if (filename[0] == env_start) {
    /* filename possibly starting with environment variable. */
    char *env;

# ifdef _WIN32
    if ((loc = (char *) strchr(filename, env_end)) == NULL)
      /* Not a Windows NT environment variable. */
      return(path);
# endif /* _WIN32 */

    /* Filename definitely starts with an environment variable. */

    if ((loc = (char *) strchr(filename, dir_sep)) == NULL) {
      /* filename consists entirely of environment variable. */

      loc = filename; loc++;
      if ((env = getenv(loc)) == NULL) {
	free(path);
	return(filename);
      }

      strcpy(path, env);
      return(path);
    }

    /* Filename has a '/' (or '\' on Windows NT) after the environment
     * variable. Try to replace the environment variable with its value. */

    (void) strncpy(username, &filename[1], loc - &filename[1]);
    username[loc - &filename[1]] = '\0';
    if ((env = getenv(username)) == NULL) {
      free(path);
      return(filename);
    }

    (void) sprintf(path, "%s%s", env, loc);
    return(path);

  } /* environment variable */
  
  /* tilde at the beginning now */
  if (filename[1] == '\0' || filename[1] == dir_sep) {
    /* current user */
    char *home;
    
    if ((home = getenv("HOME")) == NULL) {
# ifdef HAVE_STRUCT_PASSWD_PW_DIR
      /* fall back on /etc/passwd */
      if ((pw = getpwuid(getuid())) == (struct passwd *) 0) {
	free(path);
	return(filename);
      }

      (void) sprintf(path, "%s%s", pw->pw_dir, &filename[1]);
      return(path);
# else
      if ((home = getenv("HOMEPATH")) == NULL) {
	free(path);
	return(filename);
      }

      (void) sprintf(path, "%s%s", home, &filename[1]);
      return(path);
# endif /* _WIN32 */
    } else {
      (void) sprintf(path, "%s%s", home, &filename[1]);
      return(path);
    }
  } /* current user */

# ifdef HAVE_STRUCT_PASSWD_PW_DIR

  /* Not current user. */

  if ((loc = (char *) strchr(filename, dir_sep)) == NULL) {
    /* user top directory name */
    (void) strcpy(username, &filename[1]);
  } else {
    /* user file or sub directory name */
    (void) strncpy(username, &filename[1], loc - &filename[1]);
    username[loc - &filename[1]] = '\0';
  }
  
  if ((pw = getpwnam(username)) == (struct passwd *) 0) {
    free(path);
    return(filename);
  }
  
  if (loc) {
    /* user file or sub directory name*/
    (void) sprintf(path, "%s%s", pw->pw_dir, loc);
  } else {
    /* user top directory name */
    (void) sprintf(path, "%s", pw->pw_dir);
  }
  
  return(path);
# else /* _WIN32 */
  free(path);
  return(filename);
# endif /* _WIN32 */

#endif /* MSDOS */

} /* tilde_expand() */

xite_filename_type xiteFilenameType(char *name)
{
  if (!name) return(XITE_FILENAME_NONE);
  else if (name[0] == '\0') return(XITE_FILENAME_EMPTY);
  else if (name[0] == '-') {
    /* Possibly channel. */

    if (name[1] == '\0') return(XITE_FILENAME_STDINOUT);
    else if (name[1] == '0') return(XITE_FILENAME_STDIN);
    else if (name[1] == '1') return(XITE_FILENAME_STDOUT);
    else if (name[1] >= '2' && name[1] <= '9') return(XITE_FILENAME_PIPE);
    else return(XITE_FILENAME_OTHER);
  } 
  else return(XITE_FILENAME_OTHER);

} /* xiteFilenameType() */

int xite_pipe_filedes(char *filename, int reading)
{
  xite_filename_type fname_type;
  int fd = -1;

  fname_type = xiteFilenameType(filename);

  switch (fname_type) {
  case XITE_FILENAME_NONE:
  case XITE_FILENAME_EMPTY:
  case XITE_FILENAME_STDINOUT:
    if (reading) fd = 0;
    else fd = 1;
    break;
  case XITE_FILENAME_STDIN:
    fd = 0;
    break;
  case XITE_FILENAME_STDOUT:
    fd = 1;
    break;
  case XITE_FILENAME_PIPE:
    fd = -atoi(filename);
    break;
  case XITE_FILENAME_OTHER:
    /* Not a pipe. */
    fd = -1;
    break;
  } /* switch */

  return(fd);

} /* xite_pipe_filedes() */



#ifdef TEST


main()
{
  char test[80];
  printf("Enter a filename: ");
  gets(test);
  printf("%s -> %s\n", test, tilde_expand(test));
}

#endif

