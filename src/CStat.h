//                              -*- mode: C++ -*- 
// CStat.h
// Copyright © 1999 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Wed Sep 22 20:08:28 1999
// Last Modified By : Martin Larose
// Last Modified On : Tue Oct 12 19:40:34 1999
// Update Count     : 1
// Status           : Ok.
// 



#ifndef _CStat_h_
#define _CStat_h_


/** Parent class for the statements.  */

#include <iostream.h>

class CEnv;


class CStat
{
public:

  CStat () { }
  virtual ~CStat () { }

  virtual void eval (CEnv &env) { }
  virtual void display (ostream &os) { os << "Class CStat reached." << endl; }
};


#endif
