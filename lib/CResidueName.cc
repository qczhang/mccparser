//                              -*- mode: C++ -*- 
// CResidueName.cc
// Copyright © 1999, 2000 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Thu Sep 30 18:55:34 1999
// Last Modified By : Martin Larose
// Last Modified On : Mon Aug 21 13:25:07 2000
// Update Count     : 3
// Status           : Ok.
// 



#include "CResidueName.h"

#include "CResidue.h"
#include "CEnv.h"
#include "CResHolder.h"
#include "CEvalException.h"



/** Evaluation method for the residue references.  It returns the residue
    identified with the residue number and the residue id.  It throws a
    CEvalException if the residue holder is not initialized or if the
    residue was not defined in the residue holder.  */

CResidue*
CResidueName::eval (CEnv &env)
{
  CResHolder &hold = env.GetHold ();

  if (!&hold)
    throw CEvalException ("Invalid residue reference ")
      << id << no << ", no residues in memory yet.";
  
  CResidue *res = hold.GetPtr (no, id);
  
  if (!res)
    throw CEvalException ("Invalid residue reference ")
      << id << no << ", not found in the residue holder.";
  return res;
}
    
