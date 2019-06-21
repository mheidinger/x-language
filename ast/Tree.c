# include "Tree.h"

# ifndef EXTERN_C_BEGIN
# define EXTERN_C_BEGIN
# define EXTERN_C_END
# endif

EXTERN_C_BEGIN
# include <stdlib.h>
# include <stddef.h>
# include "rString.h"
# include "rSystem.h"
# include "General.h"
# include "rMemory.h"
# include "DynArray.h"
# include "StringM.h"
# include "Idents.h"
# include "Position.h"
# include "Errors.h"
EXTERN_C_END

# ifdef _MSC_VER
#  pragma warning (disable: 4100 4505 4514)
# endif

char Tree_module_does_not_match_evaluator_module_1744597;
char generate_Tree_module_without_option_0;

static FILE * yyf;

/* line 41 "xlang.ast" */

  /* The target-code of this section is copied into the generated .c file */


# ifdef __cplusplus
#  ifndef yyALLOC
#   define yyALLOC(size1, size2) yyALLOCi (size1, size2)
#  endif
static inline tTree yyALLOCi (unsigned long yysize1, unsigned long yysize2)
{ return Tree_PoolFreePtr >= Tree_PoolStartPtr + yysize1 ?
  (tTree) (Tree_PoolFreePtr -= yysize1) : Tree_Alloc (yysize2); }
static inline tTree yyALLOCk (unsigned long yysize)
{ return yyALLOC (yysize, yysize); }
static inline tTree yyALLOCt (unsigned long yysize)
{ return yyALLOC (yyAlignedSize (yysize), yysize); }
# else
#  define yyALLOCk(size) yyALLOC (size, size)
#  define yyALLOCt(size) yyALLOC (yyAlignedSize (size), size)
#  ifndef yyALLOC
#   define yyALLOC(size1, size2) (Tree_PoolFreePtr -= (long) size1) >= \
     Tree_PoolStartPtr ? (tTree) Tree_PoolFreePtr : Tree_Alloc (size2)
#  endif
# endif
# ifndef yyFREE
# define yyFREE(ptr, size) 
# endif
# ifdef getchar
# undef getchar
# endif
# ifdef putchar
# undef putchar
# endif
# include "yyTree.h"

static void yyExit ARGS ((void)) { rExit (1); }

void (* Tree_Exit) ARGS ((void)) = yyExit;

tTree TreeRoot;
unsigned long Tree_HeapUsed = 0;

yytTree_BlockPtr Tree_BlockList	= (yytTree_BlockPtr) NoTree;
char * Tree_PoolFreePtr = (char *) & Tree_BlockList;
char * Tree_PoolStartPtr = (char *) & Tree_BlockList;
rbool Tree_CheckReportNoTree = rtrue, Tree_CheckReportNodes = rtrue;
int Tree_DrawBoxHeight = 20;
int Tree_DrawBoxWidth  = 60;
int Tree_DrawDepth     = 6;
int Tree_DrawLength    = 256;
char Tree_DrawFileName [256] = "";
# ifdef SUPPORT_TVA
rbool Tree_TreatTVAasChild = rfalse;
# endif

const unsigned short Tree_NodeSize [26] = { 0,
 yyAlignedSize (sizeof (yprogram)),
 yyAlignedSize (sizeof (yDECLS)),
 yyAlignedSize (sizeof (yno_decls)),
 yyAlignedSize (sizeof (ydecls)),
 yyAlignedSize (sizeof (ydecl)),
 yyAlignedSize (sizeof (yidentifier)),
 yyAlignedSize (sizeof (yINSTRS)),
 yyAlignedSize (sizeof (yno_instrs)),
 yyAlignedSize (sizeof (yinstrs)),
 yyAlignedSize (sizeof (yINSTR)),
 yyAlignedSize (sizeof (yif_instr)),
 yyAlignedSize (sizeof (yassignment_instr)),
 yyAlignedSize (sizeof (ywhile_instr)),
 yyAlignedSize (sizeof (yfor_instr)),
 yyAlignedSize (sizeof (ytest)),
 yyAlignedSize (sizeof (yELSEIFS)),
 yyAlignedSize (sizeof (yno_elseifs)),
 yyAlignedSize (sizeof (yelseifs)),
 yyAlignedSize (sizeof (yEXPR)),
 yyAlignedSize (sizeof (yint_const)),
 yyAlignedSize (sizeof (yfloat_const)),
 yyAlignedSize (sizeof (ystring_const)),
 yyAlignedSize (sizeof (yname)),
 yyAlignedSize (sizeof (ybinary_expr)),
 yyAlignedSize (sizeof (yunary_expr)),
};
const Tree_tKind Tree_TypeRange [26] = { 0,
 kprogram,
 kdecls,
 kno_decls,
 kdecls,
 kdecl,
 kidentifier,
 kinstrs,
 kno_instrs,
 kinstrs,
 kfor_instr,
 kif_instr,
 kassignment_instr,
 kwhile_instr,
 kfor_instr,
 ktest,
 kelseifs,
 kno_elseifs,
 kelseifs,
 kunary_expr,
 kint_const,
 kfloat_const,
 kstring_const,
 kname,
 kbinary_expr,
 kunary_expr,
};
const char * const Tree_NodeName [26] = {
 "NoTree",
 "program",
 "DECLS",
 "no_decls",
 "decls",
 "decl",
 "identifier",
 "INSTRS",
 "no_instrs",
 "instrs",
 "INSTR",
 "if_instr",
 "assignment_instr",
 "while_instr",
 "for_instr",
 "test",
 "ELSEIFS",
 "no_elseifs",
 "elseifs",
 "EXPR",
 "int_const",
 "float_const",
 "string_const",
 "name",
 "binary_expr",
 "unary_expr",
};

tTree Tree_Alloc
# ifdef HAVE_ARGS
 (unsigned long yySize)
# else
 (yySize) unsigned long yySize;
# endif
{
 register yytTree_BlockPtr yyBlockPtr = Tree_BlockList;
 Tree_BlockList = (yytTree_BlockPtr) Alloc ((unsigned long) sizeof (yytTree_Block));
 if ((tTree) Tree_BlockList == NoTree) {
  ErrorMessageI (xxTreeOutOfMemory, xxFatal, NoPosition, xxString, "Tree");
  Tree_Exit ();
 }
 Tree_BlockList->yySuccessor = yyBlockPtr;
 Tree_PoolStartPtr = (char *) Tree_BlockList;
 Tree_HeapUsed += sizeof (yytTree_Block);
 return (tTree) (Tree_PoolFreePtr = Tree_PoolStartPtr +
  (yyBlockSize - yyAlignedSize (yySize)));
}

tTree MakeTree
# ifdef HAVE_ARGS
 (Tree_tKind yyKind)
# else
 (yyKind) Tree_tKind yyKind;
# endif
{
 register tTree yyt = yyALLOCk (Tree_NodeSize [yyKind]);
 Tree_InitHead (yyt, yyKind)
 return yyt;
}

rbool Tree_IsType
# ifdef HAVE_ARGS
 (register tTree yyt, register Tree_tKind yyKind)
# else
 (yyt, yyKind) register tTree yyt; register Tree_tKind yyKind;
# endif
{
 return yyt != NoTree && yyKind <= yyt->Kind &&
  yyt->Kind <= Tree_TypeRange [yyKind];
}


tTree mprogram
# ifdef HAVE_ARGS
(tStringRef pName, tTree pDeclarations, tTree pInstructions)
# else
(pName, pDeclarations, pInstructions)
tStringRef pName;
tTree pDeclarations;
tTree pInstructions;
# endif
{
 register tTree yyt = yyALLOCt (sizeof (yprogram));
 Tree_InitHead (yyt, kprogram)
 yyt->program.Name = pName;
 yyt->program.Declarations = pDeclarations;
 yyt->program.Instructions = pInstructions;
 return yyt;
}

tTree mDECLS
# ifdef HAVE_ARGS
(void)
# else
()
# endif
{
 register tTree yyt = yyALLOCt (sizeof (yDECLS));
 Tree_InitHead (yyt, kDECLS)
 return yyt;
}

tTree mno_decls
# ifdef HAVE_ARGS
(void)
# else
()
# endif
{
 register tTree yyt = yyALLOCt (sizeof (yno_decls));
 Tree_InitHead (yyt, kno_decls)
 return yyt;
}

tTree mdecls
# ifdef HAVE_ARGS
(tTree pNext, tTree pDecl)
# else
(pNext, pDecl)
tTree pNext;
tTree pDecl;
# endif
{
 register tTree yyt = yyALLOCt (sizeof (ydecls));
 Tree_InitHead (yyt, kdecls)
 yyt->decls.Next = pNext;
 yyt->decls.Decl = pDecl;
 return yyt;
}

tTree mdecl
# ifdef HAVE_ARGS
(tIOFlag pIOFlag, tVarType pType, tTree pIdentifier)
# else
(pIOFlag, pType, pIdentifier)
tIOFlag pIOFlag;
tVarType pType;
tTree pIdentifier;
# endif
{
 register tTree yyt = yyALLOCt (sizeof (ydecl));
 Tree_InitHead (yyt, kdecl)
 yyt->decl.IOFlag = pIOFlag;
 yyt->decl.Type = pType;
 yyt->decl.Identifier = pIdentifier;
 return yyt;
}

