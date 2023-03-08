
/*

________________________________________________________________

        biff.c
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

/*
------------------------------------------------------------------------

   MODULE-NAME    :  BIFF - Blab-UiO Image File Format
   PURPOSE        :  ROUTINES FOR HANDLING THE BLAB-UIO 
                     IMAGE FILE FORMAT.
   VERSION        :  2.0
   AUTHOR	  :  Tor Lønnestad
   REVISED        :  Svein Bøe
------------------------------------------------------------------------
*/

/*
------------------------------------------------------------------------
-------------------  INCLUSIONS AND DEFINITIONS  -----------------------
------------------------------------------------------------------------
*/

#ifdef HAVE_SYS_WAIT_H
# include <sys/wait.h>
#endif
#include <xite/includes.h>
#include <xite/blab.h>
#include <xite/biff.h>
#include <stdlib.h>
#include <xite/utils.h>
#include <xite/strings.h>
#include <xite/convert.h>
#include XITE_FCNTL_H
#include XITE_FILE_H
#include XITE_IO_H
#include XITE_MALLOC_H
#include XITE_MKTEMP_H
#include XITE_STAT_H
#include XITE_STDIO_H
#include XITE_STRING_H
#include XITE_TIME_H
#include XITE_TYPES_H
#include XITE_FORK_H
#include XITE_UNISTD_H
#include XITE_ENDIAN_H
#include <errno.h>
#include <xite/debug.h>

#define BLAB		1
#define BIFF		2
#ifndef MIN
# define MIN(a,b)	(((a) < (b)) ? (a) : (b))
#endif
#ifndef MAX
# define MAX(a, b)      (((a) > (b)) ? (a) : (b))
#endif
#define LF		'\012'

static BiffStatus _Imove_blks   _XITE_PARAMS(( IMAGE, long, long, long ));
static int        _Iread_       _XITE_PARAMS(( int, unsigned char *, unsigned int ));
static BiffStatus _Iread_info   _XITE_PARAMS(( IMAGE *, _Info * ));
static long       _Iread_long   _XITE_PARAMS(( unsigned char * ));
static short      _Iread_short  _XITE_PARAMS(( unsigned char * ));
static long       _Iread_val    _XITE_PARAMS(( unsigned char *, int, int, int ));
static BiffStatus _Iset_info    _XITE_PARAMS(( IMAGE ));
static int        _Iwrite_      _XITE_PARAMS(( int, unsigned char *, unsigned int ));
static BiffStatus _Iwrite_info  _XITE_PARAMS(( IMAGE ));
static void       _Iwrite_long  _XITE_PARAMS(( unsigned char *, long ));
static BiffStatus _Iwrite_short _XITE_PARAMS(( unsigned char *, int ));
static BiffStatus _Iread_bands  _XITE_PARAMS(( IMAGE img, IMAGE *new_img, int *bandarr, int *num_bands ));

/*
------------------------------------------------------------------------
------------------------  GLOBAL DATA  ---------------------------------
------------------------------------------------------------------------
*/

#ifdef DEBUG
static int Imessage = TRUE;  /* boolean, TRUE=> display error mess. */
static int Iabort = TRUE;    /* boolean, TRUE=> abort on errors     */
#else
static int Imessage = FALSE;  /* boolean, TRUE=> display error mess.*/
static int Iabort = FALSE;    /* boolean, TRUE=> abort on errors    */
#endif

static int Ioverwrite = TRUE; /* boolean, TRUE=> overwrite image files */
static unsigned char buffer[512];
static long Ilittle_endian_mask = 0x010000L;
static long Ipixtyp_mask = 0x0ffffL;
IPIXTYP Icolor_mask = 0x0100L;
#ifndef _WIN32
static char *file_spec_separator = ":";
#else /* _WIN32 */
static char *file_spec_separator = ";";
#endif /* _WIN32 */

unsigned char Ibitmask[9] = {0,1,2,4,8,16,32,64,128};
unsigned char Inegbitmask[9] = {255,254,253,251,247,239,223,191,127};

static char *pix_print_format[] = {
  NULL,      /* uns_bit */
  NULL,      /* uns_2bit */
  NULL,      /* uns_nibble */
  "%-3d",    /* uns_byte */
  "%-4d",    /* sign_byte */
  "%-5d",    /* uns_short */
  "%-6d",    /* sign_short */
/*#ifdef XITE_INT_IS_INT*/
  "%-11d",   /* integer */
/*#else*/
  /*"%-11ld",*/   /* integer */
/*#endif*/
  "%-9.3g",  /* real */
  "%-9.3g",  /* complex */
  "%-10.3g", /* double */
  "%-10.3g", /* dcomplex */
};

static char *pixtypname[] =
{
  "uns_bit",
  "uns_2bit",
  "uns_nibble",
  "uns_byte", 
  "sign_byte",
  "uns_short",
  "sign_short",
  "integer",
  "real",
  "complex",
  "double",
  "dcomplex",
  "colortable",
  "unknown",
};

/*
------------------------------------------------------------------------
-----------------------  LOWER LEVEL PROCEDURES ------------------------
------------------------------------------------------------------------
*/

static int host_little_endian(void)
{
  int one = 1;
  char* cp = (char*)&one;

  return(*cp != 0);
}

int host_byte_order(void)
{
  if (host_little_endian()) return(XITE_LITTLE_ENDIAN);
  else return(XITE_BIG_ENDIAN);
}



/*L:_Ilittle_endian*

________________________________________________________________

		_Ilittle_endian
________________________________________________________________

Name:		_Ilittle_endian
Syntax:		static int _Ilittle_endian(IBAND b);
Description:	Determine whether b is read from a file which was written from
                a computer with little-endian byte-ordering. Can not use the
                function Ipixtyp, because it only considers the 16 least
		significant bits.
Files:          <xite/biff.h>
Return value:	1 if band is read from a file which was written little-endian,
                0 otherwise.
Author:		Svein Bøe
Id: 		$Id$
________________________________________________________________

*/

static int _Ilittle_endian(IBAND b)
{
  _Iband_info *binfo;
  if (b EQ NULL) {
    Ierr_action(Ibanderr);
    return(-1);
  }
  binfo = (_Iband_info *)b[0];
  return(((int) binfo->pixtyp & Ilittle_endian_mask) != 0);
}




/*L:_Iread_short*

________________________________________________________________

		_Iread_short
________________________________________________________________

Name:		_Iread_short - Read a 16-bit unsigned integer
Syntax:		| #include <xite/biff.h>
		|
		| static short _Iread_short(buffer)
                | unsigned char *buffer;
Description:	Read a 16-bit unsigned integer, return value
                as 32 bit signed. Reads from start of 'buffer'.
Restrictions:	
Return value:	32 bit integer converted from 16-bit unsigned
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

static short _Iread_short(unsigned char *buffer)
{
  return(buffer[0]*256 + buffer[1]);
}   /*   _Iread_short   */


/*L:_Iread_long*

________________________________________________________________

		_Iread_long
________________________________________________________________

Name:		_Iread_long - Read a 32-bit signed integer
Syntax:		| #include <xite/biff.h>
		|
		| static long _Iread_long(buffer)
                | unsigned char *buffer;
Description:	Read a 32-bit signed integer from 'buffer'.
                Will read BIFF-integers correct no matter
		what internal integer representation.
Restrictions:	
Return value:	A 32 bit signed integer
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

static long _Iread_long(unsigned char *buffer)
{
  long val;
  val = ((buffer[0] BAND 127)*33554432L) + (buffer[1]*65536L)
    + (buffer[2]*256L) + buffer[3];
  return((buffer[0] BAND 128) ? (-val) : val);
}   /*   _Iread_long   */


/*L:_Iread_val*

________________________________________________________________

		_Iread_val
________________________________________________________________

Name:		_Iread_val - Read a 32-bit signed integer
Syntax:		| #include <xite/biff.h>
		|
		| static long _Iread_val(buffer, blabpos, biffpos, version)
                | unsigned char *buffer;
		| int blabpos, biffpos, version;
Description:	Read a 32-bit signed integer from 'buffer'.
                Will read BIFF integers correct no matter
		what internal integer representation, and
		no matter whether blab format or biff format.
Restrictions:	
Return value:	A 32 bit signed integer
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

static long _Iread_val(unsigned char *buffer, int blabpos, int biffpos, int version)
{
  if (version EQ BLAB) {
    return(_Iread_short(&(buffer[blabpos])));
  } else if (version EQ BIFF) {
    return(_Iread_long(&(buffer[biffpos])));
  }
  return(0);
}   /*   _Iread_val   */


/*L:_Iwrite_short*

________________________________________________________________

		_Iwrite_short
________________________________________________________________

Name:		_Iwrite_short - Write 32b signed int as 16b unsigned
Syntax:		| #include <xite/biff.h>
		|
		| static BiffStatus _Iwrite_short(buffer, val)
                | unsigned char *buffer;
		| int val;
Description:	Write 32 bit signed integer as 16 bit unsigned
                integer into 'buffer'
Restrictions:	0 <= val <= 65535 
Return value:	status   
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

static BiffStatus _Iwrite_short(unsigned char *buffer, int val)
{
  if ((val GT 65535) OR (val LT 0)) return(Iwriterr);
  buffer[0] = (val / 256);
  buffer[1] = (val MOD 256);
  return(Iok);
}   /*   _Iwrite_short   */


/*L:_Iwrite_long*

________________________________________________________________

		_Iwrite_long
________________________________________________________________

Name:		_Iwrite_long - Write 32b signed int with msb first 
Syntax:		| #include <xite/biff.h>
		|
		| static void _Iwrite_long(buffer,val)
                | unsigned char *buffer;
		| int val;
Description:	The 32 bit signed integer <val> is written into
                'buffer' with the most significant byte first.
Restrictions:	
Return value:	
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

static void _Iwrite_long(unsigned char *buffer, long int val)
{
  int neg;
  neg = (val < 0);
  val = (neg) ? -val : val;
  buffer[0] = (val / 33554432L);
  if (neg) buffer[0] += 128;
  buffer[1] = ((val / 65536L) MOD 256);
  buffer[2] = ((val / 256L) MOD 256);
  buffer[3] = (val MOD 256);
}   /*   _Iwrite_long   */




/*L:_Imove_blks*

________________________________________________________________

		_Imove_blks
________________________________________________________________

Name:		_Imove_blks - Move contigous blocks (512 B) on file
Syntax:		| #include <xite/biff.h>
		|
		| static BiffStatus _Imove_blks(i, from, to, n)
                | IMAGE i;
		| long from, to, n;
Description:	<n> blocks is read from the open file connected to
                image <i> and written back into another position,
		starting with reading at <from>, writing at <to>.
		Blocks are read and written one by one, overlapping 
		areas may be critical.
Restrictions:	Image file open with write permisson
Return value:	status
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

static BiffStatus _Imove_blks(IMAGE i, long int from, long int to, long int n)
{
  _Info *info;
  unsigned char s[512];
  long j,dist;
  info = (_Info *)i[0];
  if ((info->status EQ Ireadonly) OR (info->status EQ Ilocal))
    return(Istatuserr);
  dist = (to-from)*512;
  FOR (j=from+n-1; j GE from; DEC j)
    if (lseek(info->fd,j*512,L_SET) NE j*512) return(Iwriterr);
    if (_Iread_(info->fd,s,512) NE 512) return(Iwriterr);
    if (lseek(info->fd,dist,L_INCR) EQ -1) return(Iwriterr);
    if (_Iwrite_(info->fd,s,512) NE 512) return(Iwriterr);
  ENDFOR;
  return(Iok);
}   /*   _Imove_blks   */




/*L:_Iread_*

________________________________________________________________

		_Iread_
________________________________________________________________

Name:		_Iread_ - read data from channel fd 
Syntax:		| #include <xite/biff.h>
		|
		| static int _Iread_(fd, ptr, size)
                | int fd;
                | unsigned char *ptr;
		| unsigned int size;
Description:	Read data from channel 'fd'. The channel may be a
                pipe. 'size' bytes are read into the buffer indicated
		by 'ptr'. This routine is called by all read routines.
Restrictions:	fd refers open file 
Return value:	Number of bytes actually read.
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

static int _Iread_(int fd, unsigned char *ptr, unsigned int size)
{
  int blokk, total = 0;

  ENTER_FUNCTION_DEBUG("biff.c: _Iread");

  FPRINTF2("    size: %d\n", size);

  errno = 0;
  LOOP
    blokk = read(fd, ptr, size);
    if (blokk == -1) FPRINTF2("    Error in read(): %s\n", strerror(errno));

    FPRINTF4("    in loop, read %d, first: %d, last: %d\n",
	     blokk, ptr[0], ptr[blokk-1]);
    EXITIF (blokk LE 0);

    ptr    = (unsigned char*) (ptr+blokk);
    size  -= blokk;
    total += blokk;
    EXITIF(size EQ 0)
  ENDLOOP

  LEAVE_FUNCTION_DEBUG("biff.c: _Iread");

  return(total);
}
 

/*L:_Iwrite_*

________________________________________________________________

		_Iwrite_
________________________________________________________________

Name:		_Iwrite_ - write data to channel fd
Syntax:		| #include <xite/biff.h>
		|
		| static int _Iwrite_(fd, ptr, size)
                | int fd;
		| unsigned char *ptr;
		| unsigned int size;
Description:	Write data to channel <fd>. The channel may be a pipe.
                Attempts to write <size> bytes from <ptr> to the
		channel. This routine is called by all write routines.
Restrictions:	fd refers open file 
Return value:	Number of bytes actually written. 
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________
*/

static int _Iwrite_(int fd, unsigned char *ptr, unsigned int size)
{
  int blokk, total = 0;

  ENTER_FUNCTION_DEBUG("biff.c: _Iwrite");

  FPRINTF2("    size: %d\n",size);

  errno = 0;
  LOOP
    blokk = write(fd, ptr, size);

    if (blokk == -1) FPRINTF2("    Error in write(): %s\n", strerror(errno));
    FPRINTF4("    in loop, wrote %d, first: %d, last: %d\n",
             blokk, ptr[0], ptr[blokk-1]);

    EXITIF (blokk LE 0);

    ptr    = (unsigned char*) (ptr+blokk);
    size  -= blokk;
    total += blokk;
    EXITIF(size EQ 0)
  ENDLOOP

  LEAVE_FUNCTION_DEBUG("biff.c: _Iwrite");

  return(total);
}
 


/*L:_Iread_info*

________________________________________________________________

		_Iread_info
________________________________________________________________

Name:		_Iread_info - initiate info field from image file
Syntax:		| #include <xite/biff.h>
		|
		| static BiffStatus _Iread_info(i,info)
                | IMAGE *i;
		| _Info *info;

Description:	Initiate image info field with data from file.
                Called by Iopen_image when image is opened
                with readonly or readwrite access.

Restrictions:	<i> refers open image
Return value:	status 
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

static BiffStatus _Iread_info(IMAGE *i, _Info *info)
{
  int bandnr, j, version;
  long  nfreechars, count, rest;
  long xsz, ysz, xst, yst, xmg, ymg, pt, col;

  ENTER_FUNCTION_DEBUG("biff.c: _Iread_info");

  /* check if BLAB-UIO or BIFF-UIO file */
  count = _Iread_(info->fd,buffer,96);
  if (count NE 96) {
    if (strlen(info->filename)) close(info->fd);
    LEAVE_FUNCTION_DEBUG("biff.c: _Iread_info");
    return(Ireaderr);
  }
  if (strncmp((char *) buffer,"BLAB-",5) EQ 0)
    version = BLAB; /* old file format */
  else if (strncmp((char *) buffer,"BIFF-",5) EQ 0) version = BIFF;
  else {
    if (strlen(info->filename)) close(info->fd);
    LEAVE_FUNCTION_DEBUG("biff.c: _Iread_info");
    return(Ibifferr);
  }

  col = (buffer[5] EQ 'C');
  /* read number of blocks in the four parts */
  info->ninfoblks = _Iread_val(buffer,8,8,version);
  info->nbandblks = _Iread_val(buffer,10,12,version);
  info->ntextblks = _Iread_val(buffer,12,16,version);
  info->nblocks   = _Iread_val(buffer,14,20,version);
  /* read title and parameters */
  if (version EQ BLAB) strncpy(info->title,(char *) &buffer[16],32);
  else if (version EQ BIFF) strncpy(info->title,(char *) &buffer[24],32);

  info->title[32] = (char)0;
  for (j=0; j LT 8; j++)
    info->param[j] = _Iread_val(buffer,48,56,version);
  /* read free characters, calculate existing */
  nfreechars = _Iread_val(buffer,80,88,version);
  info->nchars = (info->ntextblks * 512) - nfreechars;
  /* initiate character posistion to one */
  info->pos = 1;
  info->text = NULL;
  info->textbufblks = 0;
  /* read number of bands */
  info->nbands = _Iread_val(buffer,82,92,version);
  /* malloc space for IMAGE */
  *i = (IMAGE)malloc((info->nbands+1)*sizeof(char*));
  if (*i EQ NULL) {
    LEAVE_FUNCTION_DEBUG("biff.c: _Iread_info");
    return(Ierr_action(Imallocerr));
  }
  (*i)[0] = (IBAND)info;
  /* read data concerning each band */
  FOR (bandnr = 1; bandnr LE info->nbands; ++ bandnr)
    count = _Iread_(info->fd,buffer,32);
    if (count NE 32) {
      if (strlen(info->filename)) close(info->fd);
      LEAVE_FUNCTION_DEBUG("biff.c: _Iread_info");
      return(Ireaderr);
    }
    pt  = _Iread_val(buffer,0,0,version);
    xsz = _Iread_val(buffer,2,4,version);
    ysz = _Iread_val(buffer,4,8,version);
    xst = _Iread_val(buffer,6,12,version);
    yst = _Iread_val(buffer,8,16,version);
    xmg = _Iread_val(buffer,10,20,version);
    ymg = _Iread_val(buffer,12,24,version);
    (*i)[bandnr] = Init_band(pt | (col * Icolor_mask), xsz, ysz);
    if ((*i)[bandnr] EQ NULL) {
      LEAVE_FUNCTION_DEBUG("biff.c: _Iread_info");
      return(Ierr_action(Imallocerr));
    }
    Iset_start((*i)[bandnr],xst,yst);
    Iset_mag((*i)[bandnr],xmg,ymg);
  ENDFOR;
  rest = 0 - 96 - info->nbands*32;
  while (rest LT 0) rest += 512;
  if (rest) {
    count = _Iread_(info->fd,buffer,rest);
    if (count NE rest) {
      if (strlen(info->filename)) close(info->fd); 
      LEAVE_FUNCTION_DEBUG("biff.c: _Iread_info");
      return(Ireaderr);
    }
  }

  LEAVE_FUNCTION_DEBUG("biff.c: _Iread_info");

  return(Iok);
}   /*   _Iread_info   */

