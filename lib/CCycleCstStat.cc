//                              -*- mode: C++ -*- 
// CCycleCstStat.cc
// Copyright © 1999, 2000 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Fri Sep 24 09:22:03 1999
// Last Modified By : Martin Larose
// Last Modified On : Mon Aug 21 13:24:27 2000
// Update Count     : 2
// Status           : Ok.
// 



#include <iostream.h>

#include "CCycleCstStat.h"

#include "CEnv.h"
#include "CResidue.h"
#include "CCycleCst.h"



/** Evaluation function for the "cycle" statement.  It creates a CCycleCst
    constraint and adds it to the environment CstBag.
*/

void
CCycleCstStat::eval (CEnv &env)
{
  CResidue *tmp_res1;
  CResidue *tmp_res2;
  CCycleCst *cst;

  tmp_res1 = res1->eval (env);
  tmp_res2 = res2->eval (env);

  cst = new CCycleCst (tmp_res1, tmp_res2, nb, dist);
  cout << "cycle ( " << tmp_res1->GetChainId () << tmp_res1->GetResNo ()
       << ", " << tmp_res2->GetChainId () << tmp_res2->GetResNo ()
       << " )" << endl;
  env.AddCst (cst);
}



/** Prints the "cycle" statement.  */

void
CCycleCstStat::display (ostream &os)
{
  os << "cycle ( ";
  res1->display (os);
  os << " ";
  res2->display (os);
  os << " " << nb << " " << dist << " )" << endl;
}
