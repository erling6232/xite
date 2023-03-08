/*
 *	Functions for drawing String's with tab characters in them
 */

#ifndef _TABSTRING_H_
#define _TABSTRING_H_

#include <xite/includes.h>
#include <X11/Xfuncproto.h>

extern void	XfwfDrawImageString(Display *display, Drawable drawable,
			GC gc, int x, int y, String string, int length,
			int *tabs);
extern void     XfwfDrawString(Display *display, Drawable drawable, GC gc,
			       int x, int y, String string, int length, 
			       int *tabs);
extern void	XtabDrawString(Display *display, Drawable drawable, GC gc,
			int x, int y, String string, int length, int *tabs);
extern int *	XfwfTablist2Tabs(char *tablist);
extern int	XfwfTextWidth(XFontStruct *font, String str, int length,
			int *tabs);
extern char *	strnchr(char *s, int c, int n);

#endif /* _TABSTRING_H_ */
