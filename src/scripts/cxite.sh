#!/bin/sh


#/*P:cxite*
#
#________________________________________________________________
#
#		cxite
#________________________________________________________________
#
#Name:		cxite - Compile and link XITE-dependent source code
#
#Syntax:	cxite [<option>...] [<cc-option>...] <sourcefile>
#
#Description:	This program is used to compile and link sourcefiles which use
#               functions and definitions from XITE (X-based Image Processing
#               Tools and Environment).
#
#               You don''t need to specify where the XITE header files and
#		library are.
#
#Options:	&-c
#               Compile only
#		&-cx
#	        Compile only. The program uses X11 functions.
#		&-g
#	        Prepare for debugging, turn off optimization.
#		&-gnu
#	        Use GNU gcc instead of cc.
#		&-gw
#	        If using gcc, then add lots of gcc warning options.
#		&-h(elp)
#		Write a usage message and explain the purpose of this program.
#		&-I<dir>
#		Add '<dir>' to the list of directories in which to search for
#		#include files.
#		&-L<dir>
#		Add '<dir>' to the list of directories to be searched for
#		object libraries specified by '-l<lib>'.
#		&-l<lib>
#		Search a standard list of directories for the object
#		library given by '<lib>', and link with this library.
#               &-matlab
#               Source depends on conversion routines between MATLAB mat files
#               and XITE BIFF files. Link with required libraries.
#		&-o <file>
#		The name of the file where the compiled and linked result is
#		placed.
#		&-q
#	        Be quiet, don''t echo the command used.
#               &-R<dir>
#               Add '<dir>' to the list of directories to be searched for
#               dynamic shared objects by the runtime linker during execution.
#               &-tiff
#               Source depends on conversion routines between TIFF files and
#               XITE BIFF files. Link with required libraries.
#		&-x
#	        Compile and link with various X11 libraries in addition
#		to the XITE library.
#		-&and '<cc-option>' is
#		&-cc <string>
#		where '<string>' is passed unchanged as option(s) to the
#		compiler/linker.
#
#Author:	Svein Bøe, Ifi, UiO
#Id: 		$Id$
#________________________________________________________________
#
#*/

progName=`basename $0`

usage="Usage: $progName [<option>...] [<cc-option>...] <sourcefile> \n\
where <option> is one of \n\
	-c        : Compile only. \n\
	-cx       : Compile only. The program uses X11 functions. \n\
	-g        : Prepare for debugging, turn off optimization. \n\
	-gnu      : Use GNU gcc instead of cc. \n\
	-gw       : If using gcc, then add lots of gcc warning options. \n\
	-I<dir>   : Add <dir> to the list of directories in which to search \n\
	            for #include files. \n\
	-L<dir>   : Add <dir> to the list of directories to be searched for \n\
	            object libraries specified by -l<lib>. \n\
	-l<lib>   : Search a standard list of directories for the object \n\
	            library given by <lib>, and link with this library. \n\
        -matlab   : Link with libraries required for conversion between \n\
                    MATLAB mat files and XITE BIFF files. \n\
	-o <file> : The name of the file where the compiled and linked \n\
	            result is placed. \n\
	-q        : Be quiet, don't echo the command used. \n\
        -R<dir>   : Add <dir> to the list of directories to be searched for\n\
                    dynamic shared objects by the runtime linker during\n\
                    execution.\n\
        -tiff     : Link with libraries required for conversion between \n\
                    TIFF files and XITE BIFF files. \n\
	-x        : Compile and link with various X11 libraries in addition \n\
	            to the XITE library. \n\
and <cc-option> is \n\
	-cc <string> \n\
where <string> is passed unchanged as option(s) to the compiler/linker."

eval `xiteStdOpt 1 -1 $progName $@ "$usage"`

# This is where the install script automatically inserts the XITE home
# directory path.
if [ -z "$XITE_HOME" ]; then
  XITE_HOME=
fi

if [ -z "$XITE_HOME" ]
then

cat <<EOF 1>&2
$progName error: The XITE home directory is not known. Set the environment
variable XITE_HOME to the XITE home directory or ask your local XITE
administrator to edit this script.  The	environment variable is set to
dir in csh or tcsh by the command 'setenv XITE_HOME dir' or in sh by the
command	'XITE_HOME=dir;export XITE_HOME'.
EOF

  exit 1
fi

# Default values
getPlatform=$XITE_HOME/etc/.install/getPlatform
xite_hosttype_def=`$getPlatform "" $XITE_HOME/etc/.install stderr 1`
xite_inc_dest_def=$XITE_HOME/include
xite_lib_dest_def=$XITE_HOME/lib/$xite_hosttype_def
x_inc_dir_def=/usr/include
x_lib_dir_def=/usr/lib
matlab_inc_def=/usr/local/matlab/extern/include
matlab_lib_def=/usr/local/matlab/extern/lib
tiff_inc_def=/usr/local/include
tiff_lib_def=/usr/local/lib


