//                              -*- mode: C++ -*- 
// CSourceStat.cc
// Copyright © 1999 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Thu Sep 23 12:59:52 1999
// Last Modified By : Martin Larose
// Last Modified On : Tue Oct 12 19:35:41 1999
// Update Count     : 1
// Status           : Ok.
// 



#include <iostream.h>
#include <vector.h>
#include <stdio.h>

#include "CSourceStat.h"

#include "CEnv.h"
#include "CRepl.h"



/** Evaluation method for the "source" statement.  It generates a new
    read-eval-print loop for batch processign and evaluates the script with
    the current environment.  */

void
CSourceStat::eval (CEnv &env)
{
  CRepl *rep;
  FILE *file = fopen (str, "r");

  if (file)
    {
      rep = new CRepl (file);
      try
	{
	  rep->repl (env);
	}
      catch (...)
	{
	  delete rep;
	  throw;
	}

      delete rep;
      fclose (file);
    }
  else
    cout << "File " << str << " not found." << endl;
}



/** Prints the "source" statement.  */

void
CSourceStat::display (ostream &os)
{
  os << "source ( \"" << str << "\" )" << endl;
}
