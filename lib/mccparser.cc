//                              -*- Mode: C++ -*- 
// mccparser.cc
// Copyright © 2000-01 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Fri Aug 25 16:28:36 2000
// Last Modified By : Philippe Thibault
// Last Modified On : Mon Mar 24 09:42:16 2003
// Update Count     : 20
// Status           : Ok.
// 


#include <string.h>

#include "mccparser.h"
#include <math.h>



vector< MccPStruct* > *astv;

bool interactive_parser;

CMccInput *input_class;


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
MccFragGenStruc::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
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
MccResidueName::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
}



void
MccQTrueFunc::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
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
MccQIdentFunc::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
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
MccQNotFunc::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
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
MccQFaceFunc::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
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
MccQAndFunc::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
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
MccQOrFunc::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
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
MccQueryExpr::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
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



MccAddPdbStat::_AddPdbStruc::_AddPdbStruc (const MccAddPdbStat::_AddPdbStruc &right)
  : current_tfo_cutoff (right.current_tfo_cutoff),
    current_confo_cutoff (right.current_confo_cutoff),
    strs (new vector< char* > ())
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
  vector < char* >::iterator it;

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
      
      current_tfo_cutoff = right.current_tfo_cutoff;
      current_confo_cutoff = right.current_confo_cutoff;
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
MccAddPdbStat::_AddPdbStruc::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
}



void
MccAddPdbStat::_AddPdbStruc::display (ostream &os) const
{
  vector< char* >::iterator it;

  os << "tfo_cutoff " << current_tfo_cutoff
     << " confo_cutoff " << current_confo_cutoff;
  for (it = strs->begin (); it != strs->end (); ++it)
    os << "  \"" << *it << "\"";
}



void
MccAddPdbStat::_AddPdbStruc::ppdisplay (ostream &os, int indent) const
{
  vector< char* >::iterator it;

  os << endl;
  whitespaces (os, indent);
  os << "tfo_cutoff   " << current_tfo_cutoff << endl;
  whitespaces (os, indent);
  os << "confo_cutoff " << current_confo_cutoff;
  for (it = strs->begin (); it != strs->end (); ++it)
    {
      os << endl;
      whitespaces (os, indent);
      os << "\"" << *it << "\"";
    }
}



MccAddPdbStat::MccAddPdbStat (const MccAddPdbStat &right)
  : strucs (new vector< _AddPdbStruc* > ())
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
}



MccAddPdbStat&
MccAddPdbStat::operator= (const MccAddPdbStat &right)
{
  if (this != &right)
    {
      vector< _AddPdbStruc* >::const_iterator cit;
      vector< _AddPdbStruc* >::iterator it;

      for (it = strucs->begin (); it != strucs->end (); ++it)
	delete *it;
      strucs->clear ();
      for (cit = right.strucs->begin (); cit != right.strucs->end (); ++cit)
	strucs->push_back ((*cit)->clone ());
    }
  return *this;
}


  
void
MccAddPdbStat::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
}



void
MccAddPdbStat::display (ostream &os) const
{
  vector< _AddPdbStruc* >::iterator it;

  os << "add_pdb (";
  for (it = strucs->begin (); it != strucs->end (); ++it)
    {
      if (it != strucs->begin ())
	os << ' ';
      (*it)->display (os);
    }
  os << ')';
}
  


void
MccAddPdbStat::ppdisplay (ostream &os, int indent) const
{
  vector< _AddPdbStruc* >::iterator it;

  os << "add_pdb" << endl;
  whitespaces (os, indent + 2);
  os << '(';
  for (it = strucs->begin (); it != strucs->end (); ++it)
    {
      (*it)->ppdisplay (os, indent + 4);
    }
  os << endl;
  whitespaces (os, indent + 2);
  os << ')' << endl;
}



MccAdjacencyCstStat::MccAdjacencyCstStat (const MccAdjacencyCstStat &right)
  : fg_struc (new MccFragGenStruc (*right.fg_struc)),
    the_min (right.the_min),
    the_max (right.the_max)
{ }



MccAdjacencyCstStat&
MccAdjacencyCstStat::operator= (const MccAdjacencyCstStat &right)
{
  if (this != &right)
    {
      delete fg_struc;
      fg_struc = right.fg_struc->clone ();
      the_min = right.the_min;
      the_max = right.the_max;
    }
  return *this;
}



void
MccAdjacencyCstStat::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
}


void
MccAdjacencyCstStat::display (ostream &os) const
{
  os << "adjacency (";
  fg_struc->display (os);
  os << ' ' << the_min << ' ' << the_max << ')';
}



void
MccAdjacencyCstStat::ppdisplay (ostream &os, int indent) const
{
  os << "adjacency (";
  fg_struc->ppdisplay (os, indent);
  os << ' ' << the_min << ' ' << the_max << ')' << endl;
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
MccAngleCstStat::_AngleStruc::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
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
MccAngleCstStat::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
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
MccAssignStat::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
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
MccBacktrackExpr::_FGStruc::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
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
MccBacktrackExpr::_BTStruc::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
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
MccBacktrackExpr::_PlaceStruc::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
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
MccBacktrackExpr::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
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



MccCacheExpr::MccCacheExpr (const MccCacheExpr &right)
  : fgref (right.fgref->clone ()),
    filter (right.filter->clone ())
{ }



MccCacheExpr&
MccCacheExpr::operator= (const MccCacheExpr &right)
{
  if (this != &right)
    {
      delete fgref;
      fgref = right.fgref->clone ();
      delete filter;
      filter = right.filter->clone ();
    }
  return *this;
}



void
MccCacheExpr::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
}



void
MccCacheExpr::display (ostream &os) const
{
  os << "cache (";
  fgref->display (os);
  os << " ";
  filter->display (os);
  os << ')';
}



void
MccCacheExpr::ppdisplay (ostream &os, int indent) const
{
  os << "cache (";
  fgref->ppdisplay (os, indent);
  os << endl;
  whitespaces (os, indent + 4);
  filter->ppdisplay (os, indent);
  os << ')' << endl;
}



MccClashCstStat::MccClashCstStat (const MccClashCstStat &right)
  : fg_struc (new MccFragGenStruc (*right.fg_struc)),
    VDWDist (right.VDWDist),
    distFac (right.distFac),
    as (right.as),
    aso (right.aso)
{ }



MccClashCstStat&
MccClashCstStat::operator= (const MccClashCstStat &right)
{
  if (this != &right)
    {
      delete fg_struc;
      fg_struc = new MccFragGenStruc (*right.fg_struc);
      VDWDist = right.VDWDist;
      distFac = right.distFac;
      as = right.as;
      aso = right.aso;
    }
  return *this;
}



void
MccClashCstStat::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
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
  switch (as)
    {
    case MCC_ALL_AS:
      os << " all";
      break;
    case MCC_BASE_AS:
      os << " base_only";
      break;
    case MCC_BACKBONE_AS:
      os << " backbone_only";
      break;
    case MCC_PSE_AS:
      os << " pse_only";
      break;
    default:
      break;
    }
  if (aso == MCC_NO_HYDROGEN)
    os << " no_hydrogen";
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
  switch (as)
    {
    case MCC_ALL_AS:
      os << " all";
      break;
    case MCC_BASE_AS:
      os << " base_only";
      break;
    case MCC_BACKBONE_AS:
      os << " backbone_only";
      break;
    case MCC_PSE_AS:
      os << " pse_only";
      break;
    default:
      break;
    }
  if (aso == MCC_NO_HYDROGEN)
    os << " no_hydrogen";
  os << ')' << endl;
}



