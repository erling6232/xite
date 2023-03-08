
/*

________________________________________________________________

        xwarp.c
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

/*P:xwarp*

________________________________________________________________

		xwarp
________________________________________________________________

Name:		xwarp - compare two images

Syntax:		| xwarp [<option>...] <inimage-1> <inimage-2>
                [<inimage-mask>]

Description:    The program is used to compare two images. 'inimage-1' is
                the reference image and will remain unchanged. 'inimage-2'
		can be geometrically corrected and then radiometrically
		corrected. Statistics can be extracted from selected regions.

		There is an overlay image on top of the image. This overlay
                is used for drawing control points for warping, and regions
		for radiometric correction and statistical analysis. 

Options:	'xwarp' supports all standard X Toolkit command line
		arguments (see X(1)) as well as the XITE 'ximage(3)' toolkit
		command line arguments.

		The following additional arguments are supported.

		&-c r
		'r' is the number of coefficients used in the polynomials
		for transforming coordinates. Legal values are between one
		and six. The default equals three.

		&-ip n
		'n' determines the degree of the gray level interpolation
		polynomial used when determining pixel values for the warping
		function. Legal values are 'n'=0 for nearest neighbour,
		'n'=1 for bilinear interpolation, and 'n'=3 for cubic
		interpolation. Default is 1.

		&-warp q, ... r, ...
		Perform the geometric correction on 'inimage-2' with the
		polynomial coefficients given by 'q', , and 'r', ,
		when starting the program. There must be the same number of
		q and r coefficients, equal to the value of option '-ip'.
		
		&-analyse prg
		The analyse button will invoke 'prg'. This program must
                have the usage:
		| prg <mask-image> <image>
		Default 'prg' is warpExAnalysis.
		
		&-rss 
                Print all control points and root mean square errors (Root
		Sum of Squares).
		
		&-qr
		Print the polynomial coefficients used by the warp function
		when transforming from coordinates of 'inimage-1' to
		coordinates of 'inimage-2'. This may be used with the warp
                option to redo the geometric correction. The output is on
                the form (with three polynomial coefficients):
		
		|   # Q = 
		|      -230.173    1.46376  0.0805173
		|   # R = 
		|     -63.7828  0.0869576    1.36232
		
		&-bg c
                The background pixel value is set to 'c'. The default equals 0.

Drawing modes:  See 'ImageOverlay(3)' for a description of menus and default
                key/button bindings for zooming and panning.

		'xwarp' has additional translations (mouse button bindings)
		defined for the four drawing modes described below.

		&Draw control points for warping

		  To calculate the parameters for the warping, a set of
		  control points which refer to the same physical location in
		  the two images, must be defined. When the program is
		  started, the left part of the two images are displayed. Use
		  the mouse to mark control points in the two images. You will
		  need at least the same number of control points as the
		  number of warping polynomial coefficients (given by option
		  '-c').

		  Apart from the standard mouse button bindings for the
		  ImageOverlay widget, the interesting mouse button binding
		  inside the image widget during the control point drawing
		  mode, is

		  &&Btn1Down
		  Set control point

		&Draw region for gray level adjustment
		  Use the mouse to draw a region whose gray levels 
		  should be equal. 

		  Apart from the standard mouse button bindings for the
		  ImageOverlay widget, the interesting mouse button bindings
		  inside the image widget during the graylevel adjustment
		  drawing mode, are

		  &&Btn1Down
		  Set control point, and draw line from a previous point.

		  &&Btn1Motion
		  Draw line from a previous point, continue drawing path
		  while moving the mouse pointer. Print coordinates and
		  pixel value.

		  &&Btn2Down
		  Close path.

		  &&Btn3Up
		  Prepare for new path.

		  -&&When the region is selected, press "Gray nonpar." for
		  non-parametric graylevel transform, "Gray par." for
		  parametric graylevel transform or "Clear" to start a new
		  drawing.

		&Draw region for analysis
		  Use the mouse to draw a region to be analysed.
		  The draw commands are similar to the graylevel adjustment
		  drawing mode. 

		&Draw line for length measurement
		  Use the mouse to draw a line and have the length of
		  the line printed.

		  Apart from the standard mouse button bindings for the
		  ImageOverlay widget, the interesting mouse button bindings
		  inside the image widget during the linelength measurement
		  mode, are

		  &&Btn3Down
		  Start line.

		  &&Btn3Up
		  End line, and print line length.

		  -&&Notice that the mouse pointer must be held down while
		  dragging and then be released at the end point of the line.
		  Coordinates and pixel value are printed while dragging.

Actions:
		&Warp
		Press the "warp" button to perform the warp.

		&Analyse
		Unless the option '-analyse' is specified, the program
		'warpExAnalysis' is invoked by the 'analyse' button. The
		result of the analysis is presented in a new window. Use
		<SHIFT>MB2 to save or kill this window.

Display mode:   Except when drawing control points, the two images may be
                displayed in four different modes:

                &Display image sum
		&Display image 1
		&Display image 2
		&Display image difference

		-&'analyse' and 'save' will be applied to the displayed image.
		
Clear:          Several regions may be drawn. 'clear' will remove
		all drawings. It will also remove all control points.

Quit:           Exit the program

Restrictions:	All images must be of equal size and have pixel type unsigned
                byte.

See also:       'ImageOverlay(3)'
Author:		Otto Milvang
Revised:        Svein Bøe
Examples:	| xwarp img1 img2
Id: 		$Id$
________________________________________________________________

*/

