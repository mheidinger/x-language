/* Project:  XLang
 * Descr:    A simple scanner generated with rex
 * Kind:     C-main function
 * Author:   Max Heidinger <mail@max-heidinger.de>, Pascal Riesinger <mail@pascal-riesinger.de>
 */

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "Position.h"
# include "l_scan.h"

int main (int argc, char *argv[])
{
   int Token, Count = 0;
   if (argc == 2) {
     if (strcmp (argv[1], "-h") == 0) {
       fprintf (stderr,
		"usage: scan [-h] [file]\n"
		"  simple scanner, reads `file' or stdin\n"
		"  -h: Help\n");
       exit (0);
     }
     l_scan_Attribute.Position.FileName = MakeIdent1 (argv[1]);
     l_scan_BeginFile (argv[1]);
     /* Read from file argv[1].
      * If this routine is not called, stdin is read.
      */
   }
   for (Token =  l_scan_GetToken ();
	Token != l_scan_EofToken;
	Token = l_scan_GetToken ()) {
      Count ++;
      WritePosition (stdout, l_scan_Attribute.Position); printf (" ");
      switch (Token) {
      case tok_int_const:
	printf("int_const     : %ld\n",l_scan_Attribute.int_const.Value);
	break;
      case tok_float_const:
	printf("float_const   : %s\n", StGetCStr(l_scan_Attribute.float_const.Value));
	break;
      case tok_string_const:
	printf("string_const  : %s\n", StGetCStr(l_scan_Attribute.string_const.Value));
	break;
      case tok_ident:
	printf("identifier    : %s\n", GetCStr(l_scan_Attribute.identifier.Identifier));
	break;
      case tok_keyword_if:
	printf("keyword_if\n");
	break;
      case tok_keyword_else:
	printf("keyword_else\n");
	break;
      case tok_keyword_elseif:
	printf("keyword_elseif\n");
	break;
      case tok_keyword_while:
	printf("keyword_while\n");
	break;
      case tok_keyword_for:
	printf("keyword_for\n");
	break;
      case tok_op_add:
	printf("op_add\n");
	break;
      default: fprintf(stderr, "FATAL ERROR, unknown token\n");
      }
   }
   printf ("Token count: %d\n", Count);
   return 0;
}

/* vim: set sw=2 :*/
