#ifndef _icon_h_
#define	_icon_h_

#ifndef NO_XPM

#include <xite/includes.h>
#include <X11/xpm.h>

Boolean cvtStringToIcon(
    Display *dpy,
    XrmValue *args,
    Cardinal *num_args,
    XrmValue *from,
    XrmValue *to,
    XtPointer *converter_data
);

/* The |Icon| type is a convenient combination of a pixmap, a mask and
the pixmaps's attributes. Not all attributes are stored, only width
and height. */

typedef struct _Icon {
    Pixmap pixmap;
    Pixmap mask;
    XpmAttributes attributes;
} Icon;

#endif /* NO_XPM */

#endif
