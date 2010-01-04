#
#      Makefile.am for src/xshow
#


@SET_MAKE@

SHELL = /bin/sh
configdir = @configdir@

include $(configdir)/@common_makefile@

#
#   Makefile definitions
#
CINCL  = fork_xshow.i
PROG   = ReadMenuList fork_xshow xshow
MANP   = xshow.m fork_xshow.m
XSSRC  = Macro.o Xprog.o Xcontrol.o Xbuild_menu.o MenuBox.o \
	 MenuCommand.o SubMenuCommand.o Xdisplay.o xshowImageOptionsMenu.o
XSSRCC = Macro.c Xprog.c Xcontrol.c Xbuild_menu.c MenuBox.c \
	 MenuCommand.c SubMenuCommand.c Xdisplay.c xshowImageOptionsMenu.c

#xshow$(EXEEXT):  $(XSSRC)
#	$(LINK_x_obj)
#	-@rm $(XITE_BIN_DEST)/$@ > /dev/null 2>&1
#	-@ln -s $(XITE_BIN_DEST)/startProg $(XITE_BIN_DEST)/$@ > \
#	  /dev/null 2>&1
#	-@chmod a+rx $(XITE_BIN_DEST)/$@.bin $(XITE_BIN_DEST)/$@ > \
#	  /dev/null 2>&1
#	-@rm xshow.o > /dev/null 2>&1
#$(XSSRC) xshow.o:$(@:.o=.c)
#	$(COMPILE_x) $(@:.o=.c)

bin_PROGRAMS=$(PROG) $(XPROG)
man1_MANS=$(MANP)

LDADD = ../../lib/ximage/libxiteXximage.la ../../lib/libxiteX.la \
	../../lib/libxite.la $(LIBGUI)

ReadMenuList_SOURCES = ReadMenuList_main.c
fork_xshow_SOURCES   = fork_xshow_main.c
xshow_SOURCES        = xshow_main.c $(XSSRCC)
