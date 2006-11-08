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

#include "mccore/Binstream.h"
#include "mccore/Messagestream.h"

#include "Version.h"


namespace mccparser
{

  Version::Version ()
    : major_no (-1),
      minor_no (-1),
      revision_no (-1),
      cpu (VERSION_CPU),
      vendor (VERSION_VENDOR),
      os (VERSION_OS)
  {
    istringstream iss (VERSION);
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
      vendor (VERSION_VENDOR),
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
      IntLibException ex ("", __FILE__, __LINE__);
      ex << "failed to initialize version to \"" << strv << "\": " << ioex.what ();
      throw ex;
    }
  }


  Version::Version (const Version& v)
    : major_no (v.major_no),
      minor_no (v.minor_no),
      revision_no (v.revision_no),
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
      this->major_no = v.major_no;
      this->minor_no = v.minor_no;
      this->revision_no = v.revision_no;
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
      this->major_no == v.major_no &&
      this->minor_no == v.minor_no &&
      this->revision_no == v.revision_no &&
      this->cpu == v.cpu &&
      this->vendor == v.vendor &&
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
    oss << PACKAGE << " " 
	<< this->major_no << "." << this->minor_no << "." << this->revision_no << " " 
	<< this->cpu << " "
	<< this->vendor << " "
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


  oBinstream&
  Version::write (oBinstream& obs) const
  {
    return obs << this->major_no << this->minor_no << this->revision_no
	       << this->cpu
	       << this->vendor
	       << this->os
	       << this->timestamp;
  }


  iBinstream& 
  Version::read (iBinstream& ibs)
  {
    Version saved = *this;

    this->cpu = this->vendor = this->os = this->timestamp = "unread";
    this->major_no = this->minor_no = this->revision_no = -1;

    ibs >> this->major_no >> this->minor_no >> this->revision_no
	>> this->cpu 
	>> this->vendor 
	>> this->os
	>> this->timestamp;

    if (*this != saved)
      gErr (5) << "Warning: reading data created from package version: " << endl
	       << "\t" << *this << endl
	       << "using package version: " << endl
	       << "\t" << saved << endl;

    return ibs;
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

namespace mccore
{
  oBinstream& 
  operator<< (oBinstream &obs, const mccparser::Version& obj)
  {
    return obj.write (obs);
  }


  iBinstream& 
  operator>> (iBinstream &ibs, mccparser::Version& obj)
  {
    return obj.read (ibs);
  }
}
