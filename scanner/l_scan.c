/* $Id: Scanner.c,v 2.67 2009/10/13 14:13:39 grosch Exp $ */

# define rbool		char
# define rtrue		1
# define rfalse		0
# define StdIn		0

# include "l_scan.h"

# ifndef EXTERN_C_BEGIN
# define EXTERN_C_BEGIN
# define EXTERN_C_END
# endif

EXTERN_C_BEGIN
#    include "l_scanSource.h"
#    include "rSystem.h"
#    include "General.h"
#    include "DynArray.h"
#    include "Position.h"
#    include "Errors.h"
EXTERN_C_END

# include <stdio.h>
# include <stdlib.h>
# include <stddef.h>

# define yyStart(State)	{ yyPreviousStart = yyStartState; yyStartState = State;}
# define yyPrevious	{ yyStateRange s = yyStartState; \
			yyStartState = yyPreviousStart; yyPreviousStart = s; }
# define yyEcho		{ (void) fwrite (l_scan_TokenPtr, sizeof (yytChar), \
			l_scan_TokenLength, stdout); }
# define yyEol(Column)	{ yyLineCount ++; \
			yyLineStart = (yytusChar *) l_scan_TokenPtr + \
			l_scan_TokenLength - 1 - (Column); }
# if l_scan_xxMaxCharacter < 256
#  define output(c)	(void) putchar ((int) c)
# else
#  define output(c)	(void) printf ("%lc", c)
# endif
# define yyColumn(Ptr)	((int) ((Ptr) - (yytChar *) yyLineStart))
# define yyOffset(Ptr)	(yyFileOffset + ((Ptr) - yyChBufferStart2))

# define yytChar	l_scan_xxtChar
# define yytusChar	l_scan_xxtusChar

# define yyDNoState		0
# define yyFirstCh	(yytusChar) '\0'
# define yyEolCh	(yytusChar) '\12'
# define yyEobCh	(yytusChar) '\177'
# define yyDStateCount	51
# define yyTableSize	1183
# define yyEobState	17
# define yyDefaultState	18
# define yyToClassArraySize	0
# define STD	1
# define STRING	3
# define COMMENT	5
# define xxGetWord
# define xxinput

static void yyExit ARGS ((void))
{ rExit (1); }

typedef unsigned short	yyStateRange;
typedef struct { yyStateRange yyCheck, yyNext; } yyCombType;

	yytChar *	l_scan_TokenPtr	;
	int		l_scan_TokenLength	;
	l_scan_tScanAttribute	l_scan_Attribute	;
	void		(* l_scan_Exit)	ARGS ((void)) = yyExit;

static	void		yyInitialize	ARGS ((void));
static	void		yyErrorMessage	ARGS ((int yyErrorCode));
static	yytChar		input		ARGS ((void));
static	void		unput		ARGS ((yytChar));
static	void		yyLess		ARGS ((int));

static	yyCombType	yyComb		[yyTableSize   + 1] = {
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   1,   20}, 
{   1,   19}, {   0,    0}, {   3,   21}, {   3,   46}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   1,   22}, {   0,    0}, {   1,   51}, 
{   3,   23}, {   0,    0}, {   3,   50}, {   5,   20}, {   5,   19}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   1,   25}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   1,   15}, {   1,    9}, {   1,    9}, 
{   1,    9}, {   1,    9}, {   1,    9}, {   1,    9}, {   1,    9}, 
{   1,    9}, {   1,    9}, {   1,    9}, {  15,   28}, {   0,    0}, 
{   0,    0}, {   5,   24}, {   0,    0}, {  15,   12}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   5,   26}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   9,   11}, {   5,    7}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   3,   47}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   1,   37}, {   1,   29}, {   0,    0}, {   0,    0}, 
{   1,   44}, {   0,    0}, {   0,    0}, {   0,    0}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   1,   32}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {  16,   17}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {  26,   27}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {  29,   30}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  30,   31}, {  32,   33}, {  33,   34}, {  34,   35}, {  35,   36}, 
{  37,   38}, {  38,   39}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  39,   40}, {  40,   41}, 
{  41,   42}, {  42,   43}, {  44,   45}, {   0,    0}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{   0,    0}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {   0,    0}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {   0,    0}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  13,   13}, {  13,   13}, {  13,   13}, {  13,   13}, 
{  13,   13}, {  13,   13}, {  13,   13}, {  13,   13}, {  13,   13}, 
{  13,   13}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{   0,    0}, {   0,    0}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {   0,    0}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {   0,    0}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {   0,    0}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, {  14,   14}, 
{  14,   14}, {  14,   14}, {  47,   48}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {  47,   49}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
};
static	yyCombType *	yyBasePtr	[yyDStateCount + 1] = { 0,
& yyComb [   0], & yyComb [   0], & yyComb [   3], & yyComb [   0], 
& yyComb [  29], & yyComb [   0], & yyComb [   0], & yyComb [ 109], 
& yyComb [  29], & yyComb [ 317], & yyComb [   0], & yyComb [ 440], 
& yyComb [ 648], & yyComb [ 706], & yyComb [  16], & yyComb [  16], 
& yyComb [   0], & yyComb [   0], & yyComb [   0], & yyComb [   0], 
& yyComb [   0], & yyComb [   0], & yyComb [   0], & yyComb [   0], 
& yyComb [   0], & yyComb [ 154], & yyComb [   0], & yyComb [   0], 
& yyComb [ 125], & yyComb [ 261], & yyComb [   0], & yyComb [ 272], 
& yyComb [ 272], & yyComb [ 270], & yyComb [ 278], & yyComb [   0], 
& yyComb [ 272], & yyComb [ 266], & yyComb [ 307], & yyComb [ 377], 
& yyComb [ 305], & yyComb [ 309], & yyComb [   0], & yyComb [ 310], 
& yyComb [   0], & yyComb [   0], & yyComb [ 928], & yyComb [   0], 
& yyComb [   0], & yyComb [   0], & yyComb [   0], 
};
static	yyStateRange	yyDefault	[yyDStateCount + 1] = { 0,
   10,     1,     8,     3,    14,     5,    16,    16,    16,    16, 
   13,    15,    16,    16,    16,     0,     0,     0,    16,     0, 
    8,     0,     8,    14,     0,     7,     0,     0,    10,    10, 
   10,    10,    10,    10,    10,    10,    10,    10,    10,    10, 
   16,    16,     0,    10,    10,    19,    16,     0,     0,     0, 
    0, 
};
static	yyStateRange	yyEobTrans	[yyDStateCount + 1] = { 0,
    0,     0,     8,     8,    14,    14,     0,     8,     0,     0, 
    0,    12,     0,    14,     0,     0,     0,     0,     0,     0, 
    8,     0,     8,    14,     0,     0,     0,     0,     0,     0, 
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0, 
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0, 
    0, 
};

