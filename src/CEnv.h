//                              -*- mode: C++ -*- 
// CEnv.h
// Copyright © 1999, 2000 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Wed Sep 29 18:42:52 1999
// Last Modified By : Martin Larose
// Last Modified On : Mon Aug 21 13:24:43 2000
// Update Count     : 4
// Status           : Ok.
// 



#ifndef _CEnv_h_
#define _CEnv_h_


/** This class contains the objects for a mcsym run.  This objet must be
    generated before the evaluation of the AST constructed during the
    parsing of a file or before the read-eval-loop of the interpreter.  The
    object is passed to the eval method of the AST.  Each node of the AST
    acts on this environment and modifies it.
*/


#include <vector.h>
#include <map.h>
#include <string.h>

#include "UMcsymLib.h"

#include "CResidue.h"
#include "CSet.h"
#include "CSetCache.h"
#include "CDatabase.h"
#include "CFragGenLib.h"
#include "CFragGen.h"
#include "CResHolder.h"

class CConstraint;


class CEnv
{
  CDatabase   *mDB;               // The confo/tfo database
  CSetCache   *mCache;            // The cache.
  CResHolder  *mHold;             // The residue repository
  CFragGenLib *mLib;              // The FragGen library
  vector< CConstraint* > mCstBag; // 
  vector< CConstraint* > mvCst;   // To keep track of all automatically 
                                  // generated constraints 
  vector< const char* > *remarks;
  
public:

  CEnv ();
  ~CEnv ();

  vector< CConstraint* >& GetCstBag () { return mCstBag; }
  void AddCst (CConstraint *cst) { mCstBag.push_back (cst); }
  void AddCstBag (vector< CConstraint* > &cst_v)
  { mCstBag.insert (mCstBag.end (), cst_v.begin (), cst_v.end ()); }

  void CstInsert (vector< CConstraint* > &cst_v)
  { mvCst.insert (mvCst.end (), cst_v.begin (), cst_v.end ()); }

  CSet< CTfoL* > GetSet (const CTfoQ &query,
			 bool shouldSort = true,
			 unsigned int sortSize = 0) 
  { return mCache->GetSet (*mDB, query, shouldSort, sortSize); }

  CSet< CResL* > GetSet (const CResQ &query,
			 bool shouldSort = true,
			 unsigned int sortSize = 0) 
  { return mCache->GetSet (*mDB, query, shouldSort, sortSize); }

  void DBLoadPdb (char *file, float ctc, float ccc)
  { mDB->Load_pdb (file, ctc, ccc); }
  void DBSave () { mDB->Save (); }
  void ResetDB ();

  void SaveCache () { mCache->Save (); }
  
  void PutNote (char *note) { mDB->PutNote (note); }
  void PrintNotes () { mDB->PrintNotes (); }

  CFragGen* GetFG (char *str) { return (*mLib)[str]; }
  void AddFG (CFragGen *fg) { mLib->push_back (fg); }

  void NewHold () { mHold = new CResHolder (); }
  CResHolder& GetHold () { return *mHold; }
  CResHolder* GetHoldPtr () { return mHold; }
  void SetHold (CResHolder *rh) { mHold = rh; }
  void SetHold (const CResHolder &rh) { *mHold = rh; }
  
  void AddSequence (SeqType theType, int nFirst, const char *theSeq,
		    char theChainId)
  { mHold->AddSequence (*mDB, *mCache, theType, nFirst, theSeq, theChainId); }

  const vector< const char* >* GetRemarks () const { return remarks; }
  void AddRemark (const char *str);
  
  void Reset ();
  void Quit () { SaveCache (); }
};


#endif
