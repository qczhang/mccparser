//                              -*- mode: C++ -*- 
// CSourceStat.h
// Copyright © 1999 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Thu Sep 23 12:57:48 1999
// Last Modified By : Martin Larose
// Last Modified On : Tue Oct 12 19:39:50 1999
// Update Count     : 1
// Status           : Ok.
// 



#ifndef _CSourceStat_h_
#define _CSourceStat_h_


/** Class representing the AST form of the "source" statement.  */

#include "CStat.h"

class ostream;
class CEnv;



class CSourceStat : public CStat
{
  char *str;

  CSourceStat () { }

public:

  CSourceStat (char *s) : str (s) { }
  virtual ~CSourceStat () { delete[] str; }

  virtual void eval (CEnv &env);
  virtual void display (ostream &os);
};


#endif
