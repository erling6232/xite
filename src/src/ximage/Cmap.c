

/*

________________________________________________________________

        Cmap.c
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



#include <xite/includes.h>
#include <stdlib.h>
#include <X11/Intrinsic.h>
#include <xite/color.h>
#include <xite/Cmap.h>
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#include <ctype.h>
#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/Shell.h>
#include <X11/Xmu/StdCmap.h>
#include <xite/Visual.h>
#ifdef HAVE_MALLOC_H
# include <malloc.h>
#endif
#include <xite/debug.h>

#ifdef MOTIF
#include <Xm/Xm.h> 
#else
#include <X11/StringDefs.h> 
#endif



/* Static variables */

static XVisualInfo *pseudo8VisInfo_s, *direct24VisInfo_s, *true24VisInfo_s;

/* Colormaps for XITE-supported visuals. */
static Colormap
  imageCmapTrue24_s   = 0, /* 24-plane TrueColor colormap. */
  imageCmapDirect24_s = 0, /* 24-plane DirectColor. */
  imageCmapDirect21_s = 0, /* 24-plane reduced DirectColor. */
  imageCmapPseudo7_s  = 0, /* 8-plane PseudoColor (w/32 overlay colors). */
  imageCmapPseudo8_s  = 0, /* 8-plane PseudoColor with no overlay colors. */
  imageReqCmap_s      = 0; /* Globally preferred Image colormap. */

/* Colormap with 16 elements */
static XColor stdcol1_s[]  = {
  { 192, 0xff00, 0xff00, 0xff00, 7, 0,},  /* White   (255, 255, 255) */

  { 193, 0xc000,      0,      0, 7, 0,},  /* Red     (192,   0,   0) */
  { 194,      0, 0xc000,      0, 7, 0,},  /* Green   (  0, 192,   0) */
  { 195,      0, 0xc000, 0xc000, 7, 0,},  /* Cyan    (  0, 192, 192) */
  { 196,      0,      0, 0xc000, 7, 0,},  /* Blue    (  0,   0, 192) */
  { 197, 0xc000,      0, 0xc000, 7, 0,},  /* Magenta (192,   0, 192) */
  { 198, 0xc000, 0xc000,      0, 7, 0,},  /* Yellow  (192, 192,   0) */

  { 199, 0xc000, 0xc000, 0xc000, 7, 0,},  /* Gray    (192, 192, 192) */
  { 200,      0,      0,      0, 7, 0,},  /* Black   (  0,   0,   0) */

  { 201,      0,      0, 0xff00, 7, 0,},  /* Blue    (  0,   0, 255) */
  { 202,      0, 0xff00,      0, 7, 0,},  /* Green   (  0, 255,   0) */
  { 203,      0, 0xff00, 0xff00, 7, 0,},  /* Cyan    (  0, 255, 255) */
  { 204, 0xff00,      0,      0, 7, 0,},  /* Red     (255,   0,   0) */
  { 205, 0xff00,      0, 0xff00, 7, 0,},  /* Magenta (255,   0, 255) */
  { 206, 0xff00, 0xff00,      0, 7, 0,},  /* Yellow1 (255, 255,   ) */
  { 207, 0xff00, 0xff00, 0xff00, 7, 0,},  /* White   (255, 255, 255) */
};

static XColor stdcol1_24_s[]  = {
 { RgbPixelC(192), 0xff00, 0xff00, 0xff00, 7, 0,},/* White   (255, 255, 255) */

 { RgbPixelC(193), 0xc000,      0,      0, 7, 0,},/* Red     (192,   0,   0) */
 { RgbPixelC(194),      0, 0xc000,      0, 7, 0,},/* Green   (  0, 192,   0) */
 { RgbPixelC(195),      0, 0xc000, 0xc000, 7, 0,},/* Cyan    (  0, 192, 192) */
 { RgbPixelC(196),      0,      0, 0xc000, 7, 0,},/* Blue    (  0,   0, 192) */
 { RgbPixelC(197), 0xc000,      0, 0xc000, 7, 0,},/* Magenta (192,   0, 192) */
 { RgbPixelC(198), 0xc000, 0xc000,      0, 7, 0,},/* Yellow  (192, 192,   0) */
 { RgbPixelC(199), 0xc000, 0xc000, 0xc000, 7, 0,},/* Gray    (192, 192, 192) */
 { RgbPixelC(200),      0,      0,      0, 7, 0,},/* Black   (  0,   0,   0) */
 { RgbPixelC(201),      0,      0, 0xff00, 7, 0,},/* Blue    (  0,   0, 255) */
 { RgbPixelC(202),      0, 0xff00,      0, 7, 0,},/* Green   (  0, 255,   0) */
 { RgbPixelC(203),      0, 0xff00, 0xff00, 7, 0,},/* Cyan    (  0, 255, 255) */
 { RgbPixelC(204), 0xff00,      0,      0, 7, 0,},/* Red     (255,   0,   0) */
 { RgbPixelC(205), 0xff00,      0, 0xff00, 7, 0,},/* Magenta (255,   0, 255) */
 { RgbPixelC(206), 0xff00, 0xff00,      0, 7, 0,},/* Yellow1 (255, 255,   )  */
 { RgbPixelC(207), 0xff00, 0xff00, 0xff00, 7, 0,},/* White   (255, 255, 255) */
};

