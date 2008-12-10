

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

char *nextSpace(char *input)
{
  /* Return first space or zero-terminating character. */

  while( !(isspace(*input)) && (*input)) input++;

  return(input);
}

char *nextNonSpace(char *input)
{
  /* Return first non-space or zero-terminating character. */

  while(isspace(*input) && (*input)) input++;

  return(input);
}

char *nextWord(char *input)
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

char *remove_quotes_from_word(char *word)
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

char *iindex ( char *s, char c )
{
  char *ss;
  ss = &s[strlen(s)-1];
  while (ss >= s) if (*s == c) return(s); else s++;
  return(NULL);
}

char *rrindex(char *s, char c)
{
  char *ss;
  ss = &s[strlen(s)-1];
  while (ss >= s) if (*ss == c) return(ss); else ss--;
  return(NULL);
}

#ifdef DEF_STRCASECMP

int strcasecmp(char *s1, char *s2)
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

char *center_text(char *long_txt, char *short_txt, int width)
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
