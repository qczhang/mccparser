//                              -*- mode: C++ -*- 
// CFragGenStruc.h
// Copyright © 1999 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Wed Sep 29 10:21:58 1999
// Last Modified By : Martin Larose
// Last Modified On : Tue Oct 12 17:42:21 1999
// Update Count     : 1
// Status           : Ok.
// 



#ifndef _CFragGenStruc_h_
#define _CFragGenStruc_h_


/** Class representing the FG reference AST node.  */

#include <iostream.h>

class CFragGen;
class CEnv;


class CFragGenStruc
{
  char *ident;

  CFragGenStruc () { }

public:

  CFragGenStruc (char *i) : ident (i) { }
  ~CFragGenStruc () { delete[] ident; }

  CFragGen* eval (CEnv &env);
  void display (ostream &os) { os << ident; }
};


#endif