/* Colormap with 32 elements */
static XColor stdcol2_s[]  = {
  { 192,      16383,      16383,      65534,  7, 0, },
  { 193,      27934,       4832,      65534,  7, 0, },
  { 194,      41173,          0,      57129,  7, 0, },
  { 195,      52357,          0,      45945,  7, 0, },
  { 196,      64619,          0,      33683,  7, 0, },
  { 197,      65535,      11758,      21009,  7, 0, },
  { 198,      65535,      22878,       9889,  7, 0, },
  { 199,      62360,      35942,          0,  7, 0, },
  { 200,      50545,      47757,          0,  7, 0, },
  { 201,      39193,      59108,          0,  7, 0, },
  { 202,      25887,      65535,       6880,  7, 0, },
  { 203,      14582,      65535,      18185,  7, 0, },
  { 204,       2676,      65534,      30090,  7, 0, },
  { 205,          0,      55219,      43083,  7, 0, },
  { 206,          0,      44107,      54194,  7, 0, },
  { 207,       1438,      31328,      65535,  7, 0, },
  { 208,      13588,      19178,      65534,  7, 0, },
  { 209,      24802,       7965,      65534,  7, 0, },
  { 210,      38071,          0,      60231,  7, 0, },
  { 211,      49557,          0,      48744,  7, 0, },
  { 212,      61178,          0,      37123,  7, 0, },
  { 213,      65534,       8842,      23925,  7, 0, },
  { 214,      65534,      20001,      12765,  7, 0, },
  { 215,      65535,      32382,        384,  7, 0, },
  { 216,      53360,      44941,          0,  7, 0, },
  { 217,      42227,      56074,          0,  7, 0, },
  { 218,      29101,      65534,       3665,  7, 0, },
  { 219,      17370,      65534,      15396,  7, 0, },
  { 220,       5967,      65534,      26800,  7, 0, },
  { 221,          0,      58205,      40097,  7, 0, },
  { 222,          0,      46941,      51361,  7, 0, },
  { 223,          0,      34939,      63362,  7, 0, },
};

/* Colormap with 32 elements, for 24-bit composite colormaps. */
static XColor stdcol2_24_s[]  = {
  { RgbPixelC(192),      16383,      16383,      65534,  7, 0, },
  { RgbPixelC(193),      27934,       4832,      65534,  7, 0, },
  { RgbPixelC(194),      41173,          0,      57129,  7, 0, },
  { RgbPixelC(195),      52357,          0,      45945,  7, 0, },
  { RgbPixelC(196),      64619,          0,      33683,  7, 0, },
  { RgbPixelC(197),      65535,      11758,      21009,  7, 0, },
  { RgbPixelC(198),      65535,      22878,       9889,  7, 0, },
  { RgbPixelC(199),      62360,      35942,          0,  7, 0, },
  { RgbPixelC(200),      50545,      47757,          0,  7, 0, },
  { RgbPixelC(201),      39193,      59108,          0,  7, 0, },
  { RgbPixelC(202),      25887,      65535,       6880,  7, 0, },
  { RgbPixelC(203),      14582,      65535,      18185,  7, 0, },
  { RgbPixelC(204),       2676,      65534,      30090,  7, 0, },
  { RgbPixelC(205),          0,      55219,      43083,  7, 0, },
  { RgbPixelC(206),          0,      44107,      54194,  7, 0, },
  { RgbPixelC(207),       1438,      31328,      65535,  7, 0, },
  { RgbPixelC(208),      13588,      19178,      65534,  7, 0, },
  { RgbPixelC(209),      24802,       7965,      65534,  7, 0, },
  { RgbPixelC(210),      38071,          0,      60231,  7, 0, },
  { RgbPixelC(211),      49557,          0,      48744,  7, 0, },
  { RgbPixelC(212),      61178,          0,      37123,  7, 0, },
  { RgbPixelC(213),      65534,       8842,      23925,  7, 0, },
  { RgbPixelC(214),      65534,      20001,      12765,  7, 0, },
  { RgbPixelC(215),      65535,      32382,        384,  7, 0, },
  { RgbPixelC(216),      53360,      44941,          0,  7, 0, },
  { RgbPixelC(217),      42227,      56074,          0,  7, 0, },
  { RgbPixelC(218),      29101,      65534,       3665,  7, 0, },
  { RgbPixelC(219),      17370,      65534,      15396,  7, 0, },
  { RgbPixelC(220),       5967,      65534,      26800,  7, 0, },
  { RgbPixelC(221),          0,      58205,      40097,  7, 0, },
  { RgbPixelC(222),          0,      46941,      51361,  7, 0, },
  { RgbPixelC(223),          0,      34939,      63362,  7, 0, },
};



