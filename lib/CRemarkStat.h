//                              -*- mode: C++ -*- 
// CRemarkStat.h
// Copyright © 1999 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Wed Nov 10 13:53:04 1999
// Last Modified By : Martin Larose
// Last Modified On : Wed Nov 10 19:23:07 1999
// Update Count     : 2
// Status           : Ok.
// 



#ifndef _CRemarkStat_h_
#define _CRemarkStat_h_


#include <iostream.h>

#include "CStat.h"

#include "CEnv.h"


class CRemarkStat : public CStat
{
  char *str;

  CRemarkStat () { }

public:

  CRemarkStat (char *s) : str (s) { }
  virtual ~CRemarkStat () { delete[] str; }

  virtual void eval (CEnv &env) { env.AddRemark (str); }
  virtual void display (ostream &os)
  { os << "remark (\"" << str << "\")" << endl << endl; }
};


#endif
