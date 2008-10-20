
/*

________________________________________________________________

        xregion.c
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

static char *Id = "$Id$, Blab, UiO";



#ifdef MAIN

/*P:xregion*

________________________________________________________________

		xregion
________________________________________________________________

Name:		xregion - interactively draw regions in an XITE overlay
                image

Syntax:		xregion [<option>...] <image-filename> [<maskimage>]

Description:    'xregion' is a tool for drawing or editing overlay images. On
		the display, the overlays are drawn with an overlay color
		palette. Each color may represent a class in a classified
		image. 'xregion' has an interface to 'regionAnalyse(1)' and
		'statistics(1)' which makes it very powerful.

		The regions can be stored as masks in an output file, with
		background = 0 and region-pixels equal to a user specified
		value (1 - 254 ), default value = 1. Regions must be filled
		with the same classnumber as they are drawn with.

Options:        'xregion' accepts all of the standard X Toolkit command
                line options (see X(1)) as well as the XITE 'ximage(3)'
		toolkit command line options.

Actions:        See 'Image(3)' and 'ImageOverlay(3)' for descriptions of
                menus, key bindings, colortables, zooming and panning.

		'xregion' has a few additional translations defined for
		editing. These translations invoke the notify action function
		defined by the 'Image(3)' widget. This action function in
		turn calls the functions on the XtNcallback list. 'xregion'
		adds the callback function which performs the actual work.

		The additional translations are:

		&<Btn1Down>
		Draw with the present "class" color. 

		&<Btn2Down>
		Fill region with the present "class" color.

		&<Btn3Down>
		Clear region. 

Draw pushbuttons:
                &Prev
		Previous image band.

		&Next
		Next image band.

                &Clear
		Remove all regions (clear all overlays).

		&Read mask
		Read a file of regions (an overlay image).

		&Save mask
		Save the regions (an overlay image).

		&Save gray
		Save the original image where regions are overlapping, set
		pixels to 0 otherwise.

		&Set class
		Set region number for drawing (new draw color).

		&Close path

		&Fill regions
		Fill all regions (all holes). This command will remove small
		areas of background pixels.

		&Checkpoint
		Save the current regions (overlays). Go back to this state
		by pressing the Undo button. Checkpoint is done automatically
		before each fill and delete operation performed with the
		mouse button translations described in the 'Actions' section
		above.

		&Undo
		Go back to the last checkpoint state.

		&Graphics
		Toggle graphic overlay on/off.

		&Region analyse
		Start the program 'regionAnalyse' on the current regions. Set
		options with the Parameters pushbuttons. See the section below
		and 'regionAnalyse(1)'.

		This program will create a new window. To kill the window or
		save the content of the window, choose from the menu you get
		by pressing Shift+<Btn2> in the text window.

		&Statistics
		Invoke the program 'statistics(1)'. Set	options with the
		Parameters pushbuttons. See the section below.

		&Quit
                Exit the program.

Parameter pushbuttons:
                These pushbuttons set options for 'regionAnalyse' and/or
		'statistics'.

Resources:      In order to specify resources, it is useful to know
                the  hierarchy of the widgets which compose 'xregion'.
                In the notation below, indentation indicates hierarchical 
                structure.

                The following hierarchy is constructed when the default
                resources/options for widget layout is used. The widgets
		of class XfwfButton and XfwfToggle can be replaced by class
		Command and Toggle (from the Athena widget set) by using
		the 'ximage(3)' option '-widgetset' or corresponding
		application resource.

		&xregion             (class XRegion)
		| imageShell         (class TopLevelShell)
		|   imageForm        (class ImageForm)
		|     image          (class ImageOverlay)
		|     menubar        (class XfwfMenuBar)
                |       colors       (class XfwfPullDown)
                |       overlays     (class XfwfPullDown)
                |       tools        (class XfwfPullDown)
                |       options      (class XfwfPullDown)
		| xregion control panel (class TopLevelShell)
		|   topContainer     (class Form)
		|     Draw commands  (class Label)
		|     Prev           (class XfwfButton)
		|     Next           (class XfwfButton)
		|     Clear          (class XfwfButton)
		|     Read mask      (class XfwfButton)
		|     Save mask      (class XfwfButton)
		|     Save gray      (class XfwfButton)
		|     Set class      (class XfwfButton)
		|     Close path     (class XfwfButton)
		|     Fill regions   (class XfwfButton)
		|     Checkpoint     (class XfwfButton)
		|     Undo           (class XfwfButton)
		|     Graphics       (class XfwfToggle)
		|     Parameters     (class Label)
		|     Behaviour      (class Label)
		|     Header         (class XfwfToggle)
		|     Nc             (class XfwfToggle)
		|     Sort           (class XfwfToggle)
		|     Split          (class XfwfToggle)
		|     Overlay properties (class Label)
		|     Objnr          (class XfwfToggle)
		|     Regnr          (class XfwfToggle)
		|     Area           (class XfwfToggle)
		|     Perimeter      (class XfwfToggle)
		|     Minmax         (class XfwfToggle)
		|     Height         (class XfwfToggle)
		|     Width          (class XfwfToggle)
		|     Point          (class XfwfToggle)
		|     Image properties (class Label)
		|     Minmaxpix      (class XfwfToggle)
		|     Xymean         (class XfwfToggle)
		|     Volume         (class XfwfToggle)
		|     Mean           (class XfwfToggle)
		|     Median         (class XfwfToggle)
		|     Stdev          (class XfwfToggle)
		|     Commands       (class Label)
		|     Region analyse (class XfwfButton)
		|     Statistics     (class XfwfButton)
		|     Quit           (class XfwfButton)
		| colorsmenu         (class SimpleMenu)
		|   menuLabel        (class SmeBSB)
		|   backgroundcol    (class SmeBSB)
		|   Work-map         (class SmeBSB)
		|   colorsep         (class SmeLine)
		|   White - 256      (class SmeBSB)
		|   Black - 256      (class SmeBSB)
		|   Red - 256        (class SmeBSB)
		|   Green - 256      (class SmeBSB)
		|   Blue - 256       (class SmeBSB)
		|   Spectrum - 256   (class SmeBSB)
		|   Hue - 256        (class SmeBSB)
		|   Rainbow - 256    (class SmeBSB)
		| overlaysmenu       (class SimpleMenu)
		|   menuLabel        (class SmeBSB)
		|   backgroundovl    (class SmeBSB)
		|   Std-overlay      (class SmeBSB)
		|   colorsep         (class SmeLine)
		|   White - mono     (class SmeBSB)
		|   Black - mono     (class SmeBSB)
		| toolsmenu          (class SimpleMenu)
		|   menuLabel        (class SmeBSB)
		|   imageinfo        (class SmeBSB)
		|   histogram        (class SmeBSB)
		|   slice            (class SmeBSB)
		| optionsmenu        (class SimpleMenu)
		|   menuLabel        (class SmeBSB)
		|   optionsmenusep   (class SmeLine)
		|   optionsmenuentry (class SmeBSB)
		|   optionsmenuentry (class SmeBSB)
		|   optionsmenuentry (class SmeBSB)
		|   optionsmenusep   (class SmeLine)
		|   optionsmenuentry (class SmeBSB)
		|   optionsmenuentry (class SmeBSB)
		|   optionsmenuentry (class SmeBSB)
		| subjobmenu         (class SimpleMenu)
		|   menuLabel        (class SmeBSB)
		|   savebuffer       (class SmeBSB)
		|   killwindow       (class SmeBSB)

See also:       'regionAnalyse(1)', 'statistics(1)', 'ximage(3)',
                'ImageOverlay(3)'

Files:          $XITE_HOME/etc/app-defaults/XRegion

Author:		Otto Milvang

Revised:        Svein Bøe 22.11.95 and 96,
                Helene Schulerud 20.1.91,
                Tor L|nnestad 25.04.91,
		Otto Milvang 06.08.91

Examples:	| xregion mona.img
                | xregion -iw 1200 nbandimg.img:3

Id: 		$Id$
________________________________________________________________

*/