# This is where the install script or the local XITE administrator may
# hardcode the platform type, especially if the platform type is different
# from the standard ones recognized by the script
# $XITE_HOME/etc/.install/getPlatform.
if [ -z "$XITE_HOSTTYPE" ]; then
  XITE_HOSTTYPE=
fi

if [ -z "$XITE_HOSTTYPE" ]; then
  # Assume default platform type, if not hardcoded above or in the environment.
  XITE_HOSTTYPE="$xite_hosttype_def"
fi

if [ -z "$XITE_HOSTTYPE" ]; then
  # Still don't know the platform type, which is needed for a compile option.

cat <<EOF 1>&2
$progName error: The computer platform type could not be determined.
Ask your XITE administrator to edit this script or the scripts it depends on.
EOF

  exit 1
fi

# Update default value depending on XITE_HOSTTYPE.
xite_lib_dest_def=$XITE_HOME/lib/$XITE_HOSTTYPE

# This is where the install script automatically inserts the XITE include
# directory path.
if test -z "$XITE_INC_DEST"
then
  XITE_INC_DEST=
fi

if test -z "$XITE_INC_DEST"
then
  # Assume default XITE include directory path, if not hardcoded above
  # or in the environment.
  XITE_INC_DEST="$xite_inc_dest_def"
fi

# This is where the install script automatically inserts the XITE library
# directory path.
if test -z "$XITE_LIB_DEST"
then
  XITE_LIB_DEST=
fi

if test -z "$XITE_LIB_DEST"
then
  # Assume default XITE library directory path, if not hardcoded above
  # or in the environment.
  XITE_LIB_DEST="$xite_lib_dest_def"
fi

# This is where the install script automatically inserts the X include
# directory path.
if test -z "$X_INC_DIR"
then
  X_INC_DIR=
fi

if test -z "$X_INC_DIR"
then
  # Assume default X include directory path, if not hardcoded above
  # or in the environment.
  X_INC_DIR="$x_inc_dir_def"
fi

# This is where the install script automatically inserts the X library
# directory path.
if test -z "$X_LIB_DIR"
then
  X_LIB_DIR=
fi

if test -z "$X_LIB_DIR"
then
  # Assume default X library directory path, if not hardcoded above
  # or in the environment.
  X_LIB_DIR="$x_lib_dir_def"
fi

if [ -z "$MATLAB_INC" ]; then
  MATLAB_INC=
fi

if [ -z "$MATLAB_INC" ]; then
  MATLAB_INC="$matlab_inc_def"
fi

if [ -z "$MATLAB_LIB" ]; then
  MATLAB_LIB=
fi

if [ -z "$MATLAB_LIB" ]; then
  MATLAB_LIB="$matlab_lib_def"
fi

if [ -z "$TIFF_INC_DIR" ]; then
  TIFF_INC_DIR=
fi

if [ -z "$TIFF_INC_DIR" ]; then
  TIFF_INC_DIR="$tiff_inc_def"
fi

if [ -z "$TIFF_LIB_DIR" ]; then
  TIFF_LIB_DIR=
fi

if [ -z "$TIFF_LIB_DIR" ]; then
  TIFF_LIB_DIR="$tiff_lib_def"
fi

echo=`xiteEcho`

c=""; ccOpt=""; sourceFile=""; comp="cc"; outFile=""; x=0; xInc="";
xL=""; xl=""; q=0; g=""; w=0; warn=""; echoUsage=0; help=0; libs=""; libDirs=""
dsoDirs=""; incDirs=""; optim=""; xiteX=""; matlab=0; matInc=""; matl="";
tiff=0; tiffInc=""; tiffl=""

