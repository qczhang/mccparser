//                              -*- mode: C++ -*- 
// CQExpr.h
// Copyright © 1999 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Thu Sep 23 18:56:12 1999
// Last Modified By : Martin Larose
// Last Modified On : Tue Oct 12 18:50:47 1999
// Update Count     : 1
// Status           : Ok.
// 



#ifndef _CQExpr_h_
#define _CQExpr_h_


/** Parent class for the Query expressions.  */

#include <iostream.h>

class CEnv;
class CQuery;


class CQExpr
{
public:
  
  CQExpr () { }
  virtual ~CQExpr () { }

  virtual CQuery* eval (CEnv &env) { return 0; }
  virtual void display (ostream &os) { os << "Class CQExpr reached." << endl; }
};


#endif
