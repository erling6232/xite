

/*

________________________________________________________________

        Visual.c
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

static char *Id = "$Id$";



/*F:visualFromName=Visual*/
/*F:visualFromClass=Visual*/
/*F:visualClassToName=Visual*/
/*F:visualNameToClass=Visual*/
/*F:DepthToReducedPlanes=Visual*/
/*F:DepthOfVisual=Visual*/
/*F:VisualInfoFromClass=Visual*/
/*F:Pseudo8VisualInfo=Visual*/
/*F:Direct24VisualInfo=Visual*/
/*F:True24VisualInfo=Visual*/
/*F:InitializeVisualInfos=Visual*/
/*F:RequestedPlanes=Visual*/
/*F:PlanesToDepth=Visual*/
/*F:RequestedImageVisual=Visual*/
/*F:Visual*

________________________________________________________________

		Visual
________________________________________________________________

Name:		Visual, visualFromName, visualFromClass, visualNameToClass,
                visualClassToName, DepthToReducedPlanes, DepthOfVisual,
		VisualInfoFromClass, Pseudo8VisualInfo, Direct24VisualInfo,
		True24VisualInfo, InitializeVisualInfos, RequestedPlanes,
		PlanesToDepth, RequestedImageVisual - XITE module for
		handling X11 display visuals.

Syntax:         | #include <xite/Visual.h>
		| 
                | Visual *visualFromName( char* name,
                |    int planes );
		|
                | int visualNameToClass( char* name );
		|
                | Visual *visualFromClass( int visualClass,
                |    int planes );
		|
                | char *visualClassToName( int visualClass );
		|
                | XVisualInfo *VisualInfoFromClass(
                |    int visualClass, int planes );
		|
                | int DepthOfVisual( Visual* visual );
		|
                | int DepthToReducedPlanes( int depth,
                |    Visual* visual );
		|
                | int PlanesToDepth( int planes );
                |
                | XVisualInfo *True24VisualInfo( void  );
                |
                | XVisualInfo *Direct24VisualInfo( void  );
                |
                | XVisualInfo *Pseudo8VisualInfo( void  );
                |
                | int RequestedPlanes( XVisualInfo *visualInfo  );
                |
                | void RequestedImageVisual( Display* dpy,
                |    int screen, int* planes, Visual** visual );
                |
                | void InitializeVisualInfos( Display* dpy,
                |    int screen, char* imageVisualClass,
                |    XVisualInfo** imageVisualInfo );
                |
Overview:	The 'Visual' module is used to gather information about the
                visual capabilities of the screen to which XITE X11 display
		programs send their graphical output (e.g.images). Such
		capabilities are characterized by the visual class and depth
		(number of planes of the visual).

		XITE users who wish to make X11 display programs will mostly
		not need to call any of the functions in this module. Refer to
		the XITE 'ximage(3)' toolkit for ways of initializing display
		programs. The toolkit uses functions from the 'Visual' module.

Visuals:        In X11, the visual class can be one of PseudoColor,
		DirectColor, TrueColor, StaticColor, GrayScale or StaticGray,
		but not all X servers or display hardware can support all of
		these. 
		
		One of the supported visuals is denoted the default visual.
		This is the visual type (with corresponding depth) which the X
		server (the display) is set up to use by default. You can find
		out what default visual type your X server is set up to use by
		issuing the Unix command 'xrdb -symbols' in the login window
		on your workstation. Look for the word immediately following
		the text "-DCLASS=". This tells you the visual type. Also
		check the number immediately following the text "-DPLANES=".
		This tells you the depth of the visual.

                You can also find out what other visual types and depths
                your display is capable of using, by issuing the Unix command
                'xdpyinfo'.

		XITE X11 display programs can currently run only on displays
		with at least one of the following capabilities

		|     Visual class      Depth
		|     -----------------------
		|     PseudoColor         8
		|     DirectColor        24
		|     TrueColor          24

		You can either choose to use the default visual if it is among
		the above three, or you can request a preferred visual
		different from the default (but of course still among the
		above three). Whatever your choice is, the chosen visual will
		be called the 'requested' or 'preferred' visual in this
		documentation (and other XITE documentation).

		The requested visual will be used for all image widgets and
		image-related (such as histogram or slice) widgets in the
		application. All textual widgets, such as control panels
		and menus, will use the default visual and depth, regardless
		of which visual was requested.

		The image widgets in an XITE display program may contain a
		menubutton which can be used to create a copy of the original
		image with a different visual. In this way, one may create
		widgets with a different visual than the requested and compare
		images displayed with different visuals at the same time.
		The appearance of the menubutton can be controlled with
		an option and an application resource if the application is
		initialized with an 'ximage' toolkit function. Refer to
		'ximage(3)' for more information.

		XITE display programs were originally developed for PseudoColor
		8-plane visuals. The behaviour may be inferior in some cases
		on the other two visuals.

		For information on how to choose visual type for an XITE
		display program based on the 'ximage' toolkit, refer to
		'ximage_init(3)' or 'ximageInit(3)'.

                For more information about visual types, refer to 'Xlib
                Programming Manual' for version 11, O''Reilly & Associates,
                chapter 7.

Functions:      'InitializeVisualInfos' initializes XITE X11 applications
		in terms of information about the visuals which 'screen'
		on 'dpy' is capable of supporting. 'imageVisualClass' is
		the requested visual for the application. If the global
		variable 'multi_visual_e' is False, only information about
		'imageVisualClass' and the default visual of the display
		will be determined (this may save memory on some X servers).
		The default is needed for non-image widgets.
		'imageVisualClass' must be one of the strings "PseudoColor",
		"DirectColor", "TrueColor" or NULL. In the latter case, choose
		a registered	visual in the order PseudoColor 8-plane,
		DirectColor 24-plane, TrueColor 24-plane.

		'Pseudo8VisualInfo' returns a reference to the registered
		XVisualInfo structure for a PseudoColor visual with depth 8.
		The visual information must have been initialized prior to
		this call.

		'Direct24VisualInfo' returns a reference to the registered
		XVisualInfo structure for a DirectColor visual with depth 24.
		The visual information must have been initialized prior to
		this call.

		'True24VisualInfo' returns a reference to the registered
		XVisualInfo structure for a TrueColor visual with depth 24.
		The visual information must have been initialized prior to
		this call.

		'DepthToReducedPlanes' returns 7 for 8-bit PseudoColor and 21
		for 24-bit DirectColor. Otherwise, returns 'depth'.
		This function reflects that XITE image widgets can
		use a full colortable or a reduced version for display. If a
		full 256-entry colortable is used on an 8-bit PseudoColor
		display, the appearance of other windows on the screen may
		change in terms of color when the mouse pointer is inside the
		image window, depending on the display hardware. This is
		because the window with the mouse-pointer inside determines
		which colormap is installed in the hardware colormap. The
		contents of the hardware colormap determines the colors of all
		the windows on the screen. Some displays have more than one
		hardware colormap, in which case this "technicolor" effect is
		reduced.

		If a reduced colortable is used on an 8-bit PseudoColor
		display, it means that only half of the entries in the
		256-entry colormap is used by the image. Some of the reminaing
		entries	are taken from the default colormap of the screen and
		some are used to enable overlays on the image.

		On a PseudoColor 8-bit display, reduced-color display means
		that an original 256-entry colortable is subsampled to occupy
		only 128 (7 bit) colors. This is why 'DepthToReducedPlanes'
		returns 7 in this cae. On a DirectColor 24-bit display,
		reduced-color display means that each primary is subsampled to
		occupy only 128 entries (three primaries, each with 7 bit
		gives 21 bit). This is why 'DepthToReducedPlanes' returns 21
		in this case. The number of colors in the latter case is
		reduced from approximately 16 million to approximately 2
		million.

		'PlanesToDepth' converts from a reduced-color value (number of
		planes) to a full-color value (depth), i.e. the reverse of
		'DepthToReducedPlanes'.

		The names of the two previous functions reflect that we use
		the term depth about the depth of the visual and the term
		planes about the size of the reduced-color colormap introduced
		by XITE.

		'RequestedPlanes' returns the number of planes for
		reduced-color display on the the visual given by 'visualInfo'.
		If 'visualInfo' is NULL, use the preferred/requested visual.

		'RequestedImageVisual' returns in 'visual' the
		preferred/requested visual. 'planes' returns the number of
		planes for reduced-color display on this visual.
		If '*visual' is zero on entry, don''t update it. Likewise,
		if '*planes' is zero on entry, don''t update it.

		'visualFromName' checks whether 'name' is a legal name for a
		registered visual with XITE-depth 'planes'. If it is, the
		visual is returned. If not, return NULL. If 'name' is NULL,
		choose one of the registered visuals in the following order:
		PseudoColor, DirectColor, TrueColor. If 'planes' is zero,
		don''t consider its value when searching for a visual.

		'visualNameToClass' returns one of the symbols PseudoColor,
		DirectColor, TrueColor, GrayScale or StaticGray for the
		corresponding strings "PseudoColor", "DirectColor",
		"TrueColor", "GrayScale" and "StaticGray" in 'name'.
		If 'name' is neither of these strings, -1 is returned.

		'visualClassToName' is the reverse of 'visualNameToClass'
		except that if 'visualClass' is unknown, NULL is returned.

		'VisualInfoFromClass' returns a reference to an XVisualInfo
		structure. 'visualClass' must be one of the symbols
		PseudoColor, DirectColor, TrueColor, GrayScale or StaticGray.
		If 'planes' is zero, don''t consider its value when searching
		for a visual.

		'visualFromClass' calls 'VisualOfVisualInfo' and returns
		a reference to the Visual structure.

		'DepthOfVisual' calls 'VisualInfoFromClass' with 'planes'
		equal to zero and returns the depth.

See also:	'InitializeVisual(3)', 'xshow(1)', 'ximage(3)', 'ximageInit(3)'

Author:		Svein Bøe

Id: 		$Id$
________________________________________________________________

*/



