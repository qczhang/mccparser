//                              -*- Mode: C++ -*- 
// mccparser.cc
// Copyright © 2000-04 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Fri Aug 25 16:28:36 2000
// $Revision$
// $Id$


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


MccFragGenStruc::MccFragGenStruc (const MccFragGenStruc &right)
  : special_char (right.special_char)
{
  ident = new char[strlen (right.ident) + 1];
  strcpy (ident, right.ident);
}



MccFragGenStruc&
MccFragGenStruc::operator= (const MccFragGenStruc &right)
{
  if (this != &right)
  {
    delete ident;
    ident = new char[strlen (right.ident) + 1];
    strcpy (ident, right.ident);
    special_char = right.special_char;
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


MccAddPdbStat::_AddPdbStruc::_AddPdbStruc (const MccAddPdbStat::_AddPdbStruc &right)
  : strs (new vector< char* > ())
{
  vector< char* >::const_iterator cit;

  for (cit = right.strs->begin (); cit != right.strs->end (); ++cit)
  {
    char *str = new char[strlen (*cit) + 1];

    strcpy (str, *cit);
    strs->push_back (str);
  }
}



MccAddPdbStat::_AddPdbStruc::~_AddPdbStruc ()
{
  vector< char* >::iterator it;

  for (it = strs->begin (); it != strs->end (); ++it)
    delete [] *it;
  delete strs;
}



MccAddPdbStat::_AddPdbStruc&
MccAddPdbStat::_AddPdbStruc::operator= (const MccAddPdbStat::_AddPdbStruc &right)
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
      char *str = new char[strlen (*cit) + 1];
	  
      strcpy (str, *cit);
      strs->push_back (str);
    }
  }
  return *this;
}


      
void
MccAddPdbStat::_AddPdbStruc::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccAddPdbStat::_AddPdbStruc::display (ostream &os) const
{
  vector< char* >::iterator it;

  for (it = strs->begin (); it != strs->end (); ++it)
    os << "  \"" << *it << "\"";
}



void
MccAddPdbStat::_AddPdbStruc::ppdisplay (ostream &os, int indent) const
{
  vector< char* >::iterator it;

  for (it = strs->begin (); it != strs->end (); ++it)
  {
    os << endl;
    whitespaces (os, indent);
    os << "\"" << *it << "\"";
  }
}



MccAddPdbStat::MccAddPdbStat (const MccAddPdbStat &right)
  : strucs (new vector< _AddPdbStruc* > ()),
    cutoffs_asp (new map< char, float > (*right.cutoffs_asp))
//     standard_cutoff (right.standard_cutoff),
//     adjacent_cutoff (right.adjacent_cutoff)
{
  vector< _AddPdbStruc* >::const_iterator cit;
  
  for (cit = right.strucs->begin (); cit != right.strucs->end (); ++cit)
    strucs->push_back ((*cit)->clone ());
}



MccAddPdbStat::~MccAddPdbStat ()
{
  vector< _AddPdbStruc* >::iterator it;

  for (it = strucs->begin (); it != strucs->end (); ++it)
    delete *it;
  delete strucs;

  delete this->cutoffs_asp;
}



MccAddPdbStat&
MccAddPdbStat::operator= (const MccAddPdbStat &right)
{
  if (this != &right)
  {
    vector< _AddPdbStruc* >::const_iterator cit;
    vector< _AddPdbStruc* >::iterator it;

//     standard_cutoff = right.standard_cutoff;
//     adjacent_cutoff = right.adjacent_cutoff;

    for (it = strucs->begin (); it != strucs->end (); ++it)
      delete *it;
    strucs->clear ();

    for (cit = right.strucs->begin (); cit != right.strucs->end (); ++cit)
      strucs->push_back ((*cit)->clone ());

    delete this->cutoffs_asp;
    this->cutoffs_asp = new map< char, float > (*right.cutoffs_asp);
  }
  return *this;
}


  
void
MccAddPdbStat::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccAddPdbStat::display (ostream &os) const
{
  vector< _AddPdbStruc* >::iterator it;
  map< char, float >::iterator cit;

  os << "add_pdb (";
  
  if (!this->cutoffs_asp->empty ())
  {
    os << " cutoff";
    for (cit = this->cutoffs_asp->begin (); cit != this->cutoffs_asp->end (); ++cit)
      os << ' ' << cit->first << " = " << cit->second;
  }

//   if (standard_cutoff >= 0.0)
//     os << " cutoff " << standard_cutoff;
//   if (adjacent_cutoff >= 0.0)
//     os << ' ' << adjacent_cutoff;
  for (it = strucs->begin (); it != strucs->end (); ++it)
    (*it)->display (os << ' ');
  os << ')';
}
  