static long _num_blocks_band(IBAND band)
{
  long size;

  size = (Ibandsize(band) + 511) / 512;

  return(size);

} /* _num_blocks_band() */

/*L:_Iset_info*

________________________________________________________________

		_Iset_info
________________________________________________________________

Name:		_Iset_info - 
Syntax:		| #include <xite/biff.h>
		|
		| static BiffStatus _Iset_info(i)
                | IMAGE i;
Description:	Set standard values in image opened with write
                access. Allocate disk space for image (with lseek)
		unless writing to a pipe.
                Called by Iopen_image when Iwrite access. 
Restrictions:	i refers to newly created/opened image file
Return value:	status  
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

static BiffStatus _Iset_info(IMAGE i)
{
  _Info *info;
  long bandnr, size;

  ENTER_FUNCTION_DEBUG("biff.c: _Iset_info");

  info = (_Info*)i[0];

  /* 13 bands in first block, 16 in later ones */
  info->ninfoblks = (((info->nbands + 2) / 16) + 1);
  size = 0;
  FOR (bandnr=1; bandnr LE info->nbands; ++ bandnr)
    size += _num_blocks_band(i[bandnr]);
  ENDFOR;
  info->nbandblks = size;
  info->ntextblks = info->textbufblks;

  /* Write at end of file to allow reading anywhere. */
  size = (info->ninfoblks + info->nbandblks)*512;
  if (NOT info->maybe_pipe)
    if (lseek(info->fd,size,L_SET) NE size) {
      LEAVE_FUNCTION_DEBUG("biff.c: _Iset_info");
      return(Iwriterr);
    }

  LEAVE_FUNCTION_DEBUG("biff.c: _Iset_info");

  return(Iok);
}   /*   _Iset_info   */    



/*L:_Iwrite_info*

________________________________________________________________

		_Iwrite_info
________________________________________________________________

Name:		_Iwrite_info - write info to file
Syntax:		| #include <xite/biff.h>
		|
		| static BiffStatus _Iwrite_info(i)
		| IMAGE i;

Description:	Write contents of image info in 'i' to file. Also writes
                info for all bands.
                Called by Iclose_image when readwrite or write access. 

Restrictions:	Info refers to open file with write permission
Return value:	status
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

static BiffStatus _Iwrite_info(IMAGE i)
{
  _Info *info;
  int j;
  long bandnr, count, rest;

  ENTER_FUNCTION_DEBUG("biff.c: _Iwrite_info");

  info = (_Info *)i[0];

  if (NOT info->maybe_pipe) lseek(info->fd, (long)0, L_SET);
  strncpy((char *) &buffer[0],"BIFF-UIO",8);
  _Iwrite_long(&buffer[8],info->ninfoblks);
  _Iwrite_long(&buffer[12],info->nbandblks);
  _Iwrite_long(&buffer[16],info->ntextblks);
  _Iwrite_long(&buffer[20],info->nblocks);
  strncpy((char *) &buffer[24],info->title,32);
  for (j=0; j LT 8; j++)
    _Iwrite_long(&buffer[56+4*j],info->param[j]);
  _Iwrite_long(&buffer[88],(info->ntextblks*512)-info->nchars);
  _Iwrite_long(&buffer[92],info->nbands);

  if (info->nbands GT 0)
    if (Ipixtyp(i[1]) GE Icolor_mask) strncpy((char *) &buffer[5],"COL",3);

  count = _Iwrite_(info->fd,buffer,96);
  if (count NE 96) {
    if (strlen(info->filename)) close(info->fd);
    LEAVE_FUNCTION_DEBUG("biff.c: _Iwrite_info");
    return(Iwriterr);
  }

  for (bandnr=1; bandnr LE info->nbands; ++ bandnr) {
    if ( (Ipixsize(Ipixtyp(i[bandnr]))/8 > 1) &&
	(host_byte_order() == XITE_LITTLE_ENDIAN))
      _Iwrite_long(&buffer[0], Ipixtyp(i[bandnr]) | Ilittle_endian_mask);
    else
      _Iwrite_long(&buffer[0], Ipixtyp(i[bandnr]));

    _Iwrite_long(&buffer[4],Ixsize(i[bandnr]));
    _Iwrite_long(&buffer[8],Iysize(i[bandnr]));
    _Iwrite_long(&buffer[12],Ixstart(i[bandnr]));
    _Iwrite_long(&buffer[16],Iystart(i[bandnr]));
    _Iwrite_long(&buffer[20],Ixmag(i[bandnr]));
    _Iwrite_long(&buffer[24],Iymag(i[bandnr]));
    _Iwrite_long(&buffer[28],0L);
    count = _Iwrite_(info->fd,buffer,32);
    if (count NE 32) {
      if (strlen(info->filename)) close(info->fd);
      LEAVE_FUNCTION_DEBUG("biff.c: _Iwrite_info");
      return(Iwriterr);
    }
  }
  rest = 0 - 96 - info->nbands*32;
  while (rest LT 0) rest += 512;
  if (rest) {
    count = _Iwrite_(info->fd,&(buffer[512-rest]),rest);
    if (count NE rest) {
      if (strlen(info->filename)) close(info->fd);
      LEAVE_FUNCTION_DEBUG("biff.c: _Iwrite_info");
      return(Iwriterr);
    }
  }

  LEAVE_FUNCTION_DEBUG("biff.c: _Iwrite_info");

  return(Iok);

}   /*   _Iwrite_info   */

/*
------------------------------------------------------------------------
--------------------  IMAGE HANDLING PROCEDURES   ----------------------
------------------------------------------------------------------------
*/

/*D:Iparse_filename=Iparse_band_spec*/
/*D:Ifilename_part=Iparse_band_spec*/
/*D:Iparse_band_spec*

________________________________________________________________

		Iparse_band_spec
________________________________________________________________

Name:		Iparse_band_spec, Iparse_filename, Ifilename_part -
                interpret/parse file name and band specification

Syntax:		| #include <xite/biff.h>
		| 
		| BiffStatus Iparse_band_spec( char *spec,
		|    int **bandarray, int *num_bands );
		|
		| BiffStatus Iparse_filename( char **filename,
		|    int **bandarray, int *num_bands );
		|
		| char *Ifilename_part( char *filename );
		|
Description:	Interpret/parse 'filename' or band specification string 'spec'.

                A file name can consist of two parts, the name of the file and
		an appended band specification. A simple band specification is
		either a band number or two band numbers separated by a "-".
		The latter kind stands for all bands in the interval,
		including the two given ones. A band specification may contain
		multiple simple band specifications, separated by ":" (under
		UNIX) or ";" (under Windows NT).

		'Iparse_band_spec' interprets the band specification in 'spec'.
		'bandarray' will return an array which holds the band numbers
		in the order specified in 'spec', starting with element
		bandarray[1]. The required memory space for 'bandarray' will
		be allocated.

		'num_bands' returns the number of bands specified, i.e.
		bandarray[num_bands] is the last valid entry. If '*num_bands'
		is non-zero on input, the band numbers will be stored in
		'bandarray' starting with bandarray[*num_bands + 1].
		This makes it convenient to call the function repeatedly
		for different 'spec' and have all the band numbers stored
		in the same 'bandarray', like this:

		| char **specs;
		| int i, num_specs, num = 0, *bands;
		| < Let 'specs' refer to 'num_specs' strings >
		| for (i = 1; i <= num_specs; i++) {
		|   status = Iparse_band_spec(specs[i], &bands, &num);
		| }

		'Ifilename_part' returns the filename part of 'filename'.
		Memory will be allocated if 'filename' is not NULL or zero
		length.

		'Iparse_filename' returns only the file name part in
		'filename', while 'bandarray' and 'num_bands' return the
		parsed band specification.

		&Examples:

		| 1            (meaning only band 1)
		| 3            (meaning only band 3)
		| 1-3          (meaning bands 1,2,3)
		| 3-1          (meaning bands 3,2,1)
		| 2:3-1:3      (meaning bands 2,3,2,1,3) (Unix)
		| 2;3-1;3      (meaning bands 2,3,2,1,3) (Windows NT)
		| file:2:3-1:3 (meaning bands 2,3,2,1,3 of file) (Unix)
		| file;2;3-1;3 (meaning bands 2,3,2,1,3 of file) (NT)

Return value:	&Iparse_band_spec
                |  0          : Ok
                | Ibandspecerr: Illegal band specification (see biff.h)
		| Imallocerr  : Memory allocation error

		&Iparse_filename
		| 0     : Ok
		| Other : Error in parsing band specification
		
See also:	Iread_image(5)
Author:		Svein Bøe and Otto Milvang
Id: 		$Id$
________________________________________________________________

*/

BiffStatus Iparse_band_spec(char *spec, int **bandarray, int *num_bands)
{
  int bn, bn1, bn2;
  size_t bandarrsize = 0;
  int last = 0;

  /* Macro for allocation of extra pointers to band numbers.
   * Increase by one hundred pointers at a time.
   */
#define INSERT(array, array_size, last, bn)			          \
  {  if (++(last) >= (array_size)) {				          \
        (array_size) += 100;					          \
        if ((array_size) == 100)                                          \
          (array) = (int *) malloc((array_size) * sizeof(int));	          \
        else                                                              \
          (array) = (int *) realloc((array), (array_size) * sizeof(int)); \
        if (!(size_t) (array)) {                                          \
          *num_bands = last;                                              \
          return(Ierr_action(Imallocerr));                                \
        }                                                                 \
      }							                  \
      (array)[last] = bn;						  \
  }

  if (*num_bands > 0 && bandarray != NULL && (*bandarray) != NULL) {
    /* Append to an already allocated array. */
    last = *num_bands;
    bandarrsize = ((last - 1) / 100 + 1) * 100;
  }

  spec = strtok(spec, file_spec_separator);

  while (spec) {
    if (strchr(spec, '-')) {
      if (sscanf(spec, "%d-%d%*s", &bn1, &bn2) == 2) {
        if (bn1 <= bn2) {
          for (bn = bn1; bn <= bn2; ++ bn)
            INSERT(*bandarray, bandarrsize, last, bn);
	} else {
          for (bn = bn1; bn >= bn2; -- bn)
            INSERT(*bandarray, bandarrsize, last, bn);
	}
      } else {
	*num_bands = last;
	return(Ierr_action(Ibandspecerr));
      }
    } else {
      if (sscanf(spec, "%d%*s", &bn1) == 1) {
        INSERT(*bandarray, bandarrsize, last, bn1);
      } else {
	*num_bands = last;
	return(Ierr_action(Ibandspecerr));
      }
    }

    spec = strtok(NULL, file_spec_separator);
  } /* while */

  *num_bands = last;

#undef INSERT

  return(Iok);

} /* Iparse_band_spec() */

char *Ifilename_part(char *filename)
{
  char *fname;

  if (!filename || strlen(filename) == 0) return(filename);

  fname = strcpy((char *) malloc(strlen(filename) + 1), filename);
  if (!fname) {
    Ierr_action(Imallocerr);
    return(filename);
  }

  /* Insert a '\0' for the first ':'. */
  fname = strtok(fname, file_spec_separator);

  return(fname);
  
} /* Ifilename_part() */

BiffStatus Iparse_filename(char **filename, int **bandarray, int *num_bands)
{
  char *fname = NULL;
  BiffStatus status = Iok;

  if (!filename) return(status);

  fname = Ifilename_part(*filename);

  if (fname == *(filename)) return(status);

  if ((status = Iparse_band_spec(NULL, bandarray, num_bands)) != Iok) {
    return(status);
  }

  *filename = fname;

  return(status);

} /* Iparse_filename() */

/*D:Iread_image_bands=Iread_image*/
/*D:Iread_image*

________________________________________________________________

		Iread_image
________________________________________________________________

Name:		Iread_image, Iread_image_bands - read (parts of ) a BIFF
                image from file

Syntax:         | #include <xite/biff.h>
		|
                | IMAGE Iread_image( char* filename );
		| IMAGE Iread_image_bands( char *filename,
		|    int *bandarr, int num_bands );

Description:	'Iread_image' reads an entire image, i.e. all the image bands
                and the text data (currently not block data), or a subset
		of bands and the text data, from file into an IMAGE data
		structure. The file is opened, read and closed.

		If 'filename' starts with "~" (tilde), it will be expanded
		to the appropriate user''s home directory before searching for
		the file, using the routine 'tilde_expand'.

		If 'filename' is "-channel", and 'channel' is the integer
		number of an open input or output channel, the image will be
		read from this channel. Use "-0", "-" or empty file name to
		read from stdin.

		The file name may be followed by one or more band
		specifications. A band specification is either a band number
		or two band numbers separated by a "-". The latter kind stands
		for all bands in the interval, including the two given ones.
		Multiple band specifications are separated by ":" (under UNIX)
		or ";" (under Windows NT), and there is a ":" (or ";") between
		the file name and the first band specification. The bands can
		be specified in any order, even when reading from a pipe.

		The band specification is parsed by Iparse_band_spec(5), which
		converts from a textual band specification to a list of
		bands.

		&Examples:

		| mona.img            (meaning all bands)
		| mona.img:1          (meaning only band 1)
		| mona.img:3          (meaning only band 3)
		| mona.img:1-3        (meaning bands 1,2,3)
		| mona.img:3-1        (meaning bands 3,2,1)
		| mona.img:2:3-1:3    (meaning bands 2,3,2,1,3)

		-&
		'Iread_image' uses the amount of memory needed to hold the
		requested bands and text data. If only a subset of the image
		bands are read from a pipe, additional memory for dumping the
		other bands is temporarily allocated (the size of this area
		equals the size of the largest unspecified band).

		'Iread_image_bands' works like 'Iread_image'. It is simply an
		alternative interface for specifying a subset of bands. It
		reads the bands listed in the 'bandarr' array, starting with
		'bandarr'[1]. The number of bands listed is given by
		'num_bands', i.e. the last band number is found in
		'bandarr'['num_bands'].

Restrictions:	The file should exist and have read access.

See also:       Iread_band(5), Iwrite_image(5), Iopen_image(5),
                Iclose_image(5), Init_image(5), Iparse_band_spec(5),
                tilde_expand(3)

Return value:	The image if ok, otherwise NULL.
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

IMAGE Iread_image_bands(char *filename, int *bandarr, int num_bands)
{
  BiffStatus status;
  IMAGE img, new_img = NULL;
  int bn1, img_nbands;

  ENTER_FUNCTION_DEBUG("biff.c: Iread_image_bands");

  FPRINTF2("    filename: %s\n", filename);

  status = Iopen_image(&img, filename, Ireadonly);
  if (status != Iok) {
    LEAVE_FUNCTION_DEBUG("biff.c: Iread_image_bands");
    return(NULL);
  }

  img_nbands = Inbands(img);
  status = _Iread_bands(img, &new_img, bandarr, &num_bands);
  if (status != Iok) {
    Iclose_image(img);
    LEAVE_FUNCTION_DEBUG("biff.c: Iread_image_bands");
    return(NULL);
  }

  if ((status = Iread_text(img)) != Iok) {
    Iclose_image(img);
    LEAVE_FUNCTION_DEBUG("biff.c: Iread_image_bands");
    return(NULL);
  }
  if ((status = Iclose_image(img)) != Iok) {
    LEAVE_FUNCTION_DEBUG("biff.c: Iread_image_bands");
    return(NULL);
  }

  /* Some bands of img may not have been read into a data structure. */
  for (bn1 = 1; bn1 LE Inbands(img); ++ bn1)
    if (Iband_status(img[bn1]) EQ Iabsent) Idel_band(&(img[bn1]));

  /* Resize img. If num_bands equals Inbands(img), nothing happens. Otherwise,
   * img will either get additional NULL band pointers or the extraneous
   * band pointers will be invalid. */

  if ((status = Iset_nbands(&img, num_bands)) != Iok) {
    LEAVE_FUNCTION_DEBUG("biff.c: Iread_image_bands");
    return(NULL);
  }

  if (new_img == NULL) {
    /* img is already correct. */

    LEAVE_FUNCTION_DEBUG("biff.c: Iread_image_bands");

    return(img);
  }

  /* More bands in new_img than in img. Hook these on to img. */

  for (bn1 = MIN(num_bands, img_nbands) + 1; bn1 <= Inbands(img); bn1++) {
    img[bn1] = new_img[bn1];
  }

  LEAVE_FUNCTION_DEBUG("biff.c: Iread_image_bands");

  return(img);

} /* Iread_image_bands() */

IMAGE Iread_image(char *filename)
{
  IMAGE img;
  int last = 0, *bandarr = NULL;
  BiffStatus status;

  ENTER_FUNCTION_DEBUG("biff.c: Iread_image");

#ifndef MSDOS
  if (filename) {
    status = Iparse_filename(&filename, &bandarr, &last);
    if (status != Iok) {
      LEAVE_FUNCTION_DEBUG("biff.c: Iread_image");
      return(NULL);
    }
  }
#endif

  img = Iread_image_bands(filename, bandarr, last);

  LEAVE_FUNCTION_DEBUG("biff.c: Iread_image");

  return(img);

} /* Iread_image() */

