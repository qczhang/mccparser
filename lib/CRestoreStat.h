//                              -*- Mode: C++ -*- 
// CRestoreStat.h
// Copyright © 2000 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Wed Jun 21 17:34:41 2000
// Last Modified By : Martin Larose
// Last Modified On : Mon Aug 21 13:25:19 2000
// Update Count     : 2
// Status           : Unknown.
// 


#ifndef __CRestoreStat_h_
#define __CRestoreStat_h_


/** Class representing the AST node "restore".  */

#include "CStat.h"

class CEnv;
class ostream;



class CRestoreStat : public CStat 
{
  char *filename;
  char *form;
  bool zipped;

  CRestoreStat () { }
  CRestoreStat (const CRestoreStat &right) { }
  const CRestoreStat& operator= (const CRestoreStat &right) { return *this; }
  
public:

  // LIFECYCLE ------------------------------------------------------------

  CRestoreStat (char *fi, char *fo, bool z)
    : filename (fi), form (fo), zipped (z) { }
  ~CRestoreStat () { delete[] filename; delete[] form; }

  // METHODS --------------------------------------------------------------

  virtual void eval (CEnv &env);

  // I/O  -----------------------------------------------------------------

  virtual void display (ostream &os);
};


#endif
