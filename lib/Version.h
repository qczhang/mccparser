//                              -*- Mode: C++ -*- 
// Version.h
// Copyright © 2005 Laboratoire de Biologie Informatique et Théorique
//                  Université de Montréal.
// Author           : Philippe Thibault <philippe.thibault@umontreal.ca>
// Created On       : Wed May 11 10:07:28 2005
// $Revision$
// $Id$
// 


#ifndef _mccparser_Version_h_
#define _mccparser_Version_h_


#include <iostream>
#include <string>


using namespace std;


namespace mccparser
{

  /**
   * @short Library version manipulation
   * 
   * @author Philippe Thibault <philippe.thibault@umontreal.ca>
   */
  class Version 
  {
 
    /**
     * Major version number: <major>.<minor>
     */
    int major_version;

    /**
     * Minor version number: <major>.<minor>
     */
    int minor_version;

    /**
     * Current CPU architecture string.
     */
    string cpu;

    /**
     * Current vendor string.
     */
    string vendor;

    /**
     * Current operating system name string.
     */
    string os;

    /**
     * Current building timestamp.
     */
    string timestamp;
   
  public:

    // LIFECYCLE ------------------------------------------------------------

    /**
     * Initializes the object.
     */
    Version ();

    Version (const Version& v);

    /**
     * Destructs the object.
     */
    ~Version () { }

    // OPERATORS ------------------------------------------------------------

    Version& operator= (const Version& v);

    bool operator== (const Version& v) const;

    bool operator!= (const Version& v) const
    {
      return !this->operator== (v);
    }

    // ACCESS ---------------------------------------------------------------

    int getMajorVersion () const
    {
      return this->major_version;
    }

    int getMinorVersion () const
    {
      return this->minor_version;
    }

    // METHODS --------------------------------------------------------------

    string toString () const;

    // I/O  -----------------------------------------------------------------

    /**
     * Writes the object to a stream.
     * @param os The stream.
     * @return The written stream.
     */
    ostream& write (ostream& os) const;

  };

}

namespace std
{
  /**
   * Writes the object to a stream.
   * @param obs The stream.
   * @param obj The object to write.
   * @return The written stream.
   */
  ostream& operator<< (ostream &os, const mccparser::Version& obj);
}

#endif
