//                              -*- mode: C++ -*- 
// CLibraryExpr.cc
// Copyright © 1999, 2000 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Fri Sep 24 08:06:49 1999
// Last Modified By : Martin Larose
// Last Modified On : Fri May 12 12:02:53 2000
// Update Count     : 3
// Status           : Ok.
// 



#include <iostream.h>
#include <stdio.h>
#include <string.h>

#include "CLibraryExpr.h"

#include "CFragGen.h"
#include "CEnv.h"
#include "CLibraryFG.h"
#include "CResidueName.h"



/** Destructor for the local class _StripStruc.  */

CLibraryExpr::_StripStruc::~_StripStruc ()
{
  vector< CResidueName* >::iterator it;

  for (it = res_vec->begin (); it != res_vec->end (); ++it)
    delete *it;
  delete res_vec;
}



/** Evaluation method for the local class _StripStruc.  For each residue
    enumerated in the "strip" substatement, it places its name in the
    strip_v.  */

void
CLibraryExpr::_StripStruc::eval (CEnv &env, vector< char* > *chainid_v,
				 vector< char* > *strip_v)
{
  vector< CResidueName* >::iterator it;

  for (it = res_vec->begin (); it != res_vec->end (); ++it)
    {
      CResidue *res = (*it)->eval (env);
      char *name;
      char strip_name[256];

      sprintf (strip_name, "%c%d", res->GetChainId (), res->GetResNo ());
      name = new char[strlen (strip_name) + 1];
      strcpy (name, strip_name);
      cout << "strip ( " << name << " )" << endl;
      strip_v->push_back (name);
    }
}



/** Prints the substatement "strip".  */

void
CLibraryExpr::_StripStruc::display (ostream &os)
{
  vector< CResidueName* >::iterator it;

  os << "strip ( ";
  for (it = res_vec->begin (); it != res_vec->end (); it++)
    {
      (*it)->display (os);
      os << ' ';
    }
  os << ')' << endl;
}



/** Evaluation method for the local class _ChangeIdStruc.  It places the id
    to change in the chainid_v.  */

void
CLibraryExpr::_ChangeIdStruc::eval (CEnv &env, vector< char* > *chainid_v,
				    vector< char* > *strip_v)
{
  char *tmpName = new char[3];
  
  tmpName[0] = from;
  tmpName[1] = to;
  tmpName[2] = '\0';
  cout << "chain ( " << tmpName << " )" << endl;
  chainid_v->push_back (tmpName);
}



/** Destructor for the "library" expression.  */

CLibraryExpr::~CLibraryExpr ()
{
  vector< _LibStruc* >::iterator it;

  delete[] str;
  for (it = strucs->begin (); it != strucs->end (); it++)
    delete *it;
  delete strucs;
}



/** Evaluation method for the "library" expression.  It evaluates each
    _LibStruc, initialize a CLibraryFG and returns it.  */

CFragGen*
CLibraryExpr::eval (CEnv &env)
{
  vector< _LibStruc* >::iterator it;
  vector< char* > chainid_v;
  vector< char* > strip_v;

  for (it = strucs->begin (); it != strucs->end (); it++)
    (*it)->eval (env, &chainid_v, &strip_v);
  return new CLibraryFG ("", &env.GetHold (), str, chainid_v, strip_v, PDB_FILE);
}



/** Prints the "library" expression.  */

void
CLibraryExpr::display (ostream &os)
{
  vector< _LibStruc* >::iterator it;

  os << "library ( pdb \"" << str << "\"";
  for (it = strucs->begin (); it != strucs->end (); it++)
    {
      (*it)->display (os);
      os << endl << "          ";
    }
  os << ')' << endl;
}
