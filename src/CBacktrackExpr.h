//                              -*- mode: C++ -*- 
// CBacktrackExpr.h
// Copyright © 1999 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Thu Sep 23 19:24:15 1999
// Last Modified By : Martin Larose
// Last Modified On : Fri Oct  8 16:34:34 1999
// Update Count     : 1
// Status           : Ok.
// 



#ifndef _CBacktrackExpr_h_
#define _CBacktrackExpr_h_


/** Class representing the AST node of the "backtrack" expression.  The
    class contains a local hierarchy of statements: _GenBTStruc (the
    parent), _FGStruc (for the fraggen reference), _BTStruc (for the residue
    enumeration) and _PlaceStruc (for the "place" statement).

    Usage: first generate a CBacktrackExpr object and then you can fill its
    _GenBTStruc* vector using the GenFGStruc, GenBTStruc and GenPlaceStruc
    methods.
*/


#include <vector.h>
#include <iostream.h>

#include "CFGExpr.h"
#include "CFragGenStruc.h"
#include "CResidueName.h"

class CEnv;
class CFragGen;
class CBacktrackFG;


class CBacktrackExpr : public CFGExpr
{
  class _GenBTStruc
  {
  protected:
    
    CResidueName *ref;
    CResidueName *res;
    CFragGenStruc *fg_struc;
    vector< CResidueName* > *res_v;
    
    _GenBTStruc () { }
    
  public:
    
    _GenBTStruc (CResidueName *rf, CResidueName *rs,
		 CFragGenStruc *f, vector< CResidueName* > *rv)
      : ref (rf), res (rs), fg_struc (f), res_v (rv) { }
    virtual ~_GenBTStruc () { }
    
    virtual void eval (CEnv&, CBacktrackFG*, bool*) { }
    virtual void display (ostream&) { }
  };
  
  
  class _FGStruc : public _GenBTStruc
  {
    _FGStruc () { }
    
  public:
    
    _FGStruc (CFragGenStruc *f) : _GenBTStruc (0, 0, f, 0) { }
    ~_FGStruc () { delete fg_struc; }
    
    void eval (CEnv&, CBacktrackFG*, bool*);
    void display (ostream &os) { fg_struc->display (os); }
  };
  
  
  
  class _BTStruc : public _GenBTStruc
  {
    _BTStruc () { }
    
  public:
    
    _BTStruc (CResidueName *rf, vector< CResidueName* > *rv)
      : _GenBTStruc (rf, 0, 0, rv) { }
    ~_BTStruc ();
    
    void eval (CEnv&, CBacktrackFG*, bool*);
    void display (ostream&);
  };
  
  
  
  class _PlaceStruc : public _GenBTStruc
  {
    _PlaceStruc ( ) { }
    
  public:
    
    _PlaceStruc (CResidueName *rf, CResidueName *rs, CFragGenStruc *fg)
      : _GenBTStruc (rf, rs, fg, 0) { }
    ~_PlaceStruc () { delete ref; delete res; delete fg_struc; }
    
    void eval (CEnv&, CBacktrackFG*, bool*);
    void display (ostream&);
  };
  
  
  vector< _GenBTStruc* > *strucs;
  
public:

  CBacktrackExpr () : strucs (new vector< _GenBTStruc* > ()) { }
  virtual ~CBacktrackExpr ();

  void GenFGStruc (CFragGenStruc *f)
  { strucs->push_back (new _FGStruc (f)); }
  void GenBTStruc (CResidueName *rf, vector< CResidueName* > *rv)
  { strucs->push_back (new _BTStruc (rf, rv)); }
  void GenPlaceStruc (CResidueName *rf, CResidueName *rs, CFragGenStruc *fg)
  { strucs->push_back (new _PlaceStruc (rf, rs, fg)); }
  
  virtual CFragGen* eval (CEnv &env);
  virtual void display (ostream &os);
};


#endif
