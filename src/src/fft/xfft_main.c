
/*

________________________________________________________________

        xfft.c
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

/*P:xfft*

________________________________________________________________

		xfft
________________________________________________________________

Name:		xfft - X based interactive 2D fourier transform and display

Syntax:		xfft [<option>...] <inimage>

Description:	'xfft' lets you interactively display the 2D fourier
                transform of parts of an image. Initially, the input
		image is displayed in one window, and the 2D fourier
		transform of the upper, left corner in another window.

		You may move the processing window by pushing the left mouse
		button at the desired new position in the image, or change the
		size of the processing window by pushing the corresponding
		screen buttons 'expand' and 'shrink'.

		The 'save' button can be used to save the fourier window in a
		file, while the 'info' button gives information about current
		sizes and positions.

		The display in the fourier window is created with the
		'fftDisplay' function after taking the 2D fourier transform
		(using the function 'fft2d'), with the 'scale' argument of
		'fftDisplay' equal to 10.0.

Options:        'xfft' accepts all X toolkit command-line options (see X(1))
                as well as the XITE 'ximage(3)' toolkit options. Additionally
		the following options may be useful:

		&-xrm 'XFft*ImageOverlay.roiX: x'
		Set horizontal coordinate 'x' for upper-left corner of
		region in spatial-domain image to be Fourier-transformed.

		&-xrm 'XFft*ImageOverlay.roiY: y'
		Set vertical coordinate 'y' for upper-left corner of region in
		spatial-domain image to	be Fourier-transformed.

		&-xrm 'XFft*ImageOverlay.roiWidth: width'
		Set 'width' of region in spatial-domain image to be
		Fourier-transformed.

		&-xrm 'XFft*ImageOverlay.roiHeight: height'
		Set 'height' of region in spatial-domain image to be
		Fourier-transformed. This must equal the width of the region.

Resources:      In order to specify resources, it is useful to know
		the  hierarchy of the widgets which compose 'xfft'.
		In the notation below, indentation indicates hierarchical 
		structure.

		The following hierarchy is constructed when the default
		resources/options for widget layout is used.

		The widgets of class XfwfButton can be replaced by class
                Command (from the Athena widget set) by using the 'ximage(3)'
                option '-widgetset' or corresponding application resource.

		&xfft                (class XFft)
		| imageShell         (class TopLevelShell)
		|   imageForm        (class ImageForm)
		|     image          (class ImageOverlay)
		|     menubar        (class XfwfMenuBar)
		|       colors       (class XfwfPullDown)
		|       overlays     (class XfwfPullDown)
		|       tools        (class XfwfPullDown)
		|       options      (class XfwfPullDown)
		| imageShell         (class TopLevelShell)
		|   imageForm        (class ImageForm)
		|     image          (class ImageOverlay)
		|     menubar        (class XfwfMenuBar)
		|       colors       (class XfwfPullDown)
		|       tools        (class XfwfPullDown)
		|       options      (class XfwfPullDown)
		| xfft control panel (class TopLevelShell)
		|   topContainer     (class Form)
		|     quit           (class XfwfButton)
		|     expand         (class XfwfButton)
		|     shrink         (class XfwfButton)
		|     save           (class XfwfButton)
		|     info           (class XfwfButton)
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

Return value:   0 when ok, 1: Can''t create subband, 2: Couldn''t allocate
                memory.

See also:       'fft2d(3)', 'fftDisplay(3)', 'ximage(3)', 'Image(3)',
                'ImageOverlay(3)'

Files:          $XITE_HOME/etc/app-defaults/XFft

Author:		Tor Lønnestad and Svein Bøe, BLAB, Ifi, UiO

Examples:       xfft mona.img

Id: 		$Id$
________________________________________________________________

*/

#include <xite/includes.h>
#include <X11/Xos.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <xite/biff.h>
#include <stdlib.h>
#include <math.h>
#include <xite/color.h>
#include <xite/convert.h>
#include <xite/fft.h>
#include <xite/message.h>
#include <xite/ximage.h>
#include <xite/ImageOverlay.h>
#include <xite/Xdialog.h>
#include <xite/ShellWids.h>
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#include <xite/statistics.h>
#include <xite/mallocp.h>
#include <xite/debug.h>

