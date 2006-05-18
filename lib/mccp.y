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
  #include <map>

  #include "mccparser.h"

  using namespace std;

  #define YYERROR_VERBOSE 1

  extern int mcclex ();
  extern int mcclineno;
  int mccerror (const char *s);
%}


%union {
  char charval;
  int intval;
  bool boolval;
  float floatval;
  map< string, string >* stringmap;
  vector< string >* strv;
  char *textval;
  MccPStruct *mccval;
  vector< MccPStruct* > *vmccval;
  vector< MccRelationStat::_RelationStruc* > *rsv;
  MccRelationStat::_RelationStruc *rs;
  vector< MccAngleCstStat::_AngleStruc* > *asv;
  MccAngleCstStat::_AngleStruc *as;
  MccCycleCstStat::_CycleStruc *cycs;
  vector< MccCycleCstStat::_CycleStruc* > *cycsv;
  vector< MccMultimerCstStat::_MultimerStruc* > *cysv;
  MccMultimerCstStat::_MultimerStruc *cys;
  vector< MccDistCstStat::_DistStruc* > *dsv;
  MccDistCstStat::_DistStruc *ds;
  vector< MccTorsionCstStat::_TorsionStruc* > *tsv;
  MccImplicitRelationCstStat::_ImplicitRelationStruc *rels;
  vector< MccImplicitRelationCstStat::_ImplicitRelationStruc* > *relsv;
  MccTorsionCstStat::_TorsionStruc *ts;
  vector< char* > *textv;
  vector< MccQueryExpr* > *qsv;
  MccQueryExpr *qs;
  MccQFunc *qfunc;
  MccASFunc *asfunc;
  vector< MccBacktrackExpr::_GenBTStruc* > *btsv;
  MccBacktrackExpr::_GenBTStruc *bts;
  vector< MccResidueName* > *rrv;
  MccResidueName *rn;
  MccResidueNamePairs *rnp;
  MccResidueNameSingletons *rns;
  MccFragGenStruc *fgr;
  MccFGExp *fgs;
  MccOutputMode *expo;
  MccInputMode *inmo;
  vector< MccInputMode* > *inmov;
  MccSamplingSize *smpls;
  MccResidueViewCache* rvc;
}


%token TOK_LBRACE
%token TOK_RBRACE
%token TOK_COMMA
%token TOK_COLON
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
%token TOK_LARROW

%token TOK_ALIGN
%token TOK_ANGLE
%token TOK_ANY
%token TOK_ASIS
%token TOK_BACKTRACK
%token TOK_BACKTRACK_RST
%token TOK_BASEADJACENCY
%token TOK_CACHE
%token TOK_CYCLE
%token TOK_DBDISPLAY
%token TOK_DBFILTER
%token TOK_DBINFO
%token TOK_DBINSERT
%token TOK_DBRESET
%token TOK_DBVERSION
%token TOK_DBNOTE
%token TOK_DISPLAYFG
%token TOK_DISTANCE
%token TOK_ENV
%token TOK_EXPLORE
%token TOK_FILENAME
%token TOK_FIXEDDIST
%token TOK_IMPLICITRELATION
%token TOK_LIBRARY
%token TOK_MERGE
%token TOK_MULTIMER
%token TOK_MULTIPLE
%token TOK_NEWTAG
%token TOK_NOTE
%token TOK_NOTES
%token TOK_BINARY
%token TOK_OPTION
%token TOK_PDB
%token TOK_RNAML
%token TOK_PLACE
%token TOK_QUIT
%token TOK_RELATION
%token TOK_REMARK
%token TOK_CLASH
%token TOK_CYCLE
%token TOK_RESET
%token TOK_RESTORE
%token TOK_IMPLICITPHOSPHATERST
%token TOK_RIBOSERST
%token TOK_RMSD
%token TOK_SEQUENCE
%token TOK_SINGLE
%token TOK_SOCKET
%token TOK_SOURCE
%token TOK_STRIP
%token TOK_THREADED
%token TOK_TORSION
%token TOK_TFOD
%token TOK_VDWDIST
%token TOK_VERSION
%token TOK_ZIPPED
%token TOK_REPORTIMPLICITPHOSPHATE
%token TOK_REPORTRIBOSE
%token TOK_UNSORTED
%token <intval> TOK_INTEGER
%token <floatval> TOK_FLOAT
%token <textval> TOK_RESNAME
%token <textval> TOK_RELATIVE_RESNAME
%token <textval> TOK_ATOM
%token <textval> TOK_IDENT
%token <textval> TOK_QUOTED_IDENT
%token <textval> TOK_STRING
%type<strv> string_plus;

