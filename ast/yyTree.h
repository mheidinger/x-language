# ifndef beginint
# define beginint(a)		
# endif
# ifndef closeint
# define closeint(a)		
# endif
# ifndef readint
# define readint(a)		fscanf (yyf, "%d", & a);
# endif
# ifndef writeint
# define writeint(a)		fprintf (yyf, "%d", a);
# endif
# ifndef getint
# define getint(a)		yyGet ((char *) & a, sizeof (a));
# endif
# ifndef putint
# define putint(a)		yyPut ((char *) & a, sizeof (a));
# endif
# ifndef copyint
# define copyint(a, b)		
# endif
# ifndef equalint
# define equalint(a, b)		(a) == (b)
# endif
# ifndef writeXMLint
# define writeXMLint(a)		fprintf (yyf, "%d", a);
# endif
# ifndef beginfloat
# define beginfloat(a)		
# endif
# ifndef closefloat
# define closefloat(a)		
# endif
# ifndef readfloat
# define readfloat(a)		fscanf (yyf, "%g", & a);
# endif
# ifndef writefloat
# define writefloat(a)		fprintf (yyf, "%g", a);
# endif
# ifndef getfloat
# define getfloat(a)		yyGet ((char *) & a, sizeof (a));
# endif
# ifndef putfloat
# define putfloat(a)		yyPut ((char *) & a, sizeof (a));
# endif
# ifndef copyfloat
# define copyfloat(a, b)	
# endif
# ifndef equalfloat
# define equalfloat(a, b)	(a) == (b)
# endif
# ifndef writeXMLfloat
# define writeXMLfloat(a)	fprintf (yyf, "%g", a);
# endif
# ifndef beginrbool
# define beginrbool(a)		
# endif
# ifndef closerbool
# define closerbool(a)		
# endif
# ifndef readrbool
# define readrbool(a)		a = fgetc (yyf) == 'T';
# endif
# ifndef writerbool
# define writerbool(a)		fputc (a ? 'T' : 'F', yyf);
# endif
# ifndef getrbool
# define getrbool(a)		yyGet ((char *) & a, sizeof (a));
# endif
# ifndef putrbool
# define putrbool(a)		yyPut ((char *) & a, sizeof (a));
# endif
# ifndef copyrbool
# define copyrbool(a, b)	
# endif
# ifndef equalrbool
# define equalrbool(a, b)	(a) == (b)
# endif
# ifndef writeXMLrbool
# define writeXMLrbool(a)	fputc (a ? 'T' : 'F', yyf);
# endif
# ifndef beginchar
# define beginchar(a)		
# endif
# ifndef closechar
# define closechar(a)		
# endif
# ifndef readchar
# define readchar(a)		a = fgetc (yyf);
# endif
# ifndef writechar
# define writechar(a)		fputc (a, yyf);
# endif
# ifndef getchar
# define getchar(a)		yyGet ((char *) & a, sizeof (a));
# endif
# ifndef putchar
# define putchar(a)		yyPut ((char *) & a, sizeof (a));
# endif
# ifndef copychar
# define copychar(a, b)		
# endif
# ifndef equalchar
# define equalchar(a, b)	(a) == (b)
# endif
# ifndef writeXMLchar
# define writeXMLchar(a)	yyWriteCharXML (a);
# endif
# ifndef begintStringRef
# define begintStringRef(a)	a = NoString;
# endif
# ifndef closetStringRef
# define closetStringRef(a)	
# endif
# ifndef readtStringRef
# define readtStringRef(a)	{ char yys [1024]; fgets (yys, 1024, yyf); ungetc ('\n', yyf); a = PutString (yys, strlen (yys) - 1); }
# endif
# ifndef writetStringRef
# define writetStringRef(a)	WriteString (yyf, a);
# endif
# ifndef gettStringRef
# define gettStringRef(a)	{ char yys [1024]; fgets (yys, 1024, yyf); a = PutString (yys, strlen (yys) - 1); }
# endif
# ifndef puttStringRef
# define puttStringRef(a)	{ WriteString (yyf, a); fputc ('\n', yyf); }
# endif
# ifndef copytStringRef
# define copytStringRef(a, b)	
# endif
# ifndef equaltStringRef
# define equaltStringRef(a, b)	(a) == (b)
# endif
# ifndef writeXMLtStringRef
# define writeXMLtStringRef(a)	yyWriteStringXML (StGetCStr (a));
# endif
# ifndef begintIdent
# define begintIdent(a)		a = NoIdent;
# endif
# ifndef closetIdent
# define closetIdent(a)		
# endif
# ifndef readtIdent
# define readtIdent(a)		a = yyReadIdent ();
# endif
# ifndef writetIdent
# define writetIdent(a)		WriteIdent (yyf, a);
# endif
# ifndef gettIdent
# define gettIdent(a)		yyGetIdent (& a);
# endif
# ifndef puttIdent
# define puttIdent(a)		yyPutIdent (a);
# endif
# ifndef copytIdent
# define copytIdent(a, b)	
# endif
# ifndef equaltIdent
# define equaltIdent(a, b)	(a) == (b)
# endif
# ifndef writeXMLtIdent
# define writeXMLtIdent(a)	yyWriteStringXML (GetCStr (a));
# endif
# ifndef beginNodeHead
# define beginNodeHead(a)	
# endif
# ifndef closeNodeHead
# define closeNodeHead(a)	
# endif
# ifndef readNodeHead
# define readNodeHead(a)	
# endif
# ifndef writeNodeHead
# define writeNodeHead(a)	
# endif
# ifndef getNodeHead
# define getNodeHead(a)		
# endif
# ifndef putNodeHead
# define putNodeHead(a)		
# endif
# ifndef copyNodeHead
# define copyNodeHead(a, b)	
# endif
# ifndef equalNodeHead
# define equalNodeHead(a, b)	rtrue
# endif
# ifndef writeXMLNodeHead
# define writeXMLNodeHead(a)	
# endif
# ifndef begintIOFlag
# define begintIOFlag(a)
# endif
# ifndef closetIOFlag
# define closetIOFlag(a)
# endif
# ifndef readtIOFlag
# define readtIOFlag(a)	\
  yyReadHex ((unsigned char *) & a, sizeof (a));
