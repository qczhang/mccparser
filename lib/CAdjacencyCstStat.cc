//                              -*- mode: C++ -*- 
// CAdjacencyCstStat.cc
// Copyright © 1999, 2000 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Tue Sep 28 15:18:24 1999
// Last Modified By : Martin Larose
// Last Modified On : Mon Aug 21 13:24:14 2000
// Update Count     : 3
// Status           : Ok.
// 


#include <iostream.h>
#include <vector.h>

#include "CAdjacencyCstStat.h"

#include "CEnv.h"
#include "CConstraint.h"
#include "CDistCst.h"
#include "CFragGen.h"



/** Evaluates the adjacency constraint expression within the environvent
    env.  The constraint bag is added to the mvCst environment member.
*/

void
CAdjacencyCstStat::eval (CEnv &env)
{
  vector< CConstraint* > cst_bag;
  CFragGen *fg;

  fg = fg_struc->eval (env);

  cout << "Adjacency( " << the_min << ", " << the_max << " ) -> " 
       << fg->GetName () << endl;

  fg->Ready ();
  
  CDistCst::GenerateAdjacency (cst_bag, the_min, the_max, fg);
  env.CstInsert (cst_bag);
  env.AddCstBag (cst_bag);
}



/** Prints the adjacency form. */

void
CAdjacencyCstStat::display (ostream &os)
{
  os << "adjacency ( ";
  fg_struc->display (os);
  os << ' ' << the_min << ' ' << the_max
     << " )" << endl << endl;
}