/*F:cmapOfVisualClass=Cmap*/
/*F:RGBPixel=Cmap*/
/*F:InitColortablePixels=Cmap*/
/*F:InitColortableGray=Cmap*/
/*F:InitColortableGrayPart=Cmap*/
/*F:SpreadGray=Cmap*/
/*F:PrintColormap=Cmap*/
/*F:PrintWidgetColormap=Cmap*/
/*F:ColormapCopy=Cmap*/
/*F:ColormapStd=Cmap*/
/*F:cmapOfVisual=Cmap*/
/*F:GetImageColormap=Cmap*/
/*F:Cmap*

________________________________________________________________

		Cmap
________________________________________________________________

Name:		GetImageColormap, cmapOfVisual, cmapOfVisualClass,
                RGBPixel, InitColortablePixels, InitColortableGray,
		InitColortableGrayPart, SpreadGray, PrintColormap,
		PrintWidgetColormap, ColormapCopy, ColormapStd - XITE
		module	for initializing X11 colormaps.

Syntax:         | #include <xite/Cmap.h>
                | 
                | void GetImageColormap( Display* dpy,
                |    int screen, int planes, Visual* visual,
                |    Colormap* colormap );
                |
                | void ColormapCopy( Display* dpy, int screen,
                |    Colormap dest, Colormap source,
                |    int composite );
                |
                | void ColormapStd( Display* dpy, int screen,
                |    Colormap map, int visualClass, int nr );
                |
                | void PrintWidgetColormap( Display* dpy,
                |    Widget wid );
                |
                | void PrintColormap( Display* dpy, Colormap map,
                |    int len, int composite );
                |
                | Colormap cmapOfVisual( Visual* visual,
                |    int planes );
                |
                | Colormap cmapOfVisualClass(
                |    int visualClass, int planes );
                |
                | void InitColortableGrayPart( XColor* col,
                |    int min, int max, int step, int composite );
                |
                | void InitColortableGray( XColor* col, int len,
                |    int step, int composite );
                |
                | void InitColortablePixels( XColor* col,
                |    int len, int offset, int composite );
		|
		| void SpreadGray( XColor *col, int num_colors,
		|    int pix_min, int num_cells );
                |
                | int RGBPixel( int pix, Visual* visual );
                |
Description:	'GetImageColormap' returns in '*colormap' the XITE-initial
                colormap for the specified 'visual' and 'planes'.
		If '*colormap' is non-NULL on entry, don''t change it.
		
		For a PseudoColor visual with 'planes' equal to 8, the
		XITE-initial colormap is a 256-entry grayscale map with values
		equally distributed in steps of 257 from 0 to 65535.

		For a PseudoColor visual with 'planes' equal to 7
		(reduced-color display of images to reduce "technicolor" effect
		and allow overlay planes), the XITE-initial colormap consists
		of four parts. The first and fourth part, entries [0..63] and
		[224..255], are either copies of the corresponding entries in
		the default colormap (if the default visual is 8-bit
		PseudoColor) or	a grayscale map with values in steps of 257
		from 0 to 16191 and from 57568 to 65535.
		The second part, entries [64..191}, is grayscale in steps
		of 514 from 0 to 65278. The third part,	entries [192..223],
		contains a predefined table for image overlays.
		
		For a DirectColor visual with 'planes' equal to 24, the
		XITE-initial colormap is similar to the colormap for the 8-bit
		PseudoColor case, except that each primary in the DirectColor
		case is a copy of the single PseudoColor map.

		For a DirectColor visual with 'planes' equal to 21, the
		XITE-initial colormap is similar to the colormap for the
		reduced-color 8-bit PseudoColor case, except that each primary
		in the DirectColor case is a copy of the single PseudoColor
		map.

		For a TrueColor visual with 'planes' equal to 24, the
		XITE-initial colormap equals the default colormap.

		'cmapOfVisual' is essentially the same as 'GetImageColormap',
		but with a different interface.

		'cmapOfVisualClass' is the same as 'cmapOfVisual' except with
		visual specified with 'class' instead of visual.

		'InitColortablePixels' initializes the pixelvalues of entries
		[0..(len-1)] in colortab 'col'. The values are in steps of 1
		from 'offset' to 'offset' + 'len' - 1. The values of the
		primaries are all set to zero. If 'composite' is True, the
		pixelvalues are regarded as consisting of three 8-bit parts,
		each of which is initialized as described.

		'InitColortableGray' initializes the pixelvalues of entries
		[0..(len-1)] the same way as 'InitColortablePixels'. In
		addition, the three primaries are given values in steps of
		'step' from 0 to 'step' * ('len' - 1).

		'InitColortableGrayPart' behaves like 'InitColortableGray',
		except that it treats the entries [min..max]. The values of
		the primaries are as for 'InitColortableGray' (compensated
		for 'min').

		'SpreadGray' spreads 'num_colors' evenly in 'col', starting
		with pixel 'pix_min'. Only the primaries in 'col' are changed,
		not the pixel or flag components. 'num_cells' / 'num_colors'
		neighbors will be equal.

		'PrintColormap' prints to stderr the contents of 'map' for
		entries [0..len-1]. The output is four-column ascii with
		pixelvalue (hexadecimal) and the three primary values. 

		'PrintWidgetColormap' prints to stderr the contents of
		the colormap presently used by 'wid'.

		'RGBPixel' constructs a three-part (8 bit each) composite
		pixelvalue if 'visual' is of class DirectColor or TrueColor.
		Otherwise, 'pix' is returned unchanged.

        	'ColormapCopy' copies the rgb values in the 'source' colormap
                into the 'dest' colormap (after "hardware-approximation").

                'ColormapStd' "hardware-approximates" the global colortable
		'stdcol1_s' (16 elements) or 'stdcol2_s' (32 elements) into the
		virtual colormap 'map'. The choice depends on 'nr' ('stdcol1_s'
		for 'nr' equal to 1, 'stdcol2_s' for 'nr' equal to 2).

See also:	'Visual(3)', 'xshow(1)', 'ximage(3)', 'ximageInit(3)'

Author:		Svein Bøe, original version by Otto Milvang.

Id: 		$Id$
________________________________________________________________

*/

int RGBPixel(int pix, Visual *visual)
{

  if (ClassOfVisual(visual) == DirectColor ||
      ClassOfVisual(visual) == TrueColor)
  return(RgbPixel(pix));
  else return(pix);

} /* RGBPixel() */




static Colormap cmapOfVisualInfo(XVisualInfo *visualInfo, int planes)
{
  Colormap cmap;
  int depth;

  depth = DepthOfVisualInfo(*visualInfo);

  if (planes != DepthToReducedPlanes(depth, VisualOfVisualInfo(*visualInfo)) &&
      planes != PlanesToDepth(planes))
    return((Colormap) NULL);

  if (pseudo8VisInfo_s &&
      ClassOfVisualInfo(*visualInfo) == ClassOfVisualInfo(*pseudo8VisInfo_s)) {

    if (planes == 8) cmap = imageCmapPseudo8_s;
    else             cmap = imageCmapPseudo7_s;

  } else if (direct24VisInfo_s && ClassOfVisualInfo(*visualInfo) ==
      ClassOfVisualInfo(*direct24VisInfo_s)) {

    if (planes == 24) cmap = imageCmapDirect24_s;
    else              cmap = imageCmapDirect21_s;

  } else if (true24VisInfo_s && ClassOfVisualInfo(*visualInfo) ==
	     ClassOfVisualInfo(*true24VisInfo_s)) {

    cmap = imageCmapTrue24_s;

  } else {
    fprintf(stderr,
	    "Error: This visual type is not supported.\n");
    cmap = (Colormap) NULL;
  }

  return(cmap);

} /* cmapOfVisualInfo() */

Colormap cmapOfVisualClass(int visualClass, int planes)
{
  XVisualInfo *visInfo;

  visInfo = VisualInfoFromClass(visualClass, planes);
  return(cmapOfVisualInfo(visInfo, planes));

} /* cmapOfVisualClass() */

Colormap cmapOfVisual(Visual *visual, int planes)
{
  int class;

  class = ClassOfVisual(visual);
  return(cmapOfVisualClass(class, planes));

} /* cmapOfVisual() */



void InitColortablePixels(XColor *col, int len, int offset, int composite)
{
  int i;

  if (composite)
    for (i=0; i < len; i++) {
      col[i].pixel = RgbPixel(i + offset);
      col[i].red   = col[i].green = col[i].blue = 0;
      col[i].flags = DoRed | DoGreen | DoBlue;
      col[i].pad   = 0;
    }
  else
    for (i=0; i < len; i++) {
      col[i].pixel = (i + offset);
      col[i].red   = col[i].green = col[i].blue = 0;
      col[i].flags = DoRed | DoGreen | DoBlue;
      col[i].pad   = 0;
    }
}