tTree midentifier
# ifdef HAVE_ARGS
(tIdent pIdent)
# else
(pIdent)
tIdent pIdent;
# endif
{
 register tTree yyt = yyALLOCt (sizeof (yidentifier));
 Tree_InitHead (yyt, kidentifier)
 yyt->identifier.Ident = pIdent;
 return yyt;
}

tTree mINSTRS
# ifdef HAVE_ARGS
(void)
# else
()
# endif
{
 register tTree yyt = yyALLOCt (sizeof (yINSTRS));
 Tree_InitHead (yyt, kINSTRS)
 return yyt;
}

tTree mno_instrs
# ifdef HAVE_ARGS
(void)
# else
()
# endif
{
 register tTree yyt = yyALLOCt (sizeof (yno_instrs));
 Tree_InitHead (yyt, kno_instrs)
 return yyt;
}

tTree minstrs
# ifdef HAVE_ARGS
(tTree pNext, tTree pInstr)
# else
(pNext, pInstr)
tTree pNext;
tTree pInstr;
# endif
{
 register tTree yyt = yyALLOCt (sizeof (yinstrs));
 Tree_InitHead (yyt, kinstrs)
 yyt->instrs.Next = pNext;
 yyt->instrs.Instr = pInstr;
 return yyt;
}

tTree mINSTR
# ifdef HAVE_ARGS
(void)
# else
()
# endif
{
 register tTree yyt = yyALLOCt (sizeof (yINSTR));
 Tree_InitHead (yyt, kINSTR)
 return yyt;
}

tTree mif_instr
# ifdef HAVE_ARGS
(tTree pTest, tTree pThen, tTree pElse, tTree pELSEIFS)
# else
(pTest, pThen, pElse, pELSEIFS)
tTree pTest;
tTree pThen;
tTree pElse;
tTree pELSEIFS;
# endif
{
 register tTree yyt = yyALLOCt (sizeof (yif_instr));
 Tree_InitHead (yyt, kif_instr)
 yyt->if_instr.Test = pTest;
 yyt->if_instr.Then = pThen;
 yyt->if_instr.Else = pElse;
 yyt->if_instr.ELSEIFS = pELSEIFS;
 return yyt;
}

tTree massignment_instr
# ifdef HAVE_ARGS
(tTree pLhs, tTree pRhs)
# else
(pLhs, pRhs)
tTree pLhs;
tTree pRhs;
# endif
{
 register tTree yyt = yyALLOCt (sizeof (yassignment_instr));
 Tree_InitHead (yyt, kassignment_instr)
 yyt->assignment_instr.Lhs = pLhs;
 yyt->assignment_instr.Rhs = pRhs;
 return yyt;
}

tTree mwhile_instr
# ifdef HAVE_ARGS
(tTree pTest, tTree pBody)
# else
(pTest, pBody)
tTree pTest;
tTree pBody;
# endif
{
 register tTree yyt = yyALLOCt (sizeof (ywhile_instr));
 Tree_InitHead (yyt, kwhile_instr)
 yyt->while_instr.Test = pTest;
 yyt->while_instr.Body = pBody;
 return yyt;
}

tTree mfor_instr
# ifdef HAVE_ARGS
(tTree pAssignment, tTree pTest, tTree pIncAssignment, tTree pBody)
# else
(pAssignment, pTest, pIncAssignment, pBody)
tTree pAssignment;
tTree pTest;
tTree pIncAssignment;
tTree pBody;
# endif
{
 register tTree yyt = yyALLOCt (sizeof (yfor_instr));
 Tree_InitHead (yyt, kfor_instr)
 yyt->for_instr.Assignment = pAssignment;
 yyt->for_instr.Test = pTest;
 yyt->for_instr.IncAssignment = pIncAssignment;
 yyt->for_instr.Body = pBody;
 return yyt;
}

tTree mtest
# ifdef HAVE_ARGS
(tStringRef poperator, tTree pLeft, tTree pRight)
# else
(poperator, pLeft, pRight)
tStringRef poperator;
tTree pLeft;
tTree pRight;
# endif
{
 register tTree yyt = yyALLOCt (sizeof (ytest));
 Tree_InitHead (yyt, ktest)
 yyt->test.operator = poperator;
 yyt->test.Left = pLeft;
 yyt->test.Right = pRight;
 return yyt;
}

tTree mELSEIFS
# ifdef HAVE_ARGS
(void)
# else
()
# endif
{
 register tTree yyt = yyALLOCt (sizeof (yELSEIFS));
 Tree_InitHead (yyt, kELSEIFS)
 return yyt;
}

tTree mno_elseifs
# ifdef HAVE_ARGS
(void)
# else
()
# endif
{
 register tTree yyt = yyALLOCt (sizeof (yno_elseifs));
 Tree_InitHead (yyt, kno_elseifs)
 return yyt;
}

tTree melseifs
# ifdef HAVE_ARGS
(tTree pNext, tTree pTest, tTree pThen)
# else
(pNext, pTest, pThen)
tTree pNext;
tTree pTest;
tTree pThen;
# endif
{
 register tTree yyt = yyALLOCt (sizeof (yelseifs));
 Tree_InitHead (yyt, kelseifs)
 yyt->elseifs.Next = pNext;
 yyt->elseifs.Test = pTest;
 yyt->elseifs.Then = pThen;
 return yyt;
}

tTree mEXPR
# ifdef HAVE_ARGS
(void)
# else
()
# endif
{
 register tTree yyt = yyALLOCt (sizeof (yEXPR));
 Tree_InitHead (yyt, kEXPR)
 return yyt;
}

tTree mint_const
# ifdef HAVE_ARGS
(int pValue)
# else
(pValue)
int pValue;
# endif
{
 register tTree yyt = yyALLOCt (sizeof (yint_const));
 Tree_InitHead (yyt, kint_const)
 yyt->int_const.Value = pValue;
 return yyt;
}

tTree mfloat_const
# ifdef HAVE_ARGS
(float pValue)
# else
(pValue)
float pValue;
# endif
{
 register tTree yyt = yyALLOCt (sizeof (yfloat_const));
 Tree_InitHead (yyt, kfloat_const)
 yyt->float_const.Value = pValue;
 return yyt;
}

tTree mstring_const
# ifdef HAVE_ARGS
(tStringRef pValue)
# else
(pValue)
tStringRef pValue;
# endif
{
 register tTree yyt = yyALLOCt (sizeof (ystring_const));
 Tree_InitHead (yyt, kstring_const)
 yyt->string_const.Value = pValue;
 return yyt;
}

tTree mname
# ifdef HAVE_ARGS
(tTree pIdent)
# else
(pIdent)
tTree pIdent;
# endif
{
 register tTree yyt = yyALLOCt (sizeof (yname));
 Tree_InitHead (yyt, kname)
 yyt->name.Ident = pIdent;
 return yyt;
}

tTree mbinary_expr
# ifdef HAVE_ARGS
(char pOp, tTree pLeft, tTree pRight)
# else
(pOp, pLeft, pRight)
char pOp;
tTree pLeft;
tTree pRight;
# endif
{
 register tTree yyt = yyALLOCt (sizeof (ybinary_expr));
 Tree_InitHead (yyt, kbinary_expr)
 yyt->binary_expr.Op = pOp;
 yyt->binary_expr.Left = pLeft;
 yyt->binary_expr.Right = pRight;
 return yyt;
}

tTree munary_expr
# ifdef HAVE_ARGS
(char pOp, tTree pExpr)
# else
(pOp, pExpr)
char pOp;
tTree pExpr;
# endif
{
 register tTree yyt = yyALLOCt (sizeof (yunary_expr));
 Tree_InitHead (yyt, kunary_expr)
 yyt->unary_expr.Op = pOp;
 yyt->unary_expr.Expr = pExpr;
 return yyt;
}

static void yyMark
# ifdef HAVE_ARGS
 (register tTree yyt)
# else
 (yyt) register tTree yyt;
# endif
{
 for (;;) {
  if (yyt == NoTree || ++ yyt->yyHead.yyMark > 1) return;

  switch (yyt->Kind) {
case kprogram:
yyMark (yyt->program.Declarations);
yyt = yyt->program.Instructions; break;
case kdecls:
yyMark (yyt->decls.Decl);
yyt = yyt->decls.Next; break;
case kdecl:
yyt = yyt->decl.Identifier; break;
case kinstrs:
yyMark (yyt->instrs.Instr);
yyt = yyt->instrs.Next; break;
case kif_instr:
yyMark (yyt->if_instr.Test);
yyMark (yyt->if_instr.Then);
yyMark (yyt->if_instr.Else);
yyt = yyt->if_instr.ELSEIFS; break;
case kassignment_instr:
yyMark (yyt->assignment_instr.Lhs);
yyt = yyt->assignment_instr.Rhs; break;
case kwhile_instr:
yyMark (yyt->while_instr.Test);
yyt = yyt->while_instr.Body; break;
case kfor_instr:
yyMark (yyt->for_instr.Assignment);
yyMark (yyt->for_instr.Test);
yyMark (yyt->for_instr.IncAssignment);
yyt = yyt->for_instr.Body; break;
case ktest:
yyMark (yyt->test.Left);
yyt = yyt->test.Right; break;
case kelseifs:
yyMark (yyt->elseifs.Test);
yyMark (yyt->elseifs.Then);
yyt = yyt->elseifs.Next; break;
case kname:
yyt = yyt->name.Ident; break;
case kbinary_expr:
yyMark (yyt->binary_expr.Left);
yyt = yyt->binary_expr.Right; break;
case kunary_expr:
yyt = yyt->unary_expr.Expr; break;
  default: return;
  }
 }
}

