
/*

________________________________________________________________

        saturmod.c
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




/*P:saturmod*

________________________________________________________________

		saturmod
________________________________________________________________

Name:		saturmod - modify saturation in color map file

Syntax:		| saturmod [<option>] <infile> <outfile>

Description:    Reads and modifies a colormap file. See 'mct(1)' for 
                description of the color map file.

		Each set of (r,g,b) is converted to (i,h,s).
		The s (saturation) is modified.

		"-" or "-0" can be given for 'infile' to mean stdin.
		"-" or "-1" can be given for 'outfile' to mean stdout.

Options:	Use only one of '-f' or '-s'.

                &-f factor
                'factor' must be >=0. The saturation will be changed like this:

		| s_new = 65535 - (65536 - s_old)/factor

		&-s saturation
		'saturation' must fulfull 0.0 <= 'saturation' <= 1.0.
		The saturation will be changed like this:
		| s_new = saturation * 65535.

		Default: 'factor' = 2.0.

See also:	mct(1), rainbow(1), ihs(3), color2ascii(1)
Author:		Otto Milvang
Revised:        Svein Bøe
Examples:       | saturmod -s 0.8 mycol.map | mct > mycol.col
Id: 		$Id$
________________________________________________________________

*/


#include <math.h>
#include <xite/includes.h>
#include <xite/color.h>
#include <stdlib.h>
#include XITE_STDIO_H
#include XITE_STRING_H
#include XITE_STRTOL_H
#include <xite/readarg.h>
#include <xite/message.h>
#include <xite/ihs.h>

static int a2i(char *str)
{
  char *test;
  int val;
  val = strtol(str,&test,0);
  if (*test != 0) 
    Warning(2, "No integer found.\n");

  return(val);
}

static unsigned long a2ul(char *str)
{
  char *test;
  unsigned long val;
  val = strtol(str,&test,0);
  if (*test != 0) 
    Warning(2, "No unsigned long integer found.\n");

  return(val);
}

int main(int argc, char **argv)
{
  FILE *infile = NULL, *outfile = NULL;
  char buffer[132], s_n[32], s_r[32], s_g[32], s_b[32];
  unsigned short r, g, b, i, h, s;
  unsigned long n;
  double factor, saturation;
  int slen, len, usesat;
  char *sarg, *farg;

  Iset_message(TRUE);
  Iset_abort(FALSE);
  InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s [<option>] <infile> <outfile>\n\
       where <option> is one of\n\
       -s <saturation> : s_new = saturation * 65535 \n\
       -f <factor>     : s_new = 65535 - (65536 - s_old)/factor \n"));

  if (argc == 1) Usage(1, NULL);

  sarg = read_switch(&argc, argv, "-s", 1, NULL);
  farg = read_switch(&argc, argv, "-f", 1, NULL);

  if (argc < 3) Usage(1, "Illegal number of arguments.\n");

  if (sarg != NULL && farg != NULL)
    Usage(2, "Use either -s or -f.\n");
  
  saturation = sarg ? atof(sarg) : 0.8;
  factor     = farg ? atof(farg) : 2.0; 
  
  if (factor < 0.0) Error(2, "Factor must be >= 0\n");
  if (saturation < 0.0 || saturation > 1.0)
    Error(2, "Saturation must be 0 <= saturation <= 1.0\n");
  usesat = sarg != NULL;
 
  if (!strcmp(argv[1], "-") || !strcmp(argv[1], "-0")) infile  = stdin;
  if (!strcmp(argv[2], "-") || !strcmp(argv[2], "-1")) outfile = stdout;

  if (!infile && (infile = fopen(argv[1], "r")) == NULL)
    Error(2, "Can't open input file %s.\n", argv[1]);
  if (!outfile && (outfile = fopen(argv[2], "w")) == NULL)
    Error(2, "Can't open output file %s.\n", argv[2]);

  while (fgets(buffer, 132, infile) != NULL)
    {
      slen = strlen(buffer);
      if (slen > 0) buffer[slen-1] = 0;
      len = sscanf(buffer, "%s %s %s %s", s_n, s_r, s_g, s_b);

      if (len == 4 && s_n[0] != '#')
	{
	  n = a2ul(s_n);
	  r = a2i(s_r);
	  g = a2i(s_g);
	  b = a2i(s_b);

	  rgb2ihs_word(r, g, b, &i, &h, &s);
	  if (s > 0) 
	    {
	      if (usesat)
		s = saturation * 65535;
	      else
		s = 65535 - 1.0 *(65535-s)/factor;
	    }
	  ihs2rgb_word(i, h, s, &r, &g, &b);
	  sprintf(buffer, "%#8lx %5d %5d %5d", n, r, g, b);
	}
      fprintf(outfile, "%s\n", buffer);
    }
  return(0);
}
