//                              -*- mode: C++ -*- 
// CClashCstStat.cc
// Copyright © 1999 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Fri Sep 24 08:38:31 1999
// Last Modified By : Martin Larose
// Last Modified On : Tue Oct 12 15:22:19 1999
// Update Count     : 2
// Status           : Ok.
// 



#include <iostream.h>

#include "CClashCstStat.h"

#include "CConstraint.h"
#include "CEnv.h"
#include "CClashCst.h"



/** Evaluation function for the "res_clash" expression.  It builds a
    constraint vector with the fraggen reference.
*/

void
CClashCstStat::eval (CEnv &env)
{
  vector< CConstraint* > cst_bag;
  CFragGen *fg;

  fg = fg_struc->eval (env);
  CClashCst::GenerateClash (cst_bag, distFac, fg, as, aso, VDWDist, distFac);
  env.CstInsert (cst_bag);
  env.AddCstBag (cst_bag);
}



/** Prints the "res_clash" expression.  */

void
CClashCstStat::display (ostream &os)
{
  os << "res_clash (" << endl
     << "  ";
  fg_struc->display (os);
  os << endl;
  if (VDWDist)
    os << "  vdw_distance ";
  else
    os << "  fixed_distance ";
  os << distFac << endl;
  switch (as)
    {
    case ALL_ATOMS_SET:
      os << "  all" << endl;
      break;
    case BASE_ATOMS_SET:
      os << "  base_only" << endl;
      break;
    case BACKBONE_ATOMS_SET:
      os << "  backbone_only" << endl;
      break;
    case PSE_ATOM_SET:
      os << "  pse_only" << endl;
      break;
    }
  switch (aso)
    {
    case NO_HYDROGEN_OPT:
      os << "  no_hydrogen" << endl;
      break;
    case NO_OPTION:
      break;
    }
  os << ')' << endl << endl;
}