# define yyInitTreeStoreSize 64

# define yyTreeStoreHashSize 256

typedef struct { tTree yyPtr; Tree_tLabel yyNext; } yytTreeStore;
static unsigned long yyTreeStoreSize = yyInitTreeStoreSize;
static yytTreeStore * yyTreeStorePtr;
static Tree_tLabel yyLabelCount;
static short yyRecursionLevel = 0;
static Tree_tLabel yyTreeStoreHash [yyTreeStoreHashSize];

static void yyBeginTreeStore ARGS ((void))
{
 register int yyi;
 if (yyRecursionLevel ++ == 0) {
  MakeArray ((char * *) & yyTreeStorePtr, & yyTreeStoreSize,
   (unsigned long) sizeof (yytTreeStore)); yyLabelCount = 0;
  for (yyi = 0; yyi < yyTreeStoreHashSize; yyi ++)
   yyTreeStoreHash [yyi] = 0;
 }
}

static void yyCloseTreeStore ARGS ((void))
{
 if (-- yyRecursionLevel == 0) {
  ReleaseArray ((char * *) & yyTreeStorePtr, & yyTreeStoreSize,
   (unsigned long) sizeof (yytTreeStore));
 }
}

static Tree_tLabel yyMapToLabel
# ifdef HAVE_ARGS
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 long yyhash = (((ptrdiff_t) yyt) >> 4) & (yyTreeStoreHashSize - 1);
 register Tree_tLabel yyi = yyTreeStoreHash [yyhash];
 for (; yyi; yyi = yyTreeStorePtr [yyi].yyNext)
  if (yyTreeStorePtr [yyi].yyPtr == yyt) return yyi;
 if (++ yyLabelCount == (Tree_tLabel) yyTreeStoreSize)
  ExtendArray ((char * *) & yyTreeStorePtr, & yyTreeStoreSize,
   (unsigned long) sizeof (yytTreeStore));
 yyTreeStorePtr [yyLabelCount].yyPtr = yyt;
 yyTreeStorePtr [yyLabelCount].yyNext = yyTreeStoreHash [yyhash];
 yyTreeStoreHash [yyhash] = yyLabelCount;
 return yyLabelCount;
}

static void yyWriteTree ARGS ((tTree yyt));
static void yyWriteTreeXML ARGS ((tTree yyt));

static void xxWriteNl ARGS ((void)) { putc ('\n', yyf); }

static void yyWriteSelector
# ifdef HAVE_ARGS
 (char * yys)
# else
 (yys) char * yys;
# endif
{
 register int yyi = 16 - strlen (yys);
 fputs (yys, yyf);
 while (yyi -- > 0) putc (' ', yyf);
 fputs (" = ", yyf);
}

static void yyWriteHex
# ifdef HAVE_ARGS
 (unsigned char * yyx, int yysize)
# else
 (yyx, yysize) unsigned char * yyx; int yysize;
# endif
{ register int yyi; for (yyi = 0; yyi < yysize; yyi ++)
   fprintf (yyf, "%02x ", yyx [yyi]); }

static void yyWriteAddr
# ifdef HAVE_ARGS
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 if (yyt == NoTree) fputs ("NoTree", yyf);
 else fprintf (yyf, "%p *", (void *) yyt);
 xxWriteNl ();
}

static void yWriteNodeprogram
# ifdef HAVE_ARGS
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyWriteSelector ("Name");
 writetStringRef (yyt->program.Name) xxWriteNl ();
 yyWriteSelector ("Declarations");
 yyWriteAddr (yyt->program.Declarations);
 yyWriteSelector ("Instructions");
 yyWriteAddr (yyt->program.Instructions);
}

static void yWriteNodedecls
# ifdef HAVE_ARGS
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyWriteSelector ("Next");
 yyWriteAddr (yyt->decls.Next);
 yyWriteSelector ("Decl");
 yyWriteAddr (yyt->decls.Decl);
}

static void yWriteNodedecl
# ifdef HAVE_ARGS
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyWriteSelector ("IOFlag");
 writetIOFlag (yyt->decl.IOFlag) xxWriteNl ();
 yyWriteSelector ("Type");
 writetVarType (yyt->decl.Type) xxWriteNl ();
 yyWriteSelector ("Identifier");
 yyWriteAddr (yyt->decl.Identifier);
}

static void yWriteNodeidentifier
# ifdef HAVE_ARGS
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyWriteSelector ("Ident");
 writetIdent (yyt->identifier.Ident) xxWriteNl ();
}

static void yWriteNodeinstrs
# ifdef HAVE_ARGS
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyWriteSelector ("Next");
 yyWriteAddr (yyt->instrs.Next);
 yyWriteSelector ("Instr");
 yyWriteAddr (yyt->instrs.Instr);
}

static void yWriteNodeif_instr
# ifdef HAVE_ARGS
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyWriteSelector ("Test");
 yyWriteAddr (yyt->if_instr.Test);
 yyWriteSelector ("Then");
 yyWriteAddr (yyt->if_instr.Then);
 yyWriteSelector ("Else");
 yyWriteAddr (yyt->if_instr.Else);
 yyWriteSelector ("ELSEIFS");
 yyWriteAddr (yyt->if_instr.ELSEIFS);
}

static void yWriteNodeassignment_instr
# ifdef HAVE_ARGS
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyWriteSelector ("Lhs");
 yyWriteAddr (yyt->assignment_instr.Lhs);
 yyWriteSelector ("Rhs");
 yyWriteAddr (yyt->assignment_instr.Rhs);
}

static void yWriteNodewhile_instr
# ifdef HAVE_ARGS
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyWriteSelector ("Test");
 yyWriteAddr (yyt->while_instr.Test);
 yyWriteSelector ("Body");
 yyWriteAddr (yyt->while_instr.Body);
}

static void yWriteNodefor_instr
# ifdef HAVE_ARGS
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyWriteSelector ("Assignment");
 yyWriteAddr (yyt->for_instr.Assignment);
 yyWriteSelector ("Test");
 yyWriteAddr (yyt->for_instr.Test);
 yyWriteSelector ("IncAssignment");
 yyWriteAddr (yyt->for_instr.IncAssignment);
 yyWriteSelector ("Body");
 yyWriteAddr (yyt->for_instr.Body);
}

static void yWriteNodetest
# ifdef HAVE_ARGS
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyWriteSelector ("operator");
 writetStringRef (yyt->test.operator) xxWriteNl ();
 yyWriteSelector ("Left");
 yyWriteAddr (yyt->test.Left);
 yyWriteSelector ("Right");
 yyWriteAddr (yyt->test.Right);
}

static void yWriteNodeelseifs
# ifdef HAVE_ARGS
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyWriteSelector ("Next");
 yyWriteAddr (yyt->elseifs.Next);
 yyWriteSelector ("Test");
 yyWriteAddr (yyt->elseifs.Test);
 yyWriteSelector ("Then");
 yyWriteAddr (yyt->elseifs.Then);
}

static void yWriteNodeint_const
# ifdef HAVE_ARGS
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyWriteSelector ("Value");
 writeint (yyt->int_const.Value) xxWriteNl ();
}

static void yWriteNodefloat_const
# ifdef HAVE_ARGS
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyWriteSelector ("Value");
 writefloat (yyt->float_const.Value) xxWriteNl ();
}

static void yWriteNodestring_const
# ifdef HAVE_ARGS
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyWriteSelector ("Value");
 writetStringRef (yyt->string_const.Value) xxWriteNl ();
}

static void yWriteNodename
# ifdef HAVE_ARGS
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyWriteSelector ("Ident");
 yyWriteAddr (yyt->name.Ident);
}

static void yWriteNodebinary_expr
# ifdef HAVE_ARGS
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyWriteSelector ("Op");
 writechar (yyt->binary_expr.Op) xxWriteNl ();
 yyWriteSelector ("Left");
 yyWriteAddr (yyt->binary_expr.Left);
 yyWriteSelector ("Right");
 yyWriteAddr (yyt->binary_expr.Right);
}

static void yWriteNodeunary_expr
# ifdef HAVE_ARGS
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyWriteSelector ("Op");
 writechar (yyt->unary_expr.Op) xxWriteNl ();
 yyWriteSelector ("Expr");
 yyWriteAddr (yyt->unary_expr.Expr);
}

void WriteTreeNode
# ifdef HAVE_ARGS
 (FILE * yyyf, tTree yyt)
# else
 (yyyf, yyt) FILE * yyyf; tTree yyt;
