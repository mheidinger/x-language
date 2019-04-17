/* Project:  COCKTAIL training
 * Descr:    a simple scanner generated with rex
 * Kind:     REX scanner specification (solution)
 * Author:   Max Heidinger <mail@max-heidinger.de>, Pascal Riesinger <mail@pascal-riesinger.de>
 */

SCANNER l_scan

EXPORT {
/* code to be put into Scanner.h */

# include "Position.h"
# include "StringM.h"

/* Token Attributes.
 * For each token with user defined attributes, we need a typedef for the
 * token attributes.
 * The first struct-field must be of type tPosition!
 */
typedef struct {tPosition Pos; long Value;} tint_const;
typedef struct {tPosition Pos; tStringRef Value;} tfloat_const;
typedef struct {tPosition Pos; tStringRef Value;} tstring_const;
typedef struct {tPosition Pos; tIdent Identifier;} tidentifier;

/* There is only one "actual" token, during scanning. Therfore
 * we use a UNION of all token-attributes as data type for that unique
 * token-attribute variable.
 * All token (with and without user defined attributes) have one
 * attribute: the source position:
 *     tPosition     Position;
 */
typedef union {
  tPosition     Position;
  tint_const    int_const;
  tfloat_const  float_const;
  tstring_const string_const;
  tidentifier   identifier;
} l_scan_tScanAttribute;

/* Tokens are coded as int's, with values >=0
 * The value 0 is reserved for the EofToken, which is defined automatically
 */
# define tok_int_const    1
# define tok_float_const  2
# define tok_string_const 3
# define tok_ident        4
# define tok_keyword_if   5
# define tok_keyword_else 6
} // EXPORT

GLOBAL {
  # include <stdlib.h>
  # include "rString.h"
} // GLOBAL

LOCAL {
  # define MAX_STRING_LENGTH 1024
  tPosition str_pos;
  char string[MAX_STRING_LENGTH];
  int str_len;
}  // LOCAL

DEFAULT {
  /* What happens if no scanner rule matches the input */
  MessageI ("Illegal character", xxError, l_scan_Attribute.Position, xxCharacter, l_scan_TokenPtr);
} // DEFAULT

EOF {
  /* What should be done if the end-of-input-file has been reached? */

  /* E.g.: check hat strings and comments are closed. */
  switch (yyStartState) {
  case STD:
    /* ok */
    break;
  default:
    Message ("OOPS: that should not happen!!", xxFatal, l_scan_Attribute.Position);
    break;
  }

  /* implicit: return the EofToken */
} // EOF

DEFINE  /* some abbreviations */
  digit  = {0-9}.
  letter = {A-Za-z}.
  character = (digit | letter).

/* define start states, note STD is defined by default, separate several states by a comma */
START STRING

RULE

/* Integers */
#STD# digit+ : {
  l_scan_Attribute.int_const.Value = atol (l_scan_TokenPtr);
  return tok_int_const;
}
/* Float numbers */
#STD# digit+ "." digit+ : {
  l_scan_Attribute.float_const.Value = PutString(l_scan_TokenPtr, l_scan_TokenLength);
  return tok_float_const;
}

/* String literals */

/* Start of string literal */
#STD# \" : {
  yyStart(STRING);
  str_len = 0; /* reset the string length counter */
  str_pos = l_scan_Attribute.Position;
}

/* End of string literal */
#STRING# \" : {
  yyStart(STD);
  l_scan_Attribute.string_const.Value = PutString(string, str_len);
  l_scan_Attribute.string_const.Pos = str_pos;
  return tok_string_const;
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
  str_len = l_scan_GetWord(&string[str_len]);
}

#STRING# \n : {
  Message("Line break in string literal", xxError, l_scan_Attribute.Position);
}


/* Keywords */
#STD# "if" : {
  return tok_keyword_if;
}

#STD# "else" : {
  return tok_keyword_else;
}

/* Line comments */
#STD# "//" ANY* :- {}

/* TODO: C-style comment */


/* Identifier */

#STD# letter (letter | digit)* : {
  l_scan_Attribute.identifier.Identifier = MakeIdent(l_scan_TokenPtr, l_scan_TokenLength);
  return tok_ident;
}

/**********************************************************************/
/* vim: set sw=2 :*/