#include <xite/includes.h>
#include <xite/biff.h>
#include <stdlib.h>
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#include <X11/Xos.h>
#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Scrollbar.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/Toggle.h>
#include <Xfwf/Toggle.h>
#include <X11/Shell.h>
#include <xite/color.h>
#include <xite/message.h>
#include <xite/ximage.h>
#include <xite/Xdialog.h>
#include <xite/FormDialog.h>
#include <xite/ImageOverlay.h>
#include <xite/Xpty.h>
#include <xite/ShellWids.h>
#include <xite/mallocp.h>
#include <xite/debug.h>
#ifdef HAVE_UNISTD_H
#  include <unistd.h>
#endif
#include <signal.h>

#define GREEN  202
#define BLUE   204
#define WHITE  207
#define BACKGR 100  
#define FOREGR  64
#define LEVEL  200

typedef enum {
  XIMAGE,
  LDRAWING,
  PREV,
  NEXT,
  CLEAR,
  READM,
  SAVEM,
  SAVEG,
  SETCLASS,
  CLOSEPATH,
  FILL,
  CHECKP,
  UNDO,
  GRAPHIC,
  LPARAMETERS,
  LBEHAVIOUR,
  T_HEADER,
  T_NC,
  T_SORT,
  T_SPLIT,
  LOVERLAY_PROP,
  T_OBJNR,
  T_REGNR,
  T_AREA,
  T_PERIMETER,
  T_MINMAX,
  T_HEIGHT,
  T_WIDTH,
  T_POINT,
  LIMAGE_PROP,
  T_MINMAXPIX,
  T_XYMEAN,
  T_VOLUME,
  T_MEAN,
  T_MEDIAN,
  T_STDEV,
  LCOMMANDS,
  REGION,
  STATISTIC,
  QUIT
} widget_number;

static int num_widgets_s = QUIT + 1;
static layout *layouts_s;

/****** END Data structure for the ximage toolkit. ******/

#define wid_XIMAGE (layouts_s ? WidgetOfLayout(layouts_s[XIMAGE]) : NULL)
#define wid_LDRAWING (layouts_s ? WidgetOfLayout(layouts_s[LDRAWING]) : NULL)
#define wid_PREV (layouts_s ? WidgetOfLayout(layouts_s[PREV]) : NULL)
#define wid_NEXT (layouts_s ? WidgetOfLayout(layouts_s[NEXT]) : NULL)
#define wid_GRAPHIC (layouts_s ? WidgetOfLayout(layouts_s[GRAPHIC]) : NULL)

String LineClose[] = {
  "line",
  "close",
};

String OrigoOff[] = {
  "origo",
  "off",
};

String OrigoOn[] = {
  "origo",
  "on",
};

static Display *dpy_s;
static int     screen_s;

static Widget  toplevel_s;