# endif
{
 yyf = yyyf;
 if (yyt == NoTree) { fputs ("NoTree\n", yyf); return; }

 fputs (Tree_NodeName [yyt->Kind], yyf); fputc ('\n', yyf);
 writeNodeHead (yyt)
 switch (yyt->Kind) {
case kprogram:
 yWriteNodeprogram (yyt); break;
case kdecls:
 yWriteNodedecls (yyt); break;
case kdecl:
 yWriteNodedecl (yyt); break;
case kidentifier:
 yWriteNodeidentifier (yyt); break;
case kinstrs:
 yWriteNodeinstrs (yyt); break;
case kif_instr:
 yWriteNodeif_instr (yyt); break;
case kassignment_instr:
 yWriteNodeassignment_instr (yyt); break;
case kwhile_instr:
 yWriteNodewhile_instr (yyt); break;
case kfor_instr:
 yWriteNodefor_instr (yyt); break;
case ktest:
 yWriteNodetest (yyt); break;
case kelseifs:
 yWriteNodeelseifs (yyt); break;
case kint_const:
 yWriteNodeint_const (yyt); break;
case kfloat_const:
 yWriteNodefloat_const (yyt); break;
case kstring_const:
 yWriteNodestring_const (yyt); break;
case kname:
 yWriteNodename (yyt); break;
case kbinary_expr:
 yWriteNodebinary_expr (yyt); break;
case kunary_expr:
 yWriteNodeunary_expr (yyt); break;
 default: ;
 }
}

static short yyIndentLevel;

void WriteTree
# ifdef HAVE_ARGS
 (FILE * yyyf, tTree yyt)
# else
 (yyyf, yyt) FILE * yyyf; tTree yyt;
# endif
{
 short yySaveLevel = yyIndentLevel;
 yyf = yyyf;
 yyMark (yyt);
 yyIndentLevel = 0;
 yyBeginTreeStore ();
 yyWriteTree (yyt);
 yyCloseTreeStore ();
 yyIndentLevel = yySaveLevel;
 if (ferror (yyyf)) {
  ErrorMessageI (xxTreeIOError, xxFatal, NoPosition, xxString, "Tree.WriteTree");
  Tree_Exit ();
 }
}

static void yyIndentSelector
# ifdef HAVE_ARGS
 (char * yys)
# else
 (yys) char * yys;
# endif
{
 register int yyi;
 for (yyi = 1; yyi <= yyIndentLevel; yyi ++) putc (' ', yyf);
 yyWriteSelector (yys);
}

static void yyIndentSelectorTree
# ifdef HAVE_ARGS
 (char * yys, tTree yyt)
# else
 (yys, yyt) char * yys; tTree yyt;
# endif
{ writeSELECTOR (yys); yyWriteTree (yyt); }

static void yWriteprogram
# ifdef HAVE_ARGS
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
{ writeSELECTOR ("Name");
writetStringRef (yyt->program.Name) xxWriteNl (); }
 yyIndentSelectorTree ("Declarations", yyt->program.Declarations);
}

static void yWritedecls
# ifdef HAVE_ARGS
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyIndentSelectorTree ("Decl", yyt->decls.Decl);
}

static void yWritedecl
# ifdef HAVE_ARGS
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
{ writeSELECTOR ("IOFlag");
writetIOFlag (yyt->decl.IOFlag) xxWriteNl (); }
{ writeSELECTOR ("Type");
writetVarType (yyt->decl.Type) xxWriteNl (); }
}

static void yWriteidentifier
# ifdef HAVE_ARGS
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
{ writeSELECTOR ("Ident");
writetIdent (yyt->identifier.Ident) xxWriteNl (); }
}

static void yWriteinstrs
# ifdef HAVE_ARGS
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyIndentSelectorTree ("Instr", yyt->instrs.Instr);
}

static void yWriteif_instr
# ifdef HAVE_ARGS
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyIndentSelectorTree ("Test", yyt->if_instr.Test);
 yyIndentSelectorTree ("Then", yyt->if_instr.Then);
 yyIndentSelectorTree ("Else", yyt->if_instr.Else);
}

static void yWriteassignment_instr
# ifdef HAVE_ARGS
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyIndentSelectorTree ("Lhs", yyt->assignment_instr.Lhs);
}

static void yWritewhile_instr
# ifdef HAVE_ARGS
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyIndentSelectorTree ("Test", yyt->while_instr.Test);
}

static void yWritefor_instr
# ifdef HAVE_ARGS
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyIndentSelectorTree ("Assignment", yyt->for_instr.Assignment);
 yyIndentSelectorTree ("Test", yyt->for_instr.Test);
 yyIndentSelectorTree ("IncAssignment", yyt->for_instr.IncAssignment);
}

static void yWritetest
# ifdef HAVE_ARGS
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
{ writeSELECTOR ("operator");
writetStringRef (yyt->test.operator) xxWriteNl (); }
 yyIndentSelectorTree ("Left", yyt->test.Left);
}

static void yWriteelseifs
# ifdef HAVE_ARGS
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 yyIndentSelectorTree ("Test", yyt->elseifs.Test);
 yyIndentSelectorTree ("Then", yyt->elseifs.Then);
}

static void yWriteint_const
# ifdef HAVE_ARGS
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
{ writeSELECTOR ("Value");
writeint (yyt->int_const.Value) xxWriteNl (); }
}

static void yWritefloat_const
# ifdef HAVE_ARGS
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
{ writeSELECTOR ("Value");
writefloat (yyt->float_const.Value) xxWriteNl (); }
}

static void yWritestring_const
# ifdef HAVE_ARGS
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
{ writeSELECTOR ("Value");
writetStringRef (yyt->string_const.Value) xxWriteNl (); }
}

static void yWritename
# ifdef HAVE_ARGS
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
}

static void yWritebinary_expr
# ifdef HAVE_ARGS
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
{ writeSELECTOR ("Op");
writechar (yyt->binary_expr.Op) xxWriteNl (); }
 yyIndentSelectorTree ("Left", yyt->binary_expr.Left);
}

static void yWriteunary_expr
# ifdef HAVE_ARGS
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
{ writeSELECTOR ("Op");
writechar (yyt->unary_expr.Op) xxWriteNl (); }
}

static void yyWriteTree
# ifdef HAVE_ARGS
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{ unsigned short yyLevel = yyIndentLevel;
 for (;;) {
  if (yyt == NoTree) { fputs (" NoTree\n", yyf); goto yyExit;
  } else if (yyt->yyHead.yyMark == 0) {
   fprintf (yyf, "^%d\n", yyMapToLabel (yyt)); goto yyExit;
  } else if (yyt->yyHead.yyMark > 1) {
   register int yyi;
   fprintf (yyf, "\n%06d:", yyMapToLabel (yyt));
   for (yyi = 8; yyi <= yyIndentLevel; yyi ++) putc (' ', yyf);
  } else putc (' ', yyf);
  yyt->yyHead.yyMark = 0;
  yyIndentLevel += 2;

  fputs (Tree_NodeName [yyt->Kind], yyf); fputc ('\n', yyf);
  writeNodeHead (yyt)
  switch (yyt->Kind) {
case kprogram: yWriteprogram (yyt);
writeSELECTOR ("Instructions");
yyt = yyt->program.Instructions; break;
case kdecls: yWritedecls (yyt);
writeSELECTOR ("Next");
yyt = yyt->decls.Next; break;
case kdecl: yWritedecl (yyt);
writeSELECTOR ("Identifier");
yyt = yyt->decl.Identifier; break;
case kidentifier: yWriteidentifier (yyt); goto yyExit;
case kinstrs: yWriteinstrs (yyt);
writeSELECTOR ("Next");
yyt = yyt->instrs.Next; break;
case kif_instr: yWriteif_instr (yyt);
writeSELECTOR ("ELSEIFS");
yyt = yyt->if_instr.ELSEIFS; break;
case kassignment_instr: yWriteassignment_instr (yyt);
writeSELECTOR ("Rhs");
yyt = yyt->assignment_instr.Rhs; break;
case kwhile_instr: yWritewhile_instr (yyt);
writeSELECTOR ("Body");
yyt = yyt->while_instr.Body; break;
case kfor_instr: yWritefor_instr (yyt);
writeSELECTOR ("Body");
yyt = yyt->for_instr.Body; break;
case ktest: yWritetest (yyt);
writeSELECTOR ("Right");
yyt = yyt->test.Right; break;
case kelseifs: yWriteelseifs (yyt);
writeSELECTOR ("Next");
yyt = yyt->elseifs.Next; break;
case kint_const: yWriteint_const (yyt); goto yyExit;
case kfloat_const: yWritefloat_const (yyt); goto yyExit;
case kstring_const: yWritestring_const (yyt); goto yyExit;
case kname: yWritename (yyt);
writeSELECTOR ("Ident");
yyt = yyt->name.Ident; break;
case kbinary_expr: yWritebinary_expr (yyt);
writeSELECTOR ("Right");
yyt = yyt->binary_expr.Right; break;
case kunary_expr: yWriteunary_expr (yyt);
writeSELECTOR ("Expr");
yyt = yyt->unary_expr.Expr; break;
  default: goto yyExit;
  }
 }
yyExit:
 yyIndentLevel = yyLevel;
}

static Tree_tProcTree yyProc;

