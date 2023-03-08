#include <xite/includes.h>
#include <stdio.h>
#include <stdlib.h>
#include <xite/biff.h>
#include XITE_STRING_H
#include <xite/draw.h>
#include <xite/message.h>

/*F:bdf2biff*

________________________________________________________________

		bdf2biff
________________________________________________________________

Name:		bdf2biff - converts a Bitmap Distribution Format font file 
                           into a BIFF image of at least 255 bands
Syntax:	        | #include <xite/draw.h>
                |
                | IMAGE bdf2biff( char bdf_filename[] );

Description:    Converts Bitmap Distribution Format files (.bdf files)
                to a BIFF image of at least 255 bands, each of which
                contains a character. The foreground pixels in the image
                will get the value `0`, and the background pixels will get
                the value `255`.

		The function reads from the file `bdf_filename` which 
		must be the name of a Bitmap Distribution Format file.
		bdf2biff returns the image containing the characters.
 
                All characters in the .bdf file with ENCODING number greater
                than 0 and less than 256, will be placed in the band with that
                number in the biff image. Characters with ENCODING number -1
                will be placed in bands with numbers larger than 255. If there
                are numbers between 0 and 255 which have no corresponding
                character, the band with that number will only contain a
                single pixel. If there are characters with encoding number not
                between -1 and 255, the program will write an error message
                and stop without making the biff image. The ENCODING number of
                a character is usually the same as the ASCII number.

		The x (and y) width of each character, read from the DWIDTH
		lines in the .bdf file, is kept by the variable xmag (and
		ymag) in the biff picture, and can be fetched by using the
		biff library function `Ixmag(band)` (and Iymag(band)).
		Bandnumbers without a character will have xmag (and ymag) =
		65535.
        
                The image info will contain the following:
        
		| #FONT <the font name>
		| #CHAR <name of char(glyph) 1> <corresp. bandno>
		| #CHAR <name of char(glyph) 2> <corresp. bandno>
		|  ...        ...                             ...
		|  ...        ...                             ...
		| #CHAR <name of char(glyph) N> <corresp. bandno>

                The font name is read from the FONT line in the .bdf file. The
                names of the characters are read from the STARTCHAR lines.

Author:		Øyvind Hillesund, NR, Copyright NR
Developed by:   BILD, Norsk Regnesentral, Oslo, Norway
See also:	bdf2biff(1)

________________________________________________________________

*/

#define FOREGROUND 0
#define BACKGROUND 255
#define MAXLINE 100
#define PIXELTYPE Iu_byte_typ

static void readline(FILE *pointer, char *buf)
{
  /* returns a line that is not a COMMENT line */
  char word[MAXLINE];
  
  fgets (buf,MAXLINE,pointer);
  sscanf (buf,"%s",word);
  while (strcmp(word,"COMMENT") == 0)
    {
      fgets(buf,MAXLINE,pointer);
      sscanf (buf,"%s",word);
    }
  return;
}

static char *hexa2bin(char string[])
{
  /* converts from hexadecimal number to binary number */
  char c, *bin;
  int n;
  
  bin = (char *) malloc((4*strlen(string) - 3)*sizeof(char));
  
  for (n = 0; n < strlen(string)-1; n++)
    {
      c = string[n];
      switch (c)
	{
	case '0': sprintf(&bin[4 * n],"%s","0000"); break;
	case '1': sprintf(&bin[4 * n],"%s","0001"); break;
	case '2': sprintf(&bin[4 * n],"%s","0010"); break;       
	case '3': sprintf(&bin[4 * n],"%s","0011"); break;
	case '4': sprintf(&bin[4 * n],"%s","0100"); break;
	case '5': sprintf(&bin[4 * n],"%s","0101"); break;
	case '6': sprintf(&bin[4 * n],"%s","0110"); break;
	case '7': sprintf(&bin[4 * n],"%s","0111"); break;
	case '8': sprintf(&bin[4 * n],"%s","1000"); break;
	case '9': sprintf(&bin[4 * n],"%s","1001"); break;
	case 'A': case 'a': sprintf(&bin[4 * n],"%s","1010"); break;
	case 'B': case 'b': sprintf(&bin[4 * n],"%s","1011"); break;
	case 'C': case 'c': sprintf(&bin[4 * n],"%s","1100"); break;
	case 'D': case 'd': sprintf(&bin[4 * n],"%s","1101"); break;
	case 'E': case 'e': sprintf(&bin[4 * n],"%s","1110"); break;
	case 'F': case 'f': sprintf(&bin[4 * n],"%s","1111"); break;
	}
    }
  return bin;
}