static int xsize_s, ysize_s;
static int modified_s = 0;
static int mask_input_s = 0;
static int classno_s = 1;
static int bandno_s = 1;
static int oldx_s, oldy_s;

static IMAGE img_s, overlay_img_s;
static IBAND band_s, overlay_s, overlay_backup_s;



static void Classnumber(void)
{
  int ret_int;
  short w;
  char *return_text;
  int status;
  char spec[100], txt[40];

  strcpy(spec, " # Class number (1 -> 254) # # d # ");
  sprintf(&(spec[strlen(spec)]), "%d", classno_s+1); /* Default value. */

  status = FormDialog(toplevel_s, " ", spec,
		      &return_text, NULL, 3,
		      "xregion dialog", "xregion dialog");

  if (status != 1) return;

  sscanf(return_text, "%d", &ret_int);
  if ((ret_int >= 1) && (ret_int <= 254)) classno_s = ret_int;

  sprintf(txt, " ---- Region Drawing (%d) ---- ", classno_s);
  XtVaGetValues(wid_LDRAWING, XtNwidth, &w, NULL);
  XtVaSetValues(wid_LDRAWING, XtNwidth, w,
		              XtNlabel, txt, NULL);
  ImageOverlayColor((ImageOverlayWidget) wid_XIMAGE, classno_s);
}



static void Quit(Widget wid)
{
  int stat;
  if (modified_s) {
    stat = ConfirmationBox(TopLevelWidget(wid), "Really quit?", "No", "Yes",
			   "xregion - info", "xregion - info");
    if (stat != 1 ) return;
  }
  XtDestroyApplicationContext(XtWidgetToApplicationContext(wid));
  exit(0);
}

static void Nextprev(int diff)
{
  bandno_s += diff;
  XtVaSetValues(wid_PREV, XtNsensitive, bandno_s > 1, NULL);
  XtVaSetValues(wid_NEXT, XtNsensitive, bandno_s < Inbands(img_s),NULL);
  band_s = img_s[bandno_s];
  XtVaSetValues(wid_XIMAGE, XtNimageBand, band_s, NULL);
}


static void Clear(Widget wid)
{
  int x, y;

  for (y = 1; y <= ysize_s; y++)
    for (x = 1; x <= xsize_s; x++)
      overlay_s[y][x] = 0; 

  mask_input_s = 0;

  /* Must use UPDATE_ZOOMPAN or UPDATE_NEWSIZE to get the new overlay band
   * contents into the data referred to by the widget. Actually, this
   * updates the Image widget data (erasing overlay data). */

  if (wid) ImageRedisplay((ImageWidget) wid_XIMAGE, UPDATE_ZOOMPAN);
}



static void Delete(void)
{
  int y, x, z;

  for (y=1; y<=ysize_s; y++)
    for (x=1; x<=xsize_s; x++) {
      z = overlay_s[y][x];
      if (z < LEVEL) 
	overlay_s[y][x] = 0;
    }

  /* Must use UPDATE_ZOOMPAN to get the new overlay band contents into the data
   * referred to by the widget. */

  ImageRedisplay((ImageWidget) wid_XIMAGE, UPDATE_ZOOMPAN);
}

static void Save_grey(void)
{
  int stat, b, x, y;
  char *name, n[100];
  IMAGE img2;
  IBAND b2;
  ISS_BAND b3, bo3;
  II_BAND b4, bo4;
  ID_BAND b5, bo5;

  stat = SelectWriteFile(toplevel_s, writeBIFF, &name);
  if (stat != 1) return;

  strcpy(n, name);

  img2 = Icopy_init(img_s);

  for(b=1; b<=Inbands(img_s); b++) {
    b2 = img2[b];

    switch((int) Ipixtyp(band_s))
      {
      case Iu_byte_typ:
      case Is_byte_typ:
	for (y=1; y<=ysize_s; y++)
	  for (x=1; x<=xsize_s; x++)
	    b2[y][x] = ( overlay_s[y][x] > 0 ) ? ( band_s[y][x] ) : 0;
	break;
      case Iu_short_typ:
      case Is_short_typ:
	b3 = (ISS_BAND) b2;
	bo3 = (ISS_BAND) band_s;
	for (y=1; y<=ysize_s; y++)
	  for (x=1; x<=xsize_s; x++)
	    b3[y][x] = ( overlay_s[y][x] > 0 ) ? ( bo3[y][x] ) : 0;
	break;
      case Integer_typ:
      case Ireal_typ:
	b4 = (II_BAND) b2;
	bo4 = (II_BAND) band_s;
	for (y=1; y<=ysize_s; y++)
	  for (x=1; x<=xsize_s; x++)
	    b4[y][x] = ( overlay_s[y][x] > 0 ) ? ( bo4[y][x] ) : 0;
	break;
      case Idouble_typ:
	b5 = (ID_BAND) b2;
	bo5 = (ID_BAND) band_s;
	for (y=1; y<=ysize_s; y++)
	  for (x=1; x<=xsize_s; x++)
	    b5[y][x] = ( overlay_s[y][x] > 0 ) ? ( bo5[y][x] ) : 0;
	break;
      }
  } /* for */

  Ihistory(img2, "xregion", " Grey");
  stat = Iwrite_image(img2, n);
  if (stat) InformationBox(toplevel_s, Ierr_message(stat),
			   "xregion - info", "xregion - info");
  Idel_image(&img2);
  modified_s = 1;
}


