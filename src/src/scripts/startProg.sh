#!/bin/sh

# $Id$

# XITE X applications should be links to this script.

prog=`basename $0`
dir=`dirname $0`
if test -n "$dir"; then
    prog_full=${dir}/${prog};
else
    prog_full=$prog;
fi

if test -z "$XITE_HOME"; then
    echo "$0 error: Missing value for environment variable XITE_HOME."
    exit 1
fi

XAPPLRESDIR=$XITE_HOME/etc/app-defaults
export XAPPLRESDIR

if [ "$XUSERFILESEARCHPATH" != "" ]; then
    XUSERFILESEARCHPATH=$XUSERFILESEARCHPATH:$XAPPLRESDIR/%N
    export XUSERFILESEARCHPATH
fi


exec "${prog_full}.bin" -name $prog "$@"
