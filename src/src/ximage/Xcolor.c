

/*

________________________________________________________________

        Xcolor.c
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

static char *Id = "$Id$, Otto Milvang, Blab, UiO";


#include <xite/includes.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/SmeBSB.h>
#include <stdlib.h>
#include <xite/Xcolor.h>
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#include <ctype.h>
#include <X11/Xos.h>
#include <xite/color.h>
#include <xite/ImageOverlay.h>
#include <xite/utils.h>
#include <xite/Visual.h>
#include <xite/Cmap.h>
#include <xite/ShellWids.h>
#ifdef HAVE_MALLOC_H
# include <malloc.h>
#endif
#include <xite/debug.h>

#define MAXCOLORS 64  /* The maximum possible number of entries in the list of
			 colormaps available from within the image widget. */
#define MAXOVERLAY 32 /* The maximum possible number of entries in the list of
			 overlay colormaps available from within the image
			 widget. */

#define MAXCOLSTRLEN 32

/* Structure to store the necessary information for each colormap available
   from within the image widget (by using Shift <Btn1> for "ordinary"
   colormaps or Alt <Btn1> for overlay colormaps). */
typedef struct
{
  Colormap map7,  /* For 128-cells colormap. The 128 elements are actually
		     stored in a 256-element colormap, in the pixel range
		     64..191. */
           map8,  /* For 256-cells colormap */
           map21, /* For 3x7-bit composite colormap */
           map24; /* For 3x8-bit composite colormap */
  char *name;     /* Name which appears in the list available from the image
                   * widget. */
  int foreground;
  int background;
  int partner_pseudo, partner_comp;
} Color;

/* External global variables */

extern Widget colorsmenu_e, overlaysmenu_e;
extern Widget Control_e;

/* Internal global variables */

static Display *dpy_s;
static int     screen_s;
static Visual  *visual_s = NULL;

static int    maxcol_s          = -1; /* One more than the number of
				       * colortables added to the
				       * colortable list 'colortab_s' which is
				       * available from the image widget. */
static int    maxovl_s          = -1; /* Same as 'maxcol_s', but for overlay
				       * colortables in 'overlaytab_s'. */

/* Two arrays with pointers to colormaps available from the image widget. */
static Color  colortab_s[MAXCOLORS], overlaytab_s[MAXOVERLAY];
static SmeBSBObject color_entries_s[MAXCOLORS], overlay_entries_s[MAXOVERLAY];

/* Linearly increasing table of 32 elements from 192 to 223. */
static int tab32_s[] = {  
  0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
  0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
  0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7,
  0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf,
};

/* Linearly increasing table of 128 elements from 64 to 191. */
static int tab128_s[] = {  
  0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
  0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
  0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
  0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
  0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
  0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
  0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
  0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,
  0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
  0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
  0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
  0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,
  0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
  0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
  0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7,
  0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf,
};

/* Linearly increasing table of 256 elements from 0 to 255. */
static int tab256_s[] = {  
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
  0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
  0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
  0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
  0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
  0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
  0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
  0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
  0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
  0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
  0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
  0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
  0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
  0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
  0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
  0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,
  0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
  0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
  0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
  0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,
  0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
  0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
  0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7,
  0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf,
  0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
  0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
  0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7,
  0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf,
  0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7,
  0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef,
  0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
  0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff,
};