while [ $# -gt 0 ]; do
  case $1 in
    -c) c=$1; shift;;
    -cx) c="-c"; x=1; shift;;
    -g) g=$1; shift;;
    -gnu) comp="gcc"; shift;;
    -gw) w=1; shift;;
    -cc)
	if [ $# -eq 1 ]; then
	  $echo "$progName error: Missing argument for option $1." 1>&2
	  echoUsage=1;
	fi
	shift;
	if [ -z "$ccOpt" ]; then
	  ccOpt="$1"
	else
	  ccOpt="$ccOpt $1"
	fi
	shift;;
    -I*)
	if [ -z "$incDirs" ]; then
	  incDirs="$1"
	else
	  incDirs="$incDirs $1"
	fi
	shift;;
    -L*)
	if [ -z "$libDirs" ]; then
	  libDirs="$1"
	else
	  libDirs="$libDirs $1"
	fi
	shift;;
    -l*) 
	if [ -z "$libs" ]; then
	  libs="$1"
	else
	  libs="$libs $1"
	fi
	shift;;
    -matlab)
        matlab=1; shift;;
    -o) 
	if [ $# -eq 1 ]; then
	  $echo "$progName error: Missing argument for option $1." 1>&2
	  echoUsage=1;
	fi
	shift;
	if [ -n "$outFile" ]; then
	  $echo "$progName error: Option -o should only be used once." 1>&2
	  echoUsage=1;
	fi
	outFile=$1
	shift;;
    -q) q=1; shift;;
    -R*)
	if [ -z "$dsoDirs" ]; then
	  dsoDirs="$1"
	else
	  dsoDirs="$dsoDirs $1"
	fi
	shift;;
    -tiff)
        tiff=1; shift;;
    -x) x=1; shift;;
    -*) $echo "$progName error: Unknown option $1." 1>&2
	shift; echoUsage=1;;
    *)  if [ -z "$sourceFile" ]; then
	  sourceFile=$1
	else
	  sourceFile="$sourceFile $1"
	fi
	shift;;
  esac
done

if [ "$echoUsage" -eq 1 ]; then
    $echo "$usage"
    exit 1
fi

if [ -z "$g" ]; then
  optim="-O"
fi

if [ -n "$outFile" ]; then
  outFile="-o $outFile"
fi

if [ "$comp" = "gcc" -a "$w" -eq 1 ]; then
  warn="-Wall -Wmissing-prototypes -Wstrict-prototypes -Wtraditional"
  warn="$warn -Wconversion -Wnested-externs -Wpointer-arith"
elif [ "$comp" != "gcc" -a "$w" -eq 1 ]; then
  $echo "$progName warning: gcc warnings not used (option '-gw') because gcc is not used." \
    1>&2
fi

if [ "$x" -eq 1 ]; then
  # Software needs X Window System
  xInc="-I$X_INC_DIR"
  if  [ -z "$c" ]; then
    xL="-L$X_LIB_DIR"
    xl="-lfwf -lxpm -lXaw -lXmu -lXt -lXext -lX11"
    xiteX="-lxiteX"
  fi
fi

if [ "$matlab" -eq 1 ]; then
  matInc="-I$MATLAB_INC"
fi

if [ "$tiff" -eq 1 ]; then
  tiffInc="-I$TIFF_INC_DIR"
fi

cflags=""
lflags=""
arch_libs=""

# REGEX must be one of empty, -DREGEX, -DNO_REGEXP
# DIRENT must be one of empty, -DNO_DIRENT