#include <math.h>
#include <xite/includes.h>
#include <X11/Xos.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/Toggle.h>
#include <xite/biff.h>
#include XITE_STDIO_H
#include XITE_UNISTD_H
#include <xite/ximage.h>
#include XITE_MALLOC_H
#include XITE_STDARG_H
#include <xite/readarg.h>
#include <xite/histo.h>
#include <xite/region.h>
#include <xite/Image.h>
#include <xite/ImageOverlay.h>
#include <xite/mallocp.h>
#include <xite/message.h>
#include <xite/geometry.h>
#include <xite/arithmetic.h>
#include <xite/Xpty.h>
#include <xite/Xdialog.h>
#include <xite/Xcolor.h>
#include <xite/ShellWids.h>
#include <xite/matrix.h>

#define XIMAGE       0
#define HEADER       (XIMAGE + 1)
#define INFO         (HEADER + 1)
#define MODELAB      (INFO + 1)
#define CONTROL      (MODELAB + 1)
#define GRAYAREA     (CONTROL + 1)
#define ANALYSISAREA (GRAYAREA + 1)
#define LINELENGTH   (ANALYSISAREA + 1)
#define DOLAB        (LINELENGTH + 1)
#define WARP         (DOLAB + 1)
#define GRAYNON      (WARP + 1)
#define GRAYPAR      (GRAYNON + 1)
#define ANALYSE      (GRAYPAR + 1)
#define CLEAR        (ANALYSE + 1)
#define DISPLAB      (CLEAR + 1)
#define ADD2         (DISPLAB + 1)
#define IMAGE1       (ADD2 + 1)
#define IMAGE2       (IMAGE1 + 1)
#define NEGATE2      (IMAGE2 + 1)
#define MISCLAB      (NEGATE2 + 1)
#define PREV         (MISCLAB + 1)
#define NEXT         (PREV + 1)
#define SAVE         (NEXT + 1)
#define QUIT         (SAVE + 1)
#define LEFT         (QUIT + 1)
#define RIGHT        (LEFT + 1)

#define WIDGETS      (QUIT + 1)

#define WARP_START_DRAW_COL 1
#define GRAY_DRAW_COL       10
#define ANALYSIS_DRAW_COL   20
#define LINELENGTH_DRAW_COL 14

static IMAGE img1_s, img2_s, resultimg_s, warpimg_s, grayimg_s, maskimg_s;
static IBAND resultband_s, lefto_s, righto_s, botho_s;
static IR_BAND ptband_s;
static char *maskfile_s, lab_s[200], *ana_prog_s;
static float f_s = 0.5;
static int neg2_s = 0, split_image_s = 1, rss_s, qr_s, ip_s, bg_s; 
static int bandnr_s, labchanged_s, lnpoint_s, rnpoint_s, ncoeffs_s;

static char *widgets_s[WIDGETS] = {
  "O:xwarp-image",
  "L:xwarp-label:xwarp control panel",
  "L:info",
  "L:mode-lab",
  "T:ctrl-pts",
  "T:gray-area",
  "T:analyse-area",
  "T:line-area",
  "L:do-lab",
  "C:warp",
  "C:gray-nonpar",
  "C:gray-parametric",
  "C:analyse",
  "C:clear",
  "L:disp-lab",
  "T:add-2",
  "T:image-1",
  "T:image-2",
  "T:sub-2",
  "L:misc-lab",
  "C:prev",
  "C:next",
  "C:save",
  "C:quit",
};

static char *ewidgets_s[WIDGETS] = {
  NULL,
  NULL,
  NULL,
  NULL,
  "Select control points",
  "Draw region for gray level adjustment",
  "Draw region for analysis",
  "Draw line for length measurement",
  NULL,
  "Perform warp",
  "Perform non-parametric gray level adjustment",
  "Perform parametric gray level adjustment",
  "Perform analysis",
  "Clear all",
  NULL,
  "Display image sum",
  "Display image 1",
  "Display image 2",
  "Display image difference",
  NULL,
  "Switch to previous band",
  "Switch to next band",
  "Save image",
  "Quit program",
};

static Widget toplevel, rwidgets_s[WIDGETS];
static Widget left_s, right_s;

static void bottomInfoHandler(Widget widget, XtPointer client_data, XEvent *event)
{
  Dimension width;
  char *p;

  if (event->type == LeaveNotify) {
    if (labchanged_s) return;
    p = lab_s;
  } else if (event->type == EnterNotify) {
    labchanged_s = 0;
    p = (char *) client_data;
  }

  XtVaGetValues(rwidgets_s[INFO], XtNwidth, &width,  NULL);
  XtVaSetValues(rwidgets_s[INFO], XtNlabel, p, XtNwidth, width,  NULL);

  return;
}

static int info(int id, char *message)
{
  Dimension width;

  (void) strcpy(lab_s, message);

  labchanged_s = 1;
  XtVaGetValues(rwidgets_s[INFO], XtNwidth, &width,  NULL);
  XtVaSetValues(rwidgets_s[INFO], XtNlabel, lab_s, XtNwidth, width,  NULL);

  return id;
}

static int _warp(IMAGE inimg, IMAGE outimg, IBAND points, int lnpoint, int rnpoint, int ncoeffs, int ip, int bg, int rss, int qr)
{
  int status;

  if (lnpoint != rnpoint)
    return(Error(1, "%d / %d points selected.\n", lnpoint, rnpoint));

  status = warp(inimg, outimg, points, lnpoint, ncoeffs, ip, bg, rss, qr);

  return(status);

} /* _warp() */