#include <xite/includes.h>
#include <stdlib.h>
#include <X11/Intrinsic.h>
#include <ctype.h>
#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <xite/Visual.h>
#include <xite/color.h>      /* For RGBMask */
#include <xite/message.h>
#include <xite/debug.h>
#include XITE_STDIO_H

#ifdef MOTIF
#include <Xm/Xm.h> 
#else
#include <X11/StringDefs.h> 
#endif



extern Boolean multi_visual_e;

/* Static variables */

/* List of available visuals for this display and screen and pointers to
 * the various list elements. */
static XVisualInfo *visualList_s,
  *defaultVisInfo_s,          /* Default visual of X server. */
  *pseudo8VisInfo_s,          /* For an 8-bit PseudoColor visual. */
  *direct24VisInfo_s,         /* For a 24-bit DirectColor visual. */
  *true24VisInfo_s,           /* For a 24-bit TrueColor visual. */
  *grayScale8VisInfo_s ,
  *staticGray8VisInfo_s,
  *grayScale4VisInfo_s,
  *staticGray4VisInfo_s,
  *staticGray1VisInfo_s,
  *imageReqVisInfo_s;         /* For globally preferred Image visual. */

static int numVisuals_s = -1; /* Number of visuals in the list. */

/* Planes (not depth) for XITE-supported visuals. */
static int
  imagePlanesDirect24Reduced_s = 21, /* Reduced-color (only cells 64..191). */
  imagePlanesPseudo8Reduced_s  = 7;  /* Reduced-color (only cells 64..191). */



