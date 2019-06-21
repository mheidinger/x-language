/* $Id: Parser.c,v 1.54 2010/07/07 09:53:21 grosch Exp $ */

# include "xlang_pars.h"
# include <stdio.h>
# include <stddef.h>

# ifndef EXTERN_C_BEGIN
#  define EXTERN_C_BEGIN
#  define EXTERN_C_END
# endif

EXTERN_C_BEGIN
# include "rString.h"
# include "rSystem.h"
# include "rMemory.h"
# include "General.h"
# include "DynArray.h"
# ifndef NO_RECOVER
#  include "Sets.h"
# endif
# if ! defined NO_RECOVER | defined YYReParse
#  include "Errors.h"
# endif
# if defined YYDEBUG | defined lex_interface
#  include "Position.h"
# endif
EXTERN_C_END

# ifdef lex_interface
#  define xlang_scan_GetToken	yylex
     extern int yylex ARGS ((void));
#  ifndef AttributeDef
#   include "Position.h"
	   typedef struct { tPosition Position; } xlang_scan_tScanAttribute;
	   xlang_scan_tScanAttribute	xlang_scan_Attribute = {{ 0, 0 }};
#  endif
#  ifndef ErrorAttributeDef
#   define xlang_scan_ErrorAttribute(Token, RepairAttribute)
#  endif
#  ifndef yyGetAttribute
#   define yyGetAttribute(yyAttrStackPtr, a) * yyAttrStackPtr = yylval
#  endif
# else
#  include "xlang_scan.h"
#  ifndef yyGetAttribute
#   define yyGetAttribute(yyAttrStackPtr, a) (yyAttrStackPtr)->Scan = a
#  endif
# endif

static void yyExit ARGS ((void)) { rExit (1); }

	void		(* xlang_pars_Exit) ARGS ((void)) = yyExit;

static void yyCheckMemory
# ifdef HAVE_ARGS
   (char * ptr)
# else
   (ptr) char * ptr;
# endif
{
  if (! (ptr)) xlang_pars_Exit ();
}

# define yyMakeArray(ptr, count, size)	 { MakeArray (ptr, count, size); \
					   yyCheckMemory (* ptr); }
# define yyExtendArray(ptr, count, size) { ExtendArray (ptr, count, size); \
					   yyCheckMemory (* ptr); }

typedef unsigned short	yyStateRange	;
typedef unsigned short	yySymbolRange	;
typedef struct { yyStateRange Check, Next; } yytComb;
typedef enum {
yyNT0_intern	= 36,
yyNTroot	= 37,
yyNTProgramm	= 38,
yyNTNamenTeil	= 39,
yyNTVariablenTeil	= 40,
yyNTDeclarationList	= 41,
yyNTVariable	= 42,
yyNTIOFlag	= 43,
yyNTVariablenTyp	= 44,
yyNTCodeTeil	= 45,
yyNTInstruction	= 46,
yyNTAssignment	= 47,
yyNTExpression	= 48,
yyNTCodeBlock	= 49,
yyNTCondition	= 50,
yyNTElseIfList	= 51,
yyNTComparison	= 52,
yyNTLoop	= 53,
yyNTWhileLoop	= 54,
yyNTForLoop	= 55
} yytNonterminal;
typedef struct { short yyMode; rbool yyActions, yyMessages; } yytControl;

static	yytControl	yyControl	= { 0, rtrue, rtrue };
	rbool		xlang_pars_Debug	= rfalse;

# define yyFirstTerminal	0
# define yyLastTerminal	35
# define yySetSize	36
# define yyFirstSymbol	0
# define yyLastSymbol	55
# define yyTTableMax	235
# define yyNTableMax	93
# define yyStartState	1
# define yyFirstReadState	1
# define yyLastReadState	66
# define yyFirstReadReduceState	67
# define yyLastReadReduceState	81
# define yyFirstReduceState	82
# define yyLastReduceState	131
# define yyLastState	131
# define yyLastStopState	82
# define YYTDefault
# define YYNDefault

# define yyFirstFinalState	yyFirstReadReduceState

# define ErrorMessages(Messages) yyControl.yyMessages = Messages
# define SemActions(Actions)	 yyControl.yyActions = Actions

# ifdef YYCSTree
#  include "cs_tree.h"
#  define CommonParsAttrs tCSTree Tree;
# endif

# ifdef YYGetLook

#  define GetLookahead(k)	yyGetLookahead ((k) - 1, yyTerminal)
#  define GetAttribute(k, a)	xxGetAttribute ((k) - 1, yyTerminal, a)

static int yyGetLookahead	ARGS ((int yyk, yySymbolRange yyToken));
static void xxGetAttribute	ARGS ((int yyk, yySymbolRange yyToken,
				xlang_scan_tScanAttribute * yyAttribute));

# endif

/* line 8 "xlang_pars.lrk" */


/* line 10 "xlang.pars" */

  # include <stdio.h>
  # include <math.h>
  # include "StringM.h"
  # define M_PI acos(-1.0) 


/* line 19 "xlang_pars.lrk" */
# ifdef CommonParsAttrs
typedef struct { CommonParsAttrs } yytCommonParsAttrs;
#  define yyCommon yytCommonParsAttrs Common;
#  define yyCommonParsAttrs CommonParsAttrs
# else
#  define yyCommon
#  define yyCommonParsAttrs
# endif


typedef union {
xlang_scan_tScanAttribute Scan;
yyCommon
} tParsAttribute;

/* line 167 "xlang_pars.c" */

# if defined lex_interface & ! defined yylvalDef
     tParsAttribute yylval;
# endif

# ifndef yyInitStackSize
#  define yyInitStackSize	100
# endif
# define yyNoState		0
# define yystandard		1
# define yytrial		2
# define yybuffer		4
# define yyreparse		8
# define yyS			yySynAttribute
# define yyA			yyAttrStackPtr
# define YYACCEPT		goto yyAccept
# define YYABORT		goto yyAbort

# ifdef YYDEC_TABLE
#  define yyDecrement(x)
#  define yySetNT(x)
# else
#  define yyDecrement(x)	yyStateStackPtr -= x; yyAttrStackPtr -= x;
#  define yySetNT(x)		yyNonterminal = x;
# endif

# ifdef YYNDefault
#  define yytNComb yytComb
# else
#  define yytNComb yyStateRange
# endif

# if defined YYDEBUG | defined YYDCRP
static	long		yyCount		= 0;
static	FILE *		yyTrace		;

static	void		yyPrintState	ARGS ((yyStateRange yyState));
static	void		yyNl		ARGS ((void));

