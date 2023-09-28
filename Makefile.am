ACLOCAL_AMFLAGS=-I m4
SUBDIRS=data etc include lib src

SUFFIXES=.m

.c.m:
	$(top_srcdir)/src/cdoc/cdoc -t $< $@
	cdoc -t $< $@
