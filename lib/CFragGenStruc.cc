//                              -*- mode: C++ -*- 
// CFragGenStruc.cc
// Copyright © 1999, 2000 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Wed Sep 29 10:26:25 1999
// Last Modified By : Martin Larose
// Last Modified On : Mon Aug 21 13:24:56 2000
// Update Count     : 3
// Status           : Ok.
// 


#include <strstream.h>

#include "CFragGenStruc.h"

#include "CEnv.h"
#include "CFragGen.h"
#include "CEvalException.h"



/** Evaluation function for the FG reference.  It returns the FG bound to
    the identifier.  If it is not present, an exception is thrown by the
    library.  */

CFragGen*
CFragGenStruc::eval (CEnv &env)
{
  return env.GetFG (ident);
}
