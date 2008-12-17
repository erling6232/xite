

/*

________________________________________________________________

        histo2ps.c
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

#include <stdlib.h>
#include <math.h>
#include <xite/includes.h>
#include <xite/biff.h>
#include XITE_STDIO_H
#include <xite/readarg.h>
#include <xite/histo.h>
#include <xite/message.h>
#include XITE_TIME_H

static void print_all(FILE *outfile, double *h, double max, char *header, int ps)
{
  int i;
  time_t now;

  time(&now);

  if (ps) fprintf(outfile, "%%!PS-Adobe-3.0\n");
  else fprintf(outfile, "%%!PS-Adobe-3.0 EPSF-3.0\n");
  fprintf(outfile, "%%%%Title: Image Histogram\n");
  fprintf(outfile, "%%%%Creator: histo2ps in XITE\n");
  fprintf(outfile, "%%%%CreationDate: %s", ctime(&now));
  fprintf(outfile, "%%%%DocumentNeededResources: font Times-Roman Times-Bold\n");
  fprintf(outfile, "%%%%For: Postscript Printer\n");
  if (ps) {
    fprintf(outfile, "%%%%Pages: (atend)\n");
    fprintf(outfile, "%%%%BoundingBox: (atend)\n");
  } else {
    fprintf(outfile, "%%%%Pages: 1\n");
    fprintf(outfile, "%%%%BoundingBox: 0 0 510 535\n");
  }
  fprintf(outfile, "%%%%EndComments\n\n");

  fprintf(outfile, "%%%%BeginProlog\n\n");
  fprintf(outfile, "%% Font Handling Functions\n");
  fprintf(outfile, "%%\n");
  fprintf(outfile, "%% Function giving y-offset to center of font\n");
  fprintf(outfile, "%% Assumes font is set and uses numbers to gauge center\n");
  fprintf(outfile, "%%\n");
  fprintf(outfile, "/choose-font	%% stack: fontsize fontname => ---\n");
  fprintf(outfile, "{\n");
  fprintf(outfile, "   findfont \n");
  fprintf(outfile, "   exch scalefont \n");
  fprintf(outfile, "   setfont\n");
  fprintf(outfile, "   newpath\n");
  fprintf(outfile, "   0 0 moveto (0) true charpath flattenpath pathbbox\n");
  fprintf(outfile, "   /top exch def pop\n");
  fprintf(outfile, "   /bottom exch def pop\n");
  fprintf(outfile, "   bottom top bottom top add 2 div\n");
  fprintf(outfile, "   /center-font-val exch def \n");
  fprintf(outfile, "   /upper-font-val exch def \n");
  fprintf(outfile, "   /lower-font-val exch def\n");
  fprintf(outfile, "} def\n");
  fprintf(outfile, "%%\n");
  fprintf(outfile, "%% Justfication offset routines\n");
  fprintf(outfile, "%%\n");
  fprintf(outfile, "/center-x-just	%% stack: (string) x y => (string) newx y\n");
  fprintf(outfile, "{\n");
  fprintf(outfile, "   exch 2 index stringwidth pop 2 div sub exch\n");
  fprintf(outfile, "} def\n");
  fprintf(outfile, "%%\n");
  fprintf(outfile, "/left-x-just	%% stack: (string) x y => (string) newx y\n");
  fprintf(outfile, "{ \n");
  fprintf(outfile, "} def\n");
  fprintf(outfile, "%%\n");
  fprintf(outfile, "/right-x-just	%% stack: (string) x y => (string) newx y\n");
  fprintf(outfile, "{\n");
  fprintf(outfile, "   exch 2 index stringwidth pop sub exch\n");
  fprintf(outfile, "} def\n");
  fprintf(outfile, "%%\n");
  fprintf(outfile, "/center-y-just	%% stack: (string) x y => (string) x newy\n");
  fprintf(outfile, "{\n");
  fprintf(outfile, "   center-font-val sub\n");
  fprintf(outfile, "} def\n");
  fprintf(outfile, "%%\n");
  fprintf(outfile, "/lower-y-just	%% stack: (string) x y => (string) x newy\n");
  fprintf(outfile, "{\n");
  fprintf(outfile, "   lower-font-val sub\n");
  fprintf(outfile, "} def\n");
  fprintf(outfile, "%%\n");
  fprintf(outfile, "/upper-y-just	%% stack: (string) x y => (string) x newy\n");
  fprintf(outfile, "{\n");
  fprintf(outfile, "   upper-font-val sub\n");
  fprintf(outfile, "} def\n");
  fprintf(outfile, "%%\n");
  fprintf(outfile, "%% Shows a string on the page subject to justification\n");
  fprintf(outfile, "%%   \n");
  fprintf(outfile, "/just-string	%% stack: (string) x y just => ---\n");
  fprintf(outfile, "{\n");
  fprintf(outfile, "   dup 0 eq { pop center-x-just center-y-just 		} if\n");
  fprintf(outfile, "   dup 1 eq { pop left-x-just center-y-just		} if\n");
  fprintf(outfile, "   dup 2 eq { pop left-x-just upper-y-just	 	} if\n");
  fprintf(outfile, "   dup 3 eq { pop center-x-just upper-y-just 		} if\n");
  fprintf(outfile, "   dup 4 eq { pop right-x-just upper-y-just	 	} if\n");
  fprintf(outfile, "   dup 5 eq { pop right-x-just center-y-just 		} if\n");
  fprintf(outfile, "   dup 6 eq { pop right-x-just lower-y-just	 	} if\n");
  fprintf(outfile, "   dup 7 eq { pop center-x-just lower-y-just  		} if\n");
  fprintf(outfile, "   dup 8 eq { pop left-x-just lower-y-just	 	} if\n");
  fprintf(outfile, "   moveto show\n");
  fprintf(outfile, "} def\n");
  fprintf(outfile, "%%\n");
  fprintf(outfile, "%% Marker definitions\n");
  fprintf(outfile, "/mark0 {/size exch def /y exch def /x exch def\n");
  fprintf(outfile, "newpath x size sub y size sub moveto\n");
  fprintf(outfile, "size size add 0 rlineto 0 size size add rlineto\n");
  fprintf(outfile, "0 size size add sub 0 rlineto closepath fill} def\n");
  fprintf(outfile, "/mark1 {/size exch def /y exch def /x exch def\n");
  fprintf(outfile, "newpath x size sub y size sub moveto\n");
  fprintf(outfile, "size size add 0 rlineto 0 size size add rlineto\n");
  fprintf(outfile, "0 size size add sub 0 rlineto closepath stroke} def\n");
  fprintf(outfile, "/mark2 {/size exch def /y exch def /x exch def\n");
  fprintf(outfile, "newpath x y moveto x y size 0 360 arc stroke} def\n");
  fprintf(outfile, "/mark3 {/size exch def /y exch def /x exch def\n");
  fprintf(outfile, "newpath x size sub y size sub moveto x size add y size add lineto\n");
  fprintf(outfile, "x size sub y size add moveto x size add y size sub lineto stroke} def\n");
  fprintf(outfile, "/mark4 {/size exch def /y exch def /x exch def\n");
  fprintf(outfile, "newpath x size sub y moveto x y size add lineto\n");
  fprintf(outfile, "x size add y lineto x y size sub lineto\n");
  fprintf(outfile, "closepath stroke} def\n");
  fprintf(outfile, "/mark5 {/size exch def /y exch def /x exch def\n");
  fprintf(outfile, "x y size mark1\n");
  fprintf(outfile, "newpath x size sub y moveto size size add 0 rlineto stroke} def\n");
  fprintf(outfile, "/mark6 {/size exch def /y exch def /x exch def\n");
  fprintf(outfile, "newpath x y moveto x y size 0 360 arc fill} def\n");
  fprintf(outfile, "/mark7 {/size exch def /y exch def /x exch def\n");
  fprintf(outfile, "newpath x y moveto x size sub y size sub lineto\n");
  fprintf(outfile, "x size add y size sub lineto closepath fill\n");
  fprintf(outfile, "newpath x y moveto x size add y size add lineto\n");
  fprintf(outfile, "x size sub y size add lineto closepath fill} def\n");
  fprintf(outfile, "%%\n");

  fprintf(outfile, "/Yscale \n");
  fprintf(outfile, "{ %10.6f mul 731 add} def\n", 100.0/max * 64 );
  fprintf(outfile, "/Xscale \n");
  fprintf(outfile, "{ 25 mul 1292 add} def\n");
  fprintf(outfile, "\n");

  fprintf(outfile, "/Point \n");
  fprintf(outfile, "{\n");
  fprintf(outfile, "  newpath\n");
  fprintf(outfile, "  exch\n");
  fprintf(outfile, "  0.5 add\n");
  fprintf(outfile, "  Xscale\n");
  fprintf(outfile, "  dup\n");
  fprintf(outfile, "  0 Yscale\n");
  fprintf(outfile, "  moveto \n");
  fprintf(outfile, "  exch\n");
  fprintf(outfile, "  Yscale\n");
  fprintf(outfile, "  lineto \n");
  fprintf(outfile, "  stroke\n");
  fprintf(outfile, "} def\n");

 
  fprintf(outfile, "%%%%EndProlog\n\n");

  fprintf(outfile, "%%%%BeginSetup\n");
  fprintf(outfile, "%%%%IncludeResource: font Times-Roman\n");
  fprintf(outfile, "%%%%IncludeResource: font Times-Bold\n");
  fprintf(outfile, "%% Set the scale\n");
  fprintf(outfile, "0.06 0.06 scale\n");
  fprintf(outfile, "%%%%EndSetup\n\n");

  fprintf(outfile, "%%%%Page: 1 1\n");

  fprintf(outfile, "%% Main body begins here\n");
  fprintf(outfile, "%%\n");

  fprintf(outfile, "300 /Times-Bold choose-font\n");
  fprintf(outfile, "(%s) 4488 8856 3 just-string\n", header);
  fprintf(outfile, "200 /Times-Roman choose-font\n");
  fprintf(outfile, "%%(par) 8556 8104 2 just-string\n");

  fprintf(outfile, "(\\%% pix) 90 8496 2 just-string\n");
  fprintf(outfile, "%%() 8886 380 5 just-string\n");
  fprintf(outfile, "\n");
  fprintf(outfile, "(%6.2f) 860 %6.2f Yscale 5 just-string\n", 0.0, 0.0);
  fprintf(outfile, "(%6.2f) 860 %6.2f Yscale 5 just-string\n", 
	  0.25 * max, 0.25*max);
  fprintf(outfile, "(%6.2f) 860 %6.2f Yscale 5 just-string\n", 
	  0.5 * max, 0.50*max);
  fprintf(outfile, "(%6.2f) 860 %6.2f Yscale 5 just-string\n", 
	  0.75 * max, 0.75*max);
  fprintf(outfile, "(%6.2f) 860 %6.2f Yscale 5 just-string\n", max, max);
  fprintf(outfile, "\n");
  fprintf(outfile, "(0) 0 Xscale 90 7 just-string\n");
  fprintf(outfile, "(64) 64 Xscale 90 7 just-string\n");
  fprintf(outfile, "(128) 128 Xscale 90 7 just-string\n");
  fprintf(outfile, "(192) 192 Xscale 90 7 just-string\n");
  fprintf(outfile, "(256) 256 Xscale 90 7 just-string\n");
  fprintf(outfile, "\n");
  fprintf(outfile, "[] 0 setdash\n");
  fprintf(outfile, "8 setlinewidth\n");
  fprintf(outfile, "newpath\n");
  fprintf(outfile, "  950 0 Yscale moveto\n");
  fprintf(outfile, "  8466 0 Yscale lineto\n");
  fprintf(outfile, "stroke\n");
  fprintf(outfile, "[8 24] 0 setdash\n");
  fprintf(outfile, "2 setlinewidth\n");
  fprintf(outfile, "newpath\n");
  fprintf(outfile, "  950 %6.2f Yscale moveto\n", 0.25 * max);
  fprintf(outfile, "  8466 %6.2f Yscale lineto\n", 0.25 * max);
  fprintf(outfile, "stroke\n");
  fprintf(outfile, "\n");
  fprintf(outfile, "[8 24] 0 setdash\n");
  fprintf(outfile, "2 setlinewidth\n");
  fprintf(outfile, "newpath\n");
  fprintf(outfile, "  950 %6.2f Yscale moveto\n", 0.5 * max);
  fprintf(outfile, "  8466 %6.2f Yscale lineto\n", 0.5 * max);
  fprintf(outfile, "stroke\n");
  fprintf(outfile, "[8 24] 0 setdash\n");
  fprintf(outfile, "2 setlinewidth\n");
  fprintf(outfile, "newpath\n");
  fprintf(outfile, "  950 %6.2f Yscale moveto\n", 0.75 * max);
  fprintf(outfile, "  8466 %6.2f Yscale lineto\n", 0.75 * max);
  fprintf(outfile, "stroke\n");
  fprintf(outfile, "[8 24] 0 setdash\n");
  fprintf(outfile, "2 setlinewidth\n");
  fprintf(outfile, "newpath\n");
  fprintf(outfile, "  950 %6.2f Yscale moveto\n", max);
  fprintf(outfile, "  8466 %6.2f Yscale lineto\n", max);
  fprintf(outfile, "stroke\n");
  fprintf(outfile, "\n");
  fprintf(outfile, "[] 0 setdash\n");
  fprintf(outfile, "8 setlinewidth\n");
  fprintf(outfile, "newpath\n");
  fprintf(outfile, "  0 Xscale 8106 moveto\n");
  fprintf(outfile, "  0 Xscale 380 lineto\n");
  fprintf(outfile, "stroke\n");
  fprintf(outfile, "\n");
  fprintf(outfile, "[8 24] 0 setdash\n");
  fprintf(outfile, "2 setlinewidth\n");
  fprintf(outfile, "newpath\n");
  fprintf(outfile, "  64 Xscale 8106 moveto\n");
  fprintf(outfile, "  64 Xscale 380 lineto\n");
  fprintf(outfile, "stroke\n");
  fprintf(outfile, "[8 24] 0 setdash\n");
  fprintf(outfile, "2 setlinewidth\n");
  fprintf(outfile, "newpath\n");
  fprintf(outfile, "  128 Xscale 8106 moveto\n");
  fprintf(outfile, "  128 Xscale 380 lineto\n");
  fprintf(outfile, "stroke\n");
  fprintf(outfile, "[8 24] 0 setdash\n");
  fprintf(outfile, "2 setlinewidth\n");
  fprintf(outfile, "newpath\n");
  fprintf(outfile, "  192 Xscale 8106 moveto\n");
  fprintf(outfile, "  192 Xscale 380 lineto\n");
  fprintf(outfile, "stroke\n");
  fprintf(outfile, "[8 24] 0 setdash\n");
  fprintf(outfile, "2 setlinewidth\n");
  fprintf(outfile, "newpath\n");
  fprintf(outfile, "  256 Xscale 8106 moveto\n");
  fprintf(outfile, "  256 Xscale 380 lineto\n");
  fprintf(outfile, "stroke\n");
  fprintf(outfile, "\n");
  fprintf(outfile, "[] 0 setdash 1 Xscale 0 Xscale sub 2 div setlinewidth \n");
  fprintf(outfile, "\n");
  fprintf(outfile, "\n");
  fprintf(outfile, "\n");
  fprintf(outfile, "\n");
  fprintf(outfile, "\n");

  for (i=0; i<=255; i++)
    fprintf(outfile, "%d %10.6f     Point \n", i, h[i]);
  fprintf(outfile, "\n");
  fprintf(outfile, "\n");
  fprintf(outfile, "%% End of xgraph output\n");
  fprintf(outfile, "\nshowpage\n\n");
  fprintf(outfile, "%%%%Trailer\n");
  if (ps) {
    fprintf(outfile, "%%%%BoundingBox: 0 0 510 535\n");
    fprintf(outfile, "%%%%Pages: 1\n");
  }
  fprintf(outfile, "%%%%EOF\n");
}

static int mkHistoMask(IBAND band, IBAND mask, int val, int *histo)
{
  int x,y,xsize,ysize, ant;

  if (Ipixtyp(band) != Iu_byte_typ)
    return(Error(-1, "Image band must be unsigned byte pixel type.\n"));
  if (mask && Ipixtyp(mask) != Iu_byte_typ)
    return(Error(-2, "Mask band must be unsigned byte pixel type.\n"));

  xsize = Ixsize(band);
  ysize = Iysize(band);

  for (x=0; x < 256; histo[x++] = 0) ;

  if (mask) {
    ant = 0;

    for (y=1; y <= ysize; y++)
      for (x=1; x <= xsize; x++)
	if (val == mask[y][x]) {
	  ++ histo[band[y][x]];
	  ++ ant;
	}
  } else {
    for (y=1; y <= ysize; y++)
      for (x=1; x <= xsize; x++)
	++ histo[band[y][x]];
    ant = xsize * ysize;
  }
  return(ant);
}


/*P:histo2ps*

________________________________________________________________

		histo2ps
________________________________________________________________

Name:		histo2ps - make a Postscript plot of a histogram

Syntax:		| histo2ps [-mimg maskimage] [-mval maskval] [-hmax max]
                |          [-cum] [-h header] [-auto] [-ps] <infile>
		|          [<PostScript-file>]

Description:    Make a Postscript plot of a histogram from an image.
                The image must be a one band unsigned byte image.
		If a 'maskfile' is specified only pixels with
		a corresponding mask == 'maskval' (default 1) is counted in
		the histogram. All values will be presented in percent.
		You may scale the histogram by giving the 'max' 
		histogram value (default 100) or autoscaling with the switch
		'-auto'. The switch '-cum' gives the cumulative histogram.
		The PostScript-code will be Encapsulated PostScript
		(suitable for inclusion in other documents), unless
		the option '-ps' is used. The output is written to stdout or
		<PostScript-file>.

Author:		Otto Milvang, NR

Examples:       | histo2ps -hmax 10 mona.img
                | histo2ps -auto mona.img monaHisto.ps

Id: 		$Id$, Blab, UiO

________________________________________________________________

*/

