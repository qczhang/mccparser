//                              -*- mode: C++ -*- 
// CNoteStat.h
// Copyright © 1999 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Thu Sep 23 18:51:06 1999
// Last Modified By : Martin Larose
// Last Modified On : Tue Oct 12 18:29:57 1999
// Update Count     : 1
// Status           : Ok.
// 



#ifndef _CNoteStat_h_
#define _CNoteStat_h_


/** Class representing the "note" statement.  */

#include <iostream.h>

#include "CStat.h"
#include "CEnv.h"


class CNoteStat : public CStat
{
  char *str;

  CNoteStat () { }
  
public:

  CNoteStat (char *s) : str (s) { }
  virtual ~CNoteStat () { delete [] str; }

  virtual void eval (CEnv &env) { env.PutNote (str); }
  virtual void display (ostream &os) { os << "note ( " << str << " )"
					  << endl; }
};


#endif
