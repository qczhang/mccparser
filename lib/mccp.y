/*                               -*- Mode: C++ -*- 
 * mccp.y
 * Copyright © 2000-04 Laboratoire de Biologie Informatique et Théorique.
 * Author           : Martin Larose
 * Created On       : Tue Aug 22 11:43:17 2000
 * $Revision$
 * $Id$
 */


%{
  #include <vector>
  #include <utility>
  #include <sstream>
  #include <stdlib.h>
  #include <string.h>
  #include <math.h>

  #include "mccparser.h"

  using namespace std;

  extern int mcclex ();
  extern int mcclineno;
  int mccerror (const char *s);
%}


%union {
  char charval;
  int intval;
  bool boolval;
  float floatval;
  char *textval;
  MccPStruct *mccval;
  vector< MccPStruct* > *vmccval;
  vector< MccRelationsStat::_RelationsStruc* > *rsv;
  MccRelationsStat::_RelationsStruc *rs;
  vector< MccAngleCstStat::_AngleStruc* > *asv;
  MccAngleCstStat::_AngleStruc *as;
  vector< MccMultimerCstStat::_MultimerStruc* > *cysv;
  MccMultimerCstStat::_MultimerStruc *cys;
  vector< MccDistCstStat::_DistStruc* > *dsv;
  MccDistCstStat::_DistStruc *ds;
  vector< MccTorsionCstStat::_TorsionStruc* > *tsv;
  MccRelationCstStat::_RelationStruc *rels;
  vector< MccRelationCstStat::_RelationStruc* > *relsv;
  MccTorsionCstStat::_TorsionStruc *ts;
  vector< MccAddPdbStat::_AddPdbStruc* > *addsv;
  MccAddPdbStat::_AddPdbStruc *adds;
  vector< char* > *textv;
  vector< MccQueryExpr* > *qsv;
  MccQueryExpr *qs;
  MccQFunc *qfunc;
  MccASFunc *asfunc;
  vector< MccExploreStat::_ParamStruc* > *expsv;
  MccExploreStat::_ParamStruc *exps;
  vector< MccBacktrackExpr::_GenBTStruc* > *btsv;
  MccBacktrackExpr::_GenBTStruc *bts;
  vector< MccLibraryExpr::_LibStruc* > *lsv;
  MccLibraryExpr::_LibStruc *ls;
  vector< MccResidueName* > *rrv;
  MccResidueName *rr;
  MccFragGenStruc *fgr;
  MccFGExp *fgs;
  MccOutputMode *expo;
  MccInputMode *inmo;
  MccSamplingSize *smpls;
  MccModelFilterStrategy *mf;
}


%token TOK_LBRACE
%token TOK_RBRACE
%token TOK_COMMA
%token TOK_COLON
%token TOK_DASH
%token TOK_LBRACKET
%token TOK_RBRACKET
%token TOK_PERCENT
%token TOK_LPAREN
%token TOK_RPAREN
%token TOK_ASSIGN
%token TOK_OR
%token TOK_AND
%token TOK_NOT
%token TOK_FACE
%token TOK_ADDPDB
%token TOK_BASEADJACENCY
%token TOK_RIBOADJACENCY
%token TOK_ALIGN
%token TOK_ANGLE
%token TOK_ANGSTROMS
%token TOK_ANY
%token TOK_ASIS
%token TOK_BACKTRACK
%token TOK_CACHE
%token TOK_CHANGEID
%token TOK_CUTOFF
%token TOK_CYCLE
%token TOK_DISPLAYFG
%token TOK_DISTANCE
%token TOK_ENV
%token TOK_EXPLORE
%token TOK_FILENAME
%token TOK_FIXEDDIST
%token TOK_LIBRARY
%token TOK_MULTIMER
%token TOK_NEWTAG
%token TOK_NOTE
%token TOK_NOTES
%token TOK_FILE_BINARY
%token TOK_FILE_PDB
%token TOK_FILE_RNAML
%token TOK_PLACE
%token TOK_PSEATOMS
%token TOK_QUIT
%token TOK_RELATION
%token TOK_RELATIONS
%token TOK_REMARK
%token TOK_CLASH
%token TOK_RESET
%token TOK_RESETDB
%token TOK_RESTORE
%token TOK_RMSD
%token TOK_SEQUENCE
%token TOK_SOCKET_BINARY
%token TOK_SOCKET_PDB
%token TOK_SOURCE
%token TOK_STRIP
%token TOK_TIMELIMIT
%token TOK_TORSION
%token TOK_VDWDIST
%token TOK_VERSION
%token TOK_ZIPPED
%token TOK_SEC
%token TOK_MIN
%token TOK_HR
%token TOK_DAY
%token <intval> TOK_INTEGER
%token <floatval> TOK_FLOAT
%token <textval> TOK_RESNAME
%token <textval> TOK_RESNAME_RANGE
%token <textval> TOK_ATOM
%token <textval> TOK_IDENT
%token <textval> TOK_QUOTED_IDENT
%token <textval> TOK_STRING