static void Read_mask(void)
{
  int stat;
  char *name, n[100];
  IMAGE img2;

  stat = SelectReadFile(toplevel_s, readBIFF, &name);
  if (stat != 1) return;

  strcpy(n, name);
  img2 = Iread_image(n);
  if (img2 == 0)
    {
      sprintf(n, "Can't read file %s", name);
      InformationBox(toplevel_s, n, "xregion - info", "xregion - info");
      return;
    }
  if (Ipixtyp(img2[1]) != Iu_byte_typ) {
    InformationBox(toplevel_s,
		   "Mask image must be unsigned byte.\nNeglecting mask.",
		   "xregion - info", "xregion - info");
    return;
  }

  overlay_img_s = img2;
  Idel_band(&overlay_backup_s);
  mask_input_s = 1;
  overlay_backup_s = overlay_s;
  overlay_s = img2[1];

  XtVaSetValues(wid_XIMAGE, XtNoverlay, overlay_s, NULL);
}


static void Save_mask(void)
{
  int stat;
  char *name, n[100];
  IMAGE img2;
  IBAND b2;

  stat = SelectWriteFile(toplevel_s, writeBIFF, &name);
  if (stat != 1) return;

  strcpy(n, name);
  img2 = Icopy_init(img_s);
  b2 = img2[1];
  img2[1] = overlay_s;

/*  for (y=1; y<=ysize_s; y++)
    for (x=1; x<=xsize_s; x++)
      {
	b2[y][x] = ( overlay_s[y][x] > 0 ) ? ( overlay_s[y][x]-LEVEL ) : 0;
      }
*/

  Ihistory(img2, "xregion", "Binary");
  stat = Iwrite_image(img2, n);
  if (stat) InformationBox(toplevel_s, Ierr_message(stat),
			   "xregion - info", "xregion - info");
  modified_s = 1;
  img2[1] = b2;
  Idel_image(&img2);
}


static void Check_point(void)
{
  int x, y;
  for (y=1; y<=ysize_s; y++)
    for (x=1; x<=xsize_s; x++) {
      overlay_backup_s[y][x] =  overlay_s[y][x];
    }
}	

static void Fill_all(void)
{
  int x, y, xs, ys;
  xs = Ixstart(overlay_s);
  ys = Iystart(overlay_s);

  for (x=1; x<=xsize_s; x++) {
    if (overlay_s[1][x] == 0)
      ImageOverlayFill((ImageOverlayWidget) wid_XIMAGE, x-1+xs, ys,
		       0, 255, 4, 1, UPDATE_NONE);
    if (overlay_s[ysize_s][x] == 0)
      ImageOverlayFill((ImageOverlayWidget) wid_XIMAGE, x-1+xs,
		       ysize_s-1+ys, 0, 255, 4, 1, UPDATE_NONE);
  }
  for (y=1; y<=ysize_s; y++) {
    if (overlay_s[y][1] == 0)
      ImageOverlayFill((ImageOverlayWidget) wid_XIMAGE, xs, y-1+ys,
		       0, 255, 4, 1, UPDATE_NONE);
    if (overlay_s[y][xsize_s] == 0)
      ImageOverlayFill((ImageOverlayWidget) wid_XIMAGE, xsize_s-1+xs,
		       y-1+ys, 0, 255, 4, 1, UPDATE_NONE);
  }
  
  for (y=2; y<ysize_s; y++) {
    for (x=2; x<xsize_s; x++)
      if (overlay_s[y][x] == 0)
	ImageOverlayFill((ImageOverlayWidget) wid_XIMAGE, x-1+xs,
			 y-1+ys, 0, overlay_s[y][x-1], 4, 1, UPDATE_NONE);
  }
  
  for (x=1; x<=xsize_s; x++) {
    if (overlay_s[1][x] == 255)
      ImageOverlayFill((ImageOverlayWidget) wid_XIMAGE, x-1+xs, ys,
		       255, 0, 4, 1, UPDATE_NONE);
    if (overlay_s[ysize_s][x] == 255)
      ImageOverlayFill((ImageOverlayWidget) wid_XIMAGE, x-1+xs,
		       ysize_s-1+ys, 255, 0, 4, 1, UPDATE_NONE);
  }
  for (y=1; y<=ysize_s; y++) {
    if (overlay_s[y][1] == 255)
      ImageOverlayFill((ImageOverlayWidget) wid_XIMAGE, xs, y-1+ys,
		       255, 0, 4, 1, UPDATE_NONE);
    if (overlay_s[y][xsize_s] == 255)
      ImageOverlayFill((ImageOverlayWidget) wid_XIMAGE, xsize_s-1+xs,
		       y-1+ys, 255, 0, 4, 1, UPDATE_NONE);
  }

  /* Do not need to use UPDATE_ZOOMPAN, because the new overlay data are
   * already in the data referred to by the widget. Using UPDATE_RESIZE
   * will not recalculate the non-overlay pixmap because the window size is
   * unchanged. However, the pixmap will change because of the new overlay
   * data. */

  ImageRedisplay((ImageWidget) wid_XIMAGE, UPDATE_RESIZE);

} /* Fill_all() */

static void Undo(void)
{
  int x, y;
  for (y=1; y<=ysize_s; y++)
    for (x=1; x<=xsize_s; x++) {
      overlay_s[y][x] =  overlay_backup_s[y][x];
    }

  /* Must use UPDATE_ZOOMPAN or UPDATE_NEWSIZE to get the new overlay band
   * contents into the data referred to by the widget. Actually, this
   * updates the Image widget data (erasing old overlay data), before
   * treating the new overlay data. */

  ImageRedisplay((ImageWidget) wid_XIMAGE, UPDATE_ZOOMPAN);
}	

