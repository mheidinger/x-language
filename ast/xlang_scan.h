# ifndef yyxlang_scan
# define yyxlang_scan

/* $Id: Scanner.h,v 2.18 2005/03/24 17:43:29 grosch rel $ */

# include "config.h"
# if HAVE_WCHAR_H
#  include <wchar.h>
# endif
# include "ratc.h"

/* line 8 "xlang.rex" */

  /* code to be put into Scanner.h */
  # include "Position.h"
  # include "StringM.h"


  void StartProgName(void);

  /* Token Attributes.
   * For each token with user defined attributes, we need a typedef for the
   * token attributes.
   * LPP extracts the token-attribute declaration from the parser specification.
   * They are inserted here.
   */
  
/* line 24 "xlang.rex" */
# ifndef CommonScanAttrs
# define CommonScanAttrs
# endif

# define zzCommonScanAttrs tPosition Position; CommonScanAttrs

typedef struct { zzCommonScanAttrs long Value; } zz_xlang_scan_int_const;
typedef struct { zzCommonScanAttrs double Value; } zz_xlang_scan_float_const;
typedef struct { zzCommonScanAttrs tStringRef Value; } zz_xlang_scan_string_const;
typedef struct { zzCommonScanAttrs tStringRef Value; } zz_xlang_scan_prog_name;
typedef struct { zzCommonScanAttrs tIdent Value; } zz_xlang_scan_identifier;
typedef struct { zzCommonScanAttrs int Dummy; } zz_xlang_scan_separator;

typedef union {
tPosition Position;
struct { zzCommonScanAttrs } Common;
zz_xlang_scan_int_const int_const;
zz_xlang_scan_float_const float_const;
zz_xlang_scan_string_const string_const;
zz_xlang_scan_prog_name prog_name;
zz_xlang_scan_identifier identifier;
zz_xlang_scan_separator separator;
} xlang_scan_tScanAttribute;

# undef zzCommonScanAttrs

extern void xlang_scan_ErrorAttribute ARGS((int Token, xlang_scan_tScanAttribute * pAttribute));
/* line 52 "xlang.rex" */


/* line 60 "xlang_scan.h" */

# define xlang_scan_EofToken	0
# define xlang_scan_xxMaxCharacter	255

# ifndef xlang_scan_xxtChar
#  if xlang_scan_xxMaxCharacter < 256
#   define xlang_scan_xxtChar	char
#  else
#   define xlang_scan_xxtChar	wchar_t
#  endif
# endif

# ifndef xlang_scan_xxtusChar
#  if xlang_scan_xxMaxCharacter < 256
#   define xlang_scan_xxtusChar	unsigned char
#  else
#   define xlang_scan_xxtusChar	wchar_t
#  endif
# endif

# ifdef lex_interface
#  define xlang_scan_GetToken	yylex
#  define xlang_scan_TokenLength	yyleng
# endif

extern xlang_scan_xxtChar *	xlang_scan_TokenPtr	;
extern int		xlang_scan_TokenLength	;
extern xlang_scan_tScanAttribute	xlang_scan_Attribute	;
extern void		(* xlang_scan_Exit) ARGS ((void));

extern void	xlang_scan_BeginScanner	ARGS ((void));
extern void	xlang_scan_BeginFile	ARGS ((char * yyFileName));
# if HAVE_WCHAR_T
extern void	xlang_scan_BeginFileW	ARGS ((wchar_t * yyFileName));
# endif
extern void	xlang_scan_BeginMemory	ARGS ((void * yyInputPtr));
extern void	xlang_scan_BeginMemoryN	ARGS ((void * yyInputPtr, int yyLength));
extern void	xlang_scan_BeginGeneric	ARGS ((void * yyInputPtr));
extern int	xlang_scan_GetToken	ARGS ((void));
extern int	xlang_scan_GetWord 	ARGS ((xlang_scan_xxtChar * yyWord));
extern int	xlang_scan_GetLower	ARGS ((xlang_scan_xxtChar * yyWord));
extern int	xlang_scan_GetUpper	ARGS ((xlang_scan_xxtChar * yyWord));
extern void	xlang_scan_CloseFile	ARGS ((void));
extern void	xlang_scan_CloseScanner	ARGS ((void));
extern void	xlang_scan_ResetScanner	ARGS ((void));

# endif