# if l_scan_xxMaxCharacter < 256
#  define yyGetLine	l_scan_GetLine
# else
#  define yyGetLine	l_scan_GetWLine
# endif

# if yyToClassArraySize == 0
#  define yyToClass(x) (x)
# else
typedef	unsigned short	yytCharClass;

static	yytCharClass	yyToClassArray	[yyToClassArraySize] = {
};
#  if l_scan_xxMaxCharacter < yyToClassArraySize
#   define yyToClass(x) (yyToClassArray [x])
#  else
#   define yyToClass(x) \
	((x) < yyToClassArraySize ? yyToClassArray [x] : yyToClassFunction (x))

static	yytusChar	yyToClassUpb	[yyToClassRangeSize] = {
};
static	yytCharClass	yyToClassClass	[yyToClassRangeSize] = {
};

static yytCharClass yyToClassFunction
# ifdef HAVE_ARGS
   (yytusChar yyCh)
# else
   (yyCh) yytusChar yyCh;
# endif
{
   register int yyLower = 1, yyUpper = yyToClassRangeSize;
   while (yyUpper - yyLower > 1) {			/* binary search */
      register int yyMiddle = (yyLower + yyUpper) / 2;
      if (yyToClassUpb [yyMiddle] < yyCh)
	 yyLower = yyMiddle;
      else
	 yyUpper = yyMiddle;
   }
   if (yyToClassUpb [yyLower - 1] < yyCh && yyCh <= yyToClassUpb [yyLower])
      return yyToClassClass [yyLower];
   else
      return yyToClassClass [yyUpper];
}

#  endif
# endif

static	yyStateRange	yyStartState	= STD;
static	yyStateRange	yyPreviousStart	= STD;
static	int		yySourceFile	= StdIn;
static	rbool		yyEof		= rfalse;
static	long		yyBytesRead	= 0;
static	long		yyFileOffset	= 0;
static	unsigned int	yyLineCount	= 1;
static	yytusChar *	yyLineStart	;
static	yytChar *	yyChBufferStart2;

					/* Start State Stack: StStSt	*/

# if defined xxyyPush | defined xxyyPop
# define		yyInitStStStackSize	16

static	yyStateRange *	yyStStStackPtr	;
static	unsigned long	yyStStStackSize	= 0;
static	unsigned int	yyStStStackIdx	= 0;
# endif

# ifdef xxyyPush
static void yyPush
# ifdef HAVE_ARGS
   (yyStateRange yyState)
# else
   (yyState) yyStateRange yyState;
# endif
{
   if (yyStStStackIdx == yyStStStackSize) {
      if (yyStStStackSize == 0) {
	 yyStStStackSize = yyInitStStStackSize;
	 MakeArray ((char * *) & yyStStStackPtr, & yyStStStackSize,
			(unsigned long) sizeof (yyStateRange));
      } else {
	 ExtendArray ((char * *) & yyStStStackPtr, & yyStStStackSize,
			(unsigned long) sizeof (yyStateRange));
      }
      if (yyStStStackPtr == NULL) yyErrorMessage (xxScannerOutOfMemory);
   }
   yyStStStackPtr [yyStStStackIdx ++] = yyStartState;
   yyStart (yyState);
}
# endif

# ifdef xxyyPop
static void yyPop ARGS ((void))
{
   yyPreviousStart = yyStartState;
   if (yyStStStackIdx > 0)
      yyStartState = yyStStStackPtr [-- yyStStStackIdx];
   else
      yyErrorMessage (xxStartStackUnderflow);
}
# endif

static void yyTab1	ARGS ((int yya));

# define yyTab		yyTab1 (0)
# define yyTab2(a,b)	yyTab1 (a)

/* line 55 "l.rex" */

  # include <stdlib.h>
  # include "rString.h"

/* line 464 "l_scan.c" */

# ifndef yySetPosition
# define yySetPosition l_scan_Attribute.Position.Line = yyLineCount; \
l_scan_Attribute.Position.Column = (int) ((yytusChar *) l_scan_TokenPtr - yyLineStart);
# endif

# undef yyTab
# undef yyTab2

# ifndef yyInitBufferSize
# define yyInitBufferSize	1024 * 8 + 256
# endif
# ifndef yyInitFileStackSize
# define yyInitFileStackSize	8
# endif
# ifndef yyTabSpace
# define yyTabSpace		8
# endif

static void yyTab1
# ifdef HAVE_ARGS
   (int yya)
# else
   (yya) int yya;
# endif
   { yyLineStart -= (yyTabSpace - 1 - ((yytusChar *) l_scan_TokenPtr -
	yyLineStart + yya - 1)) & (yyTabSpace - 1); }