/*D:Iwrite_image*

________________________________________________________________

		Iwrite_image
________________________________________________________________

Name:		Iwrite_image - write a BIFF image to file

Syntax:         | #include <xite/biff.h>
		|
                | BiffStatus Iwrite_image( IMAGE i,
                |    char* filename );

Description:	Write an entire image 'i' to file (currently not
                block data). If the overwrite status is TRUE,
		an existing file of that name is overwritten.
		If the overwrite status is FALSE and the file
		exists, the function returns with an error indication.

		If 'filename' starts with "~" (tilde), it will be expanded
		to the appropriate user home directory before searching for
		the file, using the routine tilde_expand.

		If 'filename' is "-channel", and 'channel' is the integer
		number of an open input or output channel, the image will be
		written to this channel. Use "-1", "-" or empty file name to
		write to stdout.

Restrictions:	Directory must be writeable. If overwrite=FALSE,
                the file must not exist. The image must be in Ilocal status.

See also:       Iread_image(5), Iwrite_band(5), Iopen_image(5),
                Iclose_image(5), Init_image(5), tilde_expand(3)

Return value:	status.
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

BiffStatus Iwrite_image(IMAGE i, char *filename)
{
  int bandnr;
  BiffStatus status;
  _Info *info;

  ENTER_FUNCTION_DEBUG("biff.c: Iwrite_image");

  FPRINTF2("    filename: %s\n",filename);

  status = Iopen_image(&i,filename,Iwrite); /* Also checks Ilocal status. */
  if (status NE Iok) {
    LEAVE_FUNCTION_DEBUG("biff.c: Iwrite_image");
    return(status);
  }

  info = (_Info *)i[0];

  for (bandnr=1; bandnr LE Inbands(i); ++ bandnr) {
    if ((Iband_status(i[bandnr]) EQ Inormal)
      OR (Iband_status(i[bandnr]) EQ Isubband)) {
      status = Iwrite_band(i,bandnr);
      if (status NE Iok) {
	Iclose_image(i);
	LEAVE_FUNCTION_DEBUG("biff.c: Iwrite_image");
	return(status);
      }
    }
  }
  status = Iwrite_text(i);
  if (status NE Iok) {
    Iclose_image(i);
    LEAVE_FUNCTION_DEBUG("biff.c: Iwrite_image");
    return(status);
  }
  status = Iclose_image(i);

  LEAVE_FUNCTION_DEBUG("biff.c: Iwrite_image");

  return(status);

} /* Iwrite_image() */

/*D:Iopen_image*

________________________________________________________________

		Iopen_image
________________________________________________________________

Name:		Iopen_image - open a BIFF-file

Syntax:         | #include <xite/biff.h>
		|
                | BiffStatus Iopen_image( IMAGE* i,
                |    char* filename, int acc );

Description:	Open the file 'filename' with 'acc' access, and
                connect it to image 'i'.

		'acc' is one of: Ireadonly, Ireadwrite, Iwrite.

		If 'filename' starts with "~" (tilde), it will be expanded
		to the appropriate user home directory before searching for
		the file, using the routine tilde_expand.

		If 'filename' is empty, stdin is used for readonly access,
		stdout is used for write access.

		If 'filename' is "-channel", and 'channel' is the integer
		number of an open input or output channel, this channel is
		used. Use "-0", "-" or empty file name for stdin on readonly
		access, "-1", "-" or empty file name for stdout on write
		access.

Restrictions:	When Iwrite: Info must be initiated properly, the file
                should not exist when overwrite is FALSE, and the image
		must have status Ilocal.

                When readonly or readwrite: The file should exist,
		it should adhere to BIFF format, and should allow the
		specified access mode.

See also:       Iclose_image(5), Iread_image(5), Iwrite_image(5),
                tilde_expand(3)
Return value:	status
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

static int is_channel_name(char *filename)
{
  if (!filename ||
      strlen(filename) == 0 ||
      (*filename == '-' &&
       (filename[1] == '\0' ||
	(filename[1] >= '0' && filename[1] <= '9'))))
    return(1);
  else return(0);

} /* is_channel_name() */

BiffStatus Iopen_image(IMAGE *i, char *filename, int acc)
{
  _Info *info;
  int flags, mode = 0, operation, last = 0, *bandarr = NULL;
  BiffStatus status;
  char endmark = '\0', *fname = NULL;

  ENTER_FUNCTION_DEBUG("biff.c: Iopen_image");

  if (filename EQ NULL) filename = &endmark;
  fname = filename;

  FPRINTF2("    filename: %s\n", filename);

#ifndef MSDOS
  /* Allow file names with band specifications appended. Simply ignore the
   * band specification at this stage. Use a file name copy, so as not to
   * change the contents of the original, which may be used again later. */

  fname = Ifilename_part(filename);
  if (!fname) {
    LEAVE_FUNCTION_DEBUG("biff.c: Iopen_image");
    return(Ierr_action(Imallocerr));
  }
#endif

  if ((acc EQ Ireadonly) OR (acc EQ Ireadwrite))
    info = (_Info *)malloc(sizeof(_Info));
  else {
    info = (_Info *)(*i)[0];
    if (info->status != Ilocal) {
      LEAVE_FUNCTION_DEBUG("biff.c: Iopen_image");
      return (Ierr_action(Istatuserr));
    }
  }
  
  switch (acc) {
  case Ireadonly:
    flags     = O_RDONLY BOR FNDELAY BOR O_BINARY;
    operation = LOCK_SH BOR LOCK_NB;       /* shared lock */
    info->fd  = (((*fname) EQ '-') ? (-atoi(fname)) : fileno(stdin));
    break;
  case Ireadwrite:
    flags     = O_RDWR BOR FNDELAY BOR O_BINARY;
    operation = LOCK_EX BOR LOCK_NB;       /* exclusive lock */
    break;
  case Iwrite:
    if (Ioverwrite) flags = O_RDWR BOR O_CREAT BOR O_TRUNC BOR O_BINARY;
    else            flags = O_RDWR BOR O_CREAT BOR O_EXCL  BOR O_BINARY;

    operation = LOCK_EX BOR LOCK_NB;       /* exclusive lock */
    info->fd  = *fname EQ '-' ? -atoi(fname) : fileno(stdout);
    if (info->fd == 0) info->fd = fileno(stdout);  /* Takes care of "-". */
    mode = 0666;                           /* protection mode */
    break;
  }

  info->filename = (char *) malloc(strlen(fname) + 1);

  if (is_channel_name(fname)) {
    /* Can not do lseek() or other arbitrary positioning. Image data runs
     * through a pipe. */
    info->maybe_pipe  = 1;
    *(info->filename) = '\0';
    
# ifdef HAVE_SETMODE
    errno  = 0;
    status = setmode(info->fd, O_BINARY);
    if (status == -1)
      FPRINTF2("    Error in setmode(): %s\n", strerror(errno));
    
    FPRINTF2("    old pipe status: %s\n",
	     status == O_BINARY ? "O_BINARY" : "O_TEXT");
# endif /* HAVE_SETMODE */
  } else {
    char *fname_exp;

    (void) strcpy(info->filename, fname);
    info->maybe_pipe = 0;
    fname_exp        = tilde_expand(fname);
    info->fd         = open(fname_exp, flags, mode);

    if (fname_exp != fname) free(fname_exp);
    if (info->fd EQ -1) {
      LEAVE_FUNCTION_DEBUG("biff.c: Iopen_image");
      return(Ierr_action(Iopenerr));
    }
    if (flock(info->fd,operation) NE 0 ) {
      close(info->fd);
      LEAVE_FUNCTION_DEBUG("biff.c: Iopen_image");
      return(Ierr_action(Ilockerr));
    }
  }

  if (fname != filename) free(fname);

  if (acc EQ Iwrite) {
    if ((status =_Iset_info(*i))) {
      LEAVE_FUNCTION_DEBUG("biff.c: Iopen_image");
      return(Ierr_action(status));
    }
    if (_Iwrite_info(*i)) {
      LEAVE_FUNCTION_DEBUG("biff.c: Iopen_image");
      return(Ierr_action(status));
    }
  } else {
    if ((status = _Iread_info(i,info))) {
      LEAVE_FUNCTION_DEBUG("biff.c: Iopen_image");
      return(Ierr_action(status));
    }
  }
  info->status = acc;

  LEAVE_FUNCTION_DEBUG("biff.c: Iopen_image");

  return(Iok);

}  /* Iopen_image */

/*D:Iclose_image*

________________________________________________________________

		Iclose_image
________________________________________________________________

Name:		Iclose_image - close a BIFF image file

Syntax:         | #include <xite/biff.h>
		|
                | BiffStatus Iclose_image( IMAGE i );

Description:	Close the image file connected to 'i'. Update image and
                band info on file if readwrite or write access.

		Image status is set to Ilocal.

Restrictions:	'i' refers to an open file.

See also:       Iopen_image(5), Iread_image(5), Iwrite_image(5)

Return value:	status
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

BiffStatus Iclose_image(IMAGE i)
{
  _Info *info;

  ENTER_FUNCTION_DEBUG("biff.c: Iclose_image");

  info = (_Info *)i[0];
  if ((info->status NE Ireadonly) AND NOT info->maybe_pipe) _Iwrite_info(i);

  info->status = Ilocal;
  if (strlen(info->filename) GT 0)
    if (close(info->fd) NE 0) {
      LEAVE_FUNCTION_DEBUG("biff.c: Iclose_image");
      return(Ierr_action(Icloserr));
    }

  free(info->filename);
  info->filename   = NULL;
  info->maybe_pipe = 0;

  LEAVE_FUNCTION_DEBUG("biff.c: Iclose_image");

  return(Iok);

} /* Iclose_image() */

/*D:Imake_image*

________________________________________________________________

		Imake_image
________________________________________________________________

Name:		Imake_image - Create the whole BIFF image data structure

Syntax:         | #include <xite/biff.h>
		|
                | IMAGE Imake_image( int nbands, char* title,
                |    IPIXTYP pt, long xsize, long ysize );

Description:	Create the whole BIFF image data structure, with 'nbands'
                bands, 'title' as title, every band of horizontal
		size 'xsize', vertical size 'ysize', and pixel type
		'pixtyp'.

		NOTE: The pixel values will not be initialized.

Restrictions:	Existing pixel type, 'nbands', 'xsize' and 'ysize' GE 1.    

See also:       Init_image(5), Icopy_init(5), Imake_band(5), Imake_bands(5),
                Init_band(5), Init_bands(5)

Return value:	The new image if ok, NULL otherwise.
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/
IMAGE Imake_image(int nbands, char *title, IPIXTYP pt, long int xsize, long int ysize)
{
  IMAGE ii;
  BiffStatus status;

  ENTER_FUNCTION_DEBUG("biff.c: Imake_image");

  FPRINTF2("    nbands: %d\n", nbands);

  ii = Init_image(nbands, title);
  if (ii EQ NULL) {
    LEAVE_FUNCTION_DEBUG("biff.c: Imake_image");
    return(NULL);
  }

  status = Imake_bands(ii,pt,xsize,ysize);
  if (status NE Iok) {
    LEAVE_FUNCTION_DEBUG("biff.c: Imake_image");
    return(NULL);
  }

  LEAVE_FUNCTION_DEBUG("biff.c: Imake_image");

  return(ii);

}   /*   Imake_image   */

/*D:Istatus*

________________________________________________________________

		Istatus
________________________________________________________________

Name:		Istatus - return the BIFF image status

Syntax:         | #include <xite/biff.h>
		|
                | long Istatus( IMAGE i );

Description:	Return the status of the image 'i', one of
                Ireadonly, Ireadwrite, Iwrite and Ilocal.

		If an image is not attached to a file or a channel, it has
		status Ilocal.

Restrictions:	'i' must be initialized

See also:       Iopen_image(5), Iclose_image(5), Iread_image(5),
                Iwrite_image(5), Iread_text(5), Iwrite_text(5), Iread_block(5),
		Iwrite_block(5)

Return value:	The status value
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

long Istatus(IMAGE i)
{
  _Info *info;
  info = (_Info *)i[0];
  return(info->status);
}   /*   Istatus   */

/*D:Inbands*

________________________________________________________________

		Inbands
________________________________________________________________

Name:		Inbands - return the number of bands in the BIFF image

Syntax:         | #include <xite/biff.h>
		|
                | long Inbands( IMAGE i );

Description:	Return the number of bands in the image 'i'.

Restrictions:	'i' must be initialized.

See also:       Imake_image(5), Imake_bands(5), biffinfo(1)

Return value:	Number of bands
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

long Inbands(IMAGE i)
{
  _Info *info;
  info = (_Info *)i[0];
  return(info->nbands);
}   /*   Inbands   */

/*D:Inchars*

________________________________________________________________

		Inchars
________________________________________________________________

Name:		Inchars - Report number of characters in text field

Syntax:         | #include <xite/biff.h>
		|
                | long Inchars( IMAGE i );

Description:	Return the number of characters in the text field of
                image 'i'.

Restrictions:	'i' must be initialized.

See also:       Iread_text(5), Iwrite_text(5), Icopy_text(5), Iput_text(5),
                Iget_line(5), Iend_of_text(5), Iappend_line(5), Itype_text(5),
		Iedit_text(5), Idel_text(5), Igetpos(5), Isetpos(5)

Return value:	Number of characters.
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

long Inchars(IMAGE i)
{
  _Info *info;
  info = (_Info *)i[0];
  return(info->nchars);
}   /*   Inchars   */

/*D:Ititle*

________________________________________________________________

		Ititle
________________________________________________________________

Name:		Ititle - Return the title of the BIFF image

Syntax:         | #include <xite/biff.h>
		|
                | char *Ititle( IMAGE i );

Description:	Return  a pointer to a malloc''ed(3) copy of the 
                title of the image 'i'. The receiver may free(3)
		the copy at any time.

Restrictions:	'i' must be initialized.

See also:       Iset_title(5), Icopy_init(5), biffinfo(1), newtitle(1)

Return value:	The character pointer.
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

char* Ititle(IMAGE i)
{
  char *s;
  _Info *info;
  info = (_Info *)i[0];
  s = malloc(33);
  strcpy(s, info->title);
  return(s);
}   /*   Ititle   */

/*D:Iset_title*

________________________________________________________________

		Iset_title
________________________________________________________________

Name:		Iset_title - assign a new title to the BIFF image

Syntax:         | #include <xite/biff.h>
		|
                | BiffStatus Iset_title( IMAGE i, char* title );

Description:	Assign a new title to the image 'i'. 

Restrictions:	Long titles are truncated to 31 characters.

See also:       Ititle(5), Icopy_init(5), newtitle(1)
Return value:	status
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

BiffStatus Iset_title(IMAGE i, char *title)
{
  _Info *info;
  info = (_Info *)i[0];
  strncpy(info->title,title,32);
  info->title[31] = (char)0;
  return(Iok);
}   /*   Iset_title   */

/*D:Iset_nbands*

________________________________________________________________

		Iset_nbands
________________________________________________________________

Name:		Iset_nbands - change number of bands in the BIFF image

Syntax:         | #include <xite/biff.h>
		|
                | BiffStatus Iset_nbands( IMAGE* i, int nbands );

Description:	Change the length of the band pointer in image 'i'
                to 'nbands'. If it used to be longer, the last bands
		will be lost (unless other images have pointers to
		them).

Restrictions:	Image must have status Ilocal.

See also:       Idel_band(5)
Return value:	status
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

BiffStatus Iset_nbands(IMAGE *i, int nbands)
{
  _Info *info;
  int bn;

  if (*i EQ NULL) return(Ierr_action(Imageerr));
  if (nbands LT 0) return(Ierr_action(Ibandnrerr));
  if (Istatus(*i) != Ilocal) return(Ierr_action(Istatuserr));

  if (nbands == Inbands(*i)) return(Iok);

  *i = (IMAGE) realloc(*i, (nbands + 1) * sizeof(char *));
  if (*i EQ NULL) return(Ierr_action(Imallocerr));

  info = (_Info *)(*i)[0];
  for (bn = info->nbands + 1; bn LE nbands; ++ bn)
    (*i)[bn] = NULL;

  info->nbands = nbands;

  return(Iok);
}

/*D:Iget_param*

________________________________________________________________

		Iget_param
________________________________________________________________

Name:		Iget_param - get a parameter from the BIFF image

Syntax:         | #include <xite/biff.h>
		|
                | BiffStatus Iget_param( IMAGE i, int parmnr,
                |    long* parmval );

Description:	Return parameter number 'paramnr' through the
                argument 'parmval'. These eight parameters may
		contain any value, to be interpreted as any type.
		The user/application is free to deside.

Restrictions:	1 <= parmnr <= 8

See also:       Iset_param(5)
Return value:	status
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

BiffStatus Iget_param(IMAGE i, int parmnr, long int *parmval)
{
  _Info *info;
  if (i EQ NULL) return(Ierr_action(Imageerr));
  if ((parmnr LT 1) OR (parmnr GT 8))
    return(Ierr_action(Iparnrerr));
  info = (_Info *)i[0];
  *parmval = info->param[parmnr];
  return(Iok);
}   /*   Iget_param   */

/*D:Iset_param*

________________________________________________________________

		Iset_param
________________________________________________________________

Name:		Iset_param - assign new value to BIFF image parameter

Syntax:         | #include <xite/biff.h>
		|
                | BiffStatus Iset_param( IMAGE i, int parmnr,
                |    long parmval );

Description:	Assign the parameter value 'parmval' to parameter
                number 'parmnr' in the image 'i'.

Restrictions:	1 <= parmnr <= 8

See also:       Iget_param(5)
Return value:	status
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

BiffStatus Iset_param(IMAGE i, int parmnr, long int parmval)
{
  _Info *info;
  info = (_Info *)i[0];
  if ((parmnr LT 1) OR (parmnr GT 8))
    return(Ierr_action(Iparnrerr));
  info->param[parmnr] = parmval;
  return(Iok);
}   /*   Iset_param   */

/*D:Init_image*

________________________________________________________________

		Init_image
________________________________________________________________

Name:		Init_image - initialize a BIFF image

Syntax:         | #include <xite/biff.h>
		|
                | IMAGE Init_image( int nbands, char* title );

Description:	Initialize a BIFF image; allocate the band pointer and
                image info record, and assign values to the
		"nbands" and "title" fields. Bands must also be
		created before you may use the image.

		The status of the image will be Ilocal.

Restrictions:	1 <= nbands

See also:       Icopy_init(5), Imake_image(5), Iread_image(5), Iwrite_image(5),
                Init_band(5), Init_bands(5)
Return value:	image pointer if ok, NULL otherwise.
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

IMAGE Init_image(int nbands, char *title)
{
  IMAGE i;
  _Info *info;
  int n, j;

  ENTER_FUNCTION_DEBUG("biff.c: Init_image");

  FPRINTF2("    bands: %d\n", nbands);

  if (nbands LT 0) {
    Ierr_action(Ibandnrerr);
    LEAVE_FUNCTION_DEBUG("biff.c: Init_image");
    return(NULL);
  }
  i = (IMAGE)malloc((nbands+1)*sizeof(IBAND));
  if (i EQ NULL) {
    Ierr_action(Imallocerr);
    LEAVE_FUNCTION_DEBUG("biff.c: Init_image");
    return(NULL);
  }
  info = (_Info *)malloc(sizeof(_Info));
  if (info EQ NULL) {
    Ierr_action(Imallocerr);
    LEAVE_FUNCTION_DEBUG("biff.c: Init_image");
    return(NULL);
  }
  i[0] = (IBAND)info;
  info->nbands = nbands;
  strncpy(info->title,title,32);
  info->title[32] = (char)0;
  info->text = NULL;
  info->filename = NULL;
  info->nchars = 0;
  info->pos = 1;
  for (j = 0; j < 8; j++)
    info->param[j] = 0;
  info->textbufblks = 0;
  info->nblocks = 0;
  info->status = Ilocal;
  for (n=1;n LE nbands; ++ n)
    i[n] = (IBAND)NULL;

  LEAVE_FUNCTION_DEBUG("biff.c: Init_image");

  return(i);

}   /*   Init_image   */

