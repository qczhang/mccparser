//                              -*- mode: C++ -*- 
// CNotesStat.h
// Copyright © 1999 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Thu Sep 23 18:48:19 1999
// Last Modified By : Martin Larose
// Last Modified On : Tue Oct 12 18:31:16 1999
// Update Count     : 1
// Status           : Ok.
// 


#ifndef _CNotesStat_h_
#define _CNotesStat_h_


/** Class representing the AST statement "notes".  */

#include <iostream.h>

#include "CStat.h"
#include "CEnv.h"


class CNotesStat : public CStat
{
public:

  CNotesStat () { }
  virtual ~CNotesStat () { }

  virtual void eval (CEnv &env) { env.PrintNotes (); }
  virtual void display (ostream &os) { os << "notes" << endl; }
};


#endif
