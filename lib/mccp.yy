/*                               -*- Mode: C++ -*- 
 * mccp.yy
 * Copyright © 2000-04 Laboratoire de Biologie Informatique et Théorique.
 * Author           : Martin Larose
 * Created On       : Tue Aug 22 11:18:19 2000
 * $Revision$
 * $Id$
 */


%option noyywrap
%option nounput
%option noyy_top_state
%option stack
%{
#include <iostream>
#include <vector>
#include <string.h>
  
#include "mccparser.h"
#include "y.tab.h"
  
#define YY_INPUT(buf,result,max_size) \
        if (interactive_parser) \
          { \
            int c = '*'; \
            \
            c = input_class->nextchar (); \
            if (c == EOF) \
              result = 0; \
            else \
              { \
                buf[0] = (char) c; \
                result = 1; \
              } \
          } \
	else if ( ((result = fread( buf, 1, max_size, yyin )) == 0) \
		  && ferror( yyin ) ) \
		YY_FATAL_ERROR( "input in flex scanner failed" );

using namespace std;


char* mcccopy (const char *str);
int mcclineno = 0;
%}


DIGIT        [0-9]
LETTER       [a-zA-Z]
INTEGER_LIT  (-?{DIGIT}+)

%x STRINGS QUOTES QUERIES

%%
             char mccstring_buf[4096];
             char *mccstring_buf_ptr = mccstring_buf;


<INITIAL,QUERIES>[\n\r]    ++mcclineno;
<INITIAL,QUERIES>[ \t]+       
<INITIAL,QUERIES>\/\/.*
,            return TOK_COMMA;
:            return TOK_COLON;
-            return TOK_DASH;
\[           return TOK_LBRACKET;
\]           return TOK_RBRACKET;
\%           return TOK_PERCENT;
<INITIAL,QUERIES>\(        return TOK_LPAREN;
<INITIAL,QUERIES>\)        return TOK_RPAREN;
=            return TOK_ASSIGN;
<INITIAL,QUERIES>\/           return TOK_FACE;
<INITIAL,QUERIES>\|\|         return TOK_OR;
<INITIAL,QUERIES>&&           return TOK_AND;
<INITIAL,QUERIES>!            return TOK_NOT;

add_pdb          return TOK_ADDPDB;
base_adjacency   return TOK_BASEADJACENCY;
ribose_adjacency return TOK_RIBOADJACENCY;
align            return TOK_ALIGN;
angle            return TOK_ANGLE;
angstroms        return TOK_ANGSTROMS;
Angstroms        return TOK_ANGSTROMS;
angstrom         return TOK_ANGSTROMS;
Angstrom         return TOK_ANGSTROMS;
as_is            return TOK_ASIS;
backtrack        return TOK_BACKTRACK;
cache            return TOK_CACHE;
change_id        return TOK_CHANGEID;
cutoff           return TOK_CUTOFF;
cycle            return TOK_CYCLE;
display_fg       return TOK_DISPLAYFG;
distance         return TOK_DISTANCE;
env              return TOK_ENV;
explore          return TOK_EXPLORE;
file_bin         return TOK_FILE_BINARY;
file_pdb         return TOK_FILE_PDB;
file_rnaml       return TOK_FILE_RNAML;
fixed_distance   return TOK_FIXEDDIST;
library          return TOK_LIBRARY;
multimer         return TOK_MULTIMER;
new_tag          return TOK_NEWTAG;
note             return TOK_NOTE;
notes            return TOK_NOTES;
place            return TOK_PLACE;
quit             return TOK_QUIT;
relation         return TOK_RELATION;
relations        return TOK_RELATIONS;
remark           return TOK_REMARK;
clash            return TOK_CLASH;
reset            return TOK_RESET;
reset_db         return TOK_RESETDB;
restore          return TOK_RESTORE;
rmsd             return TOK_RMSD;
sequence         return TOK_SEQUENCE;
socket_bin       return TOK_SOCKET_BINARY;
socket_pdb       return TOK_SOCKET_PDB;
source           return TOK_SOURCE;
strip            return TOK_STRIP;
time_limit       return TOK_TIMELIMIT;
torsion          return TOK_TORSION;
vdw_distance     return TOK_VDWDIST;
version          return TOK_VERSION;
zipped           return TOK_ZIPPED;
sec              return TOK_SEC;
seconds          return TOK_SEC;
min              return TOK_MIN;
minutes          return TOK_MIN;
hr               return TOK_HR;
hours            return TOK_HR;
days             return TOK_DAY;
d                return TOK_DAY;


