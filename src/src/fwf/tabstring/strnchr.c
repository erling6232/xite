#include <xite/includes.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include "TabString.h"

/*
 *	Like strchr, except has a length limit.
 */
char *
#ifndef FUNCPROTO
strnchr(s, c, n)
     char *s;
     int c;
     int n;
#else /* FUNCPROTO */
strnchr(char *s, int c, int n)
#endif /* FUNCPROTO */
{
	while (n--)
		if (*s == c) return s; else ++s;
	return NULL;
}