#ifndef FUNCPROTO
static void getVisualInfoList(dpy, screen)
Display *dpy;
int screen;
#else /* FUNCPROTO */
static void getVisualInfoList(Display *dpy, int screen)
#endif /* FUNCPROTO */
{
  XVisualInfo template;
  int i;
  
  ScreenOfVisualInfo(template) = screen;

  /* Find the visuals which this X server can support for the specified
     screen. The match is performed on the attributes of 'template' specified
     by the mask. 'numVisuals_s' returns the number of matching visuals.
     Returns NULL if no match found. */
  visualList_s =
    XGetVisualInfo(dpy, VisualScreenMask, &template, &numVisuals_s);

  if (!numVisuals_s) {
    fprintf(stderr,
	    "Unexpected error: No visuals found for this screen!!!.\n");
    exit(1);
  }
  
  for (i = 0; i < numVisuals_s; i++) {
    FPRINTF2("    Visual class %d", ClassOfVisualInfo(visualList_s[i]));
    FPRINTF2(" with depth %d,", DepthOfVisualInfo(visualList_s[i]));
    FPRINTF2(" bits_per_rgb %d and", BitsPerRgbOfVisualInfo(visualList_s[i]));
    FPRINTF2(" mapsize %d.\n", MapSizeOfVisualInfo(visualList_s[i]));
  }

} /* getVisualInfoList */