static	char *		yyRule		[] = { 0,
"0_intern : root _EOF_ ",
"root : Programm ",
"Programm : NamenTeil separator VariablenTeil separator CodeTeil ",
"NamenTeil : prog_name ",
"VariablenTeil : DeclarationList ",
"DeclarationList : Variable ';' ",
"DeclarationList : DeclarationList Variable ';' ",
"Variable : IOFlag identifier VariablenTyp ",
"IOFlag : '->' ",
"IOFlag : '<-' ",
"IOFlag : ",
"VariablenTyp : 'int' ",
"VariablenTyp : 'float' ",
"VariablenTyp : 'string' ",
"CodeTeil : Instruction ",
"CodeTeil : CodeTeil Instruction ",
"Instruction : Assignment ';' ",
"Instruction : Condition ",
"Instruction : Loop ",
"Assignment : identifier '=' Expression ",
"Expression : int_const ",
"Expression : float_const ",
"Expression : string_const ",
"Expression : identifier ",
"Expression : Expression '+' Expression ",
"Expression : Expression '-' Expression ",
"Expression : Expression '*' Expression ",
"Expression : Expression '/' Expression ",
"Expression : Expression '%' Expression ",
"Expression : Expression '^' Expression ",
"Expression : '(' Expression ')' ",
"Expression : '+' Expression ",
"Expression : '-' Expression ",
"CodeBlock : '{' CodeTeil '}' ",
"Condition : 'if' Comparison CodeBlock ",
"Condition : 'if' Comparison CodeBlock else CodeBlock ",
"Condition : 'if' Comparison CodeBlock ElseIfList CodeBlock ",
"Condition : 'if' Comparison CodeBlock ElseIfList CodeBlock else \
CodeBlock ",
"ElseIfList : 'else if' CodeBlock ",
"ElseIfList : ElseIfList 'else if' CodeBlock ",
"Comparison : Expression '<' Expression ",
"Comparison : Expression '>' Expression ",
"Comparison : Expression '<=' Expression ",
"Comparison : Expression '>=' Expression ",
"Comparison : Expression '==' Expression ",
"Comparison : Expression '!=' Expression ",
"Loop : WhileLoop ",
"Loop : ForLoop ",
"WhileLoop : 'while' Comparison CodeBlock ",
"ForLoop : 'for' Assignment ';' Comparison ';' Assignment CodeBlock ",
""
};
# endif
	char *		xlang_pars_TokenName	[yyLastTerminal + 2] = {
"_EOF_",
"int_const",
"float_const",
"string_const",
"prog_name",
"identifier",
"separator",
"->",
"<-",
"int",
"float",
"string",
";",
"=",
"+",
"-",
"*",
"/",
"%",
"^",
"(",
")",
"if",
"<",
">",
"<=",
">=",
"==",
"!=",
"{",
"}",
"else",
"else if",
"while",
"for",
0,
""
};
static	yytComb		yyTComb		[yyTTableMax + 1] = {
{  16,   84}, {   2,   82}, {   0,    0}, {   0,    0}, {   1,   67}, 
{   4,   92}, {  64,   12}, {   4,   70}, {   4,   71}, {  11,   72}, 
{  11,   73}, {  11,   74}, {  49,  110}, {   8,   11}, {  49,  110}, 
{  49,  110}, {  49,  110}, {  49,  110}, {  49,  110}, {  15,   12}, 
{  54,  125}, {  49,  110}, {   6,   86}, {  49,  110}, {  49,  110}, 
{  49,  110}, {  49,  110}, {  49,  110}, {  49,  110}, {  49,  110}, 
{  50,  111}, {   3,    4}, {  50,  111}, {  50,  111}, {  50,  111}, 
{  50,  111}, {  50,  111}, {  54,  125}, {  52,  123}, {  50,  111}, 
{   5,    9}, {  50,  111}, {  50,  111}, {  50,  111}, {  50,  111}, 
{  50,  111}, {  50,  111}, {  50,  111}, {  28,  114}, {  25,   44}, 
{  28,  114}, {  28,  114}, {  28,  114}, {  28,  114}, {  28,  114}, 
{  52,  123}, {  51,  122}, {  28,  114}, {   7,   68}, {  28,  114}, 
{  28,  114}, {  28,  114}, {  28,  114}, {  28,  114}, {  28,  114}, 
{  28,  114}, {  27,  113}, {  17,   75}, {  27,  113}, {  27,  113}, 
{  27,  113}, {  27,  113}, {  27,  113}, {  51,  122}, {  55,  126}, 
{  27,  113}, {  10,   69}, {  27,  113}, {  27,  113}, {  27,  113}, 
{  27,  113}, {  27,  113}, {  27,  113}, {  27,  113}, {  48,  109}, 
{  61,   64}, {  48,  109}, {  48,  109}, {  48,  109}, {  48,  109}, 
{  48,  109}, {  55,  126}, {  53,  124}, {  48,  109}, {  12,   18}, 
{  48,  109}, {  48,  109}, {  48,  109}, {  48,  109}, {  48,  109}, 
{  48,  109}, {  48,  109}, {  47,  108}, {  58,   42}, {  47,  108}, 
{  47,  108}, {  47,  108}, {  47,  108}, {  47,  108}, {  53,  124}, 
{  26,  101}, {  47,  108}, {  24,   42}, {  47,  108}, {  47,  108}, 
{  47,  108}, {  47,  108}, {  47,  108}, {  47,  108}, {  47,  108}, 
{  22,   30}, {  22,   31}, {  22,   32}, {  22,   33}, {  22,   34}, 
{  22,   35}, {  59,   42}, {  26,  101}, {  42,   13}, {  22,   36}, 
{  22,   37}, {  22,   38}, {  22,   39}, {  22,   40}, {  22,   41}, 
{  46,  107}, {  62,   42}, {  46,  107}, {  46,  107}, {  42,   14}, 
{  42,   15}, {  65,   42}, {  66,   42}, {  23,   42}, {  46,  107}, 
{  57,   81}, {  46,  107}, {  46,  107}, {  46,  107}, {  46,  107}, 
{  46,  107}, {  46,  107}, {  46,  107}, {  45,  106}, {  60,   62}, 
{  45,  106}, {  45,  106}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {  45,  106}, {   0,    0}, {  45,  106}, 
{  45,  106}, {  45,  106}, {  45,  106}, {  45,  106}, {  45,  106}, 
{  45,  106}, {  56,  127}, {   0,    0}, {  56,   30}, {  56,   31}, 
{  56,   32}, {  56,   33}, {  56,   34}, {  56,   35}, {  43,  116}, 
{   0,    0}, {  13,   76}, {  13,   77}, {  13,   78}, {  43,  116}, 
{  13,   79}, {   0,    0}, {   0,    0}, {  56,  127}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {  13,   19}, 
{  13,   20}, {   0,    0}, {   0,    0}, {  63,  118}, {   0,    0}, 
{  13,   21}, {  43,  116}, {   0,    0}, {  63,  118}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {  43,  116}, 
{  43,   58}, {  43,   59}, {  43,  116}, {  43,  116}, {  29,   30}, 
{  29,   31}, {  29,   32}, {  29,   33}, {  29,   34}, {  29,   35}, 
{  63,  118}, {  29,   80}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {  63,  118}, {  63,   65}, 
{   0,    0}, {  63,  118}, {  63,  118}, {   0,    0}, {   0,    0}, 
{   0,    0}, 
};
static	yytNComb	yyNComb		[yyNTableMax - yyLastTerminal] = {
{   0,    0}, {   1,    2}, {   1,   83}, {   1,    3}, {   4,    5}, 
{   4,    6}, {   4,    7}, {   0,    0}, {   0,    0}, {   9,   16}, 
{  57,   97}, {  57,   17}, {  42,   57}, {  42,   96}, {  57,   99}, 
{  58,  117}, {  44,   22}, {  57,  100}, {  57,  128}, {  57,  129}, 
{  44,   61}, {   6,   10}, {   6,    8}, {  14,   24}, {  13,   23}, 
{  43,   60}, {  36,   51}, {  21,   29}, {  35,   50}, {  34,   49}, 
{  11,   89}, {  19,   27}, {  38,   53}, {  33,   48}, {  32,   47}, 
{  31,   46}, {  30,   45}, {  37,   52}, {  39,   54}, {  18,   26}, 
{  41,   56}, {  64,   66}, {  20,   28}, {  15,   25}, {  40,   55}, 
{  60,   63}, {  23,   43}, {  66,  131}, {  65,  119}, {  62,  121}, 
{  59,  120}, {  24,  130}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, 
};
static	yytComb *	yyTBasePtr	[yyLastReadState + 1] = { 0,
& yyTComb [   0], & yyTComb [   1], & yyTComb [  25], & yyTComb [   0], 
& yyTComb [  34], & yyTComb [  16], & yyTComb [  46], & yyTComb [   8], 
& yyTComb [   0], & yyTComb [  64], & yyTComb [   0], & yyTComb [  81], 
& yyTComb [ 180], & yyTComb [   0], & yyTComb [  14], & yyTComb [   0], 
& yyTComb [  55], & yyTComb [   0], & yyTComb [   0], & yyTComb [   0], 
& yyTComb [   0], & yyTComb [ 106], & yyTComb [ 114], & yyTComb [  83], 
& yyTComb [  37], & yyTComb [  98], & yyTComb [  54], & yyTComb [  36], 
& yyTComb [ 200], & yyTComb [   0], & yyTComb [   0], & yyTComb [   0], 
& yyTComb [   0], & yyTComb [   0], & yyTComb [   0], & yyTComb [   0], 
& yyTComb [   0], & yyTComb [   0], & yyTComb [   0], & yyTComb [   0], 
& yyTComb [   0], & yyTComb [ 106], & yyTComb [ 179], & yyTComb [   0], 
& yyTComb [ 141], & yyTComb [ 123], & yyTComb [  90], & yyTComb [  72], 
& yyTComb [   0], & yyTComb [  18], & yyTComb [  44], & yyTComb [  26], 
& yyTComb [  80], & yyTComb [   8], & yyTComb [  62], & yyTComb [ 159], 
& yyTComb [ 115], & yyTComb [  74], & yyTComb [  97], & yyTComb [ 122], 
& yyTComb [  73], & yyTComb [ 107], & yyTComb [ 198], & yyTComb [   1], 
& yyTComb [ 112], & yyTComb [ 113], 
};
static	yytNComb *	yyNBasePtr	[yyLastReadState + 1] = { 0,
& yyNComb [ -36], & yyNComb [ -36], & yyNComb [ -36], & yyNComb [ -36], 
& yyNComb [ -36], & yyNComb [ -21], & yyNComb [ -36], & yyNComb [ -36], 
& yyNComb [ -36], & yyNComb [ -36], & yyNComb [ -14], & yyNComb [ -36], 
& yyNComb [ -28], & yyNComb [ -29], & yyNComb [  -4], & yyNComb [ -36], 
& yyNComb [ -36], & yyNComb [  -9], & yyNComb [ -17], & yyNComb [  -6], 
& yyNComb [ -21], & yyNComb [ -36], & yyNComb [  -3], & yyNComb [   2], 
& yyNComb [ -36], & yyNComb [ -36], & yyNComb [ -36], & yyNComb [ -36], 
& yyNComb [ -36], & yyNComb [ -12], & yyNComb [ -13], & yyNComb [ -14], 
& yyNComb [ -15], & yyNComb [ -19], & yyNComb [ -20], & yyNComb [ -22], 
& yyNComb [ -11], & yyNComb [ -16], & yyNComb [ -10], & yyNComb [  -4], 
& yyNComb [  -8], & yyNComb [ -33], & yyNComb [ -26], & yyNComb [ -32], 
& yyNComb [ -36], & yyNComb [ -36], & yyNComb [ -36], & yyNComb [ -36], 
& yyNComb [ -36], & yyNComb [ -36], & yyNComb [ -36], & yyNComb [ -36], 
& yyNComb [ -36], & yyNComb [ -36], & yyNComb [ -36], & yyNComb [ -36], 
& yyNComb [ -36], & yyNComb [ -34], & yyNComb [   1], & yyNComb [  -4], 
& yyNComb [ -36], & yyNComb [   0], & yyNComb [ -36], & yyNComb [  -6], 
& yyNComb [  -1], & yyNComb [  -2], 
};
# ifdef YYTDefault
static	unsigned short	yyTDefault	[yyLastReadState + 1] = { 0,
    0,     0,     0,     0,     0,     4,     0,     0,    42,     0, 
    0,     0,     0,    13,     0,    42,     0,    13,    13,    13, 
   13,     0,     0,     0,     0,    56,    49,    49,     0,    13, 
   13,    13,    13,    13,    13,    13,    13,    13,    13,    13, 
   13,    15,     0,    13,    22,    45,    49,    49,    45,    49, 
   56,    56,    56,    56,    56,     0,    42,     0,     0,    24, 
    0,     0,     0,     0,     0,     0, 
};
# endif
# ifdef YYNDefault
static	unsigned short	yyNDefault	[yyLastReadState + 1] = { 0,
    0,     0,     0,     6,     0,     0,     0,     0,    42,     0, 
    0,     0,    44,    44,     0,    57,     0,     0,     0,     0, 
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0, 
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0, 
    0,    57,     0,     0,     0,     0,     0,     0,     0,     0, 
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0, 
    0,     0,     0,     0,     0,     0, 
};
# endif
# if ! defined NO_RECOVER | defined YYDEC_TABLE | defined YYCSTree
static	unsigned char	yyLength	[yyLastReduceState - yyFirstReduceState
							+ 1] = {
    2,     1,     5,     1,     1,     2,     3,     3,     1,     1, 
    0,     1,     1,     1,     1,     2,     2,     1,     1,     3, 
    1,     1,     1,     1,     3,     3,     3,     3,     3,     3, 
    3,     2,     2,     3,     3,     5,     5,     7,     2,     3, 
    3,     3,     3,     3,     3,     3,     1,     1,     3,     7, 
};
# endif
# if ! defined NO_RECOVER | defined YYDEC_TABLE
static	yytNonterminal	yyLeftHandSide	[yyLastReduceState - yyFirstReduceState
							+ 1] = {
yyNT0_intern,
yyNTroot,
yyNTProgramm,
yyNTNamenTeil,
yyNTVariablenTeil,
yyNTDeclarationList,
yyNTDeclarationList,
yyNTVariable,
yyNTIOFlag,
yyNTIOFlag,
yyNTIOFlag,
yyNTVariablenTyp,
yyNTVariablenTyp,
yyNTVariablenTyp,
yyNTCodeTeil,
yyNTCodeTeil,
yyNTInstruction,
yyNTInstruction,
yyNTInstruction,
yyNTAssignment,
yyNTExpression,
yyNTExpression,
yyNTExpression,
yyNTExpression,
yyNTExpression,
yyNTExpression,
yyNTExpression,
yyNTExpression,
yyNTExpression,
yyNTExpression,
yyNTExpression,
yyNTExpression,
yyNTExpression,
yyNTCodeBlock,
yyNTCondition,
yyNTCondition,
yyNTCondition,
yyNTCondition,
yyNTElseIfList,
yyNTElseIfList,
yyNTComparison,
yyNTComparison,
yyNTComparison,
yyNTComparison,
yyNTComparison,
yyNTComparison,
yyNTLoop,
yyNTLoop,
yyNTWhileLoop,
yyNTForLoop,
};
# endif
# ifndef NO_RECOVER
static	yySymbolRange	yyContinuation	[yyLastReadState + 1] = { 0,
    4,     0,     6,     5,     6,     6,    12,     5,     5,    12, 
   11,    13,     5,     5,     5,     0,    12,     5,     5,     5, 
    5,    28,    29,    29,    12,    12,    12,    12,    21,     5, 
    5,     5,     5,     5,     5,     5,     5,     5,     5,     5, 
    5,     5,     0,     5,    12,    12,    12,    12,    12,    12, 
   12,    12,    12,    12,    12,    12,    30,    29,    29,    29, 
   12,    29,     0,     5,    29,    29, 
};
static	unsigned short	yyCondition	[yyLastState - yyLastReduceState + 1] =
{ 0,
};
# endif
static	unsigned short	yyFinalToProd	[yyLastReadReduceState -
						yyFirstReadReduceState + 2] = {
   85,    87,    88,    90,    91,    93,    94,    95,    98,   102, 
  103,   104,   105,   112,   115, 
0
};
static	unsigned short	yyStartLine	[yyLastStopState - yyFirstReduceState
							+ 2] = { 0,
91,
};
# ifdef YYaccDefault

