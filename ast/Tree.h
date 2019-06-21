# ifndef yyTree
# define yyTree

extern char Tree_module_does_not_match_evaluator_module_1744597;
extern char generate_Tree_module_without_option_0;

# include "ratc.h"

# include <stdio.h>
/* line 12 "xlang.ast" */

  /* The target-code of this section is copied into the generated .h file
   * before the declaration of the `typedef .... tTREE'
   */
  # include "StringM.h"


# ifndef rbool
# define rbool char
# endif
# define NoTree (tTree) 0L
# define kprogram 1
# define kDECLS 2
# define kno_decls 3
# define kdecls 4
# define kdecl 5
# define kidentifier 6
# define kINSTRS 7
# define kno_instrs 8
# define kinstrs 9
# define kINSTR 10
# define kif_instr 11
# define kassignment_instr 12
# define kwhile_instr 13
# define kfor_instr 14
# define ktest 15
# define kELSEIFS 16
# define kno_elseifs 17
# define kelseifs 18
# define kEXPR 19
# define kint_const 20
# define kfloat_const 21
# define kstring_const 22
# define kname 23
# define kbinary_expr 24
# define kunary_expr 25
# define yyBlockSize 20480

typedef unsigned char Tree_tKind;
typedef unsigned short Tree_tMark;
typedef unsigned short Tree_tLabel;
typedef union Tree_Node * tTree;
typedef tTree tprogram;
typedef tTree tDECLS;
typedef tTree tdecl;
typedef tTree tidentifier;
typedef tTree tINSTRS;
typedef tTree tINSTR;
typedef tTree ttest;
typedef tTree tELSEIFS;
typedef tTree tEXPR;
typedef void (* Tree_tProcTree) ARGS ((tTree));
typedef tTree * yytTreePtr;
typedef struct { tTree yyOld, yyNew; } yytTreeOldToNew;

typedef struct yysTree_Block {
 char yyBlock [yyBlockSize];
 struct yysTree_Block * yySuccessor;
} yytTree_Block, * yytTree_BlockPtr;

/* line 19 "xlang.ast" */

  /* The target-code of this section is copied into the generated .h file
   * after the declaration of the `typedef .... tTREE', but before
   * the declaration of the tree nodes struct's.
   */

  # include "Idents.h"
  typedef enum eIOFlag {
    NONE,
    INPUT,
    OUTPUT
  } tIOFlag;


  typedef enum eVarType {
    INT,
    FLOAT,
    STRING
  } tVarType;



# define Tree_InitHead(ptr, kind) ptr->Kind = kind; \
   ptr->yyHead.yyMark = 0; \
   beginNodeHead (ptr)
# ifndef Tree_NodeHead
# define Tree_NodeHead unsigned short yyx, yyy; tTree yyparent;
# endif
typedef struct { Tree_tKind yyKind; Tree_tMark yyMark;
 Tree_NodeHead } Tree_tNodeHead;
typedef struct { Tree_tNodeHead yyHead;
tStringRef Name; tTree Declarations; tTree Instructions; } yprogram;
typedef struct { Tree_tNodeHead yyHead;
} yDECLS;
typedef struct { Tree_tNodeHead yyHead;
} yno_decls;
typedef struct { Tree_tNodeHead yyHead;
tTree Next; tTree Decl; } ydecls;
typedef struct { Tree_tNodeHead yyHead;
tIOFlag IOFlag; tVarType Type; tTree Identifier; } ydecl;
typedef struct { Tree_tNodeHead yyHead;
tIdent Ident; } yidentifier;
typedef struct { Tree_tNodeHead yyHead;
} yINSTRS;
typedef struct { Tree_tNodeHead yyHead;
} yno_instrs;
typedef struct { Tree_tNodeHead yyHead;
tTree Next; tTree Instr; } yinstrs;
typedef struct { Tree_tNodeHead yyHead;
} yINSTR;
typedef struct { Tree_tNodeHead yyHead;
tTree Test; tTree Then; tTree Else; tTree ELSEIFS; } yif_instr;
typedef struct { Tree_tNodeHead yyHead;
tTree Lhs; tTree Rhs; } yassignment_instr;
typedef struct { Tree_tNodeHead yyHead;
tTree Test; tTree Body; } ywhile_instr;
typedef struct { Tree_tNodeHead yyHead;
tTree Assignment; tTree Test; tTree IncAssignment; tTree Body; } yfor_instr;
typedef struct { Tree_tNodeHead yyHead;
tStringRef operator; tTree Left; tTree Right; } ytest;
typedef struct { Tree_tNodeHead yyHead;
} yELSEIFS;
typedef struct { Tree_tNodeHead yyHead;
} yno_elseifs;
typedef struct { Tree_tNodeHead yyHead;
tTree Next; tTree Test; tTree Then; } yelseifs;
typedef struct { Tree_tNodeHead yyHead;
} yEXPR;
typedef struct { Tree_tNodeHead yyHead;
int Value; } yint_const;
typedef struct { Tree_tNodeHead yyHead;
float Value; } yfloat_const;
typedef struct { Tree_tNodeHead yyHead;
tStringRef Value; } ystring_const;
typedef struct { Tree_tNodeHead yyHead;
tTree Ident; } yname;
typedef struct { Tree_tNodeHead yyHead;
char Op; tTree Left; tTree Right; } ybinary_expr;
typedef struct { Tree_tNodeHead yyHead;
char Op; tTree Expr; } yunary_expr;

