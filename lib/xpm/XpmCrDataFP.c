/* Copyright 1990,91 GROUPE BULL -- See licence conditions in file COPYRIGHT */
/*****************************************************************************\
* XpmCrDataFP.c:                                                              *
*                                                                             *
*  XPM library                                                                *
*  Scan a pixmap and possibly its mask and create an XPM array                *
*                                                                             *
*  Developed by Arnaud Le Hors                                                *
\*****************************************************************************/

#include <xite/includes.h>
#include "xpmP.h"
#ifdef VMS
#include "sys$library:string.h"
#else
#include XITE_STRING_H
#endif

int
XpmCreateDataFromPixmap(display, data_return, pixmap, shapemask, attributes)
    Display *display;
    char ***data_return;
    Pixmap pixmap;
    Pixmap shapemask;
    XpmAttributes *attributes;
{
    XImage *image = NULL;
    XImage *shapeimage = NULL;
    unsigned int width = 0;
    unsigned int height = 0;
    int ErrorStatus;
    unsigned int dum;
    int dummy;
    Window win;

    /*
     * get geometry 
     */
    if (attributes && attributes->valuemask & XpmSize) {
	width = attributes->width;
	height = attributes->height;
    } else {
	if (pixmap)
	    XGetGeometry(display, pixmap, &win, &dummy, &dummy,
			 &width, &height, &dum, &dum);
	else if (shapemask)
	    XGetGeometry(display, shapemask, &win, &dummy, &dummy,
			 &width, &height, &dum, &dum);
    }

    /*
     * get the images 
     */
    if (pixmap)
	image = XGetImage(display, pixmap, 0, 0, width, height,
			  AllPlanes, ZPixmap);
    if (shapemask)
	shapeimage = XGetImage(display, shapemask, 0, 0, width, height,
			       AllPlanes, ZPixmap);

    /*
     * create data from images 
     */
    ErrorStatus = XpmCreateDataFromImage(display, data_return, image,
					 shapeimage, attributes);
    if (image)
	XDestroyImage(image);
    if (shapeimage)
	XDestroyImage(shapeimage);

    return (ErrorStatus);
}