/*F:Color_set=ColorsMenuInit*/
/*F:Color_ovl_set=ColorsMenuInit*/
/*F:Color_add=ColorsMenuInit*/
/*F:Color_ovl_add=ColorsMenuInit*/
/*F:Color_read=ColorsMenuInit*/
/*F:Color_ovl_read=ColorsMenuInit*/
/*F:Color_exit=ColorsMenuInit*/
/*F:Color_max=ColorsMenuInit*/
/*F:Color_ovl_max=ColorsMenuInit*/
/*F:Color_map=ColorsMenuInit*/
/*F:Color_map2=ColorsMenuInit*/
/*F:ColortabName=ColorsMenuInit*/
/*F:OvltabName=ColorsMenuInit*/
/*F:Xcolor_deftab=ColorsMenuInit*/
/*F:AddColormapCopy=ColorsMenuInit*/
/*F:Has_changeable_map=ColorsMenuInit*/
/*F:OverlaysMenuInit=ColorsMenuInit*/
/*F:ColorsMenuInit*

________________________________________________________________

		ColorsMenuInit
________________________________________________________________

Name:		ColorsMenuInit, OverlaysMenuInit, Color_set, Color_ovl_set,
                Color_add, Color_ovl_add, Color_read, Color_ovl_read,
		Color_exit, Color_max, Color_ovl_max, Color_map, Color_map2,
		ColortabName, OvltabName, Xcolor_deftab, AddColormapCopy,
		Has_changeable_map - XITE color routines

Syntax:         | #include <xite/Xcolor.h>
		| 
                | void Color_read( char* file_name );
                | void Color_ovl_read( char* file_name );
                | void Color_add( XColor coltab[], char* name,
                |    int len );
                | void Color_ovl_add( XColor coltab[],
                |    char* name, int len );
                | void ColorsMenuInit( Display* display,
                |    int screenno, Widget parent,
                |    char* colorfile );
		| void OverlaysMenuInit( Display *display,
		|    int screenno, Widget parent, char *colorfile );
                | void Color_exit( void  );
                | int Color_set( ImageWidget wid, int color );
                | int Color_ovl_set( ImageWidget wid, int color );
                | int Color_max( void  );
                | int Color_ovl_max( void  );
		| char *ColortabName( int coltab );
		| char *OvltabName( int coltab );
                | int *Xcolor_deftab( int n );
                | Colormap Color_map( int map, int xcolors );
                | Colormap Color_map2( int map, int xcolors,
                |    Visual* visual );
                | void AddColormapCopy( Widget w, Visual* visual,
                |    char* entryName );
                | Boolean Has_changeable_map( Display* dpy,
                |    int screen, Visual* visual );
		|
Description:	'ColorsMenuInit' sets the global variables 'dpy_s' and
		'screen_s' to 'display' and 'screenno' respectively. The
		global variable 'visual_s' (representing the globally
		preferred visual) is set by calling
		'GetImageVisualAndColormap'.

		'ColorsMenuInit' initializes a popup menu for changing colors
		in an Image or ImageOverlay widget. The instance name of the
		menu (for resource specifications) is "colorsmenu".

		'ColorsMenuInit' adds an entry labeled "Background" (widget
		instance name "backgroundcol") as the first entry in the popup
		menu. This entry will activate the default colormap. If the
		image is displayed with a visual different from the default
		visual of the display, then the "Background" entry is
		insensitive (can not be chosen).

		If a visual with changeable colormap exists for the display
		(i.e. PseudoColor or DirectColor) and is registered by the
		application, then a second entry called "Work-map" (widget
		instance name " Work-map") is added to the colortable popup
		menu. This entry will activate a grayscale colortable which is
		made by the routine 'Color_add'.

		Finally, if the application has registered changeable
		colormaps, 'ColorsMenuInit' reads 'colorfile' which contains a
		list of colortables, and adds them, using 'Color_read'.
		Leading "~" and environment variables in 'colorfile' will be
		expanded. If 'colorfile' is not found, try to find it in one
		of the directories listed in the environment variable
		XSHOWPATH.
		
		If 'colorfile' is NULL, use the default filename
		'xshow_colortabs' and search for this in the directories
		listed in the environment variable XSHOWPATH.

		'OverlaysMenuInit' works like 'ColorsMenuInit' except that the
		instance name of the popup menu is "overlaysmenu", the first
		entry''s instance name is "backgroundovl", and the second
		entry''s label and instance name is "  Std-overlay". The popup
		menu is used to change the colors of the overlay image in an
		ImageOverlay widget. The "  Std-overlay" menu entry refers
		to a predefined (hard-coded) overlay colortable with 32
		entries.

		'Color_add' adds the colortable 'coltabIn' to the list of
		colortables in the Image and ImageOverlay widget Colors popup
		menu. The menu entry label and widget instance name are both
		set to 'name'. Actually, two new colortables are produced
		(PseudoColor and DirectColor). One is based on a full-color
		version (256 colors for 8-bit visuals and 16 million colors
		for 24-bit visuals), the other is based on a reduced-color
		version (128 colors + background and overlay colors for 8-bit
		visuals, 2 million colors + background and overlay colors for
		24-bit visuals). Which one of these two are actually chosen by
		selecting a menu entry in the Colors menu, depends on whether
		the resource XtNdisplayMethod indicates full-color or
		reduced-color display.

		The full-color version basis is grayscale. It is fully
		modified by the colortable 'coltabIn' supplied as an argument.

		The reduced-color version basis is the XITE-initial colormap
		described in 'GetImageColormap(3)'. It is modified in the
		pixel value range [64..191] by the colortable supplied as an
		argument. The rgb values corresponding to pixel values
		[0..255] in the full-color version are squeezed into the pixel
		range [64..191] in the reduced-color version by using only
		every second value.

		With NULL 'coltabIn', call the menu entry "Work-map", and make
		the corresponding colortable grayscale.

		'Color_ovl_add' works with colors for the overlay of an
		ImageOverlay widget. One new colortable is produced and added
		as the last entry of the OverlayColors popup menu. With NULL
		'coltabIn', a predefined 32-element overlay colortable called
		"Std-overlay" is put into entries [0..31]. With 'coltabIn' not
		NULL, it will be used. It is assumed that the pixel elements
		of the color-cells in 'coltabIn' are consecutive in the range
		[0..31].

                'Color_set' stores the 'color' argument in the image widget
		XtNcolortab resource and sets the corresponding colormap for
		the image window. The colormap is chosen depending on the
		value of the image widget''s displaymethod (reduced-color map
		of 128 colors for PSEUDO_COLOR_8A (PseudoColor 8-bit visual),
		full-color map of 256 colors for PSEUDO_COLOR_8B (PseudoColor
		8-bit visual), reduced-color map of approximately 2 million
		colors for DIRECT_COLOR_24_A (DirectColor 24-bit visual),
		full-color map of approximately 16 million colors for
		DIRECT_COLOR_24_B (DirectColor 24-bit visual)).

		'Color_ovl_set' sets XtNoverlayColorCells (Image widget
		resource) colors, taken from the 8-bit PseudoColor or 24-bit
		DirectColor colormap corresponding to overlay colortable
		'ovlcolor', starting with pixel value 0, into the widget''s
		active colormap identified by XtNcolortab, starting with pixel
		value 192. The XtNoverlaytab resource is set equal to
		'ovlcolor' and the previous value of XtNoverlaytab is
		returned.

		'Color_read' expands 'file_name' (interpreting csh ~) and
		reads a BIFF colortable from file, adds it to the Colors popup
		menu for the Image and ImageOverlay widget. It uses the routine
		'Color_add'. If the filename starts with the character ''+'',
		the routine simply calls 'Color_ovl_read'. 'file_name' is
		first searched for in the directory given by the environment
		variable XSHOWPATH.

		'Color_ovl_read' behaves like 'Color_add', but the colortable
		is added as the last entry of the OverlayColors popup menu for
		ImageOverlay widgets. It uses the routine 'Color_ovl_add'.

		'Color_exit' is an empty function.

		'Color_max' returns (one less than) the number of colors in
		the list of colortables available from the Image and
		ImageOverlay widget Colors popup menu. It is the index to the
		last colortable added.

		'Color_ovl_max' returns (one less than) the number of colors
		in the list of overlay colortables available from the
		ImageOverlay widget OverlayColors popup menu. It is the index
		to the last colortable added.

		'Color_map2' checks whether the 'visual' has changeable
		colormaps and if so, uses 'map' as an index into the Image and
		ImageOverlay Colors popup menu to return the 128- or
		256-element colormap, depending on the value of 'xcolors'
		(which should be 2, 128 or 256). If 'xcolors' is zero, use the
		number of colormap entries of 'visual'. If this is 256, use
		128 (i.e. use "reduced-colors" display as default). If
		'visual' does not have changeable colormaps or if 'xcolors'
		(or the number of colormap entries) equals 2, return NULL.

		'Color_map' behaves like 'Color_map2' except that the globally
		preferred visual is used instead of 'visual'.

		'Xcolor_deftab' is obsolete since XITE version 3.3, but is
		retained for backwards compatibility. Since version 3.3,
		setting default values for the resources XtNcolorPtr and
		XtNoverlayColorPtr is taken care of by the Image widget and
		ImageOverlayWidget respectively.

		'Xcolor_deftab' returns a pointer to an integer array with 32,
		128 or 256 consecutive integer elements, depending on the
		argument 'n'. 'n' equal to 32 returns the table 192,...,223.
		'n' equal to 12 returns the table 64,...,191. 'n' equal to
		256 returns the table 0,...,255. Illegal 'n' returns NULL.
		The 32-element table is used as XtNoverlayColorPtr resource
		for ImageOverlay widgets. The 128- and 256-element tables
		are used as XtNcolorPtr resources for Image or ImageOverlay
		widgets (128 for reduced-color display, 256 for full-color
		display).

		'AddColormapCopy' adds the full- and reduced-color versions of
		the current colortable (as given by the XtNcolortab Image
		widget resource) to the Colors popup menu, with name given by
		'entryName'.

		'Has_changeable_map' checks whether 'visual' has changeable
		colormaps. 'dpy' and 'screen' are not used.

		'ColortabName' returns the name of entry number 'coltab' in
		the "Colors" menu in the image widget.

		'OvltabName' returns the name of entry number 'coltab' in
		the "OverlayColors" menu in the image widget.

Author:		Otto Milvang and Svein Bøe

Doc:            Svein Bøe

Id: 		$Id$
________________________________________________________________

*/

