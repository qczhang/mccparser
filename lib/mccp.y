/*                               -*- Mode: C++ -*- 
 * mccp.y
 * Copyright © 2000-01 , 2002, 2002Laboratoire de Biologie Informatique et Théorique.
 * Author           : Martin Larose
 * Created On       : Tue Aug 22 11:43:17 2000
 * Last Modified By : Philippe Thibault
 * Last Modified On : Tue Oct 28 09:33:14 2003
 * Update Count     : 29
 * Status           : Ok.
 */


%{
  #include <iostream.h>
  #include <vector.h>
  #include <map.h>
  #include <stdlib.h>
  #include <string.h>
  #include <math.h>

  #include "mccparser.h"

  extern int mcclex ();
  extern int mcclineno;
  int mccerror (char *s);
%}


%union {
  char charval;
  int intval;
  bool boolval;
  float floatval;
  char *textval;
  MccPStruct *mccval;
  vector< MccPStruct* > *vmccval;
  vector< MccResidueStat::_ResidueStruc* > *rsv;
  MccResidueStat::_ResidueStruc *rs;
  vector< MccConnectStat::_ConnectStruc* > *csv;
  MccConnectStat::_ConnectStruc *cs;
  vector< MccPairStat::_PairStruc* > *psv;
  MccPairStat::_PairStruc *ps;
  vector< MccAngleCstStat::_AngleStruc* > *asv;
  MccAngleCstStat::_AngleStruc *as;
  vector< MccCycleCstStat::_CycleStruc* > *cysv;
  MccCycleCstStat::_CycleStruc *cys;
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
  vector< MccBacktrackExpr::_GenBTStruc* > *btsv;
  MccBacktrackExpr::_GenBTStruc *bts;
  vector< MccLibraryExpr::_LibStruc* > *lsv;
  MccLibraryExpr::_LibStruc *ls;
  MccAS ats;
  vector< MccResidueName* > *rrv;
  MccResidueName *rr;
  MccFragGenStruc *fgr;
  MccFGExp *fgs;
  MccOutputMode *expo;
  MccInputMode *inmo;
  MccBacktrackSize *btsz;
  vector< int > *tlv;
  MccSamplingSize *smpls;
  cutoffs *ctfs;
  MccModelFilterStrategy *mf;
  MccModelSorterStrategy *ms;
  MccMosesQueries *mqs;
  pair< MccQueryExpr*, MccSamplingSize* > *qsssp;

  //! placer
  vector< MccPlacerConnectStat::_PlacerConnectStruc* > *pcsv;
  MccPlacerConnectStat::_PlacerConnectStruc *pcs;
  vector< MccPlacerPairStat::_PlacerPairStruc* > *ppsv;
  MccPlacerPairStat::_PlacerPairStruc *pps;
  MccPlacerResidueName *prr;
  vector< MccPlacerResidueName* > *prrv;
  vector< MccPlacerBuildStat::_GenBTStruc* > *pbtsv;
  MccPlacerBuildStat::_GenBTStruc *pbts;
  MccPlacerBuildStat::_ClashStruc *pbx;
  MccPlacerBuildStat::_ClosureStruc *pbc;
  MccPlacerBuildStat::_RibStruc *pbr;
  MccPlacerSearchStat::_Params *pssp;
  map< int, float >* mif;
  pair< int, float>* pif;
  //!
}