#ifndef FUNCPROTO
static void registerVisualInfos(dpy, screen)
Display *dpy;
int screen;
#else /* FUNCPROTO */
static void registerVisualInfos(Display *dpy, int screen)
#endif /* FUNCPROTO */
{
  int i;

  /* Register various supported visuals. */
  for (i=0; i < numVisuals_s; i++) {
    if      (!defaultVisInfo_s &&
	     ClassOfVisualInfo(visualList_s[i])      ==
	     ClassOfVisual(DefaultVisual(dpy, screen)) &&
	     DepthOfVisualInfo(visualList_s[i]) == DefaultDepth(dpy, screen))
    defaultVisInfo_s = &visualList_s[i];
	     
    if      (!pseudo8VisInfo_s &&
	     ClassOfVisualInfo(visualList_s[i])      == PseudoColor &&
	     DepthOfVisualInfo(visualList_s[i])      == 8)
      /*  && BitsPerRgbOfVisualInfo(visualList_s[i]) == 8) */
      pseudo8VisInfo_s = &visualList_s[i];
    else if (!direct24VisInfo_s &&
	     ClassOfVisualInfo(visualList_s[i])      == DirectColor &&
	     DepthOfVisualInfo(visualList_s[i])      == 24          &&
	     BitsPerRgbOfVisualInfo(visualList_s[i]) == 8           &&
	     (RedMaskOfVisualInfo(visualList_s[i])   |
	     GreenMaskOfVisualInfo(visualList_s[i])  |
	     BlueMaskOfVisualInfo(visualList_s[i]))  == RGBMask)
      direct24VisInfo_s = &visualList_s[i];
    else if (!true24VisInfo_s &&
	     ClassOfVisualInfo(visualList_s[i])      == TrueColor   &&
	     DepthOfVisualInfo(visualList_s[i])      == 24) /*          &&
	     BitsPerRgbOfVisualInfo(visualList_s[i]) == 8) */
      true24VisInfo_s = &visualList_s[i];
    else if (!staticGray8VisInfo_s &&
	     ClassOfVisualInfo(visualList_s[i])      == StaticGray  &&
	     DepthOfVisualInfo(visualList_s[i])      == 8) /*           &&
	     BitsPerRgbOfVisualInfo(visualList_s[i]) == 8) */
      staticGray8VisInfo_s = &visualList_s[i];
    else if (!grayScale8VisInfo_s &&
	     ClassOfVisualInfo(visualList_s[i])      == GrayScale   &&
	     DepthOfVisualInfo(visualList_s[i])      == 8) /*           &&
	     BitsPerRgbOfVisualInfo(visualList_s[i]) == 8) */
      grayScale8VisInfo_s = &visualList_s[i];
    else if (!staticGray4VisInfo_s &&
	     ClassOfVisualInfo(visualList_s[i])      == StaticGray  &&
	     DepthOfVisualInfo(visualList_s[i])      == 4) /*           &&
	     BitsPerRgbOfVisualInfo(visualList_s[i]) == 4) */
      staticGray4VisInfo_s = &visualList_s[i];
    else if (!grayScale4VisInfo_s &&
	     ClassOfVisualInfo(visualList_s[i])      == GrayScale   &&
	     DepthOfVisualInfo(visualList_s[i])      == 4) /*           &&
	     BitsPerRgbOfVisualInfo(visualList_s[i]) == 4) */
      grayScale4VisInfo_s = &visualList_s[i];
    else if (!staticGray1VisInfo_s &&
	     ClassOfVisualInfo(visualList_s[i])      == StaticGray  &&
	     DepthOfVisualInfo(visualList_s[i])      == 1) /*           &&
	     BitsPerRgbOfVisualInfo(visualList_s[i]) == 1) */
      staticGray1VisInfo_s = &visualList_s[i];
  } /* for i */

} /* registerVisualInfos */