#ifndef MIN
# define MIN(a,b)	(((a)<(b)) ? (a) : (b))
#endif
#ifndef MAX
# define MAX(a,b)	(((a)>(b)) ? (a) : (b))
#endif

enum {
  XIMAGE1,
  QUIT,
  EXPAND,
  SHRINK,
  SAVE,
  INFO
};
#define WIDGETS1 (INFO + 1)

enum {
  XIMAGE2
};
#define WIDGETS2 (XIMAGE2 + 1)

/* Create the spatial image first, so that roi resources can be read before
 * creating the fourier-domain image.
 */
static char *widgets1[WIDGETS1] = {
  "Overlay:spatial",
  "Command:quit:xfft control panel",
  "Command:expand",
  "Command:shrink",
  "Command:save",
  "Command:info",
};

static char *widgets2[WIDGETS2] = {
  "Image:fourier",
};

static Widget rwidgets1[WIDGETS1];
static Widget rwidgets2[WIDGETS2];

static ImageOverlayWidget imageWid_s;
static ImageWidget fftImageWid_s;
static IMAGE save_img_s;
static IBAND orgband_s, fftband_s;



static void fourier(IBAND band, IBAND fftband)
{
  static IC_BAND cb1 = NULL, cb2 = NULL;
  int size;

  size = Ixsize(band);
  if (cb1) {
    if (Ixsize((IBAND) cb1) != size) {
      Idel_band((IBAND *) &cb1);
      Idel_band((IBAND *) &cb2);
    }
  }
  if (!cb1) {
    cb1 = (IC_BAND)Imake_band(Icomplex_typ, size, size);
    cb2 = (IC_BAND)Imake_band(Icomplex_typ, size, size);
  }
  convertBand(band, (IBAND) cb1);
  if (fft2d((IBAND) cb1, (IBAND) cb2, FALSE, 1.0) != Iok) {
    Error(1, "fourier: Error in fft2d routine.\n");
    exit(1);
  }
  fftDisplay((IBAND) cb2, fftband, (double)10.0);
}



static int fft_pow2(int length)
{
  int i, pow;

  i = 1; pow=0;
  while (i < length)
    {
      i <<= 1;
      pow++;
    }
  return(pow);
}

static int set_to_pow2(int length, int flag)
{
  int exponent;

  exponent = fft_pow2(length);

  if (length < 1) return(length);

  if (pow((double) 2.0, (double) exponent) != length) {
    /* Go down or up. */
    if (flag == 1) length = pow((double) 2.0, (double) (exponent - 1));
    else length = pow((double) 2.0, (double) (exponent));
  }

  return(length);

} /* set_to_pow2() */

static void update_fourier(Widget wid, int roix_im, int roiy_im, int roixsize_im, int roiysize_im)
{
  IBAND subband, old_fftband;

  subband = Imake_subband(orgband_s, roix_im, roiy_im,
			  roixsize_im, roiysize_im);
  if (!subband) {
    Error(2, "xfft: Can't create subband.\n");
  }

  old_fftband = fftband_s;
  if (roixsize_im != Ixsize(fftband_s) || roiysize_im != Iysize(fftband_s)) {
    Idel_band((IBAND *) &fftband_s);
    fftband_s = Imake_band(Ireal_typ, roixsize_im, roiysize_im);
  }
  if (!fftband_s) {
    Error(2, "xfft: Couldn't allocate memory!\n");
  }

  fourier(subband, fftband_s);

  /* Update band pointer, but delay redisplay. */
  XtVaSetValues((Widget) fftImageWid_s,
		XtNimageBand, fftband_s,
		XtNimageUpdate, UPDATE_NONE, /* Delay redisplay. */
		NULL);

  /* Recalculate min/max and redisplay. */
  if (old_fftband == fftband_s)
    ImageRedisplay(fftImageWid_s, UPDATE_ZOOMPAN | UPDATE_MINMAX);
  else
    ImageRedisplay(fftImageWid_s, UPDATE_NEWSIZE | UPDATE_MINMAX);

  Idel_band(&subband);

} /* update_fourier() */