%type <mccval> statement
%type <mccval> sequence
%type <mccval> assign
%type <smpls> sampling
%type <mccval> relations
%type <rsv> reldef_plus
%type <rs> reldef
%type <expsv> explore_param_star
%type <exps> explore_param
%type <mccval> explore
%type <mccval> restore
%type <expo> output_mode_opt
%type <expo> output_mode
%type <mf> model_filter_opt
%type <mf> model_filter
%type <intval>  timelimit_opt
%type <intval>  timelimit_exp
%type <intval>  timelimit_plus
%type <intval> timelimit
%type <boolval> zfile_opt
%type <mccval> source
%type <mccval> baseAdjCst
%type <mccval> riboAdjCst
%type <mccval> angleCst
%type <asv> angledef_plus
%type <as> angledef
%type <mccval> clashCst
%type <boolval> fixorvdw_opt
%type <boolval> fixorvdw
%type <mccval> multimerCst
%type <cysv> multimerdef_plus
%type <cys> multimerdef
%type <mccval> distCst
%type <dsv> distdef_plus
%type <ds> distdef
%type <mccval> torsionCst
%type <tsv> torsiondef_plus
%type <ts> torsiondef
%type <mccval> newtag
%type <mccval> addpdb
%type <addsv> addpdbdefs_plus
%type <adds> addpdbdefs
%type <floatval> cutoff_opt
%type <textv> pdbfile_plus
%type <mccval> displayfg
%type <mccval> env
%type <mccval> note
%type <mccval> notes
%type <relsv> relationdef_plus
%type <rels> relationdef
%type <mccval> relationCst
%type <mccval> remark
%type <mccval> reset
%type <mccval> resetdb
%type <mccval> version
%type <mccval> quit

%type <qsv> queryexp_plus
%type <qs> queryexp
%type <textv> filename_star
%type <textval> filename
%type <qfunc> qprop_opt
%type <qfunc> queryorexp
%type <qfunc> queryandexp
%type <qfunc> querynotexp
%type <qfunc> queryidentexp
%type <qfunc> queryfaceexp

%type <asfunc> atomset_opt
%type <asfunc> atomset_or
%type <asfunc> atomset_and
%type <asfunc> atomset_not
%type <asfunc> atomset_ident

%type <fgs> fgexp
%type <fgs> backtrackexp
%type <fgs> cycleexp
%type <btsv> res_place_plus
%type <bts> res_place
%type <fgs> cacheexp
%type <boolval> align_opt
%type <fgs> libraryexp
%type <inmo> input_mode
%type <lsv> libopt_star
%type <ls> libopt
%type <boolval> asis_opt;

%type <rrv> residueRef_star
%type <rrv> residueRef_plus
%type <rr> residueRef
%type <textv> residueRef_pair_plus
%type <textval> residueRef_pair
%type <textval> atomRef
%type <fgr> fgRef_opt
%type <fgr> fgRef

%type <textval> ident_plus
%type <floatval> flt
%expect 1

%%


statement_star:   /* empty */
                   {
		     astv = new vector< MccPStruct* > ();
		   }
                | statement_star statement
                   {
		     if (interactive_parser)
		       {
			 astv = new vector< MccPStruct* > (1, $2);
			 YYACCEPT;
		       }
		     else
		       astv->push_back ($2);
		   }
;


