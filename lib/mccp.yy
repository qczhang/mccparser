/*                               -*- Mode: C -*- 
 * mccp.yy
 * Copyright � 2000 Laboratoire de Biologie Informatique et Th�orique.
 * Author           : Martin Larose
 * Created On       : Tue Aug 22 11:18:19 2000
 * Last Modified By : Martin Larose
 * Last Modified On : Thu Aug 31 13:07:24 2000
 * Update Count     : 1
 * Status           : Ok.
 */


%option stack
%{
  #include <iostream.h>
  #include <vector.h>
  #include <string.h>

  #include "mccparser.h"
  #include "y.tab.h"

  char* copy (const char *str);
  int yylineno = 0;
%}


DIGIT        [0-9]
LETTER       [a-zA-Z]
INTEGER_LIT  (-?{DIGIT}+)

%x STRINGS QUOTES QUERIES

%%
             char string_buf[4096];
             char *string_buf_ptr = string_buf;


<INITIAL,QUERIES>\n        ++yylineno;           
<INITIAL,QUERIES>[ \t]+       
<INITIAL,QUERIES>\/\/.*
,            return TOK_COMMA;
:            return TOK_COLON;
\[           return TOK_LBRACKET;
\]           return TOK_RBRACKET;
<INITIAL,QUERIES>\(        return TOK_LPAREN;
<INITIAL,QUERIES>\)        return TOK_RPAREN;
=            return TOK_ASSIGN;
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
rmsd         return TOK_RMSDCST;
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

{INTEGER_LIT}     yylval.intval = atoi (yytext); return TOK_INTEGER;

{INTEGER_LIT}\.{DIGIT}*  yylval.floatval = atof (yytext); return TOK_FLOAT;

{LETTER}{DIGIT}+  yylval.textval = copy (yytext); return TOK_RESNAME;

[-_<>a-zA-Z][-_<>'%\.a-zA-Z0-9]*  {
                                   yylval.textval = copy (yytext);
				   return TOK_IDENT;
                                 }


           /** Definition of QUOTES indentifiers.  */

\'               string_buf_ptr = string_buf; yy_push_state (QUOTES);
<QUOTES>\'       {
                   yy_pop_state ();
		   *string_buf_ptr = '\0';
		   yylval.textval = copy (string_buf);
		   return TOK_QUOTED_IDENT;
                 }

<QUOTES>[^\']+   {
                   char *yptr = yytext;

		   while (*yptr)
		     *string_buf_ptr++ = *yptr++;
                 }


                /** Definition of STRINGS. */

<INITIAL,QUERIES>\"   string_buf_ptr = string_buf; yy_push_state (STRINGS);

<STRINGS>\"      {
                   yy_pop_state ();
		   *string_buf_ptr = '\0';
		   yylval.textval = copy (string_buf);
		   return TOK_STRING;
                 }

<STRINGS>\n      *string_buf_ptr++ = '\n'; yylineno++;
                   
<STRINGS>\\.     *string_buf_ptr++ = '\\'; *string_buf_ptr++ = yytext[1];

<STRINGS>[^\\\"]+ {
                        char *yptr = yytext;

		        while (*yptr)
			  *string_buf_ptr++ = *yptr++;
                    }


         /* Definitions for the QUERIES state.  */

\{                         { yy_push_state (QUERIES); return TOK_LBRACE; }
<QUERIES>{INTEGER_LIT}     {
                             yylval.textval = copy (yytext);
                             return TOK_IDENT;
                           }
<QUERIES>file              return TOK_FILENAME;
<QUERIES>[-_<>a-zA-Z][-_<>'%\.a-zA-Z0-9]*  {
                                             yylval.textval = copy (yytext);
					     return TOK_IDENT;
                                           }
<QUERIES>\}                { yy_pop_state (); return TOK_RBRACE; }

.                          { throw CLexerException ("Token error: invalid character ")
			       << yytext[0] << " at line " << yylineno
			       << '.' ;
                           } 
%%


char*
copy (const char *str)
{
  char *tmp = new char[strlen (str) + 1];

  strcpy (tmp, str);
  return tmp;
}