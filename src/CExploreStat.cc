//                              -*- mode: C++ -*- 
// CExploreStat.cc
// Copyright © 1999, 2000 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Thu Sep 23 12:53:11 1999
// Last Modified By : Martin Larose
// Last Modified On : Mon Aug 21 13:24:52 2000
// Update Count     : 6
// Status           : Ok.
// 



#include <iostream.h>
#include <iomanip.h>
#include <vector.h>
#include <string.h>
#include <sys/param.h>

#include "CExploreStat.h"

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



/** Evaluation function for the "explore" statement.  It generates the
    models under the specifications of the other statements.  */

void
CExploreStat::eval (CEnv &env)
{
  CFragGen *tmp_fg;
  unsigned int count = 0;
  int outCount = 0;
  bool dump_indiv_pdb = false;
  int limit_first = 0;
  int limit_every = 0;
  float limit_prob = 0.0;

  tmp_fg = fg_struc->eval (env);
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
      cout << "PDB structures will be saved to file named {" 
	   << form << "}" << endl;
    }

  tmp_fg->AddConstraint (env.GetCstBag ());
  tmp_fg->Ready ();

  CResHolder rh (env.GetHoldPtr ()->GetPtr (tmp_fg->Const_Addresses ()));
  rh.Sort ();  

  // Consistency check
  cout << "Checking consistency of FG hierarchy..." << endl;
  if (! tmp_fg->IsConsistent ())
    {
      CEvalException exc ("ERROR : Consistency check failed.");
      throw (exc);
    }
 
  cout << "Starting exploration on " << tmp_fg->GetName () << endl << endl;

  CStatusMgr smgr (tmp_fg, &rh, &count);

  try
    {
      tmp_fg->Start ();
      while (tmp_fg->Advance ())
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
		      WriteRemark (*of, tmp_fg, env.GetRemarks ());
		      *of << rh;
		      of->close ();
		      delete of;
		    }
		}
	      outCount++;
	      
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
  tmp_fg->Report (cout);
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



/** Prints the "explore" statement.  */

void
CExploreStat::display (ostream &os)
{
  os << "explore (" << endl
     << "  ";
  fg_struc->display (os);
  os << endl;
  if (form)
    {
      os << "  fileName_pdb ( \"" << form << "\" )";
      if (zipped)
	os << " zipped";
      os << endl;
    }
  os << ')' << endl << endl;
}
  
