//                              -*- mode: C++ -*- 
// CResetDBStat.h
// Copyright © 1999 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Thu Sep 23 18:44:41 1999
// Last Modified By : Martin Larose
// Last Modified On : Tue Oct 12 19:03:43 1999
// Update Count     : 1
// Status           : Ok.
// 



#ifndef _CResetDBStat_h_
#define _CResetDBStat_h_


/** Class representing the "reset_db" statement in AST form.  */

#include <iostream.h>

#include "CStat.h"
#include "CEnv.h"


class CResetDBStat : public CStat
{
public:
  CResetDBStat () { }
  virtual ~CResetDBStat () { }

  virtual void eval (CEnv &env) { env.ResetDB (); }
  virtual void display (ostream &os) { os << "reset_db" << endl << endl; }
};


#endif