static int gray(IBAND inband1, IBAND inband2, IBAND outband, IBAND overlay, int par)
{
  region **regb, *reg, *rbg;
  histogram h1, h2, trans;
  double ant = 0, sum=0, sum2=0;
  int rsize, i, j, rnr, x, y;

  /* Eight-neighbourhood regions. */
  regionYline(overlay, &regb, 8, 0, 0, &rsize);
  if (rsize <= 1) {
    return(Error(1, "No regions selected for graylevel adjustment.\n"));
  }

  /* For each region object. */
  for(rnr = 1; rnr <= rsize; rnr++) {
    reg = regb[rnr];

    if (regionIsPoint(reg, 0, 1, 1)) continue;
    if (reg->objectnr == 0) {
      regionPoint(reg, &x, &y);
      if (x > 1) x -= 1;
      else y -= 1;

      for(i = 1; i < rnr; i++) {
	rbg = regb[i];
	if (regionIsPoint(rbg, 0, x, y)) break;
      }

      if (i == rnr) {
	return(Error(1, "No background region??\n"));
      }

      reg->next = rbg->next;
      rbg->next = reg;
    }
  }
  
  /* Zero-initialize the histograms. */
  regionHisto(inband1, NULL, 1, h1);
  regionHisto(inband2, NULL, 1, h2);

  for(rnr = 1; rnr<=rsize; rnr++) {
    reg = regb[rnr];
    if (reg->objectnr != GRAY_DRAW_COL) continue;
    regionHistoUpdate(inband1, reg, 1, h1);
    regionHistoUpdate(inband2, reg, 1, h2);
  }
  
  ant = 0.0; sum=0.0; sum2=0.0;
  for(i=0; i<=255; i++) {
    ant  += h1[i];
    sum  += i * h1[i];
    sum2 += i * i * h1[i];
  }
  
  ant = 0.0; sum=0.0; sum2=0.0;
  for(i=0; i<=255; i++) {
    ant  += h2[i];
    sum  += i*h2[i];
    sum2 += i*i*h2[i];
  }
  
  if (par) {
    double m1,m2, s1, s2, gain, disp;

    m1 = sum / ant; 
    s1 = sqrt(sum2/ant - sum*sum/ant/ant);
    m2 = sum / ant; 
    s2 = sqrt(sum2/ant - sum*sum/ant/ant);

    gain = s1 / s2;
    disp = m1 - gain * m2;

    for(i=0; i<=255; i++) {
      j = gain * (double) i + disp;
      if (j<0) j = 0;
      else if (j>255) j = 255;

      trans[i] = j;
    }
  } else {
    mkCumHisto(h1, h1);
    mkCumHisto(h2, h2);

    j=0;
    for(i=0; i<=255; i++) {
      while (h1[j] < h2[i]) j++;

      trans[i] = j;
    }
  }

  histoTransf(inband2, outband, trans);

  return(0);
  
} /* gray() */

static int SendFiles(program *prog)
{
  char ch[20];

  sprintf(ch, "-%d", prog->ch_imgin);
  Iwrite_image(maskimg_s, ch);
  Iwrite_image(resultimg_s, ch);
  close(prog->ch_imgin);

  return(0);
}

static int analyse(IBAND overlay, IBAND mask)
{
  region **regb, *reg, *rbg;
  int rsize, i, rnr, x, y, nregs;
  char *vec[30], infile1[20], infile2[20];
  int vnr = 1;

  logical_xor(mask, mask, mask);
  regionYline(overlay, &regb, 8, 0, 0, &rsize);

  /* For each region. */
  for(rnr = 1; rnr <= rsize; rnr++) {
    reg = regb[rnr];

    if (regionIsPoint(reg, 0, 1, 1)) continue;
    if (reg->objectnr == 0) {
      regionPoint(reg, &x, &y);
      if (x > 1) x -= 1;
      else y -= 1;

      for(i = 1; i < rnr; i++) {
	rbg = regb[i];
	if (regionIsPoint(rbg, 0, x, y)) break;
      }

      if (i == rnr) Warning(0, "No background region??\n");

      reg->next = rbg->next;
      rbg->next = reg;
    }
  }
  
  nregs = 0;
  for(rnr = 1; rnr <= rsize; rnr++) {
    reg = regb[rnr];
    if (reg->objectnr != ANALYSIS_DRAW_COL) continue;
    regionDraw(mask, reg, 1, 2);
    nregs ++;
  }
  if (nregs == 0) {
    return(Error(1, "No regions selected for analysis.\n"));
  }
  
  vec[0] = ana_prog_s;
  strcpy(infile1, "<infile>");
  strcpy(infile2, "<infile>");
  vec[vnr++] = infile1;
  vec[vnr++] = infile2;
  vec[vnr++] = NULL;

  XptyStartProgram(ana_prog_s, vec, 1, SendFiles, NULL, NULL, NULL, NULL);

/*  regionHisto(resultimg_s[bandnr_s], NULL, 1, h1);

  for(rnr = 1; rnr<=rsize; rnr++) {
    reg = regb[rnr];
    if (reg->objectnr != ANALYSIS_DRAW_COL) continue;
    regionHistoUpdate(resultimg_s[bandnr_s], reg, 1, h1);
  } 

  j=0;
  for(i=0; i<=255; i++) {
    ant += h1[i];
    sum += i*h1[i];
    sum2 += i*i*h1[i];
  }
  sprintf(buf, "Area=%10.0f, Mean=%10.2f, Stdev=%10.2f\n", 
	  ant, sum/ant-128.0, sqrt(sum2/ant - sum*sum/ant/ant));
  InformationBox(toplevel, buf); */

  return(0);
  
} /* analyse() */

static int center2(void)
{
  ImageOverlayDrawOrigo((ImageOverlayWidget) rwidgets_s[XIMAGE], 0, 0, 0);
  return(0);
}