void AddColormapCopy(Widget w, Visual *visual, char *entryName)
{
  /* Add the full- and reduced-color versions of the current
   * colortable to the colortable menu, with name given by "entryName".
   */

  XColor col[256];
  Colormap cmap;
  int color;

  ENTER_FUNCTION_DEBUG("Xcolor.c: AddColormapCopy");

  InitColortablePixels(col, 256, 0,
		       ClassOfVisual(visual) == DirectColor ||
		       ClassOfVisual(visual) == TrueColor);
  
  /* Get the full-color version of the currently used colormap. */
  XtVaGetValues(w, XtNcolortab, &color, NULL);
  cmap = Color_map2(color, 256, visual);
  
  /* Get the rgb values from 'cmap', for all the 256 elements. */
  XQueryColors(dpy_s, cmap, col, DisplayCells(dpy_s, screen_s));
  
  /* Add the full- and reduced-color maps to the colortable menu entry with
   * name give by "entryName".
   */

  Color_add(col, entryName, DisplayCells(dpy_s, screen_s));

  LEAVE_FUNCTION_DEBUG("Xcolor.c: AddColormapCopy");

} /* AddColormapCopy() */

int *Xcolor_deftab(int n)
{
  int *tab = NULL;

  switch(n) {
  case 32:  tab = tab32_s;  break;
  case 128: tab = tab128_s; break;
  case 256: tab = tab256_s; break;
  default:  tab = NULL;     break;
  }

  return(tab);
}

void Color_exit(void) {}

Boolean Has_changeable_map(Display *dpy, int screen, Visual *visual)
{
  int color_count;

  color_count = MapEntriesOfVisual(visual);
  return(color_count != 2 && ClassOfVisual(visual) != StaticGray && 
	     ClassOfVisual(visual) != TrueColor &&
	     ClassOfVisual(visual) != StaticColor);
}



int Color_set(ImageWidget wid, int color)
{
  int old_col;
  display_method displaymethod;
  Window image_window, top_window;
  Widget top_wid;

  ENTER_FUNCTION_DEBUG("Xcolor.c: Color_set");

  XtVaGetValues((Widget) wid, XtNcolortab,      &old_col,
		              XtNdisplayMethod, &displaymethod, NULL);

  if (!ImageHasColormap(wid)) return(old_col);

  XtVaSetValues((Widget) wid, XtNcolortab, color, NULL);

  FPRINTF2("    displaymethod: %08x.\n", displaymethod);
  image_window = XtWindow((Widget) wid);
  top_wid      = TopLevelWidget((Widget) wid);
  top_window   = XtWindow(top_wid);

  if (top_window) {

      switch(displaymethod)
	{
	case 1: break;
	case PSEUDO_COLOR_8_A:
	  /* The colormap must have the same visual type as the window,
	     or a BadMatch error results. */

	  /* 128 colors + overlay colors. */
	  XSetWindowColormap(dpy_s,image_window,colortab_s[color].map7);
	  if (top_wid == Control_e)
	    /* If the image window is not a separate popup. */
	    XSetWindowColormap(dpy_s,top_window,colortab_s[color].map7);
	  break;
	case PSEUDO_COLOR_8_B:
	  /* 256 colors. */
	  XSetWindowColormap(dpy_s,image_window,colortab_s[color].map8); 
	  if (top_wid == Control_e)
	    XSetWindowColormap(dpy_s,top_window,colortab_s[color].map8); 
	  break;
	case DIRECT_COLOR_24_A:
	  /* 128-entry composite colormap + overlay colors. */
	  XSetWindowColormap(dpy_s,image_window,colortab_s[color].map21); 
	  if (top_wid == Control_e)
	    XSetWindowColormap(dpy_s,top_window,colortab_s[color].map21); 
	  break;
	case DIRECT_COLOR_24_B:
	  /* 256-entry composite colormap. */
	  XSetWindowColormap(dpy_s,image_window,colortab_s[color].map24); 
	  if (top_wid == Control_e)
	    XSetWindowColormap(dpy_s,top_window,colortab_s[color].map24); 
	  break;
	default:
	  break;
	}
    } 

  LEAVE_FUNCTION_DEBUG("Xcolor.c: Color_set");

  return(old_col);
}



static void ColorsSelect(Widget w, XtPointer closure, XtPointer call_data)
{
  int i1;
  ImageWidget iw;

  ENTER_FUNCTION_DEBUG("Xcolor.c: ColorsSelect");

  i1 = *((int *) closure);
/*  free(closure); */
 
  iw = (ImageWidget) PopupWidget();
  Color_set(iw, i1);

  FPRINTF2("    Install colortab %d\n", i1);
  LEAVE_FUNCTION_DEBUG("Xcolor.c: ColorsSelect");
}



/*L:ColorsHook*

________________________________________________________________

		ColorsHook
________________________________________________________________

Name:		ColorsHook - 
Description:	'ColorsHook' sets or removes an 'x' in front of the label of
		each entry in the colormenu available from the Image and
		ImageOverlay widgets. The 'x' is set if the entry is the
		active colormap for this image, it is removed for all other
		entries. 'ColorsHook' is typically used as an action function
		when popping up the colormenu.
Id: 		$Id$
________________________________________________________________

*/


#define MARKIF(e) (e) ? 'x' : ' '

static void ColorsHook(Widget wid, XEvent *event, String *params, Cardinal *num_params)
{
  ImageWidget iw;
  char *label;
  int entryInd, color;
  Visual *visual;
  Boolean sensitive;

  ENTER_FUNCTION_DEBUG("Xcolor.c: ColorsHook");

  iw = (ImageWidget) ImageFromMenu(wid);
  XtVaGetValues((Widget) iw, XtNcolortab, &color, XtNvisual, &visual, NULL);

  for (entryInd = 0; entryInd <= maxcol_s; entryInd++) {
    XtVaGetValues((Widget) color_entries_s[entryInd],
		  XtNlabel, &label, NULL);
    if (entryInd != color)
      label[0] = MARKIF(FALSE);
    else
      label[0] = MARKIF(TRUE);

    sensitive = ImageHasColormap(iw) ? 1 : 0;

    XtVaSetValues((Widget) color_entries_s[entryInd], XtNlabel, label,
		  XtNsensitive, sensitive, NULL);
  }

  if (maxcol_s >= 0) {
    if (ClassOfVisual(DefaultVisual(dpy_s, screen_s)) !=
	ClassOfVisual(visual))
      XtVaSetValues((Widget) color_entries_s[0], XtNsensitive, 0, NULL);
    else
      XtVaSetValues((Widget) color_entries_s[0], XtNsensitive, 1, NULL);
  }

  LEAVE_FUNCTION_DEBUG("Xcolor.c: ColorsHook");
}