/*D:Init_band*

________________________________________________________________

		Init_band
________________________________________________________________

Name:		Init_band - initiate a BIFF band

Syntax:         | #include <xite/biff.h>
		|
                | IBAND Init_band( IPIXTYP pt, long xsize,
                |    long ysize );

Description:	Create an "absent" band. Storage for pixels will
                not be allocated, but the band info field will
		be allocated and initialized with pixel type
		'pt', horizontal size 'xsize', and vertical size
		'ysize'.

		The status of the band will be Iabsent.

Restrictions:	Existing pixel type, 'xsize' and 'ysize' GE 1.

See also:       Init_bands(5), Init_image(5), Imake_band(5), Imake_bands(5),
                Iread_band(5), Iwrite_band(5)
Return value:	Band pointer if ok, NULL otherwise.
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

IBAND Init_band(IPIXTYP pt, long int xsize, long int ysize)
{
  _Iband_info *binfo;

  ENTER_FUNCTION_DEBUG("biff.c: Init_band");

  if (Ipixsize(pt) EQ -1) {
    Ierr_action(Ipixtyperr);
    LEAVE_FUNCTION_DEBUG("biff.c: Init_band");
    return(NULL);
  }
  binfo = (_Iband_info *)malloc(sizeof(_Iband_info));
  if (binfo EQ NULL) {
    Ierr_action(Imallocerr);
    LEAVE_FUNCTION_DEBUG("biff.c: Init_band");
    return(NULL);
  }
  if ((xsize LT 0) OR (ysize LT 0)) {
    Ierr_action(Isizerr);
    LEAVE_FUNCTION_DEBUG("biff.c: Init_band");
    return(NULL);
  }

  binfo->status = Iabsent;
  binfo->xsize = xsize;
  binfo->ysize = ysize;
  binfo->xstart = 1;
  binfo->ystart = 1;
  binfo->xmag = 1;
  binfo->ymag = 1;
  binfo->pixtyp = pt; /* Also stores info about endian-ness and colortable */
  binfo->roi_xsize = xsize;
  binfo->roi_ysize = ysize;
  binfo->roi_xstart = 1;
  binfo->roi_ystart = 1;
  binfo->this_struct = (long *)binfo;
  binfo = (_Iband_info *) &(binfo->this_struct);

  LEAVE_FUNCTION_DEBUG("biff.c: Init_band");

  return((IBAND)binfo);

}   /* Init_band */

/*D:Init_bands*

________________________________________________________________

		Init_bands
________________________________________________________________

Name:		Init_bands - initialize all bands of a BIFF image

Syntax:         | #include <xite/biff.h>
		|
                | BiffStatus Init_bands( IMAGE i, IPIXTYP pt,
                |    long xsize, long ysize );

Description:	Initialize all bands of image 'i' by calling
                Init_band(pt, xsize, ysiz) for every band. Storage
		for pixels is not allocated.

Restrictions:	Init_image should have been called previously to
                allocate band pointers and image info record.
		pt, xsize and ysize requirements are the same as for Init_band.

See also:       Init_band(5), Imake_bands(5), Imake_band(5), Init_image(5),
                Iread_band(5), Iwrite_band(5)
Return value:	status
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

BiffStatus Init_bands(IMAGE i, IPIXTYP pt, long int xsize, long int ysize)
{
  int bandnr = 1;

  ENTER_FUNCTION_DEBUG("biff.c: Init_bands");

  LOOP
    EXITIF (bandnr GT Inbands(i));
    i[bandnr] = Init_band(pt, xsize, ysize);
    EXITIF (i[bandnr] EQ NULL);
    ++ bandnr;
  ENDLOOP;

  if (bandnr GT Inbands(i)) {
    LEAVE_FUNCTION_DEBUG("biff.c: Init_bands");
    return(Iok);
  } else {
    LEAVE_FUNCTION_DEBUG("biff.c: Init_bands");
    return(Imallocerr);
  }
}   /* Init_bands */

/*D:Icopy_init*

________________________________________________________________

		Icopy_init
________________________________________________________________

Name:		Icopy_init - Create BIFF image with equal size as old

Syntax:         | #include <xite/biff.h>
		|
                | IMAGE Icopy_init( IMAGE i );

Description:	Initialize a new image and make all bands with the
                same sizes, startpoints, magnifications and pixel
		types as an old image 'i'.
		The old text field is copied into the new image.

Restrictions:	'i' must be initialized.

See also:       Init_image(5), Imake_image(5), Imake_band(5), Ixsize(5),
                Iysize(5), Ixstart(5), Iystart(5), Iset_start(5), Ixmag(5),
		Iymag(5), Iset_mag(5), Ipixtyp(5), Icopy_text(5)

Return value:	Pointer to the new image if ok, NULL otherwise.
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

IMAGE Icopy_init(IMAGE i)
{
  IMAGE newi;
  int bn, n;

  ENTER_FUNCTION_DEBUG("biff.c: Icopy_init");

  n    = Inbands(i);
  newi = Init_image(n,Ititle(i));
  if (newi EQ NULL) {
    LEAVE_FUNCTION_DEBUG("biff.c: Icopy_init");
    return(NULL);
  }

  FOR (bn=1; bn LE n; bn++)
    newi[bn] = Imake_band(Ipixtyp(i[bn]), Ixsize(i[bn]), Iysize(i[bn]));
    if (newi[bn] EQ NULL) {
      LEAVE_FUNCTION_DEBUG("biff.c: Icopy_init");
      return(NULL);
    }

    Iset_start(newi[bn], Ixstart(i[bn]), Iystart(i[bn]));
    Iset_mag(newi[bn], Ixmag(i[bn]), Iymag(i[bn]));
  ENDFOR;

  Icopy_text(i, newi);

  LEAVE_FUNCTION_DEBUG("biff.c: Icopy_init");

  return(newi);

}   /*   Icopy_init   */

/*D:Idel_image*

________________________________________________________________

		Idel_image
________________________________________________________________

Name:		Idel_image - delete a BIFF image

Syntax:         | #include <xite/biff.h>
		|
                | void Idel_image( IMAGE* ip );

Description:	Delete a BIFF image, deallocate virtual memory, using
		free(3). Image pointer is set to NULL.

Restrictions:   If image does not have status Ilocal, the attached file
                is closed before deallocation.

See also:       Idel_band(5), Idel_band_pix(5)
Return value:	
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

void Idel_image(IMAGE *ip)
{
  int bn;

  ENTER_FUNCTION_DEBUG("biff.c: Idel_image");

  if ((*ip) NE NULL) {
    if (Istatus(*ip) NE Ilocal) Iclose_image(*ip);

    /* Free bands */
    for (bn=1; bn LE Inbands(*ip); ++ bn)
      Idel_band(&((*ip)[bn]));

    /* Free info */
    if (((_Info *) ((*ip)[0]))->text)
      free((((_Info *) ((*ip)[0]))->text) + 1);
    free((*ip)[0]);

    /* Free band pointers */
    free(*ip);

    *ip = NULL;
  }

  LEAVE_FUNCTION_DEBUG("biff.c: Idel_image");
}


/*------------------------------------------------------------------------
  =================   BAND HANDLING PROCEDURES   =====================
  ------------------------------------------------------------------------*/


/*D:Imake_band*

________________________________________________________________

		Imake_band
________________________________________________________________

Name:		Imake_band - create a new BIFF band

Syntax:         | #include <xite/biff.h>
		|
                | IBAND Imake_band( IPIXTYP pt, long xsize,
                |    long ysize );

Description:	Allocate (using malloc(3)) virtual memory for a
		2D band, initialize pointers, and return pointer
		to the band. The band will have pixel type 'pt',
		horizontal size 'xsize' and vertical size 'ysize'.
		The first band pixel is aligned on a boundary for DCOMPLEX
		datatype.

		The band will get status Inormal.

		NOTE: The pixel values will not be initialized.

Restrictions:	Legal pixel type, xsize and ysize GE 1.

See also:       Init_band(5), Imake_bands(5), Imake_subband(5), Init_image(5),
                Icopy_init(5), Imake_image(5), Iread_band(5)

Return value:	Pointer to the new band if ok, NULL otherwise.
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

IBAND Imake_band(IPIXTYP pt, long int xsize, long int ysize)
{
  IBAND band1,band2;
  int line,linesize,pixelsize,pointarrsize,maxpsize;
  unsigned size;

  ENTER_FUNCTION_DEBUG("biff.c: Imake_band");

  band1 = Init_band(pt,xsize,ysize);
  if (band1 EQ NULL) {
    LEAVE_FUNCTION_DEBUG("biff.c: Imake_band");
    return(NULL);
  }

  pixelsize    = Ipixsize(pt)/8;
  linesize     = xsize * pixelsize;
  pointarrsize = (ysize+1)*sizeof(char*); /* One extra for the info pointer. */

#if defined(MSDOS)
  /* Allocate pointer array and each line separately. */

  linesize += pixelsize;
  band2 = (IBAND)malloc(pointarrsize); /* Pointer array. */
  if (band2 EQ NULL) {
    Ierr_action(Imallocerr);
    LEAVE_FUNCTION_DEBUG("biff.c: Imake_band");
    return(NULL);
  }
  band2[0] = band1[0]; /* Copy the info pointer. */
  for (line=1; line LE ysize; line++) {
    band2[line] = malloc(linesize);    /* Each line. */
    if (band2[line] EQ NULL) {
      Ierr_action(Imallocerr);
      LEAVE_FUNCTION_DEBUG("biff.c: Imake_band");
      return(NULL);
    }
  }
#else
  /* Allocate pointer array and all pixels in one chunk. */
  size = pointarrsize + (ysize*linesize);

  /* The addition of maxpsize (sizeof(DCOMPLEX)-1, typically 16) here and */
  /* below, combined with rounding modulo maxpsize ensures that the bands */
  /* are aligned on the largest alignment needed */

  maxpsize = sizeof(DCOMPLEX);
  band2 = (IBAND)malloc(size+maxpsize-1); /* Pointer array and all pixels. */
  if (band2 EQ NULL) {
    Ierr_action(Imallocerr);
    LEAVE_FUNCTION_DEBUG("biff.c: Imake_band");
    return(NULL);
  }
  /* initialize pointer array. The band must be indexed from [1,1] */
  band2[0] = band1[0]; /* Copy the info pointer. */

  /* Set all line pointers.
   * Cast pointer band2 to long (required on Dec ALPHA, 64 bit arch.)
   */
  band2[1] = (UNS_BYTE*)
    (( ( (long)band2 + pointarrsize + maxpsize - 1) / maxpsize) * maxpsize
    - pixelsize);
  for (line = 2; line LE ysize; ++ line)
    band2[line] = band2[line-1] + linesize;
#endif

  ((_Iband_info *)band2[0])->status = Inormal;

  LEAVE_FUNCTION_DEBUG("biff.c: Imake_band");

  return(band2);

}   /*   Imake_band   */

/*D:Imake_bands*

________________________________________________________________

		Imake_bands
________________________________________________________________

Name:		Imake_bands - create all bands of a BIFF image

Syntax:         | #include <xite/biff.h>
		|
                | BiffStatus Imake_bands( IMAGE i, IPIXTYP pt,
                |    long xsize, long ysize );

Description:	Allocate memory (malloc(3)) and build the data
		structure for all bands of image 'i', by calling
		Imake_band repeatedly. All bands will have pixel
		type 'pt', horizontal size 'xsize' and vertical
		size 'ysize'. The image must first have been
		initialized by means of Init_image.

		NOTE: The pixel values will not be initialized.

Restrictions:	Init_image must have been called previously, 'pt'
		must be legal pixel type, 'xsize' and 'ysize' GE 1.

See also:       Imake_band(5), Init_band(5), Imake_subband(5), Init_image(5),
                Imake_image(5), Iread_band(5), Icopy_init(5)

Return value:	status
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

BiffStatus Imake_bands(IMAGE i, IPIXTYP pt, long int xsize, long int ysize)
{
  int n;

  ENTER_FUNCTION_DEBUG("biff.c: Imake_bands");

  FOR (n=1; n LE Inbands(i); ++ n)
    i[n] = Imake_band(pt, xsize, ysize);
    if (i[n] EQ NULL) {
      LEAVE_FUNCTION_DEBUG("biff.c: Imake_bands");
      return(Ierr_action(Ibanderr));
    }
  ENDFOR;

  LEAVE_FUNCTION_DEBUG("biff.c: Imake_bands");

  return(Iok);

}   /*   Imake_bands   */

/*D:Imake_subband*

________________________________________________________________

		Imake_subband
________________________________________________________________

Name:		Imake_subband - define subband of existing BIFF band

Syntax:         | #include <xite/biff.h>
		|
                | IBAND Imake_subband( IBAND band, long xstart,
                |    long ystart, long xsize, long ysize );

Description:	Creates a subband of 'band'; a data structure with pointers
		into the old band. The new band will have horizontal
		size 'xsize' and vertical size 'ysize', and indexing
		[1][1] in the subband is equivalent to indexing
		['ystart']['xstart'] in the old band. The same pixels
		are referenced, data is not copied!

		The band will get status Isubband.

Restrictions:	'band' must be created, the whole subband must be
		within the old.

See also:       Init_band(5), Imake_band(5), Iset_start(5), Ixstart(5),
                Iystart(5), Iset_mag(5), Ixmag(5), Iymag(5), subcopy(1)

Return value:	pointer to the new band if ok, NULL otherwise.
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

IBAND Imake_subband(IBAND band, long int xstart, long int ystart, long int xsize, long int ysize)
{
  IPIXTYP pt;
  long line, pixelsize;
  _Iband_info *binfo;
  IBAND band1, band2;
  unsigned long size;

  if ((xstart LT 1) OR (ystart LT 1)
    OR (xsize LT 1) OR (ysize LT 1)
    OR (xstart+xsize-1 GT Ixsize(band))
    OR (ystart+ysize-1 GT Iysize(band))) {
    Ierr_action(Isizerr); return(NULL);
  }
  pt        = Ipixtyp(band);
  pixelsize = Ipixsize(pt)/8;

  band1 = Init_band(pt,xsize,ysize);
  if (band1 EQ NULL) { Ierr_action(Imallocerr); return(NULL); }

  size  = (ysize+1)*sizeof(char *); /* One extra for the info pointer. */
  band2 = (IBAND)malloc(size);
  if (band2 EQ NULL) { Ierr_action(Imallocerr); return(NULL); }

  /* initialize pointer array. The band must be indexed from [1,1] */
  band2[0] = band1[0]; /* Copy the info pointer. */
  for (line = 1; line LE ysize; ++ line)
    band2[line] = &(band[ystart+line-1][(xstart-1)*pixelsize]);

  binfo         = (_Iband_info *)band2[0];
  binfo->status = Isubband;

  Iset_start(band2, Ixstart(band)+xstart-1, Iystart(band)+ystart-1);
  Iset_mag(band2, Ixmag(band), Iymag(band));

  return(band2);
}   /*   Imake_subband   */


/*D:Idel_band*

________________________________________________________________

		Idel_band
________________________________________________________________

Name:		Idel_band - delete a BIFF band

Syntax:         | #include <xite/biff.h>
		|
                | void Idel_band( IBAND* band );

Description:	Deallocate virtual memory for a band, using free(3).
                Band pointer is set to NULL. If this is a subband (status
		Isubband), the pixels will not be deallocated.

		Band pointer is set to NULL.

		Be careful with deleting a band and trying to read a band with
		higher band number using the Iread_band routine. The
		Iread_band routine uses info data from previous bands to
		calculate the location of a band. These info data will be
		invalid after a call to Idel_band. To save memory, call
		Idel_band_pix instead. This will only deallocate the pixel
		storage.

See also:       Idel_image(5), Idel_band_pix(5), Imake_subband(5),
                Iband_status(5)

Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

void Idel_band(IBAND *band)
{
  long stat;
#if defined(MSDOS)
  long y, ysize;
#endif

  ENTER_FUNCTION_DEBUG("biff.c: Idel_band");

  IF (*band NE NULL)
    stat = Iband_status(*band);
    IF (stat EQ Inormal)
#if defined(MSDOS)
      /* Each line was allocated separately. */
      ysize = Iysize(*band);
      for (y=1; y<=ysize; y++)
        free((*band)[y]);
#endif
      free((*band)[0]); /* The info data. */

      /* The rest of the band, i.e. pointer array and pixels (the latter
       * only if not MSDOS). */
      free(*band);      
    ELSEIF (stat EQ Isubband)
      free((*band)[0]); /* The info data. */
      free(*band);      /* The pointer array. */
    ELSEIF (stat EQ Iabsent)
      free((*band)[0]); /* The info data. No pixels allocated. */
    ENDIF;
    *band = NULL;
  ENDIF;

  LEAVE_FUNCTION_DEBUG("biff.c: Idel_band");

}   /*   Idel_band   */

/*D:Idel_band_pix*

________________________________________________________________

		Idel_band_pix
________________________________________________________________

Name:		Idel_band_pix - delete the pixels of a BIFF band

Syntax:         | #include <xite/biff.h>
		|
                | void Idel_band_pix( IBAND* band );

Description:	Deallocate virtual memory for the pixels (and the
                line pointers) of a band, using free(3). If this is a
		subband, the pixels will not be deallocated, but the
		line pointers will.

		The band pointer may have changed upon return, and the state
		of the band will be Iabsent.

See also:       Idel_image(5), Idel_band(5), Imake_subband(5), Iband_status(5)

Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

static void Icopy_band_info(_Iband_info *info1, _Iband_info *info2)
{
  info2->pixtyp      = info1->pixtyp;
  info2->xsize       = info1->xsize;
  info2->ysize       = info1->ysize;
  info2->xstart      = info1->xstart;
  info2->ystart      = info1->ystart;
  info2->xmag        = info1->xmag;
  info2->ymag        = info1->ymag;
  info2->status      = info1->status;
  info2->roi_xstart  = info1->roi_xstart;
  info2->roi_ystart  = info1->roi_ystart;
  info2->roi_xsize   = info1->roi_xsize;
  info2->roi_ysize   = info1->roi_ysize;
  info2->this_struct = (long *) info2;

  return;

} /* Icopy_band_info() */

