//                              -*- mode: C++ -*- 
// CQueryExpr.cc
// Copyright © 1999, 2000 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Fri Sep 24 09:35:58 1999
// Last Modified By : Martin Larose
// Last Modified On : Mon Feb 21 16:31:08 2000
// Update Count     : 2
// Status           : Ok.
// 



#include <string.h>

#include "UMcsymLib.h"

#include "CQueryExpr.h"

#include "CQuery.h"
#include "CQFWrapper.h"
#include "CQueryFunc.h"
#include "CEnv.h"
#include "CSymTable.h"
#include "CEvalException.h"
#include "CTfoQ.h"



/** Evaluation function for the CQTrueFunc.  It only creates a CQueryTFunc
    and returns it.  */

CQueryFunc*
CQTrueFunc::eval () const
{
  return new CQueryTFunc ();
}



/** Evaluation function for the CQIdentFunc.  It searches the string in the
    symbol table and returns a CQueryIFunc.  If the string is not in the
    symbol table a CEvalException is thrown.  */

CQueryFunc*
CQIdentFunc::eval () const
{
  const CSymbol *sym = gSymTable.GetSymbol (str, "DATABASE");
  
  if (!sym)
    {
      CEvalException exc ("*** Property ");

      throw exc << str << " doesn't exist.  Symbol ignored";
    }
  else
    return new CQueryIFunc (sym);
}



/** Evaluation function for CQNotFunc.  Returns a new CQueryNFunc.  */

CQueryFunc*
CQNotFunc::eval () const
{
  return new CQueryNFunc (fn->eval ());
}



/** Evaluation function for CQAndFunc.  Returns a new CQueryAFunc with the
    left and the right branches evaluated.  */

CQueryFunc*
CQAndFunc::eval () const
{
  return new CQueryAFunc (left->eval (), right->eval ());
}



/** Evaluation function for CQOrFunc.  Returns a new CQueryOFunc with the
    left and the right branches evaluated.  */

CQueryFunc*
CQOrFunc::eval () const
{
  return new CQueryOFunc (left->eval (), right->eval ());
}



/** Destructor for CQueryExpr.  Deletes the file strings and the query
    functions.  */

CQueryExpr::~CQueryExpr ()
{
  vector< char* >::iterator it;

  for (it = strs.begin (); it != strs.end (); ++it)
    delete[] *it;
  if (fn)
    delete fn;
}



/** Evaluation method for the query expressions.  It builds a query
    containing a filename symbol and/or some properties.  The query is
    returned.  */

CQuery*
CQueryExpr::eval (CEnv &env)
{
  CQuery *qu = new CQuery ();
  vector< char* >::iterator it;

  gSymTable.SetContext ("DATABASE");
  for (it = strs.begin (); it != strs.end (); ++it)
    {
      const CSymbol *sym = gSymTable.GetSymbol (*it);

      if (sym)
	qu->AddFileName (sym);
      else
	cout << "Unknown file name \"" << *it << "\" in query expression."
	     << endl;
    }
  qu->SetPropFunc (fn->eval ());
  return qu;
}



/** Displays the query to the ostream.  */

void
CQueryExpr::display (ostream &os)
{
  vector< char* >::iterator it;
  
  os << "{";
  for (it = strs.begin (); it != strs.end (); ++it)
    os << " file (\"" << *it << "\")";
  os << endl << "   ";
  fn->display (os);
  os << " }";
}