# endif
# ifndef writetIOFlag
# define writetIOFlag(a)	\
  yyWriteHex ((unsigned char *) & a, sizeof (a));
# endif
# ifndef gettIOFlag
# define gettIOFlag(a)	yyGet ((char *) & a, sizeof (a));
# endif
# ifndef puttIOFlag
# define puttIOFlag(a)	yyPut ((char *) & a, sizeof (a));
# endif
# ifndef copytIOFlag
# define copytIOFlag(a, b)
# endif
# ifndef equaltIOFlag
# define equaltIOFlag(a, b)	\
  memcmp ((char *) & a, (char *) & b, sizeof (a)) == 0
# endif
# ifndef writeXMLtIOFlag
# define writeXMLtIOFlag(a)	\
  yyWriteHex ((unsigned char *) & a, sizeof (a));
# endif
# ifndef begintVarType
# define begintVarType(a)
# endif
# ifndef closetVarType
# define closetVarType(a)
# endif
# ifndef readtVarType
# define readtVarType(a)	\
  yyReadHex ((unsigned char *) & a, sizeof (a));
# endif
# ifndef writetVarType
# define writetVarType(a)	\
  yyWriteHex ((unsigned char *) & a, sizeof (a));
# endif
# ifndef gettVarType
# define gettVarType(a)	yyGet ((char *) & a, sizeof (a));
# endif
# ifndef puttVarType
# define puttVarType(a)	yyPut ((char *) & a, sizeof (a));
# endif
# ifndef copytVarType
# define copytVarType(a, b)
# endif
# ifndef equaltVarType
# define equaltVarType(a, b)	\
  memcmp ((char *) & a, (char *) & b, sizeof (a)) == 0
# endif
# ifndef writeXMLtVarType
# define writeXMLtVarType(a)	\
  yyWriteHex ((unsigned char *) & a, sizeof (a));
# endif
# ifndef begintTree
# define begintTree(a)	a = NoTree;
# endif
# ifndef closetTree
# define closetTree(a)
# endif
# ifndef readtTree
# define readtTree(a)	fscanf (yyf, "%p +", (void * *) & a);
# endif
# ifndef writetTree
# define writetTree(a)	fprintf (yyf, "%p +", (void *) a);
# endif
# ifndef gettTree
# define gettTree(a)	yyGet ((char *) & a, sizeof (a));
# endif
# ifndef puttTree
# define puttTree(a)	yyPut ((char *) & a, sizeof (a));
# endif
# ifndef copytTree
# define copytTree(a, b)	a = b;
# endif
# ifndef equaltTree
# define equaltTree(a, b)	a == b
# endif
# ifndef writeXMLtTree
# define writeXMLtTree(a)	yyWriteAddrXML ((void *) a);
# endif
# ifndef readSELECTOR
# define readSELECTOR()	yySkip ();
# endif
# ifndef writeSELECTOR
# define writeSELECTOR(a)	yyIndentSelector (a);
# endif