# define yyTab		yyLineStart -= (yyTabSpace - 1 - \
((yytusChar *) l_scan_TokenPtr - yyLineStart - 1)) & (yyTabSpace - 1)
# define yyTab1(a)	yyLineStart -= (yyTabSpace - 1 - \
((yytusChar *) l_scan_TokenPtr - yyLineStart + (a) - 1)) & (yyTabSpace - 1)
# define yyTab2(a,b)	yyLineStart -= (yyTabSpace - 1 - \
((yytusChar *) l_scan_TokenPtr - yyLineStart + (a) - 1)) & (yyTabSpace - 1)

# ifndef EBCDIC
#  if l_scan_xxMaxCharacter < 256
#   include <ctype.h>
#   define yyToUpper(x)	toupper (x)
#   define yyToLower(x)	tolower (x)
#  else
#   include <wctype.h>
#   define yyToUpper(x)	towupper (x)
#   define yyToLower(x)	towlower (x)
#  endif
# else
#  define yyToLower(x)	yyToLowerArray [x]
#  define yyToUpper(x)	yyToUpperArray [x]

# ifdef xxGetLower
static	yytusChar	yyToLowerArray	[] = {
'\x00', '\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x07',
'\x08', '\x09', '\x0A', '\x0B', '\x0C', '\x0D', '\x0E', '\x0F',
'\x10', '\x11', '\x12', '\x13', '\x14', '\x15', '\x16', '\x17',
'\x18', '\x19', '\x1A', '\x1B', '\x1C', '\x1D', '\x1E', '\x1F',
'\x20', '\x21', '\x22', '\x23', '\x24', '\x25', '\x26', '\x27',
'\x28', '\x29', '\x2A', '\x2B', '\x2C', '\x2D', '\x2E', '\x2F',
'\x30', '\x31', '\x32', '\x33', '\x34', '\x35', '\x36', '\x37',
'\x38', '\x39', '\x3A', '\x3B', '\x3C', '\x3D', '\x3E', '\x3F',
'\x40', '\x41', '\x42', '\x43', '\x44', '\x45', '\x46', '\x47',
'\x48', '\x49', '\x4A', '\x4B', '\x4C', '\x4D', '\x4E', '\x4F',
'\x50', '\x51', '\x52', '\x53', '\x54', '\x55', '\x56', '\x57',
'\x58', '\x59', '\x5A', '\x5B', '\x5C', '\x5D', '\x5E', '\x5F',
'\x60', '\x61', '\x62', '\x63', '\x64', '\x65', '\x66', '\x67',
'\x68', '\x69', '\x6A', '\x6B', '\x6C', '\x6D', '\x6E', '\x6F',
'\x70', '\x71', '\x72', '\x73', '\x74', '\x75', '\x76', '\x77',
'\x78', '\x79', '\x7A', '\x7B', '\x7C', '\x7D', '\x7E', '\x7F',
'\x80',    'a',    'b',    'c',    'd',    'e',    'f',    'g',
   'h',    'i', '\x8A', '\x8B', '\x8C', '\x8D', '\x8E', '\x8F',
'\x90',    'j',    'k',    'l',    'm',    'n',    'o',    'p',
   'q',    'r', '\x9A', '\x9B', '\x9C', '\x9D', '\x9E', '\x9F',
'\xA0', '\xA1',    's',    't',   'u',     'v',    'w',    'x',
   'y',    'z', '\xAA', '\xAB', '\xAC', '\xAD', '\xAE', '\xAF',
'\xB0', '\xB1', '\xB2', '\xB3', '\xB4', '\xB5', '\xB6', '\xB7',
'\xB8', '\xB9', '\xBA', '\xBB', '\xBC', '\xBD', '\xBE', '\xBF',
'\xC0',    'a',    'b',    'c',    'd',    'e',    'f',    'g',
   'h',    'i', '\xCA', '\xCB', '\xCC', '\xCD', '\xCE', '\xCF',
'\xD0',    'j',    'k',    'l',    'm',    'n',    'o',    'p',
   'q',    'r', '\xDA', '\xDB', '\xDC', '\xDD', '\xDE', '\xDF',
'\xE0', '\xE1',    's',    't',    'u',    'v',    'w',    'x',
   'y',    'z', '\xEA', '\xEB', '\xEC', '\xED', '\xEE', '\xEF',
'\xF0', '\xF1', '\xF2', '\xF3', '\xF4', '\xF5', '\xF6', '\xF7',
'\xF8', '\xF9', '\xFA', '\xFB', '\xFC', '\xFD', '\xFE', '\xFF'
};
# endif

