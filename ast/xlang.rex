#@ line 1
/* Project:  XLang
 * Descr:    Scanner for XLang
 * Author:   Max Heidinger <mail@max-heidinger.de>, Pascal Riesinger <mail@pascal-riesinger.de>
 */ 
SCANNER xlang_scan

EXPORT {
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
  
#@ line 24
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
#@ line 21

}

GLOBAL {
  /* code to be put into Scanner.c */
  # include <stdlib.h>
  # include "Errors.h"

  void StartProgName(void) { yyStart(PROG_NAME); }

  /* Insert the routine computing "error-values" of attributes, in case the
   * parser decides during error repair to insert a token.
   */
  
#@ line 67
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
#@ line 34

}

LOCAL {
  /* user-defined local variables of the generated GetToken routine */
  # define MAX_STRING_LEN 1024
  char string[MAX_STRING_LEN];
  int str_len = 0;
  tPosition str_pos;
}

DEFAULT {
  /* What happens if no scanner rule matches the input */
  MessageI ("Illegal character",
	    xxError, xlang_scan_Attribute.Position,
	    xxCharacter, xlang_scan_TokenPtr);
}

EOF {
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
}

DEFINE /* some abbreviations */
  digit  = {0-9}.
  letter = {A-Za-z}.
  character = (digit | letter).

/* define start states, note STD is defined by default */
START COMMENT, STRING, PROG_NAME

RULES

#PROG_NAME# -{\n}* : {
  str_len = xlang_scan_GetWord(&string[str_len]);
}

#PROG_NAME# \n : {
  yyEol(0);
  yyStart(STD);
  xlang_scan_Attribute.prog_name.Value = PutString(string, str_len);
  return 4;
}

/* Separators */
#STD# \=\= (\=)+ : {
  return 6;
}

/* Integers */
#STD# digit+ : {
  xlang_scan_GetWord (string);
  xlang_scan_Attribute.int_const.Value = atol (string);
  return 1;
}

/* Float numbers */
#STD# digit + "." digit * : {
  xlang_scan_GetWord (string);
  xlang_scan_Attribute.float_const.Value = atof (string);
  return 2;
}

/* Start of string literal */
#STD# \" : {
  yyStart(STRING);
  str_len = 0; /* reset the string length counter */
  str_pos = xlang_scan_Attribute.Position;
}

/* End of string literal */
#STRING# \" : {
  yyStart(STD);
  xlang_scan_Attribute.string_const.Value = PutString(string, str_len);
  xlang_scan_Attribute.string_const.Position = str_pos;
  return 3;
}

#STRING# \\ : {
  string[str_len++] = '\\';
}

#STRING# \\ "n" : {
  string[str_len++] = '\n';
}

#STRING# \\ \" : {
  string[str_len++] = '"';
}

#STRING# -{\\\n"}* : {
  str_len = xlang_scan_GetWord(&string[str_len]);
}

#STRING# \n : {
  Message("Line break in string literal", xxError, xlang_scan_Attribute.Position);
}

/* Line comments */
#STD# "//" ANY* :- {}

/* C-style comment */
#STD# "/*" :- {
  yyStart(COMMENT);
}

#COMMENT# "*/" :- {
  yyStart(STD);
}

#COMMENT# "/" | "*" | -{*/\n\t}+ :- {}



#@ line 236
 #STD#\-\>	: { return 7; }
 #STD#\<\-	: { return 8; }
 #STD#\i"n""t"	: { return 9; }
 #STD#"f"\l\o"a""t"	: { return 10; }
 #STD#\s"t""r"\i"n"\g	: { return 11; }
 #STD#\;	: { return 12; }
 #STD#\=	: { return 13; }
 #STD#\+	: { return 14; }
 #STD#\-	: { return 15; }
 #STD#\*	: { return 16; }
 #STD#\/	: { return 17; }
 #STD#\%	: { return 18; }
 #STD#\^	: { return 19; }
 #STD#\(	: { return 20; }
 #STD#\)	: { return 21; }
 #STD#\i"f"	: { return 22; }
 #STD#\<	: { return 23; }
 #STD#\>	: { return 24; }
 #STD#\<\=	: { return 25; }
 #STD#\>\=	: { return 26; }
 #STD#\=\=	: { return 27; }
 #STD#\!\=	: { return 28; }
 #STD#\{	: { return 29; }
 #STD#\}	: { return 30; }
 #STD#else	: { return 31; }
 #STD#\e\l\s\e\ \i"f"	: { return 32; }
 #STD#\w\h\i\l\e	: { return 33; }
 #STD#"f"\o"r"	: { return 34; }
#@ line 172

#STD# (letter | _) (digit | letter | _)* : {
  xlang_scan_Attribute.identifier.Value = MakeIdent(xlang_scan_TokenPtr, xlang_scan_TokenLength);
  return 5;
}

/**********************************************************************/

/* vim: set sw=2 :*/