MccConnectStat::_ConnectStruc::_ConnectStruc (const MccConnectStat::_ConnectStruc &right)
  : res1 (new MccResidueName (*right.res1)),
    res2 (new MccResidueName (*right.res2)),
    expr (new MccQueryExpr (*right.expr)),
    ssize (right.ssize)
{ }



MccConnectStat::_ConnectStruc&
MccConnectStat::_ConnectStruc::operator= (const MccConnectStat::_ConnectStruc &right)
{
  if (this != &right)
    {
      delete res1;
      res1 = new MccResidueName (*right.res1);
      delete res2;      
      res2 = new MccResidueName (*right.res2);
      delete expr;
      expr = new MccQueryExpr (*right.expr);
      ssize = right.ssize;
    }
  return *this;
}



void
MccConnectStat::_ConnectStruc::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
}



void
MccConnectStat::_ConnectStruc::display (ostream &os) const
{
  res1->display (os);
  os << ' ';
  res2->display (os);
  os << ' ';
  expr->display (os);
  os << ' ';
  ssize->display (os);
}



void
MccConnectStat::_ConnectStruc::ppdisplay (ostream &os, int indent) const
{
  os << endl;
  whitespaces (os, indent);
  res1->ppdisplay (os, indent);
  os << ' ';
  res2->ppdisplay (os, indent);
  os << ' ';
  expr->ppdisplay (os, indent);
  os << ' ';
  ssize->ppdisplay (os, indent);
}



MccConnectStat::MccConnectStat (const MccConnectStat &right)
  : strucs (new vector< MccConnectStat::_ConnectStruc* > ())
{
  vector< MccConnectStat::_ConnectStruc* >::const_iterator cit;

  for (cit = right.strucs->begin (); cit != right.strucs->end (); cit++)
    strucs->push_back (new _ConnectStruc (**cit));
}



MccConnectStat::~MccConnectStat ()
{
  vector< _ConnectStruc* >::iterator it;

  for (it = strucs->begin (); it != strucs->end (); it++)
    delete *it;

  delete strucs;
}



MccConnectStat&
MccConnectStat::operator= (const MccConnectStat &right)
{
  if (this != &right)
    {
      vector< MccConnectStat::_ConnectStruc* >::const_iterator cit;
      vector< MccConnectStat::_ConnectStruc* >::iterator it;
      
      for (it = strucs->begin (); it != strucs->end (); it++)
	delete *it;
      strucs->clear ();
      for (cit = right.strucs->begin (); cit != right.strucs->end (); cit++)
	strucs->push_back (new _ConnectStruc (**cit));
    }
  return *this;
}



void
MccConnectStat::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
}



void
MccConnectStat::display (ostream &os) const
{
  vector< _ConnectStruc* >::iterator it;

  os << "connect (";
  for (it = strucs->begin (); it != strucs->end (); it++)
    {
      if (it != strucs->begin ())
	os << ' ';
      (*it)->display (os);
    }
  os << ')';
}



void
MccConnectStat::ppdisplay (ostream &os, int indent) const
{
  vector< _ConnectStruc* >::iterator it;

  os << "connect" << endl;
  whitespaces (os, indent + 2);
  os << '(';
  for (it = strucs->begin (); it != strucs->end (); it++)
    (*it)->ppdisplay (os, indent + 4);
  os << endl;
  whitespaces (os, indent + 2);
  os << ')' << endl;
}



MccCycleCstStat::_CycleStruc::_CycleStruc (const MccCycleCstStat::_CycleStruc &right)
  : res1 (new MccResidueName (*right.res1)),
    res2 (new MccResidueName (*right.res2)),
    dist (right.dist),
    nb (right.nb)
{ }



MccCycleCstStat::_CycleStruc&
MccCycleCstStat::_CycleStruc::operator= (const MccCycleCstStat::_CycleStruc &right)
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
MccCycleCstStat::_CycleStruc::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
}



void
MccCycleCstStat::_CycleStruc::display (ostream &os) const
{
  res1->display (os);
  os << ' ';
  res2->display (os);
  os << ' ' << dist << ' ' << nb;
}



void
MccCycleCstStat::_CycleStruc::ppdisplay (ostream &os, int indent) const
{
  os << endl;
  whitespaces (os, indent);
  res1->ppdisplay (os, indent);
  os << ' ';
  res2->ppdisplay (os, indent);
  os << ' ' << dist << ' ' << nb;
}



MccCycleCstStat::MccCycleCstStat (const MccCycleCstStat &right)
  : strucs (new vector< MccCycleCstStat::_CycleStruc* > ())
{
  vector< MccCycleCstStat::_CycleStruc* >::const_iterator cit;

  for (cit = right.strucs->begin (); cit != right.strucs->end (); cit++)
    strucs->push_back (new _CycleStruc (**cit));
}



MccCycleCstStat::~MccCycleCstStat ()
{
  vector< _CycleStruc* >::iterator it;

  for (it = strucs->begin (); it != strucs->end (); ++it)
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
      for (cit = right.strucs->begin (); cit != right.strucs->end (); cit++)
	strucs->push_back (new _CycleStruc (**cit));
    }
  return *this;
}



void
MccCycleCstStat::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
}



void
MccCycleCstStat::display (ostream &os) const
{
  vector< _CycleStruc* >::iterator it;

  os << "cycle (";
  for (it = strucs->begin (); it != strucs->end (); ++it)
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

  os << "cycle" << endl;
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
MccDisplayFGStat::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
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
MccDistCstStat::_DistStruc::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
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
MccDistCstStat::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
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



const MccBacktrackSize&
MccBacktrackSize::operator= (const MccBacktrackSize &right)
{
  if (this != &right) 
    {
      maxBT = right.maxBT;
      maxLR = right.maxLR;
    }
  return *this;
}



void
MccBacktrackSize::display (ostream &os) const
{
  os << "backtrack_size (" << maxBT << " " << maxLR << ")";
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
MccFilePdbOutput::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
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
MccFileBinaryOutput::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
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
MccSocketBinaryOutput::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
}



void
MccSocketBinaryOutput::display (ostream &os) const
{
  os << "socket_bin (\"" << serverName << "\" " << port
     << " \"" << modelName << "\")";
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
MccFilePdbInput::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
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
MccFileBinaryInput::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
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
MccSocketBinaryInput::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
}



void
MccSocketBinaryInput::display (ostream &os) const
{
  os << "socket_bin (\"" << serverName << " " << port
     << " \"" << modelName << "\")";
}



MccExploreStat::MccExploreStat (const MccExploreStat &right)
  : fg_struc (new MccFragGenStruc (*right.fg_struc)),
    filter (0),
    expOutput (0)
{
  if (right.filter)
    filter = right.filter->clone ();
  if (right.expOutput)
    expOutput = right.expOutput->clone ();
}



MccExploreStat&
MccExploreStat::operator= (const MccExploreStat &right)
{
  if (this != &right)
    {
      delete fg_struc;
      fg_struc = right.fg_struc->clone ();
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
    }
  return *this;
}



void
MccExploreStat::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
}



