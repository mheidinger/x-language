/* $Id: Scanner.c,v 2.67 2009/10/13 14:13:39 grosch Exp $ */

# define rbool		char
# define rtrue		1
# define rfalse		0
# define StdIn		0

# include "xlang_scan.h"

# ifndef EXTERN_C_BEGIN
# define EXTERN_C_BEGIN
# define EXTERN_C_END
# endif

EXTERN_C_BEGIN
#    include "xlang_scanSource.h"
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
# define yyEcho		{ (void) fwrite (xlang_scan_TokenPtr, sizeof (yytChar), \
			xlang_scan_TokenLength, stdout); }
# define yyEol(Column)	{ yyLineCount ++; \
			yyLineStart = (yytusChar *) xlang_scan_TokenPtr + \
			xlang_scan_TokenLength - 1 - (Column); }
# if xlang_scan_xxMaxCharacter < 256
#  define output(c)	(void) putchar ((int) c)
# else
#  define output(c)	(void) printf ("%lc", c)
# endif
# define yyColumn(Ptr)	((int) ((Ptr) - (yytChar *) yyLineStart))
# define yyOffset(Ptr)	(yyFileOffset + ((Ptr) - yyChBufferStart2))

# define yytChar	xlang_scan_xxtChar
# define yytusChar	xlang_scan_xxtusChar

# define yyDNoState		0
# define yyFirstCh	(yytusChar) '\0'
# define yyEolCh	(yytusChar) '\12'
# define yyEobCh	(yytusChar) '\177'
# define yyDStateCount	88
# define yyTableSize	1446
# define yyEobState	22
# define yyDefaultState	23
# define yyToClassArraySize	0
# define STD	1
# define COMMENT	3
# define STRING	5
# define PROG_NAME	7
# define xxGetWord
# define xxinput

static void yyExit ARGS ((void))
{ rExit (1); }

typedef unsigned short	yyStateRange;
typedef struct { yyStateRange yyCheck, yyNext; } yyCombType;

	yytChar *	xlang_scan_TokenPtr	;
	int		xlang_scan_TokenLength	;
	xlang_scan_tScanAttribute	xlang_scan_Attribute	;
	void		(* xlang_scan_Exit)	ARGS ((void)) = yyExit;

static	void		yyInitialize	ARGS ((void));
static	void		yyErrorMessage	ARGS ((int yyErrorCode));
static	yytChar		input		ARGS ((void));
static	void		unput		ARGS ((yytChar));
static	void		yyLess		ARGS ((int));