# ifdef xxGetUpper
static	yytusChar	yyToUpperArray	[] = {
'\x00', '\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x07',
'\x08', '\x09', '\x0A', '\x0B', '\x0C', '\x0D', '\x0E', '\x0F',
'\x10', '\x11', '\x12', '\x13', '\x14', '\x15', '\x16', '\x17',
'\x18', '\x19', '\x1A', '\x1B', '\x1C', '\x1D', '\x1E', '\x1F',
'\x20', '\x21', '\x22', '\x23', '\x24', '\x25', '\x26', '\x27',
'\x28', '\x29', '\x2A', '\x2B', '\x2C', '\x2D', '\x2E', '\x2F',
'\x30', '\x31', '\x32', '\x33', '\x34', '\x35', '\x36', '\x37',
'\x38', '\x39', '\x3A', '\x3B', '\x3C', '\x3D', '\x3E', '\x3F',
'\x40', '\x41', '\x42', '\x43', '\x44', '\x45', '\x46', '\x47',
'\x48', '\x49', '\x4A', '\x4B', '\x4C', '\x4D', '\x4E', '\x4F',
'\x50', '\x51', '\x52', '\x53', '\x54', '\x55', '\x56', '\x57',
'\x58', '\x59', '\x5A', '\x5B', '\x5C', '\x5D', '\x5E', '\x5F',
'\x60', '\x61', '\x62', '\x63', '\x64', '\x65', '\x66', '\x67',
'\x68', '\x69', '\x6A', '\x6B', '\x6C', '\x6D', '\x6E', '\x6F',
'\x70', '\x71', '\x72', '\x73', '\x74', '\x75', '\x76', '\x77',
'\x78', '\x79', '\x7A', '\x7B', '\x7C', '\x7D', '\x7E', '\x7F',
'\x80',    'A',    'B',    'C',    'D',    'E',    'F',    'G',
   'H',    'I', '\x8A', '\x8B', '\x8C', '\x8D', '\x8E', '\x8F',
'\x90',    'J',    'K',    'L',    'M',    'N',    'O',    'P',
   'Q',    'R', '\x9A', '\x9B', '\x9C', '\x9D', '\x9E', '\x9F',
'\xA0', '\xA1',    'S',    'T',    'U',    'V',    'W',    'X',
   'Y',    'Z', '\xAA', '\xAB', '\xAC', '\xAD', '\xAE', '\xAF',
'\xB0', '\xB1', '\xB2', '\xB3', '\xB4', '\xB5', '\xB6', '\xB7',
'\xB8', '\xB9', '\xBA', '\xBB', '\xBC', '\xBD', '\xBE', '\xBF',
'\xC0',    'A',    'B',    'C',    'D',    'E',    'F',    'G',
   'H',    'I', '\xCA', '\xCB', '\xCC', '\xCD', '\xCE', '\xCF',
'\xD0',    'J',    'K',    'L',    'M',    'N',    'O',    'P',
   'Q',    'R', '\xDA', '\xDB', '\xDC', '\xDD', '\xDE', '\xDF',
'\xE0', '\xE1',    'S',    'T',    'U',    'V',    'W',    'X',
   'Y',    'Z', '\xEA', '\xEB', '\xEC', '\xED', '\xEE', '\xEF',
'\xF0', '\xF1', '\xF2', '\xF3', '\xF4', '\xF5', '\xF6', '\xF7',
'\xF8', '\xF9', '\xFA', '\xFB', '\xFC', '\xFD', '\xFE', '\xFF'
};
# endif

# endif

static	yyStateRange	yyInitStateStack [4] = { yyDefaultState };
static	yyStateRange *	yyStateStack	= yyInitStateStack;
static	unsigned long	yyStateStackSize= 0;

static	yytusChar	yyInitChBuffer [] = {
   '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
   '\0', '\0', '\0', '\0', '\0', '\0', '\0', yyEolCh, yyEobCh, '\0', };
static	yytusChar *	yyChBufferPtr	= yyInitChBuffer;
static	unsigned long	yyChBufferSize	= 0;
static	yytusChar *	yyChBufferStart	= & yyInitChBuffer [16];
static	yytusChar *	yyChBufferIndex	= & yyInitChBuffer [16];

# if yyInitFileStackSize != 0
typedef	struct {
	int		yySourceFile	;
	rbool		yyEof		;
	yytusChar *	yyChBufferPtr	;
	yytusChar *	yyChBufferStart	;
	unsigned long	yyChBufferSize	;
	yytusChar *	yyChBufferIndex	;
	long		yyBytesRead	;
	long		yyFileOffset	;
	unsigned int	yyLineCount	;
	yytusChar *	yyLineStart	;
	}		yytFileStack	;

static	yytFileStack *	yyFileStack	;
static	unsigned long	yyFileStackSize	= 0;
static	yytFileStack *	yyFileStackPtr	;
# endif