union Tree_Node {
 Tree_tKind Kind;
 Tree_tNodeHead yyHead;
 yprogram program;
 yDECLS DECLS;
 yno_decls no_decls;
 ydecls decls;
 ydecl decl;
 yidentifier identifier;
 yINSTRS INSTRS;
 yno_instrs no_instrs;
 yinstrs instrs;
 yINSTR INSTR;
 yif_instr if_instr;
 yassignment_instr assignment_instr;
 ywhile_instr while_instr;
 yfor_instr for_instr;
 ytest test;
 yELSEIFS ELSEIFS;
 yno_elseifs no_elseifs;
 yelseifs elseifs;
 yEXPR EXPR;
 yint_const int_const;
 yfloat_const float_const;
 ystring_const string_const;
 yname name;
 ybinary_expr binary_expr;
 yunary_expr unary_expr;
};

extern const unsigned short Tree_NodeSize [26];
extern const Tree_tKind Tree_TypeRange [26];
extern const char * const Tree_NodeName [];

extern rbool Tree_CheckReportNoTree, Tree_CheckReportNodes;
extern int Tree_DrawDepth;
extern int Tree_DrawLength;
extern int Tree_DrawBoxWidth;
extern int Tree_DrawBoxHeight;
extern char Tree_DrawFileName [];
# ifdef SUPPORT_TVA
extern rbool Tree_TreatTVAasChild;
# endif

extern tTree TreeRoot;
extern unsigned long Tree_HeapUsed;
extern yytTree_BlockPtr Tree_BlockList;
extern char * Tree_PoolFreePtr, * Tree_PoolStartPtr;

extern void (* Tree_Exit) ARGS ((void));
extern tTree Tree_Alloc ARGS ((unsigned long yySize));
extern tTree MakeTree ARGS ((Tree_tKind yyKind));
extern rbool Tree_IsType ARGS ((register tTree yyt,
 register Tree_tKind yyKind));

extern tTree mprogram
 ARGS ((tStringRef pName, tTree pDeclarations, tTree pInstructions));
extern tTree mDECLS
 ARGS ((void));
extern tTree mno_decls
 ARGS ((void));
extern tTree mdecls
 ARGS ((tTree pNext, tTree pDecl));
extern tTree mdecl
 ARGS ((tIOFlag pIOFlag, tVarType pType, tTree pIdentifier));
extern tTree midentifier
 ARGS ((tIdent pIdent));
extern tTree mINSTRS
 ARGS ((void));
extern tTree mno_instrs
 ARGS ((void));
extern tTree minstrs
 ARGS ((tTree pNext, tTree pInstr));
extern tTree mINSTR
 ARGS ((void));
extern tTree mif_instr
 ARGS ((tTree pTest, tTree pThen, tTree pElse, tTree pELSEIFS));
extern tTree massignment_instr
 ARGS ((tTree pLhs, tTree pRhs));
extern tTree mwhile_instr
 ARGS ((tTree pTest, tTree pBody));
extern tTree mfor_instr
 ARGS ((tTree pAssignment, tTree pTest, tTree pIncAssignment, tTree pBody));
extern tTree mtest
 ARGS ((tStringRef poperator, tTree pLeft, tTree pRight));
extern tTree mELSEIFS
 ARGS ((void));
extern tTree mno_elseifs
 ARGS ((void));
extern tTree melseifs
 ARGS ((tTree pNext, tTree pTest, tTree pThen));
extern tTree mEXPR
 ARGS ((void));
extern tTree mint_const
 ARGS ((int pValue));
extern tTree mfloat_const
 ARGS ((float pValue));
extern tTree mstring_const
 ARGS ((tStringRef pValue));
extern tTree mname
 ARGS ((tTree pIdent));
extern tTree mbinary_expr
 ARGS ((char pOp, tTree pLeft, tTree pRight));
extern tTree munary_expr
 ARGS ((char pOp, tTree pExpr));

extern void ReleaseTree ARGS ((tTree yyt));
extern void ReleaseTreeModule ARGS ((void));
extern void WriteTreeNode ARGS ((FILE * yyyf, tTree yyt));
extern tTree ReadTree ARGS ((FILE * yyyf));
extern void WriteTree ARGS ((FILE * yyyf, tTree yyt));
extern void WriteTreeXML ARGS ((FILE * yyyf, tTree yyt));
extern void PutTree ARGS ((FILE * yyyf, tTree yyt));
extern tTree GetTree ARGS ((FILE * yyyf));
extern void TraverseTreeTD ARGS ((tTree yyt, Tree_tProcTree yyyProc));
extern void TraverseTreeBU ARGS ((tTree yyt, Tree_tProcTree yyProc));
extern tTree ReverseTree ARGS ((tTree yyt));
extern void ForallTree ARGS ((tTree yyt, Tree_tProcTree yyProc));
extern tTree CopyTree ARGS ((tTree yyt));
extern rbool IsEqualTree ARGS ((tTree yyt1, tTree yyt2));
extern rbool CheckTree ARGS ((tTree yyt));
extern void QueryTree ARGS ((tTree yyt));
extern void DrawTree ARGS ((tTree yyt));
extern void SetDepthTree ARGS ((int yyDepth));
extern void SetBoxTree ARGS ((int yyWidth, int yyHeight));
extern void ConfigureTree ARGS ((char * yyParameter, char * yyValue));
extern void InitTree ARGS ((register tTree yyt));
extern void BeginTree ARGS ((void));
extern void CloseTree ARGS ((void));

# endif
