

/*

________________________________________________________________

        blab.h
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

/* ----------------------------------------------------	*/
/*							*/
/*	Module	: blab.h				*/
/*	Author	: Otto Milvang				*/
/*	Data	: 23-Jul-1986				*/
/*	Version	: 1.0					*/
/*							*/
/* ---------------------------------------------------- */

#ifndef         BLABDEF

#define		BLABDEF		1

#define		FALSE		0
#define		TRUE		1

#define		AND		&&
#define		OR		||
#define		NOT		!

#define		EQ		==
#define		NE		!=
#define		LT		<
#define		LE		<=
#define		GT		>
#define		GE		>=

#define		BAND		&
#define		BOR		|
#define		BNOT		~
#define		BXOR		^
#define		LSHF		<<
#define		RSHF		>>

#define		INC		++
#define		DEC		--
#define		MOD		%
#define		SIZEOF		sizeof

#define		BEGIN		{
#define		END		;}

#define		IF(e)		{ if(e) {
#define		THEN		
#define		ELSEIF(e)	;} else if(e) {
#define		ELSE		;} else {
#define		ENDIF		;}}

#define		SWITCH(e)	{ switch(e) {
#define		CASE(e)		case e: {
#define		DEFCASE		default: {
#define		ENDCASE		;} break;
#define		ENDSWITCH	;}}

#define		FOR(e)		{ for(e) {
#define		ENDFOR		;}}

#define		LOOP		{ while(TRUE) {
#define		EXITIF(e)	; if(e) break;
#define		NEXT		; continue;
#define		ENDLOOP		;}}

#define         WHILE(e)        { while(e) {
#define         ENDWHILE        ;}}

#define		PROGRAM	
#define		PROCEDURE	void
#define		FUNCTION	

#define         STRUCT     struct {
#define		ENDSTRUCT  }

#define 	TYPEDEF	   typedef

#define		HIDDEN     static
#define		GLOBAL

/* TYPEDEF STRUCT 
  float re;
  float im;
 ENDSTRUCT complex;
*/

TYPEDEF char *string;


#endif