%type <mccval> statement
%type <mccval> sequence
%type <mccval> assign
%type <smpls> sampling
%type <mccval> relation
%type <rsv> reldef_plus
%type <rs> reldef
%type <mccval> explore
%type <stringmap> explore_option_opt
%type <mccval> restore
%type <expo> output_mode_opt
%type <expo> output_mode
%type <rvc> rv_cache_opt
%type <rvc> rv_cache
%type <boolval> zfile_opt
%type <boolval> mfile_opt
%type <mccval> source
%type <mccval> baseAdjCst
%type <mccval> cycleCst
%type <cycs> cycledef
%type <cycsv> cycledef_plus
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

%type <mccval> backtrackRst
%type <mccval> riboseRst
%type <mccval> implicitpo4Rst
%type <stringmap> keyValue_plus

%type <mccval> newtag
%type <mccval> dbdisplay
%type <mccval> dbfilter
%type <mccval> dbinsert
%type <mccval> dbreset
%type <mccval> dbversion
%type <mccval> displayfg
%type <mccval> env
%type <mccval> note
%type <mccval> notes
%type <relsv> relationdef_plus
%type <rels> relationdef
%type <mccval> implicitRelCst
%type <mccval> remark
%type <mccval> reset
%type <mccval> version
%type <mccval> quit
%type <mccval> reportImplicitPhosphate
%type <mccval> reportRibose

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
%type <fgs> libraryexp
%type <inmov> input_mode_plus
%type <inmo> input_mode
%type <boolval> asis_opt
%type <boolval> threaded_opt

%type <rrv> residueRef_star
%type <rrv> residueRef_plus
%type <rn> residueRef
%type <rnp> residueRef_pair_plus
%type <rnp> residueRef_pair
%type <rns> residueRef_singleton_plus
%type <rns> residueRef_singleton
%type <textval> atomRef
%type <fgr> fgRef_opt
%type <fgr> fgRef

%type <textval> ident_plus
%type <textval> strVal
%type <intval> intgr_percent
%type <intval> intgr_any
%type <floatval> flt
%type <floatval> flt_percent
%type <floatval> flt_any
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
           | relation { $$ = $1; }
           | explore { $$ = $1; }
           | restore { $$ = $1; }
           | source { $$ = $1; }
           | baseAdjCst { $$ = $1; }
           | angleCst { $$ = $1; }
           | clashCst { $$ = $1; }
           | cycleCst { $$ = $1; }
           | multimerCst { $$ = $1; }
           | distCst { $$ = $1; }
           | implicitRelCst { $$ = $1; }
           | torsionCst { $$ = $1; }
           | backtrackRst { $$ = $1; }
           | implicitpo4Rst { $$ = $1; }
           | riboseRst { $$ = $1; }
           | newtag { $$ = $1; }
           | dbdisplay { $$ = $1; }
           | dbinsert { $$ = $1; }
           | dbfilter { $$ = $1; }
           | dbreset { $$ = $1; }
           | dbversion { $$ = $1; }
           | displayfg { $$ = $1; }
           | env { $$ = $1; }
           | note { $$ = $1; }
           | notes { $$ = $1; }
           | remark { $$ = $1; }
           | reset { $$ = $1; }
           | version { $$ = $1; }
           | quit { $$ = $1; }
           | reportImplicitPhosphate { $$ = $1; }
           | reportRibose { $$ = $1; }
;


sequence:  
TOK_SEQUENCE TOK_LPAREN TOK_IDENT residueRef ident_plus TOK_RPAREN
{
  $$ = new MccSequenceStat ($3[0], $4, $5);
  delete[] $3;
  delete[] $5;
}
| TOK_SEQUENCE TOK_LPAREN residueRef ident_plus TOK_RPAREN
{
  $$ = new MccSequenceStat ('r', $3, $4);
  delete[] $4;
}
;