{INTEGER_LIT}     mcclval.intval = atoi (mcctext); return TOK_INTEGER;

{INTEGER_LIT}\.{DIGIT}*  mcclval.floatval = atof (mcctext); return TOK_FLOAT;

{LETTER}{DIGIT}+  mcclval.textval = mcccopy (mcctext); return TOK_RESNAME;

{LETTER}{DIGIT}+\-{LETTER}{DIGIT}+|{DIGIT}+\-{LETTER}{DIGIT}+|{LETTER}{DIGIT}+\-{DIGIT}+|{DIGIT}+\-{DIGIT}+ mcclval.textval = mcccopy (mcctext); return TOK_RESNAME_RANGE;

{DIGIT}{LETTER}{LETTER}?{DIGIT}[\*M]? {
                                        mcclval.textval = mcccopy (mcctext);
					return TOK_ATOM;
                                      }

[-_<>a-zA-Z][-_<>\'\.\*a-zA-Z0-9]*  {
                                       mcclval.textval = mcccopy (mcctext);
				       return TOK_IDENT;
                                     }




           /** Definition of QUOTES indentifiers.  */

\'               mccstring_buf_ptr = mccstring_buf; yy_push_state (QUOTES);
<QUOTES>\'       {
                   yy_pop_state ();
		   *mccstring_buf_ptr = '\0';
		   mcclval.textval = mcccopy (mccstring_buf);
		   return TOK_QUOTED_IDENT;
                 }

<QUOTES>[\n\r]   *mccstring_buf_ptr++ = '\n'; mcclineno++;

<QUOTES>[^\']+   {
                   char *yptr = mcctext;

		   while (*yptr)
		     *mccstring_buf_ptr++ = *yptr++;
                 }

<QUOTES><<EOF>>  throw LexerException ("End Of File found in quote.");


           /** Definition of STRINGS. */

<INITIAL,QUERIES>\"   mccstring_buf_ptr = mccstring_buf; yy_push_state (STRINGS);

<STRINGS>\"      {
                   yy_pop_state ();
		   *mccstring_buf_ptr = '\0';
		   mcclval.textval = mcccopy (mccstring_buf);
		   return TOK_STRING;
                 }

<STRINGS>[\n\r]  *mccstring_buf_ptr++ = '\n'; mcclineno++;
                   
<STRINGS>\\.     *mccstring_buf_ptr++ = '\\'; *mccstring_buf_ptr++ = mcctext[1];

<STRINGS>[^\\\"]+     {
                        char *yptr = mcctext;

		        while (*yptr)
			  *mccstring_buf_ptr++ = *yptr++;
                      }

<STRINGS><<EOF>>  throw LexerException ("End Of File found in string.");


           /* Definitions for the QUERIES state.  */

\{                         { yy_push_state (QUERIES); return TOK_LBRACE; }
<QUERIES>{INTEGER_LIT}     {
                             mcclval.textval = mcccopy (mcctext);
                             return TOK_IDENT;
                           }
<QUERIES>file              return TOK_FILENAME;
<QUERIES>any               return TOK_ANY;
<QUERIES>[-_<>a-zA-Z][-_<>\'\.a-zA-Z0-9]* {
                                             mcclval.textval = mcccopy (mcctext);
					     return TOK_IDENT;
                                           }
<QUERIES>\}                { yy_pop_state (); return TOK_RBRACE; }

.                          {
                             throw LexerException ("Token error: invalid character ")
			       << mcctext[0] << " at line " << mcclineno
			       << '.' ;
                           } 

<QUERIES><<EOF>>  throw LexerException ("End Of File found in queries.");

<<EOF>>           yyterminate ();

%%


char*
mcccopy (const char *str)
{
  char *tmp = new char[strlen (str) + 1];

  strcpy (tmp, str);
  return tmp;
}