void
MccExploreStat::display (ostream &os) const
{
  os << "explore (";
  fg_struc->display (os);
  if (filter)
    {
      os << ' ';
      filter->display (os);
    }
  if (expOutput)
    {
      os << ' ';
      expOutput->display (os);
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
  fg_struc->ppdisplay (os, indent + 2);
  if (filter)
    {
      os << endl;
      whitespaces (os, indent + 2);
      filter->ppdisplay (os, indent + 2);
    }
  if (expOutput)
    {
      os << endl;
      whitespaces (os, indent + 2);
      expOutput->ppdisplay (os, indent + 2);
    }
  os << endl;
  whitespaces (os, indent);
  os << ')' << endl;
}



MccExploreLVStat::MccExploreLVStat (MccFragGenStruc *fg,
				    MccModelFilterStrategy *f,
				    MccOutputMode *ef,
				    vector< int > *tl,
				    MccBacktrackSize *bs)
  : MccPStruct (),
    fg_struc (fg),
    filter (f),
    expOutput (ef),
    vtlimits (tl),
    tlimit (0),
    btsize (bs)
{
  if (vtlimits)
    {
      vector< int >::const_iterator it;
      
      tlimit = 0;
      for (it = vtlimits->begin (); it != vtlimits->end (); it++)
	tlimit += *it;
    }
}



MccExploreLVStat::MccExploreLVStat (const MccExploreLVStat &right)
  : MccPStruct (right),
    fg_struc (right.fg_struc->clone ()),
    filter (0),
    expOutput (0),
    vtlimits (0),
    tlimit (0),
    btsize (0)
{
  if (right.filter)
    filter = right.filter->clone ();
  if (right.expOutput)
    expOutput = right.expOutput->clone ();
  if (right.btsize)
    btsize = right.btsize->clone ();
  if (right.vtlimits)
    {
      vtlimits = new vector< int > (*right.vtlimits);
      tlimit = right.tlimit;
    }
}



MccExploreLVStat&
MccExploreLVStat::operator= (const MccExploreLVStat &right)
{
  if (this != &right)
    {
      MccPStruct::operator= (right);
      delete fg_struc;
      fg_struc = right.fg_struc->clone ();
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
      if (vtlimits)
        {
	  delete vtlimits;
	  vtlimits = 0;
	  tlimit = 0;
	}
      if (right.vtlimits)
        {
	  vtlimits = new vector< int  > (*right.vtlimits);
	  tlimit = right.tlimit;
	}
      if (btsize)
        {
	  delete btsize;
	  btsize = 0;
	}
      if (right.btsize)
	btsize = right.btsize->clone ();
    }
  return *this;
}



void
MccExploreLVStat::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
}



void
MccExploreLVStat::display (ostream &os) const
{
  os << "exploreLV (";
  fg_struc->display (os);
  if (filter)
    {
      os << ' ';
      filter->display (os);
    }
  if (expOutput)
    {
      os << ' ';
      expOutput->display (os);
    }
  if (vtlimits)
    os << " time_limit (" << tlimit << " sec)";
  if (btsize)
    {
      os << ' ';
      btsize->display (os);
    }
  os << ')';
}




void
MccExploreLVStat::ppdisplay (ostream &os, int indent) const
{
  os << "exploreLV" << endl;
  whitespaces (os, indent);
  os << '(' << endl;
  whitespaces (os, indent + 2);
  fg_struc->ppdisplay (os, indent + 2);
  if (filter)
    {
      os << endl;
      whitespaces (os, indent + 2);
      filter->ppdisplay (os, indent + 2);
    }
  if (expOutput)
    {
      os << endl;
      whitespaces (os, indent + 2);
      expOutput->ppdisplay (os, indent + 2);
    }
  if (vtlimits)
    {
      os << endl;
      whitespaces (os, indent + 2);
      os << "time_limit (" << tlimit << " sec)";
    }
  if (btsize)
    {
      os << endl;
      whitespaces (os, indent + 2);
      btsize->ppdisplay (os, indent + 2);
    }
  os << endl;
  whitespaces (os, indent);
  os << ')' << endl;
}



MccLibraryExpr::_LibStruc::_LibStruc (const MccLibraryExpr::_LibStruc &right)
  : res_vec (0),
    from (right.from),
    to (right.to)
{
  if (right.res_vec)
    {
      vector< MccResidueName* >::const_iterator cit;
      
      res_vec = new vector< MccResidueName* > ();
      for (cit = right.res_vec->begin (); cit != right.res_vec->end (); cit++)
	res_vec->push_back ((*cit)->clone ());
    }
}
      


MccLibraryExpr::_LibStruc::~_LibStruc ()
{
  if (res_vec)
    {
      vector< MccResidueName* >::iterator it;
      
      for (it = res_vec->begin (); it != res_vec->end (); ++it)
	delete *it;
      delete res_vec;
    }
}



MccLibraryExpr::_LibStruc&
MccLibraryExpr::_LibStruc::operator= (const MccLibraryExpr::_LibStruc &right)
{
  if (this != &right)
    {
      if (res_vec)
	{
	  vector< MccResidueName* >::iterator it;

	  for (it = res_vec->begin (); it != res_vec->end (); it++)
	    delete *it;
	  delete res_vec;
	  res_vec = 0;
	}
      if (right.res_vec)
	{
	  vector< MccResidueName* >::const_iterator cit;
	  
	  res_vec = new vector< MccResidueName* > ();
	  for (cit = right.res_vec->begin (); cit != right.res_vec->end (); cit++)
	    res_vec->push_back ((*cit)->clone ());
	}
      from = right.from;
      to = right.to;
    }
  return *this;
}
      


MccLibraryExpr::_StripStruc&
MccLibraryExpr::_StripStruc::operator= (const MccLibraryExpr::_StripStruc &right)
{
  if (this != &right)
    MccLibraryExpr::_LibStruc::operator= (right);
  return *this;
}



void
MccLibraryExpr::_StripStruc::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
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



MccLibraryExpr::_ChangeIdStruc&
MccLibraryExpr::_ChangeIdStruc::operator= (const MccLibraryExpr::_ChangeIdStruc &right)
{
  if (this != &right)
    MccLibraryExpr::_LibStruc::operator= (right);
  return *this;
}



void
MccLibraryExpr::_ChangeIdStruc::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
}



void
MccLibraryExpr::_ChangeIdStruc::ppdisplay (ostream &os, int indent) const
{
  os << endl;
  whitespaces (os, indent);
  os << "change_id (\"" << from << "\" , \"" << to << "\")";
}



MccLibraryExpr::MccLibraryExpr (const MccLibraryExpr &right)
  : inputMode (right.inputMode->clone ()),
    sorter (right.sorter ? right.sorter->clone () : NULL),
    strucs (new vector< MccLibraryExpr::_LibStruc* > ())
{
  vector< MccLibraryExpr::_LibStruc* >::const_iterator cit;
  
  for (cit = right.strucs->begin (); cit != right.strucs->end (); cit++)
    strucs->push_back ((*cit)->clone ());
}



MccLibraryExpr::~MccLibraryExpr ()
{
  vector< _LibStruc* >::iterator it;

  delete inputMode;
  if (sorter)
    delete sorter;
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
      if (sorter)
	{
	  delete sorter;
	  sorter = NULL;
	}
      if (right.sorter)
	sorter = right.sorter->clone ();
      for (it = strucs->begin (); it != strucs->end (); it++)
	delete *it;
      strucs->clear ();
      for (cit = right.strucs->begin (); cit != right.strucs->end (); cit++)
	strucs->push_back ((*cit)->clone ());
    }
  return *this;
}



void
MccLibraryExpr::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
}