static	unsigned long *	yyDefaultLook	[yyLastReadState + 1] = { 0,
};
# endif


static	int	yyParse		ARGS ((yyStateRange, yySymbolRange, int));

# ifndef NO_RECOVER
static	yyStateRange yyNext	ARGS ((yyStateRange, yySymbolRange));
#  ifdef YYDEBUG
static	int	yyErrorRecovery	ARGS ((yySymbolRange, yySymbolRange *, long));
#  else
static	int	yyErrorRecovery	ARGS ((yySymbolRange, long));
#  endif
static	void	yyComputeContinuation	ARGS ((long, tSet *));
static	rbool	yyIsContinuation	ARGS ((yySymbolRange, long));
static	void	yyComputeRestartPoints	ARGS ((long, tSet *));
# endif

# if defined YYTrialParse | defined YYReParse | defined YYGetLook

# ifndef yyInitBufferSize
#  define yyInitBufferSize 100
# endif

typedef struct { yySymbolRange	yyToken;
		 xlang_scan_tScanAttribute	yyAttribute;
# ifdef YYMemoParse
		 short		yyStart;
# endif
	       } yytBuffer;

static yytBuffer *	yyBuffer	= NULL;
static unsigned long	yyBufferSize	= yyInitBufferSize;
static long		yyBufferNext	= 1;
static long		yyBufferLast	= 1;
static rbool		yyBufferClear	= rtrue;
static unsigned short	yyParseLevel	= 0;

static void yyBufferSet
# ifdef HAVE_ARGS
   (yySymbolRange yyToken)
# else
   (yyToken) yySymbolRange yyToken;
# endif
{
   if (yyBufferNext == yyBufferLast) {
      if (yyBufferClear) yyBufferLast = 0;
      if (++ yyBufferLast >= (long) yyBufferSize) {
	 yyExtendArray ((char * *) & yyBuffer, & yyBufferSize,
			  (unsigned long) sizeof (yytBuffer));
# ifdef YYDEBUG
	 if (xlang_pars_Debug) {
	    yyPrintState (0);
	    fprintf (yyTrace, "extend  token buffer from %ld to %ld",
		yyBufferSize / 2, yyBufferSize); yyNl ();
	 }
# endif
      }
      yyBuffer [yyBufferLast].yyToken	= yyToken;
      yyBuffer [yyBufferLast].yyAttribute= xlang_scan_Attribute;
# ifdef YYMemoParse
      yyBuffer [yyBufferLast].yyStart	= 0;
# endif
      yyBufferNext = yyBufferLast;
   }
}

static int yyGetToken ARGS ((void))
{
   register yySymbolRange yyToken;

   if (yyBufferNext < yyBufferLast) {
      yyToken = yyBuffer [++ yyBufferNext].yyToken;
      xlang_scan_Attribute = yyBuffer [yyBufferNext].yyAttribute;
   } else {
      yyToken = xlang_scan_GetToken ();
      if ((yytrial | yybuffer) & yyControl.yyMode) {
	 if (++ yyBufferLast >= (long) yyBufferSize) {
	    yyExtendArray ((char * *) & yyBuffer, & yyBufferSize,
			     (unsigned long) sizeof (yytBuffer));
# ifdef YYDEBUG
	    if (xlang_pars_Debug) {
	       yyPrintState (0);
	       fprintf (yyTrace, "extend  token buffer from %ld to %ld",
		  yyBufferSize / 2, yyBufferSize); yyNl ();
	    }
# endif
	 }
	 yyBuffer [yyBufferLast].yyToken = yyToken;
	 yyBuffer [yyBufferLast].yyAttribute = xlang_scan_Attribute;
# ifdef YYMemoParse
	 yyBuffer [yyBufferLast].yyStart = 0;
# endif
	 yyBufferNext = yyBufferLast;
      }
   }
# ifdef TOKENOP
   TOKENOP
# endif
   return yyToken;
}

# else
# define yyGetToken xlang_scan_GetToken
# endif

# ifdef YYGetLook

static int yyGetLookahead
# ifdef HAVE_ARGS
   (int yyk, yySymbolRange yyToken)
# else
   (yyk, yyToken) int yyk; yySymbolRange yyToken;
# endif
{
   if (yyk == 0) return yyToken;
   if (yyControl.yyMode == yystandard) yyBufferSet (yyToken);
   while (yyBufferNext + yyk > yyBufferLast) {
      if (yyBufferNext < yyBufferLast &&
	 yyBuffer [yyBufferLast].yyToken == xlang_scan_EofToken)
	 return xlang_scan_EofToken;
      if (++ yyBufferLast >= (long) yyBufferSize) {
	 yyExtendArray ((char * *) & yyBuffer, & yyBufferSize,
			  (unsigned long) sizeof (yytBuffer));
# ifdef YYDEBUG
	 if (xlang_pars_Debug) {
	    yyPrintState (0);
	    fprintf (yyTrace, "extend  token buffer from %ld to %ld",
		yyBufferSize / 2, yyBufferSize); yyNl ();
	 }
# endif
      }
      yyBuffer [yyBufferLast].yyToken = xlang_scan_GetToken ();
      yyBuffer [yyBufferLast].yyAttribute = xlang_scan_Attribute;
# ifdef YYMemoParse
      yyBuffer [yyBufferLast].yyStart = 0;
# endif
   }
   xlang_scan_Attribute = yyBuffer [yyBufferNext].yyAttribute;
   return yyBuffer [yyBufferNext + yyk].yyToken;
}

static void xxGetAttribute
# ifdef HAVE_ARGS
   (int yyk, yySymbolRange yyToken, xlang_scan_tScanAttribute * yyAttribute)
# else
   (yyk, yyToken, yyAttribute)
   int yyk; yySymbolRange yyToken; xlang_scan_tScanAttribute * yyAttribute;
# endif
{
   if (yyk == 0) * yyAttribute = xlang_scan_Attribute;
   else {
      yyGetLookahead (yyk, yyToken);
      * yyAttribute =
	 yyBuffer [Min (yyBufferNext + yyk, yyBufferLast)].yyAttribute;
   }
}

