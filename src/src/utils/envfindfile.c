
/*

________________________________________________________________

        envfindfile.c
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



/*F:envfindfilemulti=envfindfile*/
/*F:envfindfile*

________________________________________________________________

		envfindfile
________________________________________________________________

Name:		envfindfile, envfindfilemulti - search for file in an
                environment path
Syntax:         | #include <xite/utils.h>
		|
                | char *envfindfile( char* environment,
                |    char* file );
		|
		| char *envfindfilemulti(char *environment,
		|    char *file);
Description:    Search for the file 'file' in directories
                specified by the environment variable.
		If environment == NULL, dir path = "."
Return value:	'envfindfile' and 'envfindfilemulti' both return full
                file spec of the file or NULL. If the file was found
                in more than one directory, 'envfindfilemulti' returns
		all the filenames separated by ':'.

		The memory space needed for the returned value is
		allocated by these functions. It should be freed
		(with free(3)) when not needed any more.
Example:	| char *filename, envfindfile();
		| filename = endfindfile("PROG_PATH",".progrc");
Author:		Otto Milvang and Svein Bøe
Id              $Id$
________________________________________________________________

*/

#include <xite/includes.h>
#include <xite/utils.h>
#include XITE_STDIO_H
#include XITE_STRING_H
#include XITE_FILE_H
#include <stdlib.h>  /* For getenv() */
#include XITE_UNISTD_H
#include XITE_MALLOC_H
#include XITE_PARAM_H

#ifndef F_OK
#define F_OK 0
#endif

#ifndef MAX
# define MAX(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef _WIN32
static char dir_sep_s = '/';
#else /* _WIN32 */
static char dir_sep_s = '\\';
#endif /* _WIN32 */

#ifndef FUNCPROTO
char *envfindfile(environment, file)
char *environment, *file;
#else /* FUNCPROTO */
char *envfindfile(char *environment, char *file)
#endif /* FUNCPROTO */
{
  char *env, *ptr, *tptr, *path = NULL;
  int len = 0, current_path_len = MAXPATHLEN;

  env = NULL;
  if (environment) env = getenv(environment);
  if (!env) env = ".";

  path = (char *) calloc(current_path_len+1, sizeof(char)); /* All zeros. */
  if (path == NULL) {
    Warning(1, "envfindfile: Cannot allocate space.\n");
    return(0);
  }

  for (ptr = env; ptr && *ptr; ptr = tptr) {
    /* For each directory in the environment variable. */

    tptr = strchr(ptr,':');
    if (tptr) {
      /* There is one more directory after the one referred by 'ptr'. */

      if (tptr == ptr) 
	/* Two consecutive ':' characters. */

	sprintf(path, "%s","");
      else {
	len = (int) (tptr-ptr); /* Length of additional directory name. */

	/* Add filepath. */
	sprintf(path, "%.*s/%s", len, ptr, file);
      }
      tptr++;
    } else {
      /* This is the last directory (referred to by 'ptr'). */

      /* Add filepath. */
      sprintf(path, "%s/%s", ptr, file);

    }
    if (access(path, F_OK) == 0) return (path);
  }      
  return(0);
}




/*F:realloc_string*

________________________________________________________________

		realloc_string
________________________________________________________________

Name:		realloc_string - Reallocate string to ensure enough
                memory is available for an extension.
Syntax:         | #include <xite/utils.h>
		| 
                | char *realloc_string( char* path,
                |    int max_path_len, int extra_len,
                |    int step_len );
		|
Description:	If the length of 'path' increased by 'extra_len'
                exceeds 'max_path_len', then reallocate 'path' such that
		the new string is extended by 'step_len' (or by
		'extra_len' if greater than 'step_len').

		If reallocated, the old memory space is not freed.
Author:		Svein Bøe
Id: 		$Id$
________________________________________________________________

*/

