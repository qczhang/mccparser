//                              -*- mode: C++ -*- 
// CConnectStat.h
// Copyright © 1999 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Thu Sep 23 11:54:31 1999
// Last Modified By : Martin Larose
// Last Modified On : Tue Oct 12 14:25:23 1999
// Update Count     : 1
// Status           : Ok.
// 



#ifndef _CConnectStat_h_
#define _CConnectStat_h_


/** Class representing the "connect" AST node.  It contains a local class
    _ConnectStruc that collects each statement.

    Usage: first generate a CConnectStat object and then you can fill its
    _ConnectStruc* vector using the method GenConnectStruc.
*/

#include <vector.h>

#include "CStat.h"
#include "CQExpr.h"
#include "CResidueName.h"

class ostream;
class CEnv;
class CQuery;


class CConnectStat : public CStat
{
  class _ConnectStruc
  {
    CResidueName *res1;
    CResidueName *res2;
    CQExpr *expr;
    int ssize;
    
    _ConnectStruc () { }
    
  public:
    
    _ConnectStruc (CResidueName *r1, CResidueName *r2, CQExpr *e, int s)
      : res1 (r1), res2 (r2), expr (e), ssize (s) { }
    ~_ConnectStruc () { delete res1; delete res2; delete expr; }
    
    void eval (CEnv&);
    void display (ostream&);
  };
  

  vector< _ConnectStruc* > *strucs;

public:

  CConnectStat () : strucs (new vector< _ConnectStruc* > ()) { }
  virtual ~CConnectStat ();

  void GenConnectStruc (CResidueName *r1, CResidueName *r2, CQExpr *e, int s)
  { strucs->push_back (new _ConnectStruc (r1, r2, e, s)); }
  virtual void eval (CEnv &env);
  virtual void display (ostream &os);
};


#endif
