
/*
  This header-file is produced automatically (at least in part) by
  makeHeader of BLAB, Ifi, UiO.
*/


/*C*

________________________________________________________________

        color.h
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

#ifndef _COLOR_H_
#define _COLOR_H_

_XITE_CPLUSPLUS_BEGIN

#include <xite/includes.h>
#include <xite/biff.h>
#include XITE_ENDIAN_H

typedef struct _Color_cell {
  unsigned long pixel;
  unsigned short red, green, blue;
  char flags;
  char pad;
} Color_cell;

typedef Color_cell Color_tab[256];

#include <math.h>

/* MostMask is a mask for the most significant byte in a 24-bit pixel-value,
 * regardless of byte-order
 *
 * PseudoMask will mask out everything but the 8 bits representing a pixel-
 * value on an 8-bitplane visual.
 */
#define MostMask     0x0ff0000
#define MiddleMask   0x000ff00
#define LeastMask    0x00000ff

#define RedMask      (host_byte_order() == XITE_BIG_ENDIAN ? \
		      LeastMask : MostMask)
#define GreenMask    (MiddleMask)
#define BlueMask     (host_byte_order() == XITE_BIG_ENDIAN ? \
		      MostMask  : LeastMask)
#define PseudoMask   (LeastMask)

/* Assume that RGBMask consists of consecutive bits. */

#define RGBMask                    (MostMask | MiddleMask | LeastMask)

#define LOG2(a)                    (log(a) / log((double) 2))
#define PrimaryShift(mask)         ((int) LOG2((double) \
					       ((mask) & (~(mask) + 1))))

#define RedShift                   PrimaryShift((RedMask))
#define GreenShift                 PrimaryShift((GreenMask))
#define BlueShift                  PrimaryShift((BlueMask))

#define MostShift                  PrimaryShift(MostMask)
#define MiddleShift                PrimaryShift(MiddleMask)
#define LeastShift                 PrimaryShift(LeastMask)

#define ShiftToRed(p)              ((p) << RedShift)
#define ShiftToGreen(p)            ((p) << GreenShift)
#define ShiftToBlue(p)             ((p) << BlueShift)

#define ShiftToMost(p)             ((p) << MostShift)
#define ShiftToMiddle(p)           ((p) << MiddleShift)
#define ShiftToLeast(p)            ((p) << LeastShift)

#define RgbPixel(p) \
        ((ShiftToRed(p)) | (ShiftToGreen(p)) | (ShiftToBlue(p)))

/* The following macro is to be used for variable initialization. It MUST
 * equal RgbPixel.
 */
#define RgbPixelC(p)               (((p) << 16) | ((p) << 8) | (p))

#define ShiftFromRed(p)            ((p) >> RedShift)
#define ShiftFromGreen(p)          ((p) >> GreenShift)
#define ShiftFromBlue(p)           ((p) >> BlueShift)

#define ShiftFromMost(p)           ((p) >> MostShift)
#define ShiftFromMiddle(p)         ((p) >> MiddleShift)
#define ShiftFromLeast(p)          ((p) >> LeastShift)

#define MostBasePart(p)            ShiftFromMost(  (p) & MostMask)
#define MiddleBasePart(p)          ShiftFromMiddle((p) & MiddleMask)
#define LeastBasePart(p)           ShiftFromLeast( (p) & LeastMask)

#define RedBasePart(p)             ShiftFromRed(   (p) & RedMask)
#define GreenBasePart(p)           ShiftFromGreen( (p) & GreenMask)
#define BlueBasePart(p)            ShiftFromBlue(  (p) & BlueMask)
#define PseudoBasePart(p)          LeastBasePart(p)


extern _XITE_WIN_DLL IMAGE color_create  _XITE_PARAMS(( Color_tab coltab, char *name, int len ));
extern _XITE_WIN_DLL BiffStatus color_read  _XITE_PARAMS(( char * file_name, Color_tab coltab, char **name, int *len ));
extern _XITE_WIN_DLL BiffStatus color_write  _XITE_PARAMS(( char * file_name, Color_tab coltab, char *name, int len ));
extern _XITE_WIN_DLL BiffStatus color_convertfromimg  _XITE_PARAMS(( IMAGE img, Color_tab coltab, char **name, int *len ));
extern _XITE_WIN_DLL int color2ascii _XITE_PARAMS(( Color_cell *coltab, int len, char *filename, char *type, int hex ));
extern _XITE_WIN_DLL void PrintColortable _XITE_PARAMS(( Color_cell *col, int len, int hex, FILE *fileptr ));
extern _XITE_WIN_DLL void read_colormap _XITE_PARAMS(( FILE *file, Color_cell *ctab, int *len ));

_XITE_CPLUSPLUS_END

#endif /* _COLOR_H_ */
