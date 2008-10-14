#include <xite/includes.h>
#include <stdio.h>
#include <stdlib.h>
#include <xite/biff.h>
#ifdef HAVE_STDIO_H
#  include <stdio.h>
#endif
#include <xite/draw.h>
#include <xite/message.h>

/*P:bdf2biff*

________________________________________________________________

		bdf2biff
________________________________________________________________

Name:		bdf2biff - converts a Bitmap Distribution Format font file to
                a BIFF file
Syntax:	        | bdf2biff <Bitmap Distribution Format file> [<Outfile>]

Description:    Converts Bitmap Distribution Format files (.bdf files)
                to a BIFF file of at least 255 bands, each of which
                contains a character. The foreground pixels in the image
                will get the value `0`, and the background pixels will get
                the value `255`. 

		Reads from <Bitmap Distribution Format file>. This must be
		given in the call. If <Outfile> is given, bdf2biff writes to
		the file <Outfile>, otherwise it writes to standard output.
 
		All characters in the .bdf file with ENCODING number greater
		than 0 and less than 256, will be placed in the band with that
		number in the biff image. Characters with ENCODING number -1
		will be placed in bands with numbers larger than 255. If there
		are numbers between 0 and 255 which have no corresponding
		character, the band with that number will only contain a
		single pixel. If there are characters with encoding number not
		between -1 and 255, the program will write an error message
		and stop without making the biff file. The ENCODING number of
		a character is usually the same as the ASCII number.

		The image info will contain the following:
        
		| #FONT <the font name>
		| #CHAR <name of char(glyph) 1> <corresp. bandno>
		| #CHAR <name of char(glyph) 2> <corresp. bandno>
		|  ...	     ...			     ...
		|  ...	     ...			     ...
		| #CHAR <name of char(glyph) N> <corresp. bandno>

                The font name is read from the FONT line in the .bdf file. The
                name of the characters are read from the STARTCHAR lines.

Author:		Oyvind Hillesund, Copyright NR
Developed by:   BILD, Norsk Regnesentral, Oslo, Norway
Examples:       bdf2biff charB08.bdf charB08.biff
See also:	bdf2biff(3)

________________________________________________________________

*/

static char *Id="$Id$";

int main(int argc, char *argv[])
{
 IMAGE biff_bitmap;
 
 Iset_message(1);
 Iset_abort(1);

 InitMessage(&argc, argv, xite_app_std_usage_text(
    "Usage: %s <infile> <outfile>\n"));
 if (argc == 1) Usage(1, NULL);

 if (argc != 2 && argc !=3) {
   Usage(2, "Wrong number of arguments.\n");
   exit(1);
 }
 biff_bitmap = bdf2biff(argv[1]);
 Iwrite_image(biff_bitmap,argv[2]);

 return(0);
}
