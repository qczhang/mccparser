//                              -*- Mode: C++ -*- 
// Version.cc
// Copyright © 2005 Laboratoire de Biologie Informatique et Théorique
//                  Université de Montréal.
// Author           : Philippe Thibault <philippe.thibault@umontreal.ca>
// Created On       : Wed May 11 10:07:28 2005
// $Revision$
// $Id$
// 

// cmake generated defines
#include <config.h>

#include <sstream>

#include "Version.h"


namespace mccparser
{

  Version::Version ()
    : major_no (-1),
      minor_no (-1),
      revision_no (-1),
      cpu (VERSION_CPU),
      os (VERSION_OS)
  {
    istringstream iss (PACKAGE_VERSION_STRING);
    char dot;

    iss >> this->major_no >> dot >> this->minor_no >> dot >> this->revision_no;
    this->timestamp = __DATE__;
    this->timestamp += " ";
    this->timestamp += __TIME__;
  }


  Version::Version (const string& strv)
    : major_no (-1),
      minor_no (-1),
      revision_no (-1),
      cpu (VERSION_CPU),
      os (VERSION_OS)
  {
    istringstream iss;
    char dot;

    iss.exceptions (ios::failbit | ios::badbit);

    try
    {
      iss.str (strv);
      iss >> this->major_no >> dot 
	  >> this->minor_no >> dot 
	  >> this->revision_no;
    }
    catch (ios::failure& ioex)
    {
      cerr << endl
	   << "ERROR: failed to initialize version to \"" << strv << "\": " << ioex.what () << endl
	   << endl;
      throw;
    }
  }


  Version::Version (const Version& v)
    : major_no (v.major_no),
      minor_no (v.minor_no),
      revision_no (v.revision_no),
      cpu (v.cpu),
      os (v.os),
      timestamp (v.timestamp)
  {

  }


  Version&
  Version::operator= (const Version& v)
  {
    if (this != &v)
    {
      this->major_no = v.major_no;
      this->minor_no = v.minor_no;
      this->revision_no = v.revision_no;
      this->cpu = v.cpu;
      this->os = v.os;
      this->timestamp = v.timestamp;
    }
    return *this;
  }


  bool
  Version::operator== (const Version& v) const
  {
    return 
      this->major_no == v.major_no &&
      this->minor_no == v.minor_no &&
      this->revision_no == v.revision_no &&
      this->cpu == v.cpu &&
      this->os == v.os &&
      this->timestamp == v.timestamp;
  }


  bool
  Version::operator!= (const Version& v) const
  {
    return !this->operator== (v);
  }


  bool 
  Version::equals (const Version& v) const
  {
    return 
      this->major_no == v.major_no && 
      this->minor_no == v.minor_no && 
      this->revision_no == v.revision_no;
  }


  bool
  Version::compatibleWith (const Version& v) const
  {
    return this->major_no == v.major_no && this->minor_no == v.minor_no;
  }


  string
  Version::toString () const
  {
    ostringstream oss;
    oss << PACKAGE_NAME << " " 
	<< this->major_no << "." << this->minor_no << "." << this->revision_no << " " 
	<< this->cpu << " "
	<< this->os << " "
	<< this->timestamp;
    return oss.str ();
  }


  string
  Version::version () const
  {
    ostringstream oss;
    oss << this->major_no << "." << this->minor_no << "." << this->revision_no;
    return oss.str ();
  }


  ostream&
  Version::write (ostream& os) const
  {
    return os << this->Version::toString ();
  }



}

namespace std
{
  ostream&
  operator<< (ostream& os, const mccparser::Version& obj)
  {
    return obj.write (os);
  }
}