char *ColortabName(int coltab)
{

  if (coltab < 0 || coltab > Color_max()) return(NULL);

  return(colortab_s[coltab].name);

} /* ColortabName() */

char *OvltabName(int coltab)
{

  if (coltab < 0 || coltab > Color_ovl_max()) return(NULL);

  return(overlaytab_s[coltab].name);

} /* OvltabName() */

static char *NewColortabName(XColor *coltab, char *name, int len)
{
  char *ptr;
  
  /* Construct name of colormap for menu entry. */
  if (coltab) {
    int i;

    ptr = colortab_s[maxcol_s].name =
      (char *) calloc(sizeof(char), MAXCOLSTRLEN + 1);

    (void) strcpy(ptr, "  ");
    if (strlen(name) <= MAXCOLSTRLEN - 2)
      (void) strcat(ptr, name);
    else {
      (void) strncat(ptr, name, MAXCOLSTRLEN - 2);
      ptr[MAXCOLSTRLEN] = '\0';
    }
    
    /* Remove trailing spaces */
    i=MAXCOLSTRLEN-1;
    while(i >= 0 && (isspace(ptr[i]) || ptr[i] == '\0'))
      ptr[i--] = '\0';

    /* Construct default name if only spaces found */
    if (!strlen(ptr)) sprintf(ptr,"  Colortab #%d", maxcol_s);
    
    FPRINTF2("    Add menu entry |%s|.\n", ptr);
  } else {
    /* Make grayscale colormap further down. */
    ptr = colortab_s[maxcol_s].name = (char *) malloc(strlen("  Work-map")+1);
    (void) strcpy(ptr, "  Work-map");
  }

  return(ptr);
} /* NewColortabName */



static void AddFullColor(XColor *coltabIn, XColor *coltabOut, int len, Visual *visual)
{
  int cells;
  Colormap cmap;

  ENTER_FUNCTION_DEBUG("Xcolor.c: AddFullColor");

  if (!visual) visual = visual_s;

  /* "Full-color" XITE-initial colormap for visual. */
  cmap = cmapOfVisual(visual, DepthOfVisual(visual));

  if (!cmap) {
    /* Failed in determining colormap. */
    FPRINTF1("    Failed in getting colormap.\n");
    return;
  }

  /* Number of cells in default colormap */
  cells = DisplayCells(dpy_s, screen_s);

  /* Initialize colortable before querying the present colormap */
  InitColortablePixels(coltabOut, cells, 0,
		       ClassOfVisual(visual) == DirectColor ||
		       ClassOfVisual(visual) == TrueColor);
 
  /* Find the rgb values in the XITE-initial colormap 'cmap', for all the
   * pixels in the 'col' colortable */
  XQueryColors(dpy_s, cmap, coltabOut, cells);

  if (ClassOfVisual(visual) == PseudoColor ||
      ClassOfVisual(visual) == GrayScale) {
    
    /* Create map8 virtual colormap, to be filled in with content, starting out
     * with the XITE-initial colormap, then modified by the supplied colormap
     * argument, if any, or filled in with grayscale values.
     */
    colortab_s[maxcol_s].map8 =
      XCreateColormap(dpy_s, DefaultRootWindow(dpy_s), visual, AllocAll);

  } else if (ClassOfVisual(visual) == DirectColor) {
    
    /* Create map24 virtual colormap, to be filled in with content, starting
     * out with the present XITE-initial colormap, then modified by the
     * supplied colormap argument, if any, or filled in with grayscale values.
     * */
    colortab_s[maxcol_s].map24 =
      XCreateColormap(dpy_s, DefaultRootWindow(dpy_s), visual, AllocAll);
  }
  
  if (coltabIn) {
    int y, pix;

    /* A colortable of specified length was supplied as an argument to
     * this function. Modify the rgb values in 'coltabOut'.
     *
     * Store the incoming colortable cells into their appropriate cells in
     * 'coltabOut'. 'coltabIn' may be shorter than 'coltabOut' (and possibly
     * also not sorted, although it is not likely). If 'coltabIn' is shorter
     * than 'coltabOut', not all cells in 'coltabOut' receive new values.
     * 'coltabOut' will get colorcells from 'coltabIn' "sprinkled" throughout,
     * in one or more segments or completely filled.
     */
    if (ClassOfVisual(visual) == PseudoColor ||
	ClassOfVisual(visual) == GrayScale)
      for (y=1; y <= len; y++) {
	pix = PseudoBasePart(coltabIn[y-1].pixel);
	coltabOut[pix].red   = coltabIn[y-1].red;
	coltabOut[pix].green = coltabIn[y-1].green;
	coltabOut[pix].blue  = coltabIn[y-1].blue;
      }
    else if (ClassOfVisual(visual) == DirectColor ||
	     ClassOfVisual(visual) == TrueColor)
      for (y=1; y <= len; y++) {
	pix = RedBasePart(coltabIn[y-1].pixel);
	coltabOut[pix].red   = coltabIn[y-1].red;

	pix = GreenBasePart(coltabIn[y-1].pixel);
	coltabOut[pix].green = coltabIn[y-1].green;

	pix = BlueBasePart(coltabIn[y-1].pixel);
	coltabOut[pix].blue  = coltabIn[y-1].blue;
      }
  }

  /* "Hardware-approximate" the colortable 'coltabOut', which may have been
   * modified by the incoming colortable, into the newly created global map8
   * or map24 virtual colormap.
   */
  if (ClassOfVisual(visual) == PseudoColor ||
      ClassOfVisual(visual) == GrayScale)
    XStoreColors(dpy_s, colortab_s[maxcol_s].map8, coltabOut, cells);
  else if (ClassOfVisual(visual) == DirectColor)
    XStoreColors(dpy_s, colortab_s[maxcol_s].map24, coltabOut, cells);

  LEAVE_FUNCTION_DEBUG("Xcolor.c: AddFullColor");

} /* AddFullColor() */