void
MccAddPdbStat::ppdisplay (ostream &os, int indent) const
{
  vector< _AddPdbStruc* >::iterator it;
  map< char, float >::iterator cit;

  os << "add_pdb" << endl;
  whitespaces (os, indent + 2);
  os << '(';
//   if (standard_cutoff >= 0.0)
//   {
//     os << endl;
//     whitespaces (os, indent + 2);
//     os << "cutoff " << standard_cutoff;
//     if (adjacent_cutoff >= 0.0)
//       os << ' ' << adjacent_cutoff;
//   }
  if (!this->cutoffs_asp->empty ())
  {
    whitespaces (os, indent + 4);
    os << "cutoff";
    for (cit = this->cutoffs_asp->begin (); cit != this->cutoffs_asp->end (); ++cit)
      os << ' ' << cit->first << " = " << cit->second;
  }
  for (it = strucs->begin (); it != strucs->end (); ++it)
  {
    (*it)->ppdisplay (os, indent + 4);
  }
  os << endl;
  whitespaces (os, indent + 2);
  os << ')' << endl;
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


MccRiboseAdjacencyCstStat::MccRiboseAdjacencyCstStat (const MccRiboseAdjacencyCstStat &right)
  : fg_struc (new MccFragGenStruc (*right.fg_struc)),
    the_max (right.the_max)
{ }



MccRiboseAdjacencyCstStat&
MccRiboseAdjacencyCstStat::operator= (const MccRiboseAdjacencyCstStat &right)
{
  if (this != &right)
  {
    delete fg_struc;
    fg_struc = right.fg_struc->clone ();
    the_max = right.the_max;
  }
  return *this;
}



void
MccRiboseAdjacencyCstStat::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}


void
MccRiboseAdjacencyCstStat::display (ostream &os) const
{
  os << "ribose_adjacency (";
  fg_struc->display (os);
  os << ' ' << the_max << ')';
}



void
MccRiboseAdjacencyCstStat::ppdisplay (ostream &os, int indent) const
{
  os << "ribose_adjacency (";
  fg_struc->ppdisplay (os, indent);
  os << ' ' << the_max << ')' << endl;
}



MccAngleCstStat::_AngleStruc::_AngleStruc (const MccAngleCstStat::_AngleStruc &right)
  : res1 (new MccResidueName (*right.res1)),
    res2 (new MccResidueName (*right.res2)),
    res3 (new MccResidueName (*right.res3)),
    dist_min (right.dist_min),
    dist_max (right.dist_max)
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
    dist_min = right.dist_min;
    dist_max = right.dist_max;
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
  os << ":" << at3 << ' ' << dist_min << ' ' << dist_max;
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
  os << ":" << at3 << ' ' << dist_min << ' ' << dist_max;
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
  : fgref (right.fgref->clone ()),
    filter (right.filter)
{ 
  if (this->filter)
    this->filter = this->filter->clone ();
}


MccCacheExpr::~MccCacheExpr ()
{
  delete this->fgref;
  if (this->filter)
    delete this->filter;
}


