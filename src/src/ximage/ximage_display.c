

/*C*

________________________________________________________________

        ximage_display
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

static char *Id = "$Id$, Blab, UiO";

#include <xite/includes.h>
#include <X11/Xos.h>
#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <X11/StringDefs.h>
#include <xite/biff.h>
#include <stdlib.h>
#include <xite/color.h>
#include <xite/Image.h>
#include <xite/ImageOverlay.h>
#include <xite/ImageToolsMenu.h>
#include <xite/ImageOptionsMenu.h>
#include <xite/ImageVisualsMenu.h>
#include <xite/ImageForm.h>
#include <xite/statistics.h>
#include <xite/ximage_display.h>
#include <xite/Visual.h>
#include <xite/ShellWids.h>
#include <xite/Xcolor.h>
#include <xite/XmenuBar.h>
#include XITE_MALLOC_H
#include XITE_STDIO_H
#include XITE_STRING_H
#include <xite/utils.h>
#include <xite/layout.h>
#include <xite/logPos.h>
#include <xite/callbacks.h>
#include <xite/mallocp.h>
#include <xite/debug.h>

int zoomall_e = 0;    /* Option dependent external variable. */
int rgb_e     = 0;    /* Option dependent external variable. */

extern Display     *dpy_e;
extern Widget      Control_e;
extern Widget      colorsmenu_e, overlaysmenu_e, toolsmenu_e, optionsmenu_e,
                   visualsmenu_e;

extern Atom wm_delete_window;    /* For ICCCM deletion of window. */
extern char *colortabfile_e, *ovltabfile_e, *colorlistfile_e;
extern int  InitialColorTab_e, InitialOvlTab_e;

/* Global static variables. */

static int screen_s;   /* Can be determined by application by a call
			* to DefaultScreen. */

typedef struct namelist_ {
  char *name;
  int bandnr, ant;
  struct namelist_ *next;
} namelist;

static namelist *hashname_s[32] =
{ 
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
};

static Arg ImageArgs_s[] =
{
  { XtNimageImage, (XtArgVal) 0}, 
  { XtNimageBand, (XtArgVal) 0 }, 
  { XtNvisual, (XtArgVal) 0 },
  { XtNdepth, (XtArgVal) 0 },
  { XtNimageTitle, (XtArgVal) 0 },
  { XtNimageRedBand, (XtArgVal) 0 },
  { XtNimageGreenBand, (XtArgVal) 0 },
  { XtNimageBlueBand, (XtArgVal) 0 },
};

typedef enum {
  IMAGE_IMAGE,
  IMAGE_BAND,
  VISUAL,
  DEPTH,
  IMAGE_TITLE,
  IMAGE_REDBAND,
  IMAGE_GREENBAND,
  IMAGE_BLUEBAND
} resource_index;

#ifndef FUNCPROTO
int resource_num_s = IMAGE_BLUEBAND + 1;
#else /* FUNCPROTO */
const int resource_num_s = IMAGE_BLUEBAND + 1;
#endif /* FUNCPROTO */



#ifndef FUNCPROTO
static void Free_band(wid, client_data, call_data)
Widget wid;
XtPointer client_data;
XtPointer call_data;
#else /* FUNCPROTO */
static void Free_band(Widget wid, XtPointer client_data, XtPointer call_data)
#endif /* FUNCPROTO */
{
  int i, j, num_bands;
  IMAGE img = NULL, img_old = NULL;
  IBAND band;
  Boolean rgb;

  ENTER_FUNCTION_DEBUG("ximage_display.c: Free_band");

  XtVaGetValues(wid, XtNimageImage, &img,
		     XtNimageBand,  &band,
		     XtNimageRgb,   &rgb,  NULL);
  img_old = img;

  num_bands = Inbands(img);
  if (num_bands == 1 && !rgb || num_bands == 3 && rgb) {
    /* Only one band left from image, or the three bands from an rgb image. */
    Idel_image(&img);
  } else {
    /* Find which band of img is displayed in wid. */

    for (i = 1; i <= num_bands; i++) {
      if (band == img[i]) break;
    }

    if (i <= num_bands) {
      /* Found the band in img displayed in wid.
       * Remove this band and move the remaining bands to fill the gap in
       * the pointer table.
       */

      j = i;
      Idel_band(&img[i]);
      for (i = j; i < num_bands; i++)
	img[i] = img[i+1];

      /* Update the number of bands. This will call realloc, which may change
       * img. Remember to reset XtNimageImage to the new value. */
      Iset_nbands(&img, num_bands - 1);
    }
  }

  if (img_old != img && img != NULL) {
    /* This avoids an old bug which caused a crash when killing
     * widgets from a multi-band image.
     */

    WidgetList wid_list;
    IMAGE img2;
    int num_wids;

    wid_list = imageWidgetList(&num_wids);
    for (i = 0; i < num_wids; i++) {
      XtVaGetValues(wid_list[i], XtNimageImage, &img2, NULL);
      if (img2 == img_old) {
	/* Found a widget which displays a band from the same image as 'wid'.
	 */
	XtVaSetValues(wid_list[i], XtNimageImage, img, NULL);    
      }
    }
  }
  
  LEAVE_FUNCTION_DEBUG("ximage_display.c: Free_band");

  return;

} /* Free_band() */

