//                              -*- Mode: C++ -*- 
// CDisplayFGStat.h
// Copyright © 2000 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Fri Jul 21 11:43:30 2000
// Last Modified By : Martin Larose
// Last Modified On : Mon Aug 21 13:24:35 2000
// Update Count     : 2
// Status           : Unknown.
// 


#ifndef _CDisplayFGStat_h_
#define _CDisplayFGStat_h_


#include "CStat.h"
#include "CFragGenStruc.h"

class CEnv;
class ostream;



class CDisplayFGStat : public CStat
{
  CFragGenStruc *fg_struc;

  CDisplayFGStat () { }
  
public:

  // LIFECYCLE ------------------------------------------------------------

  CDisplayFGStat (CFragGenStruc *f) : CStat (), fg_struc (f) { }
  CDisplayFGStat (const CDisplayFGStat &right);
  virtual ~CDisplayFGStat () { delete fg_struc; }

  // OPERATORS ------------------------------------------------------------

  const CDisplayFGStat& operator= (const CDisplayFGStat &right);

  // ACCESS ---------------------------------------------------------------

  // METHODS --------------------------------------------------------------

  virtual void eval (CEnv &env);

  // I/O  -----------------------------------------------------------------

  virtual void display (ostream &os);
};


#endif
