/*                               -*- Mode: C -*- 
 * mccp.yy
 * Copyright © 2000, 2001 Laboratoire de Biologie Informatique et Théorique.
 * Author           : Martin Larose
 * Created On       : Tue Aug 22 11:18:19 2000
 * Last Modified By : Martin Larose
 * Last Modified On : Wed Jul  4 11:41:48 2001
 * Update Count     : 6
 * Status           : Ok.
 */


%option noyywrap
%option nounput
%option noyy_top_state
%option stack
%{
#include <iostream.h>
#include <vector.h>
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
\[           return TOK_LBRACKET;
\]           return TOK_RBRACKET;
<INITIAL,QUERIES>\(        return TOK_LPAREN;
<INITIAL,QUERIES>\)        return TOK_RPAREN;
=            return TOK_ASSIGN;
<INITIAL,QUERIES>\/           return TOK_FACE;
<INITIAL,QUERIES>\|\|         return TOK_OR;
<INITIAL,QUERIES>&&           return TOK_AND;
<INITIAL,QUERIES>!            return TOK_NOT;

add_pdb      return TOK_ADDPDB;
adjacency    return TOK_ADJACENCY;
align        return TOK_ALIGN;
all          return TOK_ALLATOMS;
angle        return TOK_ANGLE;
backbone_only return TOK_BBATOMS;
backtrack    return TOK_BACKTRACK;
base_only    return TOK_BASEATOMS;
cache        return TOK_CACHE;
change_id    return TOK_CHANGEID;
confo_cutoff return TOK_CONFOCUT;
connect      return TOK_CONNECT;
cycle        return TOK_CYCLE;
deviation    return TOK_RMSCST;
direction    return TOK_DIRECTION;
display_fg   return TOK_DISPLAYFG;
distance     return TOK_DISTANCE;
explore      return TOK_EXPLORE;
fileName_pdb return TOK_PDB;
fixed_distance return TOK_FIXEDDIST;
library      return TOK_LIBRARY;
new_tag      return TOK_NEWTAG;
no_hydrogen  return TOK_NOHYDRO;
note         return TOK_NOTE;
notes        return TOK_NOTES;
pair         return TOK_PAIR;
pdb          return TOK_PDB;
place        return TOK_PLACE;
prop         return TOK_PROPERTIES;
pse_only     return TOK_PSEATOMS;
quit         return TOK_QUIT;
remark       return TOK_REMARK;
res          return TOK_RES;
res_clash    return TOK_RESCLASH;
reset        return TOK_RESET;
reset_db     return TOK_RESETDB;
residue      return TOK_RESIDUE;
residue_align return TOK_RESIDUEALIGN;
restore      return TOK_RESTORE;
rmsd_bound   return TOK_RMSDBOUND;
sequence     return TOK_SEQUENCE;
source       return TOK_SOURCE;
strip        return TOK_STRIP;
tfo          return TOK_TRANSFO;
tfo_cutoff   return TOK_TFOCUT;
torsion      return TOK_TORSION;
vdw_distance return TOK_VDWDIST;
version      return TOK_VERSION;
zipped       return TOK_ZIPPED;

{INTEGER_LIT}     mcclval.intval = atoi (mcctext); return TOK_INTEGER;

{INTEGER_LIT}\.{DIGIT}*  mcclval.floatval = atof (mcctext); return TOK_FLOAT;

{LETTER}{DIGIT}+  mcclval.textval = mcccopy (mcctext); return TOK_RESNAME;

[-_<>a-zA-Z][-_<>\'%\.a-zA-Z0-9]*  {
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

<QUOTES><<EOF>>  throw CLexerException ("End Of File found in quote.");


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

<STRINGS><<EOF>>  throw CLexerException ("End Of File found in string.");


           /* Definitions for the QUERIES state.  */

\{                         { yy_push_state (QUERIES); return TOK_LBRACE; }
<QUERIES>{INTEGER_LIT}     {
                             mcclval.textval = mcccopy (mcctext);
                             return TOK_IDENT;
                           }
<QUERIES>file              return TOK_FILENAME;
<QUERIES>any               return TOK_ANY;
<QUERIES>[-_<>a-zA-Z][-_<>\'%\.a-zA-Z0-9]* {
                                             mcclval.textval = mcccopy (mcctext);
					     return TOK_IDENT;
                                           }
<QUERIES>\}                { yy_pop_state (); return TOK_RBRACE; }

.                          {
                             throw CLexerException ("Token error: invalid character ")
			       << mcctext[0] << " at line " << mcclineno
			       << '.' ;
                           } 

<QUERIES><<EOF>>  throw CLexerException ("End Of File found in queries.");

<<EOF>>           yyterminate ();

%%


char*
mcccopy (const char *str)
{
  char *tmp = new char[strlen (str) + 1];

  strcpy (tmp, str);
  return tmp;
}
