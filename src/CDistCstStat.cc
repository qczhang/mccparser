//                              -*- mode: C++ -*- 
// CDistCstStat.cc
// Copyright © 1999, 2000 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Fri Sep 24 08:50:55 1999
// Last Modified By : Martin Larose
// Last Modified On : Mon Aug 21 13:24:38 2000
// Update Count     : 3
// Status           : Ok.
// 



#include <iostream.h>

#include "UMcsymLib.h"

#include "CDistCstStat.h"

#include "CConstraint.h"
#include "CEnv.h"
#include "CDistCst.h"
#include "CResidue.h"
#include "CAtom.h"
#include "CSymTable.h"
#include "CEvalException.h"



/** Evaluation method for the local class _DistStruc.  This method returns a
    CDistCst* constraint between two atoms.  The residue specified may be
    the same.
*/

CDistCst*
CDistCstStat::_DistStruc::eval (CEnv &env)
{
  CResidue *tmp_res1;
  CResidue *tmp_res2;
  const CAtom *a1;
  const CAtom *a2;
  const CSymbol *tmp_sym;

  // Get the res pointer from the resholder in the env.
  
  tmp_res1 = res1->eval (env);
  tmp_res2 = res2->eval (env);

  // Test the atom types
  
  tmp_sym = gSymTable.GetSymbol (at1, "ATOM");
  if (tmp_sym == 0)
    {
      CEvalException exc ("Invalid atom type ");

      throw exc << tmp_res1->GetChainId () << tmp_res1->GetResNo ()
		<< ":" << at1 << " in distance constraint.";
    }
  a1 = tmp_res1->GetAtomPtr (tmp_sym);
  if (a1 == 0)
    {
      CEvalException exc ("Residue ");

      throw exc << tmp_res1->GetChainId () << tmp_res1->GetResNo ()
		<< " in distance constraint does not contain atom " << at1
		<< ".";
    }
  
  tmp_sym = gSymTable.GetSymbol (at2, "ATOM");
  if (tmp_sym == 0)
    {
      CEvalException exc ("Invalid atom type ");

      throw exc << tmp_res2->GetChainId () << tmp_res2->GetResNo ()
		<< ":" << at2 << " in distance constraint.";
    }
  a2 = tmp_res2->GetAtomPtr (tmp_sym);
  if (a2 == 0)
    {
      CEvalException exc ("Residue ");

      throw exc << tmp_res2->GetChainId () << tmp_res2->GetResNo ()
		<< " in distance constraint does not contain atom " << at2
		<< ".";
    }
  
  return new CDistCst (tmp_res1, tmp_res2, a1->GetType (), a2->GetType (),
		       dist_min, dist_max);
}



/** Prints the local class _DistStruc.  */

void
CDistCstStat::_DistStruc::display (ostream &os)
{
  res1->display (os);
  os << ":" << at1 << ' ';
  res2->display (os);
  os << ":" << at2 << " " << dist_min << " " << dist_max;
}



/** Destructor for the CDistCstStat.  */

CDistCstStat::~CDistCstStat ()
{
  vector< _DistStruc* >::iterator it;

  for (it = strucs->begin (); it != strucs->end (); it++)
    delete *it;
  delete strucs;
}



/** Evaluation method for the "distance" statement.  */

void
CDistCstStat::eval (CEnv &env)
{
  vector< CConstraint* > cst_bag;
  vector< _DistStruc* >::iterator it;

  for (it = strucs->begin (); it != strucs->end (); it++)
    cst_bag.push_back ((*it)->eval (env));
  env.AddCstBag (cst_bag);
}



/** Prints the "distance" statement.  */

void
CDistCstStat::display (ostream &os)
{
  vector< _DistStruc* >::iterator it;

  os << "distance ( ";
  for (it = strucs->begin (); it != strucs->end (); it++)
    {
      os << endl << "  ";
      (*it)->display (os);
    }
  os << endl << ")" << endl << endl;
}