static	yyCombType	yyComb		[yyTableSize   + 1] = {
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   1,   25}, 
{   1,   24}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   7,   27}, {   7,   88}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   5,   26}, {   5,   82}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   1,   28}, {   1,   49}, {   1,   87}, 
{   3,   25}, {   3,   24}, {   1,   60}, {   7,   31}, {   0,    0}, 
{   1,   58}, {   1,   57}, {   1,   61}, {   1,   63}, {   5,   30}, 
{   1,   62}, {   5,   86}, {   1,   19}, {   1,   13}, {   1,   13}, 
{   1,   13}, {   1,   13}, {   1,   13}, {   1,   13}, {   1,   13}, 
{   1,   13}, {   1,   13}, {   1,   13}, {   3,   29}, {   1,   64}, 
{   1,   53}, {   1,   14}, {   1,   51}, {  14,   20}, {  15,   15}, 
{  19,   81}, {   0,    0}, {   0,    0}, {   3,   79}, {   0,    0}, 
{  19,   16}, {   0,    0}, {  13,   17}, {   3,   11}, {  13,   13}, 
{  13,   13}, {  13,   13}, {  13,   13}, {  13,   13}, {  13,   13}, 
{  13,   13}, {  13,   13}, {  13,   13}, {  13,   13}, {  17,   17}, 
{  17,   17}, {  17,   17}, {  17,   17}, {  17,   17}, {  17,   17}, 
{  17,   17}, {  17,   17}, {  17,   17}, {  17,   17}, {   1,   59}, 
{   0,    0}, {  53,   77}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   1,   40}, {   1,   32}, {   0,    0}, {   5,   83}, 
{   1,   55}, {  55,   56}, {   0,    0}, {  32,   71}, {   0,    0}, 
{   0,    0}, {  32,   33}, {  53,   54}, {   0,    0}, {  55,   75}, 
{   1,   65}, {   0,    0}, {   0,    0}, {   0,    0}, {   1,   35}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   1,   48}, {   0,    0}, 
{   1,   47}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {  21,   22}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {  33,   34}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, {   9,    9}, 
{   9,    9}, {   9,    9}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  35,   36}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  36,   37}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  37,   38}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  38,   39}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  40,   41}, {  41,   42}, 
{  42,   43}, {  43,   44}, {  44,   45}, {  45,   46}, {  49,   50}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  51,   52}, {  62,   78}, {  65,   66}, {  66,   67}, 
{  12,   12}, {  67,   68}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, {  12,   12}, 
{  12,   12}, {  12,   12}, {  12,   12}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  68,   69}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  69,   70}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  71,   72}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, 
{  16,   16}, {  16,   16}, {  16,   16}, {  16,   16}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  72,   73}, {  73,   74}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  75,   76}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  79,   80}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {   0,    0}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, {  18,   18}, 
{  83,   84}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
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
{   0,    0}, {  83,   85}, {   0,    0}, {   0,    0}, {   0,    0}, 
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
{   0,    0}, {   0,    0}, 
};
static	yyCombType *	yyBasePtr	[yyDStateCount + 1] = { 0,
& yyComb [   0], & yyComb [   0], & yyComb [  26], & yyComb [   0], 
& yyComb [  12], & yyComb [   0], & yyComb [   6], & yyComb [   0], 
& yyComb [ 126], & yyComb [ 382], & yyComb [   0], & yyComb [ 590], 
& yyComb [  26], & yyComb [   2], & yyComb [   3], & yyComb [ 713], 
& yyComb [  36], & yyComb [ 969], & yyComb [  23], & yyComb [   0], 
& yyComb [   9], & yyComb [   0], & yyComb [   0], & yyComb [   0], 
& yyComb [   0], & yyComb [   0], & yyComb [   0], & yyComb [   0], 
& yyComb [   0], & yyComb [   0], & yyComb [   0], & yyComb [   0], 
& yyComb [ 139], & yyComb [   0], & yyComb [ 288], & yyComb [ 311], 
& yyComb [ 366], & yyComb [ 408], & yyComb [   0], & yyComb [ 540], 
& yyComb [ 534], & yyComb [ 549], & yyComb [ 619], & yyComb [ 547], 
& yyComb [ 551], & yyComb [   0], & yyComb [   0], & yyComb [   0], 
& yyComb [ 593], & yyComb [   0], & yyComb [ 620], & yyComb [   0], 
& yyComb [  51], & yyComb [   0], & yyComb [   4], & yyComb [   0], 
& yyComb [   0], & yyComb [   0], & yyComb [   0], & yyComb [   0], 
& yyComb [   0], & yyComb [ 620], & yyComb [   0], & yyComb [   0], 
& yyComb [ 567], & yyComb [ 570], & yyComb [ 581], & yyComb [ 613], 
& yyComb [ 657], & yyComb [   0], & yyComb [ 729], & yyComb [ 881], 
& yyComb [ 863], & yyComb [   0], & yyComb [ 895], & yyComb [   0], 
& yyComb [   0], & yyComb [   0], & yyComb [ 969], & yyComb [   0], 
& yyComb [   0], & yyComb [   0], & yyComb [1191], & yyComb [   0], 
& yyComb [   0], & yyComb [   0], & yyComb [   0], & yyComb [   0], 
};
static	yyStateRange	yyDefault	[yyDStateCount + 1] = { 0,
   12,     1,    18,     3,    10,     5,     9,     7,    21,    21, 
   21,    21,    21,    21,    21,    19,    21,    21,    21,    15, 
    0,     0,     0,    21,     0,    10,     9,     0,    18,    10, 
    9,    12,    12,    12,    12,    12,    12,    12,    12,    12, 
   12,    12,    12,    21,    21,     0,     0,     0,    21,     0, 
   21,     0,    21,     0,    12,    12,     0,     0,     0,     0, 
    0,    21,     0,     0,    12,    12,    12,    12,    12,    12, 
   12,    12,    12,    12,    12,    12,     0,     0,    11,     0, 
    0,    24,    21,     0,     0,     0,     0,    24, 
};
static	yyStateRange	yyEobTrans	[yyDStateCount + 1] = { 0,
    0,     0,    18,    18,    10,    10,     9,     9,     9,    10, 
    0,     0,     0,     0,     0,    16,     0,    18,     0,     0, 
    0,     0,     0,     0,     0,    10,     9,     0,    18,    10, 
    9,     0,     0,     0,     0,     0,     0,     0,     0,     0, 
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0, 
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0, 
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0, 
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0, 
    0,     0,     0,     0,     0,     0,     0,     0, 
};