static void Graphic(Widget wid)
{
  Boolean state;
  char resource_name[50];

  if (XtIsSubclass(wid, toggleWidgetClass))
    (void) strcpy(resource_name, XtNstate);
  else (void) strcpy(resource_name, XtNon);

  XtVaGetValues(wid, resource_name, &state, NULL);
  ImageOverlayDisplay((ImageOverlayWidget) wid_XIMAGE, (int) state);
}	

static void Nc(Widget wid)
{
  Boolean state;
  char resource_name[50];

  if (XtIsSubclass(wid, toggleWidgetClass))
    (void) strcpy(resource_name, XtNstate);
  else (void) strcpy(resource_name, XtNon);

  XtVaGetValues(wid, resource_name, &state, NULL);
  XtVaSetValues(wid, XtNlabel, state ? "nc = 8" : "nc = 4", NULL);
}	


static void testsw(int sw, char *name, char **vec, int *vnr)
{
  Boolean state;
  Widget wid = layouts_s[sw]->core.wid;
  char resource_name[50];

  if (XtIsSubclass(wid, toggleWidgetClass))
    (void) strcpy(resource_name, XtNstate);
  else (void) strcpy(resource_name, XtNon);

  XtVaGetValues(wid, resource_name, &state, NULL);
  if (state) {
    vec[*vnr] = name;
    *vnr = *vnr+1;
  }
}

static void Clean(int pipe, int typ, int jobnr)
{
/*  if (strlen(tmp1)) 
    {
      unlink(tmp1);
      tmp1[0] = 0;
    }
  if (strlen(tmp2)) 
    {
      unlink(tmp2);
      tmp2[0] = 0;
    }*/
}


static int SendFilesReg(program *prog)
{
  char ch[20];

  sprintf(ch, "-%d", prog->ch_imgin);
  Iwrite_image(overlay_img_s, ch);
  Iwrite_image(img_s, ch);

  close(prog->ch_imgin);

  return(0);
}

static int SendFilesStat(program *prog)
{
  char ch[20];
  sprintf(ch, "-%d", prog->ch_imgin);
  if (mask_input_s) Iwrite_image(overlay_img_s, ch);
  Iwrite_image(img_s, ch);
  close(prog->ch_imgin);

  return(0);
}

static void RegionAnalyse(void)
{
  char *vec[30], infile1[20], infile2[20];
  int vnr = 1, num1 = 1;

  vec[0] = "regionAnalyse";

  testsw(T_HEADER, "-header", vec, &vnr); 
  testsw(T_NC,     "-nc",     vec, &vnr);
  if (strcmp(vec[vnr-1], "-nc")==0) vec[vnr++] = "8";
  testsw(T_SORT,   "-sort",   vec, &vnr);
  testsw(T_SPLIT,  "-split",  vec, &vnr);

  num1 = vnr;

  testsw(T_OBJNR,     "-objnr",     vec, &vnr);
  testsw(T_REGNR,     "-regnr",     vec, &vnr);
  testsw(T_AREA,      "-area",      vec, &vnr);
  testsw(T_PERIMETER, "-perimeter", vec, &vnr);
  testsw(T_MINMAX,    "-minmax",    vec, &vnr);
  testsw(T_HEIGHT,    "-height",    vec, &vnr);
  testsw(T_WIDTH,     "-width",     vec, &vnr);
  testsw(T_POINT,     "-point",     vec, &vnr);
  testsw(T_MINMAXPIX, "-minmaxpix", vec, &vnr);
  testsw(T_XYMEAN,    "-xymean",    vec, &vnr);
  testsw(T_VOLUME,    "-volume",    vec, &vnr);
  testsw(T_MEAN,      "-mean",      vec, &vnr);
  testsw(T_MEDIAN,    "-median",    vec, &vnr);
  testsw(T_STDEV,     "-stdev",     vec, &vnr);

  if (vnr == num1) {
    Warning(2, "RegionAnalyse: No options selected. Select options.\n");
    return;
  }

  strcpy(infile1, "<infile>");
  strcpy(infile2, "<infile>");

  vec[vnr++] = infile1;
  vec[vnr++] = infile2;
  vec[vnr++] = NULL;


  XptyStartProgram("regionAnalyse", vec, 1, SendFilesReg,
		   NULL, NULL, NULL, NULL);
}	

static void Statistics(void)
{
  char *vec[30], infile1[20], infile2[20];
  int vnr = 1;

  vec[0] = "statistics";

  testsw(T_HEADER,    "-h", vec, &vnr); 
  testsw(T_MINMAXPIX, "-l", vec, &vnr);
  testsw(T_MINMAXPIX, "-u", vec, &vnr);
  testsw(T_MEAN,      "-m", vec, &vnr);
  testsw(T_MEDIAN,    "-n", vec, &vnr);
  testsw(T_STDEV,     "-s", vec, &vnr);
/*  testsw(T_COVAR, "-c", vec, &vnr); */

  strcpy(infile1, "<infile>");
  strcpy(infile2, "<infile>");

  if (mask_input_s) {
    vec[vnr++] = "-r";
    vec[vnr++] = infile1;
  }
  vec[vnr++] = infile2;
  vec[vnr++] = NULL;

  XptyStartProgram("statistics", vec, 1, SendFilesStat,
		   NULL, NULL, NULL, NULL);

  
}	



