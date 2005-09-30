//                              -*- Mode: C++ -*- 
// Version.cc
// Copyright © 2005 Laboratoire de Biologie Informatique et Théorique
//                  Université de Montréal.
// Author           : Philippe Thibault <philippe.thibault@umontreal.ca>
// Created On       : Wed May 11 10:07:28 2005
// $Revision$
// $Id$
// 

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


#include <sstream>

#include "Version.h"


namespace mccparser
{

  Version::Version ()
    : major_version (-1),
      minor_version (-1),
      cpu (VERSION_CPU),
      vendor (VERSION_VENDOR),
      os (VERSION_OS)
  {
    istringstream iss (VERSION);
    char dot;

    iss >> this->major_version >> dot >> this->minor_version;
    this->timestamp = __DATE__;
    this->timestamp += " ";
    this->timestamp += __TIME__;
  }
  

  Version::Version (const Version& v)
    : major_version (v.major_version),
      minor_version (v.minor_version),
      cpu (v.cpu),
      vendor (v.vendor),
      os (v.os),
      timestamp (v.timestamp)
  {

  }


  Version&
  Version::operator= (const Version& v)
  {
    if (this != &v)
    {
      this->major_version = v.major_version;
      this->minor_version = v.minor_version;
      this->cpu = v.cpu;
      this->vendor = v.vendor;
      this->os = v.os;
      this->timestamp = v.timestamp;
    }
    return *this;
  }


  bool
  Version::operator== (const Version& v) const
  {
    return 
      this->major_version == v.major_version &&
      this->minor_version == v.minor_version &&
      this->cpu == v.cpu &&
      this->vendor == v.vendor &&
      this->os == v.os &&
      this->timestamp == v.timestamp;
  }


  string
  Version::toString () const
  {
    ostringstream oss;
    oss << PACKAGE << " " 
	<< this->major_version << "." << this->minor_version << " " 
	<< this->cpu << " "
	<< this->vendor << " "
	<< this->os << " "
	<< this->timestamp;
    return oss.str ();
  }


  ostream&
  Version::write (ostream& os) const
  {
    return os << this->toString ();
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