#ifndef FUNCPROTO
static XVisualInfo *FindRequestedVisualInfo(visualName, planes, visualInfo)
char *visualName;
int planes;
XVisualInfo *visualInfo;
#else /* FUNCPROTO */
static XVisualInfo *FindRequestedVisualInfo(char *visualName, int planes, XVisualInfo *visualInfo)
#endif /* FUNCPROTO */
{
  /* Check if 'visualInfo' is a legal reference to a registered visual-info.
   * If it is, return 'visualInfo'. If not, check whether the visual with name
   * 'visualName' is registered. If it is, return the corresponding
   * visual-info reference with depth given by 'planes'. If not, return NULL.
   * If 'visualName' is NULL, choose a registered visual in the following
   * order: PseudoColor 8-plane, DirectColor 24-plane, TrueColor 24-plane,
   * GrayScale 8-plane, StaticGray 8-plane.

   * With 'planes' == 0, don't use 'planes' when choosing visual-info
   * reference. This "guessing" is correct as long as XITE does not
   * support different depths for the same visual class.
   */

  char *t = visualName;

  if (!((direct24VisInfo_s    && visualInfo == direct24VisInfo_s) ||
	(true24VisInfo_s      && visualInfo == true24VisInfo_s) ||
	(pseudo8VisInfo_s     && visualInfo == pseudo8VisInfo_s) ||
	(grayScale8VisInfo_s  && visualInfo == grayScale8VisInfo_s) ||
	(staticGray8VisInfo_s && visualInfo == staticGray8VisInfo_s) ||
	(grayScale4VisInfo_s  && visualInfo == grayScale4VisInfo_s) ||
	(staticGray4VisInfo_s && visualInfo == staticGray4VisInfo_s) ||
	(staticGray1VisInfo_s && visualInfo == staticGray1VisInfo_s))) {
    
    visualInfo = NULL;

    if      (pseudo8VisInfo_s     && (!t || !strcmp(t, "PseudoColor"))) {
      if (planes == 7  || planes == 8 || planes == 0)
	visualInfo = pseudo8VisInfo_s;
    }
    else if (direct24VisInfo_s    && (!t || !strcmp(t, "DirectColor"))) {
      if (planes == 21 || planes == 24 || planes == 0)
	visualInfo = direct24VisInfo_s;
    }
    else if (true24VisInfo_s      && (!t || !strcmp(t, "TrueColor"))) {
      if (planes == 24 || planes == 0)
	visualInfo = true24VisInfo_s;
    }
    else if (grayScale8VisInfo_s  && (!t || !strcmp(t, "GrayScale"))) {
      if (planes == 8  || planes == 0)
	visualInfo = grayScale8VisInfo_s;
    }
    else if (staticGray8VisInfo_s && (!t || !strcmp(t, "StaticGray"))) {
      if (planes == 8  || planes == 0)
	visualInfo = staticGray8VisInfo_s;
    }
    else if (grayScale4VisInfo_s  && (!t || !strcmp(t, "GrayScale"))) {
      if (planes == 4  || planes == 0)
	visualInfo = grayScale4VisInfo_s;
    }
    else if (staticGray4VisInfo_s && (!t || !strcmp(t, "StaticGray"))) {
      if (planes == 4  || planes == 0)
	visualInfo = staticGray4VisInfo_s;
    }
    else if (staticGray1VisInfo_s && (!t || !strcmp(t, "StaticGray"))) {
      if (planes == 1  || planes == 0)
	visualInfo = staticGray1VisInfo_s;
    }
  } /* if visualInfo not legal. */
  
  return(visualInfo);

} /* FindRequestedVisualInfo */