%token TOK_LBRACE
%token TOK_RBRACE
%token TOK_COMMA
%token TOK_COLON
%token TOK_DASH
%token TOK_CTFILEID
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
%token TOK_ADJACENCY
%token TOK_ALIGN
%token TOK_ALLATOMS
%token TOK_ANGLE
%token TOK_ANGSTROMS
%token TOK_ANY
%token TOK_BBATOMS
%token TOK_BACKTRACK
%token TOK_BACKTRACKSIZE
%token TOK_BASEATOMS
%token TOK_CACHE
%token TOK_CGAUPAIRDEF
%token TOK_CGAURESDEF
%token TOK_CHANGEID
%token TOK_CLUSTERED
%token TOK_CONFOCUT
%token TOK_CONNECT
%token TOK_CYCLE
%token TOK_DISPLAYFG
%token TOK_DISTANCE
%token TOK_EXPLORE
%token TOK_EXPLORELV
%token TOK_FILENAME
%token TOK_PDB
%token TOK_FIXEDDIST
%token TOK_GUPAIRDEF
%token TOK_GURESDEF
%token TOK_JOBS
%token TOK_LIBRARY
%token TOK_LOOPCONDEF
%token TOK_LOOPRESDEF
%token TOK_MOSES
%token TOK_NEWTAG
%token TOK_NOHYDRO
%token TOK_NOTE
%token TOK_NOTES
%token TOK_PAIR
%token TOK_PDB
%token TOK_FILE_BINARY
%token TOK_FILE_PDB
%token TOK_MFOLD_EPC
%token TOK_MFOLD_OUTPUT
%token TOK_MFOLD_WIN
%token TOK_PLACE
%token TOK_PSEATOMS
%token TOK_QUIT
%token TOK_RELATION
%token TOK_REMARK
%token TOK_RES
%token TOK_RESIDUE
%token TOK_RESCLASH
%token TOK_RESET
%token TOK_RESETDB
%token TOK_RESIDUE
%token TOK_RESTORE
%token TOK_RMSD
%token TOK_SAMPLINGFACT
%token TOK_SEQUENCE
%token TOK_SOCKET_BINARY
%token TOK_SOURCE
%token TOK_STEMCONDEF
%token TOK_STRIP
%token TOK_TIMELIMIT
%token TOK_TRANSFO
%token TOK_TFOCUT
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
%token <textval> TOK_ATOM
%token <textval> TOK_IDENT
%token <textval> TOK_QUOTED_IDENT
%token <textval> TOK_STRING


//! placer
%token TOK_PLACER_SEQUENCE
%token TOK_PLACER_CONNECT
%token TOK_PLACER_PAIR
%token TOK_PLACER_FRAGMENT
%token TOK_PLACER_BUILD
%token TOK_PLACER_SEARCH
%token TOK_CLASH
%token TOK_CLOSURE
%token TOK_CLPREC
%token TOK_CLPREC_C1P
%token TOK_RIBOSE
%token TOK_ROPT5D
%token TOK_ROPT2D
%token TOK_REST
%token TOK_ROP
//!

%type <mccval> statement
%type <mccval> sequence
%type <mccval> assign
%type <smpls> sampling


%type <mccval> residue
%type <rsv> resdef_plus
%type <rs> resdef
%type <mccval> connect
%type <csv> condef_plus
%type <cs> condef
%type <mccval> pair
%type <psv> pairdef_plus
%type <ps> pairdef
%type <mccval> explore
%type <mccval> exploreLV
%type <mccval> restore
%type <expo> output_mode_opt
%type <expo> output_mode
%type <mf> model_filter_opt
%type <mf> model_filter
%type <ms> model_sorter_opt
%type <ms> model_sorter
%type <tlv>  timelimit_opt
%type <tlv>  timelimit_exp
%type <tlv>  timelimit_plus
%type <intval> timelimit
%type <btsz> backtracksize
%type <btsz> backtracksize_opt
%type <boolval> zfile_opt
%type <mccval> source
%type <mccval> adjacencyCst
%type <mccval> angleCst
%type <asv> angledef_plus
%type <as> angledef
%type <mccval> clashCst
%type <boolval> fixorvdw_opt
%type <boolval> fixorvdw
%type <mccval> cycleCst
%type <cysv> cycledef_plus
%type <cys> cycledef
%type <mccval> distCst
%type <dsv> distdef_plus
%type <ds> distdef
%type <mccval> torsionCst
%type <tsv> torsiondef_plus
%type <ts> torsiondef
%type <mccval> newtag
%type <boolval> resortfo
%type <mccval> addpdb
%type <addsv> addpdbdefs_plus
%type <adds> addpdbdefs
%type <ctfs> cutoff_opt
%type <floatval> tfo_opt
%type <floatval> cfo_opt
%type <textv> pdbfile_plus
%type <mccval> displayfg
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

