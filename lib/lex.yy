/*                               -*- Mode: C -*- 
 * tokens.l
 * Copyright © 2000 Laboratoire de Biologie Informatique et Théorique.
 * Author           : Martin Larose
 * Created On       : Tue Aug 22 10:33:44 2000
 * Last Modified By : 
 * Last Modified On : 
 * Update Count     : 0
 * Status           : Unknown.
 */


%{
  #include <stdlib.h>
  
  #include "y.tab.h"
%}

DIGIT        [0-9]
LETTER       [a-zA-Z]
INTEGER_LIT  (-?{DIGIT}+)
FLOAT_LIT    ({INTEGER_LIT}"."{DIGIT})

%%

\n           
[ \t]+       
\/\/.*
\{           return TOK_LBRACE;
\}           return TOK_RBRACE;
,            return TOK_COMMA;
:            return TOK_COLON;
\[           return TOK_LBRACKET;
\]           return TOK_RBRACKET;
\(           return TOK_LPAREN;
\)           return TOK_RPAREN;
=            return TOK_ASSIGN;
\|\|         return TOK_OR;
&&           return TOK_AND;
!            return TOK_NOT;
\"[^\"]*\"   return TOK_STRING;

add_pdb      return TOK_ADD_PDB;
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
file         return TOK_FILENAME;
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
res          return TOK_RESIDUE;
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

{INTEGER_LIT}                                yylval = atoi (yytext);
                                             return TOK_INTEGER;
{FLOAT_LIT}                                  yylval = atof (yytext);
                                             return TOK_FLOAT;
{LETTER}{DIGIT}+                             yylval = yytext;
                                             return TOK_RESNAME;
[-_<>a-zA-Z][-_<>%\.a-zA-Z0-9]*              yylval = yytext;
                                             return TOK_IDENT;
\'[-_\ \t<>a-zA-Z][-_\ \t<>%\.a-zA-Z0-9]*\'  yylval = yytext;
                                             return TOK_IDENT;
{LETTER}({LETTER}|{DIGIT})*\'?               yylval = yytext;
                                             return TOK_ATOM;
