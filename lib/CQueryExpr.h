//                              -*- mode: C++ -*- 
// CQueryExpr.h
// Copyright © 1999, 2000 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Fri Sep 24 09:33:11 1999
// Last Modified By : Martin Larose
// Last Modified On : Mon Feb 21 16:31:15 2000
// Update Count     : 2
// Status           : Ok.
// 



#ifndef _CQueryExpr_h_
#define _CQueryExpr_h_


/** Classes representing the query expressions.  The class contains a serie
    of classes describing the test fuctions (NOT, AND, OR, ID) for the
    query.  These temporary classes are transform to a query by the eval
    function.  We can display to a ostream the original query by using the
    display method.  */

#include <vector.h>
#include <iostream.h>

#include "CQExpr.h"

class CQuery;
class CQueryFunc;
class CEnv;



class CQFunc
{
public:

  CQFunc () { }
  virtual ~CQFunc () { }

  virtual CQueryFunc* eval () const { return 0; }
  virtual void display (ostream &os) { }
};



class CQTrueFunc : public CQFunc
{
public:

  CQTrueFunc () { }
  ~CQTrueFunc () { }

  CQueryFunc* eval () const;
  void display (ostream &os) { }
};



class CQIdentFunc : public CQFunc
{
  char *str;

  CQIdentFunc () { }
  
public:

  CQIdentFunc (char *s) : str (s) { }
  ~CQIdentFunc () { delete[] str; }

  CQueryFunc* eval () const;
  void display (ostream &os) { os << str; }
};



class CQNotFunc : public CQFunc
{
  CQFunc *fn;

  CQNotFunc () { }

public:

  CQNotFunc (CQFunc *f) : fn (f) { }
  ~CQNotFunc () { delete fn; }

  CQueryFunc* eval () const;
  void display (ostream &os)
  { os << "! (";  fn->display (os); os << ")"; }
};



class CQAndFunc : public CQFunc
{
  CQFunc *left;
  CQFunc *right;

  CQAndFunc () { }

public:

  CQAndFunc (CQFunc *l, CQFunc *r) : left (l), right (r) { }
  ~CQAndFunc () { delete left; delete right; }

  CQueryFunc* eval () const;
  void display (ostream &os)
  { os << "("; left->display (os); os << " && ";
    right->display (os); os << ")"; }
};
  
  

class CQOrFunc : public CQFunc
{
  CQFunc *left;
  CQFunc *right;

  CQOrFunc () { }

public:

  CQOrFunc (CQFunc *l, CQFunc *r) : left (l), right (r) { }
  ~CQOrFunc () { delete left; delete right; }

  CQueryFunc* eval () const;
  void display (ostream &os)
  { os << "("; left->display (os); os << " || "; right->display (os);
    os << ")"; }
};
  


class CQueryExpr : public CQExpr
{
  vector< char* > strs;
  CQFunc* fn;

public:

  CQueryExpr () : fn (new CQTrueFunc ()) { }
  virtual ~CQueryExpr ();

  void GenFileName (char *f) { strs.push_back (f); }
  void GenFunc (CQFunc *f) { if (fn) delete fn; fn = f; }
  virtual CQuery* eval (CEnv &env);
  virtual void display (ostream &os);
};


#endif