static void Ximage(Widget wid, XtPointer client_data, XtPointer roi)
{
  ImageOverlayCallback iroi = (ImageOverlayCallback) roi;
  int roix, roiy, roixsize, roiysize;
  int roix_im, roiy_im, roixsize_im, roiysize_im;

  if (iroi->value != ROI_MOVE) return;

  /* Coordinates in call-data roi are relative to image, not screen. */
  roix_im      = iroi->x;
  roiy_im      = iroi->y;
  roixsize_im  = iroi->width;
  roiysize_im  = iroi->height;

  /* Check that roi size is power of 2. If not, use a smaller size.
   */
  if (roixsize_im > 0) roixsize_im = set_to_pow2(roixsize_im, 1);
  else roixsize_im = 1;
  if (roiysize_im > 0) roiysize_im = set_to_pow2(roiysize_im, 1);
  else roiysize_im = 1;

  /* Translate to screen coordinates. */
  roix      = ImagePosXtoScreen((ImageWidget) imageWid_s, roix_im, 0);
  roiy      = ImagePosYtoScreen((ImageWidget) imageWid_s, roiy_im, 0);
  roixsize  = ImageWidthToScreen((ImageWidget) imageWid_s, roixsize_im, 1);
  roiysize  = ImageHeightToScreen((ImageWidget) imageWid_s, roiysize_im, 1);

  XtVaSetValues((Widget) imageWid_s, XtNroiX,      roix,
		                     XtNroiY,      roiy,
		                     XtNroiWidth,  roixsize,
		                     XtNroiHeight, roiysize,
		                     NULL);

  update_fourier(wid, roix_im, roiy_im, roixsize_im, roiysize_im);

} /* Ximage() */

static void Quit(Widget wid, XtPointer client_data, XtPointer call_data)
{
  ENTER_FUNCTION_DEBUG("xfft.c: Quit");

  if (ConfirmationBox(TopLevelWidget(wid), "Really quit?", "No", "Yes",
		      "xfft - confirmation", "xfft - conf.") != 1 ) {
    LEAVE_FUNCTION_DEBUG("xfft.c: Quit");
    return;
  }
  XtDestroyApplicationContext(XtWidgetToApplicationContext(wid));

  LEAVE_FUNCTION_DEBUG("xfft.c: Quit");
  exit(0);
}

static void Expand(Widget wid, XtPointer client_data, XtPointer call_data)
{
  int roix, roiy, roixsize, roiysize, im_width, im_height;
  int roix_im, roiy_im, roixsize_im, roiysize_im;
  Dimension core_w, core_h;

  XtVaGetValues((Widget) imageWid_s, XtNroiX,        &roix,
		                     XtNroiY,        &roiy,
                                     XtNroiWidth,    &roixsize,
		                     XtNroiHeight,   &roiysize,
		                     XtNimageWidth,  &im_width,
		                     XtNimageHeight, &im_height,
		                     XtNwidth,       &core_w,
		                     XtNheight,      &core_h,
		                     NULL);

  roix_im     = ImageScreenXtoPos((ImageWidget) imageWid_s, roix);
  roiy_im     = ImageScreenYtoPos((ImageWidget) imageWid_s, roiy);
  roixsize_im = ImageScreenToWidth((ImageWidget) imageWid_s, roixsize);
  roiysize_im = ImageScreenToHeight((ImageWidget)imageWid_s, roiysize);

  if (roixsize_im > im_width/2)  return;
  if (roiysize_im > im_height/2) return;

  roixsize_im *= 2;
  roiysize_im *= 2;

  roixsize_im = set_to_pow2(roixsize_im, 2);
  roiysize_im = set_to_pow2(roiysize_im, 2);

  if (roix_im + roixsize_im - 1 > im_width)
    roix_im = im_width - roixsize_im + 1;
  if (roiy_im + roiysize_im - 1 > im_height)
    roiy_im = im_height - roiysize_im + 1;

  roix     = ImagePosXtoScreen((ImageWidget) imageWid_s, roix_im, 0);
  roiy     = ImagePosYtoScreen((ImageWidget) imageWid_s, roiy_im, 0);
  roixsize = ImageWidthToScreen((ImageWidget) imageWid_s, roixsize_im, 1);
  roiysize = ImageHeightToScreen((ImageWidget) imageWid_s, roiysize_im, 1);

  XtVaSetValues((Widget) imageWid_s, XtNroiX,      roix,
		                     XtNroiY,      roiy,
                                     XtNroiWidth,  roixsize,
		                     XtNroiHeight, roiysize,
		                     NULL);

  update_fourier(wid, roix_im, roiy_im, roixsize_im, roiysize_im);

} /* Expand() */

