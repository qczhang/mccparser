//                              -*- mode: C++ -*- 
// CPairStat.h
// Copyright © 1999 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Wed Sep 22 21:41:31 1999
// Last Modified By : Martin Larose
// Last Modified On : Tue Oct 12 18:47:44 1999
// Update Count     : 1
// Status           : Ok.
// 



#ifndef _CPairStat_h_
#define _CPairStat_h_


/** Class representing the AST node "pair" statement.  The class contains a
    local substatement: _PairStruc that contains each pairing description.

    Usage: first generate a CPairStat object and then you can fill its
    _PairStruc* vector using the GenPairStruc method.  */

#include <vector.h>

#include "CStat.h"
#include "CResidueName.h"
#include "CQueryExpr.h"

class CEnv;
class ostream;
class CQuery;


class CPairStat : public CStat
{
  class _PairStruc
  {
    CResidueName *res;
    CResidueName *ref;
    CQueryExpr *expr;
    int ssize;
    
    _PairStruc () { }
    
  public:
    
    _PairStruc (CResidueName *r1, CResidueName *r2, CQueryExpr *e, int s)
      : res (r1), ref (r2), expr (e), ssize (s) { }
    ~_PairStruc () { delete res; delete ref; delete expr; }
    
    void eval (CEnv &env);
    void display (ostream &os);
  };


  vector< _PairStruc* > *strucs;

public:

  CPairStat () : strucs (new vector< _PairStruc* > ()) { }
  virtual ~CPairStat ();

  void GenPairStruc (CResidueName *r1, CResidueName *r2, CQueryExpr *e, int s)
  { strucs->push_back (new _PairStruc (r1, r2, e, s)); }
  
  virtual void eval (CEnv &env);
  virtual void display (ostream &os);
};


#endif
