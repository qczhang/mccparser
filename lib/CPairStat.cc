//                              -*- mode: C++ -*- 
// CPairStat.cc
// Copyright © 1999, 2000 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Wed Sep 22 21:44:31 1999
// Last Modified By : Martin Larose
// Last Modified On : Mon Aug 21 13:25:03 2000
// Update Count     : 6
// Status           : Ok.
// 


#include <iostream.h>
#include <iomanip.h>
#include <stdio.h>

#include "UMcsymLib.h"

#include "CPairStat.h"

#include "CQuery.h"
#include "CEnv.h"
#include "CTfoQ.h"
#include "CSet.h"
#include "CSetCache.h"
#include "CTransfoFG.h"
#include "CDatabase.h"
#include "CResidue.h"
#include "CSymbolMgr.h"
#include "CSymTable.h"



/** Evaluation method for the local class _PairStruc.  */

void
CPairStat::_PairStruc::eval (CEnv &env)
{
  CResidue *resi;
  CResidue *refe;
  int orig_size;
  CTfoQ tfoq1;
  CTfoQ tfoq2;
  CSet< CTfoL* > tmp_set;
  CTransfoFG *tmp_FG;
  CQuery *restrict;
  char fg_name[256];
  char fin_fg[256];
  char rfg_name[256];
  char rfin_fg[256];
  unsigned int tmp_size;
  int tmp_orig_size;

  resi = res->eval (env);
  refe = ref->eval (env);
  restrict = expr->eval (env);
  
  tfoq1.Reset ();
  tfoq2.Reset ();

  const CSymbol* base_type = gSymTable.GetSymbol (refe->GetType (),
						  "RES_OUTPUT");
  const CSymbol* dna_type;
  const CSymbol* rna_type;

  if (base_type == gRES_U) 
    {
      rna_type = gSymTable.GetSymbol (gRES_U, "RES_RNA");
      tfoq1.NewRefType (rna_type);
      tfoq2.NewResType (rna_type);
    } 
  else if (base_type == gRES_T)
    {
      dna_type = gSymTable.GetSymbol (gRES_T, "RES_DNA");
      tfoq1.NewRefType (dna_type);
      tfoq2.NewResType (dna_type);
    }
  else
    {
      dna_type = gSymTable.GetSymbol (base_type, "RES_DNA");
      rna_type = gSymTable.GetSymbol (base_type, "RES_RNA");
      tfoq1.NewRefType (rna_type);
      tfoq1.NewRefType (dna_type);
      tfoq2.NewResType (rna_type);
      tfoq2.NewResType (dna_type);
    }
  
  base_type = gSymTable.GetSymbol (resi->GetType (), "RES_OUTPUT");

  if (base_type == gRES_U)
    {
      rna_type = gSymTable.GetSymbol (gRES_U, "RES_RNA");
      tfoq1.NewResType (rna_type);
      tfoq2.NewRefType (rna_type);
    }
  else if (base_type == gRES_T)
    {
      dna_type = gSymTable.GetSymbol (gRES_T, "RES_DNA");
      tfoq1.NewResType (dna_type);
      tfoq2.NewRefType (dna_type);
    }
  else
    {
      dna_type = gSymTable.GetSymbol (base_type, "RES_DNA");
      rna_type = gSymTable.GetSymbol (base_type, "RES_RNA");
      tfoq1.NewResType (rna_type);
      tfoq1.NewResType (dna_type);
      tfoq2.NewRefType (rna_type);
      tfoq2.NewRefType (dna_type);
    }

  tfoq1.Restrict (restrict);
  tfoq2.Restrict (restrict);

  sprintf (fg_name, "%c%d -> %c%d", refe->GetChainId (), refe->GetResNo (),
	   resi->GetChainId (), resi->GetResNo ());
  sprintf (fin_fg, "%c%d", resi->GetChainId (), resi->GetResNo ());

  tmp_set = env.GetSet (tfoq1, true, ssize);
  orig_size = tmp_set.size ();
  tmp_set = tmp_set.First (ssize);
  env.SaveCache ();
  tmp_size = tmp_set.size ();
  tmp_orig_size = orig_size;
  
  tmp_FG = new CTransfoFG (fg_name, refe, resi, env.GetFG (fin_fg), tmp_set);
  env.AddFG (tmp_FG);

  sprintf (rfg_name, "%c%d -> %c%d", resi->GetChainId (), resi->GetResNo (),
	   refe->GetChainId (), refe->GetResNo ());
  sprintf (rfin_fg, "%c%d", refe->GetChainId (), refe->GetResNo ());
  
  tmp_set = env.GetSet (tfoq2, true, ssize);
  orig_size = tmp_set.size ();
  tmp_set = tmp_set.First (ssize);
  env.SaveCache ();
  cout << setw (4) << tmp_size << " / " << setw (4) << tmp_orig_size
       << "   " << setw (4) << tmp_set.size () << " / " << setw (4)
       << orig_size << "   ";
  tmp_FG = new CTransfoFG (rfg_name, resi, refe, env.GetFG (rfin_fg), tmp_set);
  env.AddFG (tmp_FG);

  cout << "Created pairing between " << rfin_fg << " and " << fin_fg << endl;
  delete restrict;
}



/** Prints the substatement _PairStruc.  */

void
CPairStat::_PairStruc::display (ostream &os)
{
  res->display (os);
  os << ' ';
  ref->display (os);
  os << ' ';
  expr->display (os);
  os << ' ' << ssize;
}



/** Destructor for the "pair" statement.  */

CPairStat::~CPairStat ()
{
  vector< _PairStruc* >::iterator it;

  for (it = strucs->begin (); it != strucs->end (); it++)
    delete *it;
  delete strucs;
}



/** Evaluation method for the "pair" statement.  It defines the relations
    between two residues that are not adjacent in the sequence.  */

void
CPairStat::eval (CEnv &env)
{
  vector< _PairStruc* >::iterator it;

  for (it = strucs->begin (); it != strucs->end (); it++)
    (*it)->eval (env);
}



/** Prints the statement "pair".  */

void
CPairStat::display (ostream &os)
{
  vector< _PairStruc* >::iterator it;
  
  os << "pair ( ";
  for (it = strucs->begin (); it != strucs->end (); it++)
    {
      if (it != strucs->begin ())
	os << "       ";
      (*it)->display (os);
      os << endl;
    }
  os << ')' << endl << endl;
}
