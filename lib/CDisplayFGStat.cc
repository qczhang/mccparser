//                              -*- Mode: C++ -*- 
// CDisplayFGStat.cc
// Copyright © 2000 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Fri Jul 21 11:43:30 2000
// Last Modified By : Martin Larose
// Last Modified On : Mon Aug 21 13:24:32 2000
// Update Count     : 2
// Status           : Unknown.
// 


#include <iostream.h>

#include "CDisplayFGStat.h"

#include "CEnv.h"
#include <CFragGen.h>



CDisplayFGStat::CDisplayFGStat (const CDisplayFGStat &right)
  : fg_struc (new CFragGenStruc (*right.fg_struc))
{ }



const CDisplayFGStat&
CDisplayFGStat::operator= (const CDisplayFGStat &right)
{
  if (this != &right)
    {
      delete fg_struc;
      fg_struc = new CFragGenStruc (*right.fg_struc);
    }
  return *this;
}



void
CDisplayFGStat::eval (CEnv &env)
{
  CFragGen *fg;

  fg = fg_struc->eval (env);
  fg->Display (cout, 0);
}



void
CDisplayFGStat::display (ostream &os)
{
  os << "display_fg ( ";
  fg_struc->display (os);
  os << " )" << endl << endl;
}