/*L*

________________________________________________________________

		Init_color
________________________________________________________________

Name:		Init_color, Init_overlay_color

Syntax:		| static int Init_color( char *colorlistfile,
		|    char *colortabfile, int *coltabInUse );
		|
		| static void Init_overlay_color( char *colorlistfile,
		|    char *ovltabfile, int *ovltabInUse );
		|
Description:    'Init_color' adds colortable popup menu entries "Background"
                and "Work-map" (gray-scale), colortables mentioned in
                'colorlistfile' and a single colortable from the file
                'colortabfile', to the image menu Colors.

		'Init_overlay_color' adds colortables to to the ImageOverlay
		menu OverlayColors.

Author:		Otto Milvang and Svein Bøe
Doc:            Svein Bøe
________________________________________________________________

*/


#ifndef FUNCPROTO
static void Init_color(colorlistfile, colortabfile, coltabInUse)
char *colorlistfile, *colortabfile;
int *coltabInUse;
#else /* FUNCPROTO */
static void Init_color(char *colorlistfile, char *colortabfile, int *coltabInUse)
#endif /* FUNCPROTO */
{
  XColor tab[256];
  char *cf, *name, *filename = NULL;
  int len;

  ENTER_FUNCTION_DEBUG("Xfunc.c: Init_color");
  if (coltabInUse) *coltabInUse = 1;

  ColorsMenuInit(dpy_e, screen_s, Control_e, colorlistfile);

  cf = tilde_expand(colortabfile);
  if (cf) {
    if ((color_read(cf, (Color_cell *) tab, &name, &len) == 0) ||
	((filename = envfindfile("XSHOWPATH", cf)) &&
	 (color_read(filename, (Color_cell *) tab, &name, &len) == 0))) {
      Color_add(tab, name, len);
      if (coltabInUse) *coltabInUse = Color_max();
    }
  }
  if (filename) free(filename);

  LEAVE_FUNCTION_DEBUG("Xfunc.c: Init_color");

  return;

} /* Init_color */

#ifndef FUNCPROTO
static void Init_overlay(colorlistfile, ovltabfile, ovltabInUse)
char *colorlistfile, *ovltabfile;
int *ovltabInUse;
#else /* FUNCPROTO */
static void Init_overlay(char *colorlistfile, char *ovltabfile, int *ovltabInUse)
#endif /* FUNCPROTO */
{
  XColor tab[256];
  char *cf, *name, *filename = NULL;
  int len;

  ENTER_FUNCTION_DEBUG("Xfunc.c: Init_overlay");
  if (ovltabInUse) *ovltabInUse = 1;

  OverlaysMenuInit(dpy_e, screen_s, Control_e, colorlistfile);

  cf = tilde_expand(ovltabfile);
  if (cf) {
    if ((color_read(cf, (Color_cell *) tab, &name, &len) == 0) ||
	((filename = envfindfile("XSHOWPATH", cf)) &&
	 (color_read(filename, (Color_cell *) tab, &name, &len) == 0))) {
    Color_ovl_add(tab, name, len);
    if (ovltabInUse) *ovltabInUse = Color_ovl_max();
    }
  }
  if (filename) free(filename);

  return;

} /* Init_overlay */

