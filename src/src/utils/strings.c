

/*C*

________________________________________________________________

        strings
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

#include <xite/includes.h>
#include <ctype.h>
#include <xite/strings.h>
#include XITE_STRING_H
#include XITE_MALLOC_H

#ifndef FUNCPROTO
char *nextSpace(input)
char *input;
#else /* FUNCPROTO */
char *nextSpace(char *input)
#endif /* FUNCPROTO */
{
  /* Return first space or zero-terminating character. */

  while( !(isspace(*input)) && (*input)) input++;

  return(input);
}

#ifndef FUNCPROTO
char *nextNonSpace(input)
char *input;
#else /* FUNCPROTO */
char *nextNonSpace(char *input)
#endif /* FUNCPROTO */
{
  /* Return first non-space or zero-terminating character. */

  while(isspace(*input) && (*input)) input++;

  return(input);
}

#ifndef FUNCPROTO
char *nextWord(input)
char *input;
#else /* FUNCPROTO */
char *nextWord(char *input)
#endif /* FUNCPROTO */
{
  char *t1, *t2, *word;

  t1 = nextNonSpace(input);
  if (*t1 == '\0') return(NULL);

  t2 = nextSpace(t1);

  word = (char *) malloc((t2 - t1 + 1) * sizeof(char));
  (void) strncpy(word, t1, t2 - t1);
  word[t2 - t1] = '\0';

  return(word);
}

#ifndef FUNCPROTO
char *remove_quotes_from_word(word)
char *word;
#else /* FUNCPROTO */
char *remove_quotes_from_word(char *word)
#endif /* FUNCPROTO */
{
  int len;

  if (!word) return(NULL);

  word = nextNonSpace(word); /* No allocation. */

  if (*word == '\"') word++;
  if (*word == '\0') return(word);

  len = strlen(word);
  if (word[len - 1] == '\"') word[len - 1] = '\0';

  return(word);

} /* remove_quotes_from_word() */

/*F:iindex*

________________________________________________________________

		iindex
________________________________________________________________

Name:		iindex - Return pointer to character in string
Syntax:         | #include <xite/strings.h>
                |
                | char *iindex( char* s, char c );
		| char *rrindex(char *s, char c);
		| char *remove_quotes_from_word( char *word );
		|
Description:    'iindex' (or 'rrindex') returns a pointer to the first
                (or last) occurence of 'c' in 's'.

		'remove_quotes_from_word' strips any single quotes (")
		from the front and back of 'word'. Any single trailing quote
		will be replaced by the null character.

Return value:   For 'iindex' and 'rrindex': Pointer to 'c' in 's' or NULL
                if 'c' is not in 's'.

		For 'remove_quotes_from_word': Pointer to the first character
		after any leading quote.
Author:		Otto Milvang
________________________________________________________________

*/

#ifdef FUNCPROTO
char *iindex ( char *s, char c )
#else
char *iindex(s, c)
char *s, c;
#endif
{
  char *ss;
  ss = &s[strlen(s)-1];
  while (ss >= s) if (*s == c) return(s); else s++;
  return(NULL);
}

#ifndef FUNCPROTO
char *rrindex(s, c)
char *s, c;
#else /* FUNCPROTO */
char *rrindex(char *s, char c)
#endif /* FUNCPROTO */
{
  char *ss;
  ss = &s[strlen(s)-1];
  while (ss >= s) if (*ss == c) return(ss); else ss--;
  return(NULL);
}

#ifdef DEF_STRCASECMP

#ifndef FUNCPROTO
int strcasecmp(s1, s2)
char *s1, *s2;
#else /* FUNCPROTO */
int strcasecmp(char *s1, char *s2)
#endif /* FUNCPROTO */
{
  int c1, c2;

  while (*s1 && *s2) {
      c1 = isupper(*s1) ? tolower(*s1) : *s1;
      c2 = isupper(*s2) ? tolower(*s2) : *s2;
      if (c1 < c2) return(-1);
      if (c1 > c2) return(1);
      s1++;
      s2++;
  }
  if (*s2) return(-1);
  if (*s1) return(1);

  return (0);

} /* strcasecmp() */

#endif /* DEF_STRCASECMP */

#ifndef FUNCPROTO
char *center_text(long_txt, short_txt, width)
char *long_txt, *short_txt;
int width;
#else /* FUNCPROTO */
char *center_text(char *long_txt, char *short_txt, int width)
#endif /* FUNCPROTO */
{
  char *ptr;
  int len;

  ptr = strchr(short_txt, ' ');
  if (ptr == NULL) ptr = short_txt + strlen(short_txt);
  len = (ptr - short_txt);                    /* Number of non-blanks. */

  (void) strncpy(long_txt, "                             ", (width - len) / 2);

  ptr = long_txt + (width - len) / 2;
  (void) strncpy(ptr, short_txt, len);
  ptr[len] = 0;

  return(long_txt);

} /* center_text() */