#ifndef FUNCPROTO
Visual *visualFromName(name, planes)
char *name;
int planes;
#else /* FUNCPROTO */
Visual *visualFromName(char *name, int planes)
#endif /* FUNCPROTO */
{
  XVisualInfo *visualInfo;

  visualInfo = FindRequestedVisualInfo(name, planes, NULL);

  if (visualInfo) return(VisualOfVisualInfo(*visualInfo));
  else return(NULL);
}

#ifndef FUNCPROTO
Visual *visualFromClass(visualClass, planes)
int visualClass, planes;
#else /* FUNCPROTO */
Visual *visualFromClass(int visualClass, int planes)
#endif /* FUNCPROTO */
{
  XVisualInfo *info;

  info = VisualInfoFromClass(visualClass, planes);
  if (!info) return(NULL);

  return(VisualOfVisualInfo(*info));
}

#ifndef FUNCPROTO
int visualNameToClass(name)
char *name;
#else /* FUNCPROTO */
int visualNameToClass(char *name)
#endif /* FUNCPROTO */
{
  if (!strcmp(name, "PseudoColor")) return(PseudoColor);
  if (!strcmp(name, "DirectColor")) return(DirectColor);
  if (!strcmp(name, "TrueColor"))   return(TrueColor);
  if (!strcmp(name, "GrayScale"))   return(GrayScale);
  if (!strcmp(name, "StaticGray"))  return(StaticGray);
  return(-1);
}

#ifndef FUNCPROTO
char *visualClassToName(visualClass)
int visualClass;
#else /* FUNCPROTO */
char *visualClassToName(int visualClass)
#endif /* FUNCPROTO */
{
  switch(visualClass) {
  case PseudoColor: return("PseudoColor");
  case DirectColor: return("DirectColor");
  case TrueColor:   return("TrueColor");
  case GrayScale:   return("GrayScale");
  case StaticGray:  return("StaticGray");
  default: return(NULL);
  }
}


#ifndef FUNCPROTO
XVisualInfo *VisualInfoFromClass(visualClass, planes)
int visualClass, planes;
#else /* FUNCPROTO */
XVisualInfo *VisualInfoFromClass(int visualClass, int planes)
#endif /* FUNCPROTO */
{
  char *name;

  name = visualClassToName(visualClass);
  if (!name) return(NULL);

  return(FindRequestedVisualInfo(name, planes, NULL));
}

#ifndef FUNCPROTO
int DepthToReducedPlanes(depth, visual)
int depth;
Visual *visual;
#else /* FUNCPROTO */
int DepthToReducedPlanes(int depth, Visual *visual)
#endif /* FUNCPROTO */
{
  if (depth == 24 &&
      ClassOfVisual(visual) == DirectColor) {
    /* Start up with only 2 mill. colors in image widget. */
    depth = imagePlanesDirect24Reduced_s;
  } else if (depth == 8 &&
      ClassOfVisual(visual) == PseudoColor) {
    /* Start up with only 128 colors in image widget. */
    depth = imagePlanesPseudo8Reduced_s;
  }

  return(depth);

} /* DepthToReducedPlanes() */

#ifndef FUNCPROTO
int DepthOfVisual(visual)
Visual *visual;
#else /* FUNCPROTO */
int DepthOfVisual(Visual *visual)
#endif /* FUNCPROTO */
{
  int class;
  XVisualInfo *info;

  if (!visual) return(0);

  class = ClassOfVisual(visual);
  info  = VisualInfoFromClass(class, 0);

  if (!info) return(0);

  return(DepthOfVisualInfo(*info));

} /* DepthOfVisual() */