void
MccLibraryExpr::display (ostream &os) const
{
  vector< _LibStruc* >::iterator it;

  os << "library (";
  inputMode->display (os);
  if (sorter)
    sorter->display (os);
  for (it = strucs->begin (); it != strucs->end (); it++)
    {
      os << ' ';
      (*it)->display (os);
    }
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
  if (sorter)
    sorter->ppdisplay (os, indent);
  for (it = strucs->begin (); it != strucs->end (); it++)
    (*it)->ppdisplay (os, indent + 4);
  os << endl;
  whitespaces (os, indent + 2);
  os << ')' << endl;
}


MccMosesExpr::MccMosesExpr (char *r, int cid, MccMosesQueries* q, 
			    int epc, int win, char* dir)
  : range (r),
    ctid (cid),
    queries (q),
    mfold_epc (epc),
    mfold_win (win),
    mfold_dir (dir)
{
}


MccMosesExpr::MccMosesExpr (const MccMosesExpr &right)
  : ctid (right.ctid),
    queries (right.queries->clone ()),
    mfold_epc (right.mfold_epc),
    mfold_win (right.mfold_win)
{
  range = right.range ? strdup (right.range) : NULL;
  mfold_dir = right.mfold_dir ? strdup (right.mfold_dir) : NULL;
}


MccMosesExpr::~MccMosesExpr ()
{
  if (range) delete range; 
  delete queries; 
  if (mfold_dir) delete mfold_dir;
}


MccMosesExpr&
MccMosesExpr::operator= (const MccMosesExpr &right)
{
  if (this != &right)
    {
      delete range;
      range = right.range ? strdup (right.range) : NULL;
      ctid = right.ctid;
      delete queries;
      queries = right.queries->clone ();
      mfold_epc = right.mfold_epc;
      mfold_win = right.mfold_win;
      delete mfold_dir;
      mfold_dir = right.mfold_dir ? strdup (right.mfold_dir) : NULL;
    }
  return *this;
}


void
MccMosesExpr::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
}


void
MccMosesExpr::display (ostream &os) const
{
  os << "moses (";
  if (range)
    os << ' ' << range;
  if (ctid >= 0)
    os << " sec_struct (" << ctid << ')';
  os << ' ';
  queries->display (os);
  if (mfold_epc >= 0)
    os << " mfold_epc (" << mfold_epc << ')';
  if (mfold_win >= 0)
    os << " mfold_win (" << mfold_win << ')';
  if (mfold_dir)
    os << " mfold_ouput (\"" << mfold_dir << "\")";
  os << ')';
}


void
MccMosesExpr::ppdisplay (ostream &os, int indent) const
{
  os << "moses" << endl;
  whitespaces (os, indent);
  os << '(' << endl;
  if (range)
    {
      whitespaces (os, indent + 2);
      os << range << endl;
    }
  if (ctid >= 0)
    {
      whitespaces (os, indent + 2);
      os << "sec_struct (" << ctid << ')' <<  endl;
    }
  queries->ppdisplay (os, indent + 2);
  if (mfold_epc >= 0)
    {
      whitespaces (os, indent + 2);
      os << "mfold_epc (" << mfold_epc << ')' << endl;
    }
  if (mfold_win >= 0)
    {
      whitespaces (os, indent + 2);
      os << "mfold_win (" << mfold_win << ')' << endl;
    }
  if (mfold_dir)
    {
      whitespaces (os, indent + 2);
      os << "mfold_ouput (\"" << mfold_dir << "\")" << endl;
    }
  whitespaces (os, indent);
  os << ')' << endl;
}


MccMosesQueries::MccMosesQueries (const MccMosesQueries &right)
{
  stem_cg_au_res = right.stem_cg_au_res ? 
    new pair< MccQueryExpr*, MccSamplingSize* > 
    (right.stem_cg_au_res->first->clone (),
     right.stem_cg_au_res->second->clone ()) 
    : NULL;
  stem_gu_res = right.stem_gu_res ? 
    new pair< MccQueryExpr*, MccSamplingSize* > 
    (right.stem_gu_res->first->clone (),
     right.stem_gu_res->second->clone ()) 
    : NULL;
  loop_res = right.loop_res ? 
    new pair< MccQueryExpr*, MccSamplingSize* > 
    (right.loop_res->first->clone (),
     right.loop_res->second->clone ()) 
    : NULL;
  stem_connect = right.stem_connect ? 
    new pair< MccQueryExpr*, MccSamplingSize* > 
    (right.stem_connect->first->clone (),
     right.stem_connect->second->clone ()) 
    : NULL;
  loop_connect = right.loop_connect ? 
    new pair< MccQueryExpr*, MccSamplingSize* > 
    (right.loop_connect->first->clone (),
     right.loop_connect->second->clone ()) 
    : NULL;
  stem_cg_au_pair = right.stem_cg_au_pair ? 
    new pair< MccQueryExpr*, MccSamplingSize* > 
    (right.stem_cg_au_pair->first->clone (),
     right.stem_cg_au_pair->second->clone ()) 
    : NULL;
  stem_gu_pair = right.stem_gu_pair ? 
    new pair< MccQueryExpr*, MccSamplingSize* > 
    (right.stem_gu_pair->first->clone (),
     right.stem_gu_pair->second->clone ()) 
    : NULL;     
}


MccMosesQueries::~MccMosesQueries ()
{
  if (stem_cg_au_res) delete stem_cg_au_res; 
  if (stem_gu_res) delete stem_gu_res; 
  if (loop_res) delete loop_res; 
  if (stem_connect) delete stem_connect; 
  if (loop_connect) delete loop_connect; 
  if (stem_cg_au_pair) delete stem_cg_au_pair; 
  if (stem_gu_pair) delete stem_cg_au_pair; 
}


MccMosesQueries&
MccMosesQueries::operator= (const MccMosesQueries &right)
{
  if (this != &right)
    {
      if (stem_cg_au_res) delete stem_cg_au_res; 
      if (stem_gu_res) delete stem_gu_res; 
      if (loop_res) delete loop_res; 
      if (stem_connect) delete stem_connect; 
      if (loop_connect) delete loop_connect; 
      if (stem_cg_au_pair) delete stem_cg_au_pair; 
      if (stem_gu_pair) delete stem_gu_pair; 
      stem_cg_au_res = right.stem_cg_au_res ? 
	new pair< MccQueryExpr*, MccSamplingSize* > 
	(right.stem_cg_au_res->first->clone (),
	 right.stem_cg_au_res->second->clone ()) 
	: NULL;
      stem_gu_res = right.stem_gu_res ? 
	new pair< MccQueryExpr*, MccSamplingSize* > 
	(right.stem_gu_res->first->clone (),
	 right.stem_gu_res->second->clone ()) 
	: NULL;
      loop_res = right.loop_res ? 
	new pair< MccQueryExpr*, MccSamplingSize* > 
	(right.loop_res->first->clone (),
	 right.loop_res->second->clone ()) 
	: NULL;
      stem_connect = right.stem_connect ? 
	new pair< MccQueryExpr*, MccSamplingSize* > 
	(right.stem_connect->first->clone (),
	 right.stem_connect->second->clone ()) 
	: NULL;
      loop_connect = right.loop_connect ? 
	new pair< MccQueryExpr*, MccSamplingSize* > 
	(right.loop_connect->first->clone (),
	 right.loop_connect->second->clone ()) 
	: NULL;
      stem_cg_au_pair = right.stem_cg_au_pair ? 
	new pair< MccQueryExpr*, MccSamplingSize* > 
	(right.stem_cg_au_pair->first->clone (),
	 right.stem_cg_au_pair->second->clone ()) 
	: NULL;
      stem_gu_pair = right.stem_gu_pair ? 
	new pair< MccQueryExpr*, MccSamplingSize* > 
	(right.stem_gu_pair->first->clone (),
	 right.stem_gu_pair->second->clone ()) 
	: NULL;     
    }
  return *this;
}


