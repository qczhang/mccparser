//                              -*- mode: C++ -*- 
// CClashCstStat.h
// Copyright © 1999 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Fri Sep 24 08:33:31 1999
// Last Modified By : Martin Larose
// Last Modified On : Tue Oct 12 15:22:01 1999
// Update Count     : 2
// Status           : Ok.
// 



#ifndef _CClashCstStat_h_
#define _CClashCstStat_h_


/** Class representing the "res_clash" AST node.  */

#include <vector.h>

#include "UMcsymLib.h"

#include "CStat.h"
#include "CFragGenStruc.h"

class ostream;


class CClashCstStat : public CStat
{
  CFragGenStruc *fg_struc;
  bool VDWDist;
  float distFac;
  AtomSet as;
  AtomSetOption aso;

  CClashCstStat () { }

public:

  CClashCstStat (CFragGenStruc *fg, bool V, float d, AtomSet a,
		 AtomSetOption ao)
    : fg_struc (fg), VDWDist (V), distFac (d), as (a), aso (ao) { }
  virtual ~CClashCstStat () { delete fg_struc; }

  virtual void eval (CEnv &env);
  virtual void display (ostream &os);
};


#endif