# if xlang_scan_xxMaxCharacter < 256
#  define yyGetLine	xlang_scan_GetLine
# else
#  define yyGetLine	xlang_scan_GetWLine
# endif

# if yyToClassArraySize == 0
#  define yyToClass(x) (x)
# else
typedef	unsigned short	yytCharClass;

static	yytCharClass	yyToClassArray	[yyToClassArraySize] = {
};
#  if xlang_scan_xxMaxCharacter < yyToClassArraySize
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

/* line 55 "xlang.rex" */

  /* code to be put into Scanner.c */
  # include <stdlib.h>
  # include "Errors.h"

  void StartProgName(void) { yyStart(PROG_NAME); }

  /* Insert the routine computing "error-values" of attributes, in case the
   * parser decides during error repair to insert a token.
   */
  
/* line 67 "xlang.rex" */
void xlang_scan_ErrorAttribute
# ifdef HAVE_ARGS
 (int Token, xlang_scan_tScanAttribute * pAttribute)
# else
 (Token, pAttribute) int Token; xlang_scan_tScanAttribute * pAttribute;
# endif
{
 pAttribute->Position = xlang_scan_Attribute.Position;
 switch (Token) {
 case /* int_const */ 1: 
 pAttribute->int_const.Value = 0;
  break;
 case /* float_const */ 2: 
 pAttribute->float_const.Value = 0.0;
  break;
 case /* string_const */ 3: 
 pAttribute->string_const.Value = NoString;
  break;
 case /* prog_name */ 4: 
 pAttribute->prog_name.Value = NoString;
  break;
 case /* identifier */ 5: 
 pAttribute->identifier.Value = NoIdent;
  break;
 case /* separator */ 6: 
 pAttribute->separator.Dummy = 0;
  break;
 }
}
/* line 97 "xlang.rex" */


/* line 573 "xlang_scan.c" */

# ifndef yySetPosition
# define yySetPosition xlang_scan_Attribute.Position.Line = yyLineCount; \
xlang_scan_Attribute.Position.Column = (int) ((yytusChar *) xlang_scan_TokenPtr - yyLineStart);
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
   { yyLineStart -= (yyTabSpace - 1 - ((yytusChar *) xlang_scan_TokenPtr -
	yyLineStart + yya - 1)) & (yyTabSpace - 1); }

# define yyTab		yyLineStart -= (yyTabSpace - 1 - \
((yytusChar *) xlang_scan_TokenPtr - yyLineStart - 1)) & (yyTabSpace - 1)
# define yyTab1(a)	yyLineStart -= (yyTabSpace - 1 - \
((yytusChar *) xlang_scan_TokenPtr - yyLineStart + (a) - 1)) & (yyTabSpace - 1)
# define yyTab2(a,b)	yyLineStart -= (yyTabSpace - 1 - \
((yytusChar *) xlang_scan_TokenPtr - yyLineStart + (a) - 1)) & (yyTabSpace - 1)