void InitColortableGray(XColor *col, int len, int step, int composite)
{
  int i;

  if (composite)
    for (i=0; i < len; i++) {
      col[i].pixel = RgbPixel(i);
      col[i].flags = DoRed | DoGreen | DoBlue;
      col[i].red   = (i) * step;
      col[i].green = (i) * step;
      col[i].blue  = (i) * step;
      col[i].pad   = 0;
    }
  else
    for (i=0; i < len; i++) {
      col[i].pixel = i;
      col[i].flags = DoRed | DoGreen | DoBlue;
      col[i].red   = (i) * step;
      col[i].green = (i) * step;
      col[i].blue  = (i) * step;
      col[i].pad   = 0;
    }
}

void InitColortableGrayPart(XColor *col, int min, int max, int step, int composite)
{
  int i;
  
  if (composite)
    /* For a composite colormap (DirectColor), all the primary components
     * are set equal. When this colortable is stored in a colormap by
     * XStoreColors(), each primary component is treated after the
     * corresponding colormask is applied to the pixel values.
     */
    for (i=min; i <= max; i++) {
      col[i].pixel = RgbPixel(i);
      col[i].flags = DoRed | DoGreen | DoBlue;
      col[i].red   = (i-min) * step;
      col[i].green = (i-min) * step;
      col[i].blue  = (i-min) * step;
      col[i].pad   = 0;
    }
  else
    for (i=min; i <= max; i++) {
      col[i].pixel = i;
      col[i].flags = DoRed | DoGreen | DoBlue;
      col[i].red   = (i-min) * step;
      col[i].green = (i-min) * step;
      col[i].blue  = (i-min) * step;
      col[i].pad   = 0;
    }
}

void SpreadGray(XColor *col, int num_colors, int pix_min, int num_cells)
{
  int step, count, color, i, j;

  step  = (int) (0x0ffff) / (num_colors - 1);
  count = num_cells / num_colors;

  for (i = 0; i < num_colors; i++) {
    color = i * step;
    for (j = 0; j < count; j++) {
      col[pix_min + i * count + j].red   = color;
      col[pix_min + i * count + j].green = color;
      col[pix_min + i * count + j].blue  = color;
    }
  }
} /* SpreadGray() */

void PrintColormap(Display *dpy, Colormap map, int len, int composite)
{
  XColor col[256];

  InitColortablePixels(col, len, 0, composite);
  XQueryColors(dpy, map, col, len); 

  PrintColortable((Color_cell *)col, len, 1, stderr);
  
} /* PrintColormap */




void PrintWidgetColormap(Display *dpy, Widget wid)
{
  int cells, composite;
  Colormap map;
  Visual *visual;
  char *name;

  ENTER_FUNCTION_DEBUG("Cmap.c: PrintWidgetColormap");

  visual = (Visual *) malloc(sizeof(Visual));
  
  XtVaGetValues(wid,
		XtNvisual,   &visual,
		XtNcolormap, &map,
		XtNname,     &name,
		NULL);

  if (ClassOfVisual(visual) == DirectColor || 
      ClassOfVisual(visual) == TrueColor)
    composite = TRUE;
  else composite = FALSE;
  
  cells = MapEntriesOfVisual(visual);
  if (cells == 0) cells = 256;

  if (name) {
    FPRINTF2("    Widget %s:\n", name);
  }
  PrintColormap(dpy, map, cells, composite);

  LEAVE_FUNCTION_DEBUG("Cmap.c: PrintWidgetColormap");

} /* PrintWidgetColormap */




static void MakeGrayPseudoColor8Map(Display *dpy)
{
  /* Create virtual colormap with the correct depth, which may be
     greater than 8. */

  int cells, colorStep;
  XColor col[256];

  ENTER_FUNCTION_DEBUG("Cmap.c: MakeGrayPseudoColor8Map");

  imageCmapPseudo8_s = 
    XCreateColormap(dpy, DefaultRootWindow(dpy), 
		    VisualOfVisualInfo(*pseudo8VisInfo_s),
		    AllocAll);
  
  cells = MapSizeOfVisualInfo(*pseudo8VisInfo_s);

  /* Initialize color table as gray, with the values
     0, colStep, 2*colStep, ..., (cells-1)*colStep.

     The last value is 65535 (2^16-1) when cells == 256. */

  colorStep = (int) (65536 / (cells - 1));
  FPRINTF3("    map_entries=%d, colorStep=%d.\n", cells, colorStep);

  InitColortableGray(col, cells, colorStep, FALSE); 

  /* Set color cells of imageCmapPseudo8_s, cells specified by col[].pixel, to
     the closest available hardware colors. */
  XStoreColors(dpy, imageCmapPseudo8_s, col, cells);

  LEAVE_FUNCTION_DEBUG("Cmap.c: MakeGrayPseudoColor8Map");
  
} /* MakeGrayPseudoColor8Map */







/*L:CheckConsecutive*

________________________________________________________________

		CheckConsecutive
________________________________________________________________

Name:		CheckConsecutive

Syntax:		| static int CheckConsecutive(Display *dpy, Colormap dcol,
                |    unsigned long pix_ret[]);

Description:	Check whether there are 160 consecutive pixel values starting
                with pixel value 64 among the 224 values in 'pix_ret'. These
		pixel values originate from 'dcol'. If not consecutive, then
		free the allocated colorcells in 'dcol'. If consecutive, then
		free the colorcells below and above the 160 consecutive
		colorcells in 'dcol'.

See also:	AllocateColor7Map(3) and MakeGrayPseudoColor7Map(3).

Return value:	| 0 : Not consecutive
                | 1 : Consecutive

Author:		Svein Bøe, extracted from previous code by Otto Milvang
Id: 		$Id$
________________________________________________________________

*/