MccCacheExpr&
MccCacheExpr::operator= (const MccCacheExpr &right)
{
  if (this != &right)
  {
    delete this->fgref;
    this->fgref = right.fgref->clone ();
    if (this->filter)
    {
      delete this->filter;
      this->filter = 0;
    }
    if (right.filter)
      this->filter = right.filter->clone ();
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
  fgref->display (os);
  if (this->filter)
    filter->display (os);
  os << ')';
}



void
MccCacheExpr::ppdisplay (ostream &os, int indent) const
{
  os << "cache (";
  fgref->ppdisplay (os, indent);
  os << endl;
  if (this->filter)
  {
    whitespaces (os, indent + 4);
    filter->ppdisplay (os, indent);
  }
  os << ')' << endl;
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


MccFilePdbOutput::MccFilePdbOutput (const MccFilePdbOutput &right)
  : form (strdup (right.form)),
    zipped (right.zipped)
{ }



MccFilePdbOutput&
MccFilePdbOutput::operator= (const MccFilePdbOutput &right)
{
  if (this != &right)
  {
    delete[] form;
    form = strdup (right.form);
    zipped = right.zipped;
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
  os << "file_pdb (\"" << form << "\"";
  if (zipped)
    os << " zipped";
  os << ")";
}



MccFileBinaryOutput::MccFileBinaryOutput (const MccFileBinaryOutput &right)
  : form (strdup (right.form)),
    zipped (right.zipped)
{ }



MccFileBinaryOutput&
MccFileBinaryOutput::operator= (const MccFileBinaryOutput &right)
{
  if (this != &right)
  {
    delete[] form;
    form = strdup (right.form);
    zipped = right.zipped;
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
  os << "file_bin (\"" << form << "\"";
  if (zipped)
    os << " zipped";
  os << ")";
}



MccSocketBinaryOutput::MccSocketBinaryOutput (const MccSocketBinaryOutput &right)
  : serverName (strdup (right.serverName)),
    port (right.port),
    modelName (strdup (right.modelName))
{ }



MccSocketBinaryOutput&
MccSocketBinaryOutput::operator= (const MccSocketBinaryOutput &right)
{
  if (this != &right)
  {
    delete[] serverName;
    serverName = strdup (right.serverName);
    port = right.port;
    delete[] modelName;
    modelName = strdup (right.modelName);
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
  os << "socket_bin (\"" << serverName << "\" " << port
     << " \"" << modelName << "\")";
}



MccSocketPdbOutput::MccSocketPdbOutput (const MccSocketPdbOutput &right)
  : serverName (strdup (right.serverName)),
    port (right.port),
    modelName (strdup (right.modelName))
{ }



MccSocketPdbOutput&
MccSocketPdbOutput::operator= (const MccSocketPdbOutput &right)
{
  if (this != &right)
  {
    delete[] serverName;
    serverName = strdup (right.serverName);
    port = right.port;
    delete[] modelName;
    modelName = strdup (right.modelName);
  }
  return *this;
}



void
MccSocketPdbOutput::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccSocketPdbOutput::display (ostream &os) const
{
  os << "socket_pdb (\"" << serverName << "\" " << port
     << " \"" << modelName << "\")";
}



MccFileRnamlOutput::MccFileRnamlOutput (const MccFileRnamlOutput &right)
  : name (strdup (right.name)),
    zipped (right.zipped)
{ }



MccFileRnamlOutput&
MccFileRnamlOutput::operator= (const MccFileRnamlOutput &right)
{
  if (this != &right)
  {
    delete[] name;
    name = strdup (right.name);
    zipped = right.zipped;
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
  os << "file_rnaml (\"" << name << "\"";
  if (zipped)
    os << " zipped";
  os << ")";
}



MccFilePdbInput::MccFilePdbInput (const MccFilePdbInput &right)
  : name (strdup (right.name))
{ }



MccFilePdbInput&
MccFilePdbInput::operator= (const MccFilePdbInput &right)
{
  if (this != &right)
  {
    delete[] name;
    name = strdup (right.name);
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
  os << "file_pdb (\"" << name << "\")";
}



MccFileBinaryInput::MccFileBinaryInput (const MccFileBinaryInput &right)
  : name (strdup (right.name))
{ }



MccFileBinaryInput&
MccFileBinaryInput::operator= (const MccFileBinaryInput &right)
{
  if (this != &right)
  {
    delete[] name;
    name = strdup (right.name);
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
  os << "file_bin (\"" << name << "\")";
}



MccSocketBinaryInput::MccSocketBinaryInput (const MccSocketBinaryInput &right)
  : serverName (strdup (right.serverName)),
    port (right.port),
    modelName (strdup (right.modelName))
{ }



MccSocketBinaryInput&
MccSocketBinaryInput::operator= (const MccSocketBinaryInput &right)
{
  if (this != &right)
  {
    delete[] serverName;
    serverName = strdup (right.serverName);
    port = right.port;
    delete[] modelName;
    modelName = strdup (right.modelName);
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
  os << "socket_bin (\"" << serverName << " " << port
     << " \"" << modelName << "\")";
}



MccFileRnamlInput::MccFileRnamlInput (const MccFileRnamlInput &right)
  : name (strdup (right.name))
{ }



MccFileRnamlInput&
MccFileRnamlInput::operator= (const MccFileRnamlInput &right)
{
  if (this != &right)
  {
    delete[] name;
    name = strdup (right.name);
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
  os << "file_rnaml (\"" << name << "\")";
}


MccExploreStat::_ParamStruc&
MccExploreStat::_ParamStruc::operator= (const MccExploreStat::_ParamStruc& right)
{
  if (this != &right)
  {
    delete[] this->name;
    this->name = strdup (right.name);
    this->value = right.value;
  }
  return *this;
}


void
MccExploreStat::_ParamStruc::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}


void
MccExploreStat::_ParamStruc::display (ostream &os) const
{
  os << this->name << ' ' << this->value;
}


void
MccExploreStat::_ParamStruc::ppdisplay (ostream &os, int indent) const
{
  os << this->name << endl;
  whitespaces (os, indent);
  os << this->value << endl;
}



MccExploreStat::MccExploreStat (const MccExploreStat &right)
  : fg_struc (new MccFragGenStruc (*right.fg_struc)),
    algo (strdup (right.algo)),
    filter (0),
    timelimit (right.timelimit),
    expOutput (0)
{
  if (right.filter)
    this->filter = right.filter->clone ();
  if (right.expOutput)
    this->expOutput = right.expOutput->clone ();
}


MccExploreStat::~MccExploreStat ()
{
  delete this->fg_struc;
  delete[] this->algo;
  vector< _ParamStruc* >::iterator pit;
  for (pit = this->params->begin (); this->params->end () != pit; ++pit)
    delete *pit;
  delete this->params;
  if (this->filter)
    delete this->filter;
  if (this->expOutput)
    delete this->expOutput;
}


MccExploreStat&
MccExploreStat::operator= (const MccExploreStat &right)
{
  if (this != &right)
  {
    delete fg_struc;
    fg_struc = right.fg_struc->clone ();
    delete[] this->algo;
    algo = strdup (right.algo);
    
    vector< _ParamStruc* >::iterator pit;
    for (pit = this->params->begin (); this->params->end () != pit; ++pit)
      delete *pit;
    this->params->clear ();
    for (pit = right.params->begin (); right.params->end () != pit; ++pit)
      this->params->push_back ((*pit)->clone ());

    delete *pit;
    if (filter)
    {
      delete filter;
      filter = 0;
    }
    if (right.filter)
      filter = right.filter->clone ();
    if (expOutput)
    {
      delete expOutput;
      expOutput = 0;
    }
    if (right.expOutput)
      expOutput = right.expOutput->clone ();

    timelimit = right.timelimit;
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
  os << "explore (";
  this->fg_struc->display (os);
  os << ' ' << this->algo;
  vector< _ParamStruc* >::iterator pit;
  for (pit = this->params->begin (); this->params->end () != pit; ++pit)
  {
    os << ' ';
    (*pit)->display (os);
  }
  if (this->filter)
  {
    os << ' ';
    this->filter->display (os);
  }
  if (timelimit > 0)
    os << " time_limit (" << timelimit << " seconds)";
  if (this->expOutput)
  {
    os << ' ';
    this->expOutput->display (os);
  }
  os << ')';
}



void
MccExploreStat::ppdisplay (ostream &os, int indent) const
{
  os << "explore" << endl;
  whitespaces (os, indent);
  os << '(' << endl;
  whitespaces (os, indent + 2);
  this->fg_struc->ppdisplay (os, indent + 2);
  os << endl;
  whitespaces (os, indent + 2);
  os << this->algo;
  vector< _ParamStruc* >::iterator pit;
  for (pit = this->params->begin (); this->params->end () != pit; ++pit)
  {
    os << endl;
    whitespaces (os, indent + 2);
    (*pit)->ppdisplay (os, indent + 2);
  }
  if (this->filter)
  {
    os << endl;
    whitespaces (os, indent + 2);
    this->filter->ppdisplay (os, indent + 2);
  }
  if (timelimit > 0)
  {
    os << endl;
    whitespaces (os, indent + 2);
    os << " time_limit (" << timelimit << " seconds)";
  }
  if (this->expOutput)
  {
    os << endl;
    whitespaces (os, indent + 2);
    this->expOutput->ppdisplay (os, indent + 2);
  }
  os << endl;
  whitespaces (os, indent);
  os << ')' << endl;
}


MccLibraryExpr::_StripStruc::_StripStruc (const MccLibraryExpr::_StripStruc &right)
  : res_vec (0)
{
  if (right.res_vec)
  {
    vector< MccResidueName* >::const_iterator cit;
    res_vec = new vector< MccResidueName* > ();
    for (cit = right.res_vec->begin (); cit != right.res_vec->end (); cit++)
      res_vec->push_back ((*cit)->clone ());
  }
}
      

MccLibraryExpr::_StripStruc::~_StripStruc ()
{
  if (this->res_vec)
  {
    vector< MccResidueName* >::iterator it;
    for (it = this->res_vec->begin (); it != this->res_vec->end (); ++it)
      delete *it;
    delete this->res_vec;
  }
}


MccLibraryExpr::_StripStruc&
MccLibraryExpr::_StripStruc::operator= (const MccLibraryExpr::_StripStruc &right)
{
  if (this != &right)
  {
    if (this->res_vec)
    {
      vector< MccResidueName* >::iterator it;
      for (it = this->res_vec->begin (); it != this->res_vec->end (); it++)
	delete *it;
      delete this->res_vec;
      this->res_vec = 0;
    }
    if (right.res_vec)
    {
      vector< MccResidueName* >::const_iterator cit;
      this->res_vec = new vector< MccResidueName* > ();
      for (cit = right.res_vec->begin (); cit != right.res_vec->end (); cit++)
	this->res_vec->push_back ((*cit)->clone ());
    }
  }
  return *this;
}


void
MccLibraryExpr::_StripStruc::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}


void
MccLibraryExpr::_StripStruc::display (ostream &os) const
{
  vector< MccResidueName* >::iterator it;

  os << "strip (";
  for (it = res_vec->begin (); it != res_vec->end (); it++)
  {
    if (it != res_vec->begin ())
      os << ' ';
    (*it)->display (os);
  }
  os << ')';
}


void
MccLibraryExpr::_StripStruc::ppdisplay (ostream &os, int indent) const
{
  vector< MccResidueName* >::iterator it;

  os << endl;
  whitespaces (os, indent);
  os << "strip (";
  for (it = res_vec->begin (); it != res_vec->end (); it++)
  {
    if (it != res_vec->begin ())
      os << ' ';
    (*it)->ppdisplay (os, indent);
  }
  os << ')';
}


MccLibraryExpr::_ChangeIdStruc::~_ChangeIdStruc ()
{
  delete old_id;
  delete new_id;
}


MccLibraryExpr::_ChangeIdStruc&
MccLibraryExpr::_ChangeIdStruc::operator= (const MccLibraryExpr::_ChangeIdStruc &right)
{
  if (this != &right)
  {
    MccLibraryExpr::_LibStruc::operator= (right);
    delete this->old_id;
    this->old_id = right.old_id->clone ();
    delete this->new_id;
    this->new_id = right.new_id->clone ();
  }
  return *this;
}


void
MccLibraryExpr::_ChangeIdStruc::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}


void
MccLibraryExpr::_ChangeIdStruc::display (ostream &os) const
{
  os << "change_id (";
  this->old_id->display (os);
  os << " , ";
  this->new_id->display (os);
  os << ")";
}


void
MccLibraryExpr::_ChangeIdStruc::ppdisplay (ostream &os, int indent) const
{
  os << endl;
  whitespaces (os, indent);
  os << "change_id (";
  this->old_id->display (os);
  os << " , ";
  this->new_id->display (os);
  os << ")";
}


MccLibraryExpr::_ChangeChainStruc&
MccLibraryExpr::_ChangeChainStruc::operator= (const MccLibraryExpr::_ChangeChainStruc &right)
{
  if (this != &right)
  {
    MccLibraryExpr::_LibStruc::operator= (right);
    this->old_chain = right.old_chain;
    this->new_chain = right.new_chain;
  }
  return *this;
}


void
MccLibraryExpr::_ChangeChainStruc::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}


void
MccLibraryExpr::_ChangeChainStruc::display (ostream &os) const
{
  os << "change_id (" << this->old_chain << " , " << this->new_chain << ')';
}


void
MccLibraryExpr::_ChangeChainStruc::ppdisplay (ostream &os, int indent) const
{
  os << endl;
  whitespaces (os, indent);
  os << "change_id (" << this->old_chain << " , " << this->new_chain << ')';
}


MccLibraryExpr::MccLibraryExpr (const MccLibraryExpr &right)
  : inputMode (right.inputMode->clone ()),
    strucs (new vector< MccLibraryExpr::_LibStruc* > ()),
    asis (right.asis)
{
  vector< MccLibraryExpr::_LibStruc* >::const_iterator cit;
  
  for (cit = right.strucs->begin (); cit != right.strucs->end (); cit++)
    strucs->push_back ((*cit)->clone ());
}



MccLibraryExpr::~MccLibraryExpr ()
{
  vector< _LibStruc* >::iterator it;

  delete inputMode;
  for (it = strucs->begin (); it != strucs->end (); it++)
    delete *it;
  delete strucs;
}



MccLibraryExpr&
MccLibraryExpr::operator= (const MccLibraryExpr &right)
{
  if (this != &right)
  {
    vector< MccLibraryExpr::_LibStruc* >::const_iterator cit;
    vector< MccLibraryExpr::_LibStruc* >::iterator it;

    delete inputMode;
    inputMode = right.inputMode->clone ();
    for (it = strucs->begin (); it != strucs->end (); it++)
      delete *it;
    strucs->clear ();
    for (cit = right.strucs->begin (); cit != right.strucs->end (); cit++)
      strucs->push_back ((*cit)->clone ());
    asis = right.asis;
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
  vector< _LibStruc* >::iterator it;

  os << "library (";
  inputMode->display (os);
  for (it = strucs->begin (); it != strucs->end (); it++)
  {
    os << ' ';
    (*it)->display (os);
  }
  if (this->asis)
    os << " as_is";
  os << ')';
}



void
MccLibraryExpr::ppdisplay (ostream &os, int indent) const
{
  vector< _LibStruc* >::iterator it;

  os << "library" << endl;
  whitespaces (os, indent + 2);
  os << '(' << endl;
  whitespaces (os, indent + 4);
  inputMode->ppdisplay (os, indent);
  for (it = strucs->begin (); it != strucs->end (); it++)
    (*it)->ppdisplay (os, indent + 4);
  if (this->asis)
  {
    os << endl;
    whitespaces (os, indent + 4);
    os << "as_is";
  }
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


MccRelationsStat::_RelationsStruc::_RelationsStruc (const MccRelationsStat::_RelationsStruc &right)
  : resrefs (new vector< char* > (right.resrefs->size ())),
    expr (right.expr->clone ()),
    ssize (right.ssize->clone ())
{
  vector< char* >::const_iterator it;
  for (it = right.resrefs->begin (); right.resrefs->end () != it; ++it)
    this->resrefs->push_back (strdup (*it));
}


MccRelationsStat::_RelationsStruc::~_RelationsStruc ()
{
  vector< char* >::iterator it;
  for (it = this->resrefs->begin (); this->resrefs->end () != it; ++it)
    delete[] *it;
  delete this->resrefs;
  delete this->expr;
  delete this->ssize;
}


MccRelationsStat::_RelationsStruc&
MccRelationsStat::_RelationsStruc::operator= (const MccRelationsStat::_RelationsStruc &right)
{
  if (this != &right)
  {
    vector< char* >::iterator it;
    vector< char* >::const_iterator cit;
    for (it = this->resrefs->begin (); this->resrefs->end () != it; ++it)
      delete[] *it;
    delete this->resrefs;
    this->resrefs = new vector< char* > (right.resrefs->size ());
    for (cit = right.resrefs->begin (); right.resrefs->end () != cit; ++cit)
      this->resrefs->push_back (strdup (*it));
    delete expr;
    expr = right.expr->clone ();
    delete ssize;
    ssize = right.ssize->clone ();
  }
  return *this;
}


void
MccRelationsStat::_RelationsStruc::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}


void
MccRelationsStat::_RelationsStruc::display (ostream &os) const
{
  vector< char* >::const_iterator it;
  for (it = this->resrefs->begin (); this->resrefs->end () != it; ++it)
  {
    if (this->resrefs->begin () != it)
      os << ',';
    os << *it;
  }
  os << ' ';
  expr->display (os);
  os << ' ';
  ssize->display (os);
}


void
MccRelationsStat::_RelationsStruc::ppdisplay (ostream &os, int indent) const
{
  os << endl;
  whitespaces (os, indent);
  vector< char* >::const_iterator it;
  for (it = this->resrefs->begin (); this->resrefs->end () != it; ++it)
  {
    if (this->resrefs->begin () != it)
      os << ',';
    os << *it;
  }
  os << ' ';
  expr->ppdisplay (os, indent);
  os << ' ';
  ssize->ppdisplay (os, indent);
}


MccRelationsStat::MccRelationsStat (const MccRelationsStat &right)
  : strucs (new vector< MccRelationsStat::_RelationsStruc* > (right.strucs->size ()))
{
  vector< MccRelationsStat::_RelationsStruc* >::const_iterator cit;
  for (cit = right.strucs->begin (); cit != right.strucs->end (); cit++)
    this->strucs->push_back (new MccRelationsStat::_RelationsStruc (**cit));
}


MccRelationsStat::~MccRelationsStat ()
{
  vector< _RelationsStruc* >::iterator it;
  for (it = this->strucs->begin (); it != this->strucs->end (); ++it)
    delete *it;
  delete this->strucs;
}


MccRelationsStat&
MccRelationsStat::operator= (const MccRelationsStat &right)
{
  if (this != &right)
  {
    vector< MccRelationsStat::_RelationsStruc* >::const_iterator cit;
    vector< MccRelationsStat::_RelationsStruc* >::iterator it;
    for (it = this->strucs->begin (); it != this->strucs->end (); ++it)
      delete *it;
    delete this->strucs;
    this->strucs = new vector< MccRelationsStat::_RelationsStruc* > (right.strucs->size ());
    for (cit = right.strucs->begin (); cit != right.strucs->end (); cit++)
      this->strucs->push_back (new MccRelationsStat::_RelationsStruc (**cit));
  }
  return *this;
}



void
MccRelationsStat::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccRelationsStat::display (ostream &os) const
{
  vector< _RelationsStruc* >::iterator it;
  
  os << "pair (";
  for (it = strucs->begin (); it != strucs->end (); it++)
  {
    if (it != strucs->begin ())
      os << ' ';
    (*it)->display (os);
  }
  os << ')';
}



void
MccRelationsStat::ppdisplay (ostream &os, int indent) const
{
  vector< _RelationsStruc* >::iterator it;
  
  os << "pair" << endl;
  whitespaces (os, indent + 2);
  os << '(';
  for (it = strucs->begin (); it != strucs->end (); it++)
    (*it)->ppdisplay (os, indent + 4);
  os << endl;
  whitespaces (os, indent + 2);
  os << ')' << endl;
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



MccRelationCstStat::_RelationStruc::_RelationStruc (const MccRelationCstStat::_RelationStruc &right)
  : ref (right.ref->clone ()),
    res (right.res->clone ()),
    expr (right.expr->clone ())
{
}

  

MccRelationCstStat::_RelationStruc::~_RelationStruc ()
{
  delete ref;
  delete res;
  delete expr;
}



MccRelationCstStat::_RelationStruc&
MccRelationCstStat::_RelationStruc::operator= (const MccRelationCstStat::_RelationStruc &right)
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
MccRelationCstStat::_RelationStruc::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccRelationCstStat::_RelationStruc::display (ostream &os) const
{
  ref->display (os);
  os << ' ';
  res->display (os);
  os << ' ';
  expr->display (os);
}



void
MccRelationCstStat::_RelationStruc::ppdisplay (ostream &os, int indent) const
{
  os << endl;
  whitespaces (os, indent);
  ref->ppdisplay (os, indent);
  os << ' ';
  res->ppdisplay (os, indent);
  os << ' ';
  expr->ppdisplay (os, indent);
}



MccRelationCstStat::MccRelationCstStat (const MccRelationCstStat &right)
  : strucs (new vector< MccRelationCstStat::_RelationStruc* > ())
{
  vector< MccRelationCstStat::_RelationStruc* >::const_iterator cit;

  for (cit = right.strucs->begin (); cit != right.strucs->end (); ++cit)
    strucs->push_back ((*cit)->clone ());
}



MccRelationCstStat::~MccRelationCstStat ()
{
  vector< _RelationStruc* >::iterator it;

  for (it = strucs->begin (); it != strucs->end (); it++)
    delete *it;
  delete strucs;
}



MccRelationCstStat&
MccRelationCstStat::operator= (const MccRelationCstStat &right)
{
  if (this != &right)
  {
    vector< MccRelationCstStat::_RelationStruc* >::const_iterator cit;
    vector< MccRelationCstStat::_RelationStruc* >::iterator it;

    for (it = strucs->begin (); it != strucs->end (); it++)
      delete *it;
    strucs->clear ();
    for (cit = right.strucs->begin (); cit != right.strucs->end (); ++cit)
      strucs->push_back ((*cit)->clone ());
  }
  return *this;
}



void
MccRelationCstStat::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccRelationCstStat::display (ostream &os) const
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
}



void
MccRelationCstStat::ppdisplay (ostream &os, int indent) const
{
  vector< _RelationStruc* >::iterator it;

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
MccResetDBStat::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccResetDBStat::ppdisplay (ostream &os, int indent) const
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


MccRMSDModelFilter&
MccRMSDModelFilter::operator= (const MccRMSDModelFilter &right)
{
  if (this != &right)
  {
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
MccRMSDModelFilter::accept (MccVisitor *visitor)
{
  visitor->visit (this);
}



void
MccRMSDModelFilter::display (ostream &os) const
{
  os << "rmsd (" << rmsBound;
  if (this->atomset)
    this->atomset->display (os);
  os << ")";
}



void
MccRMSDModelFilter::ppdisplay (ostream &os, int indent) const
{
  display (os);
}


MccSamplingSize::MccSamplingSize (float ssize, bool pflag)
  : discrete (pflag)
{
  if (discrete)
    absVal = (int)rint (ssize);
  else
    propFact = ssize / 100.0;
}



MccSamplingSize::MccSamplingSize (const MccSamplingSize &right)
  : discrete (right.discrete)
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
}



MccSequenceStat::MccSequenceStat (const MccSequenceStat &right)
  : type (right.type),
    res (new MccResidueName (*right.res))
{
  seq = new char[strlen (right.seq) + 1];
  strcpy (seq, right.seq);
}



MccSequenceStat&
MccSequenceStat::operator= (const MccSequenceStat &right)
{
  if (this != &right)
  {
    type = right.type;
    delete res;
    res = new MccResidueName (*right.res);
    delete[] seq;
    seq = new char[strlen (right.seq) + 1];
    strcpy (seq, right.seq);
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
  os << "sequence (" << type << ' ';
  res->ppdisplay (os, indent);
  os << ' ' << seq << ')' << endl;
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
    min (right.min),
    max (right.max)
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
    min = right.min;
    max = right.max;
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
  os << ':' << at4 << ' ' << min << ' ' << max;
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
  os << ':' << at4 << ' ' << min << ' ' << max;
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

