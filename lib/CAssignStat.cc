//                              -*- mode: C++ -*- 
// CAssignStat.cc
// Copyright © 1999 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Wed Sep 22 20:46:56 1999
// Last Modified By : Martin Larose
// Last Modified On : Fri Oct  8 14:30:08 1999
// Update Count     : 1
// Status           : Ok.
// 


#include <map.h>
#include <vector.h>
#include <string.h>

#include "CAssignStat.h"

#include "CEnv.h"
#include "CFragGen.h"
#include "CResidue.h"



/** Evaluates the assignment statement node. */

void
CAssignStat::eval (CEnv &env)
{
  CFragGen *fg = expr->eval (env);
  
  fg->SetName (ident);
  env.AddFG (fg);
  cout << "Inserted fragGen {" << ident << "} into the lib." << endl;
}



/** Displays the assignment statement node. */

void
CAssignStat::display (ostream &os)
{
  os << ident << " = ";
  expr->display (os);
  os << endl << endl;
}
