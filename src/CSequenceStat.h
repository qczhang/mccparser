//                              -*- mode: C++ -*- 
// CSequenceStat.h
// Copyright © 1999 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Wed Sep 22 20:16:22 1999
// Last Modified By : Martin Larose
// Last Modified On : Tue Oct 12 19:34:54 1999
// Update Count     : 1
// Status           : Ok.
// 


#ifndef _CSequenceStat_h_
#define _CSequenceStat_h_


/** Class representing the AST node for the "sequence" statement.  */

#include "CStat.h"
#include "CResidueName.h"


class CEnv;
class ostream;

class CSequenceStat : public CStat
{
  char type;
  CResidueName *res_name;
  char *seq;

  CSequenceStat () { }

public:

  CSequenceStat (char typ, CResidueName *res, char *str)
    : type (typ), res_name (res), seq (str)
  { }

  virtual ~CSequenceStat () { delete res_name; delete[] seq; }

  virtual void eval (CEnv &env);
  virtual void display (ostream &os);
};

#endif