#ifndef FUNCPROTO
void InitImageMenus(colorlistfile, colortabfile, ovltabfile, coltabInUse, ovltabInUse, menus, callbacks)
char *colorlistfile, *colortabfile, *ovltabfile;
int *coltabInUse, *ovltabInUse;
menu_button menus;
image_callbacks *callbacks;
#else /* FUNCPROTO */
void InitImageMenus(char *colorlistfile, char *colortabfile, char *ovltabfile, int *coltabInUse, int *ovltabInUse, menu_button menus, image_callbacks *callbacks)
#endif /* FUNCPROTO */
{
  screen_s = DefaultScreen(dpy_e);
  if (!colorsmenu_e && (menus & COLORS_BUTTON))
    Init_color(colorlistfile, colortabfile, coltabInUse);
  if (!overlaysmenu_e && (menus & OVERLAYS_BUTTON))
    Init_overlay(colorlistfile, ovltabfile, ovltabInUse);
  if (!optionsmenu_e && (menus & OPTIONS_BUTTON))
    OptionsMenuInit(dpy_e, screen_s, Control_e);
  if (!toolsmenu_e && (menus & TOOLS_BUTTON))
    ToolsMenuInit(dpy_e, screen_s, Control_e, ximage_display, callbacks);
  if (!visualsmenu_e && (menus & VISUALS_BUTTON))
    VisualsMenuInit(dpy_e, screen_s, Control_e, ximage_display, callbacks);

  return;

} /* InitImageMenus() */

#ifndef FUNCPROTO
static int findnumber(title, bandnr)
char *title;
int bandnr;
#else /* FUNCPROTO */
static int findnumber(char *title, int bandnr)
#endif /* FUNCPROTO */
{
  int i, j, len;
  namelist *element, *ptr;
  Boolean found_name = False, found_band = False;

  len = strlen(title);
  j   = 0;
  for(i = 0; i < len; i++) j += title[i];

  j      &= 0x01f; /* 31, the five least significant bits */
  ptr     = hashname_s[j];
  element = ptr;

  /* Search for image with same name. */
  while (element != NULL) {

    if (found_name || strcmp(element->name, title) == 0) {
      found_name = True;

      if (element->bandnr == bandnr) {
	found_band = True;
	break;
      }
    }

    ptr     = element;
    element = element->next;
  }

  if (!found_band) {
    /* Did not find an image with the same name and band number. */

    element = (namelist *) calloc(1, sizeof(namelist));
    element->name   = title;
    element->bandnr = bandnr;
    element->ant    = 1;

    if (!ptr) {
      hashname_s[j] = element;
    } else {
      ptr->next     = element;
    }

  } else element->ant++;

  /* The return value is equal to one if image name with same band number
   * was not already there.
   * Otherwise, the return value gives the number for the new image window
   * with this title and band number.
   */

  return(element->ant);
}

#ifndef FUNCPROTO
static XSizeHints *Calc_size_hints(imWid, menubar)
Widget imWid;
int menubar;
#else /* FUNCPROTO */
static XSizeHints *Calc_size_hints(Widget imWid, int menubar)
#endif /* FUNCPROTO */
{
  XSizeHints *hints;
  Dimension core_w, core_h;
  
  ENTER_FUNCTION_DEBUG("ximage_display.c: Calc_size_hints");

  XtVaGetValues(imWid, XtNwidth, &core_w, XtNheight, &core_h, NULL);

  hints                 = XAllocSizeHints();
  hints->flags          = PMinSize;
  hints->min_width      = (core_w < 64) ? core_w  : 64;
  hints->base_width     = hints->min_width;
  hints->min_aspect.x   = hints->max_aspect.x
                        = core_w;
  if (!menubar) {
    hints->min_height   = (core_h < 64) ? core_h : 64;
    hints->base_height  = hints->min_height;
    hints->min_aspect.y = hints->max_aspect.y
                        = core_h;
  } else {
    Dimension height;
    int dist;
    Widget MenuBar;

    MenuBar             = XtNameToWidget(XtParent(imWid), "menubar");
    XtVaGetValues(MenuBar, XtNheight,       &height,
		           XtNvertDistance, &dist, 
		           NULL);
    
    hints->min_height   = (core_h < 64) ?
                           (core_h + height + dist) : 64 + height + dist;
    hints->base_height  = hints->min_height;
    hints->min_aspect.y = hints->max_aspect.y
                        = core_h + height + dist;
  }
/*  hints->width_inc    = hints->base_width  / 16;
  hints->height_inc     = hints->base_height / 16;
*/
  
  FPRINTF5("    min_width:%d, min_height:%d, base_width:%d, base_height:%d\n",
	   hints->min_width, hints->min_height,
	   hints->base_width, hints->base_height);
  FPRINTF3("    min_aspect.x:%d, min_aspect.y:%d\n",
	   hints->min_aspect.x, hints->min_aspect.y);

  LEAVE_FUNCTION_DEBUG("ximage_display.c: Calc_size_hints");

  return(hints);
  
} /* Calc_size_hints() */

