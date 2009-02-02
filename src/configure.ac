# Prelude
AC_PREREQ(2.50)
AC_INIT([xite], [3.4.5], [Erling.Andersen@Helse-Bergen.NO])
AC_CONFIG_SRCDIR([include/xite/blab.h])
AC_CONFIG_HEADER([config.h])

AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

# Checks for programs
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_PROG_SED

# AM_PROG_CC_C_O

# Checks for libraries
AC_CHECK_LIB([X11],  [XBell])
# Internal xpm # AC_CHECK_LIB([xpm],  [xpmReadFile])
# AC_CHECK_LIB([Xaw],  [XawAsciiSave])
AC_CHECK_LIB([Xmu],  [XmuMakeAtom])
AC_CHECK_LIB([Xt],   [XtConvert])
#AC_CHECK_LIB([Xext], [XextAddDisplay])
AC_CHECK_LIB([m],    [sqrt])

# Checks for header files
AC_PATH_X
AC_ARG_ENABLE(gui,
	[AS_HELP_STRING([--enable-gui=auto], [Select GUI: motif, athena or auto])],
	[gui="$enableval"], [gui=auto])
LIBGUI=
AS_IF([test "x$gui" = xauto],
	[AC_CHECK_LIB([Xaw],  [XawAsciiSave],
		[AC_SUBST([LIBGUI], ["-lXaw -lXmu -lXt"])
		 AC_DEFINE([HAVE_LIBXAW], [1], [Define if you have libXaw])
		],
		[AC_CHECK_LIB([Xm],  [XmStringFree],
			[AC_SUBST([LIBGUI], ["-lXm"])
			 AC_DEFINE([HAVE_LIBMOTIF], [1], [Define if you have Motif])
			 AC_DEFINE([MOTIF], [], [Define when you have Motif])
			],
			[AC_MSG_FAILURE([Motif test failed])
			])
		])
	],
      [test "x$gui" = xathena],
	[AC_CHECK_LIB([Xaw],  [XawAsciiSave],
		[AC_SUBST([LIBGUI], ["-lXaw -lXmu -lXt"])
		 AC_DEFINE([HAVE_LIBXAW], [1], [Define if you have libXaw])
		],
	)],
      [test "x$gui" = xmotif],
	[AC_CHECK_LIB([Xm],  [XmStringFree],
		[AC_SUBST([LIBGUI], ["-lXm"])
		 AC_DEFINE([HAVE_LIBMOTIF], [1], [Define if you have Motif])
		],
	)]
)

AC_HEADER_DIRENT
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([ctype.h fcntl.h sys/fcntl.h float.h io.h sys/io.h limits.h malloc.h math.h memory.h stddef.h stdio.h stdlib.h sys/file.h sys/param.h sys/stat.h sys/time.h sys/times.h time.h unistd.h values.h])
AC_CHECK_HEADERS([direct.h varargs.h stdarg.h pwd.h process.h])
AC_CHECK_HEADER([regex.h],
                [AC_DEFINE([HAVE_REGEX_H], [1], [Define if you have regex.h])]
                [AC_DEFINE([REGEX], [1], [Define if you have regex.h])],
		[AC_CHECK_HEADER([regexp.h],
		                 [AC_DEFINE([HAVE_REGEXP_H], [1], [Define if you have regexp.h])],
				 [AC_DEFINE([NO_REGEXP], [1], [Define if you dont have regex])]
				)
	       ])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_MODE_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_HEADER_TIME
AC_CHECK_TYPES([ptrdiff_t])
AC_CHECK_MEMBERS([struct passwd.pw_dir])

#AM_CFLAGS = -O3 -DSYSTEM_V -DSYSV -Dvfork=fork \
#                  $(DEBUG) -DDEF_STRCASECMP -DREGEX $(DIRENT)
# if test "$x" = "$y" ; then ...
#if HAVE_REGEX_H
#AM_CFLAGS += -DREGEX
#endif


# Checks for library functions.
AC_FUNC_CLOSEDIR_VOID
AC_FUNC_ERROR_AT_LINE
AC_FUNC_FORK
AC_FUNC_LSTAT
AC_FUNC_LSTAT_FOLLOWS_SLASHED_SYMLINK
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_FUNC_REALLOC
AC_FUNC_SELECT_ARGTYPES
AC_TYPE_SIGNAL
AC_FUNC_STAT
AC_FUNC_STRTOD
AC_FUNC_VPRINTF
AC_FUNC_WAIT3
AC_CHECK_FUNCS([dup2 getcwd getwd isascii pow regcomp select sqrt strcasecmp strchr strcspn strerror strrchr strspn strstr strtod strtol])
AC_CHECK_FUNCS([link setmode symlink rand random srand srandom toupper tolower umask])
AC_CHECK_FUNCS([clock flock mktemp mkstemp tempnam time times])