void Idel_band_pix(IBAND *band)
{
  ENTER_FUNCTION_DEBUG("biff.c: Idel_band_pix");

  IF (*band NE NULL)
    IBAND new_band;
    BiffStatus stat;
    _Iband_info *binfo, *new_binfo;

    if ((stat = Iband_status(*band)) == Iabsent) {
      LEAVE_FUNCTION_DEBUG("biff.c: Idel_band_pix");
      return;
    }

    new_band  = Init_band(Ipixtyp(*band), Ixsize(*band), Iysize(*band));
    binfo     = (_Iband_info *) ((*band)[0]);
    new_binfo = (_Iband_info *) (new_band[0]);

    Icopy_band_info(binfo, new_binfo);
    new_binfo->status = Iabsent;

    Idel_band(band);
    *band = new_band;
  ENDIF;

  LEAVE_FUNCTION_DEBUG("biff.c: Idel_band_pix");

}   /*   Idel_band_pix   */

/*D:Iread_band*

________________________________________________________________

		Iread_band
________________________________________________________________

Name:		Iread_band - read BIFF band from file

Syntax:         | #include <xite/biff.h>
		|
                | BiffStatus Iread_band( IMAGE i, int bandnr );

Description:	'Iread_band' reads band number 'bandnr' from file into image
                'i'. The byte order of the stored band is compared to the byte
		order of the host computer. If they don''t match, the
		bytes are swapped after reading.

		If the status of the band is Iabsent, storage will be
		allocated for the pixels, by Imake_band.

		Be careful with reading a band if a band with a smaller band
		number has been deleted with Idel_band. The Iread_band routine
		uses info data from previous bands to calculate the location
		of a band. These info data will be invalid after a call to
		Idel_band. To save memory, call Idel_band_pix instead of
		Idel_band. This will only deallocate the pixel storage.

Restrictions:	Image 'i' must be open (connected to a file). This should be
                done by calling Iopen_image.

		1 <= bandnr <= Inbands(i)

See also:       Iread_image(5), Iopen_image(5), Iclose_image(5), Init_band(5),
                Init_image(5), Iwrite_band(5), Iband_status(5), biff_swap(3),
		biffswap(1)

Return value:	status
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

BiffStatus Iread_band(IMAGE i, int bandnr)
{
  _Info *info;
  _Iband_info *binfo;
  long start, size, bn, rest;
#if defined(MSDOS)
  long y, ysize, xsize;
#endif

  ENTER_FUNCTION_DEBUG("biff.c: Iread_band");

  FPRINTF2("    bandnr: %d\n", bandnr);

  info = (_Info *)i[0];
  if ((bandnr LT 1) OR (bandnr GT info->nbands)) {
    LEAVE_FUNCTION_DEBUG("biff.c: Iread_band");
    return(Ierr_action(Ibandnrerr));
  }

  start = info->ninfoblks * 512;
  for (bn=1; bn LT (long) bandnr; ++ bn)
    start += _num_blocks_band(i[bn]) * 512;

  size = Ibandsize(i[bandnr]);

  /* set file pointer to start of band */
  if (NOT info->maybe_pipe)
    if (start NE lseek(info->fd,start,L_SET)) {
      FPRINTF1("    lseek failed for start of band\n");
      LEAVE_FUNCTION_DEBUG("biff.c: Iread_band");
      return(Ierr_action(Ireaderr));
    }

  /* create band */
  binfo = (_Iband_info *)i[bandnr][0];
  if (Iband_status(i[bandnr]) EQ Iabsent)
    i[bandnr] = Imake_band(binfo->pixtyp,binfo->xsize,binfo->ysize);
  if (i[bandnr] EQ NULL) {
    LEAVE_FUNCTION_DEBUG("biff.c: Iread_band");
    return(Imallocerr);
  }

  Iset_start(i[bandnr],binfo->xstart, binfo->ystart);
  Iset_mag(i[bandnr],binfo->xmag, binfo->ymag);

#if defined(MSDOS)
  ysize = Iysize(i[bandnr]);
  xsize = Ixsize(i[bandnr]);
  FOR (y=1; y<=ysize; y++)
    if(xsize NE
       _Iread_(info->fd,
	       &(i[bandnr][y][Ipixsize(Ipixtyp(i[bandnr]))/8]),xsize)) {
      LEAVE_FUNCTION_DEBUG("biff.c: Iread_band");
      return(Ierr_action(Ireaderr));
    }
    size -= xsize;
  ENDFOR
#else
  if (size NE
      (rest = _Iread_(info->fd, (unsigned char *)
	      (&(i[bandnr][1][Ipixsize(Ipixtyp(i[bandnr]))/8])),size))) {
    FPRINTF2("    _Iread_ failed on band pixels, read %d\n", rest);
    LEAVE_FUNCTION_DEBUG("biff.c: Iread_band");
    return(Ierr_action(Ireaderr));
  }
#endif
  rest = (512 - size MOD 512);
  if (rest NE 512)
    if (rest NE (size = _Iread_(info->fd,buffer,rest))) {
      FPRINTF2("    _Iread_ failed on rest, read %d\n", size);
      LEAVE_FUNCTION_DEBUG("biff.c: Iread_band");
      return(Ierr_action(Ireaderr));
    }

  if (Ipixsize(binfo->pixtyp)/8 > 1 && 
      ((_Ilittle_endian(i[bandnr]) && host_byte_order()  == XITE_BIG_ENDIAN) ||
       (!_Ilittle_endian(i[bandnr]) &&
	host_byte_order() == XITE_LITTLE_ENDIAN)
       )
      ) {
    FPRINTF1("    Must swap band\n");
    biff_swap(i[bandnr]);
  }

  LEAVE_FUNCTION_DEBUG("biff.c: Iread_band");

  return(Iok);

} /* Iread_band() */

static BiffStatus _Iread_bands(IMAGE img, IMAGE *new_img, int *bandarr, int *num_bands)
{
  /* This routine does not read the text data. Some bands may be absent
   * upon return. Do not make this routine public. It should only be
   * called from Iread_image_bands(), to avoid messing up the image data
   * structure.
   */
  _Info *info;
  int bn1, bn2, bn3, prev_bn = 0;
  long maxbandsize = 0, size;
   BiffStatus status;
  IMAGE anotheri;
  unsigned char *dump = NULL;

  ENTER_FUNCTION_DEBUG("biff.c: _Iread_bands");

  FPRINTF2("    *num_bands: %d\n", *num_bands);

  if (new_img != NULL) *new_img = NULL;

  info = (_Info *)img[0];

  FPRINTF2("    maybe_pipe: %d\n", info->maybe_pipe);

  if (!bandarr || *num_bands == 0) {
    /* Prepare for reading all the bands. */

    *num_bands = Inbands(img);
    bandarr    = (int *) malloc((*num_bands + 1) * sizeof(int));
    if (!bandarr) {
      LEAVE_FUNCTION_DEBUG("biff.c: _Iread_bands");
      return(Ierr_action(Imallocerr));
    }
    for (bn1 = 1; bn1 <= *num_bands; bn1++)
      bandarr[bn1] = bn1;
  }

  /* Prepare for discarding band data if reading from a pipe. */
  if (info->maybe_pipe) {
    /* Determine required size of buffer for dumping band data into. Check
     * the size of all the bands, also those behind the ones to read. */
    for (bn1 = 1; bn1 <= Inbands(img); ++ bn1) {
      for (bn2 = 1; bn2 <= *num_bands; ++ bn2)
	if (bandarr[bn2] == bn1) break;
      
      if (bn2 > *num_bands) {
	/* Band number bn1 is not in the list of bands to read. */
	maxbandsize = MAX(maxbandsize, Ibandsize(img[bn1]));
      }
    }
  } /* Preparing for discarding of bands. */

  if (maxbandsize > 0) {
    FPRINTF2("    maxbandsize for discarding: %d\n", maxbandsize);
    dump = (unsigned char *) malloc(maxbandsize);
    if (!dump) {
      LEAVE_FUNCTION_DEBUG("biff.c: _Iread_bands");
      return(Ierr_action(Imallocerr));
    }

  }

  /* Read the specified bands. Minimize disk positioning by reading the
   * specified bands in increasing band number order, dumping intermediate
   * bands if reading from pipe.
   * Rearrange afterwards. */

  for (bn1 = 1; bn1 LE Inbands(img); ++ bn1) {
    for (bn2 = 1; bn2 LE *num_bands; ++ bn2)
      if (bandarr[bn2] EQ bn1) break;

    if (bn2 LE *num_bands) {
      /* Band number bn1 is in the list of bands to read. */

      if (info->maybe_pipe && prev_bn != bn1 - 1) {
	/* Discard data in pipe concerning intermediate bands. */

	for (bn3 = prev_bn + 1; bn3 < bn1; bn3++) {
	  size = Ibandsize(img[bn3]);
	  if (_Iread_(info->fd, dump, size) != size) {
	    FPRINTF2("    _Iread_ failed when discarding band %d\n", bn3);
	    LEAVE_FUNCTION_DEBUG("biff.c: _Iread_bands");
	    return(Ierr_action(Ireaderr));
	  }
	}
      } /* Discard data. */

      status  = Iread_band(img, bn1);
      prev_bn = bn1;
      if (status NE Iok) {
	FPRINTF2("    Iread_band failed in reading band %d\n", bn1);
	LEAVE_FUNCTION_DEBUG("biff.c: _Iread_bands");
	return(Ierr_action(status));
      }
    }
  } /* Reading specified bands and discarding intermediate bands. */

  /* Discard remaining bands in pipe. */
  if (info->maybe_pipe && prev_bn != 0) {
    /* At least one band read. */
    for (bn1 = prev_bn + 1; bn1 <= Inbands(img); bn1++) {
      size = Ibandsize(img[bn1]);
      if (_Iread_(info->fd, dump, size) != size) {
	FPRINTF2("    _Iread_ failed when discarding band %d\n", bn1);
	LEAVE_FUNCTION_DEBUG("biff.c: _Iread_bands");
	return(Ierr_action(Ireaderr));
      }
    }
  } /* Discarding remaining bands in pipe. */

  if (dump) free(dump);

  /* Rearrange the bands. */
  anotheri = Init_image(*num_bands, "");
  for (bn2 = 1; bn2 LE *num_bands; ++ bn2) {
    bn1 = bandarr[bn2];
    if (bn1 GT Inbands(img)) {
      LEAVE_FUNCTION_DEBUG("biff.c: _Iread_bands");
      return(Ierr_action(Ibandnrerr));
    }

    if (bn1 > 0) {
    /* Has this band been used before? */
      for (bn3 = 1; bn3 < bn2; ++ bn3) {
	if (bandarr[bn3] == -bn1) {
	  FPRINTF2("    Used band %d before.\n", bn1);
	  bn1 = -bn1;
	  break;
	}
      }
    }

    if (bn1 GT 0) {
      anotheri[bn2] = img[bn1];
      bandarr[bn2]  = -bandarr[bn2];
    } else { /* Neg. value indicates band already used, must now be copied. */
      bn1 = -bn1;
      anotheri[bn2] = Imake_band(Ipixtyp(img[bn1]),
				 Ixsize(img[bn1]), Iysize(img[bn1]));
      Iset_start(anotheri[bn2], Ixstart(img[bn1]), Iystart(img[bn1]));
      Iset_mag(anotheri[bn2], Ixmag(img[bn1]), Iymag(img[bn1]));
      Icopy_band(img[bn1], anotheri[bn2]);
    }
  }

  /* Copy back. */

  for (bn1 = 1; bn1 LE MIN(Inbands(img), Inbands(anotheri)); ++ bn1)
    img[bn1] = anotheri[bn1];

  if (Inbands(anotheri) > Inbands(img)) {
    /* img needs to be updated with the additional bands from anotheri
     * upon return from this routine. */
    *new_img = anotheri;
  }

  LEAVE_FUNCTION_DEBUG("biff.c: _Iread_bands");

  return(Iok);

} /* _Iread_bands() */

/*D:Iwrite_band*

________________________________________________________________

		Iwrite_band
________________________________________________________________

Name:		Iwrite_band - Write a BIFF band to file

Syntax:         | #include <xite/biff.h>
		|
                | BiffStatus Iwrite_band( IMAGE i, int bandnr );

Description:	Write band number 'bandnr' of image 'i' to file.

Restrictions:	Image 'i' must be connected to an open file, and have
                status Iwrite or Ireadwrite. The file must have
		write permission. Band number must be legal. The band
		status must be Inormal or Isubband.

See also:       Iwrite_image(5), Iopen_image(5), Iclose_image(5),
                Iread_band(5), Iband_status(5), biff_swap(3), biffswap(1)

Return value:	status
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

BiffStatus Iwrite_band(IMAGE i, int bandnr)
{
  _Info *info;
  long start, size, lsize, bn, y, rest;
#if defined(MSDOS)
  long ysize, xsize;
#endif

  ENTER_FUNCTION_DEBUG("biff.c: Iwrite_band");

  FPRINTF2("    bandnr: %d\n", bandnr);

  info = (_Info *)i[0];
  if ((bandnr LT 1) OR (bandnr GT info->nbands)) {
    LEAVE_FUNCTION_DEBUG("biff.c: Iwrite_band");
    return(Ierr_action(Ibandnrerr));
  }
  if ((info->status EQ Ireadonly) OR (info->status EQ Ilocal)) {
    LEAVE_FUNCTION_DEBUG("biff.c: Iwrite_band");
    return(Ierr_action(Istatuserr));
  }
  if (Iband_status(i[bandnr]) EQ Iabsent) {
    LEAVE_FUNCTION_DEBUG("biff.c: Iwrite_band");
    return(Ierr_action(Ibanderr));
  }

  start = info->ninfoblks * 512;
  for (bn=1; bn LT (long) bandnr; ++ bn)
    start += _num_blocks_band(i[bn]) * 512;

  size = Ibandsize(i[bandnr]);

  if (NOT info->maybe_pipe)
    if (start NE lseek(info->fd,start,L_SET)) {
      LEAVE_FUNCTION_DEBUG("biff.c: Iwrite_band");
      return(Ierr_action(Iwriterr));
    }

  IF (Iband_status(i[bandnr]) EQ Inormal)
#if defined(MSDOS)
    /* Write one line at a time. */
    ysize = Iysize(i[bandnr]);
    xsize = Ixsize(i[bandnr]);
    FOR (y=1; y<=ysize; y++)
      if(xsize NE
	 _Iwrite_(info->fd,
		  &(i[bandnr][y][Ipixsize(Ipixtyp(i[bandnr]))/8]),xsize)) {
	LEAVE_FUNCTION_DEBUG("biff.c: Iwrite_band");
        return(Ierr_action(Ireaderr));
      }
      size -= xsize;
    ENDFOR
#else
    /* Write all pixels. */
    if (size NE
	_Iwrite_(info->fd,
		 &(i[bandnr][1][Ipixsize(Ipixtyp(i[bandnr]))/8]),size)) {
      LEAVE_FUNCTION_DEBUG("biff.c: Iwrite_band");
      return(Ierr_action(Iwriterr));
    }
#endif
  ELSEIF (Iband_status(i[bandnr]) EQ Isubband)
    /* Write one line at a time. */
    lsize = Ipixsize(Ipixtyp(i[bandnr]))*Ixsize(i[bandnr])/8;
    FOR (y=1; y LE Iysize(i[bandnr]); ++ y)
      if (lsize NE
	  _Iwrite_(info->fd,
		   &(i[bandnr][y][Ipixsize(Ipixtyp(i[bandnr]))/8]),lsize)) {
	LEAVE_FUNCTION_DEBUG("biff.c: Iwrite_band");
	return(Ierr_action(Iwriterr));
      }
    ENDFOR;
  ENDIF;

  rest = (512 - size MOD 512);
  if (rest NE 512)
    if (rest NE _Iwrite_(info->fd,&(buffer[512-rest]),rest)) {
      LEAVE_FUNCTION_DEBUG("biff.c: Iwrite_band");
      return(Ierr_action(Iwriterr));
    }

  LEAVE_FUNCTION_DEBUG("biff.c: Iwrite_band");

  return(Iok);

}   /*   Iwrite_band   */

/*D:Iband_status*

________________________________________________________________

		Iband_status
________________________________________________________________

Name:		Iband_status - return the status of a BIFF band

Syntax:         | #include <xite/biff.h>
		|
                | long Iband_status( IBAND band );

Description:	Returns the status of 'band', one of: Inormal, Isubband,
                Iabsent.

		&Iabsent
		Band info data exists, but pixel storage has not been
		allocated.

		&Inormal
		Band info data as well as pixel data have been allocated.

		&Isubband
		Band info data exists. Pixel storage has not been allocated,
		but a pointer array refers to lines in a different band
		(possibly in a different image).

Restrictions:	'band' must be initialized.

See also:       Iread_band(5), Iwrite_band(5), Iread_image(5), Iwrite_image(5),
                Idel_band(5), Imake_subband(5)

Return value:	The status
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

long Iband_status(IBAND band)
{
  _Iband_info *binfo;

  binfo = (_Iband_info *)band[0];

  return(binfo->status);
}   /*   Iband_status   */

/*D:Ixsize*

________________________________________________________________

		Ixsize
________________________________________________________________

Name:		Ixsize - return horizontal BIFF band size

Syntax:         | #include <xite/biff.h>
		|
                | long Ixsize( IBAND band );

Description:	Return the horizontal size of band 'band' in number
                of pixels.

Restrictions:	The band must be initialized.

See also:       Iysize(5), Imake_band(5), Ixstart(5), Iystart(5), Ixmag(5),
                Iymag(5), biffinfo(1)

Return value:	The horizontal size.
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

long Ixsize(IBAND band)
{
  _Iband_info *binfo;

  binfo = (_Iband_info *)band[0];

  return(binfo->xsize);
}   /*   Ixsize   */