statement:   sequence { $$ = $1; }
           | assign { $$ = $1; }
           | relations { $$ = $1; }
           | explore { $$ = $1; }
           | restore { $$ = $1; }
           | source { $$ = $1; }
           | baseAdjCst { $$ = $1; }
           | riboAdjCst { $$ = $1; }
           | angleCst { $$ = $1; }
           | clashCst { $$ = $1; }
           | multimerCst { $$ = $1; }
           | distCst { $$ = $1; }
           | relationCst { $$ = $1; }
           | torsionCst { $$ = $1; }
           | newtag { $$ = $1; }
           | addpdb { $$ = $1; }
           | displayfg { $$ = $1; }
           | env { $$ = $1; }
           | note { $$ = $1; }
           | notes { $$ = $1; }
           | remark { $$ = $1; }
           | reset { $$ = $1; }
           | resetdb { $$ = $1; }
           | version { $$ = $1; }
           | quit { $$ = $1; }
;


sequence:  TOK_SEQUENCE TOK_LPAREN TOK_IDENT residueRef ident_plus TOK_RPAREN
            {
	      if (strlen ($3) != 1) {
		delete[] $3;
		delete[] $4;
		delete[] $5;
		throw ParserException ("Invalid sequence type.");	      
	      }
	      $$ = new MccSequenceStat ($3[0], $4, $5);
	      delete[] $3;
	    }
;


assign:    TOK_IDENT TOK_ASSIGN fgexp { $$ = new MccAssignStat ($1, $3); }
;


relations:

TOK_RELATIONS TOK_LPAREN reldef_plus TOK_RPAREN
{
  $$ = new MccRelationsStat ($3);
}
;


reldef_plus:

reldef
{
  $$ = new vector< MccRelationsStat::_RelationsStruc* > (1, $1);
}
| reldef_plus reldef
{
  $$ = $1;
  $$->push_back ($2);
}
;


reldef:

residueRef_pair_plus queryexp sampling
{
  $$ = new MccRelationsStat::_RelationsStruc ($1, $2, $3);
}
;


residueRef_pair_plus:

residueRef_pair
{
  $$ = new vector< char* > (1, $1);
}
| residueRef_pair_plus TOK_COMMA residueRef_pair
{
  $$ = $1;
  $$->push_back ($3);
}
;


residueRef_pair:

residueRef residueRef
{
  ostringstream oss;
  if (' ' != $1->id)
    oss << $1->id;
  oss << $1->no << ' ';
  if (' ' != $2->id)
    oss << $2->id;
  oss << $2->no;
  $$ = strdup (oss.str ().c_str ());
  delete $1;
  delete $2;
}
| TOK_RESNAME_RANGE
{
  $$ = $1;
}
;


explore:

TOK_EXPLORE TOK_LPAREN fgRef TOK_IDENT explore_param_star model_filter_opt timelimit_opt output_mode_opt TOK_RPAREN
{
  $$ = new MccExploreStat ($3, $4, $5, $6, $7, $8);
  delete[] $4;
}
;


explore_param_star:

/* empty */
{
  $$ = new vector< MccExploreStat::_ParamStruc* > ();
}
| explore_param_star explore_param
{
  $$ = $1;
  $$->push_back ($2);
}
;


explore_param:

TOK_STRING flt
{
  $$ = new MccExploreStat::_ParamStruc ($1, $2);
  delete[] $1;
}
;


restore:   TOK_RESTORE TOK_LPAREN TOK_STRING output_mode_opt TOK_RPAREN
            {
	      $$ = new MccRestoreStat ($3, $4);
	    }
;


model_filter_opt: /* empty */ { $$ = 0; }
                  | model_filter { $$ = $1; }
;



model_filter: TOK_RMSD TOK_LPAREN flt align_opt atomset_opt TOK_RPAREN
             {
	       $$ = new MccRmsdModelFilterStrategy ($3, $4, $5);
	     }
;



output_mode_opt:   /* empty */ { $$ = 0; }
                   | output_mode { $$ = $1; }
;



output_mode:  