#ifndef FUNCPROTO
static char *band_title(img, bandnr)
IMAGE img;
int bandnr;
#else /* FUNCPROTO */
static char *band_title(IMAGE img, int bandnr)
#endif /* FUNCPROTO */
{
  char *title, *rtitle, bnr[20];
  int pos, namenr;

  rtitle = Ititle(img); 
  if (rtitle == NULL) rtitle = "Image";

  /* Generate title. */

  title = (char *) malloc(40);
  strcpy(title, rtitle);
  pos   = strlen(title);

  /* Remove trailing blanks. */
  title[pos--] = '\0';
  while (pos > 0 && title[pos] == ' ') title[pos--] = '\0';

  if (strlen(title) == 0) strcpy(title, "Image");

  /* Find how many images already have this title. 'namenr' will give the
   * position of this new image in a list of images with this title.
   */

  namenr = findnumber(rtitle, bandnr);

  if (namenr > 1) {
    /* This image title is already in use. */

    if (Inbands(img) > 1)
      sprintf(bnr, ":%d #%d", bandnr, namenr);
    else
      sprintf(bnr, " #%d", namenr);
  } else {
    /* This is the first image with this title. */

    if (Inbands(img) > 1)
      sprintf(bnr, ":%d", bandnr);
    else bnr[0] = '\0';
  }

  strcat(title, bnr);

  return(title);

} /* band_title() */

#ifndef FUNCPROTO
static void check_visual(visual, depth)
Visual **visual;
int *depth;
#else /* FUNCPROTO */
static void check_visual(Visual **visual, int *depth)
#endif /* FUNCPROTO */
{
  if (!(*visual) || (*depth) == 0) {
    /* Visual or depth is zero. Try to guess. */

    Visual *visual2 = NULL;
    int depth2 = 0;

    if (!(*visual) && (*depth) == 0) {
      /* Both visual and depth are unknown. Use the default, i.e. globally
       * requested/preferred.
       */

      RequestedImageVisual(dpy_e, DefaultScreen(dpy_e), &depth2, &visual2);
    } else if (*depth == 0) {
      /* Unknown depth. The following guess correct as long as XITE does
       * not support different depths for the same visual class. */
      
      depth2 = DepthOfVisual(*visual);
    } else if (!(*visual)) {
      /* Unknown visual. Try the known visuals. */

      if (*depth == 8 || *depth == 7)
	visual2 = VisualOfVisualInfo(*Pseudo8VisualInfo());
      else if (*depth == 24 || *depth == 21)
	visual2 = VisualOfVisualInfo(*Direct24VisualInfo());

      if (*depth == 24 && !visual2)
	visual2 = VisualOfVisualInfo(*True24VisualInfo());
    }

    if (!(*visual))    *visual = visual2;
    if ((*depth) == 0) *depth  = depth2;
  } else {
    /* Check that supplied visual and depth are OK. */

    
  }

  *depth = PlanesToDepth(*depth);

  return;

} /* check_visual() */