#ifndef FUNCPROTO
char *realloc_string(path, max_path_len, extra_len, step_len)
char *path;
int max_path_len, extra_len, step_len;
#else /* FUNCPROTO */
char *realloc_string(char *path, int max_path_len, int extra_len, int step_len)
#endif /* FUNCPROTO */
{

  if (strlen(path) + extra_len > max_path_len) {
    path =
      (char *) realloc(path,
		       (size_t) (max_path_len + MAX(extra_len, step_len) + 1));
  }

  return(path);

} /* realloc_string() */

#ifndef FUNCPROTO
char *envfindfilemulti(environment, file)
char *environment, *file;
#else /* FUNCPROTO */
char *envfindfilemulti(char *environment, char *file)
#endif /* FUNCPROTO */
{
  char *env, *ptr, *tptr, *path = NULL, *pathptr;
  int foundOne = 0,
      len = 0,
      current_path_len = 3*MAXPATHLEN,
      path_diff = 0;

  env = NULL;
  if (environment) env = getenv(environment);
  if (!env) env = ".";
  path = (char *) calloc(current_path_len+1, sizeof(char)); /* All zeros. */
  if (path == NULL) {
    Warning(1, "envfindfilemulti: Cannot allocate space.\n");
    return(0);
  }
  pathptr = path;

  for (ptr = env; ptr && *ptr; ptr = tptr) {
    /* For each directory in the environment variable. */
    tptr = strchr(ptr,':');
    if (tptr) {
      /* There is one more directory after the one referred by 'ptr'. */

      if (tptr == ptr) {
	/* Two consecutive ':' characters. */

	sprintf(pathptr, "%s","");
	len = 0;
      } else {
	len = (int) (tptr-ptr); /* Length of additional directory name. */

	/* Make sure that the path can contain the additional text. */
	path_diff = (int) (pathptr - path);
	path = realloc_string(path, current_path_len,
			      len + strlen(file) + 2, MAXPATHLEN);
	if (path == NULL) {
	  Warning(1, "envfindfilemulti: Cannot allocate space.\n");
	  return(0);
	}
	pathptr = path + path_diff;

	/* Insert filename separator if this is not the first directory. */
	if (pathptr != path) sprintf(pathptr++, ":");

	/* Add filepath for this directory. */
	sprintf(pathptr, "%.*s/%s", len, ptr, file);
	len += strlen(file) + 1;
      }
      tptr++;
    } else {
      /* This is the last directory (referred to by 'ptr'). */

      len = strlen(ptr) + strlen(file) + 1;

      /* Make sure that the path can contain the additional text. */
      path_diff = (int) (pathptr - path);
      path = realloc_string(path, current_path_len,
			    strlen(ptr) + strlen(file) + 1, MAXPATHLEN);
      pathptr = path + path_diff;
      if (pathptr != path) sprintf(pathptr++, ":");
      sprintf(pathptr, "%s/%s", ptr, file);

    }
    if (access(pathptr, F_OK) == 0) {
      foundOne = 1;
      pathptr += len;
    }
  }
  
  if (foundOne == 1) return(path);
  else return(0);
}



#ifndef FUNCPROTO
char *String_cat(s1, s2, s3)
char *s1, *s2, *s3;
#else /* FUNCPROTO */
char *String_cat(char *s1, char *s2, char *s3)
#endif /* FUNCPROTO */
{
  char *result = NULL,
       *fnew   = NULL;
  int len      = 0;

  if (s1 || s2 || s3) {
    result    = malloc(MAXPATHLEN + 1);
    if (result == NULL) {
      Warning(1, "String_cat: Cannot allocate space.\n");
      return(NULL);
    }
    result[0] = '\0';
    len       = MAXPATHLEN;
  }

  if (s1) (void) strcat(result, s1);

  if (s2) {
    fnew = realloc_string(result, len, strlen(s2) + 1, MAXPATHLEN);
    if (fnew != result) {
      len += MAXPATHLEN;
      free(result);
      result = fnew;
      fnew = NULL;
    }
    (void) strcat(result, ":");
    (void) strcat(result, s2);
  }

  if (s3) {
    fnew = realloc_string(result, len, strlen(s3) + 1, MAXPATHLEN);
    if (fnew != result) {
      len += MAXPATHLEN;
      free(result);
      result = fnew;
      fnew = NULL;
    }
    (void) strcat(result, ":");
    (void) strcat(result, s3);
  }

  return(result);

} /* String_cat() */