%type <fgs> fgexp
%type <fgs> backtrackexp
%type <btsv> res_place_plus
%type <bts> res_place
%type <fgs> cacheexp
%type <boolval> align_opt
%type <fgs> libraryexp
%type <inmo> input_mode
%type <lsv> libopt_star
%type <ls> libopt
%type <fgs> mosesexp
%type <textval> resrange_opt
%type <intval> ctfileid_opt
%type <mqs> mosesqueries
%type <qsssp> cgau_resdef_opt
%type <qsssp> gu_resdef_opt
%type <qsssp> loop_resdef_opt
%type <qsssp> stem_condef_opt
%type <qsssp> loop_condef_opt
%type <qsssp> cgau_pairdef_opt
%type <qsssp> gu_pairdef_opt
%type <intval> mfold_epc_opt
%type <intval> mfold_win_opt
%type <textval> mfold_output_opt

%type <ats> atomset_opt
%type <ats> atomsetopt_opt
%type <rrv> residueRef_star
%type <rrv> residueRef_plus
%type <rr> residueRef_opt
%type <rr> residueRef
%type <textval> atomRef
%type <fgr> fgRef_opt
%type <fgr> fgRef

//! placer
%type <mccval> placer_sequence
%type <mccval> placer_connect
%type <pcsv> placer_condef_plus
%type <pcs> placer_condef
%type <mccval> placer_pair
%type <ppsv> placer_pairdef_plus
%type <pps> placer_pairdef
%type <prr> placer_residueRef
%type <prrv> placer_residueRef_star
%type <prrv> placer_residueRef_plus
%type <prr> placer_residueRef_opt
%type <mccval> placer_fragment
%type <mccval> placer_build
%type <pbx> clash_opt
%type <pbc> closure_opt
%type <charval> closure_mode_opt
%type <pbr> ribose_opt
%type <floatval> ribose_value_opt
%type <textval> ribose_qfct_opt
%type <pbtsv> placer_res_place_plus
%type <pbts> placer_res_place
%type <mccval> placer_search
%type <pssp> search_mode
%type <mif> search_params_star
%type <pif> search_params
//!

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
           | residue { $$ = $1; }
           | connect { $$ = $1; }
           | pair { $$ = $1; }
           | explore { $$ = $1; }
           | exploreLV { $$ = $1; }
           | restore { $$ = $1; }
           | source { $$ = $1; }
           | adjacencyCst { $$ = $1; }
           | angleCst { $$ = $1; }
           | clashCst { $$ = $1; }
           | cycleCst { $$ = $1; }
           | distCst { $$ = $1; }
           | relationCst { $$ = $1; }
           | torsionCst { $$ = $1; }
           | newtag { $$ = $1; }
           | addpdb { $$ = $1; }
           | displayfg { $$ = $1; }
           | note { $$ = $1; }
           | notes { $$ = $1; }
           | remark { $$ = $1; }
           | reset { $$ = $1; }
           | resetdb { $$ = $1; }
           | version { $$ = $1; }
           | quit { $$ = $1; }
//! placer
           | placer_sequence { $$ = $1; }
           | placer_connect { $$ = $1; }
           | placer_pair { $$ = $1; }
           | placer_fragment { $$ = $1; }
           | placer_build { $$ = $1; }
           | placer_search { $$ = $1; }

;


sequence:  TOK_SEQUENCE TOK_LPAREN TOK_IDENT residueRef ident_plus TOK_RPAREN
            {
	      if (strlen ($3) != 1)
		throw CParserException ("Invalid sequence type.");
	      $$ = new MccSequenceStat ($3[0], $4, $5);
	    }
;


assign:    TOK_IDENT TOK_ASSIGN fgexp { $$ = new MccAssignStat ($1, $3); }
;


residue:   TOK_RESIDUE TOK_LPAREN resdef_plus TOK_RPAREN
            {
	      $$ = new MccResidueStat ($3);
	    }
;


resdef_plus:   resdef
                 {
		   $$ = new vector< MccResidueStat::_ResidueStruc* > (1, $1);
		 }
              | resdef_plus resdef
                 {
		   $$ = $1;
		   $$->push_back ($2);
		 }
;


resdef:  residueRef residueRef_opt queryexp sampling
           {
	     $$ = new MccResidueStat::_ResidueStruc ($1, $2, $3, $4);
	   }
;


connect:   TOK_CONNECT TOK_LPAREN condef_plus TOK_RPAREN
            {
	      $$ = new MccConnectStat ($3);
	    }
;