#ifndef FUNCPROTO
static XtTranslations menu_translations(buttons, widgetClass)
int buttons;
WidgetClass widgetClass;
#else /* FUNCPROTO */
static XtTranslations menu_translations(int buttons, WidgetClass widgetClass)
#endif /* FUNCPROTO */
{
  static char color_translations[] =
    "!Shift <Btn1Down>: ColorsHook() PopupMenu(colorsmenu)\n";
  static char overlay_translations[] =
    "!Shift Mod1 <Btn1Down>: OverlaysHook() PopupMenu(overlaysmenu)\n";
  static char tools_translations[] =
    "!Shift <Btn2Down>: PopupMenu(toolsmenu)\n";
  static char options_translations[] =
    "!Shift <Btn3Down>: OptionsHook() PopupMenu(optionsmenu)\n";
  static char visual_translations[] =
    "!Shift Mod1 <Btn3Down>: VisualsHook() PopupMenu(visualsmenu)\n";
  static char *translations;
  int len;

  if (!translations)
    translations = (char *) malloc(strlen(color_translations) +
				   strlen(overlay_translations) +
				   strlen(tools_translations) +
				   strlen(options_translations) +
				   strlen(visual_translations) + 1);
  translations[0] = '\0';
  if  (buttons & COLORS_BUTTON)   strcat(translations, color_translations);
  if ((buttons & OVERLAYS_BUTTON) &&
      widgetClass == imageOverlayWidgetClass)
                                  strcat(translations, overlay_translations);
  if (buttons  & TOOLS_BUTTON)    strcat(translations, tools_translations);
  if (buttons  & OPTIONS_BUTTON)  strcat(translations, options_translations);
  if (buttons  & VISUALS_BUTTON)  strcat(translations, visual_translations);

  len = strlen(translations);
  if (len > 0) translations[len-1] = '\0'; /* Chop off last '\n'. */
  else translations = NULL;

  if (translations) return(XtParseTranslationTable(translations));
  else return(NULL);

} /* menu_translations() */

#ifndef FUNCPROTO
static void register_parameters(img, bandno, visual, depth)
IMAGE img;
int bandno, *depth;
Visual **visual;
#else /* FUNCPROTO */
static void register_parameters(IMAGE img, int bandno, Visual **visual, int *depth)
#endif /* FUNCPROTO */
{
  IBAND band;
  char *title = NULL;

  band = img[bandno];
  check_visual(visual, depth);
  ImageArgs_s[IMAGE_IMAGE].value = (XtArgVal) img;
  ImageArgs_s[VISUAL].value = (XtArgVal) (*visual);
  ImageArgs_s[DEPTH].value = (XtArgVal) (*depth);

  ImageArgs_s[IMAGE_BAND].value = (XtArgVal) band;
  if (img) title = band_title(img, bandno);
  ImageArgs_s[IMAGE_TITLE].value = (XtArgVal) title;
  ImageArgs_s[IMAGE_REDBAND].value = (XtArgVal) NULL;
  ImageArgs_s[IMAGE_GREENBAND].value = (XtArgVal) NULL;
  ImageArgs_s[IMAGE_BLUEBAND].value = (XtArgVal) NULL;

  return;

} /* register_parameters() */

