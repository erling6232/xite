
/*C*

________________________________________________________________

        regionAnalyse
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



#include <xite/includes.h>
#include <xite/biff.h>
#include <stdlib.h>
#include XITE_STDIO_H
#include <xite/blab.h>
#include <xite/color.h>
#include <xite/compu4.h>
#include <xite/draw.h>
#include <xite/histo.h>
#include <xite/message.h>
#include <xite/region.h>
#include <xite/readarg.h>
#include <xite/statistics.h>
#include XITE_STRING_H
#include XITE_MALLOC_H


/*P:regionAnalyse*

________________________________________________________________

		regionAnalyse
________________________________________________________________

Name:		regionAnalyse - split BIFF image into regions and describe
                regions

Syntax:		regionAnalyse [<option>...] <inimage> [<inimage2>]

Description:    'regionAnalyse' splits the image 'inimage1' into disjunct
                homogenous regions and calculates a number of properties for
                every region or every set of regions (object). The result is
                written to stdout in ascii format. Some options modify the
                "behavior" of the program, others tell the system to measure
                and write a specific property for every region or object.
                Properties are written in the order in which they appear in
                the options list.

		A number of properties (minpix, maxpix, xmean, ymean, moment,
		etc.) allow a second image 'inimage2' to be specified. If
		specified, it is used for the calculation of these properties,
		while still using 'inimage1' to define the extent of the
		regions. If 'inimage2' is missing, 'inimage1' is used to
		define the regions as well as to supply pixel values for these
		measurements.

Behaviour options:
		&-header
		Print a header showing the contents of the table.

		&-nc nc
		Neighbour connectedness: 4 or 8 (default 4).
		'nc' applies to objects (pixel value > 0).
		The opposite applies to the background (=0).

                &-sort
		Sort regions with respect to object number, store in an
		object-array.

		&-split
		Split objects (except the background, the object with object
		number 0) so that every region forms an independent object.

		&-cursor x y
		Define a cursor point ('x', 'y').
		
		&-if expr
		Use only regions which make the expression 'expr' TRUE.
		'expr' is a logical expression.

		&&Variables available for use in 'expr'
		| N - Object number.
		| R - Region number.
		| A - Region area.
		| W - Region width.
		| H - Region height.
		| M - Region pixel mean.
		| CR - Region number of cursor point
		| CN - Object number of cursor point

		&&Operators available in 'expr'
		| + - / *
		| < <= == >= > != LT LE EQ GE GT NE
		| AND OR NOT
		
		&&Examples:
		| -if 'A > 50'
		| -if 'A * 0.75 > W * H AND W > H'
		| -cursor 45 70 -if 'CR EQ R'

		-&&

		&-o outimg
		Used in combination with '-if' to make a new image containing
		only the regions which makes the if-expr TRUE.

		&-bg val
		Set the background pixel value in 'outimg' to 'val'. 
              
Property options:
		&-objnr
		Write object number.

		&-regnr
		Write region number.

		&-area
		Measure and write the area.

		&-perimeter
		Measure and write the perimeter.

		&-xmin
		Measure and write xmin.

		&-ymin
		Measure and write ymin.

		&-xmax
		Measure and write xmax.

		&-ymax
		Measure and write ymax.

		&-minmax
		Measure and write xmin, ymin, xmax and ymax.

		&-height
		Measure and write the height of the region.

		&-width
		Measure and write the width of the region.

		&-point
		Write the x and y coordinates of a point in the region (the
		leftmost point on the upper line).

		-&
		Property options acting on 'inimage2' if present,
		otherwise on 'inimage1':

		&-minpix
		Measure and write minimum pixel value within region.

		&-maxpix
		Measure and write maximum pixel value within region.

		&-minmaxpix
		Measure and write minimum and maximum pixel values within
		region.

		&-xmean
		Measure and write xmean, using 'inimage2' as weights.

		&-ymean
		Measure and write ymean, using 'inimage2' as weights.

		&-xymean
		Measure and write xmean and ymean, using 'inimage2' as weights.

		&-moment p q
		Measure and write the moment of order 'p'+'q', using 'inimage2'
		if present. This option may be used repreatedly with different
		order arguments.

		&-cmoment p q
		Measure and write the central moment of order 'p'+'q', using
		'inimage2' if present. This option may be used repreatedly
		with different order arguments.

		&-imoment
		7 invariant moments.

		&-volume
		Measure and write the volume under 'inimage2' if present.

		&-mean
		Measure and write the mean of the pixel values within the
		region(s).

		&-median
		Measure and write the median of the pixel values.

		&-stdev
		Measure and write the standard deviation of the pixel values.

		-&
		Note: If using '-sort' together with '-regnr', only the first
		region number in each object will be written.

Files:	        region.h
See also:       regionArea(3),
		regionConvexHull(3), regionCreate(3), regionDraw(3),
		regionFree(3), regionHistoUpdate(3), regionHisto(3),
		regionImoment(3), regionInsertYline(3), regionDeleteYline(3),
		regionEndInsert(3), regionXmean(3), regionYmean(3),
		regionMean(3), regionXmin(3), regionXmax(3), regionYmin(3),
		regionYmax(3), regionMinMax(3), regionCMoment(3),
		regionPMoment(3), regionMoment(3), regionPerform(3),
		regionPerimeter(3), regionIsPoint(3), regionPoint(3),
		regionSearch(3), regionSort(3), regionCopy(3), regionUnion(3),
		regionOverlap(3), regionNeighbor(3), regionVolume(3),
		regionYline(3)

Restrictions:   Input image must have bands with pixel type unsigned byte.

Author:		Tor Lønnestad, BLAB, Ifi, UiO.
                Some additional features: Otto Milvang

Examples:       | regionAnalyse -header -if 'A > 50' -regnr -area \\
                |     -width monat.img > mona.txt

Id: 		$Id$
________________________________________________________________

*/