static void nextprev(int diff)
{
  bandnr_s += diff;
  XtVaSetValues(rwidgets_s[PREV], XtNsensitive, bandnr_s > 1, NULL);
  XtVaSetValues(rwidgets_s[NEXT], XtNsensitive, bandnr_s < Inbands(img1_s),
		NULL);
  XtVaSetValues(left_s,  XtNimageBand, img1_s[bandnr_s], NULL);
  XtVaSetValues(right_s, XtNimageBand, img2_s[bandnr_s], NULL);
}

static void ximage_work(Widget wid, XtPointer client_data, XtPointer call_data)
{
  IBAND b1, b2;
  float f1, f2, off;
  int cl_data;

  cl_data = *((int *) client_data);

  switch(cl_data)
    {
    case HEADER:
      break;
    case XIMAGE: {
      ImageCallback cb;
      /* This will be executed when the "notify" action function of the
       * ImageOverlay widget (defined in the Image widget) calls the list
       * of XtNcallback functions (which includes ximage_work).
       */
      cb = (ImageCallback) call_data;
      if (cb->event == NULL || cb->event->type != MotionNotify) return;

      Message(0, "Pixel(%5d,%5d) -> %3d\n", cb->x, cb->y, 
	      *((unsigned char *) cb->data));
    }
    return;
    case INFO:
      return;
      break;
    case QUIT:
      ximage_exit(NULL, NULL, NULL);
      break;
    case CONTROL:
      if (call_data == NULL) return;
      if (split_image_s == 1) return;

      split_image_s = 1;
      XtMapWidget(left_s);
      XtMapWidget(right_s);

      XtVaSetValues(rwidgets_s[ADD2],    XtNsensitive, 0, NULL);
      XtVaSetValues(rwidgets_s[IMAGE1],  XtNsensitive, 0, NULL);
      XtVaSetValues(rwidgets_s[IMAGE2],  XtNsensitive, 0, NULL);
      XtVaSetValues(rwidgets_s[NEGATE2], XtNsensitive, 0, NULL);

      XtVaSetValues(rwidgets_s[SAVE],    XtNsensitive, 0, NULL);
      Message(0, "Select control points (at least %d)\n", ncoeffs_s);
      return;
     case GRAYAREA: /* Mode button */
      if (call_data == NULL) return;

      /* Unset drawing origo, so that new curve will be started. */
      ImageOverlayDrawOrigo((ImageOverlayWidget) rwidgets_s[XIMAGE], 0, 0, 0);

      /* Set color for overlay curve. */
      ImageOverlayColor((ImageOverlayWidget) rwidgets_s[XIMAGE],
			GRAY_DRAW_COL);

      if (split_image_s == 0) return;
      split_image_s = 0;

      XtUnmapWidget(left_s);
      XtUnmapWidget(right_s);

      XtVaSetValues(rwidgets_s[ADD2],    XtNsensitive, 1, NULL);
      XtVaSetValues(rwidgets_s[IMAGE1],  XtNsensitive, 1, NULL);
      XtVaSetValues(rwidgets_s[IMAGE2],  XtNsensitive, 1, NULL);
      XtVaSetValues(rwidgets_s[NEGATE2], XtNsensitive, 1, NULL);

      XtVaSetValues(rwidgets_s[SAVE],    XtNsensitive, 1, NULL);
      Message(0, "Draw region for gray-level adjustment.\n");
      break;
     case ANALYSISAREA: /* Mode button */
      if (call_data == NULL) return;

      /* Unset drawing origo, so that new curve will be started. */
      ImageOverlayDrawOrigo((ImageOverlayWidget) rwidgets_s[XIMAGE], 0, 0, 0);

      /* Set color for overlay curve. */
      ImageOverlayColor((ImageOverlayWidget) rwidgets_s[XIMAGE],
			ANALYSIS_DRAW_COL);

      if (split_image_s == 0) return;
      split_image_s = 0;

      XtUnmapWidget(left_s);
      XtUnmapWidget(right_s);

      XtVaSetValues(rwidgets_s[ADD2],    XtNsensitive, 1, NULL);
      XtVaSetValues(rwidgets_s[IMAGE1],  XtNsensitive, 1, NULL);
      XtVaSetValues(rwidgets_s[IMAGE2],  XtNsensitive, 1, NULL);
      XtVaSetValues(rwidgets_s[NEGATE2], XtNsensitive, 1, NULL);

      XtVaSetValues(rwidgets_s[SAVE],    XtNsensitive, 1, NULL);
      break;
     case LINELENGTH: /* Mode button */
      if (call_data == NULL) return;

      /* Unset drawing origo, so that new curve will be started. */
      ImageOverlayDrawOrigo((ImageOverlayWidget) rwidgets_s[XIMAGE], 0, 0, 0);

      /* Set color for overlay curve. */
      ImageOverlayColor((ImageOverlayWidget) rwidgets_s[XIMAGE],
			LINELENGTH_DRAW_COL);

      if (split_image_s == 0) return;
      split_image_s = 0;

      XtUnmapWidget(left_s);
      XtUnmapWidget(right_s);

      XtVaSetValues(rwidgets_s[ADD2],    XtNsensitive, 1, NULL);
      XtVaSetValues(rwidgets_s[IMAGE1],  XtNsensitive, 1, NULL);
      XtVaSetValues(rwidgets_s[IMAGE2],  XtNsensitive, 1, NULL);
      XtVaSetValues(rwidgets_s[NEGATE2], XtNsensitive, 1, NULL);

      XtVaSetValues(rwidgets_s[SAVE],    XtNsensitive, 1, NULL);
      Message(0, "Select first end point.\n"); 
      break;
    case WARP: { /* Action button */
      int i;

      if (_warp(img2_s, warpimg_s, (IBAND) ptband_s, lnpoint_s, rnpoint_s,
	       ncoeffs_s, ip_s, bg_s, rss_s, qr_s))
	return;

      for (i = 1; i <= Inbands(img2_s); i++) {
	Iset_start(grayimg_s[i], Ixstart(img2_s[i]), Iystart(img2_s[i]));
	Icopy_band(warpimg_s[i], grayimg_s[i]);
      }

      /* Unset drawing origo, so that new curve will be started. */
      ImageOverlayDrawOrigo((ImageOverlayWidget) rwidgets_s[XIMAGE], 0, 0, 0);

      if (split_image_s != 0) {
	split_image_s = 0;

	XtUnmapWidget(left_s);
	XtUnmapWidget(right_s);

	XtVaSetValues(rwidgets_s[ADD2],         XtNsensitive, 1, XtNstate, 1,
		      NULL);
	XtVaSetValues(rwidgets_s[IMAGE1],       XtNsensitive, 1, XtNstate, 0,
		      NULL);
	XtVaSetValues(rwidgets_s[IMAGE2],       XtNsensitive, 1, XtNstate, 0,
		      NULL);
	XtVaSetValues(rwidgets_s[NEGATE2],      XtNsensitive, 1, XtNstate, 0,
		      NULL);

	/* Move to next mode automatically. */
	XtVaSetValues(rwidgets_s[CONTROL],      XtNstate, 0, NULL);
	XtVaSetValues(rwidgets_s[GRAYAREA],     XtNstate, 1, NULL);
	XtVaSetValues(rwidgets_s[ANALYSISAREA], XtNstate, 0, NULL);
	XtVaSetValues(rwidgets_s[LINELENGTH],   XtNstate, 0, NULL);

	/* Set color for overlay curve. */
	ImageOverlayColor((ImageOverlayWidget) rwidgets_s[XIMAGE],
			  GRAY_DRAW_COL);

	XtVaSetValues(rwidgets_s[SAVE], XtNsensitive, 1, NULL);
      }
    }
    break;
    case GRAYNON:
      if (gray(img1_s[bandnr_s], warpimg_s[bandnr_s], grayimg_s[bandnr_s],
	       botho_s, 0))
	return;

      /* Start new curve. */
      ImageOverlayDrawOrigo((ImageOverlayWidget) rwidgets_s[XIMAGE], 0, 0, 0);

      /* Move to next mode automatically. */
      XtVaSetValues(rwidgets_s[CONTROL],      XtNstate, 0, NULL);
      XtVaSetValues(rwidgets_s[GRAYAREA],     XtNstate, 0, NULL);
      XtVaSetValues(rwidgets_s[ANALYSISAREA], XtNstate, 1, NULL);
      XtVaSetValues(rwidgets_s[LINELENGTH],   XtNstate, 0, NULL);

      ImageOverlayColor((ImageOverlayWidget) rwidgets_s[XIMAGE],
			ANALYSIS_DRAW_COL);

      XtVaSetValues(rwidgets_s[SAVE], XtNsensitive, 1, NULL);
      break;
    case GRAYPAR:
      if (gray(img1_s[bandnr_s], warpimg_s[bandnr_s], grayimg_s[bandnr_s],
	       botho_s, 1))
	return;

      /* Start new curve. */
      ImageOverlayDrawOrigo((ImageOverlayWidget) rwidgets_s[XIMAGE], 0, 0, 0);

      /* Move to next mode automatically. */
      XtVaSetValues(rwidgets_s[CONTROL],      XtNstate, 0, NULL);
      XtVaSetValues(rwidgets_s[GRAYAREA],     XtNstate, 0, NULL);
      XtVaSetValues(rwidgets_s[ANALYSISAREA], XtNstate, 1, NULL);
      XtVaSetValues(rwidgets_s[LINELENGTH],   XtNstate, 0, NULL);

      ImageOverlayColor((ImageOverlayWidget) rwidgets_s[XIMAGE],
			ANALYSIS_DRAW_COL);

      XtVaSetValues(rwidgets_s[SAVE], XtNsensitive, 1, NULL);
      break;
    case ANALYSE:
      if (analyse(botho_s, maskimg_s[1]))
	return;

      Message(2,"Analysis program terminated.\n");

      XtVaSetValues(rwidgets_s[SAVE], XtNsensitive, 1, NULL);
      return;
    case CLEAR:
      /* Clear the overlay bands. */
      logical_xor(lefto_s,  lefto_s,  lefto_s);
      logical_xor(righto_s, righto_s, righto_s);
      logical_xor(botho_s,  botho_s,  botho_s);

      lnpoint_s = rnpoint_s = 0;

      ImageRedisplay((ImageWidget) left_s,  UPDATE_ZOOMPAN | UPDATE_MINMAX);
      ImageRedisplay((ImageWidget) right_s, UPDATE_ZOOMPAN | UPDATE_MINMAX);
      ImageRedisplay((ImageWidget) rwidgets_s[XIMAGE],
		     UPDATE_ZOOMPAN | UPDATE_MINMAX);

      /* Start new curve. */
      ImageOverlayDrawOrigo((ImageOverlayWidget) rwidgets_s[XIMAGE], 0, 0, 0);

      ImageOverlayColor((ImageOverlayWidget) left_s,  WARP_START_DRAW_COL);
      ImageOverlayColor((ImageOverlayWidget) right_s, WARP_START_DRAW_COL);

      Message(0, "All control points and masks cleared.\n");
      return;
    case ADD2: 
      if (call_data == NULL) return;
      neg2_s = 0;
      f_s    = 0.5;
      Message(5,"Band %d: %5.1f %% / %5.1f %%\n", bandnr_s, 50.0, 50.0);
      break;
    case IMAGE1: 
      if (call_data == NULL) return;
      neg2_s = 0;
      f_s    = 1.0;
      Message(5,"Band %d: %5.1f %% / %5.1f %%\n", bandnr_s, 100.0, 0.0);
      break;
    case IMAGE2: 
      if (call_data == NULL) return;
      neg2_s = 0;
      f_s    = 0.0;
      Message(5,"Band %d: %5.1f %% / %5.1f %%\n", bandnr_s, 0.0, 100.0);
      break;
    case NEGATE2:
      if (call_data == NULL) return;
      neg2_s = 1;
      f_s    = 0.5;
      Message(5,"Band %d: %5.1f %% / %5.1f %%\n", bandnr_s, 50.0, -50.0);
      break;
    case PREV:
      nextprev(-1);
      Message(0, "Band no %d\n",  bandnr_s);
      break;
    case NEXT: 
      nextprev(1);
      Message(0, "Band no %d\n",  bandnr_s);
      break;
    case SAVE: {
      char lab[100], *name;
      int save, i;

      save = SelectWriteFile(TopLevelWidget(wid), writeBIFF, &name);
      if (save == 0) return;

      Message(0, "Save to file.\n");

      f1  = f_s;
      f2  = neg2_s ? (f_s - 1.0) : (1.0 - f_s);
      off = neg2_s ? (-256.0 * f2) : 0;

      for(i = 1; i < Inbands(img1_s); i++)
	addw(img1_s[bandnr_s], img2_s[bandnr_s], resultimg_s[bandnr_s],
	     f1, f2, (int) off);

      sprintf(lab, "%5.1f %% / %5.1f %%", f1 * 100.0, f2 * 100.0);
      Ihistory(resultimg_s, "xwarp", lab);

      Iwrite_image(resultimg_s, name);
    }
    break;
    } /* switch(cl_data) */

  f1  = f_s;
  f2  = neg2_s ? (f_s - 1.0) : (1.0 - f_s);
  off = neg2_s ? (-256.0 * f2) : 0;

  b1  = img1_s[bandnr_s]; 
  b2  = grayimg_s[bandnr_s];

  addw(b1, b2, resultband_s, f1, f2, (int) off);
  ImageRedisplay((ImageWidget) rwidgets_s[XIMAGE],
		 UPDATE_ZOOMPAN | UPDATE_MINMAX);

} /* ximage_work() */