assign:    TOK_IDENT TOK_ASSIGN fgexp { $$ = new MccAssignStat ($1, $3); }
;


relation:

TOK_RELATION TOK_LPAREN reldef_plus TOK_RPAREN
{
  $$ = new MccRelationStat ($3);
}
;


reldef_plus:

reldef
{
  $$ = new vector< MccRelationStat::_RelationStruc* > (1, $1);
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
  $$ = new MccRelationStat::_RelationStruc ($1, $2, $3);
}
;


explore:

TOK_EXPLORE TOK_LPAREN fgRef explore_option_opt rv_cache_opt output_mode_opt TOK_RPAREN
{
  $$ = new MccExploreStat (*$3, *$4, $5, $6);
  delete $3;
  delete $4;
}
;


explore_option_opt:

/* empty */
{
  $$ = new map< string, string > ();
}
|
TOK_OPTION TOK_LPAREN keyValue_plus TOK_RPAREN
{
  $$ = $3;
}
;


restore:   TOK_RESTORE TOK_LPAREN TOK_STRING output_mode_opt TOK_RPAREN
            {
	      $$ = new MccRestoreStat ($3, $4);
	    }
;


rv_cache_opt: 

/* empty */ 
{ 
  $$ = 0; 
}
| rv_cache 
{ 
  $$ = $1; 
}
;


rv_cache: 

TOK_RMSD TOK_LPAREN flt atomset_opt TOK_RPAREN
{
  $$ = new MccRMSDResidueViewCache ($3, $4);
}
| TOK_RMSD TOK_LPAREN flt atomset_opt TOK_ALIGN TOK_RPAREN
{
  $$ = new MccRMSDResidueViewCache ($3, $4, true);
}
| TOK_TFOD TOK_LPAREN flt TOK_RPAREN
{
  $$ = new MccTFODResidueViewCache ($3);
}
;


output_mode_opt:   

/* empty */ 
{ 
  $$ = 0; 
}
| output_mode 
{ 
  $$ = $1; 
}
;


output_mode:  

TOK_PDB TOK_LPAREN TOK_STRING zfile_opt mfile_opt TOK_RPAREN
{
  $$ = new MccFilePdbOutput ($3, $4, $5);
  delete[] $3;
}
| TOK_BINARY TOK_LPAREN TOK_STRING zfile_opt mfile_opt TOK_RPAREN
{
  $$ = new MccFileBinaryOutput ($3, $4, $5);
  delete[] $3;
}
| TOK_RNAML TOK_LPAREN TOK_STRING zfile_opt mfile_opt TOK_RPAREN
{
  $$ = new MccFileRnamlOutput ($3, $4, $5);
  delete[] $3;
}
| TOK_SOCKET TOK_LPAREN TOK_STRING TOK_INTEGER TOK_STRING mfile_opt TOK_RPAREN
{
  $$ = new MccSocketBinaryOutput ($3, $4, $5, $6);
  delete[] $3;
  delete[] $5;
}
;


zfile_opt:   /* empty */ { $$ = false; }
           | TOK_ZIPPED { $$ = true; }
;


mfile_opt:
/* empty */    { $$ = true; }
| TOK_SINGLE   { $$ = false; }
| TOK_MULTIPLE { $$ = true; }


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


backtrackRst:

TOK_BACKTRACK_RST TOK_LPAREN fgRef keyValue_plus TOK_RPAREN
{
  $$ = new MccBacktrackRstStat (*$3, *$4);
  delete $3;
  delete $4;
}
;


implicitpo4Rst:

TOK_IMPLICITPHOSPHATERST TOK_LPAREN fgRef keyValue_plus TOK_RPAREN
{
  $$ = new MccImplicitPhosphateRstStat (*$3, *$4);
  delete $3;
  delete $4;
}
|
TOK_IMPLICITPHOSPHATERST TOK_LPAREN fgRef TOK_LBRACKET residueRef_pair_plus TOK_RBRACKET keyValue_plus TOK_RPAREN
{
  $$ = new MccImplicitPhosphateRstStat (*$3, *$5, *$7);
  delete $3;
  delete $5;
  delete $7;
}
;


riboseRst:

