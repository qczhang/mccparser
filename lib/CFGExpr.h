//                              -*- mode: C++ -*- 
// CFGExpr.h
// Copyright © 1999 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Fri Oct  1 10:07:17 1999
// Last Modified By : Martin Larose
// Last Modified On : Tue Oct 12 17:35:26 1999
// Update Count     : 1
// Status           : Ok.
// 



#ifndef _CFGExpr_h_
#define _CFGExpr_h_


/** Class parent for the Fragment Generetors expressions.  */

#include <iostream.h>

class CEnv;
class CFragGen;


class CFGExpr
{
public:

  CFGExpr () { }
  virtual ~CFGExpr () { }

  virtual CFragGen* eval (CEnv &env) { return 0; }
  virtual void display (ostream &os) { os << "Class CFGExpr reached." << endl; }
};


#endif