TOK_FILE_PDB TOK_LPAREN TOK_STRING zfile_opt TOK_RPAREN
{
  $$ = new MccFilePdbOutput ($3, $4);
}
| TOK_FILE_BINARY TOK_LPAREN TOK_STRING zfile_opt TOK_RPAREN
{
  $$ = new MccFileBinaryOutput ($3, $4);
}
| TOK_SOCKET_BINARY TOK_LPAREN TOK_STRING TOK_INTEGER TOK_STRING TOK_RPAREN
{
  $$ = new MccSocketBinaryOutput ($3, $4, $5);
}
| TOK_SOCKET_PDB TOK_LPAREN TOK_STRING TOK_INTEGER TOK_STRING TOK_RPAREN
{
  $$ = new MccSocketPdbOutput ($3, $4, $5);
}
| TOK_FILE_RNAML TOK_LPAREN TOK_STRING zfile_opt TOK_RPAREN
{
  $$ = new MccFileRnamlOutput ($3, $4);
}
;


timelimit_opt:

/* empty */
{
  $$ = -1;
}
| timelimit_exp
{
  $$ = $1;
}
;


timelimit_exp:

TOK_TIMELIMIT TOK_LPAREN timelimit_plus TOK_RPAREN
{
  $$ = $3;
}
;


timelimit_plus:

timelimit
{ 
  $$ = $1;
}
| timelimit_plus timelimit
{
  $$ = $1 + $2;
}
;


timelimit:

TOK_INTEGER TOK_SEC
{
  $$ = $1;
}
| flt TOK_MIN
{
  $$ = (int)rint ($1 * 60.0);
}
| flt TOK_HR
{
  $$ = (int)rint ($1 * 3600.0);
}
| flt TOK_DAY
{
  $$ = (int)rint ($1 * 86400.0);
}
;


zfile_opt:   /* empty */ { $$ = false; }
           | TOK_ZIPPED { $$ = true; }
;



source:   TOK_SOURCE TOK_LPAREN TOK_STRING TOK_RPAREN
           {
	     mcclineno = 0;
	     $$ = new MccSourceStat ($3);
	   }
;


baseAdjCst:

TOK_BASEADJACENCY TOK_LPAREN fgRef TOK_INTEGER TOK_PERCENT TOK_RPAREN
{
  $$ = new MccBaseAdjacencyCstStat ($3, $4);
}
;


riboAdjCst:

TOK_RIBOADJACENCY TOK_LPAREN fgRef flt TOK_RPAREN
{
  $$ = new MccRiboseAdjacencyCstStat ($3, $4);
}
;


angleCst:   TOK_ANGLE TOK_LPAREN angledef_plus TOK_RPAREN
             {
	       $$ = new MccAngleCstStat ($3);
	     }
;


angledef_plus:   angledef
                  {
		    $$ = new vector< MccAngleCstStat::_AngleStruc* > (1, $1);
		  }
               | angledef_plus angledef
                  {
		    $$ = $1;
		    $$->push_back ($2);
		  }
;


angledef:   residueRef TOK_COLON atomRef residueRef TOK_COLON atomRef residueRef TOK_COLON atomRef flt flt
             {
	       $$ = new MccAngleCstStat::_AngleStruc ($1, $3, $4, $6, $7, $9, $10, $11);
	     }
;


clashCst:

TOK_CLASH TOK_LPAREN fgRef fixorvdw_opt flt atomset_opt TOK_RPAREN
{
  $$ = new MccClashCstStat ($3, $4, $5, $6);
}
;

 

fixorvdw_opt:   /* empty */ { $$ = false; }
              | fixorvdw { $$ = $1; }
;


fixorvdw:   TOK_FIXEDDIST { $$ = false; }
          | TOK_VDWDIST { $$ = true; }
;


atomset_opt:

/* empty */
{
  $$ = 0;
}
| atomset_or
{
  $$ = $1;
}
;


atomset_or:

atomset_and
{
  $$ = $1;
}
| atomset_or TOK_OR atomset_and
{
  $$ = new MccASOrFunc ($1, $3);
}
;


atomset_and:

atomset_not
{
  $$ = $1;
}
| atomset_and TOK_AND atomset_not
{
  $$ = new MccASAndFunc ($1, $3);
}
;


atomset_not:

atomset_ident
{
  $$ = $1;
}
| TOK_NOT atomset_not
{
  $$ = new MccASNotFunc ($2);
}
;


atomset_ident:

TOK_IDENT
{
  $$ = new MccASIdentFunc ($1);
}
| TOK_LPAREN atomset_or TOK_RPAREN
{
  $$ = $2;
}
;