TOK_RIBOSERST TOK_LPAREN fgRef keyValue_plus TOK_RPAREN
{
  $$ = new MccRiboseRstStat (*$3, *$4);
  delete $3;
  delete $4;
}
|
TOK_RIBOSERST TOK_LPAREN fgRef TOK_LBRACKET residueRef_singleton_plus TOK_RBRACKET keyValue_plus TOK_RPAREN
{
  $$ = new MccRiboseRstStat (*$3, *$5, *$7);
  delete $3;
  delete $5;
  delete $7;
}
;


keyValue_plus:

strVal TOK_ASSIGN strVal
{
  pair< string, string > entry ($1, $3);
  $$ = new map< string, string > ();
  $$->insert (entry);
  delete[] $1;
  delete[] $3;
}
| keyValue_plus TOK_COMMA strVal TOK_ASSIGN strVal
{
  pair< string, string > entry ($3, $5);
  $$ = $1;
  $$->insert (entry);
  delete[] $3;
  delete[] $5;
}
;


cycleCst:   

TOK_CYCLE TOK_LPAREN cycledef_plus TOK_RPAREN
{
  $$ = new MccCycleCstStat ($3);
}
;


cycledef_plus:

cycledef
{
  $$ = new vector< MccCycleCstStat::_CycleStruc* > (1, $1);
}
| cycledef_plus cycledef
{
  $$ = $1;
  $$->push_back ($2);
}
;


cycledef:

residueRef residueRef flt
{
  $$ = new MccCycleCstStat::_CycleStruc ($1, $2, $3);
}


implicitRelCst:   

TOK_IMPLICITRELATION TOK_LPAREN relationdef_plus TOK_RPAREN
{
  $$ = new MccImplicitRelationCstStat ($3);
}
;


relationdef_plus:   relationdef
                  {
		    $$ = new vector< MccImplicitRelationCstStat::_ImplicitRelationStruc* > (1, $1);
		  }
               | relationdef_plus relationdef
                  {
		    $$ = $1;
		    $$->push_back ($2);
		  }
;