void
MccMosesQueries::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
}


void
MccMosesQueries::display (ostream &os) const
{
  if (stem_cg_au_res)
    {
      os << " stem_cg_au_residue ";
      stem_cg_au_res->first->display (os);
      stem_cg_au_res->second->display (os);
    }
  if (stem_gu_res)
    {
      os << " stem_gu_residue ";
      stem_gu_res->first->display (os);
      stem_gu_res->second->display (os);
    }
 if (loop_res)
    {
      os << " loop_residue ";
      loop_res->first->display (os);
      loop_res->second->display (os);
    }
 if (stem_connect)
    {
      os << " stem_connect ";
      stem_connect->first->display (os);
      stem_connect->second->display (os);
    }
 if (loop_connect)
    {
      os << " loop_connect ";
      loop_connect->first->display (os);
      loop_connect->second->display (os);
    }
 if (stem_cg_au_pair)
    {
      os << " stem_cg_au_pair ";
      stem_cg_au_pair->first->display (os);
      stem_cg_au_pair->second->display (os);
    }
 if (stem_cg_au_pair)
    {
      os << " stem_gu_pair ";
      stem_gu_pair->first->display (os);
      stem_gu_pair->second->display (os);
    }
}


void
MccMosesQueries::ppdisplay (ostream &os, int indent) const
{
  if (stem_cg_au_res)
    {
      whitespaces (os, indent);
      os << "stem_cg_au_residue ";
      stem_cg_au_res->first->ppdisplay (os, indent);
      os << ' ';
      stem_cg_au_res->second->ppdisplay (os, indent);
      os << endl;
    }
  if (stem_gu_res)
    {
      whitespaces (os, indent);
      os << "stem_gu_residue ";
      stem_gu_res->first->ppdisplay (os, indent);
      os << ' ';
      stem_gu_res->second->ppdisplay (os, indent);
      os << endl;
    }
  if (loop_res)
    {
      whitespaces (os, indent);
      os << "loop_residue ";
      loop_res->first->ppdisplay (os, indent);
      os << ' ';
      loop_res->second->ppdisplay (os, indent);
      os << endl;
    }
  if (stem_connect)
    {
      whitespaces (os, indent);
      os << "stem_connect ";
      stem_connect->first->ppdisplay (os, indent);
      os << ' ';
      stem_connect->second->ppdisplay (os, indent);
      os << endl;
    }
  if (loop_connect)
    {
      whitespaces (os, indent);
      os << "loop_connect ";
      loop_connect->first->ppdisplay (os, indent);
      os << ' ';
      loop_connect->second->ppdisplay (os, indent);
      os << endl;
    }
  if (stem_cg_au_pair)
    {
      whitespaces (os, indent);
      os << "stem_cg_au_pair ";
      stem_cg_au_pair->first->ppdisplay (os, indent);
      os << ' ';
      stem_cg_au_pair->second->ppdisplay (os, indent);
      os << endl;
    }
  if (stem_cg_au_pair)
    {
      whitespaces (os, indent);
      os << "stem_gu_pair ";
      stem_gu_pair->first->ppdisplay (os, indent);
      os << ' ';
      stem_gu_pair->second->ppdisplay (os, indent);
      os << endl;
    }
}


MccNewTagStat::MccNewTagStat (const MccNewTagStat &right)
  : resq_opt (right.resq_opt),
    exprs (new vector< MccQueryExpr* > ())
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
  
      resq_opt = right.resq_opt;
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
MccNewTagStat::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
}



void
MccNewTagStat::display (ostream &os) const
{
  vector< MccQueryExpr* >::iterator it;

  os << "new_tag (";
  if (resq_opt)
    os << "res";
  else
    os << "tfo";
  os << " \"" << id << "\"";
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
  if (resq_opt)
    os << "res";
  else
    os << "tfo";
  os << " \"" << id << "\"";
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
MccNoteStat::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
}



void
MccNoteStat::ppdisplay (ostream &os, int indent) const
{
  display (os);
  os << endl;
}



void
MccNotesStat::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
}



void
MccNotesStat::ppdisplay (ostream &os, int indent) const
{
  display (os);
  os << endl;
}



MccPairStat::_PairStruc::_PairStruc (const MccPairStat::_PairStruc &right)
  : res1 (new MccResidueName (*right.res1)),
    res2 (new MccResidueName (*right.res2)),
    expr (new MccQueryExpr (*right.expr)),
    ssize (right.ssize)
{ }



MccPairStat::_PairStruc&
MccPairStat::_PairStruc::operator= (const MccPairStat::_PairStruc &right)
{
  if (this != &right)
    {
      delete res1;
      res1 = new MccResidueName (*right.res1);
      delete res2;
      res2 = new MccResidueName (*right.res2);
      delete expr;
      expr = new MccQueryExpr (*right.expr);
      ssize = right.ssize;
    }
  return *this;
}



void
MccPairStat::_PairStruc::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
}



void
MccPairStat::_PairStruc::display (ostream &os) const
{
  res1->display (os);
  os << ' ';
  res2->display (os);
  os << ' ';
  expr->display (os);
  os << ' ';
  ssize->display (os);
}



void
MccPairStat::_PairStruc::ppdisplay (ostream &os, int indent) const
{
  os << endl;
  whitespaces (os, indent);
  res1->ppdisplay (os, indent);
  os << ' ';
  res2->ppdisplay (os, indent);
  os << ' ';
  expr->ppdisplay (os, indent);
  os << ' ';
  ssize->ppdisplay (os, indent);
}



MccPairStat::MccPairStat (const MccPairStat &right)
  : strucs (new vector< MccPairStat::_PairStruc* > ())
{
  vector< MccPairStat::_PairStruc* >::const_iterator cit;

  for (cit = right.strucs->begin (); cit != right.strucs->end (); cit++)
    strucs->push_back (new MccPairStat::_PairStruc (**cit));
}



MccPairStat::~MccPairStat ()
{
  vector< _PairStruc* >::iterator it;

  for (it = strucs->begin (); it != strucs->end (); it++)
    delete *it;
  delete strucs;
}



MccPairStat&
MccPairStat::operator= (const MccPairStat &right)
{
  if (this != &right)
    {
      vector< MccPairStat::_PairStruc* >::const_iterator cit;
      vector< MccPairStat::_PairStruc* >::iterator it;

      for (it = strucs->begin (); it != strucs->end (); it++)
	delete *it;
      strucs->clear ();
      for (cit = right.strucs->begin (); cit != right.strucs->end (); cit++)
	strucs->push_back (new MccPairStat::_PairStruc (**cit));
    }
  return *this;
}



void
MccPairStat::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
}



void
MccPairStat::display (ostream &os) const
{
  vector< _PairStruc* >::iterator it;
  
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
MccPairStat::ppdisplay (ostream &os, int indent) const
{
  vector< _PairStruc* >::iterator it;
  
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
MccQuitStat::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
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
MccRelationCstStat::_RelationStruc::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
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
MccRelationCstStat::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
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
MccRemarkStat::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
}



void
MccRemarkStat::ppdisplay (ostream &os, int indent) const
{
  display (os);
  os << endl;
}



void
MccResetDBStat::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
}