static void AddReducedColor(XColor *coltabIn, XColor *colFull, XColor *colReduced, int len, Visual *visual)
{
  int cells;
  Colormap cmap;

  ENTER_FUNCTION_DEBUG("Xcolor.c: AddReducedColor");

  if (!visual) visual = visual_s;

  /* Create map7 virtual colormap, to be filled in with content. */
  if (ClassOfVisual(visual) == PseudoColor ||
      ClassOfVisual(visual) == GrayScale) {
    colortab_s[maxcol_s].map7  =
      XCreateColormap(dpy_s, DefaultRootWindow(dpy_s), visual, AllocAll);
  }
  else if (ClassOfVisual(visual) == DirectColor) {
    colortab_s[maxcol_s].map21 =
      XCreateColormap(dpy_s, DefaultRootWindow(dpy_s), visual, AllocAll);
  }

  /* Number of cells in default colormap */
  cells = DisplayCells(dpy_s, screen_s);

  InitColortablePixels(colReduced, cells, 0,
		       ClassOfVisual(visual) == DirectColor ||
		       ClassOfVisual(visual) == TrueColor);

  /* Find the rgb values in the XITE-initial "reduced-color" colormap
   * for all the pixels in the 'colReduced' colortable.
   */
  cmap = cmapOfVisual(visual,
		      DepthToReducedPlanes(DepthOfVisual(visual), visual));

  if (!cmap) {
    /* Failed in determining colormap. */
    FPRINTF1("    Failed in getting colormap.\n");
    return;
  }
 
  XQueryColors(dpy_s, cmap, colReduced, cells);

  /* Spread the 128 pixel values 64..191 in steps of 2 over the range 0..254
   * in 'colFull'. Copy these (including previous 'colFull' rgb values, which
   * may have been modified by the incoming colortable) into the range 64..191
   * in 'colReduced'.
   */
  if (ClassOfVisual(visual) == DirectColor ||
      ClassOfVisual(visual) == TrueColor) {
    int i;

    for (i=0; i<256; i++, i++) {       /* Go in steps of 2, from 0 to 254 */
      colFull[i].pixel = RgbPixel(64+i/2); /* 64 <= 64+i/2 <= (64+127)=191 */

      if (coltabIn) colReduced[64+i/2] = colFull[i];
    }
  } else if (Has_changeable_map(dpy_s, screen_s, visual)) {
    int i;

    for (i=0; i<256; i++, i++) {       /* Go in steps of 2, from 0 to 254 */
      colFull[i].pixel = 64+i/2;           /* 64 <= 64+i/2 <= (64+127)=191 */

      if (coltabIn) colReduced[64+i/2] = colFull[i];
    }
  }

  /* 'colReduced' is now a colortable with linearly increasing pixel values
   * from 0 to 255 (for each rgb component in the case of a 24-bit DirectColor
   * visual). In the two intervals 0..63 and 192..255, the rgb values have
   * been collected from the XITE-initial "reduced-color" colormap. In the
   * middle interval, 64..191, the rgb values have been collected from the
   * modified colortable 'colFull'. In other words, whereas all of 'colFull'
   * may have been modified by the supplied colortable, only the central
   * 128-cells part of 'colReduced' may have been modified. */
     
  /* "Hardware-approximate" the colortable 'colReduced' into the last global
   * map7 or map21 colormap.
   */
  if (ClassOfVisual(visual) == PseudoColor ||
      ClassOfVisual(visual) == GrayScale)
    XStoreColors(dpy_s, colortab_s[maxcol_s].map7, colReduced, cells);

  else if (ClassOfVisual(visual) == DirectColor)
    XStoreColors(dpy_s, colortab_s[maxcol_s].map21, colReduced, cells);

  LEAVE_FUNCTION_DEBUG("Xcolor.c: AddReducedColor");

} /* AddReducedColor() */



void Color_add(XColor *coltabIn, char *name, int len)
{
  XColor colFull[256], colReduced[256];
  char* ptr;
  Visual *pseudo_vis, *direct_vis;

  ENTER_FUNCTION_DEBUG("Xcolor.c: Color_add");

  pseudo_vis = visualFromName("PseudoColor", 0);
  direct_vis = visualFromName("DirectColor", 0);

  if (!pseudo_vis && !direct_vis) {
    LEAVE_FUNCTION_DEBUG("Xcolor.c: Color_add");
    return;
  }

  maxcol_s++ ; /* Global count of the number of colors */
  if (maxcol_s > MAXCOLORS) {
    Warning(1, "Too many colortables.\n");
    maxcol_s--;

    return;
  }

  if (maxcol_s == 2) PopupLine("colorsep", colorsmenu_e);

  /* Return the name stripped of trailing blanks, or default name "Work-map".
     */
  ptr = NewColortabName(coltabIn, name, len);

  /* Add colortable name to the colortable menu list in the image widget. */
  color_entries_s[maxcol_s] =
    (SmeBSBObject) PopupAdd(ptr, colorsmenu_e, ptr, ColorsSelect, maxcol_s);

  if (pseudo_vis && Has_changeable_map(dpy_s, screen_s, pseudo_vis)) {
    AddFullColor(coltabIn, colFull, len, pseudo_vis);
    AddReducedColor(coltabIn, colFull, colReduced, len, pseudo_vis);

    /* All new 7-bit pseudo colortables have overlay colors initialized to a
     * fixed set. */
    colortab_s[maxcol_s].partner_pseudo = 1;
  }
  if (direct_vis && Has_changeable_map(dpy_s, screen_s, direct_vis)) {
    AddFullColor(coltabIn, colFull, len, direct_vis);
    AddReducedColor(coltabIn, colFull, colReduced, len, direct_vis);

    /* All new 21-bit composite colortables have overlay colors initialized to
     * a fixed set. */
    colortab_s[maxcol_s].partner_comp = 1;
  }

  LEAVE_FUNCTION_DEBUG("Xcolor.c: Color_add");

} /* Color_add() */

void Color_read(char *file_name)
{
 char *name = NULL, *filename = NULL;
 int len;
 XColor tab[256];

 ENTER_FUNCTION_DEBUG("Xcolor.c: Color_read");

 if (file_name != NULL && file_name[0] == '+') {
   Color_ovl_read(&file_name[1]);
 } else {
   filename = envfindfile("XSHOWPATH", file_name);
   if (filename == NULL) filename = tilde_expand(file_name);
   if (filename) {
     if (color_read(filename, (Color_cell *) tab, &name, &len) == Iok) 
       Color_add(tab, name, len);
     else Message(2,
		  "Color_read: Failure in reading colortable from file %s.\n",
		  filename);
     if (filename) free(filename);
     if (name)     free(name);
   }
   else Message(2, "Color_read: Could not find colortable file.\n");
 }

 LEAVE_FUNCTION_DEBUG("Xcolor.c: Color_read");
}

int Color_max(void)
{
  return(maxcol_s);
}

Colormap Color_map2(int map, int xcolors, Visual *visual)
{
  Colormap cmap;

  ENTER_FUNCTION_DEBUG("Xcolor.c: Color_map2");

  if (!Has_changeable_map(dpy_s, screen_s, visual)) {
    FPRINTF1("    This visual does not have changeable colormaps.\n");
    LEAVE_FUNCTION_DEBUG("Xcolor.c: Color_map2");
    return(DefaultColormap(dpy_s, screen_s));
  }

  if (xcolors == 0) {
    xcolors = MapEntriesOfVisual(visual);
    if (xcolors == 256) xcolors = 128;
    if (xcolors > 128 && xcolors < 256) xcolors = 128;
  }

  switch(xcolors) {
  case 2:   {
	FPRINTF2("    xcolors: %d.\n", xcolors);
        LEAVE_FUNCTION_DEBUG("Xcolor.c: Color_map2");
	cmap = (Colormap) NULL;
	break;
      }
  case 128: {
    FPRINTF2("    xcolors: %d.\n", xcolors);
    LEAVE_FUNCTION_DEBUG("Xcolor.c: Color_map2");
    if (ClassOfVisual(visual) == PseudoColor)
      cmap = colortab_s[map].map7;
    else
      cmap = colortab_s[map].map21;
    break;
  }
  case 256: {
    FPRINTF2("    xcolors: %d.\n", xcolors);
    LEAVE_FUNCTION_DEBUG("Xcolor.c: Color_map2");
    if (ClassOfVisual(visual) == PseudoColor)
      cmap = colortab_s[map].map8;
    else
      cmap = colortab_s[map].map24;
    break;
  }
  } /* switch */

  return(cmap);

} /* Color_map2() */

