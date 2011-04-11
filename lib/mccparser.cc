//                              -*- Mode: C++ -*- 
// mccparser.cc
// Copyright � 2000-04, 07 Laboratoire de Biologie Informatique et Th�orique.
// Author           : Martin Larose
// Created On       : Fri Aug 25 16:28:36 2000
// $Revision$
// $Id$


// cmake generated defines
#include <config.h>

#include <math.h>
#include <string.h>
#include "mccparser.h"


vector< MccPStruct* > *astv;

bool interactive_parser;

MccInput *input_class;


ostream& operator<< (ostream& os, MccPStruct& obj)
{
  obj.ppdisplay (os, 0);
  return os;
}


void
whitespaces (ostream &os, unsigned int indent)
{
  for (; indent > 0; --indent)
    os << ' ';
}


MccFragGenStruc&
MccFragGenStruc::operator= (const MccFragGenStruc &right)
{
  if (this != &right)
  {
    this->ident = right.ident;
    this->special_char = right.special_char;
  }
  return *this;
}



void
MccFragGenStruc::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccFragGenStruc::display (ostream &os) const
{
  if (special_char)
    os << '\'' << ident << '\'';
  else
    os << ident;
}



MccResidueName&
MccResidueName::operator= (const MccResidueName &right)
{
  if (this != &right)
  {
    id = right.id;
    no = right.no;
  }
  return *this;
}



void
MccResidueName::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}


MccResidueNamePairs&
MccResidueNamePairs::operator= (const MccResidueNamePairs& right)
{
  if (this != &right)
  {
    this->pairs = right.pairs;
    this->ranges = right.ranges;
  }
  return *this;
}


void
MccResidueNamePairs::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}


void
MccResidueNamePairs::add (const MccResidueNamePairs& rnp)
{
  this->pairs.insert (this->pairs.end (), rnp.pairs.begin (), rnp.pairs.end ());
  this->ranges.insert (this->ranges.end (), rnp.ranges.begin (), rnp.ranges.end ());
}


void 
MccResidueNamePairs::add (const MccResidueName& rn1, const MccResidueName& rn2)
{
  pair< MccResidueName, MccResidueName > p (rn1, rn2);
  this->pairs.push_back (p);
}


void 
MccResidueNamePairs::addRange (const MccResidueName& rn1, const MccResidueName& rn2)
{
  pair< MccResidueName, MccResidueName > p (rn1, rn2);
  this->ranges.push_back (p);
}


void
MccResidueNamePairs::display (ostream &os) const
{
  list< pair< MccResidueName, MccResidueName > >::const_iterator it;

  os.fail ();

  for (it = this->pairs.begin (); it != this->pairs.end (); ++it)
  {
    if (it != this->pairs.begin ()) os << ", ";
    it->first.display (os);
    os << " ";
    it->second.display (os);
  }

  if (!this->pairs.empty ()) os << ", ";

  for (it = this->ranges.begin (); it != this->ranges.end (); ++it)
  {
    if (it != this->ranges.begin ()) os << ", ";
    it->first.display (os);
    os << ":";
    it->second.display (os);
  }
}


void
MccResidueNamePairs::ppdisplay (ostream &os, int indent) const
{
  this->display (os);
}


MccResidueNameSingletons&
MccResidueNameSingletons::operator= (const MccResidueNameSingletons& right)
{
  if (this != &right)
  {
    this->singletons = right.singletons;
    this->ranges = right.ranges;
  }
  return *this;
}


void
MccResidueNameSingletons::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}


void
MccResidueNameSingletons::add (const MccResidueNameSingletons& rnp)
{
  this->singletons.insert (this->singletons.end (), rnp.singletons.begin (), rnp.singletons.end ());
  this->ranges.insert (this->ranges.end (), rnp.ranges.begin (), rnp.ranges.end ());
}


void 
MccResidueNameSingletons::add (const MccResidueName& rn)
{
  this->singletons.push_back (rn);
}


void 
MccResidueNameSingletons::addRange (const MccResidueName& rn1, const MccResidueName& rn2)
{
  pair< MccResidueName, MccResidueName > p (rn1, rn2);
  this->ranges.push_back (p);
}


void
MccResidueNameSingletons::display (ostream &os) const
{
  list< MccResidueName >::const_iterator sit;
  list< pair< MccResidueName, MccResidueName > >::const_iterator rit;

  for (sit = this->singletons.begin (); sit != this->singletons.end (); ++sit)
  {
    if (sit != this->singletons.begin ()) os << ", ";
    sit->display (os);
  }

  if (!this->singletons.empty ()) os << ", ";

  for (rit = this->ranges.begin (); rit != this->ranges.end (); ++rit)
  {
    if (rit != this->ranges.begin ()) os << ", ";
    rit->first.display (os);
    os << ":";
    rit->second.display (os);
  }
}


void
MccResidueNameSingletons::ppdisplay (ostream &os, int indent) const
{
  this->display (os);
}


void
MccQTrueFunc::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}


MccQIdentFunc::MccQIdentFunc (const MccQIdentFunc &right)
{
  str = new char[strlen (right.str) + 1];
  strcpy (str, right.str);
}


  
MccQIdentFunc&
MccQIdentFunc::operator= (const MccQIdentFunc &right)
{
  if (this != &right)
  {
    delete[] str;
    str = new char[strlen (right.str) + 1];
    strcpy (str, right.str);
  }
  return *this;
}



void
MccQIdentFunc::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



MccQNotFunc&
MccQNotFunc::operator= (const MccQNotFunc &right)
{
  if (this != &right)
  {
    delete fn;
    fn = right.fn->clone ();
  }
  return *this;
}



void
MccQNotFunc::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccQNotFunc::ppdisplay (ostream &os, int indent) const
{
  os << "! ";
  if (dynamic_cast< const MccQIdentFunc* >(fn)
      || dynamic_cast< const MccQNotFunc* >(fn))
    fn->ppdisplay (os, indent);
  else
  {
    os << '(';
    fn->ppdisplay (os, indent);
    os << ')';
  }
}



MccQFaceFunc&
MccQFaceFunc::operator= (const MccQFaceFunc &right_val)
{
  if (this != &right_val)
  {
    delete left;
    left = right_val.left->clone ();
    delete right;
    right = right_val.right->clone ();
  }
  return *this;
}



void
MccQFaceFunc::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccQFaceFunc::ppdisplay (ostream &os, int indent) const
{
  left->ppdisplay (os, indent);
  os << " / ";
  right->ppdisplay (os, indent);
}



MccQAndFunc&
MccQAndFunc::operator= (const MccQAndFunc &right_val)
{
  if (this != &right_val)
  {
    delete left;
    left = right_val.left->clone ();
    delete right;
    right = right_val.right->clone ();
  }
  return *this;
}



void
MccQAndFunc::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccQAndFunc::ppdisplay (ostream &os, int indent) const
{
  if (dynamic_cast< const MccQOrFunc* >(left))
  {
    os << '(';
    left->ppdisplay (os, indent);
    os << ')';
  }
  else
    left->ppdisplay (os, indent);
  os << " && ";
  if (dynamic_cast< const MccQOrFunc* >(right))
  {
    os << '(';
    right->ppdisplay (os, indent);
    os << ')';
  }
  else
    right->ppdisplay (os, indent);
}



MccQOrFunc&
MccQOrFunc::operator= (const MccQOrFunc &right_val)
{
  if (this != &right_val)
  {
    delete left;
    left = right_val.left->clone ();
    delete right;
    right = right_val.right->clone ();
  }
  return *this;
}



void
MccQOrFunc::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccQOrFunc::ppdisplay (ostream &os, int indent) const
{
  left->ppdisplay (os, indent);
  os << " || ";
  right->ppdisplay (os, indent);
}



MccQueryExpr::MccQueryExpr (const MccQueryExpr &right)
  : strs (new vector< char* > ()),
    fn (right.fn->clone ())
{
  vector< char* >::const_iterator cit;

  for (cit = right.strs->begin (); cit != right.strs->end (); ++cit)
  {
    char *tmp = new char[strlen (*cit) + 1];

    strcpy (tmp, *cit);
    strs->push_back (tmp);
  }
}


      
MccQueryExpr::~MccQueryExpr ()
{
  vector< char* >::iterator it;

  for (it = strs->begin (); it != strs->end (); ++it)
    delete[] *it;
  delete strs;
  delete fn;
}



MccQueryExpr&
MccQueryExpr::operator= (const MccQueryExpr &right)
{
  if (this != &right)
  {
    vector< char* >::const_iterator cit;
    vector< char* >::iterator it;

    for (it = strs->begin (); it != strs->end (); ++it)
      delete[] *it;
    strs->clear ();
    for (cit = right.strs->begin (); cit != right.strs->end (); ++cit)
    {
      char *tmp = new char[strlen (*cit) + 1];
	  
      strcpy (tmp, *cit);
      strs->push_back (tmp);
    }
    delete fn;
    fn = right.fn->clone ();
  }
  return *this;
}



void
MccQueryExpr::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccQueryExpr::display (ostream &os) const
{
  vector< char* >::iterator it;
  
  os << '{';
  for (it = strs->begin (); it != strs->end (); ++it)
    os << " file (\"" << *it << "\")";
  os << ' ';
  fn->display (os);
  os << " }";
}



void
MccQueryExpr::ppdisplay (ostream &os, int indent) const
{
  vector< char* >::iterator it;

  os << '{';
  for (it = strs->begin (); it != strs->end (); ++it)
  {
    os << " file (\"" << *it << "\")";
  }
  os << ' ';
  fn->ppdisplay (os, indent);
  os << " }";
}


MccASIdentFunc::MccASIdentFunc (const MccASIdentFunc &right)
{
  str = new char[strlen (right.str) + 1];
  strcpy (str, right.str);
}

  
MccASIdentFunc&
MccASIdentFunc::operator= (const MccASIdentFunc &right)
{
  if (this != &right)
  {
    delete[] str;
    str = new char[strlen (right.str) + 1];
    strcpy (str, right.str);
  }
  return *this;
}


void
MccASIdentFunc::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}


MccASNotFunc&
MccASNotFunc::operator= (const MccASNotFunc &right)
{
  if (this != &right)
  {
    delete fn;
    fn = right.fn->clone ();
  }
  return *this;
}


void
MccASNotFunc::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}


void
MccASNotFunc::ppdisplay (ostream &os, int indent) const
{
  os << "! ";
  if (dynamic_cast< const MccASIdentFunc* >(fn)
      || dynamic_cast< const MccASNotFunc* >(fn))
    fn->ppdisplay (os, indent);
  else
  {
    os << '(';
    fn->ppdisplay (os, indent);
    os << ')';
  }
}


MccASAndFunc&
MccASAndFunc::operator= (const MccASAndFunc &right_val)
{
  if (this != &right_val)
  {
    delete left;
    left = right_val.left->clone ();
    delete right;
    right = right_val.right->clone ();
  }
  return *this;
}


