//                              -*- Mode: C++ -*- 
// CTorsionCstStat.h
// Copyright © 2000 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Wed Jun 28 14:55:53 2000
// Last Modified By : Martin Larose
// Last Modified On : Thu Jun 29 14:30:17 2000
// Update Count     : 2
// Status           : Ok.
// 

/** Class representing the "torsion" statement.  This statement generate a
    constraint between four atoms to fit in a lower and an upper bound.
    This class owns a local class _TorsionStruc that contains the
    informations on the constraints.

    Usage: first generate a CTorsionCstStat object and then you can fill its
    _TorsionStruc* vector using the GenTorsionStruc method. This vector
    contains all information about the specifications of the constraint.  */


#ifndef _CTorsionCstStat_h_
#define _CTorsionCstStat_h_

#include <vector.h>

#include "CStat.h"

class ostream;
class CEnv;
class CTorsionCst;
class CResidueName;



class CTorsionCstStat : public CStat
{
  class _TorsionStruc
  {
    CResidueName *res1;
    char *at1;
    CResidueName *res2;
    char *at2;
    CResidueName *res3;
    char *at3;
    CResidueName *res4;
    char *at4;

    float dist_min;
    float dist_max;
    
    _TorsionStruc () { }
    
  public:
    
    _TorsionStruc (CResidueName *r1, char *a1, CResidueName *r2, char *a2,
		   CResidueName *r3, char *a3, CResidueName *r4, char *a4,
		   float mn, float mx)
      : res1 (r1), at1 (a1), res2 (r2), at2 (a2),
	res3 (r3), at3 (a3), res4 (r4), at4 (a4),
	dist_min (mn), dist_max (mx)
    { }
    ~_TorsionStruc ();
    
    CTorsionCst* eval (CEnv &env);
    void display (ostream &os);
  };

  vector< _TorsionStruc* > *strucs;

public:

  CTorsionCstStat () : strucs (new vector< _TorsionStruc* > ()) { }
  virtual ~CTorsionCstStat ();

  void GenTorsionStruc (CResidueName *r1, char *a1, CResidueName *r2, char *a2,
			CResidueName *r3, char *a3, CResidueName *r4, char *a4,
			float mn, float mx) {
    strucs->push_back (new _TorsionStruc (r1, a1, r2, a2, r3, a3, r4, a4, mn, mx));
  }
  
  virtual void eval (CEnv &env);
  virtual void display (ostream &os);
};

#endif