multimerCst:   TOK_MULTIMER TOK_LPAREN multimerdef_plus TOK_RPAREN
             {
	       $$ = new MccMultimerCstStat ($3);
	     }
;


multimerdef_plus:   multimerdef
                  {
		    $$ = new vector< MccMultimerCstStat::_MultimerStruc* > (1, $1);
		  }
               | multimerdef_plus multimerdef
                  {
		    $$ = $1;
		    $$->push_back ($2);
		  }
;


multimerdef:   residueRef residueRef TOK_INTEGER flt
             {
	       $$ = new MccMultimerCstStat::_MultimerStruc ($1, $2, $3, (int)$4);
	     }
;


distCst:   TOK_DISTANCE TOK_LPAREN distdef_plus TOK_RPAREN
            {
	      $$ = new MccDistCstStat ($3);
	    }
;


distdef_plus:   distdef
                 {
		   $$ = new vector< MccDistCstStat::_DistStruc* > (1, $1);
		 }
              | distdef_plus distdef
                  {
		    $$ = $1;
		    $$->push_back ($2);
		  }
;


distdef:   residueRef TOK_COLON atomRef residueRef TOK_COLON atomRef flt flt
             {
	       $$ = new MccDistCstStat::_DistStruc ($1, $3, $4, $6, $7, $8);
	     }
;


relationCst:   TOK_RELATION TOK_LPAREN relationdef_plus TOK_RPAREN
               {
		 $$ = new MccRelationCstStat ($3);
	       }
;


relationdef_plus:   relationdef
                  {
		    $$ = new vector< MccRelationCstStat::_RelationStruc* > (1, $1);
		  }
               | relationdef_plus relationdef
                  {
		    $$ = $1;
		    $$->push_back ($2);
		  }
;


relationdef:   residueRef residueRef queryexp
             {
	       $$ = new MccRelationCstStat::_RelationStruc ($1, $2, $3);
	     }
;


torsionCst:   TOK_TORSION TOK_LPAREN torsiondef_plus TOK_RPAREN
             {
	       $$ = new MccTorsionCstStat ($3);
	     }
;


torsiondef_plus:   torsiondef
                    {
		      $$ = new vector< MccTorsionCstStat::_TorsionStruc* > (1, $1);
		    }
                 | torsiondef_plus torsiondef
                    {
		      $$ = $1;
		      $$->push_back ($2);
		    }
;


torsiondef:   residueRef TOK_COLON atomRef residueRef TOK_COLON atomRef residueRef TOK_COLON atomRef residueRef TOK_COLON atomRef flt flt
             {
	       $$ = new MccTorsionCstStat::_TorsionStruc ($1, $3, $4, $6, $7, $9, $10, $12, $13, $14);
	     }
;


newtag:   TOK_NEWTAG TOK_LPAREN TOK_STRING queryexp_plus TOK_RPAREN
           {
	     $$ = new MccNewTagStat ($3, $4);
	   }
;


addpdb:   TOK_ADDPDB TOK_LPAREN cutoff_opt addpdbdefs_plus TOK_RPAREN
           {
	     $$ = new MccAddPdbStat ($4, $3);
	   }
;


addpdbdefs_plus:   addpdbdefs
                    {
		      $$ = new vector< MccAddPdbStat::_AddPdbStruc* > (1, $1);
		    }
                 | addpdbdefs_plus addpdbdefs
                    {
		      $$ = $1;
		      $$->push_back ($2);
		    }
;


addpdbdefs:   pdbfile_plus
               {
		 $$ = new MccAddPdbStat::_AddPdbStruc ($1);
	       }
;


cutoff_opt:
                 { $$ = -1.0; }
| TOK_CUTOFF flt { $$ = $2; }
;

	 
pdbfile_plus:   TOK_STRING
                 {
		   $$ = new vector< char* > (1, $1);
		 }
              | pdbfile_plus TOK_STRING
                 {
		   $$ = $1;
		   $$->push_back ($2);
		 }
;


displayfg:   TOK_DISPLAYFG TOK_LPAREN fgRef TOK_RPAREN
              {
		$$ = new MccDisplayFGStat ($3);
	      }
;
  
env:   TOK_ENV { $$ = new MccEnvStat (); }
;