int main(int argc, char **argv)
{
  IMAGE img, mimg;
  int h[256], i, ip, ant, aauto, cum, ps = 0;
  double d[256], dmax;
  char * mimgname, *header;
  double max;
  FILE *f;
  
  InitMessage(&argc, argv, xite_app_std_usage_text(
      "Usage %s [-mimg maskimage] [-mval maskval] [-hmax max] [-h header]\n\
            [-cum] [-auto] [-ps] <infile> [<PostScript-file>]\n"));
  Iset_abort(1);
  Iset_message(1);

  if (argc == 1) Usage(0, NULL);

  mimgname = read_switch(&argc, argv, "-mimg", 1, NULL);
  ip       = read_iswitch(&argc, argv, "-mval", 1);
  header   = read_switch(&argc, argv, "-h",1, "Histogram");
  max      = read_dswitch(&argc, argv, "-hmax",100.0);
  aauto    = read_bswitch(&argc, argv, "-auto");
  cum      = read_bswitch(&argc, argv, "-cum");
  ps       = read_bswitch(&argc, argv, "-ps");

  if (argc > 3) Usage(0, "Illegal number of arguments.\n");

  img  = Iread_image(argv[1]);
  mimg = mimgname ? Iread_image(mimgname) : 0;
  
  ant =  mkHistoMask(img[1], mimg ? mimg[1] : NULL, ip, h);

  if (ant < 0)
    Error(2, "Error in mkHistoMask().\n");

  if (ant == 0) Error(3, "No pixels counted.\n");

  if (cum) mkCumHisto(h, h);
  dmax = 0.0;
  for (i=0; i<256; i++) {
    d[i] = 100.0 * h[i]/ant;
    if (d[i] > dmax) dmax = d[i];
    if (d[i] > max) d[i] = max*1.02;
  }
  if (aauto) max = dmax * 1.02;
  if (argc == 3) f = fopen(argv[2], "w"); else f = stdout;
  
  print_all(f, d, max, header, ps);
  if (argc == 3) fclose(f);

  return(0);
}		/* end of histo2ps */
