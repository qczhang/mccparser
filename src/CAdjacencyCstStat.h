//                              -*- mode: C++ -*- 
// CAdjacencyCstStat.h
// Copyright © 1999, 2000 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Fri Sep 24 08:27:27 1999
// Last Modified By : Martin Larose
// Last Modified On : Mon Aug 21 13:24:16 2000
// Update Count     : 3
// Status           : Ok.
// 



#ifndef _CAdjacencyCstStat_h_
#define _CAdjacencyCstStat_h_


/** Class representing the AST node "adjacency". */

#include "CStat.h"
#include "CFragGenStruc.h"

class CEnv;
class ostream;
class CConstraint;


class CAdjacencyCstStat : public CStat
{
  CFragGenStruc *fg_struc;
  float the_min, the_max;

  CAdjacencyCstStat ();

public:

  CAdjacencyCstStat (CFragGenStruc *f, float mn, float mx)
    : fg_struc (f), the_min (mn), the_max (mx) { }
  virtual ~CAdjacencyCstStat () { delete fg_struc; }

  virtual void eval (CEnv &env);
  virtual void display (ostream &os);
};


#endif