#define TESTHSTAT { if (NOT hstat)					\
       { hstat = TRUE;							\
         if (band2) regionHisto(band2, reg, TRUE, h);			\
	 else regionHisto(band1, reg, TRUE, h);				\
         hstatistics(h, &minpix, &maxpix, &median, &meanpix, &stdev); }}

#define MAXSWITCHES	100

#define	OBJNR		1
#define	REGNR		2
#define	AREA		3
#define	PERIMETER	4
#define	VOLUME		5
#define XMEAN		6
#define YMEAN		7
#define	XYMEAN		8
#define	MOMENT		9
#define	CMOMENT		10
#define XMIN		11
#define YMIN		12
#define XMAX		13
#define YMAX		14
#define	MINMAX		15
#define HEIGHT		16
#define WIDTH		17
#define MINPIX		18
#define MAXPIX		19
#define	MINMAXPIX	20
#define	POINT		21
#define	MEAN		22
#define	MEDIAN		23
#define	STDEV		24
#define	IMOMENT		25

#define nilc		((char*)0)
#define NEW(t)		((t*)malloc(sizeof(t)))
 
static int nc;

typedef struct {
  char* swPos;
  int swType;
  char* swData;
} Switch;

typedef struct { int p, q; } momentData ; 

static momentData* momData(char *s)
{
  momentData* md;
  md = NEW(momentData); if (NOT md) fprintf(stderr, "Error!\n");
  if (sscanf(s, "%d%d", &(md->p), &(md->q)) NE 2) 
    { printf("Error: bad arguments to moment\n"); return(0); }
  return(md);
}

static void wrObjnr(region *reg)
{
  printf(" %5d ", reg->objectnr);
}

static void wrRegnr(region *reg)
{
  printf(" %5d ", reg->regionnr);
}

static void wrArea(region *reg)
{
  int area;
  regionArea(reg, TRUE, &area);
  printf(" %7d ", area);
}

static void wrPerim(region *reg)
{
  int perimeter;
  regionPerimeter(reg, nc, TRUE, &perimeter);
  printf(" %5d ", perimeter);
}

static void wrVolume(region *reg, double meanpix)
{
  int area;
  regionArea(reg, TRUE, &area);
  printf(" %10.0f ", meanpix*area);
}