# define XITE_MALLOC_H     <stdlib.h>
# define XITE_MEMORY_H     <string.h>
# define XITE_STRING_H     <string.h>
# define XITE_PARAM_H      <sys/param.h>



# Common Makefile.def
AC_SUBST(configdir, [\${top_builddir}])
AC_SUBST(common_makefile, [Makefile.def])

# Output files
AC_CONFIG_FILES([
    Makefile.def
    Makefile
    data/Makefile
    data/img/Makefile
    data/xshow/Makefile
    include/Makefile
    include/matlab/Makefile
    include/X11/Makefile
    include/Xfwf/Makefile
    include/xite/Makefile
    lib/Makefile
    lib/arithmetic/Makefile
    lib/biff/Makefile
    lib/binarize/Makefile
    lib/canny/Makefile
    lib/cdoc/Makefile
    lib/classification/Makefile
    lib/color/Makefile
    lib/combine/Makefile
    lib/contour/Makefile
    lib/convert/Makefile
    lib/convolve/Makefile
    lib/copy/Makefile
    lib/dither/Makefile
    lib/draw/Makefile
    lib/extrema/Makefile
    lib/fft/Makefile
    lib/fht/Makefile
    lib/fractal/Makefile
    lib/fwf/Makefile
    lib/fwf/Board/Makefile
    lib/fwf/Button/Makefile
    lib/fwf/Common/Makefile
    lib/fwf/converters/Makefile
    lib/fwf/Dir/Makefile
    lib/fwf/FileSelector/Makefile
    lib/fwf/Frame/Makefile
    lib/fwf/Group/Makefile
    lib/fwf/Label/Makefile
    lib/fwf/MenuBar/Makefile
    lib/fwf/MultiList/Makefile
    lib/fwf/OptButton/Makefile
    lib/fwf/Prompt/Makefile
    lib/fwf/PullDown/Makefile
    lib/fwf/RowCol/Makefile
    lib/fwf/Rows/Makefile
    lib/fwf/ScrolledList/Makefile
    lib/fwf/tabstring/Makefile
    lib/fwf/TextMenu/Makefile
    lib/fwf/Toggle/Makefile
    lib/geometry/Makefile
    lib/gradInv/Makefile
    lib/histo/Makefile
    lib/hough/Makefile
    lib/haar/Makefile
    lib/ideal/Makefile
    lib/ihs/Makefile
    lib/Image/Makefile
    lib/kncn/Makefile
    lib/knn/Makefile
    lib/matlab/Makefile
    lib/median/Makefile
    lib/minmax/Makefile
    lib/morph/Makefile
    lib/negate/Makefile
    lib/noise/Makefile
    lib/pnm/Makefile
    lib/principal/Makefile
    lib/pyramid/Makefile
    lib/region/Makefile
    lib/resample/Makefile
    lib/scatter/Makefile
    lib/segmentation/Makefile
    lib/sigma/Makefile
    lib/snn/Makefile
    lib/statistics/Makefile
    lib/stdiff/Makefile
    lib/sunraster/Makefile
    lib/texture/Makefile
    lib/thin/Makefile
    lib/threshold/Makefile
    lib/thresMl/Makefile
    lib/tiff/Makefile
    lib/utils/Makefile
    lib/ximage/Makefile
    lib/xshow/Makefile
    lib/zernike/Makefile
    src/Makefile
    src/arithmetic/Makefile
    src/biff/Makefile
    src/binarize/Makefile
    src/canny/Makefile
    src/cdoc/Makefile
    src/classification/Makefile
    src/color/Makefile
    src/combine/Makefile
    src/contour/Makefile
    src/convert/Makefile
    src/convolve/Makefile
    src/copy/Makefile
    src/dither/Makefile
    src/draw/Makefile
    src/extrema/Makefile
    src/fft/Makefile
    src/fht/Makefile
    src/fractal/Makefile
    src/geometry/Makefile
    src/gradInv/Makefile
    src/histo/Makefile
    src/hough/Makefile
    src/haar/Makefile
    src/ideal/Makefile
    src/ihs/Makefile
    src/kncn/Makefile
    src/knn/Makefile
    src/matlab/Makefile
    src/median/Makefile
    src/minmax/Makefile
    src/morph/Makefile
    src/negate/Makefile
    src/noise/Makefile
    src/pnm/Makefile
    src/PostScript/Makefile
    src/principal/Makefile
    src/pyramid/Makefile
    src/region/Makefile
    src/resample/Makefile
    src/scatter/Makefile
    src/scripts/Makefile
    src/segmentation/Makefile
    src/sigma/Makefile
    src/snn/Makefile
    src/statistics/Makefile
    src/stdiff/Makefile
    src/sunraster/Makefile
    src/texture/Makefile
    src/thin/Makefile
    src/threshold/Makefile
    src/thresMl/Makefile
    src/tiff/Makefile
    src/ximage/Makefile
    src/xshow/Makefile
    src/zernike/Makefile
    ])
    #lib/xpm/Makefile
AC_OUTPUT