void
MccASAndFunc::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}


void
MccASAndFunc::ppdisplay (ostream &os, int indent) const
{
  if (dynamic_cast< const MccASOrFunc* >(left))
  {
    os << '(';
    left->ppdisplay (os, indent);
    os << ')';
  }
  else
    left->ppdisplay (os, indent);
  os << " && ";
  if (dynamic_cast< const MccASOrFunc* >(right))
  {
    os << '(';
    right->ppdisplay (os, indent);
    os << ')';
  }
  else
    right->ppdisplay (os, indent);
}


MccASOrFunc&
MccASOrFunc::operator= (const MccASOrFunc &right_val)
{
  if (this != &right_val)
  {
    delete left;
    left = right_val.left->clone ();
    delete right;
    right = right_val.right->clone ();
  }
  return *this;
}


void
MccASOrFunc::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}


void
MccASOrFunc::ppdisplay (ostream &os, int indent) const
{
  left->ppdisplay (os, indent);
  os << " || ";
  right->ppdisplay (os, indent);
}


MccRMSDResidueViewCache::MccRMSDResidueViewCache (float th, MccASFunc* asfn, bool af)
  : MccResidueViewCache (),
    threshold (th),
    atomset (asfn),
    alignflag (af)
{
}


MccResidueViewCache* 
MccRMSDResidueViewCache::clone () const
{
  return new MccRMSDResidueViewCache (this->threshold, 
				      0 == this->atomset ? 0 : this->atomset->clone (), 
				      this->alignflag);
}


MccRMSDResidueViewCache::~MccRMSDResidueViewCache ()
{
  if (this->atomset)
    delete this->atomset;
}


void
MccRMSDResidueViewCache::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}


void
MccRMSDResidueViewCache::display (ostream &os) const
{
  os << "rmsd (" << this->threshold;

  if (this->atomset)
    this->atomset->display (os << " ");

  os << " " << this->alignflag << ")";
}


void
MccRMSDResidueViewCache::ppdisplay (ostream &os, int indent) const
{
  os << endl;
  whitespaces (os, indent);
  this->display (os);
}


MccReferenceResidueViewCache::MccReferenceResidueViewCache (MccInputMode *im, MccASFunc* asfn, bool af)
  : MccResidueViewCache (),
    inputMode (im),
    atomset (asfn),
    alignflag (af)
{
}


MccResidueViewCache* 
MccReferenceResidueViewCache::clone () const
{
  return new MccReferenceResidueViewCache (this->inputMode->clone (), 
					   0 == this->atomset ? 0 : this->atomset->clone (), 
					   this->alignflag);
}


MccReferenceResidueViewCache::~MccReferenceResidueViewCache ()
{
  if (this->inputMode)
    delete this->inputMode;
  if (this->atomset)
    delete this->atomset;
}


void
MccReferenceResidueViewCache::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}


void
MccReferenceResidueViewCache::display (ostream &os) const
{
  os << "reference (";
  if (this->inputMode)
    this->inputMode->display (os);
  os << " ";
  if (this->atomset)
    this->atomset->display (os << " ");

  os << " " << this->alignflag << ")";
}


void
MccReferenceResidueViewCache::ppdisplay (ostream &os, int indent) const
{
  os << endl;
  whitespaces (os, indent);
  this->display (os);
}


MccTFODResidueViewCache::MccTFODResidueViewCache (float th)
  : MccResidueViewCache (),
    threshold (th)
{
}


MccResidueViewCache* 
MccTFODResidueViewCache::clone () const
{
  return new MccTFODResidueViewCache (this->threshold);
}


MccTFODResidueViewCache::~MccTFODResidueViewCache ()
{
}


void
MccTFODResidueViewCache::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}


void
MccTFODResidueViewCache::display (ostream &os) const
{
  os << "tfod (" << this->threshold << ")";
}


void
MccTFODResidueViewCache::ppdisplay (ostream &os, int indent) const
{
  os << endl;
  whitespaces (os, indent);
  this->display (os);
}


MccDBSetVersionStat&
MccDBSetVersionStat::operator= (const MccDBSetVersionStat &right)
{
  if (this != &right)
  {
    this->dbversion = right.dbversion;
  }
  return *this;
}

 
void
MccDBSetVersionStat::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccDBSetVersionStat::display (ostream &os) const
{
  os << "db_version (\"" << this->dbversion << "\")";
}
  


void
MccDBSetVersionStat::ppdisplay (ostream &os, int indent) const
{
  whitespaces (os, indent);
  this->display (os);
}


MccDBInsertStat::MccDBInsertStat (const MccDBInsertStat &right)
  : inputMode (right.inputMode->clone ()),
    threshold (right.threshold)
{

}



MccDBInsertStat::~MccDBInsertStat ()
{
  delete this->inputMode;
}



MccDBInsertStat&
MccDBInsertStat::operator= (const MccDBInsertStat &right)
{
  if (this != &right)
  {
    delete this->inputMode;
    this->inputMode = right.inputMode->clone ();
    this->threshold = right.threshold;
  }
  return *this;
}


  
void
MccDBInsertStat::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccDBInsertStat::display (ostream &os) const
{
  os << "db_insert (";
  this->inputMode->display (os); 
 
  if (!(this->threshold < 0.0))
    os << ' ' << this->threshold;

  os << ')';
}
  


void
MccDBInsertStat::ppdisplay (ostream &os, int indent) const
{
  whitespaces (os, indent);
  this->display (os);
  os << endl;
}


MccDBIncludeStat&
MccDBIncludeStat::operator= (const MccDBIncludeStat &right)
{
  if (this != &right)
  {
    this->filename = right.filename;
  }
  return *this;
}


  
void
MccDBIncludeStat::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccDBIncludeStat::display (ostream &os) const
{
  os << "db_include (\"" << this->filename << "\")";
}
  


void
MccDBIncludeStat::ppdisplay (ostream &os, int indent) const
{
  whitespaces (os, indent);
  this->display (os);
  os << endl;
}


MccBaseAdjacencyCstStat::MccBaseAdjacencyCstStat (const MccBaseAdjacencyCstStat &right)
  : fg_struc (new MccFragGenStruc (*right.fg_struc)),
    coverage (right.coverage)
{ }



MccBaseAdjacencyCstStat&
MccBaseAdjacencyCstStat::operator= (const MccBaseAdjacencyCstStat &right)
{
  if (this != &right)
  {
    delete fg_struc;
    fg_struc = right.fg_struc->clone ();
    this->coverage = right.coverage;
  }
  return *this;
}



void
MccBaseAdjacencyCstStat::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}


void
MccBaseAdjacencyCstStat::display (ostream &os) const
{
  os << "base_adjacency (";
  fg_struc->display (os);
  os << ' ' << this->coverage << "%)";
}



void
MccBaseAdjacencyCstStat::ppdisplay (ostream &os, int indent) const
{
  os << "base_adjacency (";
  fg_struc->ppdisplay (os, indent);
  os << ' ' << this->coverage << "%)" << endl;
}


MccAngleCstStat::_AngleStruc::_AngleStruc (const MccAngleCstStat::_AngleStruc &right)
  : res1 (new MccResidueName (*right.res1)),
    res2 (new MccResidueName (*right.res2)),
    res3 (new MccResidueName (*right.res3)),
    angle (right.angle),
    error (right.error)
{
  at1 = new char[strlen (right.at1) + 1];
  strcpy (at1, right.at1);
  at2 = new char[strlen (right.at2) + 1];
  strcpy (at2, right.at2);
  at3 = new char[strlen (right.at3) + 1];
  strcpy (at3, right.at3);
}

  

MccAngleCstStat::_AngleStruc::~_AngleStruc ()
{
  delete res1;
  delete[] at1;
  delete res2;
  delete[] at2;
  delete res3;
  delete[] at3;
}



MccAngleCstStat::_AngleStruc&
MccAngleCstStat::_AngleStruc::operator= (const MccAngleCstStat::_AngleStruc &right)
{
  if (this != &right)
  {
    delete res1;
    res1 = new MccResidueName (*right.res1);
    delete[] at1;
    at1 = new char[strlen (right.at1) + 1];
    strcpy (at1, right.at1);
    delete res2;
    res2 = new MccResidueName (*right.res2);
    delete[] at2;
    at2 = new char[strlen (right.at2) + 1];
    strcpy (at2, right.at2);
    delete res3;
    res3 = new MccResidueName (*right.res3);
    delete[] at3;
    at3 = new char[strlen (right.at3) + 1];
    strcpy (at3, right.at3);
    angle = right.angle;
    error = right.error;
  }
  return *this;
}

  

void
MccAngleCstStat::_AngleStruc::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccAngleCstStat::_AngleStruc::display (ostream &os) const
{
  res1->display (os);
  os << ":" << at1 << ' ';
  res2->display (os);
  os << ":" << at2 << ' ';
  res3->display (os);
  os << ":" << at3 << ' ' << angle << ' ' << error;
}



void
MccAngleCstStat::_AngleStruc::ppdisplay (ostream &os, int indent) const
{
  os << endl;
  whitespaces (os, indent);
  res1->ppdisplay (os, indent);
  os << ":" << at1 << ' ';
  res2->ppdisplay (os, indent);
  os << ":" << at2 << ' ';
  res3->ppdisplay (os, indent);
  os << ":" << at3 << ' ' << angle << ' ' << error;
}



MccAngleCstStat::MccAngleCstStat (const MccAngleCstStat &right)
  : strucs (new vector< MccAngleCstStat::_AngleStruc* > ())
{
  vector< MccAngleCstStat::_AngleStruc* >::const_iterator cit;

  for (cit = right.strucs->begin (); cit != right.strucs->end (); ++cit)
    strucs->push_back (new MccAngleCstStat::_AngleStruc (**cit));
}



MccAngleCstStat::~MccAngleCstStat ()
{
  vector< _AngleStruc* >::iterator it;

  for (it = strucs->begin (); it != strucs->end (); it++)
    delete *it;
  delete strucs;
}



MccAngleCstStat&
MccAngleCstStat::operator= (const MccAngleCstStat &right)
{
  if (this != &right)
  {
    vector< MccAngleCstStat::_AngleStruc* >::const_iterator cit;
    vector< MccAngleCstStat::_AngleStruc* >::iterator it;

    for (it = strucs->begin (); it != strucs->end (); it++)
      delete *it;
    strucs->clear ();
    for (cit = right.strucs->begin (); cit != right.strucs->end (); ++cit)
      strucs->push_back (new MccAngleCstStat::_AngleStruc (**cit));
  }
  return *this;
}



void
MccAngleCstStat::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccAngleCstStat::display (ostream &os) const
{
  vector< _AngleStruc* >::iterator it;

  os << "angle (";
  for (it = strucs->begin (); it != strucs->end (); it++)
  {
    if (it != strucs->begin ())
      os << ' ';
    (*it)->display (os);
  }
  os << ')';
}