static void wrXmean(IBAND band, region *reg)
{
  double xmean;
  regionXmean(band, reg, TRUE, &xmean);
  printf(" %10.3f ", xmean);
}

static void wrYmean(IBAND band, region *reg)
{
  double ymean;
  regionYmean(band, reg, TRUE, &ymean);
  printf(" %10.3f ", ymean);
}

static void wrXYmean(IBAND band, region *reg)
{
  double xmean, ymean;
  regionMean(band, reg, TRUE, &xmean, &ymean);
  printf(" %10.3f  %10.3f ", xmean, ymean);
}

static void wrMoment(IBAND band, region *reg, momentData *md)
{
  double mom;
  if (NOT md) fprintf(stderr, "Error\n");
  regionMoment(band, reg, md->p, md->q, TRUE, &mom);
  printf(" %10.3f ", mom);
}

static void wrCMoment(IBAND band, region *reg, momentData *md)
{
  double mom;
  regionCMoment(band, reg, md->p, md->q, TRUE, &mom);
  printf(" %10.3f ", mom);
}

static void wrImoment(IBAND band, region *reg)
{
  double mom[8];
  int i;
  regionImoment(band, reg, TRUE, mom);
  for(i=1; i<=7; i++) printf(mom[i]*mom[i] < 0.000001 ? " %9.3g " : " %9.6f ", mom[i]);
}

static void wrXmin(region *reg)
{
  int xmin;
  regionXmin(reg, TRUE, &xmin);
  printf(" %5d ", xmin);
}

static void wrYmin(region *reg)
{
  printf(" %5d ", reg->ymin);
}

static void wrXmax(region *reg)
{
  int xmax;
  regionXmax(reg, TRUE, &xmax);
  printf(" %5d ", xmax);
}

static void wrYmax(region *reg)
{
  printf(" %5d ", reg->ymax);
}

static void wrMinMax(region *reg)
{
  int xmin, ymin, xmax, ymax;
  regionMinMax(reg, TRUE, &xmin, &ymin, &xmax, &ymax);
  printf(" %5d  %5d  %5d  %5d ", xmin, ymin, xmax, ymax);
}

static void wrHeight(region *reg)
{
  printf(" %5d ", (reg->ymax - reg->ymin + 1));
}

static void wrWidth(region *reg)
{
  int xmin, ymin, xmax, ymax;
  regionMinMax(reg, TRUE, &xmin, &ymin, &xmax, &ymax);
  printf(" %5d ", (xmax - xmin + 1));
}

static void wrMinPix(int minpix)
{
  printf(" %5d ", minpix);
}

static void wrMaxPix(int maxpix)
{
  printf(" %5d ", maxpix);
}

static void wrMinMaxPix(int minpix, int maxpix)
{
  printf(" %5d  %5d ", minpix, maxpix);
}

static void wrPoint(region *reg)
{
  printf(" %5d  %5d ", reg->ylines[reg->ymin]->xmin, reg->ymin);
}

static void wrMean(double meanpix)
{
  printf(" %10.3f ", meanpix);
}

static void wrMedian(int med)
{
  printf(" %5d ", med);
}

static void wrStdev(double stdev)
{
  printf(" %10.3f ", stdev);
}

static void permutate(Switch *sws, int *perm, int nsw)
{
  int a, b, min;
  for (a=0; a LT nsw; a++) perm[a] = a;
  FOR (a=0; a LT nsw-1; a++)
    min = a;
    for (b=a+1; b LT nsw; b++)
      if (sws[perm[b]].swPos LT sws[perm[min]].swPos) min = b;
    b = perm[min];
    perm[min] = perm[a];
    perm[a] = b;
  ENDFOR;
}

static char *headers[] = {
  "",
  " OBJNR ",
  " REGNR ",
  "  AREA   ",
  " PERIM ",
  "   VOLUME   ",
  "   XMEAN    ",
  "   YMEAN    ",
  "   XMEAN       YMEAN    ",
  "   MOMENT   ",
  "  CMOMENT   ",
  " XMIN  ",
  " YMIN  ",
  " XMAX  ",
  " YMAX  ",
  " XMIN   YMIN   XMAX   YMAX  ",
  " HEIGHT",
  " WIDTH ",
  " MINPIX",
  " MAXPIX",
  " MINPIX MAXPIX",
  " XPOINT YPOINT",
  "    MEAN    ",
  " MEDIAN",
  "    STDEV   ",
  "   IMOM 1     IMOM 2     IMOM 3     IMOM 4     IMOM 5     IMOM 6     IMOM 7  ",
};

