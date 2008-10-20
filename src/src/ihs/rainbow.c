

/*

________________________________________________________________

        rainbow.c
        $Id$
        Copyright 1990, Blab, UiO
        Image processing lab, Department of Informatics
        University of Oslo
        E-mail: blab@ifi.uio.no
________________________________________________________________
  
  Permission to use, copy, modify and distribute this software and its
  documentation for any purpose and without fee is hereby granted, 
  provided that this copyright notice appear in all copies and that 
  both that copyright notice and this permission notice appear in supporting
  documentation and that the name of B-lab, Department of Informatics or
  University of Oslo not be used in advertising or publicity pertaining 
  to distribution of the software without specific, written prior permission.

  B-LAB DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL B-LAB
  BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
  OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
  CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 

*/

static char *Id = "$Id$, Blab, UiO";

#ifdef MAIN

/*P:rainbow*

________________________________________________________________

		rainbow
________________________________________________________________

Name:		rainbow - create a color map based on ihs

Syntax:		| rainbow [-ib <ib>] [-ie <ie>] [-hb <hb>]
                |         [-he <he>] [-sb <sb>] [-se <se>] [-len len]
		|         <outfile>

Description:	Creates a colormap based on IHS-coordinates.
                You may set start and stop values for ihs.
		|
		| for (n=0; n<len; n++) {
		|   I = ib + (ie-ib) * (double) n / (double) len; 
		|   H = hb + (he-hb) * (double) n / (double) len; 
		|   S = sb + (se-sb) * (double) n / (double) len;
		|   convert (i, h, s) to (r, g, b)
		|
		Default values:
		| 	ib = 0.5,  ie = 0.5
		|	hb = 1/3,  he = 1.0
		|	sb = 1.0   se = 1.0
		| 	len = 256

		"-" or "-1" may be used for 'outfile' to send the result to
		stdout.

Restrictions:	All values must be in the range 0.0 <= val <= 1.0 

See also:       ihs(3)

Author:		Otto Milvang

Examples:	| rainbow -ib 0 -ie 1 | mct > mycol.col

Id: 		$Id$
________________________________________________________________

*/

#include <xite/includes.h>
#include <stdlib.h>
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#ifdef HAVE_STRINGS_H
# include <strings.h>
#else
# ifdef HAVE_STRING_H
#  include <string.h>
# endif
#endif
#include <xite/utils.h>
#include <math.h>
#include <xite/readarg.h>
#include <xite/message.h>
#include <xite/ihs.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int main(int argc, char **argv)
{
  int i, len;
  double fi, fh, fs, r,g,b, ib, ie, il, hb, he, hl, sb, se, sl;
  unsigned long min, max, c;
  FILE *outp;

  Iset_message(TRUE);
  Iset_abort(FALSE);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [-ib <ib>] [-ie <ie>] [-hb <hb>] \n\
       [-he <he>] [-sb <sb>] [-se <se>] [-len len] \n\
       <outfile>\n"));

  c = 0;
  max = 0L;
  min = 65000L;
  ib = read_dswitch(&argc, argv, "-ib", 0.5);
  ie = read_dswitch(&argc, argv, "-ie", 0.5);
  il = ie - ib;
  hb = read_dswitch(&argc, argv, "-hb", 0.0);
  he = read_dswitch(&argc, argv, "-he", 1.0);
  hl = he - hb;
  sb = read_dswitch(&argc, argv, "-sb", 1.0);
  se = read_dswitch(&argc, argv, "-se", 1.0);
  sl = se - sb;
  len = read_iswitch(&argc, argv, "-len", 256);

  if (argc == 1) Usage(1, NULL);
  if (argc > 1) {
    if (strcmp(argv[1], "-") == 0 || strcmp(argv[1], "-1") == 0) {
      outp = stdout;
    } else {
      outp = fopen((char *) tilde_expand(argv[1]), "w");
      if (outp == NULL) {
	Error(2, "%s: Cannot open %s.\n", argv[0], argv[1]);
	exit(2);
      }
    }
  } else {
    outp = stdout;
  }

  fprintf(outp, "#\n");
  fprintf(outp, "#       rainbow.map - %d\n", len);
  fprintf(outp, "#\n");
  fprintf(outp, "#       The color table is defined by\n");
  fprintf(outp, "#       normalized IHS values\n");
  if (ib == ie)
    fprintf(outp, "#       I = %6.4f\n", ib); else
    fprintf(outp, "#       I = %6.4f .. %6.4f\n", ib, ie);
  if (hb == he)
    fprintf(outp, "#       H = %6.4f\n", hb); else
    fprintf(outp, "#       H = %6.4f .. %6.4f\n", hb, he);
  if (sb == se)
    fprintf(outp, "#       S = %6.4f\n", sb); else
    fprintf(outp, "#       S = %6.4f .. %6.4f\n", sb, sb);
  fprintf(outp, "#\n");
  for (i=0;i<len; i++)
    {
      fi = il * i / (double) len + ib;
      fh = hl * i / (double) len + hb;
      fs = sl * i / (double) len + sb;
      ihs2rgb(fi, fh, fs, &r, &g, &b, 1);
      r *= 65535.0; 
      g *= 65535.0; 
      b *= 65535.0; 
      fprintf(outp, "%#8lx %5d %5d %5d\n", c++, (int) r, (int) g, (int) b);
      if (r>max) max = r;
      if (g>max) max = g;
      if (b>max) max = b;
      if (r<min) min = r;
      if (g<min) min = g;
      if (b<min) min = b;
    }
  if (outp != stdout) fclose(outp);

  fprintf(stderr, "Min = %ld, Max = %ld\n", min, max);
  return(0);
}

#endif /* MAIN */