void
MccAngleCstStat::ppdisplay (ostream &os, int indent) const
{
  vector< _AngleStruc* >::iterator it;

  os << "angle" << endl;
  whitespaces (os, indent + 2);
  os << '(';
  for (it = strucs->begin (); it != strucs->end (); it++)
    (*it)->ppdisplay (os, indent + 4);
  os << endl;
  whitespaces (os, indent + 2);
  os << ')' << endl;
}



MccAssignStat::MccAssignStat (const MccAssignStat &right)
  : expr (right.expr->clone ())
{
  ident = new char[strlen (right.ident)];
  strcpy (ident, right.ident);
}



MccAssignStat&
MccAssignStat::operator= (const MccAssignStat &right)
{
  if (this != &right)
  {
    delete[] ident;
    ident = new char[strlen (right.ident)];
    strcpy (ident, right.ident);
    delete expr;
    expr = right.expr->clone ();
  }
  return *this;
}



void
MccAssignStat::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccAssignStat::display (ostream &os) const
{
  os << ident << " = ";
  expr->display (os);
}



void
MccAssignStat::ppdisplay (ostream &os, int indent) const
{
  os << ident << " = ";
  expr->ppdisplay (os, indent + strlen (ident) + 3);
}



MccBacktrackExpr::_GenBTStruc::_GenBTStruc (const MccBacktrackExpr::_GenBTStruc &right)
  : ref (0), res (0), fg_struc (0), res_v (0)
{
  if (right.ref)
    ref = new MccResidueName (*right.ref);
  if (right.res)
    res = new MccResidueName (*right.res);
  if (right.fg_struc)
    fg_struc = new MccFragGenStruc (*right.fg_struc);
  if (right.res_v)
  {
    vector< MccResidueName* >::const_iterator cit;
      
    res_v = new vector< MccResidueName* > ();
    for (cit = right.res_v->begin (); cit != right.res_v->end (); cit++)
      res_v->push_back (new MccResidueName (**cit));
  }
}



MccBacktrackExpr::_GenBTStruc&
MccBacktrackExpr::_GenBTStruc::operator= (const MccBacktrackExpr::_GenBTStruc &right)
{
  if (this != &right)
  {
    if (ref)
    {
      delete ref;
      ref = 0;
    }
    if (right.ref)
      ref = new MccResidueName (*right.ref);
    if (res)
    {
      delete res;
      res = 0;
    }
    if (right.res)
      res = new MccResidueName (*right.res);
    if (fg_struc)
    {
      delete fg_struc;
      fg_struc = 0;
    }
    if (right.fg_struc)
      fg_struc = new MccFragGenStruc (*right.fg_struc);
    if (res_v)
    {
      vector< MccResidueName* >::iterator it;

      for (it = res_v->begin (); it != res_v->end (); it++)
	delete *it;
      delete res_v;
      res_v = 0;
    }
    if (right.res_v)
    {
      vector< MccResidueName* >::const_iterator cit;
	  
      res_v = new vector< MccResidueName* > ();
      for (cit = right.res_v->begin (); cit != right.res_v->end (); cit++)
	res_v->push_back (new MccResidueName (**cit));
	  
    }
  }
  return *this;
}



MccBacktrackExpr::_FGStruc&
MccBacktrackExpr::_FGStruc::operator= (const MccBacktrackExpr::_FGStruc &right)
{
  if (this != &right)
    MccBacktrackExpr::_GenBTStruc::operator= (right);
  return *this;
}



void
MccBacktrackExpr::_FGStruc::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccBacktrackExpr::_FGStruc::ppdisplay (ostream &os, int indent) const
{
  os << endl;
  whitespaces (os, indent);
  display (os);
}



MccBacktrackExpr::_BTStruc::~_BTStruc ()
{
  vector< MccResidueName* >::iterator it;

  delete ref;
  for (it = res_v->begin (); it != res_v->end (); it++)
    delete *it;
  delete res_v;
}



void
MccBacktrackExpr::_BTStruc::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



MccBacktrackExpr::_BTStruc&
MccBacktrackExpr::_BTStruc::operator= (const MccBacktrackExpr::_BTStruc &right)
{
  if (this != &right)
    MccBacktrackExpr::_BTStruc::operator= (right);
  return *this;
}



void
MccBacktrackExpr::_BTStruc::display (ostream &os) const
{
  vector< MccResidueName* >::iterator it;

  os << '(';
  ref->display (os);
  for (it = res_v->begin (); it != res_v->end (); it++)
  {
    os << ' ';
    (*it)->display (os);
  }
  os << ')';
}



void
MccBacktrackExpr::_BTStruc::ppdisplay (ostream &os, int indent) const
{
  vector< MccResidueName* >::iterator it;

  os << endl;
  whitespaces (os, indent);
  os << '(';
  ref->ppdisplay (os, indent);
  for (it = res_v->begin (); it != res_v->end (); it++)
  {
    os << ' ';
    (*it)->ppdisplay (os, indent);
  }
  os << ')';
}



void
MccBacktrackExpr::_PlaceStruc::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



MccBacktrackExpr::_PlaceStruc&
MccBacktrackExpr::_PlaceStruc::operator= (const MccBacktrackExpr::_PlaceStruc &right)
{
  if (this != &right)
    MccBacktrackExpr::_GenBTStruc::operator= (right);
  return *this;
}



void
MccBacktrackExpr::_PlaceStruc::display (ostream &os) const
{
  os << "place (";
  ref->display (os);
  os << ' ';
  res->display (os);
  os << ' ';
  fg_struc->display (os);
  os << ')';
}



void
MccBacktrackExpr::_PlaceStruc::ppdisplay (ostream &os, int indent) const
{
  os << endl;
  whitespaces (os, indent);
  os << "place (";
  ref->ppdisplay (os, indent);
  os << ' ';
  res->ppdisplay (os, indent);
  os << ' ';
  fg_struc->ppdisplay (os, indent);
  os << ')';
}


void
MccBacktrackExpr::_MergeStruc::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



MccBacktrackExpr::_MergeStruc&
MccBacktrackExpr::_MergeStruc::operator= (const MccBacktrackExpr::_MergeStruc &right)
{
  if (this != &right)
  {
    MccBacktrackExpr::_GenBTStruc::operator= (right);

    rmsBound = right.rmsBound;
    if (this->atomset)
    {
      delete this->atomset;
      this->atomset = 0;
    }
    if (right.atomset)
      this->atomset = right.atomset->clone ();
  }
  return *this;
}



void
MccBacktrackExpr::_MergeStruc::display (ostream &os) const
{
  os << "merge (";
  fg_struc->display (os);
  os << ' ' << rmsBound;
  if (this->atomset)
    this->atomset->display (os);
  os << ')';
}



void
MccBacktrackExpr::_MergeStruc::ppdisplay (ostream &os, int indent) const
{
  os << endl;
  whitespaces (os, indent);
  os << "merge (";
  fg_struc->display (os);
  os << ' ' << rmsBound;
  if (this->atomset)
    this->atomset->display (os);
  os << ')';
}



MccBacktrackExpr::MccBacktrackExpr (const MccBacktrackExpr &right)
  : strucs (new vector< MccBacktrackExpr::_GenBTStruc* > ())
{
  vector< MccBacktrackExpr::_GenBTStruc* >::const_iterator cit;

  for (cit = right.strucs->begin (); cit != right.strucs->end (); cit++)
    strucs->push_back ((*cit)->clone ());
}



MccBacktrackExpr::~MccBacktrackExpr ()
{
  vector< _GenBTStruc* >::iterator it;

  for (it = strucs->begin (); it != strucs->end (); it++)
    delete *it;
  delete strucs;
}



MccBacktrackExpr&
MccBacktrackExpr::operator= (const MccBacktrackExpr &right)
{
  if (this != &right)
  {
    vector< MccBacktrackExpr::_GenBTStruc* >::const_iterator cit;
    vector< MccBacktrackExpr::_GenBTStruc* >::iterator it;

    for (it = strucs->begin (); it != strucs->end (); it++)
      delete *it;
    strucs->clear ();
    for (cit = right.strucs->begin (); cit != right.strucs->end (); cit++)
      strucs->push_back ((*cit)->clone ());
  }
  return *this;
}



void
MccBacktrackExpr::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccBacktrackExpr::display (ostream &os) const
{
  vector< _GenBTStruc* >::iterator it;

  os << "backtrack (";
  for (it = strucs->begin (); it != strucs->end (); it++)
  {
    if (it != strucs->begin ())
      os << ' ';
    (*it)->display (os);
  }
  os << ')';
}



void
MccBacktrackExpr::ppdisplay (ostream &os, int indent) const
{
  vector< _GenBTStruc* >::iterator it;

  os << "backtrack" << endl;
  whitespaces (os, indent + 2);
  os << '(';
  for (it = strucs->begin (); it != strucs->end (); it++)
    (*it)->ppdisplay (os, indent + 4);
  os << endl;
  whitespaces (os, indent + 2);
  os << ')' << endl;
}



MccCycleExpr::MccCycleExpr (const MccCycleExpr &right)
{
  if (right.resv)
  {
    vector< MccResidueName* >::const_iterator cit;
      
    resv = new vector< MccResidueName* > ();
    for (cit = right.resv->begin (); cit != right.resv->end (); cit++)
      resv->push_back ((*cit)->clone ());
  }
}



MccCycleExpr::~MccCycleExpr ()
{
  if (resv)
  {
    vector< MccResidueName* >::iterator it;
      
    for (it = resv->begin (); it != resv->end (); ++it)
      delete *it;
    delete resv;
  }
}



MccCycleExpr&
MccCycleExpr::operator= (const MccCycleExpr &right)
{
  if (this != &right)
  {
    if (resv)
    {
      vector< MccResidueName* >::iterator it;
	  
      for (it = resv->begin (); it != resv->end (); it++)
	delete *it;
      delete resv;
      resv = 0;
    }
    if (right.resv)
    {
      vector< MccResidueName* >::const_iterator cit;
	  
      resv = new vector< MccResidueName* > ();
      for (cit = right.resv->begin (); cit != right.resv->end (); cit++)
	resv->push_back ((*cit)->clone ());
    }
  }
  return *this;
}



void
MccCycleExpr::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccCycleExpr::display (ostream &os) const
{
  vector< MccResidueName* >::iterator it;

  os << "cycle (";
  for (it = resv->begin (); it != resv->end (); it++)
  {
    if (it != resv->begin ())
      os << ' ';
    (*it)->display (os);
  }
  os << ')';
}



void
MccCycleExpr::ppdisplay (ostream &os, int indent) const
{
  vector< MccResidueName* >::iterator it;

  os << "cycle" << endl;
  whitespaces (os, indent + 2);
  os << '(' << endl;
  whitespaces (os, indent + 4);
  for (it = resv->begin (); it != resv->end (); it++) 
  {
    if (it != resv->begin ())
      os << ' ';
    (*it)->ppdisplay (os, indent + 4);
  }
  os << endl;
  whitespaces (os, indent + 2);
  os << ')' << endl;
}