static int CheckConsecutive(Display *dpy, Colormap dcol, unsigned long pix_ret[]) 
{
  /* 224 colorcells have been allocated. Check for 160 consecutive pixel
     values. */
  
  int first, status = 1;

  ENTER_FUNCTION_DEBUG("Cmap.c: CheckConsecutive");

  /* Find index for pix_ret[i] == 64.
     0 <= pix_ret[i] <= 255 (for 8-bit colormap)
     Assume pix_ret sorted, but not consecutive.
     0 <= pix_ret[0] <= (255-224)=31
     33 <= first <= 64
     0 <= pix_ret[first] <= 65 */
  
  first = 64-pix_ret[0];
  while (pix_ret[first] > 64) first--;
  
  /* Check if 160 consecutive pixel values are available (assume sorted). */
  
  if (pix_ret[first+160] != 224) {
    /* Not 160 consecutive pixel values available in colormap.
       Make all the 224 colorcells available for re-use. */
    
    FPRINTF1("    Free all color cells.\n");
    XFreeColors(dpy, dcol, pix_ret, 224, 0);
    status = 0;
    
  } else {
    /* 160 consecutive pixel values available in colormap. */

    FPRINTF1("    Free some color cells. Return default colormap.\n");
    
    /* Free the color cells corresponding to pixel values below 
       the 160 consecutive cells */
    if (first)    XFreeColors(dpy, dcol, pix_ret, first, 0);
    
    /* Free the color cells cooresponding to pixel values above
       the consecutive 160 cells */
    if (64-first) XFreeColors(dpy, dcol, &pix_ret[160+first], 
			      64-first, 0);
  }

  LEAVE_FUNCTION_DEBUG("Cmap.c: CheckConsecutive");

  return(status);

} /* CheckConsecutive */



/*L:AllocateColor7Map*

________________________________________________________________

		AllocateColor7Map
________________________________________________________________

Name:		AllocateColor7Map

Syntax:		| static void AllocateColor7Map(Display *dpy, int screen,
                |    Colormap *dcol, int *dcolOk)

Description:	Try to allocate 160 consecutive colorcells in the default
                colormap. This is done only if the default visual class,
		depth and bits_per_rgb correspond to a PseudoColor visual
		class supported by 'ximage(3)'. If successfull, return the
		default colormap in '*dcol'. If not successfull (or not
		attempted), return '*dcol' equal to zero and create a new
		virtual colormap with no allocated cells.

		Upon return, '*dcolOk' has the following value:
                | '*dcolOk' == 0: Not able to allocate 224 colorcells or not
		|                 160 consecutive colorcells among the 224
		|                 (in the default colormap).
                |           == 1: Successfully allocated 224 colorcells,
                |                 among which 160 are consecutive (in the
		|                 default colormap).
                |           == 2: No default colormap extracted.

Restrictions:	Only for the PseudoColor visual class.
Externals:      'imageCmapPseudo7_s'

See also:	CheckConsecutive(3) and MakeGrayPseudoColor7Map(3).

Author:		Svein Bøe, extracted from previous code by Otto Milvang
Id: 		$Id$
________________________________________________________________

*/

static void AllocateColor7Map(Display *dpy, int screen, Colormap *dcol, int *dcolOk)
{
  unsigned long pix_ret[256], plane_ret[256];
  Visual *defaultVisual;

  ENTER_FUNCTION_DEBUG("Cmap.c: AllocateColor7Map");

  *dcolOk = -1;
  *dcol   = 0;

  defaultVisual = DefaultVisual(dpy, screen);
  
  /* Check if it is safe to call DefaultColormap. */

  if (pseudo8VisInfo_s &&
      ClassOfVisual(defaultVisual) == PseudoColor &&
      DefaultDepth(dpy, screen) ==
      DepthOfVisualInfo(*pseudo8VisInfo_s) &&
      BitsPerRgbOfVisual(defaultVisual) ==
      BitsPerRgbOfVisualInfo(*pseudo8VisInfo_s)) {
    
    *dcol = DefaultColormap(dpy, screen);
    
    /* Allocate 224 (160+64) read/write colorcells in the default colormap.
       Pixel values are returned in 'pix_ret'. The rgb values are undefined
       until set with XStoreColor or other.

       The reason for allocating 224, not 160, colorcells is that they are
       (probably) allocated right after the initial readonly colorcells, e.g.
       starting with pixelvalue 16. The XITE display programs demand the
       reduced-color colormaps to start with pixel value 64. To make sure
       that 160 cells are allocated starting with pixel 64, we allocate
       224 (worst case if no readonly cells defined in *dcol) and check
       the result.
       */
    
    *dcolOk = XAllocColorCells(dpy, *dcol, 0, plane_ret, 0, pix_ret, 224);
    
    if (*dcolOk == 1) {
      /* Successfull color allocation so far. */

      /* The pix_ret argument returned above may have allocated pixels
       * before pixel number 64. */
      *dcolOk = CheckConsecutive(dpy, *dcol, pix_ret);
    }

  } else *dcolOk = 2;

  /* '*dcolOk' == 0: Not able to allocate 224 colorcells or not 160
                     consecutive colorcells among the 224 (in the default
		     colormap).
               == 1: Successfully allocated 224 colorcells in the default
	             colormap, among which 160 are consecutive.
               == 2: No default colormap extracted. */

  if (pseudo8VisInfo_s && *dcolOk == 1)
    /* Return the colormap in which 224 color cells were
       allocated */
    imageCmapPseudo7_s = *dcol;

  else if (pseudo8VisInfo_s) {
    /* The colorcell allocation failed or the colorcells were freed
       because we couldn't get 160 consecutive cells. */
    
    FPRINTF1("    Create new virtual colormap.\n");
    imageCmapPseudo7_s = 
      XCreateColormap(dpy, DefaultRootWindow(dpy), 
		      VisualOfVisualInfo(*pseudo8VisInfo_s),
		      AllocNone);
  }

  LEAVE_FUNCTION_DEBUG("Cmap.c: AllocateColor7Map");

} /* AllocateColor7Map */