int l_scan_GetToken ARGS ((void))
{
   register	yyStateRange	yyState;
   register	yyStateRange *	yyStatePtr;
   register	yytusChar *	yyChBufferIndexReg;
   register	yyCombType * *	yyBasePtrReg = yyBasePtr;
/* line 60 "l.rex" */

  # define MAX_STRING_LENGTH 1024
  tPosition str_pos;
  char string[MAX_STRING_LENGTH];
  int str_len;

/* line 634 "l_scan.c" */

yyBegin:
   yyState		= yyStartState;		/* initialize */
   yyStatePtr		= & yyStateStack [1];
   yyChBufferIndexReg	= yyChBufferIndex;
   l_scan_TokenPtr		= (yytChar *) yyChBufferIndexReg;

   /* ASSERT yyChBuffer [yyChBufferIndex] == first character */

yyContinue:		/* continue after sentinel or skipping blanks */
   for (;;) {		/* execute as many state transitions as possible */
			/* determine next state and get next character */
      register yyCombType * yyTablePtr =
		      yyBasePtrReg [yyState] + yyToClass (* yyChBufferIndexReg);
      if (yyTablePtr->yyCheck == yyState) {
	 yyState = yyTablePtr->yyNext;		/* determine next state */
	 * yyStatePtr ++ = yyState;		/* push state */
	 yyChBufferIndexReg ++;			/* get next character */
	 goto yyContinue;
      }
      if ((yyState = yyDefault [yyState]) == yyDNoState) break;
   }

   for (;;) {				/* search for last final state */
      l_scan_TokenLength =
	    (int) (yyChBufferIndexReg - (yytusChar *) l_scan_TokenPtr);
      yyChBufferIndex = yyChBufferIndexReg;
switch (* -- yyStatePtr) {
case 9:;
yySetPosition
/* line 103 "l.rex" */
{
  l_scan_Attribute.int_const.Value = atol (l_scan_TokenPtr);
  return tok_int_const;

/* line 670 "l_scan.c" */
} goto yyBegin;
case 13:;
yySetPosition
/* line 108 "l.rex" */
{
  l_scan_Attribute.float_const.Value = PutString(l_scan_TokenPtr, l_scan_TokenLength);
  return tok_float_const;

/* line 679 "l_scan.c" */
} goto yyBegin;
case 51:;
yySetPosition
/* line 116 "l.rex" */
{
  yyStart(STRING);
  str_len = 0; /* reset the string length counter */
  str_pos = l_scan_Attribute.Position;

/* line 689 "l_scan.c" */
} goto yyBegin;
case 50:;
yySetPosition
/* line 123 "l.rex" */
{
  yyStart(STD);
  l_scan_Attribute.string_const.Value = PutString(string, str_len);
  l_scan_Attribute.string_const.Pos = str_pos;
  return tok_string_const;

/* line 700 "l_scan.c" */
} goto yyBegin;
case 47:;
yySetPosition
/* line 130 "l.rex" */
{
  string[str_len++] = '\\';

/* line 708 "l_scan.c" */
} goto yyBegin;
case 49:;
yySetPosition
/* line 134 "l.rex" */
{
  string[str_len++] = '\n';

/* line 716 "l_scan.c" */
} goto yyBegin;
case 48:;
yySetPosition
/* line 138 "l.rex" */
{
  string[str_len++] = '"';

/* line 724 "l_scan.c" */
} goto yyBegin;
case 8:;
case 21:;
case 23:;
yySetPosition
/* line 142 "l.rex" */
{
  str_len = l_scan_GetWord(&string[str_len]);

/* line 734 "l_scan.c" */
} goto yyBegin;
case 46:;
yySetPosition
/* line 146 "l.rex" */
{
  Message("Line break in string literal", xxError, l_scan_Attribute.Position);

/* line 742 "l_scan.c" */
} goto yyBegin;
case 45:;
yySetPosition
/* line 152 "l.rex" */
{
  return tok_keyword_if;

/* line 750 "l_scan.c" */
} goto yyBegin;
case 40:;
yySetPosition
/* line 156 "l.rex" */
{
  return tok_keyword_else;

/* line 758 "l_scan.c" */
} goto yyBegin;
case 43:;
yySetPosition
/* line 160 "l.rex" */
{
  return tok_keyword_elseif;

/* line 766 "l_scan.c" */
} goto yyBegin;
case 36:;
yySetPosition
/* line 164 "l.rex" */
{
  return tok_keyword_while;

/* line 774 "l_scan.c" */
} goto yyBegin;
case 31:;
yySetPosition
/* line 168 "l.rex" */
{
  return tok_keyword_for;

/* line 782 "l_scan.c" */
} goto yyBegin;
case 12:;
/* line 173 "l.rex" */
{
/* line 787 "l_scan.c" */
} goto yyBegin;
case 28:;
/* line 176 "l.rex" */
{
  yyStart(COMMENT);

/* line 794 "l_scan.c" */
} goto yyBegin;
case 27:;
/* line 180 "l.rex" */
{
  yyStart(STD);

/* line 801 "l_scan.c" */
} goto yyBegin;
case 7:;
case 14:;
case 24:;
case 26:;
/* line 184 "l.rex" */
{
/* line 809 "l_scan.c" */
} goto yyBegin;
case 25:;
yySetPosition
/* line 188 "l.rex" */
{
  return tok_op_add;

/* line 817 "l_scan.c" */
} goto yyBegin;
case 10:;
case 29:;
case 30:;
case 32:;
case 33:;
case 34:;
case 35:;
case 37:;
case 38:;
case 39:;
case 44:;
yySetPosition
/* line 194 "l.rex" */
{
  l_scan_Attribute.identifier.Identifier = MakeIdent(l_scan_TokenPtr, l_scan_TokenLength);
  return tok_ident;

/* line 836 "l_scan.c" */
} goto yyBegin;
case 22:;
{/* BlankAction */
while (* yyChBufferIndexReg ++ == ' ') ;
l_scan_TokenPtr = (yytChar *) -- yyChBufferIndexReg;
yyState = yyStartState;
yyStatePtr = & yyStateStack [1];
goto yyContinue;
/* line 845 "l_scan.c" */
} goto yyBegin;
case 20:;
{/* TabAction */
yyTab;
/* line 850 "l_scan.c" */
} goto yyBegin;
case 19:;
{/* EolAction */
yyEol (0);
/* line 855 "l_scan.c" */
} goto yyBegin;
case 1:;
case 2:;
case 3:;
case 4:;
case 5:;
case 6:;
case 11:;
case 15:;
case 16:;
case 41:;
case 42:;
	 /* non final states */
	 yyChBufferIndexReg --;			/* return character */
	 break;

case 18:
	 yySetPosition
      l_scan_TokenLength   = 1;
	 yyChBufferIndex = ++ yyChBufferIndexReg;
	 {
/* line 67 "l.rex" */

  /* What happens if no scanner rule matches the input */
  MessageI ("Illegal character", xxError, l_scan_Attribute.Position, xxCharacter, l_scan_TokenPtr);

/* line 882 "l_scan.c" */
	 }
	 goto yyBegin;

      case yyDNoState:
	 goto yyBegin;

case 17:
	 yyChBufferIndex = -- yyChBufferIndexReg; /* undo last state transit */
	 if (-- l_scan_TokenLength == 0) {		/* get previous state */
	    yyState = yyStartState;
	 } else {
	    yyState = * (yyStatePtr - 1);
	 }

	 if (yyChBufferIndex != & yyChBufferStart [yyBytesRead]) {
					/* end of buffer sentinel in buffer */
	    if ((yyState = yyEobTrans [yyState]) == yyDNoState) continue;
	    yyChBufferIndexReg ++;
	    * yyStatePtr ++ = yyState;		/* push state */
	    goto yyContinue;
	 }
						/* end of buffer reached */
	 if (! yyEof) {
	    register yytChar * yySource;
	    register yytChar * yyTarget;
	    unsigned long yyChBufferFree;

	    if (yyChBufferSize == 0) {
	       yyStateRange * yyOldStateStack = yyStateStack;
	       yyInitialize ();
	       yyStatePtr += yyStateStack - yyOldStateStack;
	       yyChBufferIndexReg = yyChBufferIndex;
	    }
	    yySource = l_scan_TokenPtr - 1;
	    yyTarget = (yytChar *) & yyChBufferPtr
		[(yyMaxAlign - 1 - l_scan_TokenLength) & (yyMaxAlign - 1)];
	    yyChBufferFree = Exp2 (Log2 (yyChBufferSize - 4 -
		yyMaxAlign - l_scan_TokenLength));
		/* copy initial part of token in front of the input buffer */
	    if (yySource > yyTarget) {
	       l_scan_TokenPtr = yyTarget + 1;
	       do * yyTarget ++ = * yySource ++;
	       while (yySource < (yytChar *) yyChBufferIndexReg);
	       yyLineStart += (yytusChar *) yyTarget - yyChBufferStart -
				yyBytesRead;
	       yyChBufferStart = (yytusChar *) yyTarget;
	    } else {
	       yyChBufferStart = yyChBufferIndexReg;
	    }
	    yyChBufferStart2 = (yytChar *) yyChBufferStart;
						/* extend buffer if necessary */
	    if (yyChBufferFree < yyChBufferSize >> 3 /* / 8 */ ) {
	       register ptrdiff_t yyDelta;
	       register yytusChar * yyOldChBufferPtr = yyChBufferPtr;
	       ExtendArray ((char * *) & yyChBufferPtr, & yyChBufferSize,
				(unsigned long) sizeof (yytChar));
	       if (yyChBufferPtr == NULL) yyErrorMessage (xxScannerOutOfMemory);
	       yyDelta = yyChBufferPtr - yyOldChBufferPtr;
	       yyChBufferStart	+= yyDelta;
	       yyLineStart	+= yyDelta;
	       l_scan_TokenPtr	+= yyDelta;
	       yyChBufferStart2	 = (yytChar *) yyChBufferStart;
	       yyChBufferFree = Exp2 (Log2 (yyChBufferSize - 4 -
			yyMaxAlign - l_scan_TokenLength));
	       if (yyStateStackSize < yyChBufferSize) {
		  yyStateRange * yyOldStateStack = yyStateStack;
		  ExtendArray ((char * *) & yyStateStack, & yyStateStackSize,
				   (unsigned long) sizeof (yyStateRange));
		  if (yyStateStack == NULL)
		     yyErrorMessage (xxScannerOutOfMemory);
		  yyStatePtr	+= yyStateStack - yyOldStateStack;
	       }
	    }
						/* read buffer and restart */
	    yyChBufferIndex = yyChBufferIndexReg = yyChBufferStart;
	    yyFileOffset += yyBytesRead;
	    yyBytesRead = yyGetLine (yySourceFile, (yytChar *) yyChBufferIndex,
	       (int) yyChBufferFree);
	    if (yyBytesRead <= 0) { yyBytesRead = 0; yyEof = rtrue; }
	    yyChBufferStart [yyBytesRead    ] = yyEobCh;
	    yyChBufferStart [yyBytesRead + 1] = '\0';
	    goto yyContinue;
	 }

	 if (l_scan_TokenLength == 0) {		/* end of file reached */
	    if (yyChBufferSize == 0) return l_scan_EofToken;
	    yySetPosition
	    l_scan_CloseFile ();
# if yyInitFileStackSize != 0
	    if (yyFileStackPtr == yyFileStack) {
/* line 72 "l.rex" */

  /* What should be done if the end-of-input-file has been reached? */
  switch (yyStartState) {
  case STD:
    /* ok */
    break;
  case STRING:
    Message("String literal not closed before EOF", xxError, l_scan_Attribute.Position);
    break;
  case COMMENT: 
    Message("Comment not closed before EOF", xxError, l_scan_Attribute.Position);
    break;
  default:
    Message("OOPS: that should not happen!!", xxFatal, l_scan_Attribute.Position);
    break;
  }

  /* implicit: return the EofToken */

/* line 993 "l_scan.c" */
	    }
	    if (yyFileStackPtr == yyFileStack) {
	       l_scan_ResetScanner ();
	       return l_scan_EofToken;
	    }
	    goto yyBegin;
# else
/* line 72 "l.rex" */

  /* What should be done if the end-of-input-file has been reached? */
  switch (yyStartState) {
  case STD:
    /* ok */
    break;
  case STRING:
    Message("String literal not closed before EOF", xxError, l_scan_Attribute.Position);
    break;
  case COMMENT: 
    Message("Comment not closed before EOF", xxError, l_scan_Attribute.Position);
    break;
  default:
    Message("OOPS: that should not happen!!", xxFatal, l_scan_Attribute.Position);
    break;
  }

  /* implicit: return the EofToken */

/* line 1021 "l_scan.c" */
	    l_scan_ResetScanner ();
	    return l_scan_EofToken;
# endif
	 }
	 break;

      default:
	 yyErrorMessage (xxScannerInternalError);
      }
   }
}