/*D:Iysize*

________________________________________________________________

		Iysize
________________________________________________________________

Name:		Iysize - return vertical BIFF band size

Syntax:         | #include <xite/biff.h>
		|
                | long Iysize( IBAND band );

Description:	Return the vertical size of band 'band' in number
                of pixels.

Restrictions:	The band must be initialized.

See also:       Ixsize(5), Imake_band(5), Ixstart(5), Iystart(5), Ixmag(5),
                Iymag(5), biffinfo(1)

Return value:	The vertical size.
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

long Iysize(IBAND band)
{
  _Iband_info *binfo;

  binfo = (_Iband_info *)band[0];

  return(binfo->ysize);
}   /*   Iysize   */

/*D:Ixstart*

________________________________________________________________

		Ixstart
________________________________________________________________

Name:		Ixstart - return horizontal starting point of BIFF band

Syntax:         | #include <xite/biff.h>
		|
                | long Ixstart( IBAND band );

Description:	Returns the xstart attribute of band 'band',
                telling where to place the band in a global
		coordinate system.

Restrictions:	The band must be initialized.

See also:       Iystart(5), Imake_band(5), Ixsize(5), Iysize(5), Ixmag(5),
                Iymag(5), Ibandsize(5), biffinfo(1)

Return value:	The xstart attribute.
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

long Ixstart(IBAND band)
{
  _Iband_info *binfo;

  if (band EQ NULL) return(Ierr_action(Ibanderr));
  binfo = (_Iband_info *)band[0];

  return(binfo->xstart);
}   /*   Ixstart   */

/*D:Iystart*

________________________________________________________________

		Iystart
________________________________________________________________

Name:		Iystart - return vertical starting point of BIFF band

Syntax:         | #include <xite/biff.h>
		|
                | long Iystart( IBAND band );

Description:	Returns the ystart attribute of band 'band',
                telling where to place the band in a global
		coordinate system.

Restrictions:	The band must be initialized.

See also:       Ixstart(5), Imake_band(5), Ixsize(5), Iysize(5), Ixmag(5),
                Iymag(5), Ibandsize(5), biffinfo(1)

Return value:	The ystart attribute
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

long Iystart(IBAND band)
{
  _Iband_info *binfo;

  if (band EQ NULL) return(Ierr_action(Ibanderr));
  binfo = (_Iband_info *)band[0];

  return(binfo->ystart);
}   /*   Iystart   */

/*D:Ixmag*

________________________________________________________________

		Ixmag
________________________________________________________________

Name:		Ixmag - return the horizontal magnification of BIFF band
Syntax:         | #include <xite/biff.h>
		|
                | long Ixmag( IBAND band );

Description:	Return the xmag attribute of band 'band',
                telling how many times to duplicate every pixel in
		the horizontal direction.

Restrictions:	The band must be initialized.

See also:       Iymag(5), Imake_band(5), Ixsize(5), Iysize(5), Ixstart(5),
                Iystart(5), biffinfo(1)

Return value:	The xmag attribute.
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

long Ixmag(IBAND band)
{
  _Iband_info *binfo;

  if (band EQ NULL) return(Ierr_action(Ibanderr));
  binfo = (_Iband_info *)band[0];

  return(binfo->xmag);
}   /*   Ixmag   */

/*D:Iymag*

________________________________________________________________

		Iymag
________________________________________________________________

Name:		Iymag - return the vertical magnification of BIFF band

Syntax:         | #include <xite/biff.h>
		|
                | long Iymag( IBAND band );

Description:	Return the ymag attribute of band 'band',
                telling how many times to duplicate every pixel in
		the vertical direction.

Restrictions:	The band must be initialized.

See also:       Ixmag(5), Imake_band(5), Ixsize(5), Iysize(5), Ixstart(5),
                Iystart(5), biffinfo(1)

Return value:	The ymag attribute.
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

long Iymag(IBAND band)
{
  _Iband_info *binfo;

  if (band EQ NULL) return(Ierr_action(Ibanderr));
  binfo = (_Iband_info *)band[0];

  return(binfo->ymag);
}   /*   Iymag   */

/*D:Iset_start*

________________________________________________________________

		Iset_start
________________________________________________________________

Name:		Iset_start - set the xstart and ystart attributes of BIFF band

Syntax:         | #include <xite/biff.h>
		|
                | BiffStatus Iset_start( IBAND band, long xstart,
                |    long ystart );

Description:	Assign new values 'xstart' and 'ystart' to the
                corresponding attributes in 'band'.

Restrictions:	The band must be initialized.

See also:       Ixstart(5), Iystart(5), Imake_band(5), Ixsize(5), Iysize(5),
                Ixmag(5), Iymag(5), Iset_mag(5), biffinfo(1)

Return value:	status
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

BiffStatus Iset_start(IBAND band, long int xstart, long int ystart)
{
  _Iband_info *binfo;

  if (band EQ NULL) return(Ierr_action(Ibanderr));
  binfo = (_Iband_info *)band[0];
  binfo->xstart = xstart;
  binfo->ystart = ystart;

  return(Iok);
}   /*   Iset_start   */

/*D:Iset_mag*

________________________________________________________________

		Iset_mag
________________________________________________________________

Name:		Iset_mag - set the magnification attributes of BIFF band

Syntax:         | #include <xite/biff.h>
		|
                | BiffStatus Iset_mag( IBAND band, long xmag,
                |    long ymag );

Description:	Assign new values 'xmag' and 'ymag' to the
                corresponding attributes in 'band'.

Restrictions:	The band must be initialized.

See also:       Ixmag(5), Iymag(5), Imake_band(5), Ixsize(5), Iysize(5),
                Ixstart(5), Iystart(5), Iset_start(5), biffinfo(1)

Return value:	status
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

BiffStatus Iset_mag(IBAND band, long int xmag, long int ymag)
{
  _Iband_info *binfo;

  if (band EQ NULL) return(Ierr_action(Ibanderr));
  binfo = (_Iband_info *)band[0];
  binfo->xmag = xmag;
  binfo->ymag = ymag;

  return(Iok);
}   /*   Iset_mag   */

/*D:Iget_roi*

________________________________________________________________

		Iget_roi
________________________________________________________________

Name:		Iget_roi - return Region Of Interest of BIFF band

Syntax:         | #include <xite/biff.h>
		|
                | BiffStatus Iget_roi( IBAND band,
                |    long* roi_xstart, long* roi_ystart,
                |    long* roi_xsize, long* roi_ysize );

Description:	Returns the ROI attributes of the band. Maintained
                mainly for historical reasons, use the subband
		technique whenever possible.

Restrictions:	The band must be initialized.

See also:       Imake_subband(5)
Return value:	status
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

BiffStatus Iget_roi(IBAND band, long int *roi_xstart, long int *roi_ystart, long int *roi_xsize, long int *roi_ysize)
{
  _Iband_info *binfo;

  if (band EQ NULL) return(Ierr_action(Ibanderr));
  binfo = (_Iband_info *)band[0];
  *roi_xstart = binfo->roi_xstart;
  *roi_ystart = binfo->roi_ystart;
  *roi_xsize  = binfo->roi_xsize;
  *roi_ysize  = binfo->roi_ysize;

  return(Iok);
}   /*   Iget_roi   */

/*D:Iset_roi*

________________________________________________________________

		Iset_roi
________________________________________________________________

Name:		Iset_roi - set new Region Of Interest values for BIFF band

Syntax:         | #include <xite/biff.h>
		|
                | BiffStatus Iset_roi( IBAND band,
                |    long roi_xstart, long roi_ystart,
                |    long roi_xsize, long roi_ysize );

Description:	Assig new values to the ROI attributes of 'band'.
                Maintained mainly for historical reasons, use the 
		subband technique whenever possible.

Restrictions:	The band must be initialized.

See also:       Imake_subband(5)
Return value:	status
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

BiffStatus Iset_roi(IBAND band, long int roi_xstart, long int roi_ystart, long int roi_xsize, long int roi_ysize)
{
  _Iband_info *binfo;

  if (band EQ NULL) return(Ierr_action(Ibanderr));

  binfo = (_Iband_info *)band[0];
  if ((roi_xstart LT 1) OR (roi_ystart LT 1)
    OR (roi_xsize LT 0) OR (roi_ysize LT 0)
    OR (roi_xstart+roi_xsize-1 GT binfo->xsize)
    OR (roi_ystart+roi_ysize-1 GT binfo->ysize)) {
    return(Ierr_action(Iroierr));
  }

  binfo->roi_xstart = roi_xstart;
  binfo->roi_ystart = roi_ystart;
  binfo->roi_xsize = roi_xsize;
  binfo->roi_ysize = roi_ysize;    

  return(Iok);
}   /*   Iset_roi   */

/*D:Ibandsize*

________________________________________________________________

		Ibandsize
________________________________________________________________

Name:		Ibandsize - Return BIFF band size in bytes

Syntax:         | #include <xite/biff.h>
		|
                | long Ibandsize( IBAND band );

Description:	Return the number of bytes occupied by this band.

Restrictions:	The band must be initialized.

See also:       Ixsize(5), Iysize(5), Ipixsize(5), Imake_band(5)

Return value:	Size if ok, -1 otherwise.
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

long Ibandsize(IBAND band)
{
  _Iband_info *binfo;
  long bitsize;

  if (band EQ NULL) return(-1);
  binfo = (_Iband_info *) band[0];
  bitsize = Ipixsize(binfo->pixtyp) * binfo->xsize * binfo->ysize;

  return((bitsize+7) / 8);
}    /*   Ibandsize   */

/*D:Ipixtyp*

________________________________________________________________

		Ipixtyp
________________________________________________________________

Name:		Ipixtyp - return the pixel type of the BIFF band

Syntax:         | #include <xite/biff.h>
		|
                | IPIXTYP Ipixtyp( IBAND band );

Description:	Return the pixel type of 'band' as a long defined
                by the pixel type definitions in biff.h.

		The returned value will be
		Iu_color_typ = (Iu_byte_typ | Icolor_mask) for
		an unsigned byte colortable pixeltype.

Restrictions:	The band must be initialized.

See also:       Ipixsize(5), Ipixname(5), IparsePixtyp(5), Imake_band(5),
                biffinfo(1)

Return value:	pixel type if ok, -1 otherwise.
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

IPIXTYP Ipixtyp(IBAND band)
{
  _Iband_info *binfo;

  if (band EQ NULL) { Ierr_action(Ibanderr); return(-1); }
  binfo = (_Iband_info *)band[0];

  return(binfo->pixtyp & Ipixtyp_mask);
}   /*   Ipixtyp   */

/*D:IpixPrintFormat=Ipixname*/
/*D:Ipixname*

________________________________________________________________

		Ipixname
________________________________________________________________

Name:		Ipixname, IpixPrintFormat - return the pixeltype name of a
                BIFF pixel type, and the format string for printing
		pixel values

Syntax:         | #include <xite/biff.h>
		|
                | char *Ipixname( IPIXTYP pixtyp );
		|
		| char *IpixPrintFormat( IPIXTYP pixtyp );

Description:	'Ipixname' returns the pixeltype name of 'pixtyp'.
                'pixtyp' is defined by the pixel type 
		definitions in biff.h. Colortable is also a pixel type.

		'IpixPrintFormat' returns the format string used to
		print pixel values for the pixel type given by 'pixtyp'.

See also:       Ipixtyp(5), IparsePixtyp(5), Ipixsize(5), biffinfo(1)

Example:	| IBAND band;
                | printf("Pixel type = %s\n", Ipixname(Ipixtyp(band)));

Author:		Otto Milvang and Svein Bøe, BLAB, Ifi, UiO
________________________________________________________________

*/

char* Ipixname(IPIXTYP pixtyp)
{
  pixtyp = pixtyp & Ipixtyp_mask;
  if (pixtyp == Iu_color_typ) return(pixtypname[12]);
  if (pixtyp >= 0 && pixtyp <= 11) return(pixtypname[pixtyp]);
  return(pixtypname[13]); 
}   /*   Ipixname   */

char* IpixPrintFormat(IPIXTYP pixtyp)
{
  pixtyp = pixtyp & Ipixtyp_mask;
  if (pixtyp >= 0 && pixtyp <= 11) return(pix_print_format[pixtyp]);
  return(NULL);
}   /*   IpixPrintFormat()   */

/*D:IparsePixtyp*

________________________________________________________________

		IparsePixtyp
________________________________________________________________

Name:		IparsePixtyp - return the BIFF pixeltype from a string
                representation

Syntax:         | #include <xite/biff.h>
		|
                | IPIXTYP IparsePixtyp( char *pixtyp );
		|

Description:	Return the pixeltype of 'pixtyp'. Pixeltype is defined by
                the pixeltype definitions in biff.h.

		Legal forms of 'pixtyp' are the pixeltype numbers, the
		full pixeltype name (as defined by Ipixname) and short forms
		of the pixeltype name.

		| Full name   Short name  Number  Symbolic number
		| -----------------------------------------------
		| uns_bit        ui         0     Ibit_typ      
		| uns_2bit       u2i        1     Ibit2_typ     
		| uns_nibble     un         2     Inibble_typ   
		| uns_byte       ub         3     Iu_byte_typ   
		| sign_byte      sb         4     Is_byte_typ   
		| uns_short      us         5     Iu_short_typ  
		| sign_short     ss         6     Is_short_typ  
		| integer        i          7     Integer_typ   
		| real           r/f        8     Ireal_typ     
		| complex        c          9     Icomplex_typ  
		| double         d         10     Idouble_typ   
		| dcomplex       dc        11     Id_complex_typ

See also:       Ipixname(5), Ipixtyp(5), Ipixsize(5)

Return value:	Pixeltype if pixtyp is ok, Iunknown_typ otherwise.
Author:		Tor Lønnestad and Svein Bøe, BLAB, Ifi, UiO
________________________________________________________________

*/

IPIXTYP IparsePixtyp(char *pixtyp)
{
  if (! strcasecmp(pixtyp, "ui"        )) return(Ibit_typ);
  if (! strcmp    (pixtyp, "0"         )) return(Ibit_typ);
  if (! strcasecmp(pixtyp, "uns_bit"   )) return(Ibit_typ);
  if (! strcasecmp(pixtyp, "u2i"       )) return(Ibit2_typ);
  if (! strcmp    (pixtyp, "1"         )) return(Ibit2_typ);
  if (! strcasecmp(pixtyp, "uns_2bit"  )) return(Ibit2_typ);
  if (! strcasecmp(pixtyp, "un"        )) return(Inibble_typ);
  if (! strcmp    (pixtyp, "2"         )) return(Inibble_typ);
  if (! strcasecmp(pixtyp, "uns_nibble")) return(Inibble_typ);
  if (! strcasecmp(pixtyp, "ub"        )) return(Iu_byte_typ);
  if (! strcmp    (pixtyp, "3"         )) return(Iu_byte_typ);
  if (! strcasecmp(pixtyp, "uns_byte"  )) return(Iu_byte_typ);
  if (! strcasecmp(pixtyp, "sb"        )) return(Is_byte_typ);
  if (! strcmp    (pixtyp, "4"         )) return(Is_byte_typ);
  if (! strcasecmp(pixtyp, "sign_byte" )) return(Is_byte_typ);
  if (! strcasecmp(pixtyp, "us"        )) return(Iu_short_typ);
  if (! strcmp    (pixtyp, "5"         )) return(Iu_short_typ);
  if (! strcasecmp(pixtyp, "uns_short" )) return(Iu_short_typ);
  if (! strcasecmp(pixtyp, "ss"        )) return(Is_short_typ);
  if (! strcmp    (pixtyp, "6"         )) return(Is_short_typ);
  if (! strcasecmp(pixtyp, "sign_short")) return(Is_short_typ);
  if (! strcasecmp(pixtyp, "i"         )) return(Integer_typ);
  if (! strcmp    (pixtyp, "7"         )) return(Integer_typ);
  if (! strcasecmp(pixtyp, "integer"   )) return(Integer_typ);
  if (! strcasecmp(pixtyp, "r"         )) return(Ireal_typ);
  if (! strcasecmp(pixtyp, "f"         )) return(Ireal_typ);
  if (! strcmp    (pixtyp, "8"         )) return(Ireal_typ);
  if (! strcasecmp(pixtyp, "real"      )) return(Ireal_typ);
  if (! strcasecmp(pixtyp, "c"         )) return(Icomplex_typ);
  if (! strcmp    (pixtyp, "9"         )) return(Icomplex_typ);
  if (! strcasecmp(pixtyp, "complex"   )) return(Icomplex_typ);
  if (! strcasecmp(pixtyp, "d"         )) return(Idouble_typ);
  if (! strcmp    (pixtyp, "10"        )) return(Idouble_typ);
  if (! strcasecmp(pixtyp, "double"    )) return(Idouble_typ);
  if (! strcasecmp(pixtyp, "dc"        )) return(Id_complex_typ);
  if (! strcmp    (pixtyp, "11"        )) return(Id_complex_typ);
  if (! strcasecmp(pixtyp, "dcomplex"  )) return(Id_complex_typ);
  return(Error(Iunknown_typ, "IparsePixtyp: Unknown pixel type: %s\n",pixtyp));
}

/*D:Ipixsize*

________________________________________________________________

		Ipixsize
________________________________________________________________

Name:		Ipixsize - return the BIFF pixel size in bits

Syntax:         | #include <xite/biff.h>
		|
                | long Ipixsize( IPIXTYP pity );

Description:	Return the number of bits needed to store one
                pixel of type 'pity', defined by the pixel type
		definitions in biff.h.

Restrictions:	'pity' must be legal pixel type.

See also:       Ipixtyp(5), Ipixname(5), IparsePixtyp(5)
Return value:	size if ok, -1 otherwise.
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

long Ipixsize(IPIXTYP pity)
{
  long pt;

  switch (pity BAND 0x0ff) { /* Strip off endian-bit and color-bit */
    case Ibit_typ:       pt = 1;                      break;
    case Ibit2_typ:      pt = 2;                      break;
    case Inibble_typ:    pt = 4;                      break;
    case Iu_byte_typ:    pt = 8 * sizeof(UNS_BYTE);   break;
    case Is_byte_typ:    pt = 8 * sizeof(SIGN_BYTE);  break;
    case Iu_short_typ:   pt = 8 * sizeof(UNS_SHORT);  break;
    case Is_short_typ:   pt = 8 * sizeof(SIGN_SHORT); break;
    case Integer_typ:    pt = 8 * sizeof(INTEGER);    break;
    case Ireal_typ:      pt = 8 * sizeof(REAL);       break;
    case Icomplex_typ:   pt = 8 * sizeof(COMPLEX);    break;
    case Idouble_typ:    pt = 8 * sizeof(DOUBLE);     break;
    case Id_complex_typ: pt = 8 * sizeof(DCOMPLEX);   break;
    default:             pt = -1;                     break;
    }

  return(pt);

}    /*   Ipixsize   */

