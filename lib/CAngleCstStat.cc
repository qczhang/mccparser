//                              -*- Mode: C++ -*- 
// CAngleCstStat.cc
// Copyright © 2000 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Thu Jan 20 15:58:06 2000
// Last Modified By : Martin Larose
// Last Modified On : Mon Aug 21 13:24:19 2000
// Update Count     : 6
// Status           : Unknown.
// 


#include <iostream.h>
#include <math.h>

#include "UMcsymLib.h"

#include "CAngleCstStat.h"

#include "CResidueName.h"
#include "CConstraint.h"
#include "CEnv.h"
#include "CAngleCst.h"
#include "CResidue.h"
#include "CAtom.h"
#include "CSymTable.h"
#include "CEvalException.h"
#include "UAngle.h"


CAngleCstStat::_AngleStruc::~_AngleStruc ()
{
  delete res1;
  delete[] at1;
  delete res2;
  delete[] at2;
  delete res3;
  delete[] at3;
}



/** Evaluation method for the local class _AngleStruc.  This method returns a
    CAngleCst* constraint between two atoms.  The residue specified may be
    the same.  The values for the min and max angles are in degrees, we must
    convert them when we give them to the constructor.
*/

CAngleCst*
CAngleCstStat::_AngleStruc::eval (CEnv &env)
{
  CResidue *tmp_res1;
  CResidue *tmp_res2;
  CResidue *tmp_res3;
  const CAtom *a1;
  const CAtom *a2;
  const CAtom *a3;
  const CSymbol *tmp_sym;

  // Get the res pointer from the resholder in the env.
  
  tmp_res1 = res1->eval (env);
  tmp_res2 = res2->eval (env);
  tmp_res3 = res3->eval (env);

  // Test the atoms
  
  tmp_sym = gSymTable.GetSymbol (at1, "ATOM");
  if (tmp_sym == 0)
    {
      CEvalException exc ("Invalid atom type ");

      throw exc << tmp_res1->GetChainId () << tmp_res1->GetResNo ()
		<< ":" << at1 << " in angle constraint.";
    }
  a1 = tmp_res1->GetAtomPtr (tmp_sym);
  if (a1 == 0)
    {
      CEvalException exc ("Residue ");

      throw exc << tmp_res1->GetChainId () << tmp_res1->GetResNo ()
		<< " in angle constraint does not contain atom " << at1
		<< ".";
    }
  
  tmp_sym = gSymTable.GetSymbol (at2, "ATOM");
  if (tmp_sym == 0)
    {
      CEvalException exc ("Invalid atom type ");

      throw exc << tmp_res2->GetChainId () << tmp_res2->GetResNo ()
		<< ":" << at2 << " in angle constraint.";
    }
  a2 = tmp_res2->GetAtomPtr (tmp_sym);
  if (a2 == 0)
    {
      CEvalException exc ("Residue ");

      throw exc << tmp_res2->GetChainId () << tmp_res2->GetResNo ()
		<< " in angle constraint does not contain atom " << at2
		<< ".";
    }
  
  tmp_sym = gSymTable.GetSymbol (at3, "ATOM");
  if (tmp_sym == 0)
    {
      CEvalException exc ("Invalid atom type ");

      throw exc << tmp_res3->GetChainId () << tmp_res3->GetResNo ()
		<< ":" << at3 << " in angle constraint.";
    }
  a3 = tmp_res3->GetAtomPtr (tmp_sym);
  if (a3 == 0)
    {
      CEvalException exc ("Residue ");

      throw exc << tmp_res3->GetChainId () << tmp_res3->GetResNo ()
		<< " in angle constraint does not contain atom " << at3
		<< ".";
    }
  
  return new CAngleCst (tmp_res1, tmp_res2, tmp_res3, a1->GetType (),
			a2->GetType (), a3->GetType (),
			ToRadian (dist_min), ToRadian (dist_max));
}



/** Prints the local class _AngleStruc.  */

void
CAngleCstStat::_AngleStruc::display (ostream &os)
{
  res1->display (os);
  os << ":" << at1 << ' ';
  res2->display (os);
  os << ":" << at2 << ' ';
  res3->display (os);
  os << ":" << at3 << ' ' << dist_min << ' ' << dist_max;
}



/** Destructor for the CAngleCstStat.  */

CAngleCstStat::~CAngleCstStat ()
{
  vector< _AngleStruc* >::iterator it;

  for (it = strucs->begin (); it != strucs->end (); it++)
    delete *it;
  delete strucs;
}



/** Evaluation method for the "angle" statement.  */

void
CAngleCstStat::eval (CEnv &env)
{
  vector< CConstraint* > cst_bag;
  vector< _AngleStruc* >::iterator it;

  for (it = strucs->begin (); it != strucs->end (); it++)
    cst_bag.push_back ((*it)->eval (env));
  env.AddCstBag (cst_bag);
}



/** Prints the "angle" statement.  */

void
CAngleCstStat::display (ostream &os)
{
  vector< _AngleStruc* >::iterator it;

  os << "angle ( ";
  for (it = strucs->begin (); it != strucs->end (); it++)
    {
      os << endl << "  ";
      (*it)->display (os);
    }
  os << endl << ")" << endl << endl;
}

