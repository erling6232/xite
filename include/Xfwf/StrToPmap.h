#ifndef Xfwf_StrToPmap_h_
#define Xfwf_StrToPmap_h_

#include <xite/includes.h>
#include <X11/Intrinsic.h>

extern XtConvertArgRec  Xfwf_StringToPixmap_Args[];
extern int Xfwf_Num_StringToPixmap_Args;
extern int Xfwf_StringToPixmap_AssumeDefaultVisual;

extern int XfwfLocatePixmapFile ( Screen *screen, Colormap colormap, Visual *visual, char *name, Pixmap *rval, Pixmap *mask, char **name_ret, int *widthp, int *heightp, Pixel **pixels, unsigned int *npixels );
extern Boolean XfwfCvtStringToPixmap( Display *dpy, XrmValue *args, Cardinal *num_args, XrmValue *fromVal, XrmValue *toVal, XtPointer *converter_data );
extern void XfwfInstallStringToPixmapConverter( void );

#endif