#ifndef FUNCPROTO
static void merge_args(args, num_args, merged_args, num_merged_args)
ArgList args, *merged_args;
Cardinal num_args, *num_merged_args;
#else /* FUNCPROTO */
static void merge_args(ArgList args, Cardinal num_args, ArgList *merged_args, Cardinal *num_merged_args)
#endif /* FUNCPROTO */
{
  Boolean *used;
  int i;

  if (num_args <= 0 || !args) {
    *merged_args     = ImageArgs_s;
    *num_merged_args = resource_num_s;

    return;
  }

  /* With num_args a reference, could have manipulated directly in args... */
  
  *num_merged_args = num_args + resource_num_s;
  *merged_args     = (ArgList) malloc((*num_merged_args) * sizeof(Arg));
  
  used = (Boolean *) calloc(resource_num_s, sizeof(Boolean));
  
  for (i = 0; i < (*num_merged_args); i++) {
    /* Let resources given as independent parameters to Display_band()
     * (and treated by register_parameters()) override resources in args. */
    
    if (i == num_args) break;
    
    /* Check elements in args. */
    
    (*merged_args)[i].name  = args[i].name;
    
    if (!strcmp(args[i].name, XtNimageImage)) {
      (*merged_args)[i].value = ImageArgs_s[IMAGE_IMAGE].value;
      used[IMAGE_IMAGE] = True;
      continue;
    }
    if (!strcmp(args[i].name, XtNimageBand)) {
      (*merged_args)[i].value = ImageArgs_s[IMAGE_BAND].value;
      used[IMAGE_BAND] = True;
      continue;
    }
    if (!strcmp(args[i].name, XtNvisual)) {
      (*merged_args)[i].value = ImageArgs_s[VISUAL].value;
      used[VISUAL] = True;
      continue;
    }
    if (!strcmp(args[i].name, XtNdepth)) {
      (*merged_args)[i].value = ImageArgs_s[DEPTH].value;
      used[DEPTH] = True;
      continue;
    }
    if (!strcmp(args[i].name, XtNimageTitle)) {
      (*merged_args)[i].value = ImageArgs_s[IMAGE_TITLE].value;
      used[IMAGE_TITLE] = True;
      continue;
    }
    if (!strcmp(args[i].name, XtNimageRedBand)) {
      (*merged_args)[i].value = ImageArgs_s[IMAGE_REDBAND].value;
      used[IMAGE_REDBAND] = True;
      continue;
    }
    if (!strcmp(args[i].name, XtNimageGreenBand)) {
      (*merged_args)[i].value = ImageArgs_s[IMAGE_GREENBAND].value;
      used[IMAGE_GREENBAND] = True;
      continue;
    }
    if (!strcmp(args[i].name, XtNimageBlueBand)) {
      (*merged_args)[i].value = ImageArgs_s[IMAGE_BLUEBAND].value;
      used[IMAGE_BLUEBAND] = True;
      continue;
    }
    
    (*merged_args)[i].value = args[i].value;
    
  } /* for */
  
  if (i == num_args) {
    int additional = 0;
    
    /* The above for-loop was terminated with a break-statement.
     * No more elements in args. Add remaining independent parameters
     * from ImageArgs_s. */
    
    for (i = 0; i < resource_num_s; i++) {
      if (!used[i]) {
	(*merged_args)[additional + num_args].name  = ImageArgs_s[i].name;
	(*merged_args)[additional + num_args].value = ImageArgs_s[i].value;
	additional++;
      }
    } /* for */
    
    *num_merged_args = num_args + additional;
  } /* if */

  return;

} /* merge_args() */

#ifndef FUNCPROTO
static void handle_color_resources(wid, widgetClass, visual, args, num_args)
Widget wid;
WidgetClass widgetClass;
Visual *visual;
ArgList args;
Cardinal num_args;
#else /* FUNCPROTO */
static void handle_color_resources(Widget wid, WidgetClass widgetClass, Visual *visual, ArgList args, Cardinal num_args)
#endif /* FUNCPROTO */
{
  int color_cells, colortab, overlaytab;
  Cardinal i;

  /* Some of the color resources may have been specified by the user or by
   * the args parameter to Display_band().
   */

  if (!wid) return;

  if (widgetClass == imageOverlayWidgetClass)
    XtVaGetValues(wid, XtNcolorCells,    &color_cells,
		       XtNcolortab,      &colortab,
		       XtNoverlaytab,    &overlaytab, NULL);
  else
    XtVaGetValues(wid, XtNcolorCells,    &color_cells,
		       XtNcolortab,      &colortab, NULL);

  if (!ImageHasColormap((ImageWidget) wid)) {
    colortab   = 0;
    overlaytab = 0;
  } else {
    int maxtab;

    /* Set default values if not specified (colortab == 0 set by widget
     * Initialize method). */
    maxtab = Color_max();
    if (colortab < 1 || colortab   > maxtab) colortab = InitialColorTab_e;

    if (widgetClass == imageOverlayWidgetClass) {
      maxtab = Color_ovl_max();
      if (overlaytab < 1 || overlaytab > maxtab) overlaytab = InitialOvlTab_e;
    }

    /* Let the args parameter override user and default specifications. */
    for (i = 0; i < num_args; i++) {
      if (!strcmp(args[i].name, XtNcolorCells))
	color_cells = (int) args[i].value;
      else if (!strcmp(args[i].name, XtNcolortab))
	colortab    = (int) args[i].value;
      else if (!strcmp(args[i].name, XtNoverlaytab) &&
	       widgetClass == imageOverlayWidgetClass)
	overlaytab  = (int) args[i].value;
    }

    /* Ensure legal range. */
    if (colortab   < 0 || colortab   > maxtab) colortab   = InitialColorTab_e;

    /* Background/default colormap only allowed for default visual. Set to
     * work-map otherwise. */
    if (visual != DefaultVisual(dpy_e, screen_s) && colortab == 0)
      colortab = 1;

    if (widgetClass == imageOverlayWidgetClass) {
      if (overlaytab < 0 || overlaytab > maxtab) overlaytab = InitialOvlTab_e;

      if (visual != DefaultVisual(dpy_e, screen_s) && overlaytab == 0)
	overlaytab = 1;
    }
  } /* changeable visual. */

  if (widgetClass == imageOverlayWidgetClass) {
    XtVaSetValues(wid,
		  XtNcolortab,   colortab,
		  XtNcolormap,   Color_map2(colortab, color_cells, visual),
		  XtNoverlaytab, overlaytab,
		  NULL);
    Color_ovl_set((ImageWidget) wid, overlaytab);
  } else 
    XtVaSetValues(wid,
		  XtNcolortab,  colortab,
		  XtNcolormap,  Color_map2(colortab, color_cells, visual),
		  NULL);

} /* handle_color_resources() */

