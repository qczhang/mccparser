//                              -*- mode: C++ -*- 
// CAssignStat.h
// Copyright © 1999 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Wed Sep 22 20:40:18 1999
// Last Modified By : Martin Larose
// Last Modified On : Fri Oct  8 14:31:26 1999
// Update Count     : 1
// Status           : Ok.
// 


#ifndef _CAssignStat_h_
#define _CAssignStat_h_


/** Class representing the AST node of the assignment "=" statement. */

#include "CStat.h"
#include "CFGExpr.h"

class CEnv;


class CAssignStat : public CStat
{
  char *ident;
  CFGExpr *expr;

  CAssignStat () { }

public:

  CAssignStat (char *s, CFGExpr *exp) : ident (s), expr (exp) { }
  virtual ~CAssignStat () { delete [] ident; delete expr; }

  virtual void eval (CEnv &env);
  virtual void display (ostream &os);
};

#endif
