//                              -*- mode: C++ -*- 
// CResetStat.h
// Copyright © 1999 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Thu Sep 23 14:20:10 1999
// Last Modified By : Martin Larose
// Last Modified On : Tue Oct 12 19:04:35 1999
// Update Count     : 1
// Status           : Ok.
// 



#ifndef _CResetStat_h_
#define _CResetStat_h_


/** Class representing the "reset" statement in AST form.  */

#include <iostream.h>

#include "CStat.h"
#include "CEnv.h"


class CResetStat : public CStat
{
public:
  
  CResetStat () { }
  virtual ~CResetStat () { }

  virtual void eval (CEnv &env) { env.Reset (); }
  virtual void display (ostream &os) { os << "reset" << endl; }
};


#endif
