//                              -*- mode: C++ -*- 
// CAddPdbStat.h
// Copyright © 1999 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Thu Sep 23 18:16:20 1999
// Last Modified By : Martin Larose
// Last Modified On : Fri Oct  8 14:28:19 1999
// Update Count     : 1
// Status           : Ok.
// 



#ifndef _CAddPdbStat_h_
#define _CAddPdbStat_h_


/** Class representing the AST node of the "add_pdb" form. */

#include <vector.h>

#include "CStat.h"

class CEnv;
class ostream;


class CAddPdbStat : public CStat
{
  class _AddPdbStruc
  {
    float current_tfo_cutoff;
    float current_confo_cutoff;
    vector < char* > *strs;

    _AddPdbStruc () { }

  public:

    _AddPdbStruc (float ctc, float ccc, vector< char* > *s)
      : current_tfo_cutoff (ctc), current_confo_cutoff (ccc), strs (s) { }
    ~_AddPdbStruc ();

    void eval (CEnv &env);
    void display (ostream &os);
  };


  vector< _AddPdbStruc* > *strucs;
  
public:

  CAddPdbStat () : strucs (new vector< _AddPdbStruc* > ()) { }
  virtual ~CAddPdbStat ();

  void GenPdbStruc (float ctc, float ccc, vector< char* > *s)
    { strucs->push_back (new _AddPdbStruc (ctc, ccc, s)); }
  virtual void eval (CEnv &env);
  virtual void display (ostream &os);
};


#endif
