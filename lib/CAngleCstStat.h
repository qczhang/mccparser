//                              -*- Mode: C++ -*- 
// CAngleCstStat.h
// Copyright © 2000 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Thu Jan 20 15:54:49 2000
// Last Modified By : Martin Larose
// Last Modified On : Thu Jun 29 14:10:23 2000
// Update Count     : 3
// Status           : Unknown.
// 


#ifndef _CAngleCstStat_h_
#define _CAngleCstStat_h_


/** Class representing the "angle" statement.  This statement generate a
    constraint between three atoms to fit in a lower and an upper bound.  
    This  class owns a local class _AngleStruc that contains the informations 
    on the constraints.

    Usage: first generate a CAngleCstStat object and then you can fill its
    _AngleStruc* vector using the GenAngleStruc method. This vector contains
    all information about the specifications of the constraint.
*/


#include <vector.h>

#include "CStat.h"

class ostream;
class CEnv;
class CAngleCst;
class CResidueName;


class CAngleCstStat : public CStat
{
  class _AngleStruc
  {
    CResidueName *res1;
    char *at1;
    CResidueName *res2;
    char *at2;
    CResidueName *res3;
    char *at3;
    float dist_min, dist_max;
    
    _AngleStruc () { }
    
  public:
    
    _AngleStruc (CResidueName *r1, char *a1, CResidueName *r2, char *a2,
		 CResidueName *r3, char *a3, float mn, float mx)
      : res1 (r1), at1 (a1), res2 (r2), at2 (a2), res3 (r3), at3 (a3),
	dist_min (mn), dist_max (mx)
    { }
    ~_AngleStruc ();
    
    CAngleCst* eval (CEnv &env);
    void display (ostream &os);
  };


  vector< _AngleStruc* > *strucs;

public:

  CAngleCstStat () : strucs (new vector< _AngleStruc* > ()) { }
  virtual ~CAngleCstStat ();

  void GenAngleStruc (CResidueName *r1, char *a1, CResidueName *r2, char *a2,
		      CResidueName *r3, char *a3, float mn, float mx) {
    strucs->push_back (new _AngleStruc (r1, a1, r2, a2, r3, a3, mn, mx)); 
  }
  
  virtual void eval (CEnv &env);
  virtual void display (ostream &os);
};


#endif