condef_plus:   condef
                {
		  $$ = new vector< MccConnectStat::_ConnectStruc* > (1, $1);
		}
             | condef_plus condef
                {
		  $$ = $1;
		  $$->push_back ($2);
		}
;


condef:   residueRef residueRef queryexp sampling
           {
	     $$ = new MccConnectStat::_ConnectStruc ($1, $2, $3, $4);
	   }
;


pair:   TOK_PAIR TOK_LPAREN pairdef_plus TOK_RPAREN
         {
	   $$ = new MccPairStat ($3);
	 }
;


pairdef_plus:   pairdef
                 {
		   $$ = new vector< MccPairStat::_PairStruc* > (1, $1);
		 }
             | pairdef_plus pairdef
                {
		  $$ = $1;
		  $$->push_back ($2);
		}
;


pairdef:   residueRef residueRef queryexp sampling
            {
	      $$ = new MccPairStat::_PairStruc ($1, $2, $3, $4);
	    }
;


explore:   TOK_EXPLORE TOK_LPAREN fgRef model_filter_opt output_mode_opt TOK_RPAREN
            {
	      $$ = new MccExploreStat ($3, $4, $5);
	    }
;


restore:   TOK_RESTORE TOK_LPAREN TOK_STRING output_mode_opt TOK_RPAREN
            {
	      $$ = new MccRestoreStat ($3, $4);
	    }
;


exploreLV: TOK_EXPLORELV TOK_LPAREN fgRef model_filter_opt output_mode_opt timelimit_opt backtracksize_opt TOK_RPAREN
            {
	      $$ = new MccExploreLVStat ($3, $4, $5, $6, $7);
	    }
;



model_filter_opt: /* empty */ { $$ = 0; }
                  | model_filter { $$ = $1; }
;



model_filter: TOK_RMSD TOK_LPAREN flt align_opt atomset_opt atomsetopt_opt TOK_RPAREN
             {
	       $$ = new MccRmsdModelFilterStrategy ($3, $4, $5, $6);
	     }
;



output_mode_opt:   /* empty */ { $$ = 0; }
                   | output_mode { $$ = $1; }
;



output_mode:   /* deprecated */
	       TOK_PDB TOK_LPAREN TOK_STRING TOK_RPAREN zfile_opt
               {
		 $$ = new MccFilePdbOutput ($3, $5);
	       }
             | TOK_FILE_PDB TOK_LPAREN TOK_STRING zfile_opt TOK_RPAREN
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
;



timelimit_opt:  /* empty */ { $$ = 0; }
               | timelimit_exp { $$ = $1; }
;



timelimit_exp:   TOK_TIMELIMIT TOK_LPAREN timelimit_plus TOK_RPAREN
                   { $$ = $3; }
;

timelimit_plus:  timelimit
                   { 
		     $$ = new vector< int > (1, $1);
		   }
                | timelimit_plus timelimit
                   {
		     $$ = $1;
		     $$->push_back ($2);
                   }
;


timelimit:     TOK_INTEGER TOK_SEC { $$ = $1; }
             | flt TOK_MIN { $$ = (int)rint ($1 * 60.0); }
             | flt TOK_HR  { $$ = (int)rint ($1 * 3600.0); }
             | flt TOK_DAY { $$ = (int)rint ($1 * 86400.0); }
;



backtracksize_opt:  /* empty */ { $$ = 0; }
                   | backtracksize { $$ = $1; }
;

