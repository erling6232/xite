#!/bin/sh	

if test -f /usr/5bin/echo; then
   echo=/usr/5bin/echo
elif test -f /usr/bin/echo; then
   echo=/usr/bin/echo
else
   echo=echo
fi

progName=`basename $0`
extract="-point -area -mean -stdev"

usage="Usage: $progName <mask-image> <inimage>"

eval `xiteStdOpt 2 2 $0 $@ "$usage"`

regionAnalyse -header -nc 8 -objnr $extract $1 $2 | \
  sed -e '/^[ ]*0 /d' -e 's/^#......./#/' -e 's/^ ......//'