void
MccResetDBStat::ppdisplay (ostream &os, int indent) const
{
  display (os);
  os << endl;
}



void
MccResetStat::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
}



void
MccResetStat::ppdisplay (ostream &os, int indent) const
{
  display (os);
  os << endl;
}



MccResidueStat::_ResidueStruc::_ResidueStruc (const MccResidueStat::_ResidueStruc &right)
  : res1 (new MccResidueName (*right.res1)),
    res2 (0),
    expr (new MccQueryExpr (*right.expr)),
    ssize (new MccSamplingSize (*right.ssize))
  //! theo confo
//     theo_flag (right.theo_flag)
  //!
{
  if (right.res2)
    res2 = new MccResidueName (*right.res2);
}


MccResidueStat::_ResidueStruc&
MccResidueStat::_ResidueStruc::operator= (const MccResidueStat::_ResidueStruc &right)
{
  if (this != &right)
    {
      delete res1;
      res1 = new MccResidueName (*right.res1);
      if (res2)
	{
	  delete res2;
	  res2 = 0;
	}
      if (right.res2)
	res2 = new MccResidueName (*right.res2);
      delete expr;
      expr = new MccQueryExpr (*right.expr);
      delete ssize;
      ssize = new MccSamplingSize (*right.ssize);
      //! theo confo
//       theo_flag = right.theo_flag;
      //!
    }
  return *this;
}



void
MccResidueStat::_ResidueStruc::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
}



void
MccResidueStat::_ResidueStruc::display (ostream &os) const
{
  res1->display (os);
  os << ' ';
  if (res2)
    {
      res2->display (os);
      os << ' ';
    }
  expr->display (os);
  os << ' ';
  ssize->display (os);
}



void
MccResidueStat::_ResidueStruc::ppdisplay (ostream &os, int indent) const
{
  os << endl;
  whitespaces (os, indent);
  res1->ppdisplay (os, indent);
  os << ' ';
  if (res2)
    {
      res2->ppdisplay (os, indent);
      os << ' ';
    }
  expr->ppdisplay (os, indent);
  os << ' ';
  ssize->ppdisplay (os, indent);
}



MccResidueStat::MccResidueStat (const MccResidueStat &right)
  : strucs (new vector< MccResidueStat::_ResidueStruc* > ())
{
  vector< MccResidueStat::_ResidueStruc* >::const_iterator cit;

  for (cit = right.strucs->begin (); cit != right.strucs->end (); cit++)
    strucs->push_back (new MccResidueStat::_ResidueStruc (**cit));
}



MccResidueStat::~MccResidueStat ()
{
  vector< MccResidueStat::_ResidueStruc* >::iterator it;

  for (it = strucs->begin (); it != strucs->end (); it++)
    delete *it;

  delete strucs;
}



MccResidueStat&
MccResidueStat::operator= (const MccResidueStat &right)
{
  if (this != &right)
    {
      vector< MccResidueStat::_ResidueStruc* >::const_iterator cit;
      vector< MccResidueStat::_ResidueStruc* >::iterator it;

      for (it = strucs->begin (); it != strucs->end (); it++)
	delete *it;
      strucs->clear ();
      for (cit = right.strucs->begin (); cit != right.strucs->end (); cit++)
	strucs->push_back (new MccResidueStat::_ResidueStruc (**cit));
    }
  return *this;
}



void
MccResidueStat::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
}



void
MccResidueStat::display (ostream &os) const
{
  vector< _ResidueStruc* >::iterator it;
  
  os << "residue (";
  for (it = strucs->begin (); it != strucs->end (); it++)
    {
      if (it != strucs->begin ())
	os << ' ';
      (*it)->display (os);
    }
  os << ')';
}



void
MccResidueStat::ppdisplay (ostream &os, int indent) const
{
  vector< _ResidueStruc* >::iterator it;
  
  os << "residue" << endl;
  whitespaces (os, indent + 2);
  os << '(';
  for (it = strucs->begin (); it != strucs->end (); it++)
    (*it)->ppdisplay (os, indent + 4);
  os << endl;
  whitespaces (os, indent + 2);
  os << ')' << endl;
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
MccRestoreStat::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
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



MccRmsdModelFilterStrategy&
MccRmsdModelFilterStrategy::operator= (const MccRmsdModelFilterStrategy &right)
{
  if (this != &right)
    {
      rmsBound = right.rmsBound;
      alignFlag = right.alignFlag;
      atom_set = right.atom_set;
      atom_set_opt = right.atom_set_opt;
    }
  return *this;
}



void
MccRmsdModelFilterStrategy::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
}



void
MccRmsdModelFilterStrategy::display (ostream &os) const
{
  os << "rmsd (" << rmsBound;
  if (alignFlag)
    os << " align";
  switch (atom_set)
    {
    case MCC_ALL_AS:
      os << " all";
      break;
    case MCC_BASE_AS:
      os << " base_only";
      break;
    case MCC_BACKBONE_AS:
      os << " backbone_only";
      break;
    case MCC_PSE_AS:
      os << " pse_only";
      break;
    default:
      break;
    }
  if (atom_set_opt == MCC_NO_HYDROGEN)
    os << " no_hydrogen";
  os << ")";
}



void
MccRmsdModelFilterStrategy::ppdisplay (ostream &os, int indent) const
{
  display (os);
}


MccClusteredModelSorterStrategy&
MccClusteredModelSorterStrategy::operator= (const MccClusteredModelSorterStrategy &right)
{
  if (this != &right)
    {
      partitions = right.partitions;
      rmsBound = right.rmsBound;
      atom_set = right.atom_set;
      atom_set_opt = right.atom_set_opt;
    }
  return *this;
}



void
MccClusteredModelSorterStrategy::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
}



void
MccClusteredModelSorterStrategy::display (ostream &os) const
{
  os << " clustered (";
  if (rmsBound < 0)
    os << partitions;
  else
    os << rmsBound << " Angstroms";
  switch (atom_set)
    {
    case MCC_ALL_AS:
      os << " all";
      break;
    case MCC_BASE_AS:
      os << " base_only";
      break;
    case MCC_BACKBONE_AS:
      os << " backbone_only";
      break;
    case MCC_PSE_AS:
      os << " pse_only";
      break;
    default:
      break;
    }
  if (atom_set_opt == MCC_NO_HYDROGEN)
    os << " no_hydrogen";
  os << ")";
}


void
MccClusteredModelSorterStrategy::ppdisplay (ostream &os, int indent) const
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
MccSamplingSize::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
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
MccSequenceStat::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
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
MccSourceStat::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
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
MccTorsionCstStat::_TorsionStruc::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
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
MccTorsionCstStat::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
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
MccVersion::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
}



void
MccVersion::ppdisplay (ostream &os, int indent) const
{
  display (os);
  os << endl;
}



CLexerException::CLexerException (const char *s)
{
  str = new char[strlen (s) + 1];
  strcpy (str, s);
}



CLexerException::CLexerException (const CLexerException &right)
{
  str = new char[strlen (right.str) + 1];
  strcpy (str, right.str);
}



CLexerException&
CLexerException::operator= (const CLexerException &right)
{
  if (this != &right)
    {
      delete[] str;
      str = new char[strlen (right.str) + 1];
      strcpy (str, right.str);
    }
  return *this;
}



CLexerException&
CLexerException::operator<< (char c)
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



CLexerException&
CLexerException::operator<< (int n)
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



CLexerException&
CLexerException::operator<< (float x)
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