MccCacheExpr::MccCacheExpr (const MccCacheExpr &right)
  : fg (right.fg),
    cache (0 == right.cache ? 0 : right.cache->clone ())
{ 
}


MccCacheExpr::~MccCacheExpr ()
{
  if (this->cache)
    delete this->cache;
}


MccCacheExpr&
MccCacheExpr::operator= (const MccCacheExpr &right)
{
  if (this != &right)
  {
    this->fg = right.fg;

    delete this->cache;
    if ((this->cache = right.cache))
      this->cache = this->cache->clone ();
  }
  return *this;
}


void
MccCacheExpr::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}


void
MccCacheExpr::display (ostream &os) const
{
  os << "cache (";
  fg.display (os);
  if (this->cache)
    this->cache->display (os << " ");
  os << ')';
}


void
MccCacheExpr::ppdisplay (ostream &os, int indent) const
{
  // no indent for an assignable
  this->display (os);
}


MccClashCstStat::MccClashCstStat (const MccClashCstStat &right)
  : fg_struc (new MccFragGenStruc (*right.fg_struc)),
    VDWDist (right.VDWDist),
    distFac (right.distFac),
    atomset (right.atomset)
{
  if (this->atomset)
    this->atomset = this->atomset->clone ();
}


MccClashCstStat::~MccClashCstStat ()
{
  delete this->fg_struc;
  if (this->atomset)
    delete this->atomset;
}


MccClashCstStat&
MccClashCstStat::operator= (const MccClashCstStat &right)
{
  if (this != &right)
  {
    delete fg_struc;
    fg_struc = new MccFragGenStruc (*right.fg_struc);
    VDWDist = right.VDWDist;
    distFac = right.distFac;
    if (this->atomset)
      delete this->atomset;
    if ((this->atomset = right.atomset))
      this->atomset = this->atomset->clone ();
  }
  return *this;
}



void
MccClashCstStat::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccClashCstStat::display (ostream &os) const
{
  os << "res_clash (";
  fg_struc->display (os);
  if (VDWDist)
    os << " vdw_distance ";
  else
    os << " fixed_distance ";
  os << distFac;
  if (this->atomset)
    this->atomset->display (os);
  os << ')';
}



void
MccClashCstStat::ppdisplay (ostream &os, int indent) const
{
  os << "res_clash (";
  fg_struc->ppdisplay (os, indent);
  if (VDWDist)
    os << " vdw_distance ";
  else
    os << " fixed_distance ";
  os << distFac;
  if (this->atomset)
    this->atomset->ppdisplay (os, indent);
  os << ')' << endl;
}


MccCycleCstStat::_CycleStruc::_CycleStruc (const MccCycleCstStat::_CycleStruc &right)
  : last (right.last->clone ()),
    first (right.first->clone ()),
    threshold (right.threshold)
{
}

  

MccCycleCstStat::_CycleStruc::~_CycleStruc ()
{
  delete last;
  delete first;
}



MccCycleCstStat::_CycleStruc&
MccCycleCstStat::_CycleStruc::operator= (const MccCycleCstStat::_CycleStruc &right)
{
  if (this != &right)
  {
    delete last;
    last = right.last->clone ();
    delete first;
    first = right.first->clone ();
    this->threshold = right.threshold;
  }
  return *this;
}

  

void
MccCycleCstStat::_CycleStruc::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccCycleCstStat::_CycleStruc::display (ostream &os) const
{
  last->display (os);
  os << ' ';
  first->display (os);
  os << ' ' << this->threshold;
}



void
MccCycleCstStat::_CycleStruc::ppdisplay (ostream &os, int indent) const
{
  os << endl;
  whitespaces (os, indent);
  this->display (os);
}



MccCycleCstStat::MccCycleCstStat (const MccCycleCstStat &right)
  : strucs (new vector< MccCycleCstStat::_CycleStruc* > ())
{
  vector< MccCycleCstStat::_CycleStruc* >::const_iterator cit;

  for (cit = right.strucs->begin (); cit != right.strucs->end (); ++cit)
    strucs->push_back ((*cit)->clone ());
}



MccCycleCstStat::~MccCycleCstStat ()
{
  vector< _CycleStruc* >::iterator it;

  for (it = strucs->begin (); it != strucs->end (); it++)
    delete *it;
  delete strucs;
}



MccCycleCstStat&
MccCycleCstStat::operator= (const MccCycleCstStat &right)
{
  if (this != &right)
  {
    vector< MccCycleCstStat::_CycleStruc* >::const_iterator cit;
    vector< MccCycleCstStat::_CycleStruc* >::iterator it;

    for (it = strucs->begin (); it != strucs->end (); it++)
      delete *it;
    strucs->clear ();
    for (cit = right.strucs->begin (); cit != right.strucs->end (); ++cit)
      strucs->push_back ((*cit)->clone ());
  }
  return *this;
}



void
MccCycleCstStat::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccCycleCstStat::display (ostream &os) const
{
  vector< _CycleStruc* >::iterator it;

  os << "cycle (";
  for (it = strucs->begin (); it != strucs->end (); it++)
  {
    if (it != strucs->begin ())
      os << ' ';
    (*it)->display (os);
  }
  os << ')';
}



void
MccCycleCstStat::ppdisplay (ostream &os, int indent) const
{
  vector< _CycleStruc* >::iterator it;

  os << "relation" << endl;
  whitespaces (os, indent);
  os << '(';
  for (it = strucs->begin (); it != strucs->end (); it++)
    (*it)->ppdisplay (os, indent + 2);
  os << endl;
  whitespaces (os, indent);
  os << ')' << endl;
}


MccMultimerCstStat::_MultimerStruc::_MultimerStruc (const MccMultimerCstStat::_MultimerStruc &right)
  : res1 (new MccResidueName (*right.res1)),
    res2 (new MccResidueName (*right.res2)),
    dist (right.dist),
    nb (right.nb)
{ }



MccMultimerCstStat::_MultimerStruc&
MccMultimerCstStat::_MultimerStruc::operator= (const MccMultimerCstStat::_MultimerStruc &right)
{
  if (this != &right)
  {
    delete res1;
    res1 = new MccResidueName (*right.res1);
    delete res2;
    res2 = new MccResidueName (*right.res2);
    dist = right.dist;
    nb = right.nb;
  }
  return *this;
}



void
MccMultimerCstStat::_MultimerStruc::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccMultimerCstStat::_MultimerStruc::display (ostream &os) const
{
  res1->display (os);
  os << ' ';
  res2->display (os);
  os << ' ' << dist << ' ' << nb;
}



void
MccMultimerCstStat::_MultimerStruc::ppdisplay (ostream &os, int indent) const
{
  os << endl;
  whitespaces (os, indent);
  res1->ppdisplay (os, indent);
  os << ' ';
  res2->ppdisplay (os, indent);
  os << ' ' << dist << ' ' << nb;
}



MccMultimerCstStat::MccMultimerCstStat (const MccMultimerCstStat &right)
  : strucs (new vector< MccMultimerCstStat::_MultimerStruc* > ())
{
  vector< MccMultimerCstStat::_MultimerStruc* >::const_iterator cit;

  for (cit = right.strucs->begin (); cit != right.strucs->end (); cit++)
    strucs->push_back (new _MultimerStruc (**cit));
}



MccMultimerCstStat::~MccMultimerCstStat ()
{
  vector< _MultimerStruc* >::iterator it;

  for (it = strucs->begin (); it != strucs->end (); ++it)
    delete *it;
  delete strucs;
}



MccMultimerCstStat&
MccMultimerCstStat::operator= (const MccMultimerCstStat &right)
{
  if (this != &right)
  {
    vector< MccMultimerCstStat::_MultimerStruc* >::const_iterator cit;
    vector< MccMultimerCstStat::_MultimerStruc* >::iterator it;

    for (it = strucs->begin (); it != strucs->end (); it++)
      delete *it;
    strucs->clear ();
    for (cit = right.strucs->begin (); cit != right.strucs->end (); cit++)
      strucs->push_back (new _MultimerStruc (**cit));
  }
  return *this;
}



void
MccMultimerCstStat::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccMultimerCstStat::display (ostream &os) const
{
  vector< _MultimerStruc* >::iterator it;

  os << "multimer (";
  for (it = strucs->begin (); it != strucs->end (); ++it)
  {
    if (it != strucs->begin ())
      os << ' ';
    (*it)->display (os);
  }
  os << ')';
}



void
MccMultimerCstStat::ppdisplay (ostream &os, int indent) const
{
  vector< _MultimerStruc* >::iterator it;

  os << "multimer" << endl;
  whitespaces (os, indent + 2);
  os << '(';
  for (it = strucs->begin (); it != strucs->end (); ++it)
    (*it)->ppdisplay (os, indent + 4);
  os << endl;
  whitespaces (os, indent + 2);
  os << ')' << endl;
}


void
MccDBDisplayStat::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}


void
MccDBDisplayStat::display (ostream &os) const
{
  os << "db_display";
}


void
MccDBDisplayStat::ppdisplay (ostream &os, int indent) const
{
  whitespaces (os, indent);
  os << "db_display";
}


MccDisplayFGStat::MccDisplayFGStat (const MccDisplayFGStat &right)
  : fg_struc (new MccFragGenStruc (*right.fg_struc))
{ }



MccDisplayFGStat&
MccDisplayFGStat::operator= (const MccDisplayFGStat &right)
{
  if (this != &right)
  {
    delete fg_struc;
    fg_struc = new MccFragGenStruc (*right.fg_struc);
  }
  return *this;
}



void
MccDisplayFGStat::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccDisplayFGStat::display (ostream &os) const
{
  os << "display_fg (";
  fg_struc->display (os);
  os << ')';
}



void
MccDisplayFGStat::ppdisplay (ostream &os, int indent) const
{
  os << "display_fg (";
  fg_struc->ppdisplay (os, indent);
  os << ')' << endl;
}


MccDBFilterStat&
MccDBFilterStat::operator= (const MccDBFilterStat &right)
{
  if (this != &right)
  {
    this->threshold = right.threshold;
    this->threaded = right.threaded;
  }
  return *this;
}



void
MccDBFilterStat::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccDBFilterStat::display (ostream &os) const
{
  os << "db_filter (" << this->threshold << (this->threaded ? " threaded" : "") << ")";
}



void
MccDBFilterStat::ppdisplay (ostream &os, int indent) const
{
  whitespaces (os, indent);
  this->display (os);
  os << endl;
}


MccDistCstStat::_DistStruc::_DistStruc (const MccDistCstStat::_DistStruc &right)
  : res1 (new MccResidueName (*right.res1)),
    res2 (new MccResidueName (*right.res2)),
    dist_min (right.dist_min),
    dist_max (right.dist_max)
{
  at1 = new char[strlen (right.at1) + 1];
  strcpy (at1, right.at1);
  at2 = new char[strlen (right.at2) + 1];
  strcpy (at2, right.at2);
}



