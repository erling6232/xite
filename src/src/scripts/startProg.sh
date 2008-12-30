#!/bin/sh

# $Id$

# XITE X applications should be links to this script.

prog=`basename $0`
XSHOWPATH=${XSHOWPATH:-/etc/xite}
export XSHOWPATH

exec "/usr/bin/${prog}.bin" -name $prog "$@"
