//                              -*- mode: C++ -*- 
// CResidueStat.h
// Copyright © 1999 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Wed Sep 22 20:50:28 1999
// Last Modified By : Martin Larose
// Last Modified On : Tue Oct 12 19:25:26 1999
// Update Count     : 1
// Status           : Ok.
// 


#ifndef _CResidueStat_h_
#define _CResidueStat_h_


/** Class representing the AST node of the "residue" statement.  The
    class contains a local class: _ResidueStruc that contains each residue
    description.

    Usage: first generate a CResidueStat object and then you can fill its
    _ResidueStruc* vector using the GenResidueStruc method.  */

#include <vector.h>

#include "CStat.h"
#include "CResidueName.h"
#include "CQueryExpr.h"


class CEnv;
class CQuery;
class ostream;


class CResidueStat : public CStat
{
  class _ResidueStruc
  {
    CResidueName *res1;
    CResidueName *res2;
    CQueryExpr *expr;
    int ssize;
    
    _ResidueStruc () { }
    
  public:
    
    _ResidueStruc (CResidueName *r1, CResidueName *r2, CQueryExpr *exp, int ss)
      : res1 (r1), res2 (r2), expr (exp), ssize (ss) { }
    ~_ResidueStruc () { delete res1; delete res2; delete expr; }
    
    void eval (CEnv &env);
    void display (ostream &os);
  };


  vector< _ResidueStruc* > *strucs;

public:

  CResidueStat () : strucs (new vector< _ResidueStruc* > ()) { }
  virtual ~CResidueStat ();

  void GenResidueStruc (CResidueName *r1, CResidueName *r2, CQueryExpr *exp,
			int ss)
  { strucs->push_back (new _ResidueStruc (r1, r2, exp, ss)); }
  
  virtual void eval (CEnv &env);
  virtual void display (ostream &os);
};


#endif
