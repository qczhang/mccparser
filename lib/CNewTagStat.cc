//                              -*- mode: C++ -*- 
// CNewTagStat.cc
// Copyright © 1999, 2000 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Thu Sep 23 14:37:39 1999
// Last Modified By : Martin Larose
// Last Modified On : Wed Apr 26 17:49:48 2000
// Update Count     : 3
// Status           : Ok.
// 



#include <iostream.h>

#include "CNewTagStat.h"

#include "CQueryExpr.h"
#include "CEnv.h"
#include "CSet.h"
#include "CQuery.h"
#include "CTfoQ.h"
#include "CResQ.h"
#include "CSymTable.h"



/** Destructor for the statement "new_tag".  */

CNewTagStat::~CNewTagStat ()
{
  vector< CQueryExpr* >::iterator it;

  delete[] id;
  for (it = exprs->begin (); it != exprs->end (); it++)
    delete *it;
  delete exprs;
}



/** Evaluation method for the statement "new_tag".  It adds the property id
    to the query expressions.  The database is saved after the operation.  */

void
CNewTagStat::eval (CEnv &env)
{
#ifndef RELEASE
  CQuery *restrict;
  vector< CQueryExpr* >::iterator it;

  if (resq_opt)
    {
      for (it = exprs->begin (); it != exprs->end (); it++)
	{
	  CResQ the_q;
	  CSet< CResL* > tmp_set;
      
	  restrict = (*it)->eval (env);
	  the_q.Restrict (restrict);
	  tmp_set = env.GetSet (the_q, false);
	  tmp_set.AddProp (gSymTable (id, "DATABASE"));
	  cout << "Added property " << id << " to " << tmp_set.size ()
	       << " confo elements." << endl;
	  delete restrict;
	}
    }
  else
    {
      for (it = exprs->begin (); it != exprs->end (); it++)
	{
	  CTfoQ the_q;
	  CSet< CTfoL* > tmp_set;
	  
	  restrict = (*it)->eval (env);
	  the_q.Restrict (restrict);
	  tmp_set = env.GetSet (the_q, false);
	  tmp_set.AddProp (gSymTable (id, "DATABASE"));
	  cout << "Added property " << id << " to " << tmp_set.size ()
	       << " transfo elements." << endl;
	  delete restrict;
	}
    }
  env.DBSave ();
#endif
}



/** Prints the "new_tag" statement.  */

void
CNewTagStat::display (ostream &os)
{
  vector< CQueryExpr* >::iterator it;

  os << "new_tag ( ";
  if (resq_opt)
    os << "res ";
  else
    os << "tfo ";
  os << " \"" << id << "\"";
  for (it = exprs->begin (); it != exprs->end (); it++)
    {
      os << endl << "  ";
      (*it)->display (os);
    }
  os << " )" << endl << endl;
}