# endif

# ifdef YYReParse

#  define BufferOn(Actions, Messages) yyBufferOn (Actions, Messages, yyTerminal)
#  define BufferPosition yyBufferNext

static yytControl yyPrevControl;

static long yyBufferOn
# ifdef HAVE_ARGS
   (rbool yyActions, rbool yyMessages, yySymbolRange yyToken)
# else
   (yyActions, yyMessages, yyToken)
   rbool yyActions, yyMessages; yySymbolRange yyToken;
# endif
{
   if (yyControl.yyMode == yystandard) {
      yyPrevControl		= yyControl;
      yyControl.yyMode		= yybuffer;
      yyControl.yyActions	= yyActions;
      yyControl.yyMessages	= yyMessages;
      yyBufferSet (yyToken);
      yyBufferClear		= rfalse;
   }
   return yyBufferNext;
}

static long BufferOff ARGS ((void))
{
   if (yyControl.yyMode == yybuffer) yyControl = yyPrevControl;
   return yyBufferNext;
}

static void BufferClear ARGS ((void))
{
   yyBufferClear = rtrue;
}

# endif

# if defined YYDEBUG | defined YYDCRP

static void yyNl ARGS ((void)) { putc ('\n', yyTrace); fflush (yyTrace); }

# endif

# ifdef YYDEBUG

static void yyPrintState
# ifdef HAVE_ARGS
   (yyStateRange yyState)
# else
   (yyState) yyStateRange yyState;
# endif
{
   fprintf (yyTrace, "%4ld:", ++ yyCount);
   WritePosition  (yyTrace, xlang_scan_Attribute.Position);
   fprintf (yyTrace, ":%5d  %c  ", yyState, " ST-B---R" [yyControl.yyMode]);
# if defined YYTrialParse | defined YYReParse
   if (yyParseLevel > 0) {
      register int yyi = yyParseLevel;
      fprintf (yyTrace, "%2d  ", yyi);
      do fputs ("  ", yyTrace); while (-- yyi);
   } else
# endif
   fputs ("    ", yyTrace);
}

static rbool yyPrintResult
# ifdef HAVE_ARGS
   (yyStateRange yyState, int yyLine, rbool yyCondition)
# else
   (yyState, yyLine, yyCondition)
   yyStateRange	yyState;
   int		yyLine;
   rbool	yyCondition;
# endif
{
   if (xlang_pars_Debug) {
      yyPrintState (yyState);
      fprintf (yyTrace, "check   predicate in line %d, result = %d",
	 yyLine, yyCondition); yyNl ();
   }
   return yyCondition;
}

# else
#  define yyPrintResult(State, Line, Condition) Condition
# endif

# if defined YYDEBUG | defined YYDEC_TABLE
#  define yyGotoReduce(State, Rule)	{ yyState = State; goto yyReduce; }
#  define yyGotoRead(State)		{ yyState = State; goto yyRead; }
# else
#  define yyGotoReduce(State, Rule)	goto Rule;
#  define yyGotoRead(State)		{ yyState = State; goto yyRead; }
# endif

static unsigned long	yyStateStackSize	= yyInitStackSize;
static yyStateRange *	yyStateStack		= NULL;
static yyStateRange *	yyEndOfStack		;
static unsigned long	yyAttrStackSize		= yyInitStackSize;
static tParsAttribute * yyAttributeStack	= NULL;
# if defined YYTrialParse | defined YYReParse
static yyStateRange *	yyStateStackPtr		;
static tParsAttribute * yyAttrStackPtr		;
# endif
# ifndef NO_RECOVER
static yyStateRange *	yyIsContStackPtr	= NULL;
static unsigned long	yyIsContStackSize	= yyInitStackSize;
static yyStateRange *	yyCompResStackPtr	= NULL;
static unsigned long	yyCompResStackSize	= yyInitStackSize;
# endif

void Resetxlang_pars ARGS ((void))
   {
      if (yyStateStack) {
	 ReleaseArray ((char * *) & yyStateStack, & yyStateStackSize,
			(unsigned long) sizeof (yyStateRange));
	 yyStateStack = NULL;
      }
      if (yyAttributeStack) {
	 ReleaseArray ((char * *) & yyAttributeStack, & yyAttrStackSize,
			(unsigned long) sizeof (tParsAttribute));
	 yyAttributeStack = NULL;
      }
# ifndef NO_RECOVER
      if (yyIsContStackPtr) {
	 ReleaseArray ((char * *) & yyIsContStackPtr, & yyIsContStackSize,
			(unsigned long) sizeof (yyStateRange));
	 yyIsContStackPtr = NULL;
      }
      if (yyCompResStackPtr) {
	 ReleaseArray ((char * *) & yyCompResStackPtr, & yyCompResStackSize,
			(unsigned long) sizeof (yyStateRange));
	 yyCompResStackPtr = NULL;
      }
# endif
# if defined YYTrialParse | defined YYReParse | defined YYGetLook
      if (yyBuffer) {
	 ReleaseArray ((char * *) & yyBuffer, & yyBufferSize,
			(unsigned long) sizeof (yytBuffer));
	 yyBuffer = NULL;
      }
# endif
   }

int xlang_pars ARGS ((void))
   {
      return xlang_pars2 (yyStartState);
   }

int xlang_pars2
# ifdef HAVE_ARGS
   (int yyStartSymbol)
# else
   (yyStartSymbol) int yyStartSymbol;
# endif
   {
      int		yyErrorCount;
# if defined YYDEBUG | defined YYDCRP
      yyTrace		= stdout;
# endif
      Beginxlang_pars ();
      yyStateStackSize	= yyInitStackSize;
      yyMakeArray ((char * *) & yyStateStack, & yyStateStackSize,
		     (unsigned long) sizeof (yyStateRange));
      yyAttrStackSize	= yyInitStackSize;
      yyMakeArray ((char * *) & yyAttributeStack, & yyAttrStackSize,
		     (unsigned long) sizeof (tParsAttribute));
# ifndef NO_RECOVER
      yyIsContStackSize	= yyInitStackSize;
      yyMakeArray ((char * *) & yyIsContStackPtr, & yyIsContStackSize,
		     (unsigned long) sizeof (yyStateRange));
      yyCompResStackSize= yyInitStackSize;
      yyMakeArray ((char * *) & yyCompResStackPtr, & yyCompResStackSize,
		     (unsigned long) sizeof (yyStateRange));
# endif
# if defined YYTrialParse | defined YYReParse | defined YYGetLook
      yyBufferSize	= yyInitBufferSize;
      yyMakeArray ((char * *) & yyBuffer, & yyBufferSize,
		     (unsigned long) sizeof (yytBuffer));
# endif
      yyEndOfStack	= & yyStateStack [yyStateStackSize - 1];
# if defined YYTrialParse | defined YYReParse
      yyStateStackPtr	= yyStateStack;
      yyAttrStackPtr	= yyAttributeStack;
      yyBufferNext	= 1;
      yyBufferLast	= 1;
      yyBufferClear	= rtrue;
      yyParseLevel	= 0;
# endif
# ifdef YYDEBUG
      if (xlang_pars_Debug) {
	 yyCount = 0;
	 fprintf (yyTrace,
      "  # |Position|State|Mod|Lev|Action |Terminal and Lookahead or Rule\n");
	 yyNl ();
      }
# endif
      yyControl.yyMode		= yystandard;
      yyControl.yyActions	= rtrue;
      yyControl.yyMessages	= rtrue;
      yyErrorCount = yyParse ((yyStateRange) yyStartSymbol,
	 (yySymbolRange) yyGetToken (), (int) yyStartLine [yyStartSymbol]);
      Resetxlang_pars ();
      return yyErrorCount;
   }

# ifdef YYTrialParse

# ifdef YYMemoParse
#  define MemoryClear(Position) yyBuffer [Position].yyStart = 0
# endif

static int yyTrialParse
# ifdef HAVE_ARGS
   (yyStateRange yyStartSymbol, yySymbolRange yyToken, int yyLine)
# else
   (yyStartSymbol, yyToken, yyLine)
   yyStateRange		yyStartSymbol	;
   yySymbolRange	yyToken		;
   int			yyLine		;
# endif
   {
      int	yyErrorCount		;
      ptrdiff_t	yyPrevStateStackPtr	= yyStateStackPtr - yyStateStack;
      ptrdiff_t	yyPrevAttrStackPtr	= yyAttrStackPtr - yyAttributeStack;
      long	yyPrevBufferNext	;
      yytControl yyPrevControl		;

# ifdef BEFORE_TRIAL
      BEFORE_TRIAL
# endif
# ifdef YYMemoParse
      if (yyBuffer [yyBufferNext].yyStart ==   yyStartSymbol) return 0;
      if (yyBuffer [yyBufferNext].yyStart == - yyStartSymbol) return 1;
# endif
      yyPrevControl		= yyControl;
      yyStateStackPtr		++;
      yyAttrStackPtr		++;
      yyParseLevel		++;
      if (yyControl.yyMode == yystandard) yyBufferSet (yyToken);
      yyPrevBufferNext		= yyBufferNext;
      yyControl.yyMode		= yytrial;
      yyControl.yyActions	= rfalse;
      yyControl.yyMessages	= rfalse;
      yyErrorCount		= yyParse (yyStartSymbol, yyToken, yyLine);
# ifdef YYMemoParse
      yyBuffer [yyPrevBufferNext].yyStart = yyErrorCount ?
					- yyStartSymbol : yyStartSymbol;
# endif
      yyStateStackPtr		= yyStateStack + yyPrevStateStackPtr;
      yyAttrStackPtr		= yyAttributeStack + yyPrevAttrStackPtr;
      yyBufferNext		= yyPrevBufferNext;
      yyControl			= yyPrevControl;
      yyParseLevel		--;
      xlang_scan_Attribute		= yyBuffer [yyBufferNext].yyAttribute;
# ifdef AFTER_TRIAL
      AFTER_TRIAL
# endif
      return yyErrorCount;
   }

