//                              -*- mode: C++ -*- 
// CVersionStat.h
// Copyright © 1999, 2000 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Fri Oct 22 11:30:44 1999
// Last Modified By : Martin Larose
// Last Modified On : Wed Mar  8 09:47:43 2000
// Update Count     : 5
// Status           : Ok.
// 



#ifndef _CVersionStat_h_
#define _CVersionStat_h_


/** Class representing the AST form of the "version" statement.  */


#include <iostream.h>

#include "CStat.h"

class CEnv;


class CVersion : public CStat
{
public:

  CVersion () { }
  virtual ~CVersion () { }

  virtual void eval (CEnv &env)
  { cout << PACKAGE << " " << VERSION << " (" << __DATE__ << ")" << endl; }
  virtual void display (ostream &os) { os << "version" << endl << endl; }
};


#endif