/*D:Icopy_band*

________________________________________________________________

		Icopy_band
________________________________________________________________

Name:		Icopy_band - copy a BIFF band

Syntax:         | #include <xite/biff.h>
		|
                | BiffStatus Icopy_band( IBAND b1, IBAND b2 );

Description:    The largest rectangle common to b1 and b2 is copied
                from b1 to b2. A direct copy is performed, which will
                give strange effects if b1 and b2 are partly
                overlapping subbands. b1 and b2 should have the same
                pixel type, but all currently known pixel types are
                supported.

See also:       Imake_band(5), Imake_subband(5), Init_band(5), Ipixtyp(5),
                Ixstart(5), Iystart(5)

Return value:   | Iok => ok
                | Ipixtyperr => b1 and b2 have different pixel types,
                |               or unknown pixel type
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

BiffStatus Icopy_band(IBAND b1, IBAND b2)
{
  int x, y, xsize, ysize;
  /* Declaire band pointers for all different pixel SIZES. */
  IUB_BAND iub1, iub2;
  IUS_BAND ius1, ius2;
  II_BAND  ii1, ii2;
  ID_BAND  id1, id2;
  IDC_BAND idc1, idc2;
  
  if (Ipixtyp(b1) NE Ipixtyp(b2)) return(Ierr_action(Ipixtyperr));
  xsize = MIN(Ixsize(b1), Ixsize(b2));
  ysize = MIN(Iysize(b1), Iysize(b2));

  switch (Ipixtyp(b1)) {
    case Iu_byte_typ:
    case Is_byte_typ:
      iub1 = (IUB_BAND) b1;
      iub2 = (IUB_BAND) b2;
      for (y=1; y LE ysize; ++ y)
        for (x=1; x LE xsize; ++ x)
           iub2[y][x] = iub1[y][x];
      break;
 
    case Iu_short_typ:
    case Is_short_typ:
      ius1 = (IUS_BAND) b1;
      ius2 = (IUS_BAND) b2;
      for (y=1; y LE ysize; ++ y)
        for (x=1; x LE xsize; ++ x)
           ius2[y][x] = ius1[y][x];
      break;
 
    case Integer_typ:
    case Ireal_typ:
      ii1 = (II_BAND) b1;
      ii2 = (II_BAND) b2;
      for (y=1; y LE ysize; ++ y)
        for (x=1; x LE xsize; ++ x)
           ii2[y][x] = ii1[y][x];
      break;

    case Icomplex_typ:
    case Idouble_typ:
      id1 = (ID_BAND) b1;
      id2 = (ID_BAND) b2;
      for (y=1; y LE ysize; ++ y)
        for (x=1; x LE xsize; ++ x)
           id2[y][x] = id1[y][x];
      break;

    case Id_complex_typ:
      idc1 = (IDC_BAND) b1;
      idc2 = (IDC_BAND) b2;
      for (y=1; y LE ysize; ++ y)
        for (x=1; x LE xsize; ++ x)
           idc2[y][x] = idc1[y][x];
      break;

    default:
      return(Ierr_action(Ipixtyperr));
    }
  return(0);
}   /* Icopy_band */

/*
------------------------------------------------------------------------
----------------------  TEXT HANDLING PROCEDURES  ----------------------
------------------------------------------------------------------------
*/ 


/*D:Iread_text*

________________________________________________________________

		Iread_text
________________________________________________________________

Name:		Iread_text - read text field from file into BIFF image

Syntax:         | #include <xite/biff.h>
		|
                | BiffStatus Iread_text( IMAGE i );

Description:	Read the whole text field from file into image 'i'.

Restrictions:	'i' must be connected to an open image file, and
		be in Ireadonly or Ireadwrite state.

See also:       Iwrite_text(5), Icopy_text(5), Iput_text(5), Iget_line(5),
                Iend_of_text(5), Iappend_line(5), Itype_text(5),
		Iedit_text(5), Idel_text(5), Igetpos(5), Isetpos(5),
		Inchars(5), biffinfo(1), bifftext(1)

Return value:	status
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

BiffStatus Iread_text(IMAGE i)
{
  _Info *info;
  long start,size,bufsize, rest;
  unsigned char *buf;

  ENTER_FUNCTION_DEBUG("biff.c: Iread_text");

  if (i EQ NULL) {
    LEAVE_FUNCTION_DEBUG("biff.c: Iread_text");
    return(Ierr_action(Imageerr));
  }

  info = (_Info *)i[0];

  FPRINTF2("    maybe_pipe: %d\n", info->maybe_pipe);

  if (info->status EQ Ilocal) {
    LEAVE_FUNCTION_DEBUG("biff.c: Iread_text");
    return(Ierr_action(Istatuserr));
  }
  if (info->text NE NULL) {
    LEAVE_FUNCTION_DEBUG("biff.c: Iread_text");
    return(Ierr_action(Itexterr));
  }

  size    = info->nchars;
  bufsize = ((size+511) / 512)*512;
  if (bufsize EQ 0) bufsize = 512;
  buf = (unsigned char *)malloc(bufsize);
  if (buf EQ NULL) {
    LEAVE_FUNCTION_DEBUG("biff.c: Iread_text");
    return(Ierr_action(Imallocerr));
  }

  start = (info->ninfoblks + info->nbandblks)*512;
  if (NOT info->maybe_pipe)
    if (lseek(info->fd,start,L_SET) NE start) {
      free(buf);
      LEAVE_FUNCTION_DEBUG("biff.c: Iread_text");
      return(Ierr_action(Itexterr));
    }
  if (_Iread_(info->fd, buf, size) NE size) {
    free(buf);
    LEAVE_FUNCTION_DEBUG("biff.c: Iread_text");
    return(Ierr_action(Itexterr));
  }
  info->text = (char *) DEC buf;
  info->textbufblks = bufsize / 512;
  rest = 512 - size MOD 512;
  if (rest NE 512 AND rest NE 0)
    if (_Iread_(info->fd, buffer, rest) NE rest) {
      LEAVE_FUNCTION_DEBUG("biff.c: Iread_text");
      return(Ierr_action(Itexterr));
    }

  LEAVE_FUNCTION_DEBUG("biff.c: Iread_text");

  return(Iok);

}   /*   Iread_text   */

/*D:Iwrite_text*

________________________________________________________________

		Iwrite_text
________________________________________________________________

Name:		Iwrite_text - write text field from BIFF image to file
Syntax:         | #include <xite/biff.h>
		|
                | BiffStatus Iwrite_text( IMAGE i );

Description:	Write the whole text field in image 'i' to file.

Restrictions:	'i' must be connected to an open image file in
                Ireadwrite or Iwrite state.

See also:       Iread_text(5), Icopy_text(5), Iput_text(5), Iget_line(5),
                Iend_of_text(5), Iappend_line(5), Itype_text(5),
		Iedit_text(5), Idel_text(5), Igetpos(5), Isetpos(5),
		Inchars(5), bifftext(1)

Return value:	status
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

BiffStatus Iwrite_text(IMAGE i)
{
  _Info *info;
  long start,bufsize,textblk,newblks,blockblk,newblockblk, rest;
  unsigned char *buf;

  ENTER_FUNCTION_DEBUG("biff.c: Iwrite_text");

  if (i EQ NULL) {
    LEAVE_FUNCTION_DEBUG("biff.c: Iwrite_text");
    return(Ierr_action(Imageerr));
  }

  info = (_Info *)i[0];

  FPRINTF2("    maybe_pipe: %d\n", info->maybe_pipe);

  bufsize = info->textbufblks*512;
  textblk = info->ninfoblks+info->nbandblks;
  newblks = info->textbufblks - info->ntextblks;

  if ((info->status EQ Ilocal) OR (info->status EQ Ireadonly)) {
    LEAVE_FUNCTION_DEBUG("biff.c: Iwrite_text");
    return(Ierr_action(Istatuserr));
  }
  if (info->nchars EQ 0) {
    LEAVE_FUNCTION_DEBUG("biff.c: Iwrite_text");
    return(Iok);
  }
  if (info->text EQ NULL) {
    LEAVE_FUNCTION_DEBUG("biff.c: Iwrite_text");
    return(Ierr_action(Itexterr));
  }

  IF (newblks GT 0 AND NOT info->maybe_pipe)
    /* move block data in file */
    blockblk = textblk+info->ntextblks;
    newblockblk = blockblk + newblks;
    _Imove_blks(i,blockblk,newblockblk,info->nblocks);
    info->ntextblks += newblks;
  ENDIF;

  start = textblk*512;
  buf = (unsigned char *) info->text;
  ++ buf; /* to start with character 1 */
  if (NOT info->maybe_pipe)
    if (lseek(info->fd,start,L_SET) NE start) {
      LEAVE_FUNCTION_DEBUG("biff.c: Iwrite_text");
      return(Ierr_action(Itexterr));
    }
  if (_Iwrite_(info->fd, buf, bufsize) NE bufsize) {
    LEAVE_FUNCTION_DEBUG("biff.c: Iwrite_text");
    return(Ierr_action(Itexterr));
  }

  rest = 512 - bufsize MOD 512;
  if (rest NE 512)
    if (_Iwrite_(info->fd,buffer,rest) NE rest) {
      LEAVE_FUNCTION_DEBUG("biff.c: Iwrite_text");
      return(Ierr_action(Itexterr));
    }

  LEAVE_FUNCTION_DEBUG("biff.c: Iwrite_text");

  return(Iok);

}   /*   Iwrite_text   */

/*D:Igetpos*

________________________________________________________________

		Igetpos
________________________________________________________________

Name:		Igetpos - return character position in BIFF image text field

Syntax:         | #include <xite/biff.h>
		|
                | long Igetpos( IMAGE i );

Description:	Return the character position in image 'i'.
		Characters are numbered from 1 to Inchars(i),
		and the character position may be 1..Inchars(i)+1.

Restrictions:	'i' must be initialized.

See also:       Inchars(5), Isetpos(5)

Return value:	character position if ok, -1 otherwise.
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

long Igetpos(IMAGE i)
{
  _Info *info;

  if (i EQ NULL) return(-1);
  info = (_Info *)i[0];

  return(info->pos);
}   /*   Igetpos   */

/*D:Isetpos*

________________________________________________________________

		Isetpos
________________________________________________________________

Name:		Isetpos - set new character position in BIFF image text field

Syntax:         | #include <xite/biff.h>
		|
                | BiffStatus Isetpos( IMAGE i, long pos );

Description:	Move character position in image 'i' to 'pos'.

Restrictions:	'i' must be initialized, 'pos' must be 1..Inchars(i)+1.

See also:       Igetpos(5), Inchars(5)

Return value:	status
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

BiffStatus Isetpos(IMAGE i, long int pos)
{
  _Info *info;

  if (i EQ NULL) return(Ierr_action(Imageerr));
  info = (_Info *)i[0];
  if ((pos LT 1) OR (pos GT info->nchars+1))
    return(Ierr_action(Isetposerr));
  info->pos = pos;

  return(Iok);
}    /* Isetpos */

/*D:Igetchar*

________________________________________________________________

		Igetchar
________________________________________________________________

Name:		Igetchar - read a character from BIFF image text field

Syntax:         | #include <xite/biff.h>
		|
                | BiffStatus Igetchar( IMAGE i, char* c );

Description:	Read one character from image 'i' into '*c', and
                move character position ahead. ^D is returned at
		end of text.

Restrictions:	'i' must be initialized.

See also:       Iputchar(5), Inchars(5), Igetpos(5), Isetpos(5)

Return value:	status
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

BiffStatus Igetchar(IMAGE i, char *c)
{
  _Info *info;

  if (i EQ NULL) return(Ierr_action(Imageerr));
  info = (_Info *)i[0];
  if (info->pos GT info->nchars) { *c='\004'; return(Iok); }
  if (info->text EQ NULL) return(Ierr_action(Itexterr));
  *c = info->text[info->pos];
  ++ info->pos;

  return(Iok);
}   /*   Igetchar   */

/*D:Iputchar*

________________________________________________________________

		Iputchar
________________________________________________________________

Name:		Iputchar - insert character at current position in BIFF image
                text field

Syntax:         | #include <xite/biff.h>
		|
                | BiffStatus Iputchar( IMAGE i, int c );

Description:	Insert the character 'c' at current character
		position in image 'i'. If at end of text, the
		text field is enlarged, otherwise old text is
		overwritten. Character position is advanced.

Restrictions:	'i' must be initialized.

See also:       Igetchar(5), Inchars(5), Igetpos(5), Isetpos(5)

Return value:	status
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

BiffStatus Iputchar(IMAGE i, int c)
{
  _Info *info;
  char *buf;

  if (i EQ NULL) return(Ierr_action(Imageerr));
  info = (_Info *)i[0];
  IF (info->text EQ NULL)
      buf = (char*) malloc(512);
      if (buf EQ NULL) return(Ierr_action(Itexterr));
      info->text = DEC buf;
      ++ info->textbufblks;
      info->nchars = 1;
  ELSEIF (info->pos GT info->nchars)
    /* Increasing the text */
    IF (info->pos GT info->textbufblks*512)
      /* no space, realloc to get more*/
      buf = (char*)realloc(info->text+1,(info->textbufblks*512)+512);
      if (buf EQ NULL) return(Ierr_action(Imallocerr));
      info->text = DEC buf;
      ++ info->textbufblks;
    ENDIF;
    ++ info->nchars;
  ENDIF;
  info->text[info->pos] = c;
  ++ info->pos;

  return(Iok);
}   /*   Iputchar   */

/*D:Iput_text*

________________________________________________________________

		Iput_text
________________________________________________________________

Name:		Iput_text - insert text at current position of BIFF image text
                field

Syntax:         | #include <xite/biff.h>
		|
                | BiffStatus Iput_text( IMAGE i, char* s );

Description:	Insert the string 's' at current position in the
                textfield of image 'i'. If at end of text, enlarge
		the textfield, otherwise overwrite old text. The
		null-terminator of 's' is not inserted.
		Current character position is set to after new text.

Restrictions:	'i' must be initialized.

See also:       Iget_line(5), Iappend_line(5), Inchars(5), Igetpos(5),
                Isetpos(5), Iputchar(5), bifftext(1)

Return value:	status
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

BiffStatus Iput_text(IMAGE i, char *s)
{
  int status=Iok;

  if (i EQ NULL) return(Ierr_action(Imageerr));
  if (s EQ (char*)0) return(Iok);
  WHILE (*s NE '\0')
    status = Iputchar(i, *(s++));
    EXITIF(status NE Iok);
  ENDWHILE;

  return(status);
}   /*   Iput_text   */

/*D:Iget_line*

________________________________________________________________

		Iget_line
________________________________________________________________

Name:		Iget_line - read a line from BIFF image text field

Syntax:         | #include <xite/biff.h>
		|
                | BiffStatus Iget_line( IMAGE i, char* s );

Description:	Read a whole line from text field of image 'i'
                to the string 's'. The line terminates when:
                a) ^J is encountered, or b) 80 characters are
		read, or c) at end of text. The returned string
		will not contain the eventual ^J, but it will
		contain the terminating null. The character
		position is moved to after the line.

Restrictions:	'i' should be initialized, 's' should be large
		enough (size 81 is always enough).

See also:       Iput_text(5), Inchars(5), Igetpos(5), Isetpos(5), Igetchar(5),
                biffinfo(1)
Return value:	status
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

BiffStatus Iget_line(IMAGE i, char *s)
{
  char c;
  int cp=0;

  if (i EQ NULL) return(Ierr_action(Imageerr));
  LOOP
    if (Iend_of_text(i)) { s[cp] = LF; return(Iok); }
    if (Igetchar(i, &c) NE Iok) return(Ierr_action(Ireaderr));
    if (c EQ LF) { s[cp] = LF; return(Iok); }
    EXITIF (cp EQ 80);
    s[cp ++] = c;
  ENDLOOP;
    
  /* reset pointer since char no 81 wasn't CTRL-J */
  Isetpos(i,Igetpos(i)-1);
  s[80] = LF;

  return(Iok);
}   /*   Iget_line   */

/*D:Iappend_line*

________________________________________________________________

		Iappend_line
________________________________________________________________

Name:		Iappend_line - append a line to BIFF image text field

Syntax:         | #include <xite/biff.h>
		|
                | BiffStatus Iappend_line( IMAGE i, char* s );

Description:	Append a line 's' to the text field of image 'i'.
                The character position is set to end of text, the
		string 's' is written into the text field, and
		finally an end of line mark (CTRL-J) is written
		into the text field. The character position will
		be left at end of text.

Restrictions:	'i' must be initialized.

See also:       Iput_text(5), Iget_line(5), Inchars(5), Isetpos(5), Igetpos(5),
                Ihistory(5)

Return value:	status
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

BiffStatus Iappend_line(IMAGE i, char *s)
{
  int status;

  if (i EQ NULL) return(Ierr_action(Imageerr));
  if ((status = Isetpos(i,Inchars(i)+1)) NE Iok) return(status);
  if ((status = Iput_text(i, s)) NE Iok) return(status);
  /* insert ASCII 10 = line feed (^J) */

  return(Iputchar(i, LF));
}   /*   Iappend_line   */

/*D:Itype_text*

________________________________________________________________

		Itype_text
________________________________________________________________

Name:		Itype_text - send all text in BIFF image textfield to a file

Syntax:         | #include <xite/biff.h>
		|
                | BiffStatus Itype_text( IMAGE i, FILE* fp );

Description:	Send all text in the textfield of image 'i' to the
                file 'fp'. The character position will not be moved.

Restrictions:	'fp' should be a file open for writing.

See also:       Iedit_text(5), Iread_text(5)

Return value:	status
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

BiffStatus Itype_text(IMAGE i, FILE *fp)
{
  int n;
  _Info *info;
  char *t;

  ENTER_FUNCTION_DEBUG("biff.c: Itype_text");

  if (i EQ NULL) {
    LEAVE_FUNCTION_DEBUG("biff.c: Itype_text");
    return(Ierr_action(Imageerr));
  }
  info = (_Info *)i[0];
  t = info->text;
  IF (info->nchars GT 0)
    if (t EQ NULL) {
      LEAVE_FUNCTION_DEBUG("biff.c: Itype_text");
      return(Ierr_action(Itexterr));
    }
    for (n=1; n LE info->nchars; ++ n) putc(t[n],fp);
  ENDIF;

  LEAVE_FUNCTION_DEBUG("biff.c: Itype_text");

  return(Iok);

}   /*   Itype_text   */