# endif

# ifdef YYReParse

static int ReParse
# ifdef HAVE_ARGS
   (yyStateRange yyStartSymbol, int yyFrom, int yyTo, rbool yyActions,
      rbool yyMessages)
# else
   (yyStartSymbol, yyFrom, yyTo, yyActions, yyMessages)
   yyStateRange	yyStartSymbol		;
   int		yyFrom, yyTo		;
   rbool	yyActions, yyMessages	;
# endif
   {
      int yyErrorCount = 1;

      if (1 <= yyFrom && yyFrom <= yyTo && yyTo <= yyBufferLast) {
	 ptrdiff_t yyPrevStateStackPtr	= yyStateStackPtr - yyStateStack;
	 ptrdiff_t yyPrevAttrStackPtr	= yyAttrStackPtr - yyAttributeStack;
	 long	yyPrevBufferNext	= yyBufferNext;
	 int	yyToToken		= yyBuffer [yyTo].yyToken;
	 yytControl yyPrevControl	;

	 yyPrevControl		= yyControl;
	 yyStateStackPtr	++;
	 yyAttrStackPtr		++;
	 yyParseLevel		++;
	 yyBufferNext		= yyFrom - 1;
	 yyBuffer [yyTo].yyToken= xlang_scan_EofToken;
	 yyControl.yyMode	= yyreparse;
	 yyControl.yyActions	= yyActions;
	 yyControl.yyMessages	= yyMessages;
	 yyErrorCount		= yyParse (yyStartSymbol,
	    (yySymbolRange) yyGetToken (), (int) yyStartLine [yyStartSymbol]);
	 yyStateStackPtr	= yyStateStack + yyPrevStateStackPtr;
	 yyAttrStackPtr		= yyAttributeStack + yyPrevAttrStackPtr;
	 yyBufferNext		= yyPrevBufferNext;
	 yyControl		= yyPrevControl;
	 yyParseLevel		--;
	 yyBuffer [yyTo].yyToken= yyToToken;
	 xlang_scan_Attribute		= yyBuffer [yyBufferNext].yyAttribute;
      } else {
      ErrorMessage (xxInvalidCallOfReParse, xxError, xlang_scan_Attribute.Position);
      }
      return yyErrorCount;
   }

# endif

static char * yyGetTokenName
# ifdef HAVE_ARGS
   (yySymbolRange yyTerminal)
# else
   (yyTerminal) yySymbolRange yyTerminal;
# endif
   {
      if (yyTerminal <= yyLastTerminal && xlang_pars_TokenName [yyTerminal])
	 return xlang_pars_TokenName [yyTerminal];
      else
	 return "_unknown_";
   }

static void yyExtendStack
# ifdef HAVE_ARGS
   (yyStateRange * *	yyStateStackPtr,
    tParsAttribute * *	yyAttrStackPtr,
    yyStateRange	yyState)
# else
   (yyStateStackPtr, yyAttrStackPtr, yyState)
   yyStateRange * *	yyStateStackPtr;
   tParsAttribute * *	yyAttrStackPtr;
   yyStateRange		yyState;
# endif
   {
      ptrdiff_t yyyStateStackPtr = * yyStateStackPtr - yyStateStack;
      ptrdiff_t yyyAttrStackPtr = * yyAttrStackPtr - yyAttributeStack;
      yyExtendArray ((char * *) & yyStateStack, & yyStateStackSize,
		       (unsigned long) sizeof (yyStateRange));
      yyExtendArray ((char * *) & yyAttributeStack, & yyAttrStackSize,
		       (unsigned long) sizeof (tParsAttribute));
      * yyStateStackPtr	= yyStateStack + yyyStateStackPtr;
      * yyAttrStackPtr	= yyAttributeStack + yyyAttrStackPtr;
      yyEndOfStack	= & yyStateStack [yyStateStackSize - 1];
# ifdef YYDEBUG
      if (xlang_pars_Debug) {
	 yyPrintState (yyState);
	 fprintf (yyTrace, "extend  stack from %ld to %ld",
	    yyStateStackSize / 2, yyStateStackSize); yyNl ();
      }
# endif
   }

static int yyParse
# ifdef HAVE_ARGS
   (yyStateRange yyStartSymbol, yySymbolRange yyToken, int yyLine)
# else
   (yyStartSymbol, yyToken, yyLine)
   yyStateRange		yyStartSymbol	;
   yySymbolRange	yyToken		;
   int			yyLine		;