# ifndef EBCDIC
#  if xlang_scan_xxMaxCharacter < 256
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

int xlang_scan_GetToken ARGS ((void))
{
   register	yyStateRange	yyState;
   register	yyStateRange *	yyStatePtr;
   register	yytusChar *	yyChBufferIndexReg;
   register	yyCombType * *	yyBasePtrReg = yyBasePtr;
/* line 100 "xlang.rex" */

  /* user-defined local variables of the generated GetToken routine */
  # define MAX_STRING_LEN 1024
  char string[MAX_STRING_LEN];
  int str_len = 0;
  tPosition str_pos;

/* line 744 "xlang_scan.c" */

yyBegin:
   yyState		= yyStartState;		/* initialize */
   yyStatePtr		= & yyStateStack [1];
   yyChBufferIndexReg	= yyChBufferIndex;
   xlang_scan_TokenPtr		= (yytChar *) yyChBufferIndexReg;

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
      xlang_scan_TokenLength =
	    (int) (yyChBufferIndexReg - (yytusChar *) xlang_scan_TokenPtr);
      yyChBufferIndex = yyChBufferIndexReg;
switch (* -- yyStatePtr) {
case 9:;
case 27:;
case 31:;
yySetPosition
/* line 154 "xlang.rex" */
{
  str_len = xlang_scan_GetWord(&string[str_len]);

/* line 781 "xlang_scan.c" */
} goto yyBegin;
case 88:;
yySetPosition
/* line 158 "xlang.rex" */
{
  yyEol(0);
  yyStart(STD);
  xlang_scan_Attribute.prog_name.Value = PutString(string, str_len);
  return 4;

/* line 792 "xlang_scan.c" */
} goto yyBegin;
case 15:;
yySetPosition
/* line 166 "xlang.rex" */
{
  return 6;

/* line 800 "xlang_scan.c" */
} goto yyBegin;
case 13:;
yySetPosition
/* line 171 "xlang.rex" */
{
  xlang_scan_GetWord (string);
  xlang_scan_Attribute.int_const.Value = atol (string);
  return 1;

/* line 810 "xlang_scan.c" */
} goto yyBegin;
case 17:;
yySetPosition
/* line 178 "xlang.rex" */
{
  xlang_scan_GetWord (string);
  xlang_scan_Attribute.float_const.Value = atof (string);
  return 2;

/* line 820 "xlang_scan.c" */
} goto yyBegin;
case 87:;
yySetPosition
/* line 185 "xlang.rex" */
{
  yyStart(STRING);
  str_len = 0; /* reset the string length counter */
  str_pos = xlang_scan_Attribute.Position;

/* line 830 "xlang_scan.c" */
} goto yyBegin;
case 86:;
yySetPosition
/* line 192 "xlang.rex" */
{
  yyStart(STD);
  xlang_scan_Attribute.string_const.Value = PutString(string, str_len);
  xlang_scan_Attribute.string_const.Position = str_pos;
  return 3;

/* line 841 "xlang_scan.c" */
} goto yyBegin;
case 83:;
yySetPosition
/* line 199 "xlang.rex" */
{
  string[str_len++] = '\\';

/* line 849 "xlang_scan.c" */
} goto yyBegin;
case 85:;
yySetPosition
/* line 203 "xlang.rex" */
{
  string[str_len++] = '\n';

/* line 857 "xlang_scan.c" */
} goto yyBegin;
case 84:;
yySetPosition
/* line 207 "xlang.rex" */
{
  string[str_len++] = '"';

/* line 865 "xlang_scan.c" */
} goto yyBegin;
case 10:;
case 26:;
case 30:;
yySetPosition
/* line 211 "xlang.rex" */
{
  str_len = xlang_scan_GetWord(&string[str_len]);

/* line 875 "xlang_scan.c" */
} goto yyBegin;
case 82:;
yySetPosition
/* line 215 "xlang.rex" */
{
  Message("Line break in string literal", xxError, xlang_scan_Attribute.Position);

/* line 883 "xlang_scan.c" */
} goto yyBegin;
case 16:;
/* line 220 "xlang.rex" */
{
/* line 888 "xlang_scan.c" */
} goto yyBegin;
case 81:;
/* line 223 "xlang.rex" */
{
  yyStart(COMMENT);

/* line 895 "xlang_scan.c" */
} goto yyBegin;
case 80:;
/* line 227 "xlang.rex" */
{
  yyStart(STD);

/* line 902 "xlang_scan.c" */
} goto yyBegin;
case 11:;
case 18:;
case 29:;
case 79:;
/* line 231 "xlang.rex" */
{
/* line 910 "xlang_scan.c" */
} goto yyBegin;
case 78:;
yySetPosition
/* line 236 "xlang.rex" */
{ return 7; 
/* line 916 "xlang_scan.c" */
} goto yyBegin;
case 77:;
yySetPosition
/* line 237 "xlang.rex" */
{ return 8; 
/* line 922 "xlang_scan.c" */
} goto yyBegin;
case 76:;
yySetPosition
/* line 238 "xlang.rex" */
{ return 9; 
/* line 928 "xlang_scan.c" */
} goto yyBegin;
case 74:;
yySetPosition
/* line 239 "xlang.rex" */
{ return 10; 
/* line 934 "xlang_scan.c" */
} goto yyBegin;
case 70:;
yySetPosition
/* line 240 "xlang.rex" */
{ return 11; 
/* line 940 "xlang_scan.c" */
} goto yyBegin;
case 64:;
yySetPosition
/* line 241 "xlang.rex" */
{ return 12; 
/* line 946 "xlang_scan.c" */
} goto yyBegin;
case 14:;
yySetPosition
/* line 242 "xlang.rex" */
{ return 13; 
/* line 952 "xlang_scan.c" */
} goto yyBegin;
case 63:;
yySetPosition
/* line 243 "xlang.rex" */
{ return 14; 
/* line 958 "xlang_scan.c" */
} goto yyBegin;
case 62:;
yySetPosition
/* line 244 "xlang.rex" */
{ return 15; 
/* line 964 "xlang_scan.c" */
} goto yyBegin;
case 61:;
yySetPosition
/* line 245 "xlang.rex" */
{ return 16; 
/* line 970 "xlang_scan.c" */
} goto yyBegin;
case 19:;
yySetPosition
/* line 246 "xlang.rex" */
{ return 17; 
/* line 976 "xlang_scan.c" */
} goto yyBegin;
case 60:;
yySetPosition
/* line 247 "xlang.rex" */
{ return 18; 
/* line 982 "xlang_scan.c" */
} goto yyBegin;
case 59:;
yySetPosition
/* line 248 "xlang.rex" */
{ return 19; 
/* line 988 "xlang_scan.c" */
} goto yyBegin;
case 58:;
yySetPosition
/* line 249 "xlang.rex" */
{ return 20; 
/* line 994 "xlang_scan.c" */
} goto yyBegin;
case 57:;
yySetPosition
/* line 250 "xlang.rex" */
{ return 21; 
/* line 1000 "xlang_scan.c" */
} goto yyBegin;
case 56:;
yySetPosition
/* line 251 "xlang.rex" */
{ return 22; 
/* line 1006 "xlang_scan.c" */
} goto yyBegin;
case 53:;
yySetPosition
/* line 252 "xlang.rex" */
{ return 23; 
/* line 1012 "xlang_scan.c" */
} goto yyBegin;
case 51:;
yySetPosition
/* line 253 "xlang.rex" */
{ return 24; 
/* line 1018 "xlang_scan.c" */
} goto yyBegin;
case 54:;
yySetPosition
/* line 254 "xlang.rex" */
{ return 25; 
/* line 1024 "xlang_scan.c" */
} goto yyBegin;
case 52:;
yySetPosition
/* line 255 "xlang.rex" */
{ return 26; 
/* line 1030 "xlang_scan.c" */
} goto yyBegin;
case 20:;
yySetPosition
/* line 256 "xlang.rex" */
{ return 27; 
/* line 1036 "xlang_scan.c" */
} goto yyBegin;
case 50:;
yySetPosition
/* line 257 "xlang.rex" */
{ return 28; 
/* line 1042 "xlang_scan.c" */
} goto yyBegin;
case 48:;
yySetPosition
/* line 258 "xlang.rex" */
{ return 29; 
/* line 1048 "xlang_scan.c" */
} goto yyBegin;
case 47:;
yySetPosition
/* line 259 "xlang.rex" */
{ return 30; 
/* line 1054 "xlang_scan.c" */
} goto yyBegin;
case 43:;
yySetPosition
/* line 260 "xlang.rex" */
{ return 31; 
/* line 1060 "xlang_scan.c" */
} goto yyBegin;
case 46:;
yySetPosition
/* line 261 "xlang.rex" */
{ return 32; 
/* line 1066 "xlang_scan.c" */
} goto yyBegin;
case 39:;
yySetPosition
/* line 262 "xlang.rex" */
{ return 33; 
/* line 1072 "xlang_scan.c" */
} goto yyBegin;
case 34:;
yySetPosition
/* line 263 "xlang.rex" */
{ return 34; 
/* line 1078 "xlang_scan.c" */
} goto yyBegin;
case 12:;
case 32:;
case 33:;
case 35:;
case 36:;
case 37:;
case 38:;
case 40:;
case 41:;
case 42:;
case 55:;
case 65:;
case 66:;
case 67:;
case 68:;
case 69:;
case 71:;
case 72:;
case 73:;
case 75:;
yySetPosition
/* line 266 "xlang.rex" */
{
  xlang_scan_Attribute.identifier.Value = MakeIdent(xlang_scan_TokenPtr, xlang_scan_TokenLength);
  return 5;

/* line 1106 "xlang_scan.c" */
} goto yyBegin;
case 28:;
{/* BlankAction */
while (* yyChBufferIndexReg ++ == ' ') ;
xlang_scan_TokenPtr = (yytChar *) -- yyChBufferIndexReg;
yyState = yyStartState;
yyStatePtr = & yyStateStack [1];
goto yyContinue;
/* line 1115 "xlang_scan.c" */
} goto yyBegin;
case 25:;
{/* TabAction */
yyTab;
/* line 1120 "xlang_scan.c" */
} goto yyBegin;
case 24:;
{/* EolAction */
yyEol (0);
/* line 1125 "xlang_scan.c" */
} goto yyBegin;
case 1:;
case 2:;
case 3:;
case 4:;
case 5:;
case 6:;
case 7:;
case 8:;
case 21:;
case 44:;
case 45:;
case 49:;
	 /* non final states */
	 yyChBufferIndexReg --;			/* return character */
	 break;

case 23:
	 yySetPosition
      xlang_scan_TokenLength   = 1;
	 yyChBufferIndex = ++ yyChBufferIndexReg;
	 {
/* line 108 "xlang.rex" */

  /* What happens if no scanner rule matches the input */
  MessageI ("Illegal character",
	    xxError, xlang_scan_Attribute.Position,
	    xxCharacter, xlang_scan_TokenPtr);

/* line 1155 "xlang_scan.c" */
	 }
	 goto yyBegin;

      case yyDNoState:
	 goto yyBegin;

case 22:
	 yyChBufferIndex = -- yyChBufferIndexReg; /* undo last state transit */
	 if (-- xlang_scan_TokenLength == 0) {		/* get previous state */
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
	    yySource = xlang_scan_TokenPtr - 1;
	    yyTarget = (yytChar *) & yyChBufferPtr
		[(yyMaxAlign - 1 - xlang_scan_TokenLength) & (yyMaxAlign - 1)];
	    yyChBufferFree = Exp2 (Log2 (yyChBufferSize - 4 -
		yyMaxAlign - xlang_scan_TokenLength));
		/* copy initial part of token in front of the input buffer */
	    if (yySource > yyTarget) {
	       xlang_scan_TokenPtr = yyTarget + 1;
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
	       xlang_scan_TokenPtr	+= yyDelta;
	       yyChBufferStart2	 = (yytChar *) yyChBufferStart;
	       yyChBufferFree = Exp2 (Log2 (yyChBufferSize - 4 -
			yyMaxAlign - xlang_scan_TokenLength));
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

	 if (xlang_scan_TokenLength == 0) {		/* end of file reached */
	    if (yyChBufferSize == 0) return xlang_scan_EofToken;
	    yySetPosition
	    xlang_scan_CloseFile ();
# if yyInitFileStackSize != 0
	    if (yyFileStackPtr == yyFileStack) {
/* line 115 "xlang.rex" */

  /* What should be done if the end-of-input-file has been reached? */

  /* E.g.: check hat strings and comments are closed. */
  switch (yyStartState) {
  case STD:
    /* ok */
    break;
  case COMMENT:
    Message ("Error: unclosed comment",
	     xxError, xlang_scan_Attribute.Position);
    break;
  case STRING:
    Message ("Error: unclosed string",
	    xxError, xlang_scan_Attribute.Position);
    break;
  case PROG_NAME:
    Message ("Error: program name not terminated by newline",
	    xxError, xlang_scan_Attribute.Position);
    break;
  default:
    Message ("Error: that should not happen!",
	     xxFatal, xlang_scan_Attribute.Position);
    break;
  }

  /* implicit: return the EofToken */

/* line 1275 "xlang_scan.c" */
	    }
	    if (yyFileStackPtr == yyFileStack) {
	       xlang_scan_ResetScanner ();
	       return xlang_scan_EofToken;
	    }
	    goto yyBegin;
# else
/* line 115 "xlang.rex" */

  /* What should be done if the end-of-input-file has been reached? */

  /* E.g.: check hat strings and comments are closed. */
  switch (yyStartState) {
  case STD:
    /* ok */
    break;
  case COMMENT:
    Message ("Error: unclosed comment",
	     xxError, xlang_scan_Attribute.Position);
    break;
  case STRING:
    Message ("Error: unclosed string",
	    xxError, xlang_scan_Attribute.Position);
    break;
  case PROG_NAME:
    Message ("Error: program name not terminated by newline",
	    xxError, xlang_scan_Attribute.Position);
    break;
  default:
    Message ("Error: that should not happen!",
	     xxFatal, xlang_scan_Attribute.Position);
    break;
  }

  /* implicit: return the EofToken */

/* line 1312 "xlang_scan.c" */
	    xlang_scan_ResetScanner ();
	    return xlang_scan_EofToken;
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
      xlang_scan_TokenPtr	   = (yytChar *) yyChBufferStart;
      yyEof		   = rfalse;
      yyBytesRead	   = 0;
      yyFileOffset	   = 0;
      yyLineCount	   = 1;
      yyLineStart	   = & yyChBufferStart [-1];
# ifdef HAVE_FILE_NAME
      if (xlang_scan_Attribute.Position.FileName == 0)
	 xlang_scan_Attribute.Position.FileName = 1; /* NoIdent */
# endif
   }

void xlang_scan_BeginFile
# ifdef HAVE_ARGS
   (char * yyFileName)
# else
   (yyFileName) char * yyFileName;
# endif
   {
      yyInitialize ();
      yySourceFile = yyFileName == NULL ? StdIn :
# ifdef SOURCE_VER
	 xlang_scan_BeginSourceFile (yyFileName);
# else
	 xlang_scan_BeginSource (yyFileName);
# endif
      if (yySourceFile < 0) yyErrorMessage (xxCannotOpenInputFile);
   }

# ifdef SOURCE_VER

# if HAVE_WCHAR_T

void xlang_scan_BeginFileW
# ifdef HAVE_ARGS
   (wchar_t * yyFileName)
# else
   (yyFileName) wchar_t * yyFileName;
# endif
   {
      yyInitialize ();
      yySourceFile = yyFileName == NULL ? StdIn :
	 xlang_scan_BeginSourceFileW (yyFileName);
      if (yySourceFile < 0) yyErrorMessage (xxCannotOpenInputFile);
   }

# endif

void xlang_scan_BeginMemory
# ifdef HAVE_ARGS
   (void * yyInputPtr)
# else
   (yyInputPtr) void * yyInputPtr;
# endif
   {
      yyInitialize ();
      xlang_scan_BeginSourceMemory (yyInputPtr);
   }

void xlang_scan_BeginMemoryN
# ifdef HAVE_ARGS
   (void * yyInputPtr, int yyLength)
# else
   (yyInputPtr, yyLength) void * yyInputPtr; int yyLength;
# endif
   {
      yyInitialize ();
      xlang_scan_BeginSourceMemoryN (yyInputPtr, yyLength);
   }

void xlang_scan_BeginGeneric
# ifdef HAVE_ARGS
   (void * yyInputPtr)
# else
   (yyInputPtr) void * yyInputPtr;
# endif
   {
      yyInitialize ();
      xlang_scan_BeginSourceGeneric (yyInputPtr);
   }

# endif

void xlang_scan_CloseFile ARGS ((void))
   {
# if yyInitFileStackSize != 0
      if (yyFileStackPtr == yyFileStack) yyErrorMessage (xxFileStackUnderflow);
# endif
      xlang_scan_CloseSource (yySourceFile);
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
int xlang_scan_GetWord
# ifdef HAVE_ARGS
   (yytChar * yyWord)
# else
   (yyWord) yytChar * yyWord;
# endif
   {
      register yytChar * yySource		= xlang_scan_TokenPtr;
      register yytChar * yyTarget		= yyWord;
      register yytChar * yyChBufferIndexReg	= (yytChar *) yyChBufferIndex;

      while (yySource < yyChBufferIndexReg)
	 * yyTarget ++ = * yySource ++;
      * yyTarget = '\0';
      return (int) (yyChBufferIndexReg - xlang_scan_TokenPtr);
   }
# endif

# ifdef xxGetLower
int xlang_scan_GetLower
# ifdef HAVE_ARGS
   (yytChar * yyWord)
# else
   (yyWord) yytChar * yyWord;
# endif
   {
      register yytusChar * yySource	= (yytusChar *) xlang_scan_TokenPtr;
      register yytusChar * yyTarget	= (yytusChar *) yyWord;
      register yytusChar * yyChBufferIndexReg = yyChBufferIndex;

      while (yySource < yyChBufferIndexReg)
	 * yyTarget ++ = yyToLower (* yySource ++);
      * yyTarget = '\0';
      return (int) (yyChBufferIndexReg - (yytusChar *) xlang_scan_TokenPtr);
   }
# endif

# ifdef xxGetUpper
int xlang_scan_GetUpper
# ifdef HAVE_ARGS
   (yytChar * yyWord)
# else
   (yyWord) yytChar * yyWord;
# endif
   {
      register yytusChar * yySource	= (yytusChar *) xlang_scan_TokenPtr;
      register yytusChar * yyTarget	= (yytusChar *) yyWord;
      register yytusChar * yyChBufferIndexReg = yyChBufferIndex;

      while (yySource < yyChBufferIndexReg)
	 * yyTarget ++ = yyToUpper (* yySource ++);
      * yyTarget = '\0';
      return (int) (yyChBufferIndexReg - (yytusChar *) xlang_scan_TokenPtr);
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
   { yyChBufferIndex -= xlang_scan_TokenLength - yyn; xlang_scan_TokenLength = yyn; }

void xlang_scan_BeginScanner ARGS ((void))
   {
   }

void xlang_scan_CloseScanner ARGS ((void))
   {
   }

void xlang_scan_ResetScanner ARGS ((void))
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
      ErrorMessageI (yyErrorCode, xxFatal, xlang_scan_Attribute.Position,
	 xxString, "xlang_scan");
      xlang_scan_Exit ();
   }
