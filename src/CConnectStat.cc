//                              -*- mode: C++ -*- 
// CConnectStat.cc
// Copyright © 1999, 2000 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Thu Sep 23 11:58:44 1999
// Last Modified By : Martin Larose
// Last Modified On : Mon Aug 21 13:24:24 2000
// Update Count     : 9
// Status           : Ok.
// 



#include <iostream.h>
#include <iomanip.h>
#include <stdio.h>

#include "UMcsymLib.h"

#include "CConnectStat.h"

#include "CEnv.h"
#include "CQuery.h"
#include "CResidue.h"
#include "CSet.h"
#include "CTfoQ.h"
#include "CSymbolMgr.h"
#include "CSymTable.h"
#include "CTransfoFG.h"
#include "CResHolder.h"
#include "CEvalException.h"



/** Evaluation method for the substructure in the "connect" statement.  It
    adds some TfoQ between a sequence of residues.  */

void
CConnectStat::_ConnectStruc::eval (CEnv &env)
{
  CResidue *tmp_ref;
  CResidue *tmp_res;
  CResidue *last_res;
  int orig_size;
  CSet< CTfoL* > tmp_set;
  CTfoQ tfoq1;
  CTfoQ tfoq2;
  CTransfoFG *tmp_FG;
  CQuery *restrict;

  tmp_ref = res1->eval (env);
  last_res = res2->eval (env);
  restrict = expr->eval (env);

  tmp_res = env.GetHold ().GetPtr (tmp_ref->GetResNo () + 1,
				   tmp_ref->GetChainId ());
  if (!tmp_res)
    throw CEvalException ("Invalid residue reference ")
      << tmp_ref->GetChainId () << tmp_ref->GetResNo () + 1
      << " in connect statement.";
  while (tmp_ref)
    {
      tfoq1.Reset ();
      tfoq2.Reset ();

      gSymTable.SetContext ("DATABASE");

      tfoq1.Restrict (gSymTable ("adjacent"));
      tfoq2.Restrict (gSymTable ("adjacent"));

      if (tmp_ref->GetResNo () < tmp_res->GetResNo ())
	{
	  tfoq1.NewDirection (gSymTable ("DIR_5'"));
	  tfoq2.NewDirection (gSymTable ("DIR_3'"));
	}
      else
	{
	  tfoq1.NewDirection (gSymTable ("DIR_3'"));
	  tfoq2.NewDirection (gSymTable ("DIR_5'"));
	}
      
      const CSymbol* base_type = gSymTable.GetSymbol (tmp_ref->GetType (),
						      "RES_OUTPUT");
      const CSymbol* dna1_type;
      const CSymbol* rna1_type;
      const CSymbol* dna2_type;
      const CSymbol* rna2_type;
      
      if (base_type == gRES_U || base_type == gRES_T || base_type == gRES_C)
	{
	  dna1_type = gSymTable.GetSymbol (gRES_C, "RES_DNA");
	  rna1_type = gSymTable.GetSymbol (gRES_C, "RES_RNA");
	  dna2_type = gSymTable.GetSymbol (gRES_T, "RES_DNA");
	  rna2_type = gSymTable.GetSymbol (gRES_U, "RES_RNA");
	}
      else
	{
	  dna1_type = gSymTable.GetSymbol (gRES_G, "RES_DNA");
	  rna1_type = gSymTable.GetSymbol (gRES_G, "RES_RNA");
	  dna2_type = gSymTable.GetSymbol (gRES_A, "RES_DNA");
	  rna2_type = gSymTable.GetSymbol (gRES_A, "RES_RNA");
	}
	  
      tfoq1.NewRefType (rna1_type);
      tfoq1.NewRefType (dna1_type);
      tfoq1.NewRefType (rna2_type);
      tfoq1.NewRefType (dna2_type);
      tfoq2.NewResType (rna1_type);
      tfoq2.NewResType (dna1_type);
      tfoq2.NewResType (rna2_type);
      tfoq2.NewResType (dna2_type);
    
      base_type = gSymTable.GetSymbol (tmp_res->GetType(), "RES_OUTPUT");

      if (base_type == gRES_U || base_type == gRES_T || base_type == gRES_C)
	{
	  dna1_type = gSymTable.GetSymbol (gRES_C, "RES_DNA");
	  rna1_type = gSymTable.GetSymbol (gRES_C, "RES_RNA");
	  dna2_type = gSymTable.GetSymbol (gRES_T, "RES_DNA");
	  rna2_type = gSymTable.GetSymbol (gRES_U, "RES_RNA");
	}
      else
	{
	  dna1_type = gSymTable.GetSymbol (gRES_G, "RES_DNA");
	  rna1_type = gSymTable.GetSymbol (gRES_G, "RES_RNA");
	  dna2_type = gSymTable.GetSymbol (gRES_A, "RES_DNA");
	  rna2_type = gSymTable.GetSymbol (gRES_A, "RES_RNA");
	}
	  
      tfoq1.NewResType (rna1_type);
      tfoq1.NewResType (dna1_type);
      tfoq1.NewResType (rna2_type);
      tfoq1.NewResType (dna2_type);
      tfoq2.NewRefType (rna1_type);
      tfoq2.NewRefType (dna1_type);
      tfoq2.NewRefType (rna2_type);
      tfoq2.NewRefType (dna2_type);

      tfoq1.Restrict (restrict);
      tfoq2.Restrict (restrict);

      char fg_name[256];
      char fin_FG[256];
      char rfg_name[256];
      char rfin_FG[256];
      unsigned int tmp_size;
      int tmp_orig_size;

      sprintf (fin_FG, "%c%d", tmp_res->GetChainId (), tmp_res->GetResNo ());
      sprintf (rfin_FG, "%c%d", tmp_ref->GetChainId (), tmp_ref->GetResNo ());
      sprintf (fg_name, "%s -> %s", rfin_FG , fin_FG);
      sprintf (rfg_name, "%s -> %s", fin_FG, rfin_FG);
      
      tmp_set = env.GetSet (tfoq1, true, ssize);
      tmp_orig_size = orig_size = tmp_set.size ();
      tmp_set = tmp_set.First (ssize);
      tmp_size = tmp_set.size ();
      env.SaveCache ();
      tmp_FG = new CTransfoFG (fg_name, tmp_ref, tmp_res, env.GetFG (fin_FG),
			       tmp_set);
      env.AddFG (tmp_FG);

      tmp_set = env.GetSet (tfoq2, true, ssize);
      orig_size = tmp_set.size ();
      tmp_set = tmp_set.First (ssize);
      env.SaveCache ();
      cout << setw (4) << tmp_size << " / " << setw (4) << tmp_orig_size
	   << "   " << setw (4) << tmp_set.size () << " / " << setw (4)
	   << orig_size << "   ";
      tmp_FG = new CTransfoFG (rfg_name, tmp_res, tmp_ref,
			       env.GetFG (rfin_FG), tmp_set);
      env.AddFG (tmp_FG);

      cout << "Created connect between " << rfin_FG << " and "
	   << fin_FG << endl;
      
      if (tmp_res == last_res)
	tmp_ref = 0;
      else
	{
	  tmp_ref = tmp_res;
	  tmp_res = env.GetHold ().GetPtr (tmp_ref->GetResNo () + 1,
					   tmp_ref->GetChainId ());
	  if (!tmp_res)
	    throw CEvalException ("Invalid residue reference ")
	      << tmp_ref->GetChainId () << tmp_ref->GetResNo () + 1
	      << " in connect statement.";
	}
    }
  delete restrict;
}



/** Prints the local structure of the "connect" statement.  */

void
CConnectStat::_ConnectStruc::display (ostream &os)
{
  res1->display (os);
  os << ' ';
  res2->display (os);
  os << ' ';
  expr->display (os);
  os << ' ' << ssize;
}



/** Destructor for the "connect" statement.  */

CConnectStat::~CConnectStat ()
{
  vector< _ConnectStruc* >::iterator it;

  for (it = strucs->begin (); it != strucs->end (); it++)
    delete *it;

  delete strucs;
}



/** Evaluation method for the "connect" statement.  */

void
CConnectStat::eval (CEnv &env)
{
  vector< _ConnectStruc* >::iterator it;

  for (it = strucs->begin (); it != strucs->end (); it++)
    (*it)->eval (env);
}



/** Prints the "connect" statement.  */

void
CConnectStat::display (ostream &os)
{
  vector< _ConnectStruc* >::iterator it;

  os << "connect ( ";
  for (it = strucs->begin (); it != strucs->end (); it++)
    {
      if (it != strucs->begin ())
	os << "          ";
      (*it)->display (os);
      os << endl;
    }
  os << ')' << endl << endl;
}