# endif
   {
      register	yyStateRange	yyState		= yyStartSymbol;
      register	yySymbolRange	yyTerminal	= yyToken;
      register	rbool		yyIsRepairing	= rfalse;
		tParsAttribute	yySynAttribute	;   /* synthesized attribute */
		int		yyErrorCount	= 0;
# if ! (defined YYTrialParse | defined YYReParse)
		yyStateRange *	yyStateStackPtr	= yyStateStack;
		tParsAttribute *yyAttrStackPtr	= yyAttributeStack;
# endif
# ifdef YYDEBUG
		long		yyStartCount	= yyCount + 1;
		yySymbolRange	yyPrevTerminal	= yyToken;
# endif
# ifdef YYGetLook
		yySymbolRange	yy2;
# endif

/* line 35 "xlang_pars.lrk" */


/* line 1063 "xlang_pars.c" */

# ifdef YYDEBUG
      if (xlang_pars_Debug) {
	 yyPrintState (yyStartSymbol);
	 fprintf (yyTrace, "parse   for predicate in line %d, lookahead: %s",
	    yyLine, yyGetTokenName (yyTerminal)); yyNl ();
      }
# endif

      memset (& yySynAttribute, 0, sizeof yySynAttribute);

      for (;;) {
	 if (yyStateStackPtr >= yyEndOfStack) {
	    yyExtendStack (& yyStateStackPtr, & yyAttrStackPtr, yyState);
	 }
	 * yyStateStackPtr = yyState;

	 for (;;) { /* SPEC State = Next (State, Terminal); terminal transit */
	    register yytComb * yyTCombPtr = yyTBasePtr [yyState] + yyTerminal;
# if defined YYTDefault & defined YYaccDefault
	    register unsigned long * yylp;
# endif
	    if (yyTCombPtr->Check == yyState) {
	       yyState = yyTCombPtr->Next; break;
	    }
# ifdef YYTDefault
#  ifdef YYaccDefault
	    if ((yylp = yyDefaultLook [yyState]) &&
	       (yylp [yyTerminal >> 5] >> (yyTerminal & 0x1f)) & 1) {
	       yyState = yyTDefault [yyState]; break;
	    }
	    yyState = yyNoState; break;
#  else
	    if ((yyState = yyTDefault [yyState]) == yyNoState) break;
#  endif
# endif
	 }

	 if (yyState == yyNoState ||			/* syntax error ? */
	    yyIsRepairing && yyState >= yyFirstReduceState) { /* repair mode */
	    yySymbolRange yyRepairToken;

	    if (yyState == yyNoState && ! yyIsRepairing) { /* syntax error */
# ifdef ERROR
	       for (;;) {		/* be compatible with error trick */
		  yySymbolRange yyyTerminal = (yySymbolRange) yyTerminal;
		  ERROR
		  goto yyRecover;
	       }
	       goto yyFinal;
   yyRecover:
# endif
# ifdef ERROR2
	       ERROR2
# endif
# ifdef YYTrialParse
	       if (yyControl.yyMode == yytrial) YYABORT;
# endif
# ifndef NO_RECOVER
	       yyErrorCount ++;				/* report and recover */
	       yyTerminal = yyErrorRecovery (yyTerminal,
#  ifdef YYDEBUG
			& yyPrevTerminal,
#  endif
			(long) (yyStateStackPtr - yyStateStack));
	       yyIsRepairing = rtrue;
	       yyState = * yyStateStackPtr;
	       continue;
# else
	       YYABORT;
# endif
	    }

# ifndef NO_RECOVER
	/* SPEC State = Next (Top (), Continuation [yyState]); repair transit */
	    yyState = * yyStateStackPtr;
	    yyRepairToken = yyContinuation [yyState];	/* repair */
	    yyState = yyNext (yyState, yyRepairToken);
	    if (yyState > yyLastReduceState) {		/* dynamic ? */
	       yyState = yyCondition [yyState - yyLastReduceState];
	    }
	    if (yyState <= yyLastReadReduceState) { /* read or read reduce ? */
	       xlang_scan_tScanAttribute yyRepairAttribute;
	       xlang_scan_ErrorAttribute ((int) yyRepairToken, & yyRepairAttribute);
	       if (yyControl.yyMessages)
		  ErrorMessageI (xxTokenInserted, xxRepair,
		     xlang_scan_Attribute.Position, xxString,
		     yyGetTokenName (yyRepairToken));
#  ifdef YYDEBUG
	       if (xlang_pars_Debug) {
		  yyPrintState (* yyStateStackPtr);
		  fprintf (yyTrace, "insert  %s",
		     yyGetTokenName (yyRepairToken)); yyNl ();
		  yyPrintState (* yyStateStackPtr);
		  fprintf (yyTrace, "shift   %s, lookahead: %s",
		     yyGetTokenName (yyRepairToken),
		     yyGetTokenName (yyTerminal)); yyNl ();
	       }
#  endif
	       if (yyStateStackPtr >= yyEndOfStack) {
		  yyExtendStack (& yyStateStackPtr, & yyAttrStackPtr, yyState);
	       }
	       yyStateStackPtr ++;
	       yyGetAttribute (yyAttrStackPtr ++, yyRepairAttribute);
	       if (yyState >= yyFirstFinalState) {	/* read reduce */
		  * yyStateStackPtr = yyStateStackPtr [-1];
		  yyState = yyFinalToProd [yyState - yyFirstReadReduceState];
	       } else {					/* read */
		  * yyStateStackPtr = yyState;
	       }
	    }
	    if (yyState <= yyLastReadState) continue;	/* read ? */
# endif
	 }

   yyFinal:
	 if (yyState >= yyFirstFinalState) {		/* final state ? */
	    if (yyState <= yyLastReadReduceState) {	/* read reduce ? */
	       yyStateStackPtr ++;
	       yyGetAttribute (yyAttrStackPtr ++, xlang_scan_Attribute);
	       yyTerminal = yyGetToken ();
# ifdef YYDEBUG
	       if (xlang_pars_Debug) {
		  yyStateStackPtr [0] = yyStateStackPtr [-1];
		  yyPrintState (* yyStateStackPtr);
		  fprintf (yyTrace, "shift   %s, lookahead: %s",
		     yyGetTokenName (yyPrevTerminal),
		     yyGetTokenName (yyTerminal)); yyNl ();
		  yyPrevTerminal = yyTerminal;
	       }
# endif
	       yyIsRepairing = rfalse;
	    }

	    for (;;) {
	       register yytNonterminal yyNonterminal;	/* left-hand side */

   yyReduce:
# ifdef YYDEBUG
	       if (xlang_pars_Debug) {
		  if (yyState <= yyLastReadReduceState)	/* read reduce ? */
		     yyState = yyFinalToProd [yyState - yyFirstReadReduceState];
		  yyPrintState (* yyStateStackPtr);
		  if (yyState <= yyLastReduceState) {
		     fprintf (yyTrace, "reduce  %s",
			yyRule [yyState - yyLastReadReduceState]); yyNl ();
		  } else {
		     fprintf (yyTrace, "dynamic decision %d",
			yyState - yyLastReduceState); yyNl ();
		  }
	       }
# endif
# ifdef YYCSTree
	       if (yyControl.yyActions) {
		 yyStateRange yystate = yyState;
		 if (yystate <= yyLastReadReduceState) { /* read reduce ? */
		   yystate = yyFinalToProd [yystate - yyFirstReadReduceState];
		 }
		 if (yystate <= yyLastStopState) {		/* accept */
		   CSTree = NewNonterminal (yystate - yyLastReadReduceState,
		     yyAttrStackPtr [-1].Common.Tree);
		 } else if (yystate <= yyLastReduceState) {	/* reduce */
		   int yyrule = yystate - yyLastReadReduceState;
		   int yyleng = yyLength [yyrule - 1], i;
		   tCSTree yynode = NULL, yynext = NULL;
		   for (i = yyleng - 1; i >= 0; i --) {
		     unsigned short yysymbol = yyRules [yyrule] [i];
		     tParsAttribute * yyap = & yyAttrStackPtr [i - yyleng];
		     if (yysymbol < yyEpsilon) {		/* terminal */
		       yynode = NewTerminal (yyap->Scan.Position,
			 StGetCStr (yyap->Scan.Common.Text));
		     } else {					/* nonterminal*/
		       yynode = yyap->Common.Tree;
		     }
		     yynode->Next = yynext;
		     if (yynext) yynext->Previous = yynode;
		     yynext = yynode;
		   }
		   yySynAttribute.Common.Tree = NewNonterminal (yyrule, yynode);
		 }
	       }
# endif
# ifdef YYDEC_TABLE
	       if (yyLastStopState < yyState && yyState <= yyLastReduceState) {
		  register int yyd = yyLength [yyState - yyFirstReduceState];
		  yyStateStackPtr -= yyd;
		  yyAttrStackPtr  -= yyd;
		  yyNonterminal = yyLeftHandSide [yyState - yyFirstReduceState];
	       }
# endif
switch (yyState) {
case 82:
YYACCEPT;
case 83: /* root : Programm */
yyDecrement (1) yySetNT (yyNTroot) {
/* line 1259 "xlang_pars.c" */
} break;
case 84: /* Programm : NamenTeil separator VariablenTeil separator \
CodeTeil */
yyDecrement (5) yySetNT (yyNTProgramm) {
/* line 1264 "xlang_pars.c" */
} break;
case 85:
case 67: /* NamenTeil : prog_name */
yyDecrement (1) yySetNT (yyNTNamenTeil) {
/* line 1269 "xlang_pars.c" */
} break;
case 86: /* VariablenTeil : DeclarationList */
yyDecrement (1) yySetNT (yyNTVariablenTeil) {
/* line 1273 "xlang_pars.c" */
} break;
case 87:
case 68: /* DeclarationList : Variable ';' */
yyDecrement (2) yySetNT (yyNTDeclarationList) {
/* line 1278 "xlang_pars.c" */
} break;
case 88:
case 69: /* DeclarationList : DeclarationList Variable ';' */
yyDecrement (3) yySetNT (yyNTDeclarationList) {
/* line 1283 "xlang_pars.c" */
} break;
case 89: /* Variable : IOFlag identifier VariablenTyp */
yyDecrement (3) yySetNT (yyNTVariable) {
/* line 1287 "xlang_pars.c" */
} break;
case 90:
case 70: /* IOFlag : '->' */
yyDecrement (1) yySetNT (yyNTIOFlag) {
/* line 1292 "xlang_pars.c" */
} break;
case 91:
case 71: /* IOFlag : '<-' */
yyDecrement (1) yySetNT (yyNTIOFlag) {
/* line 1297 "xlang_pars.c" */
} break;
case 92: /* IOFlag : */
yySetNT (yyNTIOFlag) {
/* line 1301 "xlang_pars.c" */
} break;
case 93:
case 72: /* VariablenTyp : 'int' */
yyDecrement (1) yySetNT (yyNTVariablenTyp) {
/* line 1306 "xlang_pars.c" */
} break;
case 94:
case 73: /* VariablenTyp : 'float' */
yyDecrement (1) yySetNT (yyNTVariablenTyp) {
/* line 1311 "xlang_pars.c" */
} break;
case 95:
case 74: /* VariablenTyp : 'string' */
yyDecrement (1) yySetNT (yyNTVariablenTyp) {
/* line 1316 "xlang_pars.c" */
} break;
case 96: /* CodeTeil : Instruction */
yyDecrement (1) yySetNT (yyNTCodeTeil) {
/* line 1320 "xlang_pars.c" */
} break;
case 97: /* CodeTeil : CodeTeil Instruction */
yyDecrement (2) yySetNT (yyNTCodeTeil) {
/* line 1324 "xlang_pars.c" */
} break;
case 98:
case 75: /* Instruction : Assignment ';' */
yyDecrement (2) yySetNT (yyNTInstruction) {
/* line 1329 "xlang_pars.c" */
} break;
case 99: /* Instruction : Condition */
yyDecrement (1) yySetNT (yyNTInstruction) {
/* line 1333 "xlang_pars.c" */
} break;
case 100: /* Instruction : Loop */
yyDecrement (1) yySetNT (yyNTInstruction) {
/* line 1337 "xlang_pars.c" */
} break;
case 101: /* Assignment : identifier '=' Expression */
yyDecrement (3) yySetNT (yyNTAssignment) {
/* line 1341 "xlang_pars.c" */
} break;
case 102:
case 76: /* Expression : int_const */
yyDecrement (1) yySetNT (yyNTExpression) {
/* line 1346 "xlang_pars.c" */
} break;
case 103:
case 77: /* Expression : float_const */
yyDecrement (1) yySetNT (yyNTExpression) {
/* line 1351 "xlang_pars.c" */
} break;
case 104:
case 78: /* Expression : string_const */
yyDecrement (1) yySetNT (yyNTExpression) {
/* line 1356 "xlang_pars.c" */
} break;
case 105:
case 79: /* Expression : identifier */
yyDecrement (1) yySetNT (yyNTExpression) {
/* line 1361 "xlang_pars.c" */
} break;
case 106: /* Expression : Expression '+' Expression */
yyDecrement (3) yySetNT (yyNTExpression) {
/* line 1365 "xlang_pars.c" */
} break;
case 107: /* Expression : Expression '-' Expression */
yyDecrement (3) yySetNT (yyNTExpression) {
/* line 1369 "xlang_pars.c" */
} break;
case 108: /* Expression : Expression '*' Expression */
yyDecrement (3) yySetNT (yyNTExpression) {
/* line 1373 "xlang_pars.c" */
} break;
case 109: /* Expression : Expression '/' Expression */
yyDecrement (3) yySetNT (yyNTExpression) {
/* line 1377 "xlang_pars.c" */
} break;
case 110: /* Expression : Expression '%' Expression */
yyDecrement (3) yySetNT (yyNTExpression) {
/* line 1381 "xlang_pars.c" */
} break;
case 111: /* Expression : Expression '^' Expression */
yyDecrement (3) yySetNT (yyNTExpression) {
/* line 1385 "xlang_pars.c" */
} break;
case 112:
case 80: /* Expression : '(' Expression ')' */
yyDecrement (3) yySetNT (yyNTExpression) {
/* line 1390 "xlang_pars.c" */
} break;
case 113: /* Expression : '+' Expression */
yyDecrement (2) yySetNT (yyNTExpression) {
/* line 1394 "xlang_pars.c" */
} break;
case 114: /* Expression : '-' Expression */
yyDecrement (2) yySetNT (yyNTExpression) {
/* line 1398 "xlang_pars.c" */
} break;
case 115:
case 81: /* CodeBlock : '{' CodeTeil '}' */
yyDecrement (3) yySetNT (yyNTCodeBlock) {
/* line 1403 "xlang_pars.c" */
} break;
case 116: /* Condition : 'if' Comparison CodeBlock */
yyDecrement (3) yySetNT (yyNTCondition) {
/* line 1407 "xlang_pars.c" */
} break;
case 117: /* Condition : 'if' Comparison CodeBlock else CodeBlock */
yyDecrement (5) yySetNT (yyNTCondition) {
/* line 1411 "xlang_pars.c" */
} break;
case 118: /* Condition : 'if' Comparison CodeBlock ElseIfList \
CodeBlock */
yyDecrement (5) yySetNT (yyNTCondition) {
/* line 1416 "xlang_pars.c" */
} break;
case 119: /* Condition : 'if' Comparison CodeBlock ElseIfList \
CodeBlock else CodeBlock */
yyDecrement (7) yySetNT (yyNTCondition) {
/* line 1421 "xlang_pars.c" */
} break;
case 120: /* ElseIfList : 'else if' CodeBlock */
yyDecrement (2) yySetNT (yyNTElseIfList) {
/* line 1425 "xlang_pars.c" */
} break;
case 121: /* ElseIfList : ElseIfList 'else if' CodeBlock */
yyDecrement (3) yySetNT (yyNTElseIfList) {
/* line 1429 "xlang_pars.c" */
} break;
case 122: /* Comparison : Expression '<' Expression */
yyDecrement (3) yySetNT (yyNTComparison) {
/* line 1433 "xlang_pars.c" */
} break;
case 123: /* Comparison : Expression '>' Expression */
yyDecrement (3) yySetNT (yyNTComparison) {
/* line 1437 "xlang_pars.c" */
} break;
case 124: /* Comparison : Expression '<=' Expression */
yyDecrement (3) yySetNT (yyNTComparison) {
/* line 1441 "xlang_pars.c" */
} break;
case 125: /* Comparison : Expression '>=' Expression */
yyDecrement (3) yySetNT (yyNTComparison) {
/* line 1445 "xlang_pars.c" */
} break;
case 126: /* Comparison : Expression '==' Expression */
yyDecrement (3) yySetNT (yyNTComparison) {
/* line 1449 "xlang_pars.c" */
} break;
case 127: /* Comparison : Expression '!=' Expression */
yyDecrement (3) yySetNT (yyNTComparison) {
/* line 1453 "xlang_pars.c" */
} break;
case 128: /* Loop : WhileLoop */
yyDecrement (1) yySetNT (yyNTLoop) {
/* line 1457 "xlang_pars.c" */
} break;
case 129: /* Loop : ForLoop */
yyDecrement (1) yySetNT (yyNTLoop) {
/* line 1461 "xlang_pars.c" */
} break;
case 130: /* WhileLoop : 'while' Comparison CodeBlock */
yyDecrement (3) yySetNT (yyNTWhileLoop) {
/* line 1465 "xlang_pars.c" */
} break;
case 131: /* ForLoop : 'for' Assignment ';' Comparison ';' Assignment \
CodeBlock */
yyDecrement (7) yySetNT (yyNTForLoop) {
/* line 1470 "xlang_pars.c" */
} break;
default: switch (yyState) {
case 1: goto yyAbort;
case 2: goto yyRead;
case 3: goto yyReduce;
}
}

	/* SPEC State = Next (Top (), Nonterminal); nonterminal transition */
# ifdef YYNDefault
	       yyState = * yyStateStackPtr ++;
	       for (;;) {
		  register yytComb * yyNCombPtr =
				yyNBasePtr [yyState] + (int) yyNonterminal;
		  if (yyNCombPtr->Check == yyState) {
		     yyState = yyNCombPtr->Next; break;
		  }
		  yyState = yyNDefault [yyState];
	       }
# else
	       yyState = yyNBasePtr [* yyStateStackPtr ++] [yyNonterminal];
# endif
	       * yyAttrStackPtr ++ = yySynAttribute;
	       if (yyState < yyFirstFinalState) break;	/* read nonterminal ? */
# ifdef YYDEBUG
	       yyStateStackPtr [0] = yyStateStackPtr [-1];
# endif
	    }

	 } else {					/* read */
   yyRead:  yyStateStackPtr ++;
	    yyGetAttribute (yyAttrStackPtr ++, xlang_scan_Attribute);
	    yyTerminal = yyGetToken ();
# ifdef YYDEBUG
	    if (xlang_pars_Debug) {
	       yyPrintState (yyStateStackPtr [-1]);
	       fprintf (yyTrace, "shift   %s, lookahead: %s",
		  yyGetTokenName (yyPrevTerminal),
		  yyGetTokenName (yyTerminal)); yyNl ();
	       yyPrevTerminal = yyTerminal;
	    }
# endif
	    yyIsRepairing = rfalse;
	 }
      }

   yyAbort:
# ifdef YYDEBUG
      if (xlang_pars_Debug) {
	 yyPrintState (* yyStateStackPtr);
	 fprintf (yyTrace, "fail    parse started at %ld", yyStartCount);
	 yyNl ();
      }
# endif
      return ++ yyErrorCount;

   yyAccept:
# ifdef YYDEBUG
      if (xlang_pars_Debug) {
	 yyPrintState (* yyStateStackPtr);
	 fprintf (yyTrace, "accept  parse started at %ld", yyStartCount);
	 yyNl ();
      }
# endif
      return yyErrorCount;
   }

