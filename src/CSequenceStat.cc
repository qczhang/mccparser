//                              -*- mode: C++ -*- 
// CSequenceStat.cc
// Copyright © 1999, 2000 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Wed Sep 22 20:28:46 1999
// Last Modified By : Martin Larose
// Last Modified On : Mon Aug 21 13:25:21 2000
// Update Count     : 7
// Status           : Ok.
// 


#include <iostream.h>

#include "CSequenceStat.h"

#include <UMcsymLib.h>

#ifdef RELEASE
#include <CLicense.h>
#endif

#include "CEnv.h"
#include <CResHolder.h>
#include <CException.h>
#include "CEvalException.h"



/** Evaluation method for the "sequence" statement.  It fills the
    environment residue holder with residues specified in the statement.  */

void
CSequenceStat::eval (CEnv &env)
{
  SeqType seq_type = SeqType_DNA;
  unsigned int i;

  switch (type)
    {
    case 'd':
      seq_type = SeqType_DNA;
      break;
    case 'r':
      seq_type = SeqType_RNA;
      break;
    default:
      CEvalException exc ("Sequence statement error, invalid type ");

      throw exc << type << ".";
    }

#ifdef RELEASE
  if (glic->isdemo () && strlen (seq) > 9)
    {
      CException exc ("This is a demo version of MCSYM.\n");

      throw exc << "Please contact us at http://www-lbit.iro.umontreal.ca\n"
		<< "or mailto:adbit@iro.umontreal.ca\n"
		<< "for the release version.";
    }
#endif
  if (!env.GetHoldPtr ())
    env.NewHold ();

  for (i = 0; i < strlen (seq); i++)
    {
      if (env.GetHoldPtr ()->GetPtr (res_name->GetNo (), res_name->GetId ()))
	{
	  CEvalException exc ("Sequence statement error, residue ");

	  throw exc << res_name->GetId () << res_name->GetNo () + i
		    << " has already been allocated.";
	}
    }
  env.AddSequence (seq_type, res_name->GetNo (), seq, res_name->GetId ());
}



/** Prints the "sequence" statement.  */

void
CSequenceStat::display (ostream &os)
{
  os << "sequence ( " << type << ' ';
  res_name->display (os);
  os << ' ' << seq << " )" << endl << endl;
}