MccDistCstStat::_DistStruc&
MccDistCstStat::_DistStruc::operator= (const MccDistCstStat::_DistStruc &right)
{
  if (this != &right)
  {
    delete res1;
    res1 = new MccResidueName (*right.res1);
    delete[] at1;
    at1 = new char[strlen (right.at1) + 1];
    strcpy (at1, right.at1);
    delete res2;
    res2 = new MccResidueName (*right.res2);
    delete[] at2;
    at2 = new char[strlen (right.at2) + 1];
    strcpy (at2, right.at2);
    dist_min = right.dist_min;
    dist_max = right.dist_max;
  }
  return *this;
}



void
MccDistCstStat::_DistStruc::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccDistCstStat::_DistStruc::display (ostream &os) const
{
  res1->display (os);
  os << ':' << at1 << ' ';
  res2->display (os);
  os << ':' << at2 << ' ' << dist_min << ' ' << dist_max;
}



void
MccDistCstStat::_DistStruc::ppdisplay (ostream &os, int indent) const
{
  os << endl;
  whitespaces (os, indent);
  res1->ppdisplay (os, indent);
  os << ':' << at1 << ' ';
  res2->ppdisplay (os, indent);
  os << ':' << at2 << ' ' << dist_min << ' ' << dist_max;
}



MccDistCstStat::MccDistCstStat (const MccDistCstStat &right)
  : strucs (new vector< MccDistCstStat::_DistStruc* > ())
{
  vector< MccDistCstStat::_DistStruc* >::const_iterator cit;

  for (cit = right.strucs->begin (); cit != right.strucs->end (); cit++)
    strucs->push_back (new _DistStruc (**cit));
}



MccDistCstStat::~MccDistCstStat ()
{
  vector< _DistStruc* >::iterator it;

  for (it = strucs->begin (); it != strucs->end (); it++)
    delete *it;
  delete strucs;
}



MccDistCstStat&
MccDistCstStat::operator= (const MccDistCstStat &right)
{
  if (this != &right)
  {
    vector< MccDistCstStat::_DistStruc* >::const_iterator cit;
    vector< MccDistCstStat::_DistStruc* >::iterator it;

    for (it = strucs->begin (); it != strucs->end (); it++)
      delete *it;
    strucs->clear ();
    for (cit = right.strucs->begin (); cit != right.strucs->end (); cit++)
      strucs->push_back (new _DistStruc (**cit));
  }
  return *this;
}



void
MccDistCstStat::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccDistCstStat::display (ostream &os) const
{
  vector< _DistStruc* >::iterator it;

  os << "distance (";
  for (it = strucs->begin (); it != strucs->end (); it++)
  {
    if (it != strucs->begin ())
      os << ' ';
    (*it)->display (os);
  }
  os << ')';
}



void
MccDistCstStat::ppdisplay (ostream &os, int indent) const
{
  vector< _DistStruc* >::iterator it;

  os << "distance" << endl;
  whitespaces (os, indent + 2);
  os << '(';
  for (it = strucs->begin (); it != strucs->end (); it++)
    (*it)->ppdisplay (os, indent + 4);
  os << endl;
  whitespaces (os, indent + 2);
  os << ')' << endl;
}


void
MccEnvStat::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}


void
MccEnvStat::ppdisplay (ostream &os, int indent) const
{
  display (os);
  os << endl;
}



MccFilePdbOutput&
MccFilePdbOutput::operator= (const MccFilePdbOutput &right)
{
  if (this != &right)
  {
    this->filename = right.filename;
    this->zipped = right.zipped;
    this->mult_file = right.mult_file;
  }
  return *this;
}



void
MccFilePdbOutput::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccFilePdbOutput::display (ostream &os) const
{
  os << "pdb (\"" << filename << "\"";
  if (zipped) 
    os << " zipped";
  if (mult_file) 
    os << " multiple";
  else 
    os << " single";
  os << ")";
}


MccFileBinaryOutput&
MccFileBinaryOutput::operator= (const MccFileBinaryOutput &right)
{
  if (this != &right)
  {
    this->filename = right.filename;
    this->zipped = right.zipped;
    this->mult_file = right.mult_file;
  }
  return *this;
}



void
MccFileBinaryOutput::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccFileBinaryOutput::display (ostream &os) const
{
  os << "bin (\"" << this->filename << "\"";
  if (this->zipped)
    os << " zipped";
  if (this->mult_file) 
    os << " multiple";
  else 
    os << " single";
  os << ")";
}



MccSocketBinaryOutput&
MccSocketBinaryOutput::operator= (const MccSocketBinaryOutput &right)
{
  if (this != &right)
  {
    serverName = right.serverName;
    port = right.port;
    modelName = right.modelName;
    mult_file = right.mult_file;
  }
  return *this;
}



void
MccSocketBinaryOutput::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccSocketBinaryOutput::display (ostream &os) const
{
  os << "socket (\"" << serverName << "\" " << port
     << " \"" << modelName << "\"";
  if (mult_file) 
    os << " multiple";
  else 
    os << " single";
  os << ")";
}



MccFileRnamlOutput&
MccFileRnamlOutput::operator= (const MccFileRnamlOutput &right)
{
  if (this != &right)
  {
    this->filename = right.filename;
    zipped = right.zipped;
    mult_file = right.mult_file;
  }
  return *this;
}



void
MccFileRnamlOutput::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccFileRnamlOutput::display (ostream &os) const
{
  os << "rnaml (\"" << filename << "\"";
  if (zipped)
    os << " zipped";
  if (mult_file) 
    os << " multiple";
  else 
    os << " single";
  os << ")";
}




MccFilePdbInput&
MccFilePdbInput::operator= (const MccFilePdbInput &right)
{
  if (this != &right)
  {
    this->filename = right.filename;
  }
  return *this;
}



void
MccFilePdbInput::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccFilePdbInput::display (ostream &os) const
{
  os << "pdb (\"" << filename << "\")";
}


MccFileBinaryInput&
MccFileBinaryInput::operator= (const MccFileBinaryInput &right)
{
  if (this != &right)
  {
    this->filename = right.filename;
  }
  return *this;
}



void
MccFileBinaryInput::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccFileBinaryInput::display (ostream &os) const
{
  os << "bin (\"" << filename << "\")";
}




MccSocketBinaryInput&
MccSocketBinaryInput::operator= (const MccSocketBinaryInput &right)
{
  if (this != &right)
  {
    serverName = right.serverName;
    port = right.port;
    modelNames = right.modelNames;
  }
  return *this;
}



void
MccSocketBinaryInput::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccSocketBinaryInput::display (ostream &os) const
{
  vector< string >::const_iterator it;

  os << "socket (\"" << serverName << " " << port;

  for (it = this->modelNames.begin (); it != this->modelNames.end (); ++it)
    os << " \"" << *it << "\"";

  os << ")";
}


MccFileRnamlInput&
MccFileRnamlInput::operator= (const MccFileRnamlInput &right)
{
  if (this != &right)
  {
    filename = right.filename;
  }
  return *this;
}



void
MccFileRnamlInput::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccFileRnamlInput::display (ostream &os) const
{
  os << "rnaml (\"" << filename << "\")";
}



MccExploreStat::MccExploreStat (const MccExploreStat &right)
  : fg_struc (right.fg_struc),
    exp_parameters (right.exp_parameters),
    filter (0 == right.filter ? 0 : right.filter->clone ()),
    output (0 == right.output ? 0 : right.output->clone ())
    
{
}


MccExploreStat::~MccExploreStat ()
{
  if (this->filter) delete this->filter;
  if (this->output) delete this->output;
}


MccExploreStat&
MccExploreStat::operator= (const MccExploreStat &right)
{
  if (this != &right)
  {
    this->fg_struc = right.fg_struc;
    this->exp_parameters = right.exp_parameters;
    this->filter = 0 == right.filter ? 0 : right.filter->clone ();
    this->output = 0 == right.output ? 0 : right.output->clone ();
  }
  return *this;
}



void
MccExploreStat::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccExploreStat::display (ostream &os) const
{
  map< string, string >::const_iterator it;

  os << "explore (";
  this->fg_struc.display (os);
  
  if (!this->exp_parameters.empty ())
  {
    os << " option (";
    for (it = this->exp_parameters.begin (); it != this->exp_parameters.end (); ++it)
      os << " " << it->first << " = " << it->second;
    os << ")";
  }

  if (this->filter)
  {
    os << ' ';
    this->filter->display (os);
  }

  if (this->output)
  {
    os << ' ';
    this->output->display (os);
  }

  os << ')';
}



void
MccExploreStat::ppdisplay (ostream &os, int indent) const
{
  map< string, string >::const_iterator it;

  whitespaces (os, indent);
  os << "explore" << endl;
  whitespaces (os, indent);
  os << '(' << endl;

  whitespaces (os, indent + 2);
  this->fg_struc.display (os);

  if (!this->exp_parameters.empty ())
  {
    os << endl;
    whitespaces (os, indent + 2);
    os << "option" << endl;
    whitespaces (os, indent + 2);
    os << "(" << endl;
    for (it = this->exp_parameters.begin (); it != this->exp_parameters.end (); ++it)
    {
      whitespaces (os, indent + 4);
      os << it->first << " = " << it->second << endl;
    }
    whitespaces (os, indent + 2);
    os << ")";
  }

  if (this->filter) this->filter->ppdisplay (os, indent + 2);
  if (this->output) this->output->ppdisplay (os, indent + 2);

  os << endl;
  whitespaces (os, indent);
  os << ')' << endl;
}


MccLibraryExpr::MccLibraryExpr (const MccLibraryExpr &right)
  : inputModes (right.inputModes),
    res_from (right.res_from),
    res_to (right.res_to),
    asis (right.asis),
    library (0 == right.library ? 0 : right.library->clone ())
{
  vector< MccInputMode* >::iterator it;

  for (it = this->inputModes.begin (); it != this->inputModes.end (); ++it)
    *it = (*it)->clone ();
}



MccLibraryExpr::~MccLibraryExpr ()
{
  vector< MccInputMode* >::iterator it;

  for (it = this->inputModes.begin (); it != this->inputModes.end (); ++it)
    delete *it;

  if (this->library)
    delete this->library;
}



MccLibraryExpr&
MccLibraryExpr::operator= (const MccLibraryExpr &right)
{
  if (this != &right)
  {
    vector< MccInputMode* >::iterator it;

    for (it = this->inputModes.begin (); it != this->inputModes.end (); ++it)
      delete *it;
    
    this->inputModes = right.inputModes;

    for (it = this->inputModes.begin (); it != this->inputModes.end (); ++it)
      *it = (*it)->clone ();

    this->res_from = right.res_from;
    this->res_to = right.res_to;
    this->asis = right.asis;

    if (this->library)
      delete this->library;
    if ((this->library = right.library))
      this->library = this->library->clone ();
  }
  return *this;
}



