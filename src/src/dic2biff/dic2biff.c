/*C*

___________________________________________________________________

        acr2biff
        $Id$
	Copyright 1996, Lars Ersland Haukeland University Hospital                
___________________________________________________________________
  

*/
/*P:acr2biff*

________________________________________________________________

		acr2biff
________________________________________________________________

Name:		acr2biff -  Converts ACR-NEMA 2.0 files to biff-format
Syntax:		
  acr2biff [-options] <infile> <outfile>
 | <infile>      - ACR-NEMA 2.0 SPI image  (- = stdin)
 | <outfile>     - BIFF image file (- = stdout)
Options:
 | No options in this version.

Description: Reads the ACR NEMA SPI input file and converts it to biff format
Developed for SIEMENS MR-images (Impact, Numaris 1.5, 2.1A). Also tested on
Philips ACR NEMA 1.0 (SPI) images, but the SPI part is only implemented
for SIEMENS. (which means that the SPI-information is not converted to biff-
format in Philips-images).


Diagnostics:
Restrictions:
Return value:

Author:		| Lars Ersland
		| Department of Clinical Engineering
		| Haukeland University Hospital
		| N-5021 Bergen
		|
        	| (modified extensively from the original version by
                | Arvid Lundervold
                | Section for Medical Image Analysis and Pattern Recognition
                | Department og Physiology
                | University of Bergen
                | N-5009 Bergen, NORWAY )
                |
                | Version 27.12.2001 on SGI IRIX 6.5 (Lars)
Project:	
Examples: %dic2biff file.acr file.biff
	  %dic2biff file.acr | xshow
	  
Files: acr2biff.c read_acr.c read_spi.c wspibiff.c wacrbiff.c acrspi.def
biff.h blab.h readarg.h Makefile libxite.a
Bugs: A lot..	

*/

/* EXTERN defined as extern in all other modules. */
#define EXTERN
#define VERSION  20011227
#define int32 long int
#define int16 int

static char * cvs_id="$Id$";

/*Include Files:*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <xite/biff.h>
#include <xite/readarg.h>
#include "acrspi.def"
/*(Non-global) External Fuctions:*/

