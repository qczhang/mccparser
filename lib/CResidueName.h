//                              -*- mode: C++ -*- 
// CResidueName.h
// Copyright © 1999 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Fri Sep 24 09:59:48 1999
// Last Modified By : Martin Larose
// Last Modified On : Tue Oct 12 19:15:50 1999
// Update Count     : 1
// Status           : Ok.
// 



#ifndef _CResidueName_h_
#define _CResidueName_h_


/** Class representing the residue reference.  */

#include <iostream.h>

class CResidue;
class CEnv;


class CResidueName
{
  char id;
  int no;

  CResidueName () { }

public:

  CResidueName (char i, int n) : id (i), no (n) { }
  virtual ~CResidueName () { }

  char GetId () const { return id; }
  int GetNo () const { return no; }
  
  virtual CResidue* eval (CEnv &env);
  virtual void display (ostream &os)
  { if (id != ' ') os << id; os << no; }
};


#endif