case "$XITE_HOSTTYPE" in
  AIX)     
           REGEX=
	   DIRENT=
	   cflags="-DSYSTEM_V -DSYSV -Dvfork=fork -DDEF_STRCASECMP"
	   cflags="$cflags $REGEX $DIRENT"
	   if [ "$comp" = "gcc" ]; then
	     cflags="$cflags -fsigned-char"
	   fi
	   if [ -n "$xl" ]; then
	     xl="$xl -liconv"
	   fi
	   if [ "$matlab" -eq 1 ]; then
	     matl="-L$MATLAB_LIB -lmat -lmx -lxite_matlab"
	   fi
	   if [ "$tiff" -eq 1 ]; then
	     tiffl="-L$TIFF_LIB_DIR -ltiff -lxite_tiff"
	   fi
	   ;;
  IRIX*)   
           REGEX=
	   DIRENT=
	   cflags="-DSYSTEM_V -DSYSV -D_BSD_SIGNALS -Dvfork=fork -DGETPTY"
	   cflags="$cflags $REGEX $DIRENT"
	   case "$comp" in
	     gcc) cflags="$cflags -fsigned-char";;
	     *)   cflags="$cflags -signed";;
	   esac
	   lflags="-rpath $XITE_LIB_DEST"
	   if [ -n "$xL" ]; then
	     xL="$xL -rpath $X_LIB_DIR"
	   fi
	   if [ "$matlab" -eq 1 ]; then
	     matl="-L$MATLAB_LIB -rpath $MATLAB_LIB -lmat -lmx -lxite_matlab"
	   fi
	   if [ "$tiff" -eq 1 ]; then
	     tiffl="-L$TIFF_LIB_DIR -rpath $TIFF_LIB_DIR -ltiff -lxite_tiff"
	   fi
	   ;;
  Linux)   
           REGEX=
	   DIRENT=
	   cflags="-Dinstring=__instring"
	   cflags="$cflags $REGEX $DIRENT"
	   lflags="-Xlinker -rpath -Xlinker $XITE_LIB_DEST"
	   if [ -n "$xL" ]; then
	     xL="$xL -Xlinker -rpath -Xlinker $X_LIB_DIR"
	   fi
	   if [ "$matlab" -eq 1 ]; then
	     matl="-L$MATLAB_LIB -Xlinker -rpath -Xlinker $MATLAB_LIB"
             matl="$matl -lmat -lmx -lxite_matlab"
	   fi
	   if [ "$tiff" -eq 1 ]; then
	     tiffl="-L$TIFF_LIB_DIR -Xlinker -rpath -Xlinker $TIFF_LIB_DIR"
	     tiffl="$tiffl -ltiff -lxite_tiff"
	   fi
	   ;;
  OSF1)    
           REGEX=
	   DIRENT=
	   cflags="-DSYSTEM_V -DSYSV -Dvfork=fork"
	   cflags="$cflags $REGEX $DIRENT"
	   if [ "$comp" != "gcc" ]; then
	     cflags="$cflags -std1"
	   fi
	   if [ "$matlab" -eq 1 ]; then
	     matl="-L$MATLAB_LIB -lmat -lmx -lxite_matlab"
	   fi
	   if [ "$tiff" -eq 1 ]; then
	     tiffl="-L$TIFF_LIB_DIR -ltiff -lxite_tiff"
	   fi
           arch_libs="-ldnet_stub -lots"
	   ;;
  SunOS)   
           REGEX=
	   DIRENT=
	   cflags="-DDEF_STRCASECMP"
	   cflags="$cflags $REGEX $DIRENT"
	   if [ "$matlab" -eq 1 ]; then
	     matl="-L$MATLAB_LIB -lmat -lmx -lxite_matlab"
	   fi
	   if [ "$tiff" -eq 1 ]; then
	     tiffl="-L$TIFF_LIB_DIR -ltiff -lxite_tiff"
	   fi
	   ;;
  SunOS5)  
           REGEX=
	   DIRENT=
	   cflags="-DSYSTEM_V -DSYSV -I/usr/ucbinclude"
	   cflags="$cflags $REGEX $DIRENT"
           lflags="-L/usr/ucblib -R/usr/ucblib -R$XITE_LIB_DEST"
	   if [ -n "$xL" ]; then
	     xL="$xL -R$X_LIB_DIR"
	   fi
	   if [ "$matlab" -eq 1 ]; then
	     matl="-L$MATLAB_LIB -R$MATLAB_LIB -lmat -lmx -lmi -lut"
             matl="$matl -lxite_matlab"
	   fi
	   if [ "$tiff" -eq 1 ]; then
	     tiffl="-L$TIFF_LIB_DIR -R$TIFF_LIB_DIR -ltiff -lxite_tiff"
	   fi
           arch_libs="-lucb"
           ;;
  ULTRIX)  
           REGEX=
	   DIRENT=
	   cflags="$cflags $REGEX $DIRENT"
	   if [ "$matlab" -eq 1 ]; then
	     matl="-L$MATLAB_LIB -lmat -lmx -lxite_matlab"
	   fi
	   if [ "$tiff" -eq 1 ]; then
	     tiffl="-L$TIFF_LIB_DIR -ltiff -lxite_tiff"
	   fi
	   ;;
  *)
           REGEX=
	   DIRENT=
	   cflags="-DSYSTEM_V -DSYSV"
	   cflags="$cflags $REGEX $DIRENT"
	   if [ "$matlab" -eq 1 ]; then
	     matl="-L$MATLAB_LIB -lmat -lmx -lxite_matlab"
	   fi
	   if [ "$tiff" -eq 1 ]; then
	     tiffl="-L$TIFF_LIB_DIR -ltiff -lxite_tiff"
	   fi
	   ;;
esac

if [ "$c" = "-c" ]; then
  command="$comp $c $g $optim -D$XITE_HOSTTYPE $ccOpt $warn $cflags"
  command="$command -I$XITE_INC_DEST $xInc $incDirs $matInc $tiffInc $outFile"
  command="$command $sourceFile"
else
  command="$comp $g $optim -DMAIN -D$XITE_HOSTTYPE $ccOpt $warn $cflags"
  command="$command -I$XITE_INC_DEST $xInc $incDirs $matInc $tiffInc"
  command="$command -L$XITE_LIB_DEST"
  command="$command $xL $libDirs $dsoDirs $lflags $outFile $sourceFile $libs"
  command="$command $xiteX -lxite $xl $matl $tiffl $arch_libs -lm"
fi

if [ "$q" -eq 0 ]; then
  $echo "$command"
fi

eval "$command"