/*L:StoreColors7Map*

________________________________________________________________

		StoreColors7Map
________________________________________________________________

Name:		StoreColors7Map

Syntax:		StoreColors7Map(Display *dpy, Colormap dcol, int dcolOk)

Description:	Generate a gray-scale colortable for the pixels [64..191],
                with the primaries going in steps of 514 from 0 to 65278.
		Get the colorcells for the pixels [0..63] and [224..255]
		from the colormap 'dcol' (if 'dcolOk' != 2) or make them
		gray, with primaries in steps of 257 from 0 to 16191 and
		in steps of 257 from 57568 to 65535. Get the colorcells
		for the pixels [192..223] from a predefined colortable.

Restrictions:	Only for the PseudoColor visual class.

See also:	AllocateColor7Map(3) and MakeGrayPseudoColor7Map(3).

Author:		Svein Bøe, extracted from previous code by Otto Milvang
Id: 		$Id$
________________________________________________________________

*/


static void StoreColors7Map(Display *dpy, Colormap dcol, int dcolOk)
{
  int cells, colorStep, i, status;
  XColor col[256];
  
  ENTER_FUNCTION_DEBUG("Cmap.c: StoreColors7Map");
  
  cells = MapSizeOfVisualInfo(*pseudo8VisInfo_s);
  
  /* Initialize color table as gray, with the values
     0, colStep, 2*colStep, ..., (cells-1)*colStep.

     The last value is 65535 (2^16-1) when cells == 256. */

  colorStep = (int) (65536 / (cells - 1));  
  FPRINTF3("    map_entries=%d, colorStep=%d.\n", cells, colorStep);
  
  InitColortableGray(col, cells, colorStep, FALSE);

  if (dcolOk != 2)
    /* Obtain rgb values from dcol, returned in col. */
    XQueryColors(dpy, dcol, col, cells); 

  /* Change center half of color table to gray, with the values
     0, colorStep, 2*colorStep, ..., 127*colorStep = 65278. */
  colorStep = (int) 2 * (65536 / (cells - 1));
  InitColortableGrayPart(col, 64, 191, colorStep, FALSE);
  
  /* Fill in most of upper quarter of color table with predefined
   * 32-element table, for color overlay. */
  for (i=0; i< 32; i++)
    col[192+i] = stdcol2_s[i];
  
  /* Set colorcells of 8-plane, but 7-bit + overlay PseudoColor colormap to
     the closest available hardware colors. */
  if (dcolOk == 1) {
    /* 160 consecutive colors were successfully allocated in the standard
       colormap, starting at pixel value 64. */
    XStoreColors(dpy, imageCmapPseudo7_s, &col[64], 160);
  } else {
    /* Allocation of 160 consecutive colorcells in the pixel range [64..223]
     * in the standard colormap failed. Created new virtual colormap.
     * Allocate colors in this map. Finally store the read/write colors. */

    unsigned long pixels_return[224];
    unsigned long mn = 256, mx = 0;

    /* Bottom default-color part, central gray-level part and overlay part.
     * First allocate 224 colors (which for sure start with pixel 0). Then
     * free the bottom part for default colors copied from the default
     * colormap. Do it this way, to be sure that the read/write colors are in
     * the range [64..223].
     */
    if ((status =
	 XAllocColorCells(dpy, imageCmapPseudo7_s, 1, NULL, 0, pixels_return,
		     224)) == 0)
      fprintf(stderr,
	      "StoreColors7Map: Trouble in allocating 224 read/write colors.");

    /* Free cells for read-only colors copied from the default colormap. */
    XFreeColors(dpy, imageCmapPseudo7_s, pixels_return, 64, 0);

    /* Allocate read-only colorcells copied from the default colormap*/
    for (i = 0; i <= 63; i++) {
      if ((status = XAllocColor(dpy, imageCmapPseudo7_s, &col[i])) == 0)
	fprintf(stderr, "%s%d\n",
		"StoreColors7Map: Trouble in allocating readonly color ",
		i);
      if (col[i].pixel < mn) mn = col[i].pixel;
      if (col[i].pixel > mx) mx = col[i].pixel;
    }
    
    mn = 256;
    mx = 0;
    for (i = 224; i < cells; i++) {
      if ((status = XAllocColor(dpy, imageCmapPseudo7_s, &col[i])) == 0)
	fprintf(stderr, "%s%d\n",
		"StoreColors7Map: Trouble in allocating readonly color ",
		i);
      if (col[i].pixel < mn) mn = col[i].pixel;
      if (col[i].pixel > mx) mx = col[i].pixel;
    }

    XStoreColors(dpy, imageCmapPseudo7_s, &col[64], 160);
  }
  
  LEAVE_FUNCTION_DEBUG("Cmap.c: StoreColors7Map");

} /* StoreColors7Map */



static void MakeGrayPseudoColor7Map(Display *dpy, int screen)
{
  /* PseudoColor, assume 8 planes. Create a 160-color map, with 128 colors
     (7-bit) map for image and 32 colors for overlay, preferably as part of
     the default colormap. Make a new virtual colormap if necessary. */

  Colormap dcol = 0;
  int dcolOk  = 0;

  ENTER_FUNCTION_DEBUG("Cmap.c: MakeGrayPseudoColor7Map");

  AllocateColor7Map(dpy, screen, &dcol, &dcolOk);
  
  /* At this point, imageCmapPseudo7_s has been set (unless this display
     does not support PseudoColor visual). imageCmapPseudo7_s may refer to
     the standard colormap ('dcol'), in which case it has allocated cells for
     pixel values in the range 64..224. imageCmapPseudo7_s may alternatively
     be a separate virtual colormap. */

  StoreColors7Map(dpy, dcol, dcolOk);
  XFlush(dpy);

  LEAVE_FUNCTION_DEBUG("Cmap.c: MakeGrayPseudoColor7Map");

} /* MakeGrayPseudoColor7Map */



static void MakeGrayPseudoColormaps(Display *dpy, int screen)
{
  ENTER_FUNCTION_DEBUG("Cmap.c: MakeGrayPseudoColormaps");
  
  /* First get 8-bit map, then 7-bit map. */

  if (!imageCmapPseudo8_s) MakeGrayPseudoColor8Map(dpy);
  if (!imageCmapPseudo7_s) MakeGrayPseudoColor7Map(dpy, screen);

  LEAVE_FUNCTION_DEBUG("Cmap.c: MakeGrayPseudoColormaps");

} /* MakeGrayPseudoColormaps */