static void Shrink(Widget wid, XtPointer client_data, XtPointer call_data)
{
  int roix, roiy, roixsize, roiysize;
  int roix_im, roiy_im, roixsize_im, roiysize_im;
  Dimension core_w, core_h;

  XtVaGetValues((Widget) imageWid_s, XtNroiX,      &roix,
		                     XtNroiY,      &roiy,
                                     XtNroiWidth,  &roixsize,
		                     XtNroiHeight, &roiysize,
		                     XtNwidth,     &core_w,
		                     XtNheight,    &core_h,
		                     NULL);

  roix_im     = ImageScreenXtoPos((ImageWidget) imageWid_s, roix);
  roiy_im     = ImageScreenYtoPos((ImageWidget) imageWid_s, roiy);
  roixsize_im = ImageScreenToWidth((ImageWidget) imageWid_s, roixsize);
  roiysize_im = ImageScreenToHeight((ImageWidget)imageWid_s, roiysize);

  if (roixsize_im <= 2 || roiysize_im <= 2) return;

  roixsize_im /= 2;
  roiysize_im /= 2;

  roixsize_im = set_to_pow2(roixsize_im, 2);
  roiysize_im = set_to_pow2(roiysize_im, 2);

  roix     = ImagePosXtoScreen((ImageWidget) imageWid_s, roix_im, 0);
  roiy     = ImagePosYtoScreen((ImageWidget) imageWid_s, roiy_im, 0);
  roixsize = ImageWidthToScreen((ImageWidget) imageWid_s, roixsize_im, 1);
  roiysize = ImageHeightToScreen((ImageWidget) imageWid_s, roiysize_im, 1);

  XtVaSetValues((Widget) imageWid_s, XtNroiX,      roix,
		                     XtNroiY,      roiy,
                                     XtNroiWidth,  roixsize,
		                     XtNroiHeight, roiysize,
		                     NULL);

  update_fourier(wid, roix_im, roiy_im, roixsize_im, roiysize_im);

} /* Shrink() */

static void Save(Widget wid, XtPointer client_data, XtPointer call_data)
{
  char* name;
  int status;

  if ((status =
       SelectWriteFile(TopLevelWidget(wid), writeBIFF, &name)) != 0) {

    save_img_s[1] = fftband_s;
    if ((status = Iwrite_image(save_img_s, name)) != Iok)
      InformationBox(TopLevelWidget(wid), Ierr_message(status),
		      "xfft - information", "xfft - info");
    save_img_s[1] = NULL; /* Not to de-alloc. static storage fftband_s. */
    Idel_image(&save_img_s);
  }
}