static void yyInitialize ARGS ((void))
   {
      if (yyStateStackSize == 0) {
	 yyStateStackSize = yyInitBufferSize;
	 MakeArray ((char * *) & yyStateStack, & yyStateStackSize,
		       (unsigned long) sizeof (yyStateRange));
	 if (yyStateStack == NULL) yyErrorMessage (xxScannerOutOfMemory);
	 yyStateStack [0] = yyDefaultState;
      }
# if yyInitFileStackSize != 0
      if (yyFileStackSize == 0) {
	 yyFileStackSize = yyInitFileStackSize;
	 MakeArray ((char * *) & yyFileStack, & yyFileStackSize,
			(unsigned long) sizeof (yytFileStack));
	 if (yyFileStack == NULL) yyErrorMessage (xxScannerOutOfMemory);
	 yyFileStackPtr = yyFileStack;
      }

      if (yyFileStackPtr >= yyFileStack + yyFileStackSize - 1) {
	 ptrdiff_t yyyFileStackPtr = yyFileStackPtr - yyFileStack;
	 ExtendArray ((char * *) & yyFileStack, & yyFileStackSize,
			   (unsigned long) sizeof (yytFileStack));
	 if (yyFileStack == NULL) yyErrorMessage (xxScannerOutOfMemory);
	 yyFileStackPtr = yyFileStack + yyyFileStackPtr;
      }
      yyFileStackPtr ++;			/* push file */
      yyFileStackPtr->yySourceFile	= yySourceFile		;
      yyFileStackPtr->yyEof		= yyEof			;
      yyFileStackPtr->yyChBufferPtr	= yyChBufferPtr		;
      yyFileStackPtr->yyChBufferStart	= yyChBufferStart	;
      yyFileStackPtr->yyChBufferSize	= yyChBufferSize	;
      yyFileStackPtr->yyChBufferIndex	= yyChBufferIndex	;
      yyFileStackPtr->yyBytesRead	= yyBytesRead		;
      yyFileStackPtr->yyFileOffset	= yyFileOffset		;
      yyFileStackPtr->yyLineCount	= yyLineCount		;
      yyFileStackPtr->yyLineStart	= yyLineStart		;
# endif
						/* initialize file state */
      yyChBufferSize	   = yyInitBufferSize;
      MakeArray ((char * *) & yyChBufferPtr, & yyChBufferSize,
			(unsigned long) sizeof (yytChar));
      if (yyChBufferPtr == NULL) yyErrorMessage (xxScannerOutOfMemory);
      yyChBufferStart	   = & yyChBufferPtr [yyMaxAlign];
      yyChBufferStart2	   = (yytChar *) yyChBufferStart;
      yyChBufferStart [-1] = yyEolCh;		/* begin of line indicator */
      yyChBufferStart [ 0] = yyEobCh;		/* end of buffer sentinel */
      yyChBufferStart [ 1] = '\0';
      yyChBufferIndex	   = yyChBufferStart;
      l_scan_TokenPtr	   = (yytChar *) yyChBufferStart;
      yyEof		   = rfalse;
      yyBytesRead	   = 0;
      yyFileOffset	   = 0;
      yyLineCount	   = 1;
      yyLineStart	   = & yyChBufferStart [-1];
# ifdef HAVE_FILE_NAME
      if (l_scan_Attribute.Position.FileName == 0)
	 l_scan_Attribute.Position.FileName = 1; /* NoIdent */
# endif
   }