note:   TOK_NOTE TOK_LPAREN TOK_STRING TOK_RPAREN
         {
	   $$ = new MccNoteStat ($3);
	 }
;


notes:   TOK_NOTES { $$ = new MccNotesStat (); }
;


remark:   TOK_REMARK TOK_LPAREN TOK_STRING TOK_RPAREN
           {
	     $$ = new MccRemarkStat ($3);
	   }
;


reset:   TOK_RESET { $$ = new MccResetStat (); }
;


resetdb:   TOK_RESETDB { $$ = new MccResetDBStat (); }
;


version:   TOK_VERSION { $$ = new MccVersion (); }
;


quit:   TOK_QUIT { $$ = new MccQuitStat (); }
;


sampling: TOK_INTEGER             { $$ = new MccSamplingSize ($1, true); }
          | flt TOK_PERCENT       { $$ = new MccSamplingSize ($1, false); }
;


queryexp_plus:   queryexp
                  {
		    $$ = new vector< MccQueryExpr* > (1, $1);
		  }
               | queryexp_plus queryexp
                  {
		    $$ = $1;
		    $$->push_back ($2);
		  }
;

	  
queryexp:   TOK_LBRACE filename_star qprop_opt TOK_RBRACE
             {
	       $$ = new MccQueryExpr ($2, $3);
	     }
;


filename_star:   /* empty */ { $$ = new vector< char* > (); }
               | filename_star filename
                  {
		    $$ = $1;
		    $$->push_back ($2);
		  }
;


filename:   TOK_FILENAME TOK_LPAREN TOK_STRING TOK_RPAREN
             {
	       $$ = $3;
	     }
;
			

qprop_opt:   /* empty */ { $$ = new MccQTrueFunc (); }
           | queryorexp { $$ = $1; }
;


queryorexp:   queryandexp { $$ = $1; }
            | queryorexp TOK_OR queryandexp
               {
		 $$ = new MccQOrFunc ($1, $3);
	       }
;


queryandexp:   queryfaceexp { $$ = $1; }
             | queryandexp TOK_AND queryfaceexp
                {
		  $$ = new MccQAndFunc ($1, $3);
		}
;


queryfaceexp:  querynotexp { $$ = $1; }
             | querynotexp TOK_FACE querynotexp
                {
		  $$ = new MccQFaceFunc ($1, $3);
		}
;


querynotexp:   queryidentexp { $$ = $1; }
             | TOK_NOT querynotexp
                {
		  $$ = new MccQNotFunc ($2);
		}
;



queryidentexp: TOK_IDENT { $$ = new MccQIdentFunc ($1); }
               | TOK_ANY { $$ = new MccQTrueFunc (); }
               | TOK_LPAREN queryorexp TOK_RPAREN { $$ = $2; }
;

 

fgexp:   backtrackexp { $$ = $1; }
       | cycleexp { $$ = $1; }
       | cacheexp { $$ = $1; }
       | libraryexp { $$ = $1; }
;


backtrackexp: TOK_BACKTRACK TOK_LPAREN fgRef_opt res_place_plus TOK_RPAREN
               {
		 MccBacktrackExpr *tmp = new MccBacktrackExpr ();

		 if ($3)
		   tmp->GenFGStruc ($3);
		 tmp->AddBTStrucs ($4);
		 $$ = tmp;
	       }
             | TOK_BACKTRACK TOK_LPAREN fgRef TOK_RPAREN
                {
		  MccBacktrackExpr *tmp = new MccBacktrackExpr ();
		  tmp->GenFGStruc ($3);
		  $$ = tmp;
		}
;


res_place_plus:   res_place
                   {
		     $$ = new vector< MccBacktrackExpr::_GenBTStruc* > (1, $1);
		   }
                | res_place_plus res_place
                   {
		     $$ = $1;
		     $$->push_back ($2);
		   }
;


res_place:  TOK_LPAREN residueRef residueRef_star TOK_RPAREN
             {
	       $$ = new MccBacktrackExpr::_BTStruc ($2, $3);
	     }
          | TOK_PLACE TOK_LPAREN residueRef residueRef fgRef TOK_RPAREN
             {
	       $$ = new MccBacktrackExpr::_PlaceStruc ($3, $4, $5);
	     }