static void MakeGrayDirectColor24Map(Display *dpy, int screen)
{
  /* Create default 24-bit DirectColor virtual colormap. Assume 8 bits for
     each of red, green and blue. */

  int cells, colorStep;
  XColor col[256];
  
  ENTER_FUNCTION_DEBUG("Cmap.c: MakeGrayDirectColor24Map");

/*  imageCmapDirect24_s = DefaultColormap(dpy, screen); */

  /* Using AllocAll will cause the whole colormap to be allocated
   * read/write. For a DirectColor visual this corresponds to calling
   * XAllocColorPlanes, returning a pixel value of zero and masks given
   * by the corresponding masks of the visual.
   */
  imageCmapDirect24_s =
    XCreateColormap(dpy, DefaultRootWindow(dpy), 
		    VisualOfVisualInfo(*direct24VisInfo_s),
		    AllocAll);

  cells = MapSizeOfVisualInfo(*direct24VisInfo_s);
  colorStep = (int) (65536 / (cells - 1));
  
  FPRINTF3("    map_entries=%d, colorStep=%d.\n", cells, colorStep);

  /* Initialize color table as "gray", with the values
     0, colStep, 2*colStep, ..., (cells-1)*colStep.

     The last value is 65535 (2^16-1) when cells == 256. */

  InitColortableGray(col, cells, colorStep, TRUE);

  /* Set color cells of imageCmapDirect24_s, cells specified by col[].pixel, to
     the closest available hardware colors. */
  XStoreColors(dpy, imageCmapDirect24_s, col, cells);

  LEAVE_FUNCTION_DEBUG("Cmap.c: MakeGrayDirectColor24Map");
  
} /* MakeGrayDirectColor24Map */



static void AllocateColor21Map(Display *dpy, int screen, Colormap *dcol, int *dcolOk)
{
  Visual *defaultVisual;

  *dcolOk = -1;
  *dcol   = 0;

  defaultVisual = DefaultVisual(dpy, screen);
  
  /* Check if it is safe to call DefaultColormap. */

  if (direct24VisInfo_s &&
      ClassOfVisual(defaultVisual) == DirectColor &&
      DefaultDepth(dpy, screen) ==
      DepthOfVisualInfo(*direct24VisInfo_s) &&
      BitsPerRgbOfVisual(defaultVisual) ==
      BitsPerRgbOfVisualInfo(*direct24VisInfo_s)) {
    
    *dcol = DefaultColormap(dpy, screen);
    
    /* Allocate 224 (160+64) read/write colorcells in the default colormap.
       Pixel values are returned in 'pix_ret'.
       The rgb values are undefined until set with XStoreColor or other. */
    
/*    *dcolOk = XAllocColorCells(dpy, *dcol, 1, NULL, 0, pix_ret, 224); */

    /* Always assume that there were not enough vacancies in the default
     * colormap. Still, the colorcells from the default map will be copied
     * into the new map before the new map is changed to suit our needs.
     * 
     * The call in the previous line should not be to XAllocColorCells, but
     * to XAllocColorPlanes, but it is now commented out anyway.
     */
    *dcolOk = 0;

/*  if (*dcolOk == 1) */
      /* Successfull color allocation so far.
       * Should check that enough consecutive colorcells were allocated.
       */

  } else *dcolOk = 2;

  /* '*dcolOk' == 0: Not able to allocate 224 colorcells or not 160
                     consecutive colorcells among the 224.
               == 1: Successfully allocated 224 colorcells in the default
	             colormap, among which 160 are consecutive.
               == 2: No default colormap extracted. */

  if (direct24VisInfo_s && *dcolOk == 1)
    /* Return the default colormap in which 224 color cells were
       allocated */
    imageCmapDirect21_s = *dcol;

  else if (direct24VisInfo_s) {
    /* The colorcell allocation failed or the colorcells were freed
       because we couldn't get 160 consecutive cells. */
    
    FPRINTF1("    Create new virtual colormap.\n");
    imageCmapDirect21_s = 
      XCreateColormap(dpy, DefaultRootWindow(dpy), 
		      VisualOfVisualInfo(*direct24VisInfo_s),
		      AllocNone);
  }
} /* AllocateColor21Map */



static void StoreColors21Map(Display *dpy, Colormap dcol, int dcolOk)
{
  int cells, colorStep, i;
  XColor col[256];
  
  ENTER_FUNCTION_DEBUG("Cmap.c: StoreColors21Map");
  
  if (!direct24VisInfo_s) return;

  cells = MapSizeOfVisualInfo(*direct24VisInfo_s);
  
  FPRINTF3("    map_entries=%d, colorStep=%d.\n", cells, colorStep);
  
  /* Initialize color table as "gray", with the values
     0, colStep, 2*colStep, ..., (cells-1)*colStep.

     The last value is 65535 (2^16-1) when cells == 256. */
  colorStep = (int) (65536 / (cells - 1));
  InitColortableGray(col, cells, colorStep, TRUE);

  if (dcolOk != 2)
    /* Obtain rgb values from dcol, returned in col. */
    XQueryColors(dpy, dcol, col, cells); 
  
  /* Change center half of color table to "gray", with the values
     0, colorStep, 2*colorStep, ..., 127*colorStep = 65278. */
  colorStep = (int) 2 * (65536 / (cells - 1));
  InitColortableGrayPart(col, 64, 191, colorStep, TRUE);
  
  /* Fill in most of upper quarter of color table with predefined
     32-element table, for color overlay. */
  for (i=0; i< 32; i++)
    col[192+i] = stdcol2_24_s[i];
  
  /* Set colorcells of 24-plane, but 21-bit + overlay DirectColor colormap to
     the closest available hardware colors. */
  if (dcolOk == 1) {
    /* 160 consecutive colors were successfully allocated in the standard
       colormap, starting at pixel value 64. */
    XStoreColors(dpy, imageCmapDirect21_s, &col[64], 160);
  } else {
    /* Same strategy as for StoreColors7Map(). */

    unsigned long pixels_return[224];

    /* Central gray-level part and overlay part. */
    XAllocColorCells(dpy, imageCmapDirect21_s, 1, NULL, 0, pixels_return, 224);

    /* Free cells for read-only colors copied from the default colormap. */
    XFreeColors(dpy, imageCmapDirect21_s, pixels_return, 64, 0);
 
    /* Allocate read-only colorcells, copied from the default colormap. */
    for (i = 0; i <= 63; i++) {
      XAllocColor(dpy, imageCmapDirect21_s, &col[i]);
    }
    for (i = 224; i < cells; i++) {
      XAllocColor(dpy, imageCmapDirect21_s, &col[i]);
    }

    XStoreColors(dpy, imageCmapDirect21_s, &col[64], 160);
  }
  
  LEAVE_FUNCTION_DEBUG("Cmap.c: StoreColors21Map");

} /* StoreColors21Map */



