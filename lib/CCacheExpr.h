//                              -*- mode: C++ -*- 
// CCacheExpr.h
// Copyright © 1999 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Fri Sep 24 07:23:36 1999
// Last Modified By : Martin Larose
// Last Modified On : Tue Oct 12 13:49:42 1999
// Update Count     : 1
// Status           : Ok.
// 



#ifndef _CCacheExpr_h_
#define _CCacheExpr_h_


/** Class representing the AST node for the "cache" expression.  */

#include "UMcsymLib.h"

#include "CFGExpr.h"
#include "CFragGenStruc.h"

class ostream;
class CEnv;
class CFragGen;


class CCacheExpr : public CFGExpr
{
  CFragGenStruc *fgref;
  float rms_bound;
  AtomSet atom_set;
  AtomSetOption atom_set_opt;
  bool align;

  CCacheExpr () { }

public:

  CCacheExpr (CFragGenStruc *f, float rms, AtomSet as, AtomSetOption aso,
	      bool al)
    : fgref (f), rms_bound (rms), atom_set (as), atom_set_opt (aso),
      align (al) { }
  virtual ~CCacheExpr () { delete fgref; }

  virtual CFragGen* eval (CEnv &env);
  virtual void display (ostream &os);
};


#endif