void l_scan_BeginFile
# ifdef HAVE_ARGS
   (char * yyFileName)
# else
   (yyFileName) char * yyFileName;
# endif
   {
      yyInitialize ();
      yySourceFile = yyFileName == NULL ? StdIn :
# ifdef SOURCE_VER
	 l_scan_BeginSourceFile (yyFileName);
# else
	 l_scan_BeginSource (yyFileName);
# endif
      if (yySourceFile < 0) yyErrorMessage (xxCannotOpenInputFile);
   }

# ifdef SOURCE_VER

# if HAVE_WCHAR_T

void l_scan_BeginFileW
# ifdef HAVE_ARGS
   (wchar_t * yyFileName)
# else
   (yyFileName) wchar_t * yyFileName;
# endif
   {
      yyInitialize ();
      yySourceFile = yyFileName == NULL ? StdIn :
	 l_scan_BeginSourceFileW (yyFileName);
      if (yySourceFile < 0) yyErrorMessage (xxCannotOpenInputFile);
   }

# endif

void l_scan_BeginMemory
# ifdef HAVE_ARGS
   (void * yyInputPtr)
# else
   (yyInputPtr) void * yyInputPtr;
# endif
   {
      yyInitialize ();
      l_scan_BeginSourceMemory (yyInputPtr);
   }

void l_scan_BeginMemoryN
# ifdef HAVE_ARGS
   (void * yyInputPtr, int yyLength)
# else
   (yyInputPtr, yyLength) void * yyInputPtr; int yyLength;
# endif
   {
      yyInitialize ();
      l_scan_BeginSourceMemoryN (yyInputPtr, yyLength);
   }

void l_scan_BeginGeneric
# ifdef HAVE_ARGS
   (void * yyInputPtr)
# else
   (yyInputPtr) void * yyInputPtr;
# endif
   {
      yyInitialize ();
      l_scan_BeginSourceGeneric (yyInputPtr);
   }

# endif

void l_scan_CloseFile ARGS ((void))
   {
# if yyInitFileStackSize != 0
      if (yyFileStackPtr == yyFileStack) yyErrorMessage (xxFileStackUnderflow);
# endif
      l_scan_CloseSource (yySourceFile);
      ReleaseArray ((char * *) & yyChBufferPtr, & yyChBufferSize,
			(unsigned long) sizeof (yytChar));
# if yyInitFileStackSize != 0
						/* pop file */
      yySourceFile	= yyFileStackPtr->yySourceFile		;
      yyEof		= yyFileStackPtr->yyEof			;
      yyChBufferPtr	= yyFileStackPtr->yyChBufferPtr		;
      yyChBufferStart	= yyFileStackPtr->yyChBufferStart	;
      yyChBufferStart2	= (yytChar *) yyChBufferStart		;
      yyChBufferSize	= yyFileStackPtr->yyChBufferSize	;
      yyChBufferIndex	= yyFileStackPtr->yyChBufferIndex	;
      yyBytesRead	= yyFileStackPtr->yyBytesRead		;
      yyFileOffset	= yyFileStackPtr->yyFileOffset		;
      yyLineCount	= yyFileStackPtr->yyLineCount		;
      yyLineStart	= yyFileStackPtr->yyLineStart		;
      yyFileStackPtr --;
# endif
   }