Colormap Color_map(int map, int xcolors)
{
  ENTER_FUNCTION_DEBUG("Xcolor.c: Color_map");
  LEAVE_FUNCTION_DEBUG("Xcolor.c: Color_map");

  return(Color_map2(map, xcolors, visual_s));

} /* Color_map() */

int Color_ovl_set(ImageWidget wid, int ovlcolor)
{
  XColor col[256];
  int old_ovl, i, color, ovl_cells;
  Visual *visual;

  if (!XtIsSubclass((Widget) wid, imageOverlayWidgetClass)) return(-1);

  ENTER_FUNCTION_DEBUG("Xcolor.c: Color_ovl_set");

  XtVaGetValues((Widget) wid, XtNcolortab,          &color,
		              XtNoverlaytab,        &old_ovl,
		              XtNoverlayColorCells, &ovl_cells,
		              XtNvisual,            &visual,    NULL);

  if (color == 0 && ovlcolor != old_ovl) {
    Message(1, "%s\n%s\n%s\n",
	    "Warning: Can not change default colormap.",
	    "         Choose another non-overlay colormap before",
	    "         trying to change the overlay colors.");
    return(old_ovl);
  } else if (color == 0 && ovlcolor == old_ovl) return(old_ovl);

  XtVaSetValues((Widget) wid, XtNoverlaytab, ovlcolor, NULL);

  InitColortablePixels(col, ovl_cells, 0,
		       ClassOfVisual(visual) == DirectColor ||
		       ClassOfVisual(visual) == TrueColor);

  if (ClassOfVisual(visual) == DirectColor) {
    XQueryColors(dpy_s, overlaytab_s[ovlcolor].map24, col, ovl_cells);

    for (i=0; i < ovl_cells; i++) col[i].pixel = RgbPixel(192+i);
    XStoreColors(dpy_s, colortab_s[color].map21, col, ovl_cells);

    colortab_s[color].partner_comp = ovlcolor;
  } else if (ImageHasColormap(wid)) {
    XQueryColors(dpy_s, overlaytab_s[ovlcolor].map8, col, ovl_cells);

    for (i=0; i < ovl_cells; i++) col[i].pixel = 192+i;
    XStoreColors(dpy_s, colortab_s[color].map7, col, ovl_cells);

    colortab_s[color].partner_pseudo = ovlcolor;
  }



  LEAVE_FUNCTION_DEBUG("Xcolor.c: Color_ovl_set");

  return(old_ovl);
}

static void OverlaysSelect(Widget w, XtPointer closure, XtPointer call_data)
{
  int i1;
  ImageWidget iw;

  ENTER_FUNCTION_DEBUG("Xcolor.c: OverlaysSelect");

  i1 = *((int *) closure);
/*  free(closure); */
  iw = (ImageWidget) PopupWidget();
  Color_ovl_set(iw, i1);

  FPRINTF2("    Install overlay colortab %d\n", i1);
  LEAVE_FUNCTION_DEBUG("Xcolor.c: OverlaysSelect");
}



/*L:OverlaysHook*

________________________________________________________________

		OverlaysHook
________________________________________________________________

Name:		OverlaysHook - 
Description:	'OverlaysHook' sets or removes an 'x' in front of the label of
		each entry in the overlay colormenu available from the
		ImageOverlay widgets. The 'x' is set if the entry is the
		active overlay colormap for this image, it is removed for all
		other entries. The active overlay colormap depends on the
		active non-overlay colormap. 'OverlaysHook' is typically used
		as an action function when popping up the overlay colormenu.
Id: 		$Id$
________________________________________________________________

*/

static void OverlaysHook(Widget wid, XEvent *event, String *params, Cardinal *num_params)
{
  ImageWidget iw;
  char *label;
  int entryInd, color, ovl_col;
  Visual *visual;
  Boolean sensitive;

  ENTER_FUNCTION_DEBUG("Xcolor.c: OverlaysHook");

  iw = (ImageWidget) ImageFromMenu(wid);

  XtVaGetValues((Widget) iw, XtNcolortab, &color, XtNvisual, &visual, NULL);

  if (ClassOfVisual(visual) == PseudoColor)
    ovl_col = colortab_s[color].partner_pseudo;
  else if (ClassOfVisual(visual) == DirectColor ||
	   ClassOfVisual(visual) == TrueColor)
    ovl_col = colortab_s[color].partner_comp;

  for (entryInd = 0; entryInd <= maxovl_s; entryInd++) {
    XtVaGetValues((Widget) overlay_entries_s[entryInd],
		  XtNlabel, &label, NULL);

    if (entryInd != ovl_col)
      label[0] = MARKIF(FALSE);
    else
      label[0] = MARKIF(TRUE);

    sensitive = (ImageHasColormap(iw) &&
		 XtIsSubclass((Widget) iw, imageOverlayWidgetClass)) ? 1 : 0;

    XtVaSetValues((Widget) overlay_entries_s[entryInd], XtNlabel, label,
		  XtNsensitive, sensitive, NULL);
  }

  if (maxovl_s >= 0) {
    if (ClassOfVisual(DefaultVisual(dpy_s, screen_s)) !=
	ClassOfVisual(visual))
      XtVaSetValues((Widget) overlay_entries_s[0], XtNsensitive, 0, NULL);
    else if (XtIsSubclass((Widget) iw, imageOverlayWidgetClass))
      XtVaSetValues((Widget) overlay_entries_s[0], XtNsensitive, 1, NULL);
  }

  LEAVE_FUNCTION_DEBUG("Xcolor.c: OverlaysHook");
}