static void yyTraverseTreeTD
# ifdef HAVE_ARGS
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 for (;;) {
  if (yyt == NoTree || yyt->yyHead.yyMark == 0) return;
  yyProc (yyt);
  yyt->yyHead.yyMark = 0;

  switch (yyt->Kind) {
case kprogram:
yyTraverseTreeTD (yyt->program.Declarations);
yyt = yyt->program.Instructions; break;
case kdecls:
yyTraverseTreeTD (yyt->decls.Decl);
yyt = yyt->decls.Next; break;
case kdecl:
yyt = yyt->decl.Identifier; break;
case kinstrs:
yyTraverseTreeTD (yyt->instrs.Instr);
yyt = yyt->instrs.Next; break;
case kif_instr:
yyTraverseTreeTD (yyt->if_instr.Test);
yyTraverseTreeTD (yyt->if_instr.Then);
yyTraverseTreeTD (yyt->if_instr.Else);
yyt = yyt->if_instr.ELSEIFS; break;
case kassignment_instr:
yyTraverseTreeTD (yyt->assignment_instr.Lhs);
yyt = yyt->assignment_instr.Rhs; break;
case kwhile_instr:
yyTraverseTreeTD (yyt->while_instr.Test);
yyt = yyt->while_instr.Body; break;
case kfor_instr:
yyTraverseTreeTD (yyt->for_instr.Assignment);
yyTraverseTreeTD (yyt->for_instr.Test);
yyTraverseTreeTD (yyt->for_instr.IncAssignment);
yyt = yyt->for_instr.Body; break;
case ktest:
yyTraverseTreeTD (yyt->test.Left);
yyt = yyt->test.Right; break;
case kelseifs:
yyTraverseTreeTD (yyt->elseifs.Test);
yyTraverseTreeTD (yyt->elseifs.Then);
yyt = yyt->elseifs.Next; break;
case kname:
yyt = yyt->name.Ident; break;
case kbinary_expr:
yyTraverseTreeTD (yyt->binary_expr.Left);
yyt = yyt->binary_expr.Right; break;
case kunary_expr:
yyt = yyt->unary_expr.Expr; break;
  default: return;
  }
 }
}

void TraverseTreeTD
# ifdef HAVE_ARGS
 (tTree yyt, Tree_tProcTree yyyProc)
# else
 (yyt, yyyProc) tTree yyt; Tree_tProcTree yyyProc;
# endif
{
# ifdef SUPPORT_TVA
 rbool yyPrevTreatTVAasChild = Tree_TreatTVAasChild;
 Tree_TreatTVAasChild = rfalse;
 yyMark (yyt);
 Tree_TreatTVAasChild = yyPrevTreatTVAasChild;
# else
 yyMark (yyt);
# endif
 yyProc = yyyProc;
 yyTraverseTreeTD (yyt);
}

tTree ReverseTree
# ifdef HAVE_ARGS
 (tTree yyOld)
# else
 (yyOld) tTree yyOld;
# endif
{
 register tTree yyNew, yyNext, yyTail;
 yyNew = yyOld;
 yyTail = yyOld;
 for (;;) {
  switch (yyOld->Kind) {
case kdecls: yyNext = yyOld->decls.Next;
 yyOld->decls.Next = yyNew; break;
case kinstrs: yyNext = yyOld->instrs.Next;
 yyOld->instrs.Next = yyNew; break;
case kelseifs: yyNext = yyOld->elseifs.Next;
 yyOld->elseifs.Next = yyNew; break;
  default: goto yyExit;
  }
  yyNew = yyOld;
  yyOld = yyNext;
 }
yyExit:
 switch (yyTail->Kind) {
case kdecls: yyTail->decls.Next = yyOld; break;
case kinstrs: yyTail->instrs.Next = yyOld; break;
case kelseifs: yyTail->elseifs.Next = yyOld; break;
 default: ;
 }
 return yyNew;
}

void ForallTree
# ifdef HAVE_ARGS
 (tTree yyt, Tree_tProcTree yyProc)
# else
 (yyt, yyProc) tTree yyt; Tree_tProcTree yyProc;
# endif
{
 for (;;) {
  tTree yyyt;
  if ((yyyt = yyt) == NoTree) return;
  switch (yyt->Kind) {
case kdecls: yyt = yyt->decls.Next; break;
case kinstrs: yyt = yyt->instrs.Next; break;
case kelseifs: yyt = yyt->elseifs.Next; break;
  default: return;
  }
  yyProc (yyyt);
 }
}

static rbool yyResult;

static void yyCheckTree ARGS ((tTree yyt));

rbool CheckTree
# ifdef HAVE_ARGS
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
# ifdef SUPPORT_TVA
 rbool yyPrevTreatTVAasChild = Tree_TreatTVAasChild;
 Tree_TreatTVAasChild = rfalse;
 yyMark (yyt);
 Tree_TreatTVAasChild = yyPrevTreatTVAasChild;
# else
 yyMark (yyt);
# endif
 yyResult = rtrue;
 yyCheckTree (yyt);
 return yyResult;
}

static void yyCheckChild2
# ifdef HAVE_ARGS
 (tTree yyParent, tTree yyChild, Tree_tKind yyType, char * yySelector)
# else
 (yyParent, yyChild, yyType, yySelector)
 tTree yyParent, yyChild;
 Tree_tKind yyType;
 char * yySelector;
# endif
{
 if (yyChild == NoTree) {
  if (Tree_CheckReportNoTree) goto yyReport;
 } else if (yyType > yyChild->Kind || yyChild->Kind > Tree_TypeRange [yyType]) {
yyReport: yyResult = rfalse;
  fprintf (stderr, "%s\n", CodeToText (xxCheckTreeError));
  fprintf (stderr, "   %s %p\n", CodeToText (xxCheckTreeAddrOfParent), (void *) yyParent);
  fprintf (stderr, "   %s %s\n", CodeToText (xxCheckTreeTypeOfParent), Tree_NodeName [yyParent->Kind]);
  fprintf (stderr, "   %s %s\n", CodeToText (xxCheckTreeNameOfChild), yySelector);
  if (yyChild == NoTree)
   fprintf (stderr, "   %s NoTree\n", CodeToText (xxCheckTreeValueOfChild));
  else {
   fprintf (stderr, "   %s %p\n", CodeToText (xxCheckTreeAddrOfChild), (void *) yyChild);
   fprintf (stderr, "   %s %s\n", CodeToText (xxCheckTreeTypeOfChild), Tree_NodeName [yyChild->Kind]);
  }
  fprintf (stderr, "   %s %s\n", CodeToText (xxCheckTreeExpectedType), Tree_NodeName [yyType]);
  if (Tree_CheckReportNodes) {
   fprintf (stderr, "   %s\n", CodeToText (xxCheckTreeParentNode));
   WriteTreeNode (stderr, yyParent);
   fprintf (stderr, "   %s\n", CodeToText (xxCheckTreeChildNode));
   WriteTreeNode (stderr, yyChild);
  }
  fputc ('\n', stderr);
 }
}

static void yyCheckChild
# ifdef HAVE_ARGS
 (tTree yyParent, tTree yyChild, Tree_tKind yyType, char * yySelector)
# else
 (yyParent, yyChild, yyType, yySelector)
 tTree yyParent, yyChild;
 Tree_tKind yyType;
 char * yySelector;
# endif
{
 yyCheckChild2 (yyParent, yyChild, yyType, yySelector);
 yyCheckTree (yyChild);
}

static void yyCheckTree
# ifdef HAVE_ARGS
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 for (;;) {
  if (yyt == NoTree) { if (Tree_CheckReportNoTree) yyResult = rfalse; return; }
  if (yyt->yyHead.yyMark == 0) return;
  yyt->yyHead.yyMark = 0;

  switch (yyt->Kind) {
case kprogram:
yyCheckChild (yyt, yyt->program.Declarations,
kDECLS, "Declarations");
yyCheckChild2 (yyt, yyt->program.Instructions,
kINSTRS, "Instructions");
yyt = yyt->program.Instructions; break;
case kdecls:
yyCheckChild (yyt, yyt->decls.Decl,
kdecl, "Decl");
yyCheckChild2 (yyt, yyt->decls.Next,
kDECLS, "Next");
yyt = yyt->decls.Next; break;
case kdecl:
yyCheckChild2 (yyt, yyt->decl.Identifier,
kidentifier, "Identifier");
yyt = yyt->decl.Identifier; break;
case kinstrs:
yyCheckChild (yyt, yyt->instrs.Instr,
kINSTR, "Instr");
yyCheckChild2 (yyt, yyt->instrs.Next,
kINSTRS, "Next");
yyt = yyt->instrs.Next; break;
case kif_instr:
yyCheckChild (yyt, yyt->if_instr.Test,
ktest, "Test");
yyCheckChild (yyt, yyt->if_instr.Then,
kINSTRS, "Then");
yyCheckChild (yyt, yyt->if_instr.Else,
kINSTRS, "Else");
yyCheckChild2 (yyt, yyt->if_instr.ELSEIFS,
kELSEIFS, "ELSEIFS");
yyt = yyt->if_instr.ELSEIFS; break;
case kassignment_instr:
yyCheckChild (yyt, yyt->assignment_instr.Lhs,
kidentifier, "Lhs");
yyCheckChild2 (yyt, yyt->assignment_instr.Rhs,
kEXPR, "Rhs");
yyt = yyt->assignment_instr.Rhs; break;
case kwhile_instr:
yyCheckChild (yyt, yyt->while_instr.Test,
ktest, "Test");
yyCheckChild2 (yyt, yyt->while_instr.Body,
kINSTRS, "Body");
yyt = yyt->while_instr.Body; break;
case kfor_instr:
yyCheckChild (yyt, yyt->for_instr.Assignment,
kassignment_instr, "Assignment");
yyCheckChild (yyt, yyt->for_instr.Test,
ktest, "Test");
yyCheckChild (yyt, yyt->for_instr.IncAssignment,
kassignment_instr, "IncAssignment");
yyCheckChild2 (yyt, yyt->for_instr.Body,
kINSTRS, "Body");
yyt = yyt->for_instr.Body; break;
case ktest:
yyCheckChild (yyt, yyt->test.Left,
kEXPR, "Left");
yyCheckChild2 (yyt, yyt->test.Right,
kEXPR, "Right");
yyt = yyt->test.Right; break;
case kelseifs:
yyCheckChild (yyt, yyt->elseifs.Test,
ktest, "Test");
yyCheckChild (yyt, yyt->elseifs.Then,
kINSTRS, "Then");
yyCheckChild2 (yyt, yyt->elseifs.Next,
kELSEIFS, "Next");
yyt = yyt->elseifs.Next; break;
case kname:
yyCheckChild2 (yyt, yyt->name.Ident,
kidentifier, "Ident");
yyt = yyt->name.Ident; break;
case kbinary_expr:
yyCheckChild (yyt, yyt->binary_expr.Left,
kEXPR, "Left");
yyCheckChild2 (yyt, yyt->binary_expr.Right,
kEXPR, "Right");
yyt = yyt->binary_expr.Right; break;
case kunary_expr:
yyCheckChild2 (yyt, yyt->unary_expr.Expr,
kEXPR, "Expr");
yyt = yyt->unary_expr.Expr; break;
  default: return;
  }
 }
}

