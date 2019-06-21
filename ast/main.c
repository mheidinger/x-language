/* Project:  XLang compiler
 * Descr:    A compiler for the X language
 * Author:   Max Heidinger <mail@max-heidinger.de>, Pascal Riesinger <mail@pascal-riesinger.de>
 */

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "Position.h"
# include "Errors.h"
# include "xlang_scan.h"
# include "xlang_pars.h"
# include "Tree.h"

int main (int argc, char *argv[])
{
   int errors = 0;
   if (argc == 2) {
     if (strcmp (argv[1], "-h") == 0) {
       fprintf (stderr,
		"usage: %s [-h] [file]\n"
		"  expression LR based parser, reads `file' or stdin\n"
		"  -h: Help\n", argv[0]);
       exit (0);
     }

     xlang_scan_Attribute.Position.FileName = MakeIdent1 (argv[1]);
     xlang_scan_BeginFile (argv[1]);
     /* Read from file argv[1].
      * If this routine is not called, stdin is read.
      */
   }

   BeginTree (); /* see expr.ast */

   StartProgName();

   errors = xlang_pars ();  /* the parser */

   printf ("parser returned: %d number of errors: %d\n",
           errors,GetCount (xxError));

   if (!CheckTree (TreeRoot)) {
     fprintf (stderr, "Der Baum ist falsch aufgebaut\n");
     exit (1);
   }

   /* ASCII output of the tree: */
   /* WriteTree (stdout, TreeRoot); */

   /* set some parameters for the Tree Browser */
   SetDepthTree (20);
   SetBoxTree   (100, 20);
   DrawTree     (TreeRoot);

   return (errors == 0)? 0 : 1;
}
