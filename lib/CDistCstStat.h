//                              -*- mode: C++ -*- 
// CDistCstStat.h
// Copyright © 1999 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Fri Sep 24 08:46:37 1999
// Last Modified By : Martin Larose
// Last Modified On : Tue Oct 12 15:24:52 1999
// Update Count     : 1
// Status           : Ok.
// 



#ifndef _CDistCstStat_h_
#define _CDistCstStat_h_


/** Class representing the "distance" statement.  This statement generate a
    constraint between two atoms with a lower and an upper bound.  This
    class owns a local class _DistStruc that contains the informations on
    the constraints.

    Usage: first generate a CDistCstStat object and then you can fill its
    _DistStruc* vector using the GenDistStruc method.
*/


#include <vector.h>

#include "CStat.h"
#include "CResidueName.h"

class ostream;
class CConstraint;
class CEnv;
class CDistCst;


class CDistCstStat : public CStat
{
  class _DistStruc
  {
    CResidueName *res1;
    char *at1;
    CResidueName *res2;
    char *at2;
    float dist_min, dist_max;
    
    _DistStruc () { }
    
  public:
    
    _DistStruc (CResidueName *r1, char *a1, CResidueName *r2, char *a2,
		float mn, float mx)
      : res1 (r1), at1 (a1), res2 (r2), at2 (a2), dist_min (mn), dist_max (mx)
    { }
    ~_DistStruc () { delete res1; delete[] at1; delete res2; delete[] at2; }
    
    CDistCst* eval (CEnv &env);
    void display (ostream &os);
  };


  vector< _DistStruc* > *strucs;

public:

  CDistCstStat () : strucs (new vector< _DistStruc* > ()) { }
  virtual ~CDistCstStat ();

  void GenDistStruc (CResidueName *r1, char *a1, CResidueName *r2, char *a2,
		     float mn, float mx)
  { strucs->push_back (new _DistStruc (r1, a1, r2, a2, mn, mx)); }
  virtual void eval (CEnv &env);
  virtual void display (ostream &os);
};


#endif
