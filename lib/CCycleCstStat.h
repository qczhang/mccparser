//                              -*- mode: C++ -*- 
// CCycleCstStat.h
// Copyright © 1999, 2000 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Fri Sep 24 09:02:51 1999
// Last Modified By : Martin Larose
// Last Modified On : Mon Aug 21 13:24:29 2000
// Update Count     : 2
// Status           : Ok.
// 



#ifndef _CCycleCstStat_h_
#define _CCycleCstStat_h_


/** Class representing the AST node "cycle" statement.  */


#include <vector.h>

#include "CStat.h"
#include "CResidueName.h"

class CEnv;
class ostream;


class CCycleCstStat : public CStat
{
  CResidueName *res1;
  CResidueName *res2;
  float dist;
  int nb;

  CCycleCstStat () { }

public:

  CCycleCstStat (CResidueName *r1, CResidueName *r2, float d, int n)
    : res1 (r1), res2 (r2), dist (d), nb (n) { }
  virtual ~CCycleCstStat () { delete res1; delete res2; }

  virtual void eval (CEnv &env);
  virtual void display (ostream &os);
};


#endif
