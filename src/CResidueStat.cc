//                              -*- mode: C++ -*- 
// CResidueStat.cc
// Copyright © 1999, 2000 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Wed Sep 22 21:14:44 1999
// Last Modified By : Martin Larose
// Last Modified On : Mon Aug 21 13:25:12 2000
// Update Count     : 5
// Status           : Ok.
// 


#include <iostream.h>
#include <iomanip.h>
#include <stdio.h>

#include "CResidueStat.h"

#include "CEnv.h"
#include "CQuery.h"
#include "CResQ.h"
#include "CSet.h"
#include "CSetCache.h"
#include "CConfoFG.h"
#include "CResHolder.h"
#include "CEvalException.h"



/** Evaluation method for the local class _ResidueStruc.  It generates some
    CConfoFG that are added to the environment Fragment Generator
    library.  */

void
CResidueStat::_ResidueStruc::eval (CEnv &env)
{
  CResidue *resi1;
  CResidue *resi2 = 0;
  CResidue *tmp_res;
  CResQ resq;
  CSet< CResL* > tmp_set;
  int orig_size;
  CQuery *restrict;
  
  resi1 = res1->eval (env);
  if (res2)
    resi2 = res2->eval (env);
  restrict = expr->eval (env);
  tmp_res = resi1;
  while (tmp_res)
    {
      char fg_name[256];
      
      resq.Reset ();
      resq.NewType (tmp_res->GetType ());
      resq.Restrict (restrict);
      sprintf (fg_name, "%c%d", tmp_res->GetChainId (), tmp_res->GetResNo ());
      tmp_set = env.GetSet (resq, true, ssize);
      orig_size = tmp_set.size ();
      tmp_set = tmp_set.First (ssize);
      env.SaveCache ();
      CConfoFG *tmp_FG = new CConfoFG (fg_name, tmp_res, tmp_set);
      env.AddFG (tmp_FG);

      cout << setw (4) << ssize << " / " << setw (4) << orig_size
	   << "   Created residue " << fg_name << "." << endl;
      
      if (tmp_res == resi2 || !resi2)
	tmp_res = 0;
      else
	{
	  CResidue *tmp = env.GetHold ().GetPtr (tmp_res->GetResNo () + 1,
						 tmp_res->GetChainId ());
	  if (!tmp)
	    throw CEvalException ("Invalid residue reference ")
	      << tmp_res->GetChainId () << tmp_res->GetResNo () + 1
	      << " in residue statement.";
	  tmp_res = tmp;
	}
    }
  delete restrict;
}



/** Prints the substatement _ResidueStruc.  */

void
CResidueStat::_ResidueStruc::display (ostream &os)
{
  res1->display (os);
  os << ' ';
  if (res2)
    {
      res2->display (os);
      os << ' ';
    }
  expr->display (os);
  os << ' ' << ssize;
}



/** Destructor for the statement "residue".  */

CResidueStat::~CResidueStat ()
{
  vector< _ResidueStruc* >::iterator it;

  for (it = strucs->begin (); it != strucs->end (); it++)
    delete *it;
  delete strucs;
}



/** Evaluation method for the statement "residue".  */

void
CResidueStat::eval (CEnv &env)
{
  vector< _ResidueStruc* >::iterator it;

  for (it = strucs->begin (); it != strucs->end (); it++)
    (*it)->eval (env);
}



/** Prints the statement "residue".  */

void
CResidueStat::display (ostream &os)
{
  vector< _ResidueStruc* >::iterator it;
  
  os << "residue ( ";
  for (it = strucs->begin (); it != strucs->end (); it++)
    {
      if (it != strucs->begin ())
	os << "          ";
      (*it)->display (os);
      os << endl;
    }
  os << ')' << endl << endl;;
}