/*(Non-global) External Variables*/
static void usage(char *s)
{
  fprintf(stderr, "\nUsage: %s [-options] <infile> <outfile> \
\n\n <infile>          - ACR NEMA 2.0 image  \
\n <outfile>          - biff image  \
\n\n Options:\
\n None available in this version. \
\n\n", s);

  exit(0);
}


  int read_group0008(long int bc);
  int read_group0010(long int bc);
  int read_group0018(long int bc);
  int read_group0020(long int bc);
  int read_group0028(long int bc);

  int read_group0009(long int bc);
  int read_group0011(long int bc);
  int read_group0019(long int bc);
  int read_group0021(long int bc);
  int read_group0029(long int bc);
  int read_group0051(long int bc);

  int read_image(long int bc, char title[80]);
  /* read pixel data */
  /*int read_group7e..(long int bc.......)
  */

  int write_group0010(long int bc);
  int write_group0018(long int bc);
  int write_group0020(long int bc);
  int write_group0028(long int bc);

  int write_group0009(long int bc);
  int write_group0011(long int bc);
  int write_group0019(long int bc);
  int write_group0021(long int bc);
  int write_group0029(long int bc);
  int write_group0051(long int bc);

  int printt(long int length, char text[80]);


  int get_acrnema_header(char title[80], char biffile[80])
  {

  long              bc, bc1, bc_dicom;            /* Byte counter */
  int               group_no;
  int               element_no;
  long              length;
  long int          s;
  long int          val;
  long int          rest;
  long int          i,j,k,x,y;
  unsigned char     *byteStream;
  int               byteStreamLen;
  int               nofDispLines;  /* Number of lines in 'hex dump' */
  int               msb_gr;        /* Most significant byte in group number */
  int               lsb_gr;        /* Least significant byte in group number */
  int               msb_el;        /* Most significant byte in element number */
  int               lsb_el;        /* Least significant byte in element number */
  int               b[16];         /* Sequence of 16 bytes */
  float             l_mantissa,l_exp;
  double            l_x;
  int               l_nbyte,l_i;
  int               l_bytes[32];
  float             l_2pover;
  char              bin_str_0[1 + 8 * sizeof(char)];
  char              bin_str_1[1 + 8 * sizeof(char)];
  char              bin_str_2[1 + 8 * sizeof(char)];
  char              bin_str_3[1 + 8 * sizeof(char)];
  /* lars */
    /* int  W, H; */
    int c256, lsb_var, msb_var, no_columns, no_rows;
    long int l, W, H, p;
    size_t dim;
    int bc_group;
    char image_nr;

   

    /* testing for a fixed file size: up to 1024 matrix */
    W = 3000;
    H = 3000;
    c256 = 256;
    dim = W * H * 2;
    /* printf("dim= %ld %ld %d\n", W, H, dim);*/
    
    /* error checking */
    if ((ptr = fopen(title,"rb")) == NULL )
     {
     printf("dic2biff: Can't open file: %s\n", title);
     exit(0);
     }
    fclose(ptr); 
    ptr= fopen(title,"rb");


    buffer1 = (unsigned char*) calloc(dim, 1);
    buffer2 = (unsigned char*) calloc(dim, 1);

    fread (buffer1, 1, dim, ptr);

   /* lars end */



  /* Checking the file type, DICOM, DICOM(ctn), ACR-NEMA(ver. 1.0 or 2.0) 
  */
  

  /* Checking for DICOM file */
  bc1=0;
  bc=0;
  for (i = 0; i <= 500; i++) {

     if ((buffer1[bc1] == 0x44) && (buffer1[bc1+1] == 0x49) && (buffer1[bc1+2] == 0x43)) {

	bc_dicom = bc1;
	bc=338; 
	printf("Oops a real DICOM file.... %0x (bc_dicom=%d)\n", buffer1[bc1], bc_dicom);
        }
     bc1 = bc1+1;  
  }
  
  /* Checking for DICOM (ctn) or ACR-NEMA file */


  /* checking if the first group is group 08 or 03 */
  if ((buffer1[bc] == 0x08) && (buffer1[bc+1] == 0x00))
     printf("DICOM (ctn) file.... %0x (bc=%d)\n", buffer1[bc], bc);
  else if ((buffer1[bc] == 0x03) && (buffer1[bc+1] == 0x00))
     printf("DICOM (ctn) file.... %0x (bc=%d)\n", buffer1[bc], bc);
  else exit(0);

	

  /*   SKRIVE UT ALLE GRUPPER */

    bc_group = 0;
    /*bc=0; */
    for (i = 0; i <= 700; i++)
  {
    /*printf("HEX-values: 2%x\n",buffer1[i]);*/
    /*printout all group numbers */

  lsb_gr = buffer1[bc];
  bc++;
  msb_gr = buffer1[bc];
  bc++;

  group_no = c256 * msb_gr + lsb_gr;


  lsb_gr = buffer1[bc];
  bc++;
  msb_gr = buffer1[bc];
  bc++;

  element_no = c256 * msb_gr + lsb_gr;


  b[1] = buffer1[bc];
  bc++;
  b[2] = buffer1[bc];
  bc++;
  b[3] = buffer1[bc];
  bc++;
  b[4] = buffer1[bc];
  bc++;

  length = c256 * c256 * c256 * b[4] + c256 * c256 * b[3] + c256 * b[2] + b[1];

#ifdef DEBUG
  fprintf(stderr,"%04x %04x %6ld %6d %6d\n", group_no, element_no, length, group_no, element_no);
#endif

  /* case strukture to print out all information */
  /* case (group_no) of:
     0008: print_group0008();
     0010: print_group0010();
     0011: print_group0011();
     .
     .
     
     */

  switch (group_no) {
      /* NEMA-groups: */
    case (8) : bc_group = read_group0008(bc);
#ifdef DEBUG
fprintf(stderr," ... OK ...\n");
#endif
      break;

    case (16): bc_group = read_group0010(bc);
      break;
		  
    case (24): bc_group = read_group0018(bc);
      break;

    case (32): bc_group = read_group0020(bc);
      break;

    case (40): bc_group = read_group0028(bc);
      break;
		  /* Shadow groups SPI, CMS */
    case (9):  bc_group = read_group0009(bc);
	 break;
    case (17): bc_group = read_group0011(bc);
	 break;
    case (25): bc_group = read_group0019(bc);
	 break;
    case (33): bc_group = read_group0021(bc);
	 break;
    case (41): bc_group = read_group0029(bc);
	 break;
    case (81): bc_group = read_group0051(bc);
         break;
      
		  /* Image data:   */
    case (32736): bc_group = read_image(bc, title);
      break;


     default:  printf("Unknown group: %04x\n", group_no);
       break;  

  }



    for (j = 1; j <= length; j++)
      {
	bc++;
      } 

    if ((group_no == 32736) && (element_no == 16))
      {
	/*printf("Pixeldata er nådd ved byte nr: %ld\n",bc);*/
	
	return 1;
      } 

  }

#ifdef DEBUG
  fprintf(stderr,"rest: %d (bc=%d)", rest, bc);
#endif


#ifdef DEBUG
  fprintf(stderr, "\n A total of %d bytes is scanned from start of file\n",  bc);
#endif

  return 0;

}               /* end of get_acrnema_header */