static void ebutton(ImageCallback button)
{
  int x, y, ix, iy, etype;
  IBAND band;
  ImageData pix;

  x    = button->x;
  y    = button->y;
  band = (IBAND) button->band;
  ix   = button->x + 1 - Ixstart(band);
  iy   = button->y + 1 - Iystart(band);
  if (ix < 1 || ix > xsize_s || iy < 1 || iy > ysize_s) return; 

  etype = button->event ? (button->event->type) : 0;

  switch(etype)
    {
    case 0:
    case Expose:
      return;
    case ButtonRelease:
      if (button->event->xbutton.button == 2) {
	Check_point();
	ImageOverlayFill((ImageOverlayWidget) wid_XIMAGE, x, y, 0,
			 classno_s, 4, 1, UPDATE_RESIZE);
	modified_s = 1;
      } 
      if (button->event->xbutton.button == 3) {
	Check_point();
	pix = (ImageData) ImageOverlayPosXYtoAdr((ImageOverlayWidget)
						 wid_XIMAGE, x, y, 0);
	if (*pix) ImageOverlayFill((ImageOverlayWidget) wid_XIMAGE,
				   x, y, *pix, 0, 8, 1, UPDATE_RESIZE);
	modified_s = 1;
      } 
      break;
    case  MotionNotify:
      return;
    default:
      fprintf(stderr, "Unknown event type %d\n", etype);
      return;
    }
  oldx_s = x;
  oldy_s = y;
}

static void ximage_work(Widget wid, XtPointer client_data, XtPointer call_data)
{
  widget_number wid_num = (widget_number) *((int *) client_data);

  FPRINTF2("ximage_work: cl_data=%d\n", wid_num);

  switch(wid_num)
    {
    case XIMAGE:
      ebutton(call_data);
      break;
    case QUIT:
      Quit(wid);
      break;
    case PREV:
      Nextprev(-1);
      break;
    case NEXT:
      Nextprev(1);
      break;
    case CLEAR:
      Clear(wid);
      break;
    case READM:
      Read_mask();
      break;
    case SAVEM:
      Save_mask();
      break;
    case SAVEG:
      Save_grey();
      break;
    case SETCLASS:
      Classnumber();
      break;
    case CLOSEPATH:
      XtCallActionProc(wid_XIMAGE, "draw", NULL, LineClose, 2);
      XtCallActionProc(wid_XIMAGE, "draw", NULL, OrigoOff, 2);
      break;
    case FILL:
      Fill_all();
      break;
    case CHECKP:
      Check_point();
      break;
    case UNDO:
      Undo();
      break;
    case GRAPHIC:
      Graphic(wid);
      break;
    case REGION:
      RegionAnalyse();
      break;
    case STATISTIC:
      Statistics();
      break;
    case T_NC:
      Nc(wid);
      break;
    default:
      break;
  } /* switch */

  return;

} /* ximage_work() */



static void Handler(int signl)
{
  fprintf(stderr, "xregion aborted ...\n");
  Clean(0,0,0);
  XptyKill(NULL);
  exit(2);
}


static void Setup_handler(void)
{
  signal(SIGHUP,  Handler);
  signal(SIGINT,  Handler);
  signal(SIGQUIT, Handler);
  signal(SIGCLD,  SIG_IGN);
}

static void draw_callback(Widget wid, XtPointer client_data, XtPointer call_data)
{
  ImageOverlayCallbackRec *draw = (ImageOverlayCallbackRec *) call_data;

  if (!draw) return;

  if (draw->reason == 10) {
    Dimension w;
    char txt[40];
    Widget wid = wid_LDRAWING;

    classno_s++;
    if (classno_s < 1) classno_s = 1;
    else if (classno_s > 254) classno_s = 254;

    sprintf(txt, " ---- Region Drawing (%d) ---- ", classno_s);
    XtVaGetValues(wid, XtNwidth, &w, NULL);
    XtVaSetValues(wid, XtNwidth,  w, XtNlabel, txt, NULL);
  } else if (draw->reason == 8) {
    char resource_name[50];
    Boolean state;
    Widget wid = wid_GRAPHIC;

    if (XtIsSubclass(wid, toggleWidgetClass))
      (void) strcpy(resource_name, XtNstate);
    else (void) strcpy(resource_name, XtNon);

    XtVaGetValues(wid, resource_name, &state, NULL);
    XtVaSetValues(wid, resource_name, !state, NULL);
  }

} /* draw_callback() */