static void draw_callback(Widget wid, XtPointer client_data, XtPointer call_data)
{
  ImageOverlayCallback icb;
  int cl_data;

  /* This will be executed when the "draw" action function of the
   * ImageOverlay widget calls the list of XtNdrawProc functions.
   */

  cl_data = *((int *) client_data);
  icb = (ImageOverlayCallback) call_data;

  switch(cl_data) {
  case LEFT:
    if (icb->reason != DRAW_POINT) return;
    lnpoint_s++;
    ptband_s[lnpoint_s][1] =  icb->x;
    ptband_s[lnpoint_s][2] =  icb->y;
    Message(3,"Left image: %d,   Right image: %d\n", lnpoint_s, rnpoint_s); 
    return;
  case RIGHT:
    if (icb->reason != DRAW_POINT) return;
    rnpoint_s++;
    ptband_s[rnpoint_s][3] =  icb->x;
    ptband_s[rnpoint_s][4] =  icb->y;
    Message(4,"Left image: %d,   Right image: %d\n", lnpoint_s, rnpoint_s); 
    return;
  case XIMAGE:
    if (icb->value != LINELENGTH_DRAW_COL) return;
    
    /* We are now in LINELENGTH mode. */
    if (icb->event->xbutton.button != 3) return;
    if (icb->reason == DRAW_POINT) {
      Message(0, "Select second end point.\n");
      return;
    }
    if (icb->reason == DRAW_LINE) {
      Message(0, "Draw (%d %d) -> (%d %d), length: %.2f\n", 
	      icb->xorigo, icb->yorigo, icb->x, icb->y, 
	      sqrt((double) (icb->xorigo - icb->x) * (icb->xorigo - icb->x) +
		   (icb->yorigo - icb->y) * (icb->yorigo - icb->y)) );
      break;
    }
  } /* switch */

  return;

} /* draw_callback() */