int printt(long int length, char text[80])
{
   int i;
     /*printf("%ld", length); */
     for (i = 0; i < (length); i++)
     {
	printf("%c", text[i]);

     }
     printf("\n");
     return 0;
}



int main(int argc, char *argv[])
{
  int               i, print_help_info;
 /* char              title[80]; */
  char              biffile[80];
  char              biffile1[80];
  int               status;
  long int	    test;


  /*InitMessage(&argc, argv, "Usage %s [-switches] <infile> <outfile>\n"); 
  Info(0,"%s from %s Copyright 1996, Lars Ersland\n", argv[0], cvs_id); 

  Iset_message(1);   warnings from blab-software
  Iset_abort(0);     but don't abort */

#ifdef CVS
  fprintf(stderr, "%s from $Id$ Copyright 1993, Lars Ersland\n", argv[0]);
#endif


  /*sscanf(read_switch(&argc, argv, "-t", 1, "ACR-NEMA_2.0_to_BIFF"),"%s", title); 
  print_help_info = read_switch(&argc, argv, "-h", 0, NULL) != NULL; */

  /* printf("Antall Argument %d\n",argc); */

  if ((argc < 2) || (argc > 3))
    {
      (void) usage(argv[0]);
      exit(0);
    } 

  /*if (argc != 4) Skjoenner ikke hvorfor ikke denne virker, lars.
    {
      (void) usage(argv[0]);
      exit(0);
    } */


    

    /*(void) usage(argv[0]); */

  /*if (print_help_info == 1)
    (void) usage(argv[0]);*/



#ifdef DEBUG
  fprintf(stderr,"DEBUG: -h  %d\n", print_help_info);
  fprintf(stderr,"DEBUG: -t  %s\n", title);
#endif

  /* biffile = argv[2];
  printf("------ ARGV[0] ------:%s\n", argv[0]); 
  printf("------ ARGV[1] ------:%s\n", argv[1]);
  printf("------ ARGV[2] ------:%s\n", argv[2]);*/
  
  status = get_acrnema_header(argv[1], argv[2]);

/*  printf("argc: %d\n",  argc);
  for (i = 1; i < argc; i++)
      printf("%s%s", argv[i], (i < argc - 1) ? " " : "");
  printf("\n");  */
  /*printf("------finish------\n\n");*/

  sprintf(l_string,"----------------------------------------------------");
  Iappend_line((IUB_IMAGE) outimage, l_string);
  sprintf(l_string,"------------------ NEMA GROUP 0008: ----------------");
  Iappend_line((IUB_IMAGE) outimage, l_string);
  sprintf(l_string,"----------------------------------------------------");
  Iappend_line((IUB_IMAGE) outimage, l_string); 
  write_group0008();

  sprintf(l_string,"----------------------------------------------------");
  Iappend_line((IUB_IMAGE) outimage, l_string);
  sprintf(l_string,"------------------ NEMA GROUP 0010: ----------------");
  Iappend_line((IUB_IMAGE) outimage, l_string);
  sprintf(l_string,"----------------------------------------------------");
  Iappend_line((IUB_IMAGE) outimage, l_string); 
  status = write_group0010(test);
  
  sprintf(l_string,"----------------------------------------------------");
  Iappend_line((IUB_IMAGE) outimage, l_string);
  sprintf(l_string,"------------------ NEMA GROUP 0018: ----------------");
  Iappend_line((IUB_IMAGE) outimage, l_string);
  sprintf(l_string,"----------------------------------------------------");
  Iappend_line((IUB_IMAGE) outimage, l_string); 
  status = write_group0018(test);

  sprintf(l_string,"----------------------------------------------------");
  Iappend_line((IUB_IMAGE) outimage, l_string);
  sprintf(l_string,"------------------ NEMA GROUP 0020: ----------------");
  Iappend_line((IUB_IMAGE) outimage, l_string);
  sprintf(l_string,"----------------------------------------------------");
  Iappend_line((IUB_IMAGE) outimage, l_string); 
  status = write_group0020(test);

  sprintf(l_string,"----------------------------------------------------");
  Iappend_line((IUB_IMAGE) outimage, l_string);
  sprintf(l_string,"------------------ NEMA GROUP 0028: ----------------");
  Iappend_line((IUB_IMAGE) outimage, l_string);
  sprintf(l_string,"----------------------------------------------------");
  Iappend_line((IUB_IMAGE) outimage, l_string); 
  status = write_group0028(test);

  sprintf(l_string,"----------------------------------------------------");
  Iappend_line((IUB_IMAGE) outimage, l_string);
  sprintf(l_string,"------------------ SPI GROUP 0009: -----------------");
  Iappend_line((IUB_IMAGE) outimage, l_string);
  sprintf(l_string,"----------------------------------------------------");
  Iappend_line((IUB_IMAGE) outimage, l_string); 
  status = write_group0009(test);

  sprintf(l_string,"----------------------------------------------------");
  Iappend_line((IUB_IMAGE) outimage, l_string);
  sprintf(l_string,"------------------ SPI GROUP 0011: -----------------");
  Iappend_line((IUB_IMAGE) outimage, l_string);
  sprintf(l_string,"----------------------------------------------------");
  Iappend_line((IUB_IMAGE) outimage, l_string); 
  status = write_group0011(test);

  sprintf(l_string,"----------------------------------------------------");
  Iappend_line((IUB_IMAGE) outimage, l_string);
  sprintf(l_string,"------------------ SPI GROUP 0019: -----------------");
  Iappend_line((IUB_IMAGE) outimage, l_string);
  sprintf(l_string,"----------------------------------------------------");
  Iappend_line((IUB_IMAGE) outimage, l_string); 
  status = write_group0019(test);

  sprintf(l_string,"----------------------------------------------------");
  Iappend_line((IUB_IMAGE) outimage, l_string);
  sprintf(l_string,"------------------ SPI GROUP 0021: -----------------");
  Iappend_line((IUB_IMAGE) outimage, l_string);
  sprintf(l_string,"----------------------------------------------------");
  Iappend_line((IUB_IMAGE) outimage, l_string); 
  status = write_group0021(test);

  sprintf(l_string,"----------------------------------------------------");
  Iappend_line((IUB_IMAGE) outimage, l_string);
  sprintf(l_string,"------------------ SPI GROUP 0029: -----------------");
  Iappend_line((IUB_IMAGE) outimage, l_string);
  sprintf(l_string,"----------------------------------------------------");
  Iappend_line((IUB_IMAGE) outimage, l_string); 
  status = write_group0029(test);

  /*sprintf(l_string,"----------------------------------------------------");
  Iappend_line((IUB_IMAGE) outimage, l_string);
  sprintf(l_string,"------------------ SPI GROUP 0051: -----------------");
  Iappend_line((IUB_IMAGE) outimage, l_string);
  sprintf(l_string,"----------------------------------------------------");
  Iappend_line((IUB_IMAGE) outimage, l_string); 
  status = write_group0051(test);*/

       /* writing header-data to BIFF-format */

   
       /*Iwrite_image(outimage, biffile);*/
       Iwrite_image((IUB_IMAGE) outimage, argv[2]);

}               /* end of main() of main.c */

