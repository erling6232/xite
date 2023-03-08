#
#      Makefile.am for lib/ximage
#


@SET_MAKE@

SHELL = /bin/sh

include $(top_srcdir)/Makefile.def

#
#   Makefile definitions
#

noinst_LTLIBRARIES = libxiteXximage.la

MANF =  cmapOfVisualClass.3 RGBPixel.3 InitColortablePixels.3 \
	InitColortableGray.3 InitColortableGrayPart.3 SpreadGray.3 \
	PrintColormap.3 PrintWidgetColormap.3 ColormapCopy.3 \
	ColormapStd.3 cmapOfVisual.3 GetImageColormap.3 Cmap.3 \
	Histogram.3 RequestedImageVisualAndColormap.3 CreateAppShell.3 \
	CreateTopLevelShell.3 CreatePopTopShell.3 CreatePopupShell.3 \
	CreatePopOver.3 AddAction.3 TopLevelWidget.3 PopupExit.3 \
	PopupInit.3 PopupAdd.3 Popup_add.3 PopupLine.3 PopupWidget.3 \
	ImageFromMenu.3 PopupMenu.3 InitializeVisual.3 ShellWids.3 \
	visualFromName.3 visualFromClass.3 visualClassToName.3 \
	visualNameToClass.3 DepthToReducedPlanes.3 DepthOfVisual.3 \
	VisualInfoFromClass.3 Pseudo8VisualInfo.3 Direct24VisualInfo.3 \
	True24VisualInfo.3 InitializeVisualInfos.3 RequestedPlanes.3 \
	PlanesToDepth.3 RequestedImageVisual.3 Visual.3 Color_set.3 \
	Color_ovl_set.3 Color_add.3 Color_ovl_add.3 Color_read.3 \
	Color_ovl_read.3 Color_exit.3 Color_max.3 Color_ovl_max.3 \
	Color_map.3 Color_map2.3 ColortabName.3 OvltabName.3 \
	Xcolor_deftab.3 AddColormapCopy.3 Has_changeable_map.3 \
	OverlaysMenuInit.3 ColorsMenuInit.3 FormDialog.3 CreateDialog.3 \
	InformationBox.3 ConfirmationBox.3 SelectReadFile.3 \
	SelectWriteFile.3 init_button_table.3 SimpleDialog.3 \
	xhistogram.3 ximage_init.3 ximageInit.3 ximage_layout.3 \
	ximageLayout.3 ximage_histogram.3 ximage_slice.3 ximage_info.3 \
	ximage_colorbar.3 ximage_mainloop.3 ximage_exit.3 \
	ximage_display.3 ximage_app_resources.3 ximage_app_options.3 \
	ximage_usage_text.3 ximage_app_usage_text.3 set_init_color.3 \
	set_init_ovl_color.3 LogPosition.3 LogPositionCallback.3 \
	Make_label_layout.3 Make_command_layout.3 Make_prompt_layout.3 \
	Make_scroll_layout.3 Make_menu_layout.3 Make_entry_layout.3 \
	Add_entry_layout.3 Make_list_layout.3 Make_image_layout.3 \
	add_callback_to_image_layout.3 init_image_callbacks.3 \
	add_to_image_callbacks.3 add_callback_first.3 \
	add_callback_last.3 ximage.3 XptyMessage.3 XptyClose.3 \
	XptyStartProgram.3 XptyJobs.3 XptyKill.3 XptyList.3 XptyInit.3 \
	colorbar_image.3 colorbar_widget.3 xslice.3

man3_MANS=$(MANF)

libxiteXximage_la_SOURCES = Cmap.c Colormap.c Histogram.c ImageForm.c \
	ImageOptionsMenu.c ImageToolsMenu.c ImageVisualsMenu.c ShellWids.c \
	Visual.c Xcolor.c Xdialog.c FormDialog.c Xhistogram.c XmenuBar.c \
	Xpty.c Xslice.c layout.c logPos.c callbacks.c ximage_display.c \
	ximage.c xite_usleep.c