static void info(Widget wid, XtPointer client_data, XtPointer call_data)
{
  char msg[1000];
  Dimension core_w, core_h, core_fft_w, core_fft_h;
  int fft_width, fft_height;
  int im_width, im_height, roix, roiy, roixsize, roiysize;
  int roix_im = -1, roiy_im = -1, roixsize_im, roiysize_im;


  XtVaGetValues((Widget) fftImageWid_s,
		                     XtNimageWidth,  &fft_width,
		                     XtNimageHeight, &fft_height,
                                     XtNwidth,       &core_fft_w,
		                     XtNheight,      &core_fft_h, NULL);
  XtVaGetValues((Widget) imageWid_s, XtNimageWidth,  &im_width,
		                     XtNimageHeight, &im_height,
		                     XtNwidth,       &core_w,
		                     XtNheight,      &core_h,
		                     XtNroiX,        &roix,
		                     XtNroiY,        &roiy,
                                     XtNroiWidth,    &roixsize,
                                     XtNroiHeight,   &roiysize, NULL);

  if (roix >= 0) {
    roix_im = ImageScreenXtoPos((ImageWidget) imageWid_s, roix);
  } else {
    roix_im = ImageScreenXtoPos((ImageWidget) imageWid_s, 0) -
      ImageScreenToWidth((ImageWidget) imageWid_s, -roix);
  }

  if (roiy >= 0) {
    roiy_im = ImageScreenYtoPos((ImageWidget) imageWid_s, roiy);
  } else {
    roiy_im = ImageScreenYtoPos((ImageWidget) imageWid_s, 0) -
      ImageScreenToHeight((ImageWidget) imageWid_s, -roiy);
  }

  roixsize_im = ImageScreenToWidth((ImageWidget) imageWid_s, roixsize);
  roiysize_im = ImageScreenToHeight((ImageWidget)imageWid_s, roiysize);
  roixsize_im = set_to_pow2(roixsize_im, 2);
  roiysize_im = set_to_pow2(roiysize_im, 2);

/*
  sprintf(msg, 
" Spatial-domain image\n\
 ====================\n\
   Image data\n\
     Size of original image         : %d x %d\n\
     Size of fft-subarea            : %d x %d\n\
     Position of fft_subarea        : (%d, %d)\n\
   Screen window data\n\
     Screen-size of window          : %d x %d\n\
     Screen-size of fft-subarea     : %d x %d\n\
     Screen-position of fft_subarea : (%d, %d)\n\
\n\
 Fourier-domain image\n\
 ====================\n\
   Image data\n\
     Size of image                  : %d x %d\n\
   Screen window data\n\
     Screen-size of window          : %d x %d", 
  im_width,    im_height,
  roixsize_im, roiysize_im,
  roix_im,     roiy_im,

  core_w,      core_h,
  roixsize,    roiysize,
  roix,        roiy,

  fft_width,   fft_height,
  core_fft_w,  core_fft_h);

*/
  sprintf(msg, 
" Spatial-domain image\n\
 ====================\n\
   Size of original image         : %d x %d\n\
   Size of fft-subarea            : %d x %d\n\
   Position of fft_subarea        : (%d, %d)\n\
\n\
 Fourier-domain image\n\
 ====================\n\
   Size of image                  : %d x %d",
  im_width,    im_height,
  roixsize_im, roiysize_im,
  roix_im,     roiy_im,

  fft_width,   fft_height);

  InformationBox(TopLevelWidget(wid), msg,
		 "xfft - information", "xfft - info");
}




static void ximage_work(Widget wid, XtPointer c_data, XtPointer call_data)
{
  int client_data = *((int *) c_data);

  ENTER_FUNCTION_DEBUG("xfft.c: ximage_work");

  FPRINTF2("    client_data=%d\n", client_data);
  switch (client_data)
    {
    case XIMAGE1: break;
    case QUIT   : Quit(wid, c_data, call_data); break;
    case EXPAND : Expand(wid, c_data, call_data); break;
    case SHRINK : Shrink(wid, c_data, call_data); break;
    case SAVE   : Save(wid, c_data, call_data); break;
    case INFO   : info(wid, c_data, call_data); break;
    default     : Warning(2, "Callback from unknown widget.\n");
    }

  LEAVE_FUNCTION_DEBUG("xfft.c: ximage_work");

} /* ximage_work() */