# ifdef xxGetWord
int l_scan_GetWord
# ifdef HAVE_ARGS
   (yytChar * yyWord)
# else
   (yyWord) yytChar * yyWord;
# endif
   {
      register yytChar * yySource		= l_scan_TokenPtr;
      register yytChar * yyTarget		= yyWord;
      register yytChar * yyChBufferIndexReg	= (yytChar *) yyChBufferIndex;

      while (yySource < yyChBufferIndexReg)
	 * yyTarget ++ = * yySource ++;
      * yyTarget = '\0';
      return (int) (yyChBufferIndexReg - l_scan_TokenPtr);
   }
# endif

# ifdef xxGetLower
int l_scan_GetLower
# ifdef HAVE_ARGS
   (yytChar * yyWord)
# else
   (yyWord) yytChar * yyWord;
# endif
   {
      register yytusChar * yySource	= (yytusChar *) l_scan_TokenPtr;
      register yytusChar * yyTarget	= (yytusChar *) yyWord;
      register yytusChar * yyChBufferIndexReg = yyChBufferIndex;

      while (yySource < yyChBufferIndexReg)
	 * yyTarget ++ = yyToLower (* yySource ++);
      * yyTarget = '\0';
      return (int) (yyChBufferIndexReg - (yytusChar *) l_scan_TokenPtr);
   }
# endif

# ifdef xxGetUpper
int l_scan_GetUpper
# ifdef HAVE_ARGS
   (yytChar * yyWord)
# else
   (yyWord) yytChar * yyWord;
# endif
   {
      register yytusChar * yySource	= (yytusChar *) l_scan_TokenPtr;
      register yytusChar * yyTarget	= (yytusChar *) yyWord;
      register yytusChar * yyChBufferIndexReg = yyChBufferIndex;

      while (yySource < yyChBufferIndexReg)
	 * yyTarget ++ = yyToUpper (* yySource ++);
      * yyTarget = '\0';
      return (int) (yyChBufferIndexReg - (yytusChar *) l_scan_TokenPtr);
   }
# endif

# ifdef xxinput
static yytChar input ARGS ((void))
   {
      if (yyChBufferIndex == & yyChBufferStart [yyBytesRead]) {
	 if (! yyEof) {
	    yyLineStart -= yyBytesRead;
	    yyChBufferIndex = yyChBufferStart = & yyChBufferPtr [yyMaxAlign];
	    yyChBufferStart2 = (yytChar *) yyChBufferStart;
	    yyFileOffset += yyBytesRead;
	    yyBytesRead = yyGetLine (yySourceFile, (yytChar *) yyChBufferIndex,
	       (int) Exp2 (Log2 (yyChBufferSize)));
	    if (yyBytesRead <= 0) { yyBytesRead = 0; yyEof = rtrue; }
	    yyChBufferStart [yyBytesRead    ] = yyEobCh;
	    yyChBufferStart [yyBytesRead + 1] = '\0';
	 }
      }
      if (yyChBufferIndex == & yyChBufferStart [yyBytesRead]) return '\0';
      else return * yyChBufferIndex ++;
   }
# endif

static void unput
# ifdef HAVE_ARGS
   (yytChar yyc)
# else
   (yyc) yytChar yyc;
# endif
   { * (-- yyChBufferIndex) = yyc; }

static void yyLess
# ifdef HAVE_ARGS
   (int yyn)
# else
   (yyn) int yyn;
# endif
   { yyChBufferIndex -= l_scan_TokenLength - yyn; l_scan_TokenLength = yyn; }

void l_scan_BeginScanner ARGS ((void))
   {
   }

void l_scan_CloseScanner ARGS ((void))
   {
   }

void l_scan_ResetScanner ARGS ((void))
   {
      yyChBufferPtr	= yyInitChBuffer;
      yyChBufferSize	= 0;
      yyChBufferStart	= & yyInitChBuffer [16];
      yyChBufferIndex	= & yyInitChBuffer [16];
      if (yyStateStackSize != 0)
	 ReleaseArray ((char * *) & yyStateStack, & yyStateStackSize,
			(unsigned long) sizeof (yyStateRange));
      yyStateStack	= yyInitStateStack;
      yyStateStackSize	= 0;
# if yyInitFileStackSize != 0
      if (yyFileStackSize != 0)
	 ReleaseArray ((char * *) & yyFileStack, & yyFileStackSize,
			(unsigned long) sizeof (yytFileStack));
      yyFileStackSize	= 0;
# endif
# if defined xxyyPush | defined xxyyPop
      if (yyStStStackSize != 0)
	 ReleaseArray ((char * *) & yyStStStackPtr, & yyStStStackSize,
			(unsigned long) sizeof (yyStateRange));
      yyStStStackSize	= 0;
      yyStStStackIdx	= 0;
# endif
      yyStartState	= STD;
      yyPreviousStart	= STD;
      yySourceFile	= StdIn;
   }

static void yyErrorMessage
# ifdef HAVE_ARGS
   (int yyErrorCode)
# else
   (yyErrorCode) int yyErrorCode;
# endif
   {
      ErrorMessageI (yyErrorCode, xxFatal, l_scan_Attribute.Position,
	 xxString, "l_scan");
      l_scan_Exit ();
   }