IMAGE bdf2biff(char bdf_filename[])
{
  IMAGE img;
  FILE *fp;
  IPIXTYP pt = PIXELTYPE;
  int xsize, ysize, xorigo, yorigo, no_chars, char_nr, code_nr, char_line;
  int xstart,ystart,n,m,BBw,BBh,BBox,BBoy,Irregularnumber, no_irregular ;
  int Xmag, Ymag;
  char buf[MAXLINE], word1[MAXLINE], *bin, name[14], c;
  
  Iset_message(1);  /* warnings from blab-software */
  Iset_abort(0);    /* but don't abort */
  
  if ((fp = fopen(bdf_filename,"r")) == NULL)
    {
      fprintf(stderr,"\nCan't open input bitmap file %s!\n\n",bdf_filename);
      exit(1);
    }
  
  no_irregular = 0;       /* Counting the number of irregular characters  */
  fgets(buf,MAXLINE,fp);  /* (Those with ENCODING -1) */
  while(strcmp(buf,"ENDFONT\n") != 0)
    {
      sscanf(buf,"%s",word1);
      if (strcmp(word1, "ENCODING") == 0)
	{
	  sscanf(buf,"%s %d",word1,&n);
	  if (n == -1)
	    no_irregular++;  
	} 
      fgets(buf,MAXLINE,fp);
    } 
  rewind(fp);
  Irregularnumber = 256; /* The irregular characters are put into */
                         /* bands with number larger than 255.    */   
  
  
  img = Init_image(255 + no_irregular , bdf_filename);
  fgets(buf,MAXLINE,fp);
  if (strcmp(buf, "STARTFONT 2.1\n") != 0)
    {
      fprintf(stderr,"\n%s is not a bdf-file\n",bdf_filename);
      exit(1);
    }
  readline(fp, buf);           /* reads the FONT line */
  sscanf(buf,"%s",word1);
  if (strcmp(word1,"FONT") != 0)
    {
      fprintf(stderr,"\nUnable to read the font name.\n");
      exit(1);
    }
  sscanf(buf,"%[^\n]",word1);
  sprintf(buf,"#%s\n",word1);    
  Iput_text(img,buf);
  readline(fp, buf);        /* reading the SIZE line*/
  readline(fp, buf);        /* reading the FONTBOUNDINGBOX line */
  if (sscanf(buf,"%s %d %d %d %d",word1,&xsize,&ysize,&xorigo,&yorigo) != 5
      || strcmp(word1,"FONTBOUNDINGBOX") != 0 )
    {
      fprintf(stderr,"\nUnable to read the FONTBOUNDINGBOX.\n");
      exit(1);
    }
  readline(fp, buf);
  sscanf(buf,"%s",word1);
  while (strcmp(word1,"CHARS") != 0)
    {
      fgets(buf,MAXLINE,fp);
      sscanf(buf,"%s",word1);
    }
  if (sscanf(buf,"%s %d",word1, &no_chars) != 2)
    {
      fprintf(stderr,"\nUnable to read the number of characters.\n");
      exit(1);
    }
  for (char_nr = 1 ; char_nr <= no_chars ; char_nr ++) /* reading the chars */
    {
      readline(fp, buf);      /* reads STARTCHAR */
      if(sscanf(buf,"%s %s",word1,name) !=2 || strcmp(word1,"STARTCHAR") != 0)
	{
	  fprintf(stderr,"\nUnable to read character info %d.\n",char_nr);
	  exit(1);
	}
      readline(fp, buf);      /* reads ENCODING */
      if(sscanf(buf,"%s %d",word1,&code_nr) !=2 
	 || strcmp(word1,"ENCODING") != 0)
	{
	  fprintf(stderr,"\nUnable to read character number %d.\n",char_nr);
	  exit(1);
	}
      if (code_nr == -1)
	{
	  code_nr = Irregularnumber;
	  Irregularnumber++;
	}
      sprintf(buf,"#CHAR %s %d", name, code_nr); 
      Iappend_line(img,buf);     /* Appends character info to the text-data. */
      img[code_nr] = Imake_band(pt, xsize, ysize);
      for (n = 1; n <= ysize; n++)
	for (m = 1; m <= xsize; m++)
	  img[code_nr][n][m] = BACKGROUND;
      readline(fp, buf);         /* reads SWIDTH */
      
      readline(fp, buf);         /* reads DWIDTH */
      if ((sscanf(buf, "%s %d %d", word1, &Xmag, &Ymag) != 3) 
	  || (strcmp(word1,"DWIDTH") != 0))
	{
	  fprintf(stderr,"\nUnable to read DWIDTH number %d.\n",char_nr);
	  exit(1);
	}
      Iset_mag(img[code_nr], Xmag, Ymag);

      readline(fp, buf);         /* reads BBX */
      if (sscanf(buf, "%s %d %d %d %d",word1,&BBw,&BBh,&BBox,&BBoy) != 5 
	  || strcmp(word1,"BBX") != 0)   
	{
	  fprintf(stderr,"\nUnable to read BBX number %d.\n",char_nr);
	  exit(1);
	}  
      readline(fp, buf);          /* reads BITMAP */
      
      xstart = BBox - xorigo +1; 
      ystart = ysize + yorigo - BBh - BBoy;
      for (char_line = 1; char_line <= BBh; char_line ++)
	{
	  readline(fp, buf);                 /* reads hexadecimal code  */
	  bin = hexa2bin(buf);                /* converts to binary code  */
	  for (n = 0; n < BBw; n++)
	    {
	      c = bin[n];
	      if (c == '1') 
		img[code_nr][ystart + char_line][xstart + n] = FOREGROUND;
	    }
	}
      readline(fp, buf); /* reads ENDCHAR */
    }          
  for (n=1; n<=255; n++)              /* Make the bands which are not */
    if (img[n]==NULL)                 /* defined in the .bdf file     */
      {
	img[n] = Imake_band(pt,1,1); 
	Iset_mag(img[n], 65535, 65535);       /* xmag and ymag equals 65535 */
      }
  fclose(fp);
  return(img);
}
