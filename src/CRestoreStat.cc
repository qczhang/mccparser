//                              -*- Mode: C++ -*- 
// CRestoreStat.cc
// Copyright © 2000 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Wed Jun 21 17:34:42 2000
// Last Modified By : Martin Larose
// Last Modified On : Mon Aug 21 13:25:14 2000
// Update Count     : 2
// Status           : Unknown.
// 


#include <iostream.h>
#include <iomanip.h>
#include <vector.h>
#include <string.h>
#include <sys/param.h>

#include "CRestoreStat.h"

#include "CEnv.h"
#include "CFragGen.h"
#include "CConstraint.h"
#include "CResHolder.h"
#include "CEvalException.h"
#include "CException.h"
#include "CStatusMgr.h"
#include "CFragGenLib.h"
#include "CAngleCst.h"
#include "CClashCst.h"
#include "CCycleCst.h"
#include "CDistCst.h"
#include "CTorsionCst.h"
#include "Pdbstream.h"
#include "fPdbstream.h"
#include "zfPdbstream.h"



/** Evaluation function for the "restore" statement.  It restores the status
    of an earlier interrupted mcsym run via its .msf file and restart the
    run.  */

void
CRestoreStat::eval (CEnv &env)
{
  unsigned int count = 0;
  unsigned int oldCount;
  int outCount = 0;
  int limit_first = 0;
  int limit_every = 0;
  float limit_prob = 0.0;
  bool dump_indiv_pdb = false;
  CFragGen *fg;
  CStatusMgr smgr (&count);

  if (form != 0)
    {
      if (zipped)
	{
	  char *str = new char[strlen (form) + 1 + 3];
	  
	  strcpy (str, form);
	  strcat (str, ".gz");
	  delete[] form;
	  form = str;
	}
      dump_indiv_pdb = true;
      cout << "PDB structures will be saved to files named {" 
	   << form << "}" << endl;
    }

  env.Reset ();
  smgr.RestoreStatus (filename);
  fg = smgr.GetFG ();
  env.AddFG (fg);
  env.SetHold (smgr.GetRH ());
  
  cout << "Checking consistency of FG hierarchy..." << endl;
  if (! fg->IsConsistent ())
    throw CEvalException ("ERROR : Consistency check failed.");
  
  count = smgr.GetCount ();
  outCount = count;

  fg->Reapply ();
  const vector< CFragGen* > &fg_list = fg->ListFG ();
  vector< CFragGen* >::const_iterator fg_it;
  
  for (fg_it = fg_list.begin (); fg_it != fg_list.end (); ++fg_it)
    env.AddFG (*fg_it);
  
  cout << "Restarting exploration on " << fg->GetName () << endl;

  for (oldCount = 0; oldCount != count; ++oldCount)
    {
      if (oldCount % 50 == 0)
	cout << endl << setw (8) << oldCount << ' ';
      else if (oldCount % 10 == 0)
	cout << ' ';
      cout << ',' << flush;
    }
  try
    {
      while (fg->Advance ())
	{
	  if (count % 50 == 0)
	    cout << endl << setw (8) << count << ' ';
	  else if (count % 10 == 0)
	    cout << ' ';
	  
	  count++;
	  
	  if ((!limit_every && limit_prob == 0.0)
	      || (limit_every && count % limit_every == 1)
	      || (limit_prob != 0.0 && ((float)rand () / 32768) < limit_prob))
	    {
	      if (dump_indiv_pdb)
		{
		  char filename[MAXPATHLEN];
		  oPdbstream *of;
		  
		  sprintf (filename, form, count);
		  if (zipped)
		    of = new ozfPdbstream (filename);
		  else
		    of = new ofPdbstream (filename);
		  if (! *of)
		    cerr << "File " << filename << " could not be opened."
			 << endl;
		  else
		    {
		      WriteRemark (*of, fg, env.GetRemarks ());
		      *of << env.GetHold ();
		      of->close ();
		      delete of;
		    }
		}
	      ++outCount;
	      cout << '.' << flush;
	    }
	  else
	    cout << '+' << flush;
	  
	  if (outCount == limit_first)
	    break;
	}
      
      if (limit_every || limit_prob != 0.0)
	cout << endl << count << " structures explored." << endl;
      if (limit_first == outCount)
	cout << endl << endl << outCount
	     << " structures generated...  There may be more!"
	     << endl;
      else
	{
	  cout << endl << endl << outCount
	       << " structures generated...  The search was complete."
	       << endl;
	  smgr.UnlinkStatusFile ();
	}
    }
  catch (CInterruptException &x)
    {
      cout << endl << endl << count << " structures explored." << endl
	   << "The search was stop with an interrupt signal..." << endl;
    }
  
  cout << endl;
  fg->Report (cout);
  cout << endl;
  
  cout << "Angle constraint report:" << endl;
  CAngleCst ().ReportGlobal (cout);
  cout << endl;
  
  cout << "Distance constraint report:" << endl;
  CDistCst ().ReportGlobal (cout);
  cout << endl;

  cout << "Clash constraint report:" << endl;
  CClashCst ().ReportGlobal (cout);
  cout << endl;

  cout << "Cycle constraint report:" << endl;
  CCycleCst ().ReportGlobal (cout);
  cout << endl;

  cout << "Torsion constraint report:" << endl;
  CTorsionCst ().ReportGlobal (cout);
  cout << endl;
}



/** Prints the "restore" statement.  */

void
CRestoreStat::display (ostream &os)
{
  os << "restore (" << filename;
  if (form)
    {
      os << " fileName_pdb (\"" << form << "\")";
      if (zipped)
	os << " zipped";
    }
  os << ')' << endl << endl;
}
