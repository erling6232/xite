#!/bin/sh


#/*P:xiteEcho*
#
#________________________________________________________________
#
#		xiteEcho
#________________________________________________________________
#
#Name:		xiteEcho - Find a suitable system echo command
#
#Syntax:	xiteEcho
#
#Description:   Find a system echo command which will interpret backslash
#               escape sequences.
#
#Author:	Svein Bøe, Ifi, UiO
#Id: 		$Id$
#________________________________________________________________
#
#*/

progName=`basename $0`

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

if test -f /usr/5bin/echo; then
  echo=/usr/5bin/echo
elif test -f /usr/bin/echo; then
  echo=/usr/bin/echo
else
  if [ -n "$SHELL" ]; then
    bash=`expr $SHELL : '.*\(bash\).*'`
    if [ -n "$bash" ]; then
      echo="echo -e"
    fi
  fi

  if [ -z "$echo" ]; then
    if [ "$XITE_HOSTTYPE" = "Linux" ]; then
      echo="echo -e"
    fi    
  fi
fi
if [ -z "$echo" ]; then
  echo=echo
fi

echo "$echo"
