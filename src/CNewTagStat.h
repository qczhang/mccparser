//                              -*- mode: C++ -*- 
// CNewTagStat.h
// Copyright © 1999 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Thu Sep 23 14:30:57 1999
// Last Modified By : Martin Larose
// Last Modified On : Tue Oct 12 18:29:05 1999
// Update Count     : 1
// Status           : Ok.
// 



#ifndef _CNewTagStat_h_
#define _CNewTagStat_h_


/** Class representing the "new_tag" statement.  */

#include <vector.h>

#include "CStat.h"

class ostream;
class CEnv;
class CQueryExpr;


class CNewTagStat : public CStat
{
  bool resq_opt;
  char *id;
  vector< CQueryExpr* > *exprs;

  CNewTagStat () { }

public:

  CNewTagStat (bool o, char *i, vector< CQueryExpr* > *e)
    : resq_opt (o), id (i), exprs (e) { }
  virtual ~CNewTagStat ();

  virtual void eval (CEnv &env);
  virtual void display (ostream &os);
};


#endif