# ifndef NO_RECOVER
static int yyErrorRecovery
# ifdef HAVE_ARGS
   (yySymbolRange yyTerminal,
#  ifdef YYDEBUG
    yySymbolRange * yyPrevTerminal,
#  endif
    long yyStackPtr)
# else
   (yyTerminal,
#  ifdef YYDEBUG
    yyPrevTerminal,
#  endif
    yyStackPtr)
   yySymbolRange	yyTerminal	;
#  ifdef YYDEBUG
   yySymbolRange *	yyPrevTerminal	;
#  endif
   long			yyStackPtr	;
# endif
   {
#     define	yyContinueSize	5000
      rbool	yyTokensSkipped	;
      tSet	yyContinueSet	;
      tSet	yyRestartSet	;
      int	yyLength	;
      char	yyContinueString [yyContinueSize + 2];

      if (yyControl.yyMessages) {
   /* 1. report an error */
	 ErrorMessage (xxSyntaxError, xxError, xlang_scan_Attribute.Position);

   /* 2. report the offending token */
	 strcpy (yyContinueString, yyGetTokenName (yyTerminal));
# ifdef SPELLING
	 if (strncmp (yyContinueString, xlang_scan_TokenPtr, xlang_scan_TokenLength)) {
	    yyContinueString [yyLength = strlen (yyContinueString)] = ' ';
	    xlang_scan_GetWord (& yyContinueString [++ yyLength]);
	 }
# endif
	 ErrorMessageI (xxTokenFound, xxInformation, xlang_scan_Attribute.Position,
	    xxString, yyContinueString);

   /* 3. report the set of expected terminal symbols */
	 MakeSet (& yyContinueSet, yyLastTerminal);
	 yyComputeContinuation (yyStackPtr, & yyContinueSet);
	 yyLength = 0;
	 yyContinueString [0] = '\0';
	 while (! IsEmpty (& yyContinueSet)) {
	    char * yyTokenString =
	       yyGetTokenName ((yySymbolRange) Extract (& yyContinueSet));
	    int yyl = strlen (yyTokenString);
	    if (yyLength + yyl >= yyContinueSize) break;
	    strcpy (& yyContinueString [yyLength], yyTokenString);
	    yyLength += yyl;
	    yyContinueString [yyLength ++] = ' ';
	 }
	 yyContinueString [-- yyLength] = '\0';
	 ErrorMessageI (xxExpectedTokens, xxInformation, xlang_scan_Attribute.Position,
	    xxString, yyContinueString);
	 ReleaseSet (& yyContinueSet);
      }

   /* 4. compute the set of terminal symbols for restart of the parse */
      MakeSet (& yyRestartSet, yyLastTerminal);
      yyComputeRestartPoints (yyStackPtr, & yyRestartSet);

   /* 5. skip terminal symbols until a restart point is reached */
      yyTokensSkipped = rfalse;
      while (! IsElement (yyTerminal, & yyRestartSet)) {
	 yyTerminal = yyGetToken ();
	 yyTokensSkipped = rtrue;
# ifdef YYDEBUG
	 if (xlang_pars_Debug) {
	    yyPrintState (yyStateStack [yyStackPtr]);
	    fprintf (yyTrace, "skip    %s, lookahead: %s",
	       yyGetTokenName (* yyPrevTerminal),
	       yyGetTokenName (yyTerminal)); yyNl ();
	    * yyPrevTerminal = yyTerminal;
	 }
# endif
      }
      ReleaseSet (& yyRestartSet);

   /* 6. report the restart point */
      if (yyTokensSkipped & yyControl.yyMessages)
	 ErrorMessage (xxRestartPoint, xxInformation, xlang_scan_Attribute.Position);

      return yyTerminal;
   }

/*
   compute the set of terminal symbols that can be accepted (read)
   in a given stack configuration (eventually after reduce actions)
*/

static void yyComputeContinuation
# ifdef HAVE_ARGS
   (long yyStackPtr, tSet * yyContinueSet)
# else
   (yyStackPtr, yyContinueSet)
   long			yyStackPtr	;
   tSet *		yyContinueSet	;
# endif
   {
      register yySymbolRange	yyTerminal;
      register yyStateRange	yyState = yyStateStack [yyStackPtr];

      AssignEmpty (yyContinueSet);
      for (yyTerminal = yyFirstTerminal; yyTerminal <= yyLastTerminal;
							yyTerminal ++) {
	 yyStateRange yyNextState = yyNext (yyState, yyTerminal);
	 if (yyNextState != yyNoState && (yyNextState <= yyLastStopState ||
	    yyIsContinuation (yyTerminal, yyStackPtr))) {
	    Include (yyContinueSet, yyTerminal);
	 }
      }
   }