static void MakeGrayDirectColor21Map(Display *dpy, int screen)
{
  /* Create default 21-bit DirectColor virtual colormap. Assume 8 bits for
     each of red, green and blue. */

  Colormap dcol = 0;
  int dcolOk  = 0;
  
  ENTER_FUNCTION_DEBUG("Cmap.c: MakeGrayDirectColor21Map");

  AllocateColor21Map(dpy, screen, &dcol, &dcolOk);
  StoreColors21Map(dpy, dcol, dcolOk);
  XFlush(dpy);

  LEAVE_FUNCTION_DEBUG("Cmap.c: MakeGrayDirectColor21Map");
  
} /* MakeGrayDirectColor21Map */



static void MakeGrayDirectColormaps(Display *dpy, int screen)
{
  ENTER_FUNCTION_DEBUG("Cmap.c: MakeGrayDirectColormaps");
  
  if (!imageCmapDirect24_s) MakeGrayDirectColor24Map(dpy, screen);
  if (!imageCmapDirect21_s) MakeGrayDirectColor21Map(dpy, screen);

  LEAVE_FUNCTION_DEBUG("Cmap.c: MakeGrayDirectColormaps");

} /* MakeGrayDirectColormaps */

static void MakeTrueColormaps(Display *dpy, int screen)
{
  ENTER_FUNCTION_DEBUG("Cmap.c: MakeTrueColormaps");
  
  if (!imageCmapTrue24_s) imageCmapTrue24_s = DefaultColormap(dpy, screen);

  LEAVE_FUNCTION_DEBUG("Cmap.c: MakeTrueColormaps");

} /* MakeTrueColormaps */



static void InitializeColormaps(Display *dpy, int screen)
{
  Visual *visual = NULL;
  int planes = 0;

  if ((pseudo8VisInfo_s  = Pseudo8VisualInfo()))
    MakeGrayPseudoColormaps(dpy, screen);
  if ((direct24VisInfo_s = Direct24VisualInfo()))
    MakeGrayDirectColormaps(dpy, screen);
  if ((true24VisInfo_s   = True24VisualInfo()))
    MakeTrueColormaps(dpy, screen);

  RequestedImageVisual(dpy, screen, &planes, &visual);
  imageReqCmap_s = cmapOfVisual(visual, planes);

} /* InitializeColormaps() */




void GetImageColormap(Display *dpy, int screen, int planes, Visual *visual, Colormap *colormap)
{

  ENTER_FUNCTION_DEBUG("Cmap.c: GetImageColormap");

  if (!imageReqCmap_s) InitializeColormaps(dpy, screen);

  /* Find colormap, depending on depth. */
  if (colormap) {
    /* It is legal to reference '*colormap'. */

    if (ClassOfVisual(visual) == PseudoColor ||
        ClassOfVisual(visual) == GrayScale ||
        ClassOfVisual(visual) == DirectColor) {
      if (*colormap == 0) {
        if (planes == 7) {
          /* 8-plane PseudoColor or GrayScale with reduced-color display. */
          *colormap = imageCmapPseudo7_s;
          FPRINTF1("    Returning imageCmapPseudo7_s.\n");
        } else if (planes == 8) {
          /* PseudoColor or GrayScale with full-color display. */
          *colormap = imageCmapPseudo8_s;
          FPRINTF1("    Returning imageCmapPseudo8_s.\n");
        } else if (planes == 21) {
          /* DirectColor with reduced-color display. */
          *colormap = imageCmapDirect21_s;
          FPRINTF1("    Returning imageCmapDirect21_s.\n");
        } else if (planes == 24) {
          /* DirectColor with full-color display. */
          *colormap = imageCmapDirect24_s;
          FPRINTF1("    Returning imageCmapDirect24_s.\n");
        } else {
          /* Take the initial/preferred/requested one. */
          FPRINTF1("    No colormap returned.\n");
	  *colormap = imageReqCmap_s;
        }
      }
    } else {
      FPRINTF1("    Returning default colormap.\n");
      *colormap = DefaultColormap(dpy, screen);
    }
  }

  LEAVE_FUNCTION_DEBUG("Cmap.c: GetImageColormap");
}



void ColormapCopy(Display *dpy, int screen, Colormap dest, Colormap source,
int composite)
{
  XColor col[256];
  int cells;

  ENTER_FUNCTION_DEBUG("Cmap.c: ColormapCopy");

  cells = DisplayCells(dpy, screen);

  InitColortablePixels(col, cells, 0, composite);

  FPRINTF3("    source=%lu, dest=%lu\n", source, dest);
  XQueryColors(dpy, source, col, cells);
  XStoreColors(dpy, dest, col, cells);
  LEAVE_FUNCTION_DEBUG("Cmap.c: ColormapCopy");
}


void ColormapStd(Display *dpy, int screen, Colormap map, int visualClass, int nr)
{
  ENTER_FUNCTION_DEBUG("Cmap.c: ColormapStd");

  switch(nr)
    {
    case 1:
      if (visualClass == DirectColor || visualClass == TrueColor)
	XStoreColors(dpy, map, stdcol1_24_s, 16);
      else
	XStoreColors(dpy, map, stdcol1_s, 16);
      break;
    case 2:
      if (visualClass == DirectColor || visualClass == TrueColor)
	XStoreColors(dpy, map, stdcol2_24_s, 32);
      else
	XStoreColors(dpy, map, stdcol2_s, 32);
      break;
    }

  LEAVE_FUNCTION_DEBUG("Cmap.c: ColormapStd");
}