int main(int argc, char **argv)
{
  Widget top;
  IBAND subband;
  IMAGE input_img;
  int roix, roiy, roixsize, roiysize;
  int roix_im, roiy_im, roixsize_im, roiysize_im;
  int xsize_max, ysize_max;
  Dimension core_w, core_h;
  char *title;
  char *local_usage =
  "Usage: %s [<option>...] <filename>\n\
   An <option> may be\n\
   -xrm 'XFft*ImageOverlay.roiX: x'           : Hor. coordinate of roi. \n\
   -xrm 'XFft*ImageOverlay.roiY: y'           : Vert. coordinate of roi. \n\
   -xrm 'XFft*ImageOverlay.roiWidth: width'   : Width of roi.\n\
   -xrm 'XFft*ImageOverlay.roiHeight: height' : Height of roi.\n";

  Iset_message(1);
  Iset_abort(0);
  InitMessage(&argc, argv, ximage_app_usage_text(local_usage));

  /* Not sending along fallback-resources because a real app-defaults file
   * (i.e. one which is located in the standard system-wide directory) is
   * not used in the standard XITE distribution. The defaults file is
   * located in a directory local to XITE.
   */

  top = ximage_init(&argc, argv, "XFft", NULL, True);
/*                        NULL, */ /* resources        */
/*                        0,    */ /* num_resources    */
/*                        NULL, */ /* resources result */
/*                        NULL, */ /* options          */
/*                        0,    */ /* num_options      */
/*                        NULL, */ /* fallback         */
/*                        TRUE  */ /* warn             ); */

  if (argc != 2) Usage(0, NULL);

  input_img = Iread_image(argv[1]);
  if (input_img == NULL) Error(2, "Can't open input file %s", argv[1]);
  Iset_message(FALSE);

  orgband_s = input_img[1];

  /* Create the spatial image first, so that ROI resources can be read and
   * checked before creating the Fourier domain image.
   */

  ximage_layout(top, orgband_s, widgets1, rwidgets1, WIDGETS1, ximage_work,
		NULL);
  imageWid_s = (ImageOverlayWidget) rwidgets1[XIMAGE1];
  XtAddCallback((Widget) imageWid_s, XtNroiProc, Ximage, NULL);

  XtVaGetValues((Widget) imageWid_s, XtNroiX,      &roix,
		                     XtNroiY,      &roiy,
		                     XtNroiWidth,  &roixsize,
		                     XtNroiHeight, &roiysize,
			             XtNwidth,     &core_w,
			             XtNheight,    &core_h,
			             NULL);

  xsize_max = Ixsize(orgband_s);
  ysize_max = Iysize(orgband_s);

  /* ROI resources are relative to screen coordinate system.
   * Translate to image coordinate system.
   */

  roix_im     = ImageScreenXtoPos((ImageWidget) imageWid_s, roix);
  roiy_im     = ImageScreenYtoPos((ImageWidget) imageWid_s, roiy);
  roixsize_im = ImageScreenToWidth((ImageWidget) imageWid_s, roixsize);
  roiysize_im = ImageScreenToHeight((ImageWidget)imageWid_s, roiysize);

  /* Limit the size, in image coord. system. */
  if (roixsize_im <= 0) roixsize_im = MIN(32, xsize_max / 2);
  else roixsize_im = MIN(xsize_max / 2, roixsize_im);
  if (roiysize_im <= 0) roiysize_im = MIN(32, ysize_max / 2);
  else roiysize_im = MIN(ysize_max / 2, roiysize_im);

  /* Make image-resource ROI size power of two, in image coord. system. */
  roixsize_im = set_to_pow2(roixsize_im, 2);
  roiysize_im = set_to_pow2(roiysize_im, 2);

  /* Determine roix and roiy in image coordinate system. */
  if (roix_im < 1 || roix_im > xsize_max || roix_im + roixsize_im > xsize_max)
    roix_im = xsize_max - roixsize_im + 1;
  if (roiy_im < 1 || roiy_im > ysize_max || roiy_im + roiysize_im > ysize_max)
    roiy_im = ysize_max - roiysize_im + 1;

  /* Translate back to screen coordinate system. */
  roix     = ImagePosXtoScreen((ImageWidget) imageWid_s, roix_im, 0);
  roiy     = ImagePosYtoScreen((ImageWidget) imageWid_s, roiy_im, 0);
  roixsize = ImageWidthToScreen((ImageWidget) imageWid_s, roixsize_im, 1);
  roiysize = ImageHeightToScreen((ImageWidget) imageWid_s, roiysize_im, 1);

  XtVaSetValues((Widget) imageWid_s, XtNroiX,      roix,
		                     XtNroiY,      roiy,
		                     XtNroiWidth,  roixsize,
		                     XtNroiHeight, roiysize,
		                     NULL);

  subband = Imake_subband(orgband_s, roix_im, roiy_im,
			  roixsize_im, roiysize_im);
  if (!subband) {
    Error(2, "xfft: Can't create subband\n");
    exit(1);
  }

  fftband_s = Imake_band(Ireal_typ, roixsize_im, roiysize_im);
  if (! fftband_s) {
    Error(2, "xfft: Couldn't allocate memory!\n");
    exit(2);
  }

  fourier(subband, fftband_s);
  Idel_band(&subband);

  title = (char *) malloc(strlen(Ititle(input_img)) +
			  strlen("Transform of ") + 1);
  (void) strcpy(title, "Transform of ");
  (void) strcat(title, Ititle(input_img));
  save_img_s    = Init_image(1, title);
  save_img_s[1] = fftband_s;

  ximage_layout(top, fftband_s, widgets2, rwidgets2, WIDGETS2, NULL, NULL);

  fftImageWid_s = (ImageWidget) rwidgets2[XIMAGE2];

  ximage_mainloop();
  return(0);
}
