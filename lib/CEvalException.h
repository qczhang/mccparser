//                              -*- mode: C++ -*- 
// CEvalException.h
// Copyright © 1999, 2000 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Wed Sep 29 11:57:45 1999
// Last Modified By : Martin Larose
// Last Modified On : Mon Aug 21 13:24:50 2000
// Update Count     : 6
// Status           : Ok.
// 



#ifndef _CEvalException_h_
#define _CEvalException_h_


/** General class of exceptions used within the evaluation of mcsym
    commands.  Theses exceptions are exclusively used for errors generated
    from a badly written script (user error).  It is not intended for
    internal errors.  These errors must be handled via an other type of
    exception, via error message and abort or via an error message and exit.
*/



class ostream;


class CEvalException
{
  char *str;

  CEvalException () { }

public:

  // LIFECYCLE ------------------------------------------------------------

  CEvalException (const char *s);
  CEvalException (const CEvalException &other);
  ~CEvalException () { delete[] str; }

  // OPERATORS ------------------------------------------------------------

  const CEvalException& operator= (const CEvalException &other);
  CEvalException& operator<< (char c);
  CEvalException& operator<< (int i);
  CEvalException& operator<< (unsigned int ui);
  CEvalException& operator<< (const char *s);

  // ACCESS ---------------------------------------------------------------

  // METHODS --------------------------------------------------------------

  // I/O ------------------------------------------------------------------
  
  friend ostream& operator<< (ostream &os, const CEvalException &ex);
};




/** Exception class used for parsing errors.  */

class CParserException
{
  char *str;

  CParserException () { }

public:

  // LIFECYCLE ------------------------------------------------------------

  CParserException (const char *s);
  CParserException (const CParserException &right);
  ~CParserException () { delete[] str; }

  // OPERATORS ------------------------------------------------------------

  const CParserException& operator= (const CParserException &right);

  // ACCESS ---------------------------------------------------------------

  // METHODS --------------------------------------------------------------

  // I/O ------------------------------------------------------------------
  
  friend ostream& operator<< (ostream &os, const CParserException &ex);
};

#endif