CLexerException&
CLexerException::operator<< (const char *s)
{
  char *tmp = new char[strlen (str) + strlen (s) + 1];
  strcpy (tmp, str);
  strcat (tmp, s);
  delete[] str;
  str = tmp;
  return *this;
}



ostream&
operator<< (ostream &os, const CLexerException &ex)
{
  return os << ex.GetMessage ();
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



CParserException&
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



CParserException&
CParserException::operator<< (char c)
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



CParserException&
CParserException::operator<< (int n)
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



CParserException&
CParserException::operator<< (float x)
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



CParserException&
CParserException::operator<< (const char *s)
{
  char *tmp = new char[strlen (str) + strlen (s) + 1];
  strcpy (tmp, str);
  strcat (tmp, s);
  delete[] str;
  str = tmp;
  return *this;
}


//! placer

MccPlacerResidueName&
MccPlacerResidueName::operator= (const MccPlacerResidueName &right)
{
  if (this != &right)
    {
      id = right.id;
      no = right.no;
    }
  return *this;
}



void
MccPlacerResidueName::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
}


MccPlacerSequenceStat::MccPlacerSequenceStat (const MccPlacerSequenceStat &right)
  : type (right.type),
    res (new MccPlacerResidueName (*right.res))
{
  seq = new char[strlen (right.seq) + 1];
  strcpy (seq, right.seq);
}



MccPlacerSequenceStat&
MccPlacerSequenceStat::operator= (const MccPlacerSequenceStat &right)
{
  if (this != &right)
    {
      type = right.type;
      delete res;
      res = new MccPlacerResidueName (*right.res);
      delete[] seq;
      seq = new char[strlen (right.seq) + 1];
      strcpy (seq, right.seq);
    }
  return *this;
}



void
MccPlacerSequenceStat::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
}



void
MccPlacerSequenceStat::display (ostream &os) const
{
  os << "placer_sequence (" << type << ' ';
  res->display (os);
  os << ' ' << seq << ')';
}



void
MccPlacerSequenceStat::ppdisplay (ostream &os, int indent) const
{
  os << "placer_sequence (" << type << ' ';
  res->ppdisplay (os, indent);
  os << ' ' << seq << ')' << endl;
}


MccPlacerConnectStat::_PlacerConnectStruc::_PlacerConnectStruc (const MccPlacerConnectStat::_PlacerConnectStruc &right)
  : res1 (new MccPlacerResidueName (*right.res1)),
    res2 (new MccPlacerResidueName (*right.res2)),
    expr (new MccQueryExpr (*right.expr)),
    ssize (right.ssize)
{ }



MccPlacerConnectStat::_PlacerConnectStruc&
MccPlacerConnectStat::_PlacerConnectStruc::operator= (const MccPlacerConnectStat::_PlacerConnectStruc &right)
{
  if (this != &right)
    {
      delete res1;
      res1 = new MccPlacerResidueName (*right.res1);
      delete res2;      
      res2 = new MccPlacerResidueName (*right.res2);
      delete expr;
      expr = new MccQueryExpr (*right.expr);
      ssize = right.ssize;
    }
  return *this;
}



void
MccPlacerConnectStat::_PlacerConnectStruc::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
}



void
MccPlacerConnectStat::_PlacerConnectStruc::display (ostream &os) const
{
  res1->display (os);
  os << ' ';
  res2->display (os);
  os << ' ';
  expr->display (os);
  os << ' ';
  ssize->display (os);
}



void
MccPlacerConnectStat::_PlacerConnectStruc::ppdisplay (ostream &os, int indent) const
{
  os << endl;
  whitespaces (os, indent);
  res1->ppdisplay (os, indent);
  os << ' ';
  res2->ppdisplay (os, indent);
  os << ' ';
  expr->ppdisplay (os, indent);
  os << ' ';
  ssize->ppdisplay (os, indent);
}



MccPlacerConnectStat::MccPlacerConnectStat (const MccPlacerConnectStat &right)
  : strucs (new vector< MccPlacerConnectStat::_PlacerConnectStruc* > ())
{
  vector< MccPlacerConnectStat::_PlacerConnectStruc* >::const_iterator cit;

  for (cit = right.strucs->begin (); cit != right.strucs->end (); cit++)
    strucs->push_back (new _PlacerConnectStruc (**cit));
}



MccPlacerConnectStat::~MccPlacerConnectStat ()
{
  vector< _PlacerConnectStruc* >::iterator it;

  for (it = strucs->begin (); it != strucs->end (); it++)
    delete *it;

  delete strucs;
}



MccPlacerConnectStat&
MccPlacerConnectStat::operator= (const MccPlacerConnectStat &right)
{
  if (this != &right)
    {
      vector< MccPlacerConnectStat::_PlacerConnectStruc* >::const_iterator cit;
      vector< MccPlacerConnectStat::_PlacerConnectStruc* >::iterator it;
      
      for (it = strucs->begin (); it != strucs->end (); it++)
	delete *it;
      strucs->clear ();
      for (cit = right.strucs->begin (); cit != right.strucs->end (); cit++)
	strucs->push_back (new _PlacerConnectStruc (**cit));
    }
  return *this;
}



void
MccPlacerConnectStat::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
}



void
MccPlacerConnectStat::display (ostream &os) const
{
  vector< _PlacerConnectStruc* >::iterator it;

  os << "placer_connect (";
  for (it = strucs->begin (); it != strucs->end (); it++)
    {
      if (it != strucs->begin ())
	os << ' ';
      (*it)->display (os);
    }
  os << ')';
}



void
MccPlacerConnectStat::ppdisplay (ostream &os, int indent) const
{
  vector< _PlacerConnectStruc* >::iterator it;

  os << "placer_connect" << endl;
  whitespaces (os, indent + 2);
  os << '(';
  for (it = strucs->begin (); it != strucs->end (); it++)
    (*it)->ppdisplay (os, indent + 4);
  os << endl;
  whitespaces (os, indent + 2);
  os << ')' << endl;
}


MccPlacerPairStat::_PlacerPairStruc::_PlacerPairStruc (const MccPlacerPairStat::_PlacerPairStruc &right)
  : res1 (new MccPlacerResidueName (*right.res1)),
    res2 (new MccPlacerResidueName (*right.res2)),
    expr (new MccQueryExpr (*right.expr)),
    ssize (right.ssize)
{ }



MccPlacerPairStat::_PlacerPairStruc&
MccPlacerPairStat::_PlacerPairStruc::operator= (const MccPlacerPairStat::_PlacerPairStruc &right)
{
  if (this != &right)
    {
      delete res1;
      res1 = new MccPlacerResidueName (*right.res1);
      delete res2;
      res2 = new MccPlacerResidueName (*right.res2);
      delete expr;
      expr = new MccQueryExpr (*right.expr);
      ssize = right.ssize;
    }
  return *this;
}



void
MccPlacerPairStat::_PlacerPairStruc::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
}



void
MccPlacerPairStat::_PlacerPairStruc::display (ostream &os) const
{
  res1->display (os);
  os << ' ';
  res2->display (os);
  os << ' ';
  expr->display (os);
  os << ' ';
  ssize->display (os);
}



void
MccPlacerPairStat::_PlacerPairStruc::ppdisplay (ostream &os, int indent) const
{
  os << endl;
  whitespaces (os, indent);
  res1->ppdisplay (os, indent);
  os << ' ';
  res2->ppdisplay (os, indent);
  os << ' ';
  expr->ppdisplay (os, indent);
  os << ' ';
  ssize->ppdisplay (os, indent);
}