static char *NewColorOvltabName(XColor *coltab, char *name, int len)
{
  char *ptr;
  
  /* Construct name of colormap for menu entry. */
  if (coltab) {
    int i;

    ptr = overlaytab_s[maxovl_s].name =
      (char *) calloc(sizeof(char), MAXCOLSTRLEN + 1);

    (void) strcpy(ptr, "  ");
    if (strlen(name) <= MAXCOLSTRLEN - 2)
      (void) strcat(ptr, name);
    else {
      (void) strncat(ptr, name, MAXCOLSTRLEN - 2);
      ptr[MAXCOLSTRLEN] = '\0';
    }

    /* Remove trailing spaces */
    i=MAXCOLSTRLEN-1;
    while(i >= 0 && (isspace(ptr[i]) || ptr[i] == '\0'))
      ptr[i--] = '\0';

    /* Construct default name if only spaces found */
    if (!strlen(ptr)) sprintf(ptr,"  Overlaytab #%d", maxovl_s);
    
    FPRINTF2("    Add menu entry |%s|.\n", ptr);
  } else {
    /* Make grayscale colormap further down. */
    ptr = overlaytab_s[maxovl_s].name =
      (char *) malloc(strlen("  Std-overlay")+1);
    (void) strcpy(ptr, "  Std-overlay");
  }

  return(ptr);

} /* NewColorOvltabName */



static void AddOvlColor(XColor *coltabIn, XColor *coltabOut, int len, Visual *visual)
{
  Colormap cmap;
  int y;

  ENTER_FUNCTION_DEBUG("Xcolor.c: AddOvlColor");

  if (!visual) visual = visual_s;
  if (len < 0) len = 32;

  /* Create virtual colormap, to be filled in with content, starting out with
   * the XITE-initial colormap, then modified by the supplied colormap
   * argument, if any. */

  if (ClassOfVisual(visual) == PseudoColor ||
      ClassOfVisual(visual) == GrayScale) {
    
    overlaytab_s[maxovl_s].map8 =
      XCreateColormap(dpy_s, DefaultRootWindow(dpy_s), visual, AllocAll);

  } else if (ClassOfVisual(visual) == DirectColor) {
    
    overlaytab_s[maxovl_s].map24 =
      XCreateColormap(dpy_s, DefaultRootWindow(dpy_s), visual, AllocAll);
  }
  
  /* Initialize colortable pixels (not colors) for overlays. */
  InitColortablePixels(coltabOut, len, 192,
		       ClassOfVisual(visual) == DirectColor ||
		       ClassOfVisual(visual) == TrueColor);

  /* Get the overlay-color part of the XITE-initial "reduced-color" colormap
   * (initial/standard overlay map in the cells [192..223]). */
  cmap = cmapOfVisual(visual,
		      DepthToReducedPlanes(DepthOfVisual(visual), visual));
  XQueryColors(dpy_s, cmap, coltabOut, len);

  /* Change the pixel elements of the overlay-color table to start at zero. */
  if (ClassOfVisual(visual) == PseudoColor ||
      ClassOfVisual(visual) == GrayScale) {
    
    for (y = 0; y <= len; y++) {
      coltabOut[y].pixel = y;
    }
  } else {
    for (y = 0; y <= len; y++) {
      coltabOut[y].pixel = RgbPixel(y);
    }
  }
 
  if (coltabIn) {
    int pix, y;

    /* A colortable of specified length was supplied as an argument to
     * this function. Modify the rgb values in 'coltabOut'.
     *
     * Store the incoming colortable cells into their appropriate cells in
     * 'coltabOut'. 'coltabIn' may be shorter than 'coltabOut' (and possibly
     * also not sorted, although it is not likely). If 'coltabIn' is shorter
     * than 'coltabOut', not all cells in 'coltabOut' receive new values.
     * 'coltabOut' will get colorcells from 'coltabIn' "sprinkled" throughout,
     * in one or more segments or completely filled.
     */
    if (ClassOfVisual(visual) == PseudoColor ||
	ClassOfVisual(visual) == GrayScale)
      for (y=1; y <= len; y++) {
	pix = PseudoBasePart(coltabIn[y-1].pixel);
	coltabOut[pix].red   = coltabIn[y-1].red;
	coltabOut[pix].green = coltabIn[y-1].green;
	coltabOut[pix].blue  = coltabIn[y-1].blue;
      }
    else if (ClassOfVisual(visual) == DirectColor ||
	     ClassOfVisual(visual) == TrueColor)
      for (y=1; y <= len; y++) {
	pix = RedBasePart(coltabIn[y-1].pixel);
	coltabOut[pix].red   = coltabIn[y-1].red;

	pix = GreenBasePart(coltabIn[y-1].pixel);
	coltabOut[pix].green = coltabIn[y-1].green;

	pix = BlueBasePart(coltabIn[y-1].pixel);
	coltabOut[pix].blue  = coltabIn[y-1].blue;
      }
  } /* coltabIn */

  /* "Hardware-approximate" the colortable 'coltabOut', which may have been
   * modified by the incoming colortable, into the newly created global
   * virtual colormap.
   */
  if (ClassOfVisual(visual) == PseudoColor ||
      ClassOfVisual(visual) == GrayScale)
    XStoreColors(dpy_s, overlaytab_s[maxovl_s].map8, coltabOut, len);
  else if (ClassOfVisual(visual) == DirectColor)
    XStoreColors(dpy_s, overlaytab_s[maxovl_s].map24, coltabOut, len);

  LEAVE_FUNCTION_DEBUG("Xcolor.c: AddOvlColor");

} /* AddOvlColor() */


void Color_ovl_add(XColor *coltabIn, char *name, int len)
{
  XColor col[256];
  char* ptr;
  Visual *pseudo_vis, *direct_vis;

  ENTER_FUNCTION_DEBUG("Xcolor.c: Color_ovl_add");

  pseudo_vis = visualFromName("PseudoColor", 0);
  direct_vis = visualFromName("DirectColor", 0);

  if (!pseudo_vis && !direct_vis) {
    LEAVE_FUNCTION_DEBUG("Xcolor.c: Color_ovl_add");
    return;
  }

  maxovl_s++;
  if (maxovl_s > MAXOVERLAY) {
    Warning(1, "Too many overlay colortables.\n");
    maxovl_s--;

    return;
  }

  if (maxovl_s == 2) PopupLine("colorsep", overlaysmenu_e);

  ptr = NewColorOvltabName(coltabIn, name, len);

  overlay_entries_s[maxovl_s] = (SmeBSBObject)
    PopupAdd(ptr, overlaysmenu_e, ptr, OverlaysSelect, maxovl_s);

  if (pseudo_vis && Has_changeable_map(dpy_s, screen_s, pseudo_vis))
    AddOvlColor(coltabIn, col, len, pseudo_vis);
  if (direct_vis && Has_changeable_map(dpy_s, screen_s, direct_vis))
    AddOvlColor(coltabIn, col, len, direct_vis);

  LEAVE_FUNCTION_DEBUG("Xcolor.c: Color_ovl_add");
}

void Color_ovl_read(char *file_name)
{
  char *name = NULL, *filename = NULL;
  int len;
  XColor tab[256];
  
  ENTER_FUNCTION_DEBUG("Xcolor.c: Color_ovl_read");
  
  filename = envfindfile("XSHOWPATH", file_name);
  if (filename == NULL) filename = tilde_expand(file_name);
  if (filename) {
    if (color_read(filename, (Color_cell *) tab, &name, &len) == Iok)
      Color_ovl_add(tab, name, len);
    else Message(2, "%s %s.\n",
		 "Color_ovl_read: Failure in reading colortable from file",
		 filename);
    if (filename) free(filename);
    if (name)     free(name);
  }
  else Message(2, "Color_ovl_read: Could not find colortable file.\n");
  
  LEAVE_FUNCTION_DEBUG("Xcolor.c: Color_ovl_read");
}