cmapOfVisualClass.3:	Cmap.3
RGBPixel.3:	Cmap.3
InitColortablePixels.3:	Cmap.3
InitColortableGray.3:	Cmap.3
InitColortableGrayPart.3:	Cmap.3
SpreadGray.3:	Cmap.3
PrintColormap.3:	Cmap.3
PrintWidgetColormap.3:	Cmap.3
ColormapCopy.3:	Cmap.3
ColormapStd.3:	Cmap.3
cmapOfVisual.3:	Cmap.3
GetImageColormap.3:	Cmap.3
RequestedImageVisualAndColormap.3:	ShellWids.3
CreateAppShell.3:	ShellWids.3
CreateTopLevelShell.3:	ShellWids.3
CreatePopTopShell.3:	ShellWids.3
CreatePopupShell.3:	ShellWids.3
CreatePopOver.3:	ShellWids.3
AddAction.3:	ShellWids.3
TopLevelWidget.3:	ShellWids.3
PopupExit.3:	ShellWids.3
PopupInit.3:	ShellWids.3
PopupAdd.3:	ShellWids.3
Popup_add.3:	ShellWids.3
PopupLine.3:	ShellWids.3
PopupWidget.3:	ShellWids.3
ImageFromMenu.3:	ShellWids.3
PopupMenu.3:	ShellWids.3
InitializeVisual.3:	ShellWids.3
visualFromName.3:	Visual.3
visualFromClass.3:	Visual.3
visualClassToName.3:	Visual.3
visualNameToClass.3:	Visual.3
DepthToReducedPlanes.3:	Visual.3
DepthOfVisual.3:	Visual.3
VisualInfoFromClass.3:	Visual.3
Pseudo8VisualInfo.3:	Visual.3
Direct24VisualInfo.3:	Visual.3
True24VisualInfo.3:	Visual.3
InitializeVisualInfos.3:	Visual.3
RequestedPlanes.3:	Visual.3
PlanesToDepth.3:	Visual.3
RequestedImageVisual.3:	Visual.3
Color_set.3:	ColorsMenuInit.3
Color_ovl_set.3:	ColorsMenuInit.3
Color_add.3:	ColorsMenuInit.3
Color_ovl_add.3:	ColorsMenuInit.3
Color_read.3:	ColorsMenuInit.3
Color_ovl_read.3:	ColorsMenuInit.3
Color_exit.3:	ColorsMenuInit.3
Color_max.3:	ColorsMenuInit.3
Color_ovl_max.3:	ColorsMenuInit.3
Color_map.3:	ColorsMenuInit.3
Color_map2.3:	ColorsMenuInit.3
ColortabName.3:	ColorsMenuInit.3
OvltabName.3:	ColorsMenuInit.3
Xcolor_deftab.3:	ColorsMenuInit.3
AddColormapCopy.3:	ColorsMenuInit.3
Has_changeable_map.3:	ColorsMenuInit.3
OverlaysMenuInit.3:	ColorsMenuInit.3
ColorsMenuInit.3:	Xcolor.3
FormDialog.3:	SimpleDialog.3
CreateDialog.3:	SimpleDialog.3
InformationBox.3:	SimpleDialog.3
ConfirmationBox.3:	SimpleDialog.3
SelectReadFile.3:	SimpleDialog.3
SelectWriteFile.3:	SimpleDialog.3
init_button_table.3:	SimpleDialog.3
SimpleDialog.3:	Xdialog.3
xhistogram.3:	Xhistogram.3
ximage_init.3:	ximage.3
ximageInit.3:	ximage.3
ximage_layout.3:	ximage.3
ximageLayout.3:	ximage.3
ximage_histogram.3:	ximage.3
ximage_slice.3:	ximage.3
ximage_info.3:	ximage.3
ximage_colorbar.3:	ximage.3
ximage_mainloop.3:	ximage.3
ximage_exit.3:	ximage.3 
ximage_display.3:	ximage.3
ximage_app_resources.3:	ximage.3
ximage_app_options.3:	ximage.3
ximage_usage_text.3:	ximage.3
ximage_app_usage_text.3:	ximage.3
set_init_color.3:	ximage.3
set_init_ovl_color.3:	ximage.3
LogPosition.3:	ximage.3
LogPositionCallback.3:	ximage.3
Make_label_layout.3:	ximage.3
Make_command_layout.3:	ximage.3
Make_prompt_layout.3:	ximage.3
Make_scroll_layout.3:	ximage.3
Make_menu_layout.3:	ximage.3
Make_entry_layout.3:	ximage.3
Add_entry_layout.3:	ximage.3
Make_list_layout.3:	ximage.3
Make_image_layout.3:	ximage.3
add_callback_to_image_layout.3:	ximage.3
init_image_callbacks.3:	ximage.3
add_to_image_callbacks.3:	ximage.3
add_callback_first.3:	ximage.3
add_callback_last.3:	ximage.3
XptyMessage.3:	XptyInit.3
XptyClose.3:	XptyInit.3
XptyStartProgram.3:	XptyInit.3
XptyJobs.3:	XptyInit.3
XptyKill.3:	XptyInit.3
XptyList.3:	XptyInit.3
XptyInit.3:	Xpty.3
colorbar_image.3:	xslice.3
colorbar_widget.3:	xslice.3
xslice.3:	Xslice.3