/*F:NextFilename*

________________________________________________________________

		NextFilename
________________________________________________________________

Name:		NextFilename - extract next string in a ":"-separated
                list of strings.
Syntax:         | #include <xite/utils.h>
		| 
                | void NextFilename( char** filenames,
                |    char** next );
Description:	Return in '*next' the substring starting at the
                address given by '*filenames', ending before the next
		":". Update '*filenames' to refer to the first character
		after the extracted substring. If no ":" found, return
		'*filenames' equal to NULL. If length of substring is
		zero, return '*next' equal to NULL.

		Required memory space for *next is allocated by
		this function. It should be freed (with 'free(3)') when
		not needed any more.
See also:	'envfindfilemulti(3)'
Author:		Svein Bøe
Doc:            Svein Bøe
Id: 		$Id$
________________________________________________________________

*/


#ifndef FUNCPROTO
void NextFilename(namelist, filename)
char **namelist, **filename;
#else /* FUNCPROTO */
void NextFilename(char **namelist, char **filename)
#endif /* FUNCPROTO */
{
  char fname[MAXPATHLEN+1], *fname2, *cptr;

  fname2 = *namelist;
  if (!fname2) {
    *filename = NULL;
    return;
  }

  cptr = strchr(fname2, ':');

  if (cptr) {
    /* Found ':'. */
    int len;
    
    len = (int) (cptr - fname2);
    (void) strncpy(fname, fname2, len);
    fname[len] = 0;

    *namelist += strlen(fname) + 1;

  } else {
    /* No more ':'. */
    
    (void) strcpy(fname, fname2);

    *namelist = NULL;
  }

  if (strlen(fname) > 0) {
    *filename = (char *) malloc(strlen(fname) + 1);
    (*filename)[0] = '\0';
    (void) strcpy(*filename, fname);
  }
  else *filename = NULL;

} /* NextFilename() */





/*F:FilenamePart*

________________________________________________________________

		FilenamePart
________________________________________________________________

Name:		FilenamePart - Return only the filename part of a path.
Syntax:         | #include <xite/utils.h>
		| 
                | char *FilenamePart( char* filename );

Description:	Return a pointer to the last substring of a string which may
                consist of "/"-separated (under UNIX) or "\"-separated (under
		Windows NT) parts.

		No memory space is allocated by this function.

Return value:   NULL if "/" (or "\") is the last character in 'filename',
                otherwise a string.
Author:		Svein Bøe
Doc:            Svein Bøe
Id: 		$Id$
________________________________________________________________

*/

#ifndef FUNCPROTO
char *FilenamePart(filename)
char *filename;
#else /* FUNCPROTO */
char *FilenamePart(char *filename)
#endif /* FUNCPROTO */
{
  char *cptr, *stp;

  stp = cptr = filename;
  while(*cptr) {
    if (*cptr == dir_sep_s) stp = &cptr[1];
    cptr++;
  }

  /* Is "/" the last character? */
  if (*stp == '\0') return(NULL);

  return(stp);

} /* FilenamePart() */



/*L:envfindfile*

________________________________________________________________

		test
________________________________________________________________

*/


#ifdef TEST

main(argc, argv)
int argc;
char **argv;
{
  char *e;
  if (argc != 3)
    {
      fprintf(stderr, "usage: %s <environment> <filename>\n",argv[0]);
      exit(2);
    }
  e = envfindfile(argv[1], argv[2]);
  fprintf(stderr,"%s\n",e? e: "Not found"); 
}

#endif