#ifndef FUNCPROTO
static void handle_menubar(wid, menu_bar, menubar)
Widget wid, menu_bar;
Boolean *menubar;
#else /* FUNCPROTO */
static void handle_menubar(Widget wid, Widget menu_bar, Boolean *menubar)
#endif /* FUNCPROTO */
{

  if (!wid || !menu_bar) return;

  XtVaGetValues(wid, XtNmenubar, menubar, NULL);

  if (*menubar) {
    Dimension width, height;

    XtVaGetValues(wid,      XtNwidth,    &width,   NULL);
    XtVaSetValues(menu_bar, XtNwidth,    width,    NULL);
    XtVaSetValues(wid,      XtNfromVert, menu_bar, NULL);
    XtVaGetValues(menu_bar, XtNheight,   &height,  NULL);
    if (height > 500)
      XtVaSetValues(menu_bar, XtNheight, 30,  NULL);
  } else {
    XtDestroyWidget(menu_bar);
    XtVaSetValues(wid,      XtNfromVert, NULL,    NULL);
  }
} /* handle_menubar() */

#ifndef FUNCPROTO
static Widget Display_band(parent, resource_name, shell_resource_name, img, bandno, widgetClass, visual, depth, buttons, args, num_args)
Widget parent;
char *resource_name, *shell_resource_name;
IMAGE img;
int bandno, depth;
WidgetClass widgetClass;
Visual *visual;
menu_button buttons;
ArgList args;
Cardinal num_args;
#else /* FUNCPROTO */
static Widget Display_band(Widget parent, char *resource_name, char *shell_resource_name, IMAGE img, int bandno, WidgetClass widgetClass, Visual *visual, int depth, menu_button buttons, ArgList args, Cardinal num_args)
#endif /* FUNCPROTO */
{
  Boolean menubar = False;
  Widget image_form = NULL, menu_bar = NULL, image, image_shell = NULL;
  ArgList merged_args;
  Cardinal num_merged_args;
  Boolean new_window;

  ENTER_FUNCTION_DEBUG("ximage_display.c: Display_band");

  register_parameters(img, bandno, &visual, &depth);
  merge_args(args, num_args, &merged_args, &num_merged_args);

  new_window = XtIsSubclass(parent, shellWidgetClass);

  if (new_window) {
    if (!shell_resource_name) shell_resource_name = "imageShell";
    image_shell = CreatePopTopShell(shell_resource_name, parent);
    image_form  = XtVaCreateManagedWidget("imageForm", imageFormWidgetClass,
					image_shell, NULL);
    if (widgetClass != imageOverlayWidgetClass)
      menu_bar  = CreateMenuBar(image_form, buttons & (~OVERLAYS_BUTTON));
    else
      menu_bar  = CreateMenuBar(image_form, buttons);
  } else image_form = parent;

  if (!resource_name) resource_name = "image";
  image = XtCreateWidget(resource_name, widgetClass, image_form,
			 merged_args, num_merged_args);

  if (buttons)
    XtOverrideTranslations(image, menu_translations(buttons, widgetClass));

  handle_color_resources(image, widgetClass, visual,
			 merged_args, num_merged_args);
  handle_menubar(image, menu_bar, &menubar);

  if (zoomall_e != -1) {
    ImageSetZoomAll(image, zoomall_e);
    zoomall_e = -1;
  }

  XtManageChild(image);

  if (new_window) {
    XSizeHints *hints;

    XtRealizeWidget(image_shell);
    XtCallActionProc(image, "WMColormap", NULL, NULL, 0);

    hints = Calc_size_hints(image, menubar);
    XmbSetWMProperties(dpy_e, XtWindow(image_shell),
		       (char *) ImageArgs_s[IMAGE_TITLE].value,
		       (char *) ImageArgs_s[IMAGE_TITLE].value,
		       NULL, 0, hints, NULL, NULL);

    (void) XSetWMProtocols(dpy_e, XtWindow(image_shell),
			   &wm_delete_window, 1);
    XFree((char *) hints);
  }

  LEAVE_FUNCTION_DEBUG("ximage_display.c: Display_band");

  return(image);

} /* Display_band() */

