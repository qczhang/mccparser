//                              -*- mode: C++ -*- 
// CLibraryExpr.h
// Copyright © 1999 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Fri Sep 24 08:02:24 1999
// Last Modified By : Martin Larose
// Last Modified On : Tue Oct 12 18:03:55 1999
// Update Count     : 1
// Status           : Ok.
// 



#ifndef _CLibraryExpr_h_
#define _CLibraryExpr_h_


/** Class representing a "library" AST node.  The class contains a local
    hierarchy of statements: _LibStruc (the parent), _StripStruc (for the
    "strip" substatement) and _ChangeIdStruc (for the "change_id"
    substatement).

    Usage: first generate a CLibraryExpr object and then you can fill its
    _LibStruc* vector using the GenStripStruc and GenChangeIdStruc methods.  */


#include <vector.h>

#include "CFGExpr.h"

class ostream;
class CFragGen;
class CEnv;
class CResidueName;


class CLibraryExpr : public CFGExpr
{
  class _LibStruc
  {
  protected:
    
    vector< CResidueName* > *res_vec;
    char from;
    char to;
    
    _LibStruc () { }
    
  public:
    
    _LibStruc (vector< CResidueName* > *rv, char f, char t)
      : res_vec (rv), from (f), to (t) { }
    virtual ~_LibStruc () { }
    
    virtual void eval (CEnv&, vector< char* >*, vector< char* >*) { }
    virtual void display (ostream&) { }
  };
  
  
  class _StripStruc : public _LibStruc
  {
    _StripStruc () { }
    
  public:
    
    _StripStruc (vector< CResidueName* > *res_vec)
      : _LibStruc (res_vec, 0, 0) { }
    ~_StripStruc ();
    
    void eval (CEnv&, vector< char* >*, vector< char* >*);
    virtual void display (ostream&);
  };
  
  
  class _ChangeIdStruc : public _LibStruc
  {
    _ChangeIdStruc () { }
    
  public:
    
    _ChangeIdStruc (char f, char t) : _LibStruc (0, f, t) { }
    ~_ChangeIdStruc () { }
    
    virtual void eval (CEnv&, vector< char* >*, vector< char* >*);
    virtual void display (ostream &os)
    { os << "change_id ( " << from << " : " << to << " )" << endl; }
  };
  
  
  char *str;
  vector< _LibStruc* > *strucs;

public:

  CLibraryExpr (char *s) : str (s), strucs (new vector< _LibStruc* > ()) { }
  virtual ~CLibraryExpr ();

  void GenStripStruc (vector< CResidueName* > *res_vec)
  { strucs->push_back (new _StripStruc (res_vec)); }
  void GenChangeIdStruc (char f, char t)
  { strucs->push_back (new _ChangeIdStruc (f, t)); }
  
  virtual CFragGen* eval (CEnv &env);
  virtual void display (ostream &os);
};


#endif