# ifndef EXTERN_C_BEGIN
# define EXTERN_C_BEGIN
# define EXTERN_C_END
# endif

EXTERN_C_BEGIN
# include "Position.h"
EXTERN_C_END

static unsigned long yyLine, yyCurLine;
static tTree yyTheTree, yyNode;
static tIdent yyFile;

static void yySearch4
# ifdef HAVE_ARGS
 (tTree yyt, tPosition yyp)
# else
 (yyt, yyp) tTree yyt; tPosition yyp;
# endif
{
 if ((yyFile == NoIdent || yyFile == yyp.FileName) &&
   yyLine <= yyp.Line && yyp.Line < yyCurLine)
  { yyNode = yyt; yyCurLine = yyp.Line; }
}

static void yySearch2
# ifdef HAVE_ARGS
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 switch (yyt->Kind) {
 }
}

# include <tcl.h>
# include <tk.h>

# if (TCL_MAJOR_VERSION >= 8) && (TCL_MINOR_VERSION >= 4)
#  define AST_CONST const
# else
#  define AST_CONST
# endif

# define yyMaxCoord		65535
# define yyIsBusy		65535
# define yyTruncByDepth	65535
# define yyTruncByLength	65534
static Tcl_Interp * yygInterp;

static int yySearch
# ifdef HAVE_ARGS
 (ClientData yyclass, Tcl_Interp * yyinterp, int yyargc, AST_CONST char * yyargv [])
# else
 (yyclass, yyinterp, yyargc, yyargv)
 ClientData	yyclass;
 Tcl_Interp *	yyinterp;
 int		yyargc;
 char *	yyargv [];
# endif
{
 tTree yyTheTree = (tTree) atol (yyargv [1]);
 char yyArray [32];
 yyLine = atol (yyargv [2]);
 yyFile = yyargv [3][0] == '\0' ? NoIdent : MakeIdent1 (yyargv [3]);
 yyCurLine = 1000000;
 yyNode = yyTheTree;
 TraverseTreeTD (yyTheTree, yySearch2);
 sprintf (yyArray, "%lu", (unsigned long) yyNode);
 Tcl_SetResult (yyinterp, yyArray, TCL_VOLATILE);
 return TCL_OK;
}

static rbool yyphase1;
static int yymaxx, yymaxy;

static int yySetY
# ifdef HAVE_ARGS
 (tTree yyp, tTree yyt, int yyy, int yyk, int yyl)
# else
 (yyp, yyt, yyy, yyk, yyl) tTree yyp, yyt; int yyy, yyk, yyl;
# endif
{
 int yy, yymax = ++ yyy;
 if (yyphase1) {
  if (yyt == NoTree || (yyt->yyHead.yyMark == 0 &&
   (yyt->yyHead.yyx == yyIsBusy || yyt->yyHead.yyy >= (unsigned short) yyy ||
    yyp->yyHead.yyy >= yyTruncByLength)))
   return yymax;
  yyt->yyHead.yyparent = yyp;
  yyt->yyHead.yyx = yyIsBusy;
 } else {
  if (yyt == NoTree ||
   yyt->yyHead.yyMark == 0 || yyt->yyHead.yyparent != yyp) return yymax;
 }
 yyt->yyHead.yyMark = 0;
 if (++ yyl > Tree_DrawLength) yyk = Tree_DrawDepth;
 yyk ++;
 yyt->yyHead.yyy = (unsigned short) (yyk <= Tree_DrawDepth ? 0 : yyTruncByDepth);
 switch (yyt->Kind) {
case kprogram:
yy = yySetY (yyt, yyt->program.Declarations, yyy, yyk, 0);
yymax = Max (yymax, yy);
yy = yySetY (yyt, yyt->program.Instructions, yyy, yyk, 0);
yymax = Max (yymax, yy);
break;
case kdecls:
yy = yySetY (yyt, yyt->decls.Decl, yyy, yyk, 0);
yymax = Max (yymax, yy);
yymax = yySetY (yyt, yyt->decls.Next, yymax - 1, yyk - 1, yyl);
break;
case kdecl:
yy = yySetY (yyt, yyt->decl.Identifier, yyy, yyk, 0);
yymax = Max (yymax, yy);
break;
case kinstrs:
yy = yySetY (yyt, yyt->instrs.Instr, yyy, yyk, 0);
yymax = Max (yymax, yy);
yymax = yySetY (yyt, yyt->instrs.Next, yymax - 1, yyk - 1, yyl);
break;
case kif_instr:
yy = yySetY (yyt, yyt->if_instr.Test, yyy, yyk, 0);
yymax = Max (yymax, yy);
yy = yySetY (yyt, yyt->if_instr.Then, yyy, yyk, 0);
yymax = Max (yymax, yy);
yy = yySetY (yyt, yyt->if_instr.Else, yyy, yyk, 0);
yymax = Max (yymax, yy);
yy = yySetY (yyt, yyt->if_instr.ELSEIFS, yyy, yyk, 0);
yymax = Max (yymax, yy);
break;
case kassignment_instr:
yy = yySetY (yyt, yyt->assignment_instr.Lhs, yyy, yyk, 0);
yymax = Max (yymax, yy);
yy = yySetY (yyt, yyt->assignment_instr.Rhs, yyy, yyk, 0);
yymax = Max (yymax, yy);
break;
case kwhile_instr:
yy = yySetY (yyt, yyt->while_instr.Test, yyy, yyk, 0);
yymax = Max (yymax, yy);
yy = yySetY (yyt, yyt->while_instr.Body, yyy, yyk, 0);
yymax = Max (yymax, yy);
break;
case kfor_instr:
yy = yySetY (yyt, yyt->for_instr.Assignment, yyy, yyk, 0);
yymax = Max (yymax, yy);
yy = yySetY (yyt, yyt->for_instr.Test, yyy, yyk, 0);
yymax = Max (yymax, yy);
yy = yySetY (yyt, yyt->for_instr.IncAssignment, yyy, yyk, 0);
yymax = Max (yymax, yy);
yy = yySetY (yyt, yyt->for_instr.Body, yyy, yyk, 0);
yymax = Max (yymax, yy);
break;
case ktest:
yy = yySetY (yyt, yyt->test.Left, yyy, yyk, 0);
yymax = Max (yymax, yy);
yy = yySetY (yyt, yyt->test.Right, yyy, yyk, 0);
yymax = Max (yymax, yy);
break;
case kelseifs:
yy = yySetY (yyt, yyt->elseifs.Test, yyy, yyk, 0);
yymax = Max (yymax, yy);
yy = yySetY (yyt, yyt->elseifs.Then, yyy, yyk, 0);
yymax = Max (yymax, yy);
yymax = yySetY (yyt, yyt->elseifs.Next, yymax - 1, yyk - 1, yyl);
break;
case kname:
yy = yySetY (yyt, yyt->name.Ident, yyy, yyk, 0);
yymax = Max (yymax, yy);
break;
case kbinary_expr:
yy = yySetY (yyt, yyt->binary_expr.Left, yyy, yyk, 0);
yymax = Max (yymax, yy);
yy = yySetY (yyt, yyt->binary_expr.Right, yyy, yyk, 0);
yymax = Max (yymax, yy);
break;
case kunary_expr:
yy = yySetY (yyt, yyt->unary_expr.Expr, yyy, yyk, 0);
yymax = Max (yymax, yy);
break;
 }
 yyt->yyHead.yyx = 0;
 if (yyl > Tree_DrawLength) {
  yyt->yyHead.yyy = yyTruncByLength;
  return yyy;
 } else if (yyk > Tree_DrawDepth) {
  yyt->yyHead.yyy = yyTruncByDepth;
  return yyy;
 } else {
  yyt->yyHead.yyy = (unsigned short) yyy;
  yymaxy = Max (yymaxy, yyy);
  return yymax;
 }
}