static void prepare_layout(void)
{

  layouts_s = (layout *) malloc(num_widgets_s * sizeof(layout));

  layouts_s[XIMAGE] = (layout)
    Make_image_layout(NULL, "Overlay", "image",   "imageShell", NULL, XIMAGE,
		      img_s, init_image_callbacks());

  add_callback_to_image_layout((layout_image) layouts_s[XIMAGE],
			       XtNcallback, ximage_work,
			       (XtPointer) MallocIntP(XIMAGE), False);
  add_callback_to_image_layout((layout_image) layouts_s[XIMAGE],
			       XtNdrawProc, draw_callback, NULL, False);

  layouts_s[LDRAWING] = (layout)
    Make_label_layout(NULL, "Label", "Draw commands", "xregion control panel",
		      NULL, LDRAWING);
  
  layouts_s[PREV] = (layout)
    Make_command_layout(NULL, "Command", "Prev",       NULL, NULL, PREV,
			ximage_work, (XtPointer) MallocIntP(PREV));
  
  layouts_s[NEXT] = (layout)
    Make_command_layout(NULL, "Command", "Next",       NULL, NULL, NEXT,
			ximage_work, (XtPointer) MallocIntP(NEXT));
  
  layouts_s[CLEAR] = (layout)
    Make_command_layout(NULL, "Command", "Clear",      NULL, NULL, CLEAR,
			ximage_work, (XtPointer) MallocIntP(CLEAR));
  
  layouts_s[READM] = (layout)
    Make_command_layout(NULL, "Command", "Read mask",  NULL, NULL, READM,
			ximage_work, (XtPointer) MallocIntP(READM));
  
  layouts_s[SAVEM] = (layout)
    Make_command_layout(NULL, "Command", "Save mask",  NULL, NULL, SAVEM,
			ximage_work, (XtPointer) MallocIntP(SAVEM));
  
  layouts_s[SAVEG] = (layout)
    Make_command_layout(NULL, "Command", "Save gray",  NULL, NULL, SAVEG,
			ximage_work, (XtPointer) MallocIntP(SAVEG));
  
  layouts_s[SETCLASS] = (layout)
    Make_command_layout(NULL, "Command", "Set class",  NULL, NULL, SETCLASS,
			ximage_work, (XtPointer) MallocIntP(SETCLASS));
  
  layouts_s[CLOSEPATH] = (layout)
    Make_command_layout(NULL, "Command", "Close path", NULL, NULL, CLOSEPATH,
			ximage_work, (XtPointer) MallocIntP(CLOSEPATH));
  
  layouts_s[FILL] = (layout)
    Make_command_layout(NULL, "Command", "Fill regions", NULL, NULL, FILL,
			ximage_work, (XtPointer) MallocIntP(FILL));
  
  layouts_s[CHECKP] = (layout)
    Make_command_layout(NULL, "Command", "Checkpoint", NULL, NULL, CHECKP,
			ximage_work, (XtPointer) MallocIntP(CHECKP));
  
  layouts_s[UNDO] = (layout)
    Make_command_layout(NULL, "Command", "Undo",       NULL, NULL, UNDO,
			ximage_work, (XtPointer) MallocIntP(UNDO));
  
  layouts_s[GRAPHIC] = (layout)
    Make_command_layout(NULL, "Toggle", "Graphics",    NULL, NULL, GRAPHIC,
			ximage_work, (XtPointer) MallocIntP(GRAPHIC));
  
  layouts_s[LPARAMETERS] = (layout)
    Make_label_layout(  NULL, "Label", "Parameters", NULL, NULL, LPARAMETERS);

  layouts_s[LBEHAVIOUR] = (layout)
    Make_label_layout(  NULL, "Label", "Behaviour", NULL, NULL, LBEHAVIOUR);
  
  layouts_s[T_HEADER] = (layout)
    Make_command_layout(NULL, "Toggle", "Header",      NULL, NULL, T_HEADER,
			ximage_work, (XtPointer) MallocIntP(T_HEADER));
  
  layouts_s[T_NC] = (layout)
    Make_command_layout(NULL, "Toggle", "Nc",          NULL, NULL, T_NC,
			ximage_work, (XtPointer) MallocIntP(T_NC));
  
  layouts_s[T_SORT] = (layout)
    Make_command_layout(NULL, "Toggle", "Sort",        NULL, NULL, T_SORT,
			ximage_work, (XtPointer) MallocIntP(T_SORT));
  
  layouts_s[T_SPLIT] = (layout)
    Make_command_layout(NULL, "Toggle", "Split",       NULL, NULL, T_SPLIT,
			ximage_work, (XtPointer) MallocIntP(T_SPLIT));
  
  layouts_s[LOVERLAY_PROP] = (layout)
    Make_label_layout(  NULL, "Label", "Overlay properties", NULL, NULL,
			LOVERLAY_PROP);

  layouts_s[T_OBJNR] = (layout)
    Make_command_layout(NULL, "Toggle", "Objnr",       NULL, NULL, T_OBJNR,
			ximage_work, (XtPointer) MallocIntP(T_OBJNR));
  
  layouts_s[T_REGNR] = (layout)
    Make_command_layout(NULL, "Toggle", "Regnr",       NULL, NULL, T_REGNR,
			ximage_work, (XtPointer) MallocIntP(T_REGNR));
  
  layouts_s[T_AREA] = (layout)
    Make_command_layout(NULL, "Toggle", "Area",        NULL, NULL, T_AREA,
			ximage_work, (XtPointer) MallocIntP(T_AREA));
  
  layouts_s[T_PERIMETER] = (layout)
    Make_command_layout(NULL, "Toggle", "Perimeter",   NULL, NULL, T_PERIMETER,
			ximage_work, (XtPointer) MallocIntP(T_PERIMETER));
  
  layouts_s[T_MINMAX] = (layout)
    Make_command_layout(NULL, "Toggle", "Minmax",      NULL, NULL, T_MINMAX,
			ximage_work, (XtPointer) MallocIntP(T_MINMAX));
  
  layouts_s[T_HEIGHT] = (layout)
    Make_command_layout(NULL, "Toggle", "Height",      NULL, NULL, T_HEIGHT,
			ximage_work, (XtPointer) MallocIntP(T_HEIGHT));
  
  layouts_s[T_WIDTH] = (layout)
    Make_command_layout(NULL, "Toggle", "Width",       NULL, NULL, T_WIDTH,
			ximage_work, (XtPointer) MallocIntP(T_WIDTH));
  
  layouts_s[T_POINT] = (layout)
    Make_command_layout(NULL, "Toggle", "Point",       NULL, NULL, T_POINT,
			ximage_work, (XtPointer) MallocIntP(T_POINT));
  
  layouts_s[LIMAGE_PROP] = (layout)
    Make_label_layout(  NULL, "Label", "Image properties", NULL, NULL,
			LIMAGE_PROP);

  layouts_s[T_MINMAXPIX] = (layout)
    Make_command_layout(NULL, "Toggle", "Minmaxpix",   NULL, NULL, T_MINMAXPIX,
			ximage_work, (XtPointer) MallocIntP(T_MINMAXPIX));
  
  layouts_s[T_XYMEAN] = (layout)
    Make_command_layout(NULL, "Toggle", "Xymean",      NULL, NULL, T_XYMEAN,
			ximage_work, (XtPointer) MallocIntP(T_XYMEAN));
  
  layouts_s[T_VOLUME] = (layout)
    Make_command_layout(NULL, "Toggle", "Volume",      NULL, NULL, T_VOLUME,
			ximage_work, (XtPointer) MallocIntP(T_VOLUME));
  
  layouts_s[T_MEAN] = (layout)
    Make_command_layout(NULL, "Toggle", "Mean",        NULL, NULL, T_MEAN,
			ximage_work, (XtPointer) MallocIntP(T_MEAN));
  
  layouts_s[T_MEDIAN] = (layout)
    Make_command_layout(NULL, "Toggle", "Median",      NULL, NULL, T_MEDIAN,
			ximage_work, (XtPointer) MallocIntP(T_MEDIAN));
  
  layouts_s[T_STDEV] = (layout)
    Make_command_layout(NULL, "Toggle", "Stdev",       NULL, NULL, T_STDEV,
			ximage_work, (XtPointer) MallocIntP(T_STDEV));

  layouts_s[LCOMMANDS] = (layout)
    Make_label_layout(  NULL, "Label", "Commands", NULL, NULL,
			LCOMMANDS);

  layouts_s[REGION] = (layout)
    Make_command_layout(NULL, "Command", "Region analyse", NULL, NULL, REGION,
			ximage_work, (XtPointer) MallocIntP(REGION));
  
  layouts_s[STATISTIC] = (layout)
    Make_command_layout(NULL, "Command", "Statistics", NULL, NULL, STATISTIC,
			ximage_work, (XtPointer) MallocIntP(STATISTIC));
  
  layouts_s[QUIT] = (layout)
    Make_command_layout(NULL, "Command", "Quit",       NULL, NULL, QUIT,
			ximage_work, (XtPointer) MallocIntP(QUIT));
  
  return;

} /* prepare_layout() */