void
MccLibraryExpr::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccLibraryExpr::display (ostream &os) const
{
  vector< MccInputMode* >::const_iterator it;

  os << "library (";

  for (it = this->inputModes.begin (); it != this->inputModes.end (); ++it)
    (*it)->display (os);

  if (!this->res_from.empty ())
  {
    this->res_from.display (os);
    os << " <- ";
    this->res_to.display (os);
  }

  if (this->asis)
    os << " as_is";

  if (this->library)
    this->library->display (os << " ");

  os << ')';
}



void
MccLibraryExpr::ppdisplay (ostream &os, int indent) const
{
  vector< MccInputMode* >::const_iterator it;

  whitespaces (os, indent);
  os << "library" << endl;
  whitespaces (os, indent + 2);
  os << '(' << endl;

  for (it = this->inputModes.begin (); it != this->inputModes.end (); ++it)
    (*it)->ppdisplay (os, indent + 4);

  if (!this->res_from.empty ())
  {
    os << endl;
    whitespaces (os, indent + 4);
    this->res_from.display (os);
    os << " <- ";
    this->res_to.display (os);
  }

  if (this->asis)
  {
    os << endl;
    whitespaces (os, indent + 4);
    os << "as_is";
  }

  if (this->library)
    this->library->ppdisplay (os, indent + 4);

  os << endl;
  whitespaces (os, indent + 2);
  os << ')' << endl;
}


MccNewTagStat::MccNewTagStat (const MccNewTagStat &right)
  : exprs (new vector< MccQueryExpr* > ())
{
  vector< MccQueryExpr* >::const_iterator cit;
  
  id = new char[strlen (right.id) + 1];
  strcpy (id, right.id);
  for (cit = right.exprs->begin (); cit != right.exprs->end (); cit++)
    exprs->push_back (new MccQueryExpr (**cit));
}


MccNewTagStat::~MccNewTagStat ()
{
  vector< MccQueryExpr* >::iterator it;

  delete[] id;
  for (it = exprs->begin (); it != exprs->end (); it++)
    delete *it;
  delete exprs;
}



MccNewTagStat&
MccNewTagStat::operator= (const MccNewTagStat &right)
{
  if (this != &right)
  {
    vector< MccQueryExpr* >::const_iterator cit;
    vector< MccQueryExpr* >::iterator it;
  
    delete[] id;
    id = new char[strlen (right.id) + 1];
    strcpy (id, right.id);
    for (it = exprs->begin (); it != exprs->end (); it++)
      delete *it;
    exprs->clear ();
    for (cit = right.exprs->begin (); cit != right.exprs->end (); cit++)
      exprs->push_back (new MccQueryExpr (**cit));
  }
  return *this;
}



void
MccNewTagStat::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccNewTagStat::display (ostream &os) const
{
  vector< MccQueryExpr* >::iterator it;

  os << "new_tag (\"" << id << "\"";
  for (it = exprs->begin (); it != exprs->end (); it++)
  {
    os << ' ';
    (*it)->display (os);
  }
  os << ')';
}



void
MccNewTagStat::ppdisplay (ostream &os, int indent) const
{
  vector< MccQueryExpr* >::iterator it;

  os << "new_tag" << endl;
  whitespaces (os, indent + 2);
  os << '(' << endl;
  whitespaces (os, indent + 4);
  os << "\"" << id << "\"";
  for (it = exprs->begin (); it != exprs->end (); it++)
  {
    os << endl;
    whitespaces (os, indent + 4);
    (*it)->ppdisplay (os, indent + 4);
  }
  os << endl;
  whitespaces (os, indent + 2);
  os << ')' << endl;
}



MccNoteStat::MccNoteStat (const MccNoteStat &right)
{
  str = new char[strlen (right.str) + 1];
  strcpy (str, right.str);
}



MccNoteStat&
MccNoteStat::operator= (const MccNoteStat &right)
{
  if (this != &right)
  {
    delete[] str;
    str = new char[strlen (right.str) + 1];
    strcpy (str, right.str);
  }
  return *this;
}



void
MccNoteStat::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccNoteStat::ppdisplay (ostream &os, int indent) const
{
  display (os);
  os << endl;
}



void
MccNotesStat::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccNotesStat::ppdisplay (ostream &os, int indent) const
{
  display (os);
  os << endl;
}


MccRelationStat::_RelationStruc::_RelationStruc (const MccRelationStat::_RelationStruc &right)
  : rnpairs (right.rnpairs->clone ()),
    expr (right.expr->clone ()),
    ssize (right.ssize->clone ())
{

}


MccRelationStat::_RelationStruc::~_RelationStruc ()
{
  delete this->rnpairs;
  delete this->expr;
  delete this->ssize;
}


MccRelationStat::_RelationStruc&
MccRelationStat::_RelationStruc::operator= (const MccRelationStat::_RelationStruc &right)
{
  if (this != &right)
  {
    delete this->rnpairs;
    rnpairs = right.rnpairs->clone ();
    delete expr;
    expr = right.expr->clone ();
    delete ssize;
    ssize = right.ssize->clone ();
  }
  return *this;
}


void
MccRelationStat::_RelationStruc::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}


void
MccRelationStat::_RelationStruc::display (ostream &os) const
{
  this->rnpairs->display (os);
  os << ' ';
  this->expr->display (os);
  os << ' ';
  this->ssize->display (os);
}


void
MccRelationStat::_RelationStruc::ppdisplay (ostream &os, int indent) const
{
  os << endl;
  whitespaces (os, indent);
  this->rnpairs->display (os);
  os << ' ';
  this->expr->display (os);
  os << ' ';
  this->ssize->display (os);
}


MccRelationStat::MccRelationStat (const MccRelationStat &right)
  : strucs (new vector< MccRelationStat::_RelationStruc* > (right.strucs->size ())),
    threads (right.threads)
{
  vector< MccRelationStat::_RelationStruc* >::const_iterator cit;
  for (cit = right.strucs->begin (); cit != right.strucs->end (); cit++)
    this->strucs->push_back (new MccRelationStat::_RelationStruc (**cit));
}


MccRelationStat::~MccRelationStat ()
{
  vector< _RelationStruc* >::iterator it;
  for (it = this->strucs->begin (); it != this->strucs->end (); ++it)
    delete *it;
  delete this->strucs;
}


MccRelationStat&
MccRelationStat::operator= (const MccRelationStat &right)
{
  if (this != &right)
  {
    vector< MccRelationStat::_RelationStruc* >::const_iterator cit;
    vector< MccRelationStat::_RelationStruc* >::iterator it;
    for (it = this->strucs->begin (); it != this->strucs->end (); ++it)
      delete *it;
    delete this->strucs;
    this->strucs = new vector< MccRelationStat::_RelationStruc* > (right.strucs->size ());
    for (cit = right.strucs->begin (); cit != right.strucs->end (); cit++)
      this->strucs->push_back (new MccRelationStat::_RelationStruc (**cit));

    this->threads = right.threads;
  }
  return *this;
}



void
MccRelationStat::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccRelationStat::display (ostream &os) const
{
  vector< _RelationStruc* >::iterator it;
  
  os << "relation (";
  for (it = strucs->begin (); it != strucs->end (); it++)
  {
    if (it != strucs->begin ())
      os << ' ';
    (*it)->display (os);
  }
  os << ')';
  if (this->threads > 1)
    os << ':' << this->threads;
}



void
MccRelationStat::ppdisplay (ostream &os, int indent) const
{
  vector< _RelationStruc* >::iterator it;
  
  os << "relation" << endl;
  whitespaces (os, indent + 2);
  os << '(';
  for (it = strucs->begin (); it != strucs->end (); it++)
    (*it)->ppdisplay (os, indent + 4);
  os << endl;
  whitespaces (os, indent + 2);
  os << ')';
  if (this->threads > 1)
    os << ':' << this->threads;
  os << endl;
}


MccBacktrackRstStat::MccBacktrackRstStat (const MccFragGenStruc& fg, 
					  const map< string, string >& par)
  : fg_struc (fg),
    parameters (par)
{ 
}


MccBacktrackRstStat::MccBacktrackRstStat (const MccBacktrackRstStat& brst)
  : fg_struc (brst.fg_struc),
    parameters (brst.parameters)
{
}


MccBacktrackRstStat&
MccBacktrackRstStat::operator= (const MccBacktrackRstStat& brst)
{
  if (this != &brst)
  {
    fg_struc = brst.fg_struc;
    parameters = brst.parameters;
  }
  return *this;
}


void
MccBacktrackRstStat::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}


void
MccBacktrackRstStat::display (ostream& os) const
{
  map< string, string >::const_iterator it;

  this->fg_struc.display (os << "backtrack_rst (");
   
  for (it = this->parameters.begin (); it != this->parameters.end (); ++it)
    os << " " << it->first << " = " << it->second;

  os << ")";
}


void
MccBacktrackRstStat::ppdisplay (ostream& os, int indent) const
{
  map< string, string >::const_iterator it;

  whitespaces (os, indent);
  os << "backtrack_rst" << endl;
  whitespaces (os, indent);
  os << "(" << endl;
  whitespaces (os, indent + 2);
  this->fg_struc.display (os);

  for (it = this->parameters.begin (); it != this->parameters.end (); ++it)
  {
    os << endl;
    whitespaces (os, indent + 2);
    os << it->first << " = " << it->second;
  }

  os << endl;
  whitespaces (os, indent);
  os << ")" << endl;
}


MccRiboseRstStat::MccRiboseRstStat (const MccFragGenStruc& fg, 
				    const map< string, string >& par)
  : fg_struc (fg),
    parameters (par)
{ 
}


MccRiboseRstStat::MccRiboseRstStat (const MccFragGenStruc& fg, 
				    const MccResidueNameSingletons& res,
				    const map< string, string >& par)
  : fg_struc (fg),
    restrictions (res),
    parameters (par)
{ 
}


MccRiboseRstStat::MccRiboseRstStat (const MccRiboseRstStat& rrst)
  : fg_struc (rrst.fg_struc),
    restrictions (rrst.restrictions),
    parameters (rrst.parameters)
{
}


MccRiboseRstStat&
MccRiboseRstStat::operator= (const MccRiboseRstStat& rrst)
{
  if (this != &rrst)
  {
    fg_struc = rrst.fg_struc;
    restrictions = rrst.restrictions;
    parameters = rrst.parameters;
  }
  return *this;
}


void
MccRiboseRstStat::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}


void
MccRiboseRstStat::display (ostream& os) const
{
  map< string, string >::const_iterator it;

  this->fg_struc.display (os << "ribose_rst (");

  if (!this->restrictions.empty ())
  {
    this->restrictions.display (os << "[");
    os << "]";
  }
   
  for (it = this->parameters.begin (); it != this->parameters.end (); ++it)
    os << " " << it->first << " = " << it->second;

  os << ")";
}