#ifndef FUNCPROTO
XVisualInfo *Pseudo8VisualInfo()
#else /* FUNCPROTO */
XVisualInfo *Pseudo8VisualInfo(void)
#endif /* FUNCPROTO */
{
  return(pseudo8VisInfo_s);
}

#ifndef FUNCPROTO
XVisualInfo *Direct24VisualInfo()
#else /* FUNCPROTO */
XVisualInfo *Direct24VisualInfo(void)
#endif /* FUNCPROTO */
{
  return(direct24VisInfo_s);
}

#ifndef FUNCPROTO
XVisualInfo *True24VisualInfo()
#else /* FUNCPROTO */
XVisualInfo *True24VisualInfo(void)
#endif /* FUNCPROTO */
{
  return(true24VisInfo_s);
}

#ifndef FUNCPROTO
void InitializeVisualInfos(dpy, screen, imageVisualClass, imageVisualInfo)
Display *dpy;
int screen;
char *imageVisualClass;
XVisualInfo **imageVisualInfo;
#else /* FUNCPROTO */
void InitializeVisualInfos(Display *dpy, int screen, char *imageVisualClass, XVisualInfo **imageVisualInfo)
#endif /* FUNCPROTO */
{

  ENTER_FUNCTION_DEBUG("Visual.c: InitializeVisualInfos");

  /* Create the list visualList_s with numVisuals_s entries. */
  getVisualInfoList(dpy, screen);
  if (visualList_s == NULL) {
    fprintf(stderr,
	    "Unexpected error: Display does not support any visual class.\n");
    exit(1);
  }

  /* Set reference variables pseudo8VisInfo_s, direct24VisInfo_s etc. */
  registerVisualInfos(dpy, screen);

  /* Register the globally preferred visuals (perhaps given by application
   * option or resources). */
  imageReqVisInfo_s = FindRequestedVisualInfo(imageVisualClass, 0, NULL);

  if (!imageReqVisInfo_s) {
    fprintf(stderr, "%s\n%s\n",
	    "Error: The requested combination of visual class and depth",
	    "       is not supported.");
    exit(1);
  }

  if (imageVisualInfo) *imageVisualInfo = imageReqVisInfo_s;

  if ((imageReqVisInfo_s != pseudo8VisInfo_s  &&
       imageReqVisInfo_s != direct24VisInfo_s &&
       imageReqVisInfo_s != true24VisInfo_s)) {
    fprintf(stderr, "%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
    "Warning: You have chosen a visual type different from PseudoColor",
    "         8-plane, DirectColor 24-plane and TrueColor 24-plane for",
    "         image windows.",
    "         XITE display programs have not been well tested for the chosen",
    "         visual.",
    "         We'll continue anyway, but be prepared for trouble!",
    "         You may want to consider using the option -iv or the X resource",
    "         imageVisualClass.");
  }

  if (imageReqVisInfo_s == true24VisInfo_s &&
      imageReqVisInfo_s != defaultVisInfo_s) {
    fprintf(stderr, "%s\n%s\n%s\n",
    "Error: You have chosen a TrueColor visual type for the image",
    "       windows. This is only possible if the default visual of the",
    "       screen is TrueColor.");
    exit(1);
  }

  if (!multi_visual_e) {
    /* Throw away everything which does not pertain to imageReqVisInfo_s,
     * except the default visual of the display (the default is needed for
     * non-image widgets). This is especially useful on SGI IRIX 5.3 X11
     * server which becomes very large for DirectColor and/or TrueColor visual
     * colormaps. Avoid this when not needed.
     */

    if (imageReqVisInfo_s    != direct24VisInfo_s    &&
	direct24VisInfo_s    != defaultVisInfo_s)
      direct24VisInfo_s       = NULL;
    if (imageReqVisInfo_s    != true24VisInfo_s      &&
	true24VisInfo_s      != defaultVisInfo_s)
      true24VisInfo_s         = NULL;
    if (imageReqVisInfo_s    != pseudo8VisInfo_s     &&
	pseudo8VisInfo_s     != defaultVisInfo_s)
      pseudo8VisInfo_s        = NULL;
    if (imageReqVisInfo_s    != grayScale8VisInfo_s  &&
	grayScale8VisInfo_s  != defaultVisInfo_s)
      grayScale8VisInfo_s     = NULL;
    if (imageReqVisInfo_s    != staticGray8VisInfo_s &&
	staticGray8VisInfo_s != defaultVisInfo_s)
      staticGray8VisInfo_s    = NULL;
    if (imageReqVisInfo_s    != grayScale4VisInfo_s  &&
	grayScale4VisInfo_s  != defaultVisInfo_s)
      grayScale4VisInfo_s     = NULL;
    if (imageReqVisInfo_s    != staticGray4VisInfo_s &&
	staticGray4VisInfo_s != defaultVisInfo_s)
      staticGray4VisInfo_s    = NULL;
    if (imageReqVisInfo_s    != staticGray1VisInfo_s &&
	staticGray1VisInfo_s != defaultVisInfo_s)
      staticGray1VisInfo_s    = NULL;
  }

  LEAVE_FUNCTION_DEBUG("Visual.c: InitializeVisualInfos");

} /* InitializeVisualInfos() */