static void wrHeader(Switch *sws, int *perm, int nsw)
{
  int sw, hlen=0;

  printf("#");
  FOR (sw=0; sw LT nsw; sw++)
    printf("%s", headers[sws[perm[sw]].swType]);
    hlen += strlen(headers[sws[perm[sw]].swType]);
  ENDFOR;
  printf("\n#");
  for (sw=0; sw LT hlen; sw++) printf("-");
  printf("\n");
}


int main(int argc, char **argv)
{
  IMAGE i1, i2;
  IBAND band1, band2;
  region** regs, **preg;
  region* reg;
  Switch switches[MAXSWITCHES];
  int perm[MAXSWITCHES]; /* array for permutation of switches */
  int header, sort, split, size, n, swn, swNo = 0;
  histogram h;
  int minpix, maxpix, median, hstat;
  int cursorx, cursory, cursorr, cursoro;
  double meanpix, stdev;
  char *ifline, *cursorline;
  char *outfile, *args;
  int bg;
  int area, xmin, xmax, ymin, ymax;

  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [<option>...] <inimage1> [<inimage2>]\n\
       See manual for the numerous options.\n"));

  if (argc == 1) Usage(1, NULL);
  args = argvOptions(argc, argv);

  /* behaviour options */
  header     = read_bswitch(&argc, argv, "-header");
  nc         = read_iswitch(&argc, argv, "-nc", 4);
  if ((nc NE 4) AND (nc NE 8)) nc = 4;
  sort       = read_bswitch(&argc, argv, "-sort");
  split      = read_bswitch(&argc, argv, "-split");
  ifline     = read_switch(&argc, argv, "-if", 1, NULL);
  outfile    = read_switch(&argc, argv, "-o", 1, NULL);
  cursorline = read_switch(&argc, argv, "-cursor", 2, "0 0");
  sscanf(cursorline, "%d%d", &cursorx, &cursory);
  bg         = read_iswitch(&argc, argv, "-bg", 0);

  /* property switches */
  if ((switches[swNo].swPos = read_switch(&argc, argv, "-objnr", FALSE, nilc)))
    { switches[swNo].swType = OBJNR; switches[swNo++].swData = nilc; }
  if ((switches[swNo].swPos = read_switch(&argc, argv, "-regnr", FALSE, nilc)))
    { switches[swNo].swType = REGNR; switches[swNo++].swData = nilc;}
  if ((switches[swNo].swPos = read_switch(&argc, argv, "-area", FALSE, nilc)))
    { switches[swNo].swType = AREA; switches[swNo++].swData = nilc;}
  if ((switches[swNo].swPos =
       read_switch(&argc, argv, "-perimeter", FALSE, nilc)))
    { switches[swNo].swType = PERIMETER; switches[swNo++].swData = nilc;}
  if ((switches[swNo].swPos =
       read_switch(&argc, argv, "-volume", FALSE, nilc)))
    { switches[swNo].swType = VOLUME; switches[swNo++].swData = nilc;}
  if ((switches[swNo].swPos = read_switch(&argc, argv, "-xmean", FALSE, nilc)))
    { switches[swNo].swType = XMEAN; switches[swNo++].swData = nilc;}
  if ((switches[swNo].swPos = read_switch(&argc, argv, "-ymean", FALSE, nilc)))
    { switches[swNo].swType = YMEAN; switches[swNo++].swData = nilc;}
  if ((switches[swNo].swPos =
       read_switch(&argc, argv, "-xymean", FALSE, nilc)))
    { switches[swNo].swType = XYMEAN; switches[swNo++].swData = nilc;}
  while ((switches[swNo].swPos = read_switch(&argc, argv, "-moment", 2, nilc)))
    { switches[swNo].swType = MOMENT;
    switches[swNo].swData = (char*)momData(switches[swNo].swPos);
    swNo++;}
  while ((switches[swNo].swPos =
	  read_switch(&argc, argv, "-cmoment", 2, nilc)))
    { switches[swNo].swType = CMOMENT;
    switches[swNo].swData = (char*)momData(switches[swNo].swPos);
    swNo++;}
  if ((switches[swNo].swPos = read_switch(&argc, argv, "-xmin", FALSE, nilc)))
    { switches[swNo].swType = XMIN; switches[swNo++].swData = nilc;}
  if ((switches[swNo].swPos = read_switch(&argc, argv, "-ymin", FALSE, nilc)))
    { switches[swNo].swType = YMIN; switches[swNo++].swData = nilc;}
  if ((switches[swNo].swPos = read_switch(&argc, argv, "-xmax", FALSE, nilc)))
    { switches[swNo].swType = XMAX; switches[swNo++].swData = nilc;}
  if ((switches[swNo].swPos = read_switch(&argc, argv, "-ymax", FALSE, nilc)))
    { switches[swNo].swType = YMAX; switches[swNo++].swData = nilc;}
  if ((switches[swNo].swPos =
       read_switch(&argc, argv, "-minmax", FALSE, nilc)))
    { switches[swNo].swType = MINMAX; switches[swNo++].swData = nilc;}
  if ((switches[swNo].swPos =
       read_switch(&argc, argv, "-height", FALSE, nilc)))
    { switches[swNo].swType = HEIGHT; switches[swNo++].swData = nilc;}
  if ((switches[swNo].swPos = read_switch(&argc, argv, "-width", FALSE, nilc)))
    { switches[swNo].swType = WIDTH; switches[swNo++].swData = nilc;}
  if ((switches[swNo].swPos =
       read_switch(&argc, argv, "-minpix", FALSE, nilc)))
    { switches[swNo].swType = MINPIX; switches[swNo++].swData = nilc;}
  if ((switches[swNo].swPos =
       read_switch(&argc, argv, "-maxpix", FALSE, nilc)))
    { switches[swNo].swType = MAXPIX; switches[swNo++].swData = nilc;}
  if ((switches[swNo].swPos =
       read_switch(&argc, argv, "-minmaxpix", FALSE, nilc)))
    { switches[swNo].swType = MINMAXPIX; switches[swNo++].swData = nilc;}
  if ((switches[swNo].swPos = read_switch(&argc, argv, "-point", FALSE, nilc)))
    { switches[swNo].swType = POINT; switches[swNo++].swData = nilc;}
  if ((switches[swNo].swPos = read_switch(&argc, argv, "-mean", FALSE, nilc)))
    { switches[swNo].swType = MEAN; switches[swNo++].swData = nilc;}
  if ((switches[swNo].swPos =
       read_switch(&argc, argv, "-median", FALSE, nilc)))
    { switches[swNo].swType = MEDIAN; switches[swNo++].swData = nilc;}
  if ((switches[swNo].swPos = read_switch(&argc, argv, "-stdev", FALSE, nilc)))
    { switches[swNo].swType = STDEV; switches[swNo++].swData = nilc;}
  if ((switches[swNo].swPos =
       read_switch(&argc, argv, "-imoment", FALSE, nilc)))
    { switches[swNo].swType = IMOMENT; switches[swNo++].swData = nilc;}

  if ((argc LT 2) OR (argc GT 3))
    Usage(2, "Illegal number of arguments.\n");

  if (swNo EQ 0 && (ifline EQ NULL OR outfile EQ NULL))
    exit(Error(3, "Really? No property switches? - OK\n"));

  /* sort acording to switch position - swPos */
  if (swNo > 0) permutate(switches, perm, swNo);

  /* read images */
  i1 = Iread_image(argv[1]);
  if (! i1) exit(Error(4,"Can't read file %s\n", argv[1]));
  band1 = i1[1];
  if (argc EQ 3) {
    i2 = Iread_image(argv[2]);
    if (! i2) exit(Error(5, "Can't read file %s\n", argv[2]));
    band2 = i2[1];
  } else {
    band2 = (IBAND)0;
  }

  /* split into homogenous regions */
  regionYline(band1, &regs, nc, sort, split, &size);
  if (cursorx > 0 && cursory > 0) {
    for (n=0; n<=size; n++) {
      reg = regs[n];
      while (reg) {
	if (regionIsPoint(reg, 0, cursorx, cursory)) {
	    cursoro = reg->objectnr;
	    cursorr = reg->regionnr;
	}
	reg = reg->next;
      }
    }
  }
  if (ifline) {
    for (n=0; n<=size; n++) {
      preg = &regs[n];
      reg = *preg;
      while (reg) {
	hstat=0;
	regionArea(reg, 0, &area);
	regionMinMax(reg, 0, &xmin, &ymin, &xmax, &ymax);
	TESTHSTAT;
	compute_line("N = %d", reg->objectnr);
	compute_line("R = %d", reg->regionnr);
	compute_line("A = %d", area);
	compute_line("W = %d", xmax - xmin + 1);
	compute_line("H = %d", ymax - ymin + 1);
	compute_line("CR = %d", cursorr);
	compute_line("CN = %d", cursoro);
	compute_line("M = %f", meanpix);
	hstat = compute_line(ifline);
	if (hstat == 0) *preg = reg->next; else preg = &reg->next;
	reg = *preg;
      }
    }
  }

  if (header) wrHeader(switches, perm, swNo);

  /* measure and write properties */
  if (swNo > 0) {
    for (n=0; n LE size; n++) {
      if ((reg = regs[n])) {
	hstat=0;
	for (swn=0; swn LT swNo; swn++) {
	  switch (switches[perm[swn]].swType) {
	  case OBJNR:	  wrObjnr(reg);	break;
	  case REGNR:	  wrRegnr(reg);	break;
	  case AREA:	  wrArea(reg);	break;
	  case PERIMETER: wrPerim(reg);	break;
	  case VOLUME:    TESTHSTAT; wrVolume(reg, meanpix);	break;
	  case XMEAN:	  wrXmean(band2, reg);	break;
	  case YMEAN:	  wrYmean(band2, reg);	break;
	  case XYMEAN:    wrXYmean(band2, reg); break;
	  case MOMENT:    wrMoment( band2, reg,
				    (momentData *)switches[perm[swn]].swData);
	  break;
	  case CMOMENT:   wrCMoment(band2, reg,
				    (momentData *)switches[perm[swn]].swData);
	  break;
	  case XMIN:	  wrXmin(reg);      break;
	  case YMIN:	  wrYmin(reg);      break;
	  case XMAX:	  wrXmax(reg);      break;
	  case YMAX:	  wrYmax(reg);      break;
	  case MINMAX:    wrMinMax(reg);    break;
	  case HEIGHT:    wrHeight(reg);    break;
	  case WIDTH:     wrWidth(reg);     break;
	  case MINPIX:    TESTHSTAT; wrMinPix(minpix);	break;
	  case MAXPIX:    TESTHSTAT; wrMaxPix(maxpix);	break;
	  case MINMAXPIX: TESTHSTAT; wrMinMaxPix(minpix, maxpix); break;
	  case POINT:     wrPoint(reg);     break;
	  case MEAN:	  TESTHSTAT; wrMean(meanpix); break;
	  case MEDIAN:    TESTHSTAT; wrMedian(median);	break;
	  case STDEV:     TESTHSTAT; wrStdev(stdev); break;
	  case IMOMENT:   TESTHSTAT; wrImoment(band2, reg); break;
	  default: Warning(6, "Error: bad switch type: %d\n",
			   switches[perm[swn]].swType);
	  break;
	  }
	}
	printf("\n");
      }
    }
  }

  if (outfile) {
    drawBand(band1, (double) bg, 0.0);
    for (n=0; n LE size; n++) {
      if ((reg = regs[n])) {
	regionDraw(band1, reg, 0, reg->objectnr);
      }
    }
    Ihistory(i1, "regionAnalyse", ifline);
    Iwrite_image(i1, outfile);
  }

  return(0);
}
