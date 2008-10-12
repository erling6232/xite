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

# Checks for libraries
AC_CHECK_LIB([libX11],  [XBell])
AC_CHECK_LIB([libxpm],  [xpmReadFile])
AC_CHECK_LIB([libXaw],  [XawAsciiSave])
AC_CHECK_LIB([libXmu],  [XmuMakeAtom])
AC_CHECK_LIB([libXt],   [XtConvert])
AC_CHECK_LIB([libXext], [XextAddDisplay])

# Checks for header files
AC_PATH_X
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([ctype.h fcntl.h float.h io.h limits.h malloc.h math.h stddef.h stdlib.h sys/file.h sys/param.h sys/time.h unistd.h values.h])
AC_CHECK_HEADER([src/biff/biff.h])
AC_CHECK_HEADERS([regex.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_MODE_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_HEADER_TIME
AC_CHECK_TYPES([ptrdiff_t])

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
AC_CHECK_FUNCS([dup2 getcwd isascii pow regcomp select sqrt strcasecmp strchr strcspn strerror strrchr strspn strstr strtol])
AC_CHECK_FUNCS([random srandom toupper tolower])

# define XITE_FCNTL_H      <sys/fcntl.h>
# define XITE_IO_H         <xite/dummy.h>
# define XITE_MALLOC_H     <stdlib.h>
# define XITE_MEMORY_H     <string.h>
# define XITE_STRING_H     <string.h>
# define XITE_PARAM_H      <sys/param.h>

# Checks for library functions

# Common Makefile.def
AC_SUBST(configdir, [\${top_srcdir}/config])
AC_SUBST(common_makefile, [Makefile.def])

# Output files
AC_CONFIG_FILES([
    config/Makefile.def
    Makefile
    src/Makefile
    src/arithmetic/Makefile
    ])
AC_OUTPUT