static int checkimg(char **argv, IMAGE img1, IMAGE img2)
{
  int i, nbands, xsize, ysize;
  
  if (img1 == NULL || img2 == NULL) {
    return(Error(2, "Can't find image: %s.\n", argv[img1 == NULL ? 1 : 2]));
  }
  if (Inbands(img1) != Inbands(img2)) {
    return(Error(2, "Input images must have the same number of bands.\n"));
  }

  nbands = Inbands(img1);
  xsize  = Ixsize(img1[1]);
  ysize  = Iysize(img1[1]);

  for (i=1; i<=nbands; i++)
    if (Ixsize(img1[i]) != xsize || Iysize(img1[i]) != ysize ||
	Ixsize(img2[i]) != xsize || Iysize(img2[i]) != ysize) {
      return(Error(2, "All bands must be of equal size.\n"));
    }

  for (i=1; i<=nbands; i++)
    if (Ipixtyp(img1[i]) != Iu_byte_typ || Ipixtyp(img2[i]) != Iu_byte_typ) {
      return(Error(2, "All bands must have pixel type unsigned byte.\n"));
    }

  return(0);

} /* checkimg() */

int main(int argc, char **argv)
{
  IBAND b1, b2;  
  Dimension rw, rh;
  int i, b, screen;
  Display *dpy;
  double xxa[6], yya[6];
  Arg args[8];
  char *warp;
  char *local_usage =
"Usage: %s [<option>...] <inimage-1> <inimage-2> [<inimage-mask>] \n\
       where <option> may be \n\
       -c <r>        : The number of polynomial coefficients for coordinate\n\
                       transformation.\n\
       -ip <n>       : Determines the degree of the gray level interpolation\n\
                       polynomial.\n\
       -warp <q>,... <r>,... : Parameters for geometric corr. at startup. \n\
       -analyse <prg>: Invoke <prg> from the analyse button. \n\
       -rss          : Print all control points and root mean square errors.\n\
       -qr           : Print the polynomials used by the warp function.\n\
       -bg <c>       : The background pixel value.\n";

  Iset_message(1);
  Iset_abort(0);
  InitMessage(&argc, argv, ximage_app_usage_text(local_usage));

  if (argc == 1) Usage(1, NULL);

  toplevel = ximage_init(&argc, argv, "XWarp", NULL, 0);

  dpy    = XtDisplay(toplevel);
  screen = DefaultScreen(dpy);

  ncoeffs_s  = read_iswitch(&argc, argv, "-c", 3);
  ip_s       = read_iswitch(&argc, argv, "-ip", 1);
  warp       = read_switch (&argc, argv, "-warp", 1, NULL);
  ana_prog_s = read_switch (&argc, argv, "-analyse", 1, "warpExAnalysis");
  rss_s      = read_bswitch(&argc, argv, "-rss");
  qr_s       = read_bswitch(&argc, argv, "-qr");
  bg_s       = read_iswitch(&argc, argv, "-bg", 0);

  if (argc != 3 && argc != 4) Usage(2, "Illegal number of arguments.\n");

  img1_s     = Iread_image(argv[1]);
  img2_s     = Iread_image(argv[2]);
  maskfile_s = argc == 4 ? argv[3] : NULL;

  PushMessage(info, NULL, NULL, 0);

  checkimg(argv, img1_s, img2_s);

  bandnr_s  = 1;
  lnpoint_s = rnpoint_s = 0;

  b1          = img1_s[1]; 
  b2          = img2_s[1];
  resultimg_s = Icopy_init(img1_s);
  warpimg_s   = Icopy_init(img1_s);
  grayimg_s   = Icopy_init(img1_s);

  for (i=1; i<=Inbands(img2_s); i++) {
    Icopy_band(img2_s[i], warpimg_s[i]);
    Icopy_band(img2_s[i], grayimg_s[i]);
  }

  resultband_s = resultimg_s[1];

  lefto_s    = Imake_band(Iu_byte_typ, Ixsize(b1), Iysize(b1));
  righto_s   = Imake_band(Iu_byte_typ, Ixsize(b1), Iysize(b1));
  maskimg_s  = Imake_image(1, "Mask image", Iu_byte_typ,
			   Ixsize(b1), Iysize(b1));
  if (maskfile_s == NULL)
    botho_s = Imake_band(Iu_byte_typ, Ixsize(b1), Iysize(b1));
  else botho_s = (Iread_image(maskfile_s))[1];

  /* Initialize overlay bands to all zeros. */
  logical_xor(lefto_s,  lefto_s,  lefto_s);
  logical_xor(righto_s, righto_s, righto_s);
  if (maskfile_s == NULL) logical_xor(botho_s, botho_s, botho_s);

  ptband_s = (IR_BAND) Imake_band(Ireal_typ, 4, 100);
  Ihistory(resultimg_s, argv[0], "End of Image 1 history");
  Icopy_text(img2_s, resultimg_s);
  Ihistory(resultimg_s, argv[0], "End of Image 2 history");

  if (warp) {
    switch (ncoeffs_s) {
    case 1:
      sscanf(warp, "%lf%lf", &xxa[0], &yya[0]);
      break;
    case 2:
      sscanf(warp, "%lf%lf%lf%lf",
	     &xxa[0], &xxa[1],
	     &yya[0], &yya[1]);
      break;
    case 3:
      sscanf(warp, "%lf%lf%lf%lf%lf%lf",
	     &xxa[0], &xxa[1], &xxa[2],
	     &yya[0], &yya[1], &yya[2]);
      break;
    case 4:
      sscanf(warp, "%lf%lf%lf%lf%lf%lf%lf%lf",
	     &xxa[0], &xxa[1], &xxa[2], &xxa[3],
	     &yya[0], &yya[1], &yya[2], &yya[3]);
      break;
    case 5:
      sscanf(warp, "%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf",
	     &xxa[0], &xxa[1], &xxa[2], &xxa[3], &xxa[4],
	     &yya[0], &yya[1], &yya[2], &yya[3], &yya[4]);
      break;
    case 6:
      sscanf(warp, "%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf",
	     &xxa[0], &xxa[1], &xxa[2], &xxa[3], &xxa[4], &xxa[5],
	     &yya[0], &yya[1], &yya[2], &yya[3], &yya[4], &yya[5]);
      break;
    } /* switch */

    for (i = 1; i <= Inbands(img2_s); i++)
      Iset_start(warpimg_s[i], Ixstart(img2_s[i]), Iystart(img2_s[i]));

    if (warp_resample(img2_s, warpimg_s, xxa, yya, ncoeffs_s, ip_s, bg_s))
      return(Error(1, "In transform().\n"));

    for (i = 1; i <= Inbands(img2_s); i++) {
      Iset_start(grayimg_s[i], Ixstart(img2_s[i]), Iystart(img2_s[i]));
      Icopy_band(warpimg_s[i], grayimg_s[i]);
    }

    b2 = grayimg_s[1];
  }

  addw(b1, b2, resultband_s, 0.5, 0.5, 0);

  ximage_layout(toplevel, resultband_s, widgets_s, rwidgets_s, WIDGETS,
		ximage_work, NULL);

  XtVaSetValues(rwidgets_s[CONTROL],      XtNradioGroup, rwidgets_s[CONTROL],
		NULL);
  XtVaSetValues(rwidgets_s[GRAYAREA],     XtNradioGroup, rwidgets_s[CONTROL],
		NULL);
  XtVaSetValues(rwidgets_s[ANALYSISAREA], XtNradioGroup, rwidgets_s[CONTROL],
		NULL);
  XtVaSetValues(rwidgets_s[LINELENGTH],   XtNradioGroup, rwidgets_s[CONTROL],
		NULL);
  XtVaSetValues(rwidgets_s[CONTROL],      XtNstate,      1,
		NULL);

  XtVaSetValues(rwidgets_s[ADD2],    XtNradioGroup, rwidgets_s[ADD2], NULL);
  XtVaSetValues(rwidgets_s[IMAGE1],  XtNradioGroup, rwidgets_s[ADD2], NULL);
  XtVaSetValues(rwidgets_s[IMAGE2],  XtNradioGroup, rwidgets_s[ADD2], NULL);
  XtVaSetValues(rwidgets_s[NEGATE2], XtNradioGroup, rwidgets_s[ADD2], NULL);
  XtVaSetValues(rwidgets_s[ADD2],    XtNstate,      1,                NULL);

  XtVaGetValues(rwidgets_s[XIMAGE], XtNwidth, &rw, XtNheight, &rh, NULL);

  /*
    Prepare a widget, half the width of rwidgets_s[XIMAGE], initially
    containing the left half of img1_s. This widget will be displayed
    in the left half of rwidget_s[XIMAGE]. Also prepare a similar
    widget, initially containing the left half of img2_s. This widget
    will be displayed in the right half of rwidget_s[XIMAGE]. When these
    widgets are mapped on top of resultband_s, it becomes invisible.

    Both of the widget-halves can be be scrolled/zoomed individually.
  */

  args[0].name  = XtNoverlay;
  args[0].value = (XtArgVal) lefto_s;
  args[1].name  = XtNwidth;
  args[1].value = (XtArgVal) (rw / 2);
  args[2].name  = XtNheight;
  args[2].value = (XtArgVal) rh;
  args[3].name  = XtNx;
  args[3].value = (XtArgVal) 0;
  args[4].name  = XtNy;
  args[4].value = (XtArgVal) 0;
  args[5].name  = XtNdrawColor;
  args[5].value = (XtArgVal) 1;
  args[6].name  = XtNborderWidth;
  args[6].value = (XtArgVal) 0;
  args[7].name  = XtNvariableAspect;
  args[7].value = True; /* To allow separate setting of height and width. */

  /* The widget does not have its own window, it will appear inside
     rwidgets_s[XIMAGE] which is already visible (ximage_layout above
     calls XtPopup).
  */
  left_s  = ximage_display(rwidgets_s[XIMAGE],
			   "left", NULL, img1_s, 1, imageOverlayWidgetClass,
			   NULL, 0, 0, args, 8, NULL);

  XtVaSetValues(left_s, XtNvariableAspect, False, NULL);

  args[0].value = (XtArgVal) righto_s;
  args[1].value = (XtArgVal) ((rw + 1)/ 2);
  args[3].value = (XtArgVal) (rw / 2);

  right_s = ximage_display(rwidgets_s[XIMAGE],
			   "right", NULL, img2_s, 1, imageOverlayWidgetClass,
			   NULL, 0, 0, args, 8, NULL);

  XtVaSetValues(right_s, XtNvariableAspect, False, NULL);

  /* Callback functions to invoke before returning from the "draw" action
   * function.
   */
  XtAddCallback(left_s,  XtNdrawProc, draw_callback, MallocIntP(LEFT));
  XtAddCallback(right_s, XtNdrawProc, draw_callback, MallocIntP(RIGHT));
  XtAddCallback(rwidgets_s[XIMAGE], XtNdrawProc, draw_callback,
		MallocIntP(XIMAGE));

  if (warp) {
    split_image_s = 0;
    XtUnmapWidget(left_s);
    XtUnmapWidget(right_s);
    XtVaSetValues(rwidgets_s[CONTROL],      XtNstate, 0, NULL);
    XtVaSetValues(rwidgets_s[GRAYAREA],     XtNstate, 1, NULL);
    XtVaSetValues(rwidgets_s[ANALYSISAREA], XtNstate, 0, NULL);
  } else {
    XtVaSetValues(rwidgets_s[ADD2],    XtNsensitive, 0, NULL);
    XtVaSetValues(rwidgets_s[IMAGE1],  XtNsensitive, 0, NULL);
    XtVaSetValues(rwidgets_s[IMAGE2],  XtNsensitive, 0, NULL);
    XtVaSetValues(rwidgets_s[NEGATE2], XtNsensitive, 0, NULL);

    XtVaSetValues(rwidgets_s[SAVE],    XtNsensitive, 0, NULL);
  }
  
  XtVaSetValues(rwidgets_s[XIMAGE], XtNoverlay,           botho_s,
		                    XtNoverlayColorCells, 32,
		                    XtNoverlayColorPtr,   Xcolor_deftab(32),
		                    NULL);
  ImageOverlayRedisplay(rwidgets_s[XIMAGE], UPDATE_RESIZE);

  for(i=0; i<WIDGETS; i++) {
    if (ewidgets_s[i])
       XtAddEventHandler(rwidgets_s[i],
			 (EventMask) (EnterWindowMask|LeaveWindowMask),
			 False,
			 (XtEventHandler) bottomInfoHandler,
			 (XtPointer) ewidgets_s[i]);
  }

  nextprev(0);
  Message(0, "Select control points.\n");
  XptyInit(dpy, screen, toplevel);
  ximage_mainloop();

  return(0);
}
