//                              -*- mode: C++ -*- 
// CEvalException.cc
// Copyright © 1999, 2000 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Wed Oct  6 16:13:03 1999
// Last Modified By : Martin Larose
// Last Modified On : Mon Aug 21 13:24:47 2000
// Update Count     : 6
// Status           : Ok.
// 



#include <iostream.h>
#include <string.h>
#include <stdio.h>

#include "CEvalException.h"



CEvalException::CEvalException (const char *s)
{
  str = new char[strlen (s) + 1];
  strcpy (str, s);
}



CEvalException::CEvalException (const CEvalException &other)
{
  str = new char[strlen (other.str) + 1];
  strcpy (str, other.str);
}



const CEvalException&
CEvalException::operator= (const CEvalException &other)
{
  if (this != &other)
    {
      delete[] str;
      str = new char[strlen (other.str) + 1];
      strcpy (str, other.str);
    }
  return *this;
}



CEvalException&
CEvalException::operator<< (char c)
{
  char *tmp = new char[strlen (str) + 1 + 1];

  sprintf (tmp, "%s%c", str, c);
  delete[] str;
  str = tmp;
  return *this;
}



CEvalException&
CEvalException::operator<< (int i)
{
  char *tmp = new char[strlen (str) + 256];

  sprintf (tmp, "%s%d", str, i);
  delete[] str;
  str = tmp;
  return *this;
}



CEvalException&
CEvalException::operator<< (unsigned int ui)
{
  char *tmp = new char[strlen (str) + 256];

  sprintf (tmp, "%s%u", str, ui);
  delete[] str;
  str = tmp;
  return *this;
}



CEvalException&
CEvalException::operator<< (const char *s)
{
  char *tmp = new char[strlen (str) + strlen (s) + 1];

  sprintf (tmp, "%s%s", str, s);
  delete[] str;
  str = tmp;
  return *this;
}



/** Output method of the exception.  */

ostream&
operator<< (ostream &os, const CEvalException &ex)
{
  return os << ex.str;
}



CParserException::CParserException (const char *s)
{
  str = new char[strlen (s) + 1];
  strcpy (str, s);
}



CParserException::CParserException (const CParserException &right)
{
  str = new char[strlen (right.str) + 1];
  strcpy (str, right.str);
}



const CParserException&
CParserException::operator= (const CParserException &right)
{
  if (this != &right)
    {
      delete[] str;
      str = new char[strlen (right.str) + 1];
      strcpy (str, right.str);
    }
  return *this;
}



ostream&
operator<< (ostream &os, const CParserException &ex)
{
  return os << ex.str;
}