/*
   check whether a given terminal symbol can be accepted (read)
   in a certain stack configuration (eventually after reduce actions)
*/

static rbool yyIsContinuation
# ifdef HAVE_ARGS
   (yySymbolRange yyTerminal, long yyStackPtr)
# else
   (yyTerminal, yyStackPtr)
   yySymbolRange	yyTerminal	;
   long			yyStackPtr	;
# endif
   {
      register yyStateRange	yState		;
      register yytNonterminal	yyNonterminal	;

      while (yyStackPtr >= yyIsContStackSize)	       /* pass Stack by value */
	 yyExtendArray ((char * *) & yyIsContStackPtr, & yyIsContStackSize,
			  (unsigned long) sizeof (yyStateRange));
      memcpy ((char *) yyIsContStackPtr, (char *) yyStateStack,
		(int) sizeof (yyStateRange) * (yyStackPtr + 1));

      yState = yyIsContStackPtr [yyStackPtr];
      for (;;) {
	 yyIsContStackPtr [yyStackPtr] = yState;
	 yState = yyNext (yState, yyTerminal);
	 if (yState == yyNoState) return rfalse;

	 do {						/* reduce */
	    if (yState > yyLastReduceState) {		/* dynamic ? */
	       yState = yyCondition [yState - yyLastReduceState];
	    }
	    if (yState <= yyLastStopState) { /* read, read reduce, or accept? */
	       return rtrue;
	    } else {					/* reduce */
	       yyStackPtr -= yyLength [yState - yyFirstReduceState];
	       yyNonterminal = yyLeftHandSide [yState - yyFirstReduceState];
	    }

	    yState = yyNext (yyIsContStackPtr [yyStackPtr],
				(yySymbolRange) yyNonterminal);
	    yyStackPtr ++;
	    if (yyStackPtr >= yyIsContStackSize) {
	       yyExtendArray ((char * *) & yyIsContStackPtr,
		  & yyIsContStackSize, (unsigned long) sizeof (yyStateRange));
	       if (yyStackPtr >= 1000) return rfalse; /* EMERGENCY break !!! */
	    }
	 } while (yState >= yyFirstFinalState);
      }
   }

/*
   compute the set of terminal symbols that can be accepted (read)
   directly in a given stack configuration
*/

static void yyCompContinuation
# ifdef HAVE_ARGS
   (yyStateRange yyState, tSet * yyContinueSet)
# else
   (yyState, yyContinueSet)
   yyStateRange		yyState		;
   tSet *		yyContinueSet	;
# endif
   {
      register yySymbolRange	yyTerminal;

      AssignEmpty (yyContinueSet);
      for (yyTerminal = yyFirstTerminal; yyTerminal <= yyLastTerminal;
							yyTerminal ++) {
	 yyStateRange yyNextState = yyNext (yyState, yyTerminal);
	 if (yyNextState != yyNoState && yyNextState <= yyLastStopState) {
	    Include (yyContinueSet, yyTerminal);
	 }
      }
   }

/*
   compute a set of terminal symbols that can be used to restart
   parsing in a given stack configuration. We simulate parsing until
   end of file using a suffix program synthesized by the function (array)
   yyContinuation. All symbols acceptable in the states reached during
   the simulation can be used to restart parsing.
*/

static void yyComputeRestartPoints
# ifdef HAVE_ARGS
   (long yyStackPtr, tSet * yyRestartSet)
# else
   (yyStackPtr, yyRestartSet)
   long			yyStackPtr	;
   tSet *		yyRestartSet	;
# endif
   {
      register yyStateRange	yState		;
      register yytNonterminal	yyNonterminal	;
	       tSet		yyContinueSet	;

      while (yyStackPtr >= yyCompResStackSize)	       /* pass Stack by value */
	 yyExtendArray ((char * *) & yyCompResStackPtr, & yyCompResStackSize,
			  (unsigned long) sizeof (yyStateRange));
      memcpy ((char *) yyCompResStackPtr, (char *) yyStateStack,
		(int) sizeof (yyStateRange) * (yyStackPtr + 1));

      MakeSet (& yyContinueSet, yyLastTerminal);
      AssignEmpty (yyRestartSet);
      yState = yyCompResStackPtr [yyStackPtr];

      for (;;) {
	 if (yyStackPtr >= yyCompResStackSize)
	    yyExtendArray ((char * *) & yyCompResStackPtr, & yyCompResStackSize,
			     (unsigned long) sizeof (yyStateRange));
	 yyCompResStackPtr [yyStackPtr] = yState;
	 yyCompContinuation (yState, & yyContinueSet);
	 Union (yyRestartSet, & yyContinueSet);
# ifdef YYDCRP
	 {
	    int yyLength = 0;
	    char yyContinueString [yyContinueSize + 2];
	    yyContinueString [0] = '\0';
	    while (! IsEmpty (& yyContinueSet)) {
	       char * yyTokenString =
		  yyGetTokenName ((yySymbolRange) Extract (& yyContinueSet));
	       int yyl = strlen (yyTokenString);
	       if (yyLength + yyl >= yyContinueSize) break;
	       strcpy (& yyContinueString [yyLength], yyTokenString);
	       yyLength += yyl;
	       yyContinueString [yyLength ++] = ' ';
	    }
	    yyContinueString [-- yyLength] = '\0';
	    fprintf (yyTrace, "%5d union %s", yState, yyContinueString);
	    yyNl ();
	 }
# endif
	 yState = yyNext (yState, yyContinuation [yState]);

	 if (yState >= yyFirstFinalState) {		/* final state ? */
	    if (yState <= yyLastReadReduceState) {	/* read reduce ? */
	       yyStackPtr ++;
	       yState = yyFinalToProd [yState - yyFirstReadReduceState];
# ifdef YYDCRP
	       yyCompResStackPtr [yyStackPtr] =
					yyCompResStackPtr [yyStackPtr - 1];
	       fprintf (yyTrace, "%5d shift   %s",
		  yyCompResStackPtr [yyStackPtr], yyGetTokenName
		  (yyContinuation [yyCompResStackPtr [yyStackPtr]])); yyNl ();
# endif
	    }

	    do {					/* reduce */
	       if (yState > yyLastReduceState) {	/* dynamic ? */
# ifdef YYDCRP
		  fprintf (yyTrace, "%5d dynamic decision %d",
		    yyCompResStackPtr [yyStackPtr], yState - yyLastReduceState);
		  yyNl ();
# endif
		  yState = yyCondition [yState - yyLastReduceState];
	       }
	       if (yyFirstReduceState <= yState &&
		   yState <= yyLastStopState) {		/* accept */
# ifdef YYDCRP
		  fprintf (yyTrace, "%5d accept",
		     yyCompResStackPtr [yyStackPtr]); yyNl ();
# endif
		  ReleaseSet (& yyContinueSet);
		  return;
	       } else if (yState < yyFirstFinalState) {	/* read */
		  goto yyRead;
	       } else {					/* reduce */
# ifdef YYDCRP
		  fprintf (yyTrace, "%5d reduce  %s",
		     yyCompResStackPtr [yyStackPtr],
		     yyRule [yState - yyLastReadReduceState]); yyNl ();
# endif
		  yyStackPtr -= yyLength [yState - yyFirstReduceState];
		  yyNonterminal = yyLeftHandSide [yState - yyFirstReduceState];
	       }

	       yState = yyNext (yyCompResStackPtr [yyStackPtr],
				(yySymbolRange) yyNonterminal);
	       yyStackPtr ++;
	    } while (yState >= yyFirstFinalState);
	 } else {					/* read */
yyRead:
# ifdef YYDCRP
	    fprintf (yyTrace, "%5d shift   %s",
	       yyCompResStackPtr [yyStackPtr], yyGetTokenName
	       (yyContinuation [yyCompResStackPtr [yyStackPtr]])); yyNl ();
# endif
	    yyStackPtr ++;
	 }
      }
   }

/* access the parse table:   Next : State x Symbol -> Action */

static yyStateRange yyNext
# ifdef HAVE_ARGS
   (yyStateRange yyState, yySymbolRange yySymbol)
# else
   (yyState, yySymbol) yyStateRange yyState; yySymbolRange yySymbol;
# endif
   {
      if (yySymbol <= yyLastTerminal) {
	 for (;;) {
	    register yytComb * yyTCombPtr = yyTBasePtr [yyState] + yySymbol;
# if defined YYTDefault & defined YYaccDefault
	    register unsigned long * yylp;
# endif
	    if (yyTCombPtr->Check == yyState) return yyTCombPtr->Next;
# ifdef YYTDefault
#  ifdef YYaccDefault
	    return (yylp = yyDefaultLook [yyState]) &&
	       (yylp [yySymbol >> 5] >> (yySymbol & 0x1f)) & 1 ?
		  yyTDefault [yyState] : yyNoState;
#  else
	    if ((yyState = yyTDefault [yyState]) == yyNoState) return yyNoState;
#  endif
# else
	    return yyNoState;
# endif
	 }
      }
# ifdef YYNDefault
      for (;;) {
	 register yytComb * yyNCombPtr = yyNBasePtr [yyState] + yySymbol;
	 if (yyNCombPtr->Check == yyState) return yyNCombPtr->Next;
	 yyState = yyNDefault [yyState];
      }
# else
      return yyNBasePtr [yyState] [yySymbol];
# endif
   }
# endif

void Beginxlang_pars ARGS ((void))
   {
/* line 38 "xlang_pars.lrk" */


/* line 1898 "xlang_pars.c" */
   }

void Closexlang_pars ARGS ((void))
   {
/* line 41 "xlang_pars.lrk" */


/* line 1906 "xlang_pars.c" */
   }