int Color_ovl_max(void)
{
  return(maxovl_s);
}



static Boolean ProcessTableList(char *colorfilename, int standard)
{
  FILE *f;
  int len;
  char buf[300];
  int ovl;
  char *cf = NULL, *cf2 = NULL;

  ENTER_FUNCTION_DEBUG("Xcolor.c: ProcessTableList");

  ovl = 0;
  cf = colorfilename;
  if (cf == NULL) {
    /* Try default file for list of colortable filenames. */
    cf = envfindfile("XSHOWPATH", "xshow_colortabs");
  }
  
  /* cf may now be NULL or a filename (with full path, without full path, with
   * leading environment variable or with leading '~'.
   *
   * Try first to open cf after expanding '~' and leading environment
   * variable. If this does not succeed, assume that cf is local to one of the
   * directories in XSHOWPATH and return the first one found. */

  if ((f = fopen(cf2 = tilde_expand(cf),"r")) || /* Handle '~' and $. */
      (f = fopen(cf2 = envfindfile("XSHOWPATH", cf),"r"))) {
    while(fgets(buf, 300, f) != NULL) {
      if ((len = strlen(buf)) <= 1) continue;
      if (buf[0] == ';' || buf[0] == '!' || buf[0] == '#') continue;
      buf[len-1] = 0;
      if (buf[0] == ':' && (buf[1] == 'S' || buf[1] == 's')) {
	ovl = 0;
	continue;
      }
      if (buf[0] == ':' && (buf[1] == 'O' || buf[1] == 'o')) {
	ovl = 1;
	continue;
      }
      if (buf[0] == '@') {
	ProcessTableList(&buf[1], standard);
	continue;
      }      
      if (ovl && !standard) Color_ovl_read(buf);
      else if (!ovl && standard) Color_read(buf);
    }
    fclose(f);
  }

  if (cf  != NULL && cf != colorfilename) free(cf);
  if (cf2 != NULL) free(cf2);

  LEAVE_FUNCTION_DEBUG("Xcolor.c: ProcessTableList");

  return(1);
}

void ColorsMenuInit(Display *display, int screenno, Widget parent, char *colorfile)
{
  Visual *pseudo_vis, *direct_vis;
  char *label = NULL;

  ENTER_FUNCTION_DEBUG("Xcolor.c: ColorsMenuInit");

  dpy_s = display;
  screen_s = screenno;

  RequestedImageVisualAndColormap(dpy_s, screen_s, 0, &visual_s, NULL);
  pseudo_vis = visualFromName("PseudoColor", 0);
  direct_vis = visualFromName("DirectColor", 0);

  colorsmenu_e = (Widget) PopupInit("colorsmenu", parent, NULL);
  XtVaGetValues(colorsmenu_e, XtNlabel, &label, NULL);
  if (!label) {
    XtDestroyWidget(colorsmenu_e);
    colorsmenu_e = (Widget) PopupInit("colorsmenu", parent, "Colors menu");
  }

  /* Add menu entry called "Background" as the first entry in the Image
     widget popupmenu for colortables. */
  FPRINTF1("    Create menu.\n");
  maxcol_s = 0;
  colortab_s[maxcol_s].map7  = DefaultColormap(dpy_s, screen_s);
  colortab_s[maxcol_s].map8  = DefaultColormap(dpy_s, screen_s);
  colortab_s[maxcol_s].map21 = DefaultColormap(dpy_s, screen_s);
  colortab_s[maxcol_s].map24 = DefaultColormap(dpy_s, screen_s);
  colortab_s[maxcol_s].name  = "  Background";
  colortab_s[maxcol_s].foreground = 1;
  colortab_s[maxcol_s].background = 0;
  color_entries_s[maxcol_s] = (SmeBSBObject)
    PopupAdd("backgroundcol", colorsmenu_e, "  Background", ColorsSelect, 0);
  colortab_s[0].partner_pseudo = 0;
  colortab_s[0].partner_comp   = 0;

  AddAction(parent, "ColorsHook", ColorsHook, 0);

  if (!pseudo_vis && !direct_vis) return;

  /* Add gray-scale colormaps, called "Work-map" (for 7, 8, 21 and 24 bit).
   * Increase maxcol_s by one, to become equal to one. */

  Color_add(NULL, "", 0);

  ProcessTableList(colorfile, 1);

  LEAVE_FUNCTION_DEBUG("Xcolor.c: ColorsMenuInit");
}

void OverlaysMenuInit(Display *display, int screenno, Widget parent, char *colorfile)
{
  Visual *pseudo_vis, *direct_vis;
  char *label = NULL;

  ENTER_FUNCTION_DEBUG("Xcolor.c: OverlaysMenuInit");

  dpy_s = display;
  screen_s = screenno;

  RequestedImageVisualAndColormap(dpy_s, screen_s, 0, &visual_s, NULL);
  pseudo_vis = visualFromName("PseudoColor", 0);
  direct_vis = visualFromName("DirectColor", 0);

  overlaysmenu_e = (Widget) PopupInit("overlaysmenu", parent, NULL);
  XtVaGetValues(overlaysmenu_e, XtNlabel, &label, NULL);
  if (!label) {
    XtDestroyWidget(overlaysmenu_e);
    overlaysmenu_e = (Widget) PopupInit("overlaysmenu", parent,
					"OverlayColors menu");
  }

  maxovl_s = 0; /* Becomes equal to zero. */
  overlaytab_s[maxovl_s].map8  = DefaultColormap(dpy_s, screen_s);
  overlaytab_s[maxovl_s].map24 = DefaultColormap(dpy_s, screen_s);
  overlaytab_s[maxovl_s].name  = "  Background";
  overlaytab_s[maxovl_s].foreground = 1;
  overlaytab_s[maxovl_s].background = 0;
  overlay_entries_s[maxovl_s] = (SmeBSBObject)
    PopupAdd("backgroundovl", overlaysmenu_e, "  Background",
	     OverlaysSelect, 0);

  AddAction(parent, "OverlaysHook", OverlaysHook, 0);

  if (!pseudo_vis && !direct_vis) return;

  /* Add predefined 32-element overlay colormap called "Std-overlay".
   * Increase maxovl_s by one to become equal to one. */
  Color_ovl_add(NULL, "", -1);

  /* Do not run ProcessTableList in the background, because the menus must
   * be available in the image-window menubar.
   */
  ProcessTableList(colorfile, 0);

  LEAVE_FUNCTION_DEBUG("Xcolor.c: OverlaysMenuInit");
}