typedef struct { unsigned short yyfirst, yylast; } yytFirstLast;

static int yySetX
# ifdef HAVE_ARGS
 (tTree yyp, tTree yyt, int yyx, yytFirstLast * yyout)
# else
 (yyp, yyt, yyx, yyout) tTree yyp, yyt; int yyx; yytFirstLast * yyout;
# endif
{
 int yyxin = yyx, yyw;
 yytFirstLast yyFirstLast;
 if (yyt == NoTree || yyt->yyHead.yyMark == 0 ||
  yyt->yyHead.yyparent != yyp) return yyx;
 yyt->yyHead.yyMark = 0;
 yyFirstLast.yyfirst = yyMaxCoord;
 yyFirstLast.yylast = 0;
 switch (yyt->Kind) {
case kprogram:
yyx = yySetX (yyt, yyt->program.Declarations, yyx, & yyFirstLast);
yyx = yySetX (yyt, yyt->program.Instructions, yyx, & yyFirstLast);
break;
case kdecls:
yyw = yySetX (yyt, yyt->decls.Next, yyx ++, & yyFirstLast);
yyx = yySetX (yyt, yyt->decls.Decl, yyx, & yyFirstLast);
goto yyList;
case kdecl:
yyx = yySetX (yyt, yyt->decl.Identifier, yyx, & yyFirstLast);
break;
case kinstrs:
yyw = yySetX (yyt, yyt->instrs.Next, yyx ++, & yyFirstLast);
yyx = yySetX (yyt, yyt->instrs.Instr, yyx, & yyFirstLast);
goto yyList;
case kif_instr:
yyx = yySetX (yyt, yyt->if_instr.Test, yyx, & yyFirstLast);
yyx = yySetX (yyt, yyt->if_instr.Then, yyx, & yyFirstLast);
yyx = yySetX (yyt, yyt->if_instr.Else, yyx, & yyFirstLast);
yyx = yySetX (yyt, yyt->if_instr.ELSEIFS, yyx, & yyFirstLast);
break;
case kassignment_instr:
yyx = yySetX (yyt, yyt->assignment_instr.Lhs, yyx, & yyFirstLast);
yyx = yySetX (yyt, yyt->assignment_instr.Rhs, yyx, & yyFirstLast);
break;
case kwhile_instr:
yyx = yySetX (yyt, yyt->while_instr.Test, yyx, & yyFirstLast);
yyx = yySetX (yyt, yyt->while_instr.Body, yyx, & yyFirstLast);
break;
case kfor_instr:
yyx = yySetX (yyt, yyt->for_instr.Assignment, yyx, & yyFirstLast);
yyx = yySetX (yyt, yyt->for_instr.Test, yyx, & yyFirstLast);
yyx = yySetX (yyt, yyt->for_instr.IncAssignment, yyx, & yyFirstLast);
yyx = yySetX (yyt, yyt->for_instr.Body, yyx, & yyFirstLast);
break;
case ktest:
yyx = yySetX (yyt, yyt->test.Left, yyx, & yyFirstLast);
yyx = yySetX (yyt, yyt->test.Right, yyx, & yyFirstLast);
break;
case kelseifs:
yyw = yySetX (yyt, yyt->elseifs.Next, yyx ++, & yyFirstLast);
yyx = yySetX (yyt, yyt->elseifs.Test, yyx, & yyFirstLast);
yyx = yySetX (yyt, yyt->elseifs.Then, yyx, & yyFirstLast);
goto yyList;
case kname:
yyx = yySetX (yyt, yyt->name.Ident, yyx, & yyFirstLast);
break;
case kbinary_expr:
yyx = yySetX (yyt, yyt->binary_expr.Left, yyx, & yyFirstLast);
yyx = yySetX (yyt, yyt->binary_expr.Right, yyx, & yyFirstLast);
break;
case kunary_expr:
yyx = yySetX (yyt, yyt->unary_expr.Expr, yyx, & yyFirstLast);
break;
 }
 if (yyt->yyHead.yyy >= yyTruncByLength) {
  return yyxin;
 } else {
  yyt->yyHead.yyx = (unsigned short) (yyFirstLast.yyfirst == yyMaxCoord ? yyx :
   (yyFirstLast.yyfirst + yyFirstLast.yylast) / 2);
  yymaxx = Max ((unsigned short) yymaxx, yyt->yyHead.yyx);
  if (yyt->yyHead.yyparent == yyp) {
   if (yyout->yyfirst == yyMaxCoord) yyout->yyfirst = yyt->yyHead.yyx;
   yyout->yylast = yyt->yyHead.yyx;
  }
  yyxin ++;
  return Max (yyx, yyxin);
 }
yyList:
 if (yyt->yyHead.yyy >= yyTruncByLength) {
  return yyxin;
 } else {
  yyt->yyHead.yyx = (unsigned short) yyxin;
  yymaxx = Max (yymaxx, yyxin);
  if (yyt->yyHead.yyparent == yyp) {
   if (yyout->yyfirst == yyMaxCoord) yyout->yyfirst = yyt->yyHead.yyx;
   yyout->yylast = yyt->yyHead.yyx;
  }
  return Max (yyx, yyw);
 }
}

static void yyDrawEdge
# ifdef HAVE_ARGS
 (tTree yyfrom, tTree yyto)
# else
 (yyfrom, yyto) tTree yyfrom, yyto;
# endif
{
 char yyCoord [32];
 if (yyto == NoTree) return;
 if (yyto->yyHead.yyy < yyTruncByLength)
  sprintf (yyCoord, "%d %d %d %d", yyfrom->yyHead.yyx, yyfrom->yyHead.yyy,
   yyto->yyHead.yyx, yyto->yyHead.yyy);
 else if (yyto->yyHead.yyy == yyTruncByDepth)
  sprintf (yyCoord, "%d %d %d %d 1", yyfrom->yyHead.yyx, yyfrom->yyHead.yyy,
   yyfrom->yyHead.yyx + 1, yyfrom->yyHead.yyy);
 else
  sprintf (yyCoord, "%d %d %d %d 1", yyfrom->yyHead.yyx, yyfrom->yyHead.yyy,
   yyfrom->yyHead.yyx, yyfrom->yyHead.yyy + 1);
 Tcl_VarEval (yygInterp, "draw_edge ", yyCoord, NULL);
}

static void yyDrawNode
# ifdef HAVE_ARGS
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 char yyCoord [32];
 if (yyt->yyHead.yyy >= yyTruncByLength) return;
 sprintf (yyCoord, "%d %d %lu ", yyt->yyHead.yyx, yyt->yyHead.yyy, (unsigned long) yyt);
 Tcl_VarEval (yygInterp, "draw_node ", yyCoord,
  Tree_NodeName [yyt->Kind], NULL);
 switch (yyt->Kind) {
case kprogram:
yyDrawEdge (yyt, yyt->program.Declarations);
yyDrawEdge (yyt, yyt->program.Instructions);
break;
case kdecls:
yyDrawEdge (yyt, yyt->decls.Next);
yyDrawEdge (yyt, yyt->decls.Decl);
break;
case kdecl:
yyDrawEdge (yyt, yyt->decl.Identifier);
break;
case kinstrs:
yyDrawEdge (yyt, yyt->instrs.Next);
yyDrawEdge (yyt, yyt->instrs.Instr);
break;
case kif_instr:
yyDrawEdge (yyt, yyt->if_instr.Test);
yyDrawEdge (yyt, yyt->if_instr.Then);
yyDrawEdge (yyt, yyt->if_instr.Else);
yyDrawEdge (yyt, yyt->if_instr.ELSEIFS);
break;
case kassignment_instr:
yyDrawEdge (yyt, yyt->assignment_instr.Lhs);
yyDrawEdge (yyt, yyt->assignment_instr.Rhs);
break;
case kwhile_instr:
yyDrawEdge (yyt, yyt->while_instr.Test);
yyDrawEdge (yyt, yyt->while_instr.Body);
break;
case kfor_instr:
yyDrawEdge (yyt, yyt->for_instr.Assignment);
yyDrawEdge (yyt, yyt->for_instr.Test);
yyDrawEdge (yyt, yyt->for_instr.IncAssignment);
yyDrawEdge (yyt, yyt->for_instr.Body);
break;
case ktest:
yyDrawEdge (yyt, yyt->test.Left);
yyDrawEdge (yyt, yyt->test.Right);
break;
case kelseifs:
yyDrawEdge (yyt, yyt->elseifs.Next);
yyDrawEdge (yyt, yyt->elseifs.Test);
yyDrawEdge (yyt, yyt->elseifs.Then);
break;
case kname:
yyDrawEdge (yyt, yyt->name.Ident);
break;
case kbinary_expr:
yyDrawEdge (yyt, yyt->binary_expr.Left);
yyDrawEdge (yyt, yyt->binary_expr.Right);
break;
case kunary_expr:
yyDrawEdge (yyt, yyt->unary_expr.Expr);
break;
 }
}

