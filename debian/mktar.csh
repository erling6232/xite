#!/bin/csh
#
# Run this script above the xite/ directory to produce
# a xite-3.4.4.tar.gz file without the CVS directories.
#

if (-d xite-3.4.4.OLD) then
	echo "Removing xite-3.4.4.OLD/..."
	rm -rf xite-3.4.4.OLD
endif

echo "Moving xite-3.4.4/ to xite-3.4.4.OLD/..."
mv xite-3.4.4 xite-3.4.4.OLD

echo "Copying xite/ to xite-3.4.4/..."
cp -r xite xite-3.4.4

echo "Stripping CVS files..."
find xite-3.4.4 -name 'CVS' -print | xargs rm -rf

echo "Creating xite-3.4.4.tar.gz..."
tar cf - xite-3.4.4 | gzip > xite-3.4.4.tar.gz
#rm -rf xite-3.4.4