;


cycleexp:   TOK_CYCLE TOK_LPAREN residueRef_star TOK_RPAREN
             {
	       $$ = new MccCycleExpr ($3);
	     }
;


cacheexp:   TOK_CACHE TOK_LPAREN fgRef model_filter TOK_RPAREN
             {
	       $$ = new MccCacheExpr ($3, $4);
	     }
;


align_opt:   /* empty */ { $$ = false; }
           | TOK_ALIGN { $$ = true; }
;


libraryexp:

TOK_LIBRARY TOK_LPAREN input_mode libopt_star asis_opt TOK_RPAREN
{
  $$ = new MccLibraryExpr ($3, $4, $5);
}
;


input_mode:

TOK_FILE_BINARY TOK_LPAREN TOK_STRING TOK_RPAREN
{
  $$ = new MccFileBinaryInput ($3);
}
| TOK_FILE_PDB TOK_LPAREN TOK_STRING TOK_RPAREN
{
  $$ = new MccFilePdbInput ($3);
}
| TOK_SOCKET_BINARY TOK_LPAREN TOK_STRING TOK_INTEGER TOK_STRING TOK_RPAREN
{
  $$ = new MccSocketBinaryInput ($3, $4, $5);
}
| TOK_FILE_RNAML TOK_LPAREN TOK_STRING TOK_RPAREN
{
  $$ = new MccFileRnamlInput ($3);
}
;


libopt_star:

/* empty */
{
  $$ = new vector< MccLibraryExpr::_LibStruc* > ();
}
| libopt_star libopt
{
  $$ = $1;
  $$->push_back ($2);
}
;


libopt:

TOK_STRIP TOK_LPAREN residueRef_plus TOK_RPAREN
{
  $$ = new MccLibraryExpr::_StripStruc ($3);
}
| TOK_CHANGEID TOK_LPAREN residueRef TOK_COMMA residueRef TOK_RPAREN
{
  $$ = new MccLibraryExpr::_ChangeIdStruc ($3, $5);
}
| TOK_CHANGEID TOK_LPAREN TOK_STRING TOK_COMMA TOK_STRING TOK_RPAREN
{
  $$ = new MccLibraryExpr::_ChangeChainStruc ($3[0], $5[0]);
  delete[] $3;
  delete[] $5;
}
;


asis_opt:

/* empty */
{
  $$ = false;
}
| TOK_ASIS
{
  $$ = true;
}
;


residueRef_star:   /* empty */ { $$ = new vector< MccResidueName* > (); }
                 | residueRef_star residueRef
                    {
		      $$ = $1;
		      $$->push_back ($2);
		    }
;


residueRef_plus:   residueRef { $$ = new vector< MccResidueName* > (1, $1); }
                 | residueRef_plus residueRef
                    {
		      $$ = $1;
		      $$->push_back ($2);
		    }
;
		 

residueRef:   TOK_INTEGER { $$ = new MccResidueName ($1); }
            | TOK_RESNAME
               {
		 char tmp_char;
		 int tmp_int;
		 
		 sscanf ($1, "%c%d", &tmp_char, &tmp_int); 
		 $$ = new MccResidueName (tmp_char, tmp_int);
		 delete $1;
	       }
;


atomRef:   TOK_IDENT { $$ = $1; }
         | TOK_RESNAME { $$ = $1; }
         | TOK_ATOM { $$ = $1; }
;


fgRef_opt:   /* empty */ { $$ = 0; }
           | fgRef { $$ = $1; }
;


fgRef:   TOK_IDENT { $$ = new MccFragGenStruc ($1); }
       | TOK_QUOTED_IDENT { $$ = new MccFragGenStruc ($1, true); }
;


ident_plus:   TOK_IDENT { $$ = $1; }
            | ident_plus TOK_IDENT
               {
		 $$ = new char[strlen ($1) + strlen ($2) + 1];
		 strcpy ($$, $1);
		 strcat ($$, $2);
		 delete[] $1;
		 delete[] $2;
	       }
;


flt:   TOK_INTEGER { $$ = $1; }
     | TOK_FLOAT { $$ = $1; }
;


%%

int
mccerror (const char *s)
{
  ParserException ex ("Parse error at line ");
  ex << mcclineno;
  throw ex;
}
