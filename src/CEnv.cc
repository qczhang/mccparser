//                              -*- mode: C++ -*- 
// CEnv.cc
// Copyright © 1999, 2000 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Wed Sep 29 18:44:36 1999
// Last Modified By : Martin Larose
// Last Modified On : Mon Aug 21 13:24:41 2000
// Update Count     : 4
// Status           : Ok.
// 


#include <unistd.h>

#include "CEnv.h"

#include "CConstraint.h"



/** Constructor for the environment.  It initialises the FragGen librarie,
    database and cache and load them.  */

CEnv::CEnv ()
{
  mHold = 0;
  
  mLib = new CFragGenLib();
  mDB = new CDatabase();
  mCache = new CSetCache ();
  
  // Load the database.
  mDB->Load ();
  // Load the cache.
  mCache->Load ();

  remarks = new vector< const char* > ();
}



/** Destructor for the environment.  It deletes all the allocated memory in
    the environment.  The cache is NOT saved, if you want to save it use the
    quit method before.
*/

CEnv::~CEnv ()
{
  vector< CConstraint* >::iterator it;
  vector< const char* >::const_iterator char_it;
  
  delete mDB;
  delete mCache;
  delete mHold;
  delete mLib;
  for (it = mCstBag.begin (); it != mCstBag.end (); ++it)
    delete *it;
  for (char_it = remarks->begin (); char_it != remarks->end (); ++char_it)
    delete[] *char_it;
  remarks->clear ();
  delete remarks;
}



void
CEnv::ResetDB ()
{
  unlink (gDBName);
  delete mDB;
  mDB = new CDatabase ();
  mCache->Reset ();
}



/** Adds a string in the remark's vector.  */

void
CEnv::AddRemark (const char *str)
{
  char *s = new char[strlen (str) + 1];

  strcpy (s, str);
  remarks->push_back (s);
}



/** This method deletes the residue holder, the FragGen librarie.  It clears
    the constraint vector and the CstBag.  */

void
CEnv::Reset ()
{
  vector< CConstraint* >::iterator it;
  vector< const char* >::const_iterator char_it;

  if (mHold)
    delete mHold;
  mHold = 0;
  
  if (mLib)
    delete mLib;
  mLib = new CFragGenLib ();

  for (it = mvCst.begin (); it != mvCst.end (); it++)
    delete *it;
  mvCst.clear ();
  mCstBag.clear ();

  for (char_it = remarks->begin (); char_it != remarks->end (); ++char_it)
    delete[] *char_it;
  remarks->clear ();
}

