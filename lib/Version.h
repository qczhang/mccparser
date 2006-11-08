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


namespace mccore
{
  class iBinstream;
  class oBinstream;
}


using namespace std;
using namespace mccore;


namespace mccparser
{

  /**
   * @short Library version manipulation
   * 
   * @author Philippe Thibault <philippe.thibault@umontreal.ca>
   */
  class Version 
  {
  protected:

    /**
     * Major version number: <major>.<minor>.<revision>
     */
    short int major_no;

    /**
     * Minor version number: <major>.<minor>.<revision>
     */
    short int minor_no;

    /**
     * Revision version number: <major>.<minor>.<revision>
     */
    short int revision_no;

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

    Version (const string& strv);

    Version (const Version& v);

    /**
     * Destructs the object.
     */
    virtual ~Version () { }

    // OPERATORS ------------------------------------------------------------

    Version& operator= (const Version& v);

    virtual bool operator== (const Version& v) const;

    virtual bool operator!= (const Version& v) const;

    // ACCESS ---------------------------------------------------------------

    int getMajor () const
    {
      return this->major_no;
    }

    int getMinor () const
    {
      return this->minor_no;
    }

    int getRevision () const
    {
      return this->revision_no;
    }

    // METHODS --------------------------------------------------------------

    bool equals (const Version& v) const;

    bool compatibleWith (const Version& v) const;

    virtual string toString () const;

    string version () const;

    // I/O  -----------------------------------------------------------------

    /**
     * Writes the object to a stream.
     * @param os The stream.
     * @return The written stream.
     */
    virtual ostream& write (ostream& os) const;

    /**
     * Writes the object to a binary stream.
     * @param obs The stream.
     * @return The written stream.
     */
    virtual oBinstream& write (oBinstream& obs) const;

    /**
     * Reads the object from a binary stream.
     * @param ibs The stream.
     * @return The read stream.
     */
    virtual iBinstream& read (iBinstream& ibs);

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

namespace mccore
{
  /**
   * Writes the object to a binary stream.
   * @param obs The stream.
   * @param obj The object to write.
   * @return The written stream.
   */
  oBinstream& operator<< (oBinstream &obs, const mccparser::Version& obj);

  /**
   * Reads the object from a binary stream.
   * @param ibs The stream.
   * @param obj The object to read.
   * @return The read stream.
   */
  iBinstream& operator>> (iBinstream &ibs, mccparser::Version& obj);
}

#endif