void
MccRiboseRstStat::ppdisplay (ostream& os, int indent) const
{
  map< string, string >::const_iterator it;

  whitespaces (os, indent);
  os << "ribose_rst" << endl;
  whitespaces (os, indent);
  os << "(" << endl;
  whitespaces (os, indent + 2);
  this->fg_struc.display (os);

  if (!this->restrictions.empty ())
  {
    this->restrictions.display (os << "[");
    os << "]";
  }

  for (it = this->parameters.begin (); it != this->parameters.end (); ++it)
  {
    os << endl;
    whitespaces (os, indent + 2);
    os << it->first << " = " << it->second;
  }

  os << endl;
  whitespaces (os, indent);
  os << ")" << endl;
}


MccImplicitPhosphateRstStat::MccImplicitPhosphateRstStat (const MccFragGenStruc& fg, 
							  const map< string, string >& par)
  : fg_struc (fg),
    parameters (par)
{ 
}


MccImplicitPhosphateRstStat::MccImplicitPhosphateRstStat (const MccFragGenStruc& fg, 
							  const MccResidueNamePairs& res,
							  const map< string, string >& par)
  : fg_struc (fg),
    restrictions (res),
    parameters (par)
{ 
}


MccImplicitPhosphateRstStat::MccImplicitPhosphateRstStat (const MccImplicitPhosphateRstStat& rrst)
  : fg_struc (rrst.fg_struc),
    restrictions (rrst.restrictions),
    parameters (rrst.parameters)
{
}


MccImplicitPhosphateRstStat&
MccImplicitPhosphateRstStat::operator= (const MccImplicitPhosphateRstStat& rrst)
{
  if (this != &rrst)
  {
    fg_struc = rrst.fg_struc;
    restrictions = rrst.restrictions;
    parameters = rrst.parameters;
  }
  return *this;
}


void
MccImplicitPhosphateRstStat::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}


void
MccImplicitPhosphateRstStat::display (ostream& os) const
{
  map< string, string >::const_iterator it;

  this->fg_struc.display (os << "ribose_rst (");

  if (!this->restrictions.empty ())
  {
    this->restrictions.display (os << "[");
    os << "]";
  }
   
  for (it = this->parameters.begin (); it != this->parameters.end (); ++it)
    os << " " << it->first << " = " << it->second;

  os << ")";
}


void
MccImplicitPhosphateRstStat::ppdisplay (ostream& os, int indent) const
{
  map< string, string >::const_iterator it;

  whitespaces (os, indent);
  os << "ribose_rst" << endl;
  whitespaces (os, indent);
  os << "(" << endl;
  whitespaces (os, indent + 2);
  this->fg_struc.display (os);

  if (!this->restrictions.empty ())
  {
    this->restrictions.display (os << "[");
    os << "]";
  }

  for (it = this->parameters.begin (); it != this->parameters.end (); ++it)
  {
    os << endl;
    whitespaces (os, indent + 2);
    os << it->first << " = " << it->second;
  }

  os << endl;
  whitespaces (os, indent);
  os << ")" << endl;
}


void
MccQuitStat::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccQuitStat::ppdisplay (ostream &os, int indent) const
{
  display (os);
  os << endl;
}



MccImplicitRelationCstStat::_ImplicitRelationStruc::_ImplicitRelationStruc (const MccImplicitRelationCstStat::_ImplicitRelationStruc &right)
  : ref (right.ref->clone ()),
    res (right.res->clone ()),
    expr (right.expr->clone ())
{
}

  

MccImplicitRelationCstStat::_ImplicitRelationStruc::~_ImplicitRelationStruc ()
{
  delete ref;
  delete res;
  delete expr;
}



MccImplicitRelationCstStat::_ImplicitRelationStruc&
MccImplicitRelationCstStat::_ImplicitRelationStruc::operator= (const MccImplicitRelationCstStat::_ImplicitRelationStruc &right)
{
  if (this != &right)
  {
    delete ref;
    ref = right.ref->clone ();
    delete res;
    res = right.res->clone ();
    delete expr;
    expr = right.expr->clone ();
  }
  return *this;
}

  

void
MccImplicitRelationCstStat::_ImplicitRelationStruc::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccImplicitRelationCstStat::_ImplicitRelationStruc::display (ostream &os) const
{
  ref->display (os);
  os << ' ';
  res->display (os);
  os << ' ';
  expr->display (os);
}



void
MccImplicitRelationCstStat::_ImplicitRelationStruc::ppdisplay (ostream &os, int indent) const
{
  os << endl;
  whitespaces (os, indent);
  ref->ppdisplay (os, indent);
  os << ' ';
  res->ppdisplay (os, indent);
  os << ' ';
  expr->ppdisplay (os, indent);
}



MccImplicitRelationCstStat::MccImplicitRelationCstStat (const MccImplicitRelationCstStat &right)
  : strucs (new vector< MccImplicitRelationCstStat::_ImplicitRelationStruc* > ())
{
  vector< MccImplicitRelationCstStat::_ImplicitRelationStruc* >::const_iterator cit;

  for (cit = right.strucs->begin (); cit != right.strucs->end (); ++cit)
    strucs->push_back ((*cit)->clone ());
}



MccImplicitRelationCstStat::~MccImplicitRelationCstStat ()
{
  vector< _ImplicitRelationStruc* >::iterator it;

  for (it = strucs->begin (); it != strucs->end (); it++)
    delete *it;
  delete strucs;
}



MccImplicitRelationCstStat&
MccImplicitRelationCstStat::operator= (const MccImplicitRelationCstStat &right)
{
  if (this != &right)
  {
    vector< MccImplicitRelationCstStat::_ImplicitRelationStruc* >::const_iterator cit;
    vector< MccImplicitRelationCstStat::_ImplicitRelationStruc* >::iterator it;

    for (it = strucs->begin (); it != strucs->end (); it++)
      delete *it;
    strucs->clear ();
    for (cit = right.strucs->begin (); cit != right.strucs->end (); ++cit)
      strucs->push_back ((*cit)->clone ());
  }
  return *this;
}



void
MccImplicitRelationCstStat::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccImplicitRelationCstStat::display (ostream &os) const
{
  vector< _ImplicitRelationStruc* >::iterator it;

  os << "relation (";
  for (it = strucs->begin (); it != strucs->end (); it++)
  {
    if (it != strucs->begin ())
      os << ' ';
    (*it)->display (os);
  }
  os << ')';
}



void
MccImplicitRelationCstStat::ppdisplay (ostream &os, int indent) const
{
  vector< _ImplicitRelationStruc* >::iterator it;

  os << "relation" << endl;
  whitespaces (os, indent + 2);
  os << '(';
  for (it = strucs->begin (); it != strucs->end (); it++)
    (*it)->ppdisplay (os, indent + 4);
  os << endl;
  whitespaces (os, indent + 2);
  os << ')' << endl;
}



MccRemarkStat::MccRemarkStat (const MccRemarkStat &right)
{
  str = new char[strlen (right.str) + 1];
  strcpy (str, right.str);
}



MccRemarkStat&
MccRemarkStat::operator= (const MccRemarkStat &right)
{
  if (this != &right)
  {
    delete[] str;
    str = new char[strlen (right.str) + 1];
    strcpy (str, right.str);
  }
  return *this;
}



void
MccRemarkStat::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccRemarkStat::ppdisplay (ostream &os, int indent) const
{
  display (os);
  os << endl;
}



void
MccDBResetStat::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccDBResetStat::ppdisplay (ostream &os, int indent) const
{
  display (os);
  os << endl;
}



void
MccResetStat::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccResetStat::ppdisplay (ostream &os, int indent) const
{
  display (os);
  os << endl;
}


MccRestoreStat::MccRestoreStat (const MccRestoreStat &right)
  : filename (strdup (right.filename)),
    expOutput (0)
{
  if (right.expOutput)
    expOutput = right.expOutput->clone ();
}



MccRestoreStat&
MccRestoreStat::operator= (const MccRestoreStat &right)
{
  if (this != &right)
  {
    delete[] filename;
    filename = strdup (right.filename);
    if (expOutput)
    {
      delete expOutput;
      expOutput = 0;
    }
    if (right.expOutput)
      expOutput = right.expOutput->clone ();
  }
  return *this;
}
 

  
void
MccRestoreStat::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccRestoreStat::display (ostream &os) const
{
  os << "restore (" << filename;
  if (expOutput)
    expOutput->display (os);
  os << ')';
}



void
MccRestoreStat::ppdisplay (ostream &os, int indent) const
{
  os << "restore" << endl;
  whitespaces (os, indent + 2);
  os << '(' << endl;
  whitespaces (os, indent + 4);
  os << "\"" << filename << "\"";
  if (expOutput)
  {
    os << endl;
    whitespaces (os, indent + 4);
    expOutput->ppdisplay (os, indent + 4);
  }
  os << endl;
  whitespaces (os, indent + 2);
  os << ')' << endl;
}


MccSamplingSize::MccSamplingSize (float ssize, bool pflag, bool sortflag)
  : discrete (pflag),
    unsorted (sortflag)
{
  if (discrete)
    absVal = (int)rint (ssize);
  else
    propFact = ssize;
}



MccSamplingSize::MccSamplingSize (const MccSamplingSize &right)
  : discrete (right.discrete),
    unsorted (right.unsorted)
{
  if (discrete)
    absVal = right.absVal;
  else
    propFact = right.propFact;
}



MccSamplingSize&
MccSamplingSize::operator= (const MccSamplingSize &right)
{
  if (this != &right)
  {
    discrete = right.discrete;
    unsorted = right.unsorted;
    if (discrete)
      absVal = right.absVal;
    else
      propFact = right.propFact;
  }
  return *this;
}



void
MccSamplingSize::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccSamplingSize::display (ostream &os) const
{
  if (discrete)
    os << absVal;
  else
    os << propFact * 100.0 << '%';
  if (unsorted)
    os << " unsorted";
}



MccSequenceStat::MccSequenceStat (const MccSequenceStat &right)
  : type (right.type),
    res (new MccResidueName (*right.res)),
    seq (right.seq)
{

}



MccSequenceStat&
MccSequenceStat::operator= (const MccSequenceStat &right)
{
  if (this != &right)
  {
    type = right.type;
    delete res;
    res = new MccResidueName (*right.res);
    this->seq = right.seq;
  }
  return *this;
}



void
MccSequenceStat::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccSequenceStat::display (ostream &os) const
{
  os << "sequence (" << type << ' ';
  res->display (os);
  os << ' ' << seq << ')';
}



void
MccSequenceStat::ppdisplay (ostream &os, int indent) const
{
  whitespaces (os, indent);
  this->display (os);
}



MccSourceStat::MccSourceStat (const MccSourceStat &right)
{
  str = new char[strlen (right.str) + 1];
  strcpy (str, right.str);
}



