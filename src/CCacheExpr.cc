//                              -*- mode: C++ -*- 
// CCacheExpr.cc
// Copyright © 1999, 2000 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Fri Sep 24 07:32:29 1999
// Last Modified By : Martin Larose
// Last Modified On : Mon Aug 21 13:24:22 2000
// Update Count     : 3
// Status           : Ok.
// 


#include <iostream.h>

#include "CCacheExpr.h"

#include "CEnv.h"
#include "CFragGen.h"
#include "CCacheFG.h"



/** Evaluation function for the expression "cache".  This method returns a
    cache FragGen.
*/

CFragGen*
CCacheExpr::eval (CEnv &env)
{
  CFragGen *cached_frag_gen;

  cached_frag_gen = fgref->eval (env);
  return new CCacheFG ("", *cached_frag_gen, rms_bound, align, atom_set,
		       atom_set_opt);
}



/** Prints the "cache" expression.  */

void
CCacheExpr::display (ostream &os)
{
  os << "cache (" << endl;
  fgref->display (os);
  os << endl;
  if (align)
    os << "  align" << endl;
  if (rms_bound != 0.0)
    os << "  rmsd_bound " << rms_bound << endl;
  if (atom_set == ALL_ATOMS_SET)
    os << "  all";
  else if (atom_set == BASE_ATOMS_SET)
    os << "  base_only";
  else if (atom_set == BACKBONE_ATOMS_SET)
    os << "  backbone_only";
  else
    os << "  pse_only";
  os << endl;
  if (atom_set_opt == NO_HYDROGEN_OPT)
    os << "  no_hydrogen" << endl;
  os << ')' << endl << endl;
}
