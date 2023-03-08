#
#      Makefile.am for lib/Image
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxiteXImage.la

MANF =  ImageError.3 ImageHasColormap.3 ImageInstallPixelType.3 \
	ImageRedisplay.3 ImageRedisplayScreen.3 ImageRedisplayArea.3 \
	ImageUpdateMinMax.3 ImageKill.3 ImageScreenXtoPos.3 \
	ImageScreenYtoPos.3 ImageScreenXYtoAdr.3 ImageScreenToHeight.3 \
	ImageScreenToWidth.3 ImageHeightToScreen.3 ImageWidthToScreen.3 \
	ImagePosXYtoAdr.3 ImageTranslateZoomPanArgs.3 ImagePosXtoScreen.3 \
	ImagePosYtoScreen.3 ImageGetZoom.3 ImageSetZoom.3 \
	ImageCopyImagePart.3 ImageCopyResources.3 ImageGetResources.3 \
	ImageGetImageResources.3 ImageGetZoomAll.3 ImageSetZoomAll.3 \
	xiteImage.3 \
	ImageTitle.3 ImageAddress.3 ImageWidth.3 ImageHeight.3 \
	ImageLineLength.3 ImageXstart.3 ImageYstart.3 ImagePixName.3 \
	ImagePixPrintFormat.3 ImagePixNameToBIFFPixTyp.3 ImageFormat.3 \
	ImageOverlayColor.3 ImageOverlayCopyImageOverlayPart.3 \
	ImageOverlayCopyImagePart.3 ImageOverlayCopyResources.3 \
	ImageOverlayGetResources.3 ImageOverlayDisplay.3 \
	ImageOverlayDrawBox.3 ImageOverlayDrawLine.3 \
	ImageOverlayDrawOrigo.3 ImageOverlayDrawPoint.3 \
	ImageOverlayDrawRectangle.3 ImageOverlayFill.3 \
	ImageOverlayPosXYtoAdr.3 ImageOverlayProtect.3 \
	ImageOverlayRedisplay.3 ImageOverlayScreenXYtoAdr.3 \
	ImageOverlay.3 \
	ImageByteResize.3 ImageOverlayResize.3 ImageSignedByteResize.3 \
	ImageUnsignedShortResize.3 ImageShortResize.3 \
	ImageLongResize.3 ImageFloatResize.3 ImageDoubleResize.3 \
	ImageResize.3

man3_MANS=$(MANF)

libxiteXImage_la_SOURCES = Image.c ImageFormat.c ImageBytePix.c ImageSignedBytePix.c \
	ImageShortPix.c ImageUnsignedShortPix.c ImageLongPix.c \
	ImageFloatPix.c ImageDoublePix.c ImageOverlay.c \
	ImageOverlayPix.c ImageComplexPix.c ImageDoubleComplexPix.c

ImageError.3:	xiteImage.3
ImageHasColormap.3:	xiteImage.3
ImageInstallPixelType.3:	xiteImage.3
ImageRedisplay.3:	xiteImage.3
ImageRedisplayScreen.3:	xiteImage.3
ImageRedisplayArea.3:	xiteImage.3
ImageUpdateMinMax.3:	xiteImage.3
ImageKill.3:	xiteImage.3
ImageScreenXtoPos.3:	xiteImage.3
ImageScreenYtoPos.3:	xiteImage.3
ImageScreenXYtoAdr.3:	xiteImage.3
ImageScreenToHeight.3:	xiteImage.3
ImageScreenToWidth.3:	xiteImage.3
ImageHeightToScreen.3:	xiteImage.3
ImageWidthToScreen.3:	xiteImage.3
ImagePosXYtoAdr.3:	xiteImage.3
ImageTranslateZoomPanArgs.3:	xiteImage.3
ImagePosXtoScreen.3:	xiteImage.3
ImagePosYtoScreen.3:	xiteImage.3
ImageGetZoom.3:	xiteImage.3
ImageSetZoom.3:	xiteImage.3
ImageCopyImagePart.3:	xiteImage.3
ImageCopyResources.3:	xiteImage.3
ImageGetResources.3:	xiteImage.3
ImageGetImageResources.3:	xiteImage.3
ImageGetZoomAll.3:	xiteImage.3
ImageSetZoomAll.3:	xiteImage.3
xiteImage.3:	Image.3
	cp Image.3 xiteImage.3

ImageTitle.3:	ImageFormat.3
ImageAddress.3:	ImageFormat.3
ImageWidth.3:	ImageFormat.3
ImageHeight.3:	ImageFormat.3
ImageLineLength.3:	ImageFormat.3
ImageXstart.3:	ImageFormat.3
ImageYstart.3:	ImageFormat.3
ImagePixName.3:	ImageFormat.3
ImagePixPrintFormat.3:	ImageFormat.3
ImagePixNameToBIFFPixTyp.3:	ImageFormat.3

ImageOverlayColor.3:	ImageOverlay.3
ImageOverlayCopyImageOverlayPart.3:	ImageOverlay.3
ImageOverlayCopyImagePart.3:	ImageOverlay.3
ImageOverlayCopyResources.3:	ImageOverlay.3
ImageOverlayGetResources.3:	ImageOverlay.3
ImageOverlayDisplay.3:	ImageOverlay.3
ImageOverlayDrawBox.3:	ImageOverlay.3
ImageOverlayDrawLine.3:	ImageOverlay.3
ImageOverlayDrawOrigo.3:	ImageOverlay.3
ImageOverlayDrawPoint.3:	ImageOverlay.3
ImageOverlayDrawRectangle.3:	ImageOverlay.3
ImageOverlayFill.3:	ImageOverlay.3
ImageOverlayPosXYtoAdr.3:	ImageOverlay.3
ImageOverlayProtect.3:	ImageOverlay.3
ImageOverlayRedisplay.3:	ImageOverlay.3
ImageOverlayScreenXYtoAdr.3:	ImageOverlay.3

ImageByteResize.3:	ImageResize.3
ImageOverlayResize.3:	ImageResize.3
ImageSignedByteResize.3:	ImageResize.3
ImageUnsignedShortResize.3:	ImageResize.3
ImageShortResize.3:	ImageResize.3
ImageLongResize.3:	ImageResize.3
ImageFloatResize.3:	ImageResize.3
ImageDoubleResize.3:	ImageResize.3
ImageResize.3:	ImageProtoPix.3