int main(int argc, char **argv)
{
  char *local_usage =
    "Usage: %s [<option>...] <image-filename> [<maskimage>]\n";

  Iset_message(1);
  Iset_abort(0);
  InitMessage(&argc, argv, ximage_app_usage_text(local_usage));
  toplevel_s = ximage_init(&argc, argv, "XRegion", NULL, True);

  dpy_s = XtDisplay(toplevel_s);
  screen_s = DefaultScreen(dpy_s);

  if (argc < 2) Usage(1, NULL);
  img_s = Iread_image(argv[1]); /* Image proper. */
  if (img_s == NULL) Usage(2, "Can't find file %s.\n", argv[1]);

  overlay_img_s = NULL;
  if (argc >=3) {
    overlay_img_s = Iread_image(argv[2]); /* Mask image. */
    if (overlay_img_s == NULL) Usage(2,"Can't find file %s.\n",argv[2]);
    if (Ipixtyp(overlay_img_s[1]) != Iu_byte_typ) {
      InformationBox(toplevel_s,
		     "Mask image must be unsigned byte.\nNeglecting mask.",
		     "xregion - info", "xregion - info");
      overlay_img_s = NULL;
    }
    else mask_input_s = 1;
  }

  band_s  = img_s[1];
  xsize_s = Ixsize(band_s);
  ysize_s = Iysize(band_s);
  if (overlay_img_s) {
    overlay_s = overlay_img_s[1];       /* From mask file. */
  } else {
    overlay_img_s = Imake_image(1, Ititle(img_s), 3, xsize_s, ysize_s);
    overlay_s     = overlay_img_s[1];   /* Uninitialized pixel values. */
    Clear(NULL);                        /* Zero all overlay pixel-values. */
  }
  
  Iset_start(overlay_s, Ixstart(band_s), Iystart(band_s));
  if (Ixsize(overlay_s) != xsize_s || Iysize(overlay_s) != ysize_s)
    Usage(3,"Image and mask must have the same size.\n");
  overlay_backup_s = Imake_band(3, xsize_s, ysize_s);

  /* Copy overlay_s to overlay_backup_s, i.e. zero all pixel-values in
   * overlay_backup_s. */
  Check_point();

  prepare_layout();
  (void) ximageLayout(toplevel_s, layouts_s, num_widgets_s);

  XtVaSetValues(wid_XIMAGE, XtNoverlay, overlay_s, NULL);
  Nextprev(0);
  Graphic(wid_GRAPHIC);
  ImageOverlayColor((ImageOverlayWidget) wid_XIMAGE, 1);

  Setup_handler();  

  XptyInit(dpy_s, screen_s, toplevel_s);
  ximage_mainloop();

  return(0);
}

#endif /* MAIN */