#ifndef FUNCPROTO
int RequestedPlanes(visualInfo)
XVisualInfo *visualInfo;
#else /* FUNCPROTO */
int RequestedPlanes(XVisualInfo *visualInfo)
#endif /* FUNCPROTO */
{
  int planes;

  ENTER_FUNCTION_DEBUG("Visual.c: RequestedPlanes");

  if (!visualInfo) *visualInfo = *imageReqVisInfo_s;
  planes = DepthOfVisualInfo(*visualInfo);
  planes = DepthToReducedPlanes(planes,
				VisualOfVisualInfo(*visualInfo));

  FPRINTF1("    For image widgets:\n");
  FPRINTF5("    Visual class= %d, depth=%d, cells=%d, bits_per_rgb=%d\n",
	   ClassOfVisualInfo(*visualInfo),
	   DepthOfVisualInfo(*visualInfo),
	   MapSizeOfVisualInfo(*visualInfo),
	   BitsPerRgbOfVisualInfo(*visualInfo));

  LEAVE_FUNCTION_DEBUG("Visual.c: RequestedPlanes");

  return(planes);

} /* RequestedPlanes() */

#ifndef FUNCPROTO
int PlanesToDepth(planes)
int planes;
#else /* FUNCPROTO */
int PlanesToDepth(int planes)
#endif /* FUNCPROTO */
{
  switch(planes) {
  case 7:  planes++;    break;
  case 21: planes += 3; break;
  case 8:
  case 24:
    break;
  default: {
    fprintf(stderr, "Unsupported value for planes: %d\n", planes);
    exit(1);
  }
    break;
  } /* switch */

  return(planes);

} /* PlanesToDepth() */

#ifndef FUNCPROTO
void RequestedImageVisual(dpy, screen, planes, visual)
Display *dpy;
int screen;
int *planes;
Visual **visual;
#else /* FUNCPROTO */
void RequestedImageVisual(Display *dpy, int screen, int *planes, Visual **visual)
#endif /* FUNCPROTO */
{

  ENTER_FUNCTION_DEBUG("Visual.c: RequestedImageVisual");

  if (imageReqVisInfo_s == NULL)
    InitializeVisualInfos(dpy, screen, NULL, NULL);

  if (visual) {
    /* It is legal to reference '*visual'. */
    if (*visual == NULL) {
      /* Return default value */

      *visual = VisualOfVisualInfo(*imageReqVisInfo_s);
    }
  }
  
  /* Find depth. */
  if (planes) {
    /* It is legal to reference '*planes'. */

    if (*planes == 0) {
      /* Return default value */

      *planes = RequestedPlanes(imageReqVisInfo_s);
    }
  }

  if (planes) {
    FPRINTF2("    Planes=%d\n", *planes);
  }

  LEAVE_FUNCTION_DEBUG("Visual.c: RequestedImageVisual");
}