MccPlacerPairStat::MccPlacerPairStat (const MccPlacerPairStat &right)
  : strucs (new vector< MccPlacerPairStat::_PlacerPairStruc* > ())
{
  vector< MccPlacerPairStat::_PlacerPairStruc* >::const_iterator cit;

  for (cit = right.strucs->begin (); cit != right.strucs->end (); cit++)
    strucs->push_back (new MccPlacerPairStat::_PlacerPairStruc (**cit));
}



MccPlacerPairStat::~MccPlacerPairStat ()
{
  vector< _PlacerPairStruc* >::iterator it;

  for (it = strucs->begin (); it != strucs->end (); it++)
    delete *it;
  delete strucs;
}



MccPlacerPairStat&
MccPlacerPairStat::operator= (const MccPlacerPairStat &right)
{
  if (this != &right)
    {
      vector< MccPlacerPairStat::_PlacerPairStruc* >::const_iterator cit;
      vector< MccPlacerPairStat::_PlacerPairStruc* >::iterator it;

      for (it = strucs->begin (); it != strucs->end (); it++)
	delete *it;
      strucs->clear ();
      for (cit = right.strucs->begin (); cit != right.strucs->end (); cit++)
	strucs->push_back (new MccPlacerPairStat::_PlacerPairStruc (**cit));
    }
  return *this;
}



void
MccPlacerPairStat::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
}



void
MccPlacerPairStat::display (ostream &os) const
{
  vector< _PlacerPairStruc* >::iterator it;
  
  os << "placer_pair (";
  for (it = strucs->begin (); it != strucs->end (); it++)
    {
      if (it != strucs->begin ())
	os << ' ';
      (*it)->display (os);
    }
  os << ')';
}



void
MccPlacerPairStat::ppdisplay (ostream &os, int indent) const
{
  vector< _PlacerPairStruc* >::iterator it;
  
  os << "placer_pair" << endl;
  whitespaces (os, indent + 2);
  os << '(';
  for (it = strucs->begin (); it != strucs->end (); it++)
    (*it)->ppdisplay (os, indent + 4);
  os << endl;
  whitespaces (os, indent + 2);
  os << ')' << endl;
}

MccPlacerBuildStat::_GenBTStruc::_GenBTStruc (const MccPlacerBuildStat::_GenBTStruc &right)
  : ref (0), res (0), fg_struc (0), res_v (0)
{
  if (right.ref)
    ref = new MccPlacerResidueName (*right.ref);
  if (right.res)
    res = new MccPlacerResidueName (*right.res);
  if (right.fg_struc)
    fg_struc = new MccFragGenStruc (*right.fg_struc);
  if (right.res_v)
    {
      vector< MccPlacerResidueName* >::const_iterator cit;
      
      res_v = new vector< MccPlacerResidueName* > ();
      for (cit = right.res_v->begin (); cit != right.res_v->end (); cit++)
	res_v->push_back (new MccPlacerResidueName (**cit));
    }
}



MccPlacerBuildStat::_GenBTStruc&
MccPlacerBuildStat::_GenBTStruc::operator= (const MccPlacerBuildStat::_GenBTStruc &right)
{
  if (this != &right)
    {
      if (ref)
	{
	  delete ref;
	  ref = 0;
	}
      if (right.ref)
	ref = new MccPlacerResidueName (*right.ref);
      if (res)
	{
	  delete res;
	  res = 0;
	}
      if (right.res)
	res = new MccPlacerResidueName (*right.res);
      if (fg_struc)
	{
	  delete fg_struc;
	  fg_struc = 0;
	  }
      if (right.fg_struc)
	fg_struc = new MccFragGenStruc (*right.fg_struc);
      if (res_v)
	{
	  vector< MccPlacerResidueName* >::iterator it;

	  for (it = res_v->begin (); it != res_v->end (); it++)
	    delete *it;
	  delete res_v;
	  res_v = 0;
	}
      if (right.res_v)
	{
	  vector< MccPlacerResidueName* >::const_iterator cit;
	  
	  res_v = new vector< MccPlacerResidueName* > ();
	  for (cit = right.res_v->begin (); cit != right.res_v->end (); cit++)
	    res_v->push_back (new MccPlacerResidueName (**cit));
	  
	}
    }
  return *this;
}


MccPlacerBuildStat::_BTStruc::~_BTStruc ()
{
  vector< MccPlacerResidueName* >::iterator it;

  delete ref;
  for (it = res_v->begin (); it != res_v->end (); it++)
    delete *it;
  delete res_v;
}



void
MccPlacerBuildStat::_BTStruc::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
}



MccPlacerBuildStat::_BTStruc&
MccPlacerBuildStat::_BTStruc::operator= (const MccPlacerBuildStat::_BTStruc &right)
{
  if (this != &right)
    MccPlacerBuildStat::_BTStruc::operator= (right);
  return *this;
}



void
MccPlacerBuildStat::_BTStruc::display (ostream &os) const
{
  vector< MccPlacerResidueName* >::iterator it;

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
MccPlacerBuildStat::_BTStruc::ppdisplay (ostream &os, int indent) const
{
  vector< MccPlacerResidueName* >::iterator it;

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


MccPlacerBuildStat::MccPlacerBuildStat (const MccPlacerBuildStat &right)
  : strucs (new vector< MccPlacerBuildStat::_GenBTStruc* > ())
{
  vector< MccPlacerBuildStat::_GenBTStruc* >::const_iterator cit;

  for (cit = right.strucs->begin (); cit != right.strucs->end (); cit++)
    strucs->push_back ((*cit)->clone ());
}



MccPlacerBuildStat::~MccPlacerBuildStat ()
{
  vector< _GenBTStruc* >::iterator it;

  for (it = strucs->begin (); it != strucs->end (); it++)
    delete *it;
  delete strucs;
}



MccPlacerBuildStat&
MccPlacerBuildStat::operator= (const MccPlacerBuildStat &right)
{
  if (this != &right)
    {
      vector< MccPlacerBuildStat::_GenBTStruc* >::const_iterator cit;
      vector< MccPlacerBuildStat::_GenBTStruc* >::iterator it;

      for (it = strucs->begin (); it != strucs->end (); it++)
	delete *it;
      strucs->clear ();
      for (cit = right.strucs->begin (); cit != right.strucs->end (); cit++)
	strucs->push_back ((*cit)->clone ());
    }
  return *this;
}



void
MccPlacerBuildStat::Accept (MccVisitor *visitor)
{
  visitor->Visit (this);
}



void
MccPlacerBuildStat::display (ostream &os) const
{
  vector< _GenBTStruc* >::iterator it;

  os << "placer_build (";
  for (it = strucs->begin (); it != strucs->end (); it++)
    {
      if (it != strucs->begin ())
	os << ' ';
      (*it)->display (os);
    }
  os << ')';
}



void
MccPlacerBuildStat::ppdisplay (ostream &os, int indent) const
{
  vector< _GenBTStruc* >::iterator it;

  os << "placer_build" << endl;
  whitespaces (os, indent + 2);
  os << '(';
  for (it = strucs->begin (); it != strucs->end (); it++)
    (*it)->ppdisplay (os, indent + 4);
  os << endl;
  whitespaces (os, indent + 2);
  os << ')' << endl;
}


//!


ostream&
operator<< (ostream &os, const CParserException &ex)
{
  return os << ex.GetMessage ();
}