MccSourceStat&
MccSourceStat::operator= (const MccSourceStat &right)
{
  if (this != &right)
  {
    delete[] str;
    str = new char[strlen (right.str) + 1];
    strcpy (str, right.str);
  }
  return *this;
}



void
MccSourceStat::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccSourceStat::ppdisplay (ostream &os, int indent) const
{
  display (os);
  os << endl;
}



MccTorsionCstStat::_TorsionStruc::_TorsionStruc (const MccTorsionCstStat::_TorsionStruc &right)
  : res1 (new MccResidueName (*right.res1)),
    res2 (new MccResidueName (*right.res2)),
    res3 (new MccResidueName (*right.res3)),
    res4 (new MccResidueName (*right.res4)),
    angle (right.angle),
    error (right.error)
{
  at1 = new char[strlen (right.at1) + 1];
  strcpy (at1, right.at1);
  at2 = new char[strlen (right.at2) + 1];
  strcpy (at2, right.at2);
  at3 = new char[strlen (right.at3) + 1];
  strcpy (at3, right.at3);
  at4 = new char[strlen (right.at4) + 1];
  strcpy (at4, right.at4);
}

  

MccTorsionCstStat::_TorsionStruc::~_TorsionStruc ()
{
  delete res1;
  delete res2;
  delete res3;
  delete res4;
  delete[] at1;
  delete[] at2;
  delete[] at3;
  delete[] at4;
}



MccTorsionCstStat::_TorsionStruc&
MccTorsionCstStat::_TorsionStruc::operator= (const MccTorsionCstStat::_TorsionStruc &right)
{
  if (this != &right)
  {
    delete res1;
    res1 = new MccResidueName (*right.res1);
    delete[] at1;
    at1 = new char[strlen (right.at1) + 1];
    strcpy (at1, right.at1);
    delete res2;
    res2 = new MccResidueName (*right.res2);
    delete[] at2;
    at2 = new char[strlen (right.at2) + 1];
    strcpy (at2, right.at2);
    delete res3;
    res3 = new MccResidueName (*right.res3);
    delete[] at3;
    at3 = new char[strlen (right.at3) + 1];
    strcpy (at3, right.at3);
    delete res4;
    res4 = new MccResidueName (*right.res4);
    delete at4;
    at4 = new char[strlen (right.at4) + 1];
    strcpy (at4, right.at4);
    angle = right.angle;
    error = right.error;
  }
  return *this;
}

  

void
MccTorsionCstStat::_TorsionStruc::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccTorsionCstStat::_TorsionStruc::display (ostream &os) const
{
  res1->display (os);
  os << ':' << at1 << ' ';
  res2->display (os);
  os << ':' << at2 << ' ';
  res3->display (os);
  os << ':' << at3 << ' ';
  res4->display (os);
  os << ':' << at4 << ' ' << angle << ' ' << error;
}



void
MccTorsionCstStat::_TorsionStruc::ppdisplay (ostream &os, int indent) const
{
  os << endl;
  whitespaces (os, indent);
  res1->ppdisplay (os, indent);
  os << ':' << at1 << ' ';
  res2->ppdisplay (os, indent);
  os << ':' << at2 << ' ';
  res3->ppdisplay (os, indent);
  os << ':' << at3 << ' ';
  res4->ppdisplay (os, indent);
  os << ':' << at4 << ' ' << angle << ' ' << error;
}



MccTorsionCstStat::MccTorsionCstStat (const MccTorsionCstStat &right)
  : strucs (new vector< MccTorsionCstStat::_TorsionStruc* > ())
{
  vector< MccTorsionCstStat::_TorsionStruc* >::const_iterator cit;

  for (cit = right.strucs->begin (); cit != right.strucs->end (); cit++)
    strucs->push_back (new MccTorsionCstStat::_TorsionStruc (**cit));
}



MccTorsionCstStat::~MccTorsionCstStat ()
{
  vector< _TorsionStruc* >::iterator it;

  for (it = strucs->begin (); it != strucs->end (); it++)
    delete *it;
  delete strucs;
}



MccTorsionCstStat&
MccTorsionCstStat::operator= (const MccTorsionCstStat &right)
{
  if (this != &right)
  {
    vector< MccTorsionCstStat::_TorsionStruc* >::const_iterator cit;
    vector< MccTorsionCstStat::_TorsionStruc* >::iterator it;

    for (it = strucs->begin (); it != strucs->end (); it++)
      delete *it;
    strucs->clear ();
    for (cit = right.strucs->begin (); cit != right.strucs->end (); cit++)
      strucs->push_back (new MccTorsionCstStat::_TorsionStruc (**cit));
  }
  return *this;
}



void
MccTorsionCstStat::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccTorsionCstStat::display (ostream &os) const
{
  vector< _TorsionStruc* >::iterator it;

  os << "torsion (";
  for (it = strucs->begin (); it != strucs->end (); ++it)
  {
    if (it != strucs->begin ())
      os << ' ';
    (*it)->display (os);
  }
  os << ')';
}



void
MccTorsionCstStat::ppdisplay (ostream &os, int indent) const
{
  vector< _TorsionStruc* >::iterator it;

  os << "torsion" << endl;
  whitespaces (os, indent + 2);
  os << '(';
  for (it = strucs->begin (); it != strucs->end (); ++it)
    (*it)->ppdisplay (os, indent + 4);
  os << endl;
  whitespaces (os, indent + 2);
  os << ')' << endl;
}



void
MccVersion::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccVersion::ppdisplay (ostream &os, int indent) const
{
  display (os);
  os << endl;
}


MccReportImplicitPhosphateStat::MccReportImplicitPhosphateStat (const MccFragGenStruc& fg)
  : fg_struc (fg)
{ 
}


MccReportImplicitPhosphateStat::MccReportImplicitPhosphateStat (const MccReportImplicitPhosphateStat& obj)
  : fg_struc (obj.fg_struc)
{
}


MccReportImplicitPhosphateStat&
MccReportImplicitPhosphateStat::operator= (const MccReportImplicitPhosphateStat& obj)
{
  if (this != &obj)
  {
    fg_struc = obj.fg_struc;
  }
  return *this;
}


void
MccReportImplicitPhosphateStat::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}


void
MccReportImplicitPhosphateStat::display (ostream& os) const
{
  this->fg_struc.display (os << "report_implicit_phosphate (");
  os << ")";
}


void
MccReportImplicitPhosphateStat::ppdisplay (ostream& os, int indent) const
{
  whitespaces (os, indent);
  this->display (os);
  os << endl;
}


MccReportRiboseStat::MccReportRiboseStat (const MccFragGenStruc& fg)
  : fg_struc (fg)
{ 
}


MccReportRiboseStat::MccReportRiboseStat (const MccReportRiboseStat& obj)
  : fg_struc (obj.fg_struc)
{
}


MccReportRiboseStat&
MccReportRiboseStat::operator= (const MccReportRiboseStat& obj)
{
  if (this != &obj)
  {
    fg_struc = obj.fg_struc;
  }
  return *this;
}


void
MccReportRiboseStat::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}


void
MccReportRiboseStat::display (ostream& os) const
{
  this->fg_struc.display (os << "report_ribose (");
  os << ")";
}


void
MccReportRiboseStat::ppdisplay (ostream& os, int indent) const
{
  whitespaces (os, indent);
  this->display (os);
  os << endl;
}


LexerException::LexerException (const char *s)
{
  str = new char[strlen (s) + 1];
  strcpy (str, s);
}



LexerException::LexerException (const LexerException &right)
{
  str = new char[strlen (right.str) + 1];
  strcpy (str, right.str);
}



LexerException&
LexerException::operator= (const LexerException &right)
{
  if (this != &right)
  {
    delete[] str;
    str = new char[strlen (right.str) + 1];
    strcpy (str, right.str);
  }
  return *this;
}



LexerException&
LexerException::operator<< (char c)
{
  size_t sz = strlen (str) + 1 + 1;
  char *tmp = new char[sz];

  strcpy (tmp, str);
  delete[] str;
  str = tmp;
  tmp += sz - 2;
  *tmp++ = c;
  *tmp = '\0';
  return *this;
}



LexerException&
LexerException::operator<< (int n)
{
  char tmp1[256];
  char *tmp2;

  sprintf (tmp1, "%d", n);
  tmp2 = new char[strlen (str) + strlen (tmp1) + 1];
  strcpy (tmp2, str);
  strcat (tmp2, tmp1);
  delete[] str;
  str = tmp2;
  return *this;
}



LexerException&
LexerException::operator<< (float x)
{
  char tmp1[256];
  char *tmp2;

  sprintf (tmp1, "%f", x);
  tmp2 = new char[strlen (str) + strlen (tmp1) + 1];
  strcpy (tmp2, str);
  strcat (tmp2, tmp1);
  delete[] str;
  str = tmp2;
  return *this;
}



LexerException&
LexerException::operator<< (const char *s)
{
  char *tmp = new char[strlen (str) + strlen (s) + 1];
  strcpy (tmp, str);
  strcat (tmp, s);
  delete[] str;
  str = tmp;
  return *this;
}



ostream&
operator<< (ostream &os, const LexerException &ex)
{
  return os << ex.GetMessage ();
}



ParserException::ParserException (const char *s)
{
  str = new char[strlen (s) + 1];
  strcpy (str, s);
}



ParserException::ParserException (const ParserException &right)
{
  str = new char[strlen (right.str) + 1];
  strcpy (str, right.str);
}



ParserException&
ParserException::operator= (const ParserException &right)
{
  if (this != &right)
  {
    delete[] str;
    str = new char[strlen (right.str) + 1];
    strcpy (str, right.str);
  }
  return *this;
}



ParserException&
ParserException::operator<< (char c)
{
  size_t sz = strlen (str) + 1 + 1;
  char *tmp = new char[sz];

  strcpy (tmp, str);
  delete[] str;
  str = tmp;
  tmp += sz - 2;
  *tmp++ = c;
  *tmp = '\0';
  return *this;
}



ParserException&
ParserException::operator<< (int n)
{
  char tmp1[256];
  char *tmp2;

  sprintf (tmp1, "%d", n);
  tmp2 = new char[strlen (str) + strlen (tmp1) + 1];
  strcpy (tmp2, str);
  strcat (tmp2, tmp1);
  delete[] str;
  str = tmp2;
  return *this;
}



ParserException&
ParserException::operator<< (float x)
{
  char tmp1[256];
  char *tmp2;

  sprintf (tmp1, "%f", x);
  tmp2 = new char[strlen (str) + strlen (tmp1) + 1];
  strcpy (tmp2, str);
  strcat (tmp2, tmp1);
  delete[] str;
  str = tmp2;
  return *this;
}



ParserException&
ParserException::operator<< (const char *s)
{
  char *tmp = new char[strlen (str) + strlen (s) + 1];
  strcpy (tmp, str);
  strcat (tmp, s);
  delete[] str;
  str = tmp;
  return *this;
}



ostream&
operator<< (ostream &os, const ParserException &ex)
{
  return os << ex.GetMessage ();
}

