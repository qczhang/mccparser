//                              -*- mode: C++ -*- 
// CBacktrackExpr.cc
// Copyright © 1999, 2000 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Thu Sep 23 19:26:37 1999
// Last Modified By : Martin Larose
// Last Modified On : Fri May 12 12:02:45 2000
// Update Count     : 3
// Status           : Ok.
// 



#include <stdio.h>

#include "CBacktrackExpr.h"

#include "CFragGen.h"
#include "CEnv.h"
#include "CResidue.h"
#include "CBacktrackFG.h"
#include "CTransfoFG.h"
#include "CEvalException.h"



/** Evaluation method of the fraggen reference in the "backtrack"
    expression.  It sets the flag global reference of the caller.
*/

void
CBacktrackExpr::_FGStruc::eval (CEnv &env, CBacktrackFG *bt, bool *gref)
{
  if (!*gref)
    {
      CFragGen *tmp_fg = fg_struc->eval (env);

      cout << "Adding global reference " << tmp_fg->GetName()
	   << " to backtrack." << endl;
      bt->AddFG (tmp_fg);
      *gref = true;
    }
}



/** Destructor of the nodes of the different residue references in the
    "backtrack" expression.
*/

CBacktrackExpr::_BTStruc::~_BTStruc ()
{
  vector< CResidueName* >::iterator it;

  delete ref;
  for (it = res_v->begin (); it != res_v->end (); it++)
    delete *it;
  delete res_v;
}



/** Evaluation method of the residue references in the "backtrack"
    expression.  If the global FragGen is not set yet it will refers to
    the FG related to the residue identified by ref.  If other residues are
    present their bindings will be added to the FG, ex:
                    ref ... resn

	            ref -> res1     will be added
		        ...
		    resn-1 -> resn  will be added
*/

void
CBacktrackExpr::_BTStruc::eval (CEnv &env, CBacktrackFG *bt, bool *gref)
{
  CResidue *tmp_res;
  CResidue *tmp_ref = 0;
  vector< CResidueName* >::iterator it;
  char ref_name[256];
  char res_name[256];
      
  tmp_ref = ref->eval (env);
  sprintf (ref_name, "%c%d", tmp_ref->GetChainId (), tmp_ref->GetResNo ());
  if (!*gref)
    {
      cout << "Adding global reference " << ref_name << " to backtrack."
	   << endl;
      bt->AddFG (env.GetFG (ref_name));
      *gref = true;
    }

  for (it = res_v->begin (); it != res_v->end (); it++)
    {
      char tmp_name[512];
      
      tmp_res = (*it)->eval (env);
      sprintf (res_name, "%c%d", tmp_res->GetChainId (), tmp_res->GetResNo ());
      sprintf (tmp_name, "%s -> %s", ref_name, res_name);
	       
      cout << "Adding build order: " << tmp_name << endl;
      bt->AddFG (env.GetFG (tmp_name));
      tmp_ref = tmp_res;
      strcpy (ref_name, res_name);
    }
}
  


/** Prints the AST for the residues part of the "backtrack" expression. */

void
CBacktrackExpr::_BTStruc::display (ostream &os)
{
  vector< CResidueName* >::iterator it;

  os << "( ";
  ref->display (os);
  os << ' ';
  for (it = res_v->begin (); it != res_v->end (); it++)
    {
      (*it)->display (os);
      os << ' ';
    }
  os << ')';
}



/** Evaluation method of the "place" statement in the "backtrack"
    expression.  It places the residues FG relative to the FG reference.
*/

void
CBacktrackExpr::_PlaceStruc::eval (CEnv &env, CBacktrackFG *bt, bool *gref)
{
  CResidue *tmp_ref;
  CResidue *tmp_res;
  CFragGen *tmp_fg;
  CFragGen *orig;
  char fg_name[256];
  
  tmp_ref = ref->eval (env);
  tmp_res = res->eval (env);
  tmp_fg = fg_struc->eval (env);

  sprintf (fg_name, "%c%d -> %c%d", tmp_ref->GetChainId (),
	   tmp_ref->GetResNo (), tmp_res->GetChainId (),
	   tmp_res->GetResNo ());
	   
  orig = env.GetFG (fg_name);
  CTransfoFG *tmpFg = 0;

  if (dynamic_cast< CTransfoFG* > (orig))
    tmpFg = new CTransfoFG (*(CTransfoFG*)orig);
  else
    throw CEvalException ("Invalid CTranfoFG in backtrack.");
      
  tmpFg->ChangeToFrag (tmp_fg);
  // This fraggen is unamed and not registered in the library...
  bt->AddFG (tmpFg);
}



/** Prints the AST for the "place" statement of the "backtrack" expression.
 */

void
CBacktrackExpr::_PlaceStruc::display (ostream &os)
{
  os << "place ( ";
  ref->display (os);
  os << ' ';
  res->display (os);
  os << ' ';
  fg_struc->display (os);
  os << " )" << endl;
}



/** Destructor for all the substatements in the "backtrack" expression.
 */

CBacktrackExpr::~CBacktrackExpr ()
{
  vector< _GenBTStruc* >::iterator it;

  for (it = strucs->begin (); it != strucs->end (); it++)
    delete *it;
  delete strucs;
}



/** Evaluation function for the "backtrack" expression.  It goes through all
    the substatements of this AST.
*/

CFragGen*
CBacktrackExpr::eval (CEnv &env)
{
  CBacktrackFG *bt = new CBacktrackFG ();
  vector< _GenBTStruc* >::iterator it;
  bool globalRef = false;

  for (it = strucs->begin (); it != strucs->end (); it++)
    (*it)->eval (env, bt, &globalRef);
  return bt;
}



/** Prints the AST for the "backtrack" expression.  */

void
CBacktrackExpr::display (ostream &os)
{
  vector< _GenBTStruc* >::iterator it;

  os << "backtrack ( " << endl;
  for (it = strucs->begin (); it != strucs->end (); it++)
    {
      os << "  ";
      (*it)->display (os);
      os << endl;
    }
  os << ')' << endl;
}