/*D:Iedit_text*

________________________________________________________________

		Iedit_text
________________________________________________________________

Name:		Iedit_text - edit the text in the BIFF image textfield

Syntax:         | #include <xite/biff.h>
		|
                | BiffStatus Iedit_text( IMAGE i );

Description:	Let the user edit the text field in image 'i' with
		some text editor. The whole text field is copied to a
		temporary file, an editor is started, and when the
		editor is terminated, the file will be read back into
		the image, and the temporary file will be deleted.
		The character position will be left at end of the text.

		If the "EDITOR" environment variable is set, that editor will
		be started, otherwise "emacs" will be used on a UNIX system,
		and "Notepad" will be used on a Windows NT system.

Restrictions:	'i' must be initialized.

See also:       Itype_text(5), Iread_text(5), Icopy_text(5), bifftext(1)

Return value:	status
Author:		Tor Lønnestad, BLAB, Ifi, UiO
Modified:       Svein Bøe, BLAB, Ifi, UiO
________________________________________________________________

*/

BiffStatus Iedit_text(IMAGE i)
{
#ifdef MSDOS
  system("");
#else /* MSDOS */
  char *tmpfilename;
  char *editor;
  int pid,ic;
  char c;
  FILE *tmpfp;

  if (i EQ NULL) return(Ierr_action(Imageerr));

  /* open a temporary file */ 
  tmpfilename = tempnam(NULL, "Iedt.");
  tmpfp = fopen(tmpfilename,"w");
  if ((Itype_text(i, tmpfp)) NE Iok) return(Itmpfilerr);
  fclose(tmpfp);

  /* start the editor */
  editor = getenv("EDITOR");
  if ((NOT editor) OR (editor[0] EQ '\000'))
# ifndef _WIN32
    editor = "emacs";
# else /* _WIN32 */
    editor = "notepad";
# endif /* _WIN32 */

# ifdef HAVE_FORK
  pid = fork();
  IF (pid EQ 0)
    /* Inside child process, start editor. */

    execlp(editor, editor, tmpfilename, NULL );
    /* execlp should never terminate, this point is only reached if execlp
     * failed. */
    Ierr_action(Itmpfilerr);
    exit(1);
  ELSE
    /* Inside parent process. */
    LOOP
      /* wait for edit process to terminate */
      EXITIF(pid EQ wait(0));
    ENDLOOP;
  ENDIF;
# else
  pid = spawnlp(_P_WAIT, editor, editor, tmpfilename, NULL);
  if (pid == -1) Ierr_action(Iexecerr);
# endif /*_WIN32 */

  /* collect text from file, insert in image */
  tmpfp = fopen(tmpfilename,"r");
  Isetpos(i,1);
  Idel_text(i);
  LOOP
    ic=getc(tmpfp);
    EXITIF (ic EQ EOF);
    c = ic;
    Iputchar(i, c);
  ENDLOOP;

  fclose(tmpfp);
  if (unlink(tmpfilename) NE 0) return(Ierr_action(Itmpfilerr));
#endif /* !MSDOS */

  return(Iok);
}   /*   Iedit_text   */

/*D:Idel_text*

________________________________________________________________

		Idel_text
________________________________________________________________

Name:		Idel_text - delete some text in the BIFF image textfield

Syntax:         | #include <xite/biff.h>
		|
                | BiffStatus Idel_text( IMAGE i );

Description:	Delete all text in the text field of image 'i'
                from character position to end of text. Character
		position will be left at new end of text.

Restrictions:	'i' must be initialized.

See also:       Inchars(5), Isetpos(5), Igetpos(5)

Return value:	status
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

BiffStatus Idel_text(IMAGE i)
{
  _Info *info;

  if (i EQ NULL) return(Ierr_action(Imageerr));
  info = (_Info *)i[0];
  info->nchars = info->pos - 1;

  return(Iok);
}   /*   Idel_text   */

/*D:Iend_of_text*

________________________________________________________________

		Iend_of_text
________________________________________________________________

Name:		Iend_of_text - return TRUE if at end of BIFF image text field

Syntax:         | #include <xite/biff.h>
		|
                | BiffStatus Iend_of_text( IMAGE i );

Description:	Return TRUE if character position in the text
		field of image 'i' is at end of text, that is, if 
		character position equals the number of characters
		plus one. Otherwise return FALSE.

Restrictions:	'i' must be initialized.

See also:       Inchars(5), Igetpos(5), Isetpos(5)

Return value:	TRUE if at end of text, FALSE otherwise. TRUE if
		some kind of error ('i' not initialized).
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

BiffStatus Iend_of_text(IMAGE i)
{
  if (i EQ NULL) { Ierr_action(Imageerr); return(TRUE); }
  return (Igetpos(i) EQ (Inchars(i)+1));
}   /*   Iend_of_text   */

/*D:Icopy_text*

________________________________________________________________

		Icopy_text
________________________________________________________________

Name:		Icopy_text - copy textfield from one BIFF image to another

Syntax:         | #include <xite/biff.h>
		|
                | BiffStatus Icopy_text( IMAGE fromimg,
                |    IMAGE toimg );

Description:	All text in the textfield of image 'fromimg' is 
                appended to the text field of image 'toimg'.
		Character position is set to end of text in both
		images.

Restrictions:	Both images should be initialized.

See also:       Iget_line(5), Iappend_line(5), Iread_text(5), Iend_of_text(5),
                Igetchar(5), Iputchar(5), Icopy_init(5)

Return value:	status
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

BiffStatus Icopy_text(IMAGE fromimg, IMAGE toimg)
{
  char c;

  if ((fromimg EQ NULL) OR (toimg EQ NULL))
    return(Ierr_action(Imageerr));
  if (Isetpos(fromimg,1L) NE Iok) return(Ierr_action(Isetposerr));
  if (Isetpos(toimg,Inchars(toimg)+1) NE Iok)
    return(Ierr_action(Isetposerr));
     
  LOOP
    EXITIF(Iend_of_text(fromimg));
    EXITIF(Igetchar(fromimg, &c) NE Iok);
    EXITIF(Iputchar(toimg, c) NE Iok);
  ENDLOOP;
    
  if (Iend_of_text(fromimg)) return(Iok);

  return(Ierr_action(Itexterr));
}   /*   Icopy_text   */

/*D:Ihistory*

________________________________________________________________

		Ihistory
________________________________________________________________

Name:		Ihistory - append a history line in the BIFF image text field

Syntax:         | #include <xite/biff.h>
		|
                | BiffStatus Ihistory( IMAGE i, char* progname,
                |    char* comment );

Description:	Append a history line in the text field of image 'i'.
		The history line will consist of a) Date and time
		of calling this routine followed by a space, b) the
		'progname' parameter followed by a space, c) the
		'comment' parameter.

		The name of the application should be given as 'progname',
		typically argv[0]. Other arguments to the
		application telling what has been done to the image
		could be specified in the 'comment' parameter.
		One possibility is to use the text returned by the
		argvOptions(3) function	as the 'comment' parameter.

		Character position will be left at end of text.

Restrictions:	'i' must be initialized.

See also:       argvOptions(3), Iappend_line(5), biffinfo(1)

Return value:	status
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

BiffStatus Ihistory(IMAGE i, char *progname, char *comment)
{
  int status;
  time_t now;

  if ((status = Isetpos(i, Inchars(i)+1)) NE Iok) return(status);
  time(&now);
  if ((status = Iput_text(i, ctime(&now))) NE Iok) return(status);
  /* overwrite CR in date */
  if ((status = Isetpos(i, Igetpos(i)-1)) NE Iok) return(status);
  if ((status = Iputchar(i, ' ')) NE Iok) return(status);
  if ((status = Iput_text(i, progname)) NE Iok) return(status);
  if ((status = Iputchar(i, ' ')) NE Iok) return(status);

  return(Iappend_line(i, comment));
}

/*
------------------------------------------------------------------------
---------------------   BLOCK HANDLING PROCEDURES   --------------------
------------------------------------------------------------------------
*/


/*D:Iread_block*

________________________________________________________________

		Iread_block
________________________________________________________________

Name:		Iread_block - read a block from a BIFF image file

Syntax:         | #include <xite/biff.h>
		|
                | BiffStatus Iread_block( IMAGE i,
                |    unsigned char *blk, long blnr );

Description:	Read one block of data (512 bytes) from block field
                number 'blnr' in the file connected to image 'i', to
		data area 'blk'.

Restrictions:	'i' must be connected to an open image and be in
                Ireadonly or Ireadwrite state. 'blnr' should be in
		1..Inblocks(i). 'blk' must be at least 512 bytes long.

See also:       Iwrite_block(5)

Return value:	status
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

BiffStatus Iread_block(IMAGE i, unsigned char *blk, long int blnr)
{
    _Info *info;
    long start;

    ENTER_FUNCTION_DEBUG("biff.c: Iread_block");

    if (i EQ NULL) {
      LEAVE_FUNCTION_DEBUG("biff.c: Iread_block");
      return(Ierr_action(Imageerr));
    }
    info = (_Info *)i[0];

    FPRINTF2("    maybe_pipe: %d\n", info->maybe_pipe);

    if (info->status EQ Ilocal) {
      LEAVE_FUNCTION_DEBUG("biff.c: Iread_block");
      return(Ierr_action(Istatuserr));
    }
    if ((blnr LT 1) OR (blnr GE info->nblocks)) {
      LEAVE_FUNCTION_DEBUG("biff.c: Iread_block");
      return(Ierr_action(Iblocknrerr));
    }
    if (NOT info->maybe_pipe) {
      start = ((info->ninfoblks + info->nbandblks
              + info->ntextblks + (blnr-1)) * 512) - 1;
      if (lseek(info->fd, start, L_SET) NE start) {
	LEAVE_FUNCTION_DEBUG("biff.c: Iread_block");
        return(Ierr_action(Ireaderr));
      }
    }
    if (_Iread_(info->fd, blk, 512) NE 512) {
      LEAVE_FUNCTION_DEBUG("biff.c: Iread_block");
      return(Ierr_action(Ireaderr));
    }

    LEAVE_FUNCTION_DEBUG("biff.c: Iread_block");

    return(Iok);

}   /*   Iwrite_block   */

/*D:Iwrite_block*

________________________________________________________________

		Iwrite_block
________________________________________________________________

Name:		Iwrite_block - write a block to BIFF image file

Syntax:         | #include <xite/biff.h>
		|
                | BiffStatus Iwrite_block( IMAGE i,
                |    unsigned char *blk, long blnr );

Description:	Write one block of data (512 bytes) from  data 
                area 'blk' to block number 'blnr' in file connected
		to image 'i'. 

Restrictions:	'i' must be connected to an open image and be in
                Ireadwrite or Iwrite state. 'blnr' should be in
		1..Inblocks(i)+1. 'blk' must be at least 512 bytes long.

See also:       Iread_block(5)

Return value:	status
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

BiffStatus Iwrite_block(IMAGE i, unsigned char *blk, long int blnr)
{
    _Info *info;
    long start;

    ENTER_FUNCTION_DEBUG("biff.c: Iwrite_block");

    if (i EQ NULL) {
      LEAVE_FUNCTION_DEBUG("biff.c: Iwrite_block");
      return(Ierr_action(Imageerr));
    }
    info = (_Info *)i[0];

    FPRINTF2("    maybe_pipe: %d\n", info->maybe_pipe);

    if ((info->status EQ Ireadonly)
	OR (info->status EQ Ilocal)) {
      LEAVE_FUNCTION_DEBUG("biff.c: Iwrite_block");
      return(Ierr_action(Istatuserr));
    }
    if ((blnr LT 1) OR (blnr GT info->nblocks+1)) {
      LEAVE_FUNCTION_DEBUG("biff.c: Iwrite_block");
      return(Ierr_action(Iblocknrerr));
    }
    if (NOT info->maybe_pipe) {
      start = ((info->ninfoblks + info->nbandblks 
              + info->ntextblks + (blnr-1)) * 512) - 1;
      if (lseek(info->fd,start,L_SET) NE start) {
	LEAVE_FUNCTION_DEBUG("biff.c: Iwrite_block");
        return(Ierr_action(Iwriterr));
      }
    }
    if (_Iwrite_(info->fd,blk,512) NE 512) {
      LEAVE_FUNCTION_DEBUG("biff.c: Iwrite_block");
      return(Ierr_action(Iwriterr));
    }
    if (blnr GT info->nblocks) ++ info->nblocks;

    LEAVE_FUNCTION_DEBUG("biff.c: Iwrite_block");

    return(Iok);

}   /*   Iwrite_block   */


/*
------------------------------------------------------------------------
--------------------   ERROR HANDLING PROCEDURES   ---------------------
------------------------------------------------------------------------
*/


/*D:Ierr_message*

________________________________________________________________

		Ierr_message
________________________________________________________________

Name:		Ierr_message - return BIFF format error message

Syntax:         | #include <xite/biff.h>
		|
                | char *Ierr_message( BiffStatus ernr );

Description:	Return pointer to the error message corresponding
                to the specified error number 'ernr'.
Restrictions:	

See also:       Iset_message(5), Iset_abort(5), Ierr_action(5)

Return value:	Pointer to error message
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

char* Ierr_message(BiffStatus ernr)
{
  char *s;

  switch (ernr) {
    case Iok:         s = "Everything OK"; break;
    case Ibifferr:    s = "Not BIFF-format"; break;
    case Iopenerr:    s = "Couldn't open file"; break;
    case Icloserr:    s = "Couldn't close file"; break;
    case Inotoperr:   s = "File is not open"; break;
    case Ibandnrerr:  s = "Bad band number"; break;
    case Isizerr:     s = "Bad xsize or ysize"; break;
    case Istrtmagerr: s = "Bad x/ystart or x/ymag"; break;
    case Ireaderr:    s = "Couldn't read from file"; break;
    case Iwriterr:    s = "Couldn't write to file"; break;
    case Itexterr:    s = "Couldn't read text field"; break;
    case Iblockerr:   s = "Couldn't read block field"; break;
    case Ipixtyperr:  s = "Bad pixel type"; break;
    case Ilockerr:    s = "Couldn't open file, in use"; break;
    case Istatuserr:  s = "Bad image status"; break;
    case Iblocknrerr: s = "Bad block number"; break;
    case Isetposerr:  s = "Bad <pos> at setpos"; break;
    case Igetcharerr: s = "Bad <pos> at getchar"; break;
    case Infoerr:     s = "Couldn't write image info"; break;
    case Iparnrerr:   s = "Bad parameter number"; break;
    case Imkarrerr:   s = "This error message is not used"; break;
    case Itmpfilerr:  s = "Couldn't handle tmp-file"; break;
    case Iroierr:     s = "Bad roi arguments"; break;
    case Imageerr:    s = "Bad image - maybe nil"; break;
    case Ibanderr:    s = "Bad band - maybe nil"; break;
    case Imallocerr:  s = "Couldn't allocate virtual memory"; break;
    case Inotimplerr: s = "Not implemented"; break;
    case Icolpixerr:  s = "Band is not colortable"; break;
    case Iexecerr:    s = "Could not execute child program"; break;
    default:          s = "No such error message"; break; 
    }
  return(s);
}   /*   Ierr_message   */

/*D:Ierr_action*

________________________________________________________________

		Ierr_action
________________________________________________________________

Name:		Ierr_action - perform error handling

Syntax:         | #include <xite/biff.h>
		|
                | BiffStatus Ierr_action( BiffStatus ernr );

Description:	Perform error handling; send error message to
                stderr if error message status is TRUE (set by
		Iset_message), and exit if abort status is TRUE
		(set by Iset_abort).

See also:       Iset_message(5), Iset_abort(5), Ierr_message(5)

Return value:	The input error number, if any return.

Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

BiffStatus Ierr_action(BiffStatus ernr)
{
  char *s;

  if (Imessage) {
    s = Ierr_message(ernr);
    fputs("BIFF-error: ", stderr);
    fputs(s, stderr);
    fputs("\n", stderr);
  }
  if (Iabort) exit(1);

  return(ernr);
}   /*   Ierr_action   */

/*D:Iset_message*

________________________________________________________________

		Iset_message
________________________________________________________________

Name:		Iset_message - turn BIFF error messaging on/off

Syntax:         | #include <xite/biff.h>
		|
                | int Iset_message( int on );

Description:	Turn error message system on or off, on if 'on'
                is TRUE, off if 'on' is FALSE. Initial status
		value is FALSE.

Return value:	Old error message status.

See also:       Iset_abort(5), Ierr_action(5), Ierr_message(5)
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

int Iset_message(int on)
{
  int old = Imessage;

  if (on) Imessage = TRUE;
  else Imessage = FALSE;

  return(old); 
}   /*   Iset_message   */

/*D:Iset_abort*

________________________________________________________________

		Iset_abort
________________________________________________________________

Name:		Iset_abort - turn BIFF error abort on/off

Syntax:         | #include <xite/biff.h>
		|
                | int Iset_abort( int on );

Description:	Turn error abort system on or off, on if 'on'
                is TRUE, off if 'on' is FALSE.  Initial status
		value is FALSE.

Return value:	Old abort status.

See also:       Iset_message(5), Ierr_action(5), Ierr_message(5)
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

int Iset_abort(int on)
{
  int old = Iabort;

  if (on) Iabort = TRUE;
  else Iabort = FALSE;

  return(old);
}   /*   Iset_abort   */

/*D:Iset_overwrite*

________________________________________________________________

		Iset_overwrite
________________________________________________________________

Name:		Iset_overwrite - turn BIFF image file overwrite on/off

Syntax:         | #include <xite/biff.h>
		|
                | int Iset_overwrite( int on );

Description:	Turn overwrite status on or off, on if 'on' is 
                TRUE, off if 'on' is FALSE. Initial status
		value is TRUE. If you try to write an image to file,
		and the file already exists, the overwrite status
		is examined. If TRUE, the old file is overwritten,
		if FALSE an error situation has occurred.

See also:       Iwrite_band(5), Iwrite_image(5)

Return value:	The old overwrite status
Author:		Tor Lønnestad, BLAB, Ifi, UiO
________________________________________________________________

*/

int Iset_overwrite(int on)
{
  int old = Ioverwrite;

  if (on) Ioverwrite = TRUE;
  else Ioverwrite = FALSE;

  return(old);
}   /*   Iset_abort   */
