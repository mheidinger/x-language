# ifndef yyxlang_pars
# define yyxlang_pars

/* $Id: Parser.h,v 1.8 2002/09/25 14:26:49 grosch rel $ */

# include "ratc.h"

/* line 2 "xlang_pars.lrk" */


/* line 12 "xlang_pars.h" */
/* line 5 "xlang_pars.lrk" */


/* line 16 "xlang_pars.h" */

# ifdef yacc_interface
# define xlang_pars			yyparse
# define yyInitStackSize	YYMAXDEPTH
# endif
# define rbool		char
# define rtrue		1
# define rfalse		0
					/* named constants for start symbols */
					/* named constants for tokens */

extern	rbool	xlang_pars_Debug;
extern	char *	xlang_pars_TokenName	[];
extern	void	(* xlang_pars_Exit)	ARGS ((void));

extern	void	Beginxlang_pars	ARGS ((void));
extern	int	xlang_pars	ARGS ((void));
extern	int	xlang_pars2	ARGS ((int yyStartSymbol));
extern	void	Resetxlang_pars	ARGS ((void));
extern	void	Closexlang_pars	ARGS ((void));

# endif