backtracksize:   TOK_BACKTRACKSIZE TOK_LPAREN TOK_INTEGER TOK_INTEGER TOK_RPAREN
                  {
		    $$ = new MccBacktrackSize ($3, $4);
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


adjacencyCst:   TOK_ADJACENCY TOK_LPAREN fgRef flt flt TOK_RPAREN
                 {
		   $$ = new MccAdjacencyCstStat ($3, $4, $5);
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


clashCst:   TOK_RESCLASH TOK_LPAREN fgRef fixorvdw_opt flt atomset_opt atomsetopt_opt TOK_RPAREN
             {
	       $$ = new MccClashCstStat ($3, $4, $5, $6, $7);
	     }
;


fixorvdw_opt:   /* empty */ { $$ = false; }
              | fixorvdw { $$ = $1; }
;


fixorvdw:   TOK_FIXEDDIST { $$ = false; }
          | TOK_VDWDIST { $$ = true; }
;


cycleCst:   TOK_CYCLE TOK_LPAREN cycledef_plus TOK_RPAREN
             {
	       $$ = new MccCycleCstStat ($3);
	     }
;


cycledef_plus:   cycledef
                  {
		    $$ = new vector< MccCycleCstStat::_CycleStruc* > (1, $1);
		  }
               | cycledef_plus cycledef
                  {
		    $$ = $1;
		    $$->push_back ($2);
		  }
;


cycledef:   residueRef residueRef TOK_INTEGER flt
             {
	       $$ = new MccCycleCstStat::_CycleStruc ($1, $2, $3, $4);
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


newtag:   TOK_NEWTAG TOK_LPAREN resortfo TOK_STRING queryexp_plus TOK_RPAREN
           {
	     $$ = new MccNewTagStat ($3, $4, $5);
	   }
;


resortfo:   TOK_RES { $$ = true; }
          | TOK_TRANSFO { $$ = false; }
;


addpdb:   TOK_ADDPDB TOK_LPAREN addpdbdefs_plus TOK_RPAREN
           {
	     $$ = new MccAddPdbStat ($3);
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


addpdbdefs:   cutoff_opt pdbfile_plus
               {
		 $$ = new MccAddPdbStat::_AddPdbStruc ($1->ctc, $1->ccc, $2);
		 delete $1;
	       }
;


cutoff_opt:   /* empty */ { $$ = new cutoffs (); }
            | TOK_TFOCUT flt cfo_opt
               {
		 $$ = new cutoffs ($2, $3);
	       }
            | TOK_CONFOCUT flt tfo_opt
               {
		 $$ = new cutoffs ($3, $2);
	       }
;


cfo_opt:   /* empty */ { $$ = .5; }
         | TOK_CONFOCUT flt { $$ = $2; }
;


tfo_opt:   /* empty */ { $$ = .5; }
         | TOK_TFOCUT flt { $$ = $2; }
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
       | cacheexp { $$ = $1; }
       | libraryexp { $$ = $1; }
       | mosesexp { $$ = $1; }
;


backtrackexp: TOK_BACKTRACK TOK_LPAREN fgRef_opt res_place_plus TOK_RPAREN
               {
		 MccBacktrackExpr *tmp = new MccBacktrackExpr ();

		 if ($3)
		   tmp->GenFGStruc ($3);
		 tmp->AddBTStrucs ($4);
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


cacheexp:   TOK_CACHE TOK_LPAREN fgRef model_filter TOK_RPAREN
             {
	       $$ = new MccCacheExpr ($3, $4);
	     }
;


align_opt:   /* empty */ { $$ = false; }
           | TOK_ALIGN { $$ = true; }
;


libraryexp:   TOK_LIBRARY TOK_LPAREN input_mode model_sorter_opt libopt_star TOK_RPAREN
              {
		$$ = new MccLibraryExpr ($3, $4, $5);
	      }
;


input_mode:   TOK_PDB TOK_STRING  /* deprecated */
              {
		$$ = new MccFilePdbInput ($2);
	      }
            | TOK_FILE_BINARY TOK_LPAREN TOK_STRING TOK_RPAREN
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
;


model_sorter_opt: /* empty */ { $$ = 0; }
                  | model_sorter { $$ = $1; }
;


model_sorter:   TOK_CLUSTERED TOK_LPAREN TOK_INTEGER atomset_opt atomsetopt_opt TOK_RPAREN
                {
		  $$ = new MccClusteredModelSorterStrategy ($3, $4, $5);
		}
             | TOK_CLUSTERED TOK_LPAREN flt TOK_ANGSTROMS atomset_opt atomsetopt_opt TOK_RPAREN
                {
		  $$ = new MccClusteredModelSorterStrategy ((float)$3, $5, $6);
		}
;


libopt_star:   /* empty */
                {
		  $$ = new vector< MccLibraryExpr::_LibStruc* > ();
		}
             | libopt_star libopt
                {
		  $$ = $1;
		  $$->push_back ($2);
		}
;


libopt:   TOK_STRIP TOK_LPAREN residueRef_plus TOK_RPAREN
           {
	     $$ = new MccLibraryExpr::_StripStruc ($3);
	   }
        | TOK_CHANGEID TOK_LPAREN TOK_STRING TOK_COMMA TOK_STRING TOK_RPAREN
           {
	     $$ = new MccLibraryExpr::_ChangeIdStruc ($3[0], $5[0]);
	     delete[] $3;
	     delete[] $5;
	   }
;


mosesexp:  TOK_MOSES TOK_LPAREN resrange_opt ctfileid_opt mosesqueries mfold_epc_opt mfold_win_opt mfold_output_opt TOK_RPAREN
            {
	      $$ = new MccMosesExpr ($3, $4, $5, $6, $7, $8);
            }
;


resrange_opt:    /* empty */ { $$ = 0; }
               | TOK_RESNAME TOK_DASH TOK_RESNAME
                  {
		    char *str = new char[256];
		    sprintf (str, "%s-%s", $1, $3);
		    $$ = str;
		    delete $1;
		    delete $3;
		  }
               | TOK_INTEGER TOK_DASH TOK_INTEGER
                  {
		    char *str = new char[256];
		    sprintf (str, "%d-%d", $1, $3);
		    $$ = str;
		  }
;


ctfileid_opt:    /* empty */ { $$ = -1; }
               | TOK_CTFILEID TOK_LPAREN TOK_INTEGER TOK_RPAREN { $$ = $3; }
;


mosesqueries:  cgau_resdef_opt gu_resdef_opt loop_resdef_opt stem_condef_opt loop_condef_opt cgau_pairdef_opt gu_pairdef_opt 
                {
		  $$ = new MccMosesQueries ($1, $2, $3, $4, $5, $6, $7);
	        }
;


cgau_resdef_opt:          /* empty */ { $$ = 0; }
                      | TOK_CGAURESDEF queryexp sampling
                         {
			   $$ = new pair< MccQueryExpr*, MccSamplingSize* > ($2, $3);
			 }
;


gu_resdef_opt:      /* empty */ { $$ = 0; }
                      | TOK_GURESDEF queryexp sampling
                         {
			   $$ = new pair< MccQueryExpr*, MccSamplingSize* > ($2, $3);
			 }
;


loop_resdef_opt:       /* empty */ { $$ = 0; }
                      | TOK_LOOPRESDEF queryexp sampling
                         {
			   $$ = new pair< MccQueryExpr*, MccSamplingSize* > ($2, $3);
			 }
;


stem_condef_opt:       /* empty */ { $$ = 0; }
                      | TOK_STEMCONDEF queryexp sampling
                         {
			   $$ = new pair< MccQueryExpr*, MccSamplingSize* > ($2, $3);
			 }
;


loop_condef_opt:       /* empty */ { $$ = 0; }
                      | TOK_LOOPCONDEF queryexp sampling
                         {
			   $$ = new pair< MccQueryExpr*, MccSamplingSize* > ($2, $3);
			 }
;


cgau_pairdef_opt:       /* empty */ { $$ = 0; }
                      | TOK_CGAUPAIRDEF queryexp sampling
                         {
			   $$ = new pair< MccQueryExpr*, MccSamplingSize* > ($2, $3);
			 }
;


gu_pairdef_opt:         /* empty */ { $$ = 0; }
                      | TOK_GUPAIRDEF queryexp sampling
                         {
			   $$ = new pair< MccQueryExpr*, MccSamplingSize* > ($2, $3);
			 }
;


mfold_epc_opt:    /* empty */ { $$ = -1; }
                | TOK_MFOLD_EPC TOK_LPAREN TOK_INTEGER TOK_PERCENT TOK_RPAREN { $$ = $3; }
;


mfold_win_opt:    /* empty */ { $$ = -1; }
                | TOK_MFOLD_WIN TOK_LPAREN TOK_INTEGER TOK_RPAREN { $$ = $3; }
;


mfold_output_opt:    /* empty */ { $$ = 0; }
                   | TOK_MFOLD_OUTPUT TOK_LPAREN TOK_STRING TOK_RPAREN { $$ = $3; }
;

		  
atomset_opt:   /* empty */ { $$ = MCC_PSE_AS; }
             | TOK_ALLATOMS { $$ = MCC_ALL_AS; }
             | TOK_BASEATOMS { $$ = MCC_BASE_AS; }
             | TOK_BBATOMS { $$ = MCC_BACKBONE_AS; }
             | TOK_PSEATOMS { $$ = MCC_PSE_AS; }
;


atomsetopt_opt:   /* empty */ { $$ = MCC_NO_OPTION; }
                | TOK_NOHYDRO { $$ = MCC_NO_HYDROGEN; }
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
		 

residueRef_opt:   /* empty */ { $$ = 0; }
                | residueRef { $$ = $1; }
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


//! placer

placer_sequence:  TOK_PLACER_SEQUENCE TOK_LPAREN TOK_IDENT placer_residueRef ident_plus TOK_RPAREN
{
  if (strlen ($3) != 1)
    throw CParserException ("Invalid sequence type.");
  $$ = new MccPlacerSequenceStat ($3[0], $4, $5);
}
;

placer_connect:   TOK_PLACER_CONNECT TOK_LPAREN placer_condef_plus TOK_RPAREN
            {
	      $$ = new MccPlacerConnectStat ($3);
	    }
;


placer_condef_plus:   placer_condef
                {
		  $$ = new vector< MccPlacerConnectStat::_PlacerConnectStruc* > (1, $1);
		}
             | placer_condef_plus placer_condef
                {
		  $$ = $1;
		  $$->push_back ($2);
		}
;


placer_condef:   placer_residueRef placer_residueRef queryexp sampling
           {
	     $$ = new MccPlacerConnectStat::_PlacerConnectStruc ($1, $2, $3, $4);
	   }
;


placer_pair:   TOK_PLACER_PAIR TOK_LPAREN placer_pairdef_plus TOK_RPAREN
         {
	   $$ = new MccPlacerPairStat ($3);
	 }
;


placer_pairdef_plus:   placer_pairdef
                 {
		   $$ = new vector< MccPlacerPairStat::_PlacerPairStruc* > (1, $1);
		 }
             | placer_pairdef_plus placer_pairdef
                {
		  $$ = $1;
		  $$->push_back ($2);
		}
;


placer_pairdef:   placer_residueRef placer_residueRef queryexp sampling
            {
	      $$ = new MccPlacerPairStat::_PlacerPairStruc ($1, $2, $3, $4);
	    }
;


placer_residueRef_star:   /* empty */ { $$ = new vector< MccPlacerResidueName* > (); }
                 | placer_residueRef_star placer_residueRef
                    {
		      $$ = $1;
		      $$->push_back ($2);
		    }
;


placer_residueRef_plus:   placer_residueRef { $$ = new vector< MccPlacerResidueName* > (1, $1); }
                 | placer_residueRef_plus placer_residueRef
                    {
		      $$ = $1;
		      $$->push_back ($2);
		    }
;
		 

placer_residueRef_opt:   /* empty */ { $$ = 0; }
                | placer_residueRef { $$ = $1; }
;


placer_residueRef:   TOK_INTEGER { $$ = new MccPlacerResidueName ($1); }
            | TOK_RESNAME
               {
		 char tmp_char;
		 int tmp_int;
		 
		 sscanf ($1, "%c%d", &tmp_char, &tmp_int); 
		 $$ = new MccPlacerResidueName (tmp_char, tmp_int);
		 delete $1;
	       }
;


placer_fragment: TOK_PLACER_FRAGMENT TOK_LPAREN TOK_IDENT placer_residueRef input_mode TOK_RPAREN
{
  $$ = new MccPlacerFragmentStat ($3, $4, $5);
}
;


placer_build: TOK_PLACER_BUILD TOK_LPAREN clash_opt closure_opt ribose_opt /*fgRef_opt*/ placer_res_place_plus TOK_RPAREN
               {
		 MccPlacerBuildStat *tmp = new MccPlacerBuildStat ($3, $4, $5);

// 		 if ($3)
// 		   tmp->GenFGStruc ($3);
// 		 tmp->AddBTStrucs ($4);
		 tmp->AddBTStrucs ($6);
		 $$ = tmp;
	       }
;


clash_opt:  /* empty */
{
  $$ = new MccPlacerBuildStat::_ClashStruc (); 
}
| TOK_CLASH TOK_LPAREN TOK_IDENT flt TOK_RPAREN
{
  $$ = new MccPlacerBuildStat::_ClashStruc ($3[0], $4);
};


closure_opt: /* empty */
{
  $$ = new MccPlacerBuildStat::_ClosureStruc (); 
}
| TOK_CLOSURE TOK_LPAREN closure_mode_opt TOK_CLPREC TOK_CLPREC_C1P TOK_INTEGER TOK_PERCENT TOK_RPAREN
{
  $$ = new MccPlacerBuildStat::_ClosureStruc ($3, 'c', $6); 
}
;


closure_mode_opt:
{
  $$ = 'd';
}
| TOK_RMSD
{
  $$ = 'r';
}
| TOK_DISTANCE
{
  $$ = 'd';
}
;


ribose_opt: /* empty */
{
  $$ = new MccPlacerBuildStat::_RibStruc ();
}
| TOK_RIBOSE TOK_LPAREN ribose_value_opt TOK_ROPT5D ribose_qfct_opt TOK_RPAREN
{
  $$ = new MccPlacerBuildStat::_RibStruc ($3, '5', $5, -1, -1, -1);
}
| TOK_RIBOSE TOK_LPAREN ribose_value_opt TOK_ROPT5D flt flt flt ribose_qfct_opt TOK_RPAREN
{
  $$ = new MccPlacerBuildStat::_RibStruc ($3, '5', $8, $5, $6, $7);
}
| TOK_RIBOSE TOK_LPAREN ribose_value_opt TOK_ROPT2D ribose_qfct_opt TOK_RPAREN
{
  $$ = new MccPlacerBuildStat::_RibStruc ($3, '2', $5, -1, -1, -1);
}
| TOK_RIBOSE TOK_LPAREN ribose_value_opt TOK_ROPT2D flt flt flt ribose_qfct_opt TOK_RPAREN
{
  $$ = new MccPlacerBuildStat::_RibStruc ($3, '2', $8, $5, $6, $7);
}
| TOK_RIBOSE TOK_LPAREN ribose_value_opt TOK_REST ribose_qfct_opt TOK_RPAREN
{
  $$ = new MccPlacerBuildStat::_RibStruc ($3, 'o', $5, -1, -1, -1);
}
;


ribose_value_opt:
/* empty */
{
  $$ = -1;
}
| flt
{
  $$ = $1;
}
;


ribose_qfct_opt:
/* empty */
{
  $$ = 0;
}
| TOK_IDENT
{
  $$ = $1;
}
;


placer_res_place_plus:   placer_res_place
                   {
		     $$ = new vector< MccPlacerBuildStat::_GenBTStruc* > (1, $1);
		   }
                | placer_res_place_plus placer_res_place
                   {
		     $$ = $1;
		     $$->push_back ($2);
		   }
;


placer_res_place:  TOK_LPAREN placer_residueRef placer_residueRef_star TOK_RPAREN
{
  $$ = new MccPlacerBuildStat::_BTStruc ($2, $3);
}
| TOK_PLACE TOK_LPAREN TOK_IDENT placer_residueRef TOK_RPAREN
{
  $$ = new MccPlacerBuildStat::_PlaceStruc ($3, $4);
}
| TOK_PLACE TOK_LPAREN TOK_IDENT TOK_RPAREN
{
  $$ = new MccPlacerBuildStat::_PlaceStruc ($3);
}
;


placer_search:   TOK_PLACER_SEARCH TOK_LPAREN search_mode model_filter_opt output_mode_opt TOK_RPAREN
            {
	      $$ = new MccPlacerSearchStat ($3, $4, $5);
	    }
;


search_mode: TOK_IDENT TOK_LPAREN search_params_star TOK_RPAREN
{
  $$ = new MccPlacerSearchStat::_Params ($1, $3);
};


search_params_star: /* empty */
{
  $$ = new map< int, float > ();
}
| search_params_star search_params
{
  $$ = $1;
  $$->insert (*$2);
  delete $2;
};


search_params: TOK_IDENT flt
{
  $$ = new pair< int, float > (MccPlacerSearchStat::getParamId ($1), (float)$2);
};


//!

%%

int
mccerror (char *s)
{
  throw CParserException ("Parse error at line ") << mcclineno << ".";
}
