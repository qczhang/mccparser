//                              -*- mode: C++ -*- 
// CAddPdbStat.cc
// Copyright © 1999, 2000 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Thu Sep 23 18:28:38 1999
// Last Modified By : Martin Larose
// Last Modified On : Wed Apr 26 17:49:39 2000
// Update Count     : 4
// Status           : Ok.
// 



#include <iostream.h>

#include "CAddPdbStat.h"

#include "CEnv.h"



/** Destructor on strings accumulated in the structure.
 */

CAddPdbStat::_AddPdbStruc::~_AddPdbStruc ()
{
  vector < char* >::iterator it;

  for (it = strs->begin (); it != strs->end (); ++it)
    delete [] *it;
  delete strs;
}



/** Calls the environment load pdb method on each filename, essencially
    Load_pdb from CDatabase.
*/

void
CAddPdbStat::_AddPdbStruc::eval (CEnv &env)
{
  vector < char* >::iterator it;

  for (it = strs->begin (); it != strs->end (); ++it)
    {
      cout << "Parsing file: " << *it << endl;
      env.DBLoadPdb (*it, current_tfo_cutoff, current_confo_cutoff);
    }
  env.DBSave ();
}



/** Prints the pdb files.  */

void
CAddPdbStat::_AddPdbStruc::display (ostream &os)
{
  vector< char* >::iterator it;

  
  if (current_tfo_cutoff != 0)
    os << endl << "  tfo_cutoff " << current_tfo_cutoff;
  if (current_confo_cutoff != 0)
    os << endl << "  confo_cutoff " << current_confo_cutoff << endl;
      
  for (it = strs->begin (); it != strs->end (); ++it)
    os << endl << "  \"" << *it << "\"";
}



/** Destructor for CAddPdbStat. */

CAddPdbStat::~CAddPdbStat ()
{
  vector< _AddPdbStruc* >::iterator it;

  for (it = strucs->begin (); it != strucs->end (); ++it)
    delete *it;
  delete strucs;
}



/** Evaluates all _AddPdbStruc in strucs.  These structures contains
    pdb files to load with different confo and tfo cutoffs.  */

void
CAddPdbStat::eval (CEnv &env)
{
#ifndef RELEASE
  vector< _AddPdbStruc* >::iterator it;

  for (it = strucs->begin (); it != strucs->end (); ++it)
    (*it)->eval (env);
#endif
}



/** Prints the statement add_pdb. */

void
CAddPdbStat::display (ostream &os)
{
  vector< _AddPdbStruc* >::iterator it;

  os << "add_pdb ( ";
  for (it = strucs->begin (); it != strucs->end (); ++it)
    (*it)->display (os);
  os << ')' << endl << endl;
}
  