static int yyPutAttr
# ifdef HAVE_ARGS
 (ClientData yyclass, Tcl_Interp * yyinterp, int yyargc, AST_CONST char * yyargv [])
# else
 (yyclass, yyinterp, yyargc, yyargv)
 ClientData	yyclass;
 Tcl_Interp *	yyinterp;
 int		yyargc;
 char *	yyargv [];
# endif
{
 FILE * yyf = fopen ("yyNode", "w");
 if (yyf) {
  WriteTreeNode (yyf, (tTree) atol (yyargv [1]));
  fclose (yyf);
 } else {
  Tcl_SetResult (yyinterp, CodeToText (xxDrawTreeyyNode), TCL_STATIC);
 }
 return TCL_OK;
}

static void yyDrawTree (tTree yyt, ClientData yyclass);

static int yyDrawSubTree
# ifdef HAVE_ARGS
 (ClientData yyclass, Tcl_Interp * yyinterp, int yyargc, AST_CONST char * yyargv [])
# else
 (yyclass, yyinterp, yyargc, yyargv)
 ClientData	yyclass;
 Tcl_Interp *	yyinterp;
 int		yyargc;
 char *	yyargv [];
# endif
{
 DrawTree ((tTree) atol (yyargv [1]));
 return TCL_OK;
}

# ifndef DrawAttr
# define DrawAttr(x, y)
# endif

static int yyCallAttr
# ifdef HAVE_ARGS
 (ClientData yyclass, Tcl_Interp * yyinterp, int yyargc, AST_CONST char * yyargv [])
# else
 (yyclass, yyinterp, yyargc, yyargv)
 ClientData	yyclass;
 Tcl_Interp *	yyinterp;
 int		yyargc;
 char *	yyargv [];
# endif
{
 DrawAttr ((tTree) atol (yyargv [1]), (char *) yyargv [2]);
 return TCL_OK;
}

static int yySetParam
# ifdef HAVE_ARGS
 (ClientData yyclass, Tcl_Interp * yyinterp, int yyargc, AST_CONST char * yyargv [])
# else
 (yyclass, yyinterp, yyargc, yyargv)
 ClientData	yyclass;
 Tcl_Interp *	yyinterp;
 int		yyargc;
 char *	yyargv [];
# endif
{
 Tree_DrawDepth     = atoi (yyargv [1]);
 Tree_DrawLength    = atoi (yyargv [2]);
 Tree_DrawBoxWidth  = atoi (yyargv [3]);
 Tree_DrawBoxHeight = atoi (yyargv [4]);
 strncpy (Tree_DrawFileName, yyargv [5], 256);
 Tree_DrawFileName [255] = '\0';
 return TCL_OK;
}

void SetDepthTree
# ifdef HAVE_ARGS
 (int yyDepth)
# else
 (yyDepth) int yyDepth;
# endif
{
 Tree_DrawDepth = yyDepth;
}

void SetBoxTree
# ifdef HAVE_ARGS
 (int yyWidth, int yyHeight)
# else
 (yyWidth, yyHeight) int yyWidth, yyHeight;
# endif
{
 Tree_DrawBoxWidth  = yyWidth;
 Tree_DrawBoxHeight = yyHeight;
}

static void yyDrawTree
# ifdef HAVE_ARGS
 (tTree yyt, ClientData yyclass)
# else
 (yyt, yyclass) tTree yyt; ClientData yyclass;
# endif
{
 char yyString [128];
 yytFirstLast yyDummy;
# ifdef SUPPORT_TVA
 rbool yyPrevTreatTVAasChild = Tree_TreatTVAasChild;
 Tree_TreatTVAasChild = rfalse;
# endif
 yyDummy.yyfirst = yyMaxCoord;
 yymaxx = yymaxy = 0;
 yyMark (yyt); yyphase1 = rtrue ;
 yySetY ((tTree) & yyt, yyt, 0, 0, 0);
 yyMark (yyt); yyphase1 = rfalse;
 yySetY ((tTree) & yyt, yyt, 0, 0, 0);
 yyMark (yyt);
 yySetX ((tTree) & yyt, yyt, 0, & yyDummy);
 yymaxx = Max (yymaxx, 16);
 yymaxy = Max (yymaxy, 16);
 sprintf (yyString, "%d %d %d %d %d %d {%s}", yymaxx, yymaxy, Tree_DrawBoxWidth, Tree_DrawBoxHeight, Tree_DrawDepth, Tree_DrawLength, Tree_DrawFileName);
 Tcl_VarEval (yygInterp, "draw_tree ", yyString, NULL);
 TraverseTreeTD (yyt, yyDrawNode);
# ifdef SUPPORT_TVA
 Tree_TreatTVAasChild = yyPrevTreatTVAasChild;
# endif
}

void DrawTree
# ifdef HAVE_ARGS
 (tTree yyt)
# else
 (yyt) tTree yyt;
# endif
{
 int yyCode;
 Tcl_Interp * yyInterp;
 char yyPath [256], * yyp;

 Tcl_FindExecutable ("");
 yygInterp = yyInterp = Tcl_CreateInterp ();
 yyCode = Tcl_Init (yyInterp);
 if (yyCode != TCL_OK) {
  ErrorMessageI (xxTclTkError, xxError, NoPosition, xxString, Tcl_GetStringResult (yyInterp));
  goto yyReturn;
 }
 yyCode = Tk_Init (yyInterp);
 if (yyCode != TCL_OK) {
  ErrorMessageI (xxTclTkError, xxError, NoPosition, xxString, Tcl_GetStringResult (yyInterp));
  goto yyReturn;
 }
 Tcl_StaticPackage (yyInterp, "Tk", Tk_Init, (Tcl_PackageInitProc *) NULL);

 Tcl_CreateCommand (yyInterp, "put_attr", yyPutAttr, (ClientData) NULL, NULL);
 Tcl_CreateCommand (yyInterp, "search_pos", yySearch, (ClientData) NULL, NULL);
 Tcl_CreateCommand (yyInterp, "draw_subtree", yyDrawSubTree, (ClientData) NULL, NULL);
 Tcl_CreateCommand (yyInterp, "call_attr", yyCallAttr, (ClientData) NULL, NULL);
 Tcl_CreateCommand (yyInterp, "set_param", yySetParam, (ClientData) NULL, NULL);
 yyCode = Tcl_EvalFile (yyInterp, "Tree.tcl");
 if (yyCode != TCL_OK) {
  yyp = (char *) getenv ("HOME");
  if (yyp) {
   strcat (strcpy (yyPath, yyp), "/.Tree.tcl");
   yyCode = Tcl_EvalFile (yyInterp, yyPath);
  }
 }
 if (yyCode != TCL_OK) {
  yyp = (char *) getenv ("CT_DIR");
  if (yyp) {
   strcat (strcpy (yyPath, yyp), "/lib/cg/Tree.tcl");
   yyCode = Tcl_EvalFile (yyInterp, yyPath);
  }
 }
 if (yyCode != TCL_OK) {
  yyCode = Tcl_EvalFile (yyInterp, "/usr/local/lib/cocktail/cg/Tree.tcl");
 }
 if (yyCode != TCL_OK) {
  ErrorMessageI (xxTclTkError, xxError, NoPosition, xxString, Tcl_GetStringResult (yyInterp));
  goto yyReturn;
 }
 yyDrawTree (yyt, NULL);
 Tk_MainLoop ();
yyReturn: Tcl_DeleteInterp (yyInterp);
}

void ConfigureTree
# ifdef HAVE_ARGS
 (char * yyParameter, char * yyValue)
# else
 (yyParameter, yyValue) char * yyParameter; char * yyValue;
# endif
{
 if (yyParameter == NULL || yyValue == NULL) return;
        if (strcmp (yyParameter, "CheckReportNoTree") == 0) {
  Tree_CheckReportNoTree = atoi (yyValue) != 0;
 } else if (strcmp (yyParameter, "CheckReportNodes") == 0) {
  Tree_CheckReportNodes = atoi (yyValue) != 0;
 } else if (strcmp (yyParameter, "DrawBoxHeight") == 0) {
  Tree_DrawBoxHeight = atoi (yyValue);
 } else if (strcmp (yyParameter, "DrawBoxWidth") == 0) {
  Tree_DrawBoxWidth = atoi (yyValue);
 } else if (strcmp (yyParameter, "DrawDepth") == 0) {
  Tree_DrawDepth = atoi (yyValue);
 } else if (strcmp (yyParameter, "DrawLength") == 0) {
  Tree_DrawLength = atoi (yyValue);
 } else if (strcmp (yyParameter, "DrawFileName") == 0) {
  strncpy (Tree_DrawFileName, yyValue, 256);
  Tree_DrawFileName [255] = '\0';
 } else if (strcmp (yyParameter, "TreatTVAasChild") == 0) {
# ifdef SUPPORT_TVA
  Tree_TreatTVAasChild = atoi (yyValue) != 0;
# endif
 } else {
  ErrorMessageI (xxConfigureUnknownParam, xxError, NoPosition, xxString, yyParameter);
 }
}

void BeginTree ARGS ((void))
{
/* line 46 "xlang.ast" */

  /* Target code to be placed in the BeginTREE() function */

}

void CloseTree ARGS ((void))
{
/* line 50 "xlang.ast" */

  /* Target code to be placed in the CloseTREE() function */

}
