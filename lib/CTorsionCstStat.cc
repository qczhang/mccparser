//                              -*- Mode: C++ -*- 
// CTorsionCstStat.cc
// Copyright © 2000 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Wed Jun 28 14:55:53 2000
// Last Modified By : Martin Larose
// Last Modified On : Mon Aug 21 13:25:26 2000
// Update Count     : 3
// Status           : Ok.
// 


#include <iostream.h>
#include <math.h>

#include "UMcsymLib.h"

#include "CTorsionCstStat.h"

#include "CResidueStat.h"
#include "CConstraint.h"
#include "CEnv.h"
#include "CTorsionCst.h"
#include "CResidue.h"
#include "CAtom.h"
#include "CSymTable.h"
#include "CEvalException.h"
#include "UAngle.h"



CTorsionCstStat::_TorsionStruc::~_TorsionStruc ()
{
  delete res1;
  delete res2;
  delete res3;
  delete res4;
  delete[] at1;
  delete[] at2;
  delete[] at3;
  delete[] at4;
}



/** Evaluation method for the local class _TorsionStruc.  This method
    returns a CTorsionCst* constraint between four atoms.  The residue
    specified may be the same.  The values for the min and max angles are in
    degrees, we must convert them when we give them to the constructor.  */

CTorsionCst*
CTorsionCstStat::_TorsionStruc::eval (CEnv &env)
{
  CResidue *tmp_res1;
  CResidue *tmp_res2;
  CResidue *tmp_res3;
  CResidue *tmp_res4;
  const CAtom *a1;
  const CAtom *a2;
  const CAtom *a3;
  const CAtom *a4;
  const CSymbol *tmp_sym;

  tmp_res1 = res1->eval (env);
  tmp_res2 = res2->eval (env);
  tmp_res3 = res3->eval (env);
  tmp_res4 = res4->eval (env);
  
  tmp_sym = gSymTable.GetSymbol (at1, "ATOM");
  if (tmp_sym == 0)
    {
      CEvalException exc ("Invalid atom type ");

      throw exc << tmp_res1->GetChainId () << tmp_res1->GetResNo ()
		<< ":" << at1 << " in torsion constraint.";
    }
  a1 = tmp_res1->GetAtomPtr (tmp_sym);
  if (a1 == 0)
    {
      CEvalException exc ("Residue ");

      throw exc << tmp_res1->GetChainId () << tmp_res1->GetResNo ()
		<< " in torsion constraint does not contain atom " << at1
		<< ".";
    }
  
  tmp_sym = gSymTable.GetSymbol (at2, "ATOM");
  if (tmp_sym == 0)
    {
      CEvalException exc ("Invalid atom type ");

      throw exc << tmp_res2->GetChainId () << tmp_res2->GetResNo ()
		<< ":" << at2 << " in torsion constraint.";
    }
  a2 = tmp_res2->GetAtomPtr (tmp_sym);
  if (a2 == 0)
    {
      CEvalException exc ("Residue ");

      throw exc << tmp_res2->GetChainId () << tmp_res2->GetResNo ()
		<< " in torsion constraint does not contain atom " << at2
		<< ".";
    }
  
  tmp_sym = gSymTable.GetSymbol (at3, "ATOM");
  if (tmp_sym == 0)
    {
      CEvalException exc ("Invalid atom type ");

      throw exc << tmp_res3->GetChainId () << tmp_res3->GetResNo ()
		<< ":" << at3 << " in torsion constraint.";
    }
  a3 = tmp_res3->GetAtomPtr (tmp_sym);
  if (a3 == 0)
    {
      CEvalException exc ("Residue ");

      throw exc << tmp_res3->GetChainId () << tmp_res3->GetResNo ()
		<< " in torsion constraint does not contain atom " << at3
		<< ".";
    }
  
  tmp_sym = gSymTable.GetSymbol (at4, "ATOM");
  if (tmp_sym == 0)
    {
      CEvalException exc ("Invalid atom type ");

      throw exc << tmp_res4->GetChainId () << tmp_res4->GetResNo ()
		<< ":" << at4 << " in torsion constraint.";
    }
  a4 = tmp_res4->GetAtomPtr (tmp_sym);
  if (a4 == 0)
    {
      CEvalException exc ("Residue ");

      throw exc << tmp_res4->GetChainId () << tmp_res4->GetResNo ()
		<< " in torsion constraint does not contain atom " << at4
		<< ".";
    }
  
  return new CTorsionCst (tmp_res1, tmp_res2, tmp_res3, tmp_res4,
			  a1->GetType (), a2->GetType (),
			  a3->GetType (), a4->GetType (),
			  ToRadian (dist_min), ToRadian (dist_max));
}



/** Prints the local class _TorsionStruc.  */

void
CTorsionCstStat::_TorsionStruc::display (ostream &os)
{
  res1->display (os);
  os << ":" << at1 << ' ';
  res2->display (os);
  os << ":" << at2 << ' ';
  res3->display (os);
  os << ":" << at3 << ' ';
  res4->display (os);
  os << ":" << at4 << ' ' << dist_min << ' ' << dist_max;
}



CTorsionCstStat::~CTorsionCstStat ()
{
  vector< _TorsionStruc* >::iterator it;

  for (it = strucs->begin (); it != strucs->end (); it++)
    delete *it;
  delete strucs;
}



/** Evaluation method for the "torsion" statement.  */

void
CTorsionCstStat::eval (CEnv &env)
{
  vector< CConstraint* > cst_bag;
  vector< _TorsionStruc* >::iterator it;

  for (it = strucs->begin (); it != strucs->end (); ++it)
    cst_bag.push_back ((*it)->eval (env));
  env.AddCstBag (cst_bag);
}



/** Prints the "torsion" statement.  */

void
CTorsionCstStat::display (ostream &os)
{
  vector< _TorsionStruc* >::iterator it;

  os << "torsion ( ";
  for (it = strucs->begin (); it != strucs->end (); ++it)
    {
      os << endl << "  ";
      (*it)->display (os);
    }
  os << endl << ")" << endl << endl;
}