#ifndef FUNCPROTO
Widget ximage_display(parent, resource_name, shell_resource_name, img, bandno, widgetClass, visual, depth, buttons, args, num_args, callbacks)
Widget parent;
char *resource_name, *shell_resource_name;
IMAGE img;
WidgetClass widgetClass;
Visual *visual;
int depth, bandno;
menu_button buttons;
ArgList args;
Cardinal num_args;
image_callbacks *callbacks;
#else /* FUNCPROTO */
Widget ximage_display(Widget parent, char *resource_name, char *shell_resource_name, IMAGE img, int bandno, WidgetClass widgetClass, Visual *visual, int depth, menu_button buttons, ArgList args, Cardinal num_args, image_callbacks *callbacks)
#endif /* FUNCPROTO */
{
  Widget image;

  ENTER_FUNCTION_DEBUG("ximage_display.c: ximage_display");

  screen_s = DefaultScreen(dpy_e);

  if ((!colorsmenu_e   && (buttons & COLORS_BUTTON))   ||
      (!overlaysmenu_e && (buttons & OVERLAYS_BUTTON)) ||
      (!toolsmenu_e    && (buttons & TOOLS_BUTTON))    ||
      (!optionsmenu_e  && (buttons & OPTIONS_BUTTON))  ||
      (!visualsmenu_e  && (buttons & VISUALS_BUTTON)))
    InitImageMenus(colorlistfile_e, colortabfile_e, ovltabfile_e,
		   &InitialColorTab_e, &InitialOvlTab_e, buttons, callbacks);

  if (bandno < 1) bandno = 1;
  image = Display_band(parent, resource_name, shell_resource_name, img, bandno,
		       widgetClass, visual, depth, buttons,
		       args, num_args);

  if (rgb_e && bandno == 1 && Inbands(img) == 3)
    XtVaSetValues(image, XtNimageRedBand,   img[1],
		         XtNimageGreenBand, img[2],
                         XtNimageBlueBand,  img[3], NULL);

  /* The 'callbacks' parameter may already contain callbacks. Make sure that
   * Free_band() is only added once to the XtNdestroyCallback list. */
  if (!callbacks)
    callbacks = add_to_image_callbacks(callbacks, XtNdestroyCallback,
				       Free_band, NULL, True);
  else {
    int i = 0;

    while (callbacks->destroy_cb && callbacks->destroy_cb[i].callback &&
	   callbacks->destroy_cb[i].callback != Free_band) i++;

    if (!callbacks->destroy_cb ||
	callbacks->destroy_cb[i].callback != Free_band)
      (void) add_to_image_callbacks(callbacks, XtNdestroyCallback,
				    Free_band, NULL, True);
  }

  XtAddCallbacks(image, XtNdestroyCallback, callbacks->destroy_cb);

  if (callbacks->callback_cb) XtAddCallbacks(image, XtNcallback,
					     callbacks->callback_cb);
  if (callbacks->resize_cb)   XtAddCallbacks(image, XtNresizeCallback,
					     callbacks->resize_cb);

  if (XtIsSubclass(image, imageOverlayWidgetClass) && callbacks->draw_cb)
    XtAddCallbacks(image, XtNdrawProc, callbacks->draw_cb);
  if (XtIsSubclass(image, imageOverlayWidgetClass) && callbacks->roi_cb)
    XtAddCallbacks(image, XtNroiProc, callbacks->roi_cb);

  LEAVE_FUNCTION_DEBUG("ximage_display.c: ximage_display");

  return(image);

} /* ximage_display() */
