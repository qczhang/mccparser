//                              -*- mode: C++ -*- 
// CExploreStat.h
// Copyright © 1999 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Thu Sep 23 12:50:18 1999
// Last Modified By : Martin Larose
// Last Modified On : Tue Oct 12 17:33:48 1999
// Update Count     : 1
// Status           : Ok.
// 



#ifndef _CExploreStat_h_
#define _CExploreStat_h_


/** Class representing the AST node "explore".  */

#include "CStat.h"
#include "CFragGenStruc.h"

class ostream;
class CEnv;


class CExploreStat : public CStat
{
  CFragGenStruc *fg_struc;
  char *form;
  bool zipped;

  CExploreStat () { }

public:

  CExploreStat (CFragGenStruc* fg, char* f, bool z)
    : fg_struc (fg), form (f), zipped (z) { }
  virtual ~CExploreStat () { delete fg_struc; delete [] form; }

  virtual void eval (CEnv &env);
  virtual void display (ostream &os);
};


#endif