relationdef:   residueRef residueRef queryexp
             {
	       $$ = new MccImplicitRelationCstStat::_ImplicitRelationStruc ($1, $2, $3);
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

	 
dbdisplay:   

TOK_DBDISPLAY
{
  $$ = new MccDBDisplayStat ();
}
;


dbfilter:   

TOK_DBFILTER TOK_LPAREN flt threaded_opt TOK_RPAREN
{
  $$ = new MccDBFilterStat ($3, $4);
}
;


threaded_opt:

/* empty */
{
  $$ = false;
}
| TOK_THREADED
{
  $$ = true;
}
;


dbinsert:   

TOK_DBINSERT TOK_LPAREN input_mode TOK_RPAREN
{
  $$ = new MccDBInsertStat ($3);
}
| TOK_DBINSERT TOK_LPAREN input_mode flt TOK_RPAREN
{
  $$ = new MccDBInsertStat ($3, $4);
}
;


dbreset:
   
TOK_DBRESET
{ 
  $$ = new MccDBResetStat (); 
}
;


dbversion:   

TOK_DBVERSION TOK_LPAREN TOK_STRING TOK_RPAREN
{
  $$ = new MccDBSetVersionStat ($3);
  delete[] $3;
}
;




displayfg:   TOK_DISPLAYFG TOK_LPAREN fgRef TOK_RPAREN
              {
		$$ = new MccDisplayFGStat ($3);
	      }
;




env:   TOK_ENV { $$ = new MccEnvStat (); }
;


note:   
TOK_NOTE TOK_LPAREN TOK_STRING TOK_RPAREN
{
  $$ = new MccNoteStat ($3);
}
| TOK_DBNOTE TOK_LPAREN TOK_STRING TOK_RPAREN
{
  $$ = new MccNoteStat ($3);
}
;


notes:   
TOK_NOTES 
{ 
  $$ = new MccNotesStat (); 
}
| TOK_DBINFO
{
  $$ = new MccNotesStat ();
}
;


remark:   TOK_REMARK TOK_LPAREN TOK_STRING TOK_RPAREN
           {
	     $$ = new MccRemarkStat ($3);
	   }
;


reset:   TOK_RESET { $$ = new MccResetStat (); }
;


version:   TOK_VERSION { $$ = new MccVersion (); }
;


quit:   TOK_QUIT { $$ = new MccQuitStat (); }
;


reportImplicitPhosphate:

TOK_REPORTIMPLICITPHOSPHATE TOK_LPAREN fgRef TOK_RPAREN
{
  $$ = new MccReportImplicitPhosphateStat (*$3);
  delete $3;
}
;


reportRibose:

TOK_REPORTRIBOSE TOK_LPAREN fgRef TOK_RPAREN
{
  $$ = new MccReportRiboseStat (*$3);
  delete $3;
}
;


// sampling: TOK_INTEGER             { $$ = new MccSamplingSize ($1, true); }
//           | flt TOK_PERCENT       { $$ = new MccSamplingSize ($1, false); }
// ;
sampling:

TOK_INTEGER
{
  $$ = new MccSamplingSize ($1, true, false);
}
|
TOK_INTEGER TOK_UNSORTED
{
  $$ = new MccSamplingSize ($1, true, true);
}
|
flt_percent
{
  $$ = new MccSamplingSize ($1, false, false);
}
|
flt_percent TOK_UNSORTED
{
  $$ = new MccSamplingSize ($1, false, true);
}
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
          | TOK_MERGE TOK_LPAREN fgRef flt atomset_opt TOK_RPAREN
             {
	       $$ = new MccBacktrackExpr::_MergeStruc ($3, $4, $5);
	     }
;


cycleexp:   TOK_CYCLE TOK_LPAREN residueRef_star TOK_RPAREN
             {
	       $$ = new MccCycleExpr ($3);
	     }
;


cacheexp:   

TOK_CACHE TOK_LPAREN fgRef rv_cache_opt TOK_RPAREN
{
  $$ = new MccCacheExpr (*$3, $4);
  delete $3;
}
;


libraryexp:

TOK_LIBRARY TOK_LPAREN input_mode_plus asis_opt rv_cache_opt TOK_RPAREN
{
  $$ = new MccLibraryExpr (*$3, $4, $5);
  delete $3;
}
|
TOK_LIBRARY TOK_LPAREN input_mode_plus residueRef_singleton_plus TOK_LARROW residueRef_singleton_plus asis_opt rv_cache_opt TOK_RPAREN
{
  $$ = new MccLibraryExpr (*$3, *$4, *$6, $7, $8);
  delete $3;
  delete $4;
  delete $6;
}
;


input_mode_plus:

input_mode
{
  $$ = new vector< MccInputMode* > (1, $1);
}
|
input_mode_plus input_mode
{
  $$ = $1;
  $$->push_back ($2);
}
;


input_mode:

TOK_BINARY TOK_LPAREN TOK_STRING TOK_RPAREN
{
  $$ = new MccFileBinaryInput ($3);
  delete[] $3;
}
| TOK_PDB TOK_LPAREN TOK_STRING TOK_RPAREN
{
  $$ = new MccFilePdbInput ($3);
  delete[] $3;
}
| TOK_RNAML TOK_LPAREN TOK_STRING TOK_RPAREN
{
  $$ = new MccFileRnamlInput ($3);
  delete[] $3;
}
| TOK_SOCKET TOK_LPAREN TOK_STRING TOK_INTEGER string_plus TOK_RPAREN
{
  $$ = new MccSocketBinaryInput ($3, $4, *$5);
  delete[] $3;
  delete $5;
}
;


string_plus:

TOK_STRING
{
  $$ = new vector< string > (1, $1);
  delete[] $1;
}
| string_plus TOK_STRING
{
  $$ = $1;
  $$->push_back ($2);
  delete[] $2;
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
		 

residueRef:   

TOK_INTEGER 
{ 
  $$ = new MccResidueName ($1); 
}
| TOK_RESNAME
{
  char tmp_char;
  int tmp_int;
  
  sscanf ($1, "%c%d", &tmp_char, &tmp_int); 
  $$ = new MccResidueName (tmp_char, tmp_int);
  delete $1;
}
| TOK_RELATIVE_RESNAME
{
  char tmp_char;
  int tmp_int;
  
  sscanf ($1, "%c%d", &tmp_char, &tmp_int); 
  $$ = new MccResidueName (tmp_char, tmp_int);
  delete $1;
}
| TOK_QUOTED_IDENT TOK_INTEGER
{
  if (strlen ($1) > 1)
  {
    ostringstream oss;
    oss << "invalid multicharacter chain ID \'" << $1 << "\'";
    mccerror (oss.str ().c_str ());
  }

  $$ = new MccResidueName ($1[0], $2);
  delete $1;
}
;


residueRef_pair_plus:

residueRef_pair
{
  $$ = $1;
}
| residueRef_pair_plus TOK_COMMA residueRef_pair
{
  $$ = $1;
  $$->add (*$3);
  delete $3;
}
;


residueRef_pair:

residueRef residueRef
{
  $$ = new MccResidueNamePairs ();
  $$->add (*$1, *$2);
  delete $1;
  delete $2;
}
| residueRef TOK_COLON residueRef
{
  $$ = new MccResidueNamePairs ();
  $$->addRange (*$1, *$3);
  delete $1;
  delete $3;
}
;


residueRef_singleton_plus:

residueRef_singleton
{
  $$ = $1;
}
| residueRef_singleton_plus TOK_COMMA residueRef_singleton
{
  $$ = $1;
  $$->add (*$3);
  delete $3;
}
;


residueRef_singleton:

residueRef
{
  $$ = new MccResidueNameSingletons ();
  $$->add (*$1);
  delete $1;
}
| residueRef TOK_COLON residueRef
{
  $$ = new MccResidueNameSingletons ();
  $$->addRange (*$1, *$3);
  delete $1;
  delete $3;
}
;


atomRef:   TOK_IDENT { $$ = $1; }
         | TOK_RESNAME { $$ = $1; }
         | TOK_ATOM { $$ = $1; }
;


fgRef_opt:   

/* empty */ 
{ 
  $$ = 0; 
}
| fgRef 
{ 
  $$ = $1; 
}
;


fgRef:   

TOK_IDENT 
{ 
  $$ = new MccFragGenStruc ($1);
  delete[] $1; 
}
| TOK_QUOTED_IDENT 
{ 
  $$ = new MccFragGenStruc ($1, true); 
  delete[] $1; 
}
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




intgr_percent:

TOK_INTEGER TOK_PERCENT
{
  $$ = (int)round ((double)$1 / 100.0);
  //  cout << "intgr_percent: " << $1 << "% = " << $$ << endl;
}
;


intgr_any:

TOK_INTEGER
{
  $$ = $1;
  //  cout << "int any " << $1 << endl;
}
| intgr_percent
{
  $$ = $1;
  //  cout << "int any (%) " << $1 << endl;
}


flt:   

TOK_INTEGER 
{ 
  $$ = $1; 
  //  cout << "flt (int) " << $1 << endl;
}
| TOK_FLOAT 
{ 
  $$ = $1; 
  //  cout << "flt " << $1 << endl;
}
;


flt_percent:

flt TOK_PERCENT
{
  $$ = $1 / 100.0;
  //  cout << "flt_percent: " << $1 << "% = " << $$ << endl;
}
;


flt_any:

flt
{
  $$ = $1;
  //  cout << "flt any " << $1 << endl;
}
| flt_percent
{
  $$ = $1;
  //  cout << "flt any  (%) " << $1 << endl;
}
;


strVal:

TOK_STRING
{
  $$ = $1;
}
| TOK_IDENT
{
  $$ = $1;
}
| TOK_QUOTED_IDENT
{
  $$ = $1;
}
| flt_any
{
  ostringstream oss;
  oss.setf (ios::fixed, ios::floatfield);
  oss.precision (15);
  oss << $1;
  $$ = strdup (oss.str ().c_str ());
}
// | flt TOK_PERCENT
// {
//   ostringstream oss;
//   oss.setf (ios::fixed, ios::floatfield);
//   oss.precision (15);
//   oss << ($1 / 100.0);
//   $$ = strdup (oss.str ().c_str ());
// }
;

%%

int
mccerror (const char *s)
{
  ParserException ex ("Parse error at line ");
  ex << (mcclineno + 1) << ": " << s;
  throw ex;
}
