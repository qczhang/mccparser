//                              -*- Mode: C++ -*- 
// mccparser.h
// Copyright © 2000-04 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Thu Aug 24 12:14:42 2000
// $Revision$
// $Id$


#ifndef _mccparser_h_
#define _mccparser_h_

#include <iostream>
#include <utility>
#include <stdio.h>
#include <vector>
#include <set>
#include <map>
#include <list>

class MccPStruct;
class MccVisitor;
class MccQueryExpr;

using namespace std;


/**
 * The variable containing the AST read by the parser.
 */
extern vector< MccPStruct* > *astv;


/**
 * The variable indicating if the parser must run interactively or not.
 */
extern bool interactive_parser;


/**
 * The variable containing the input stream.  It is defined in lex.mccp.c.
 */
extern FILE *mccin;


/**
 * The parser function.  It is defined in y.tab.c.
 */
extern int mccparse ();


/**
 * @short This class is used for reading input from terminal.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
class MccInput
{
  /**
   * Initializes the object with the rights contents.  It must not be used.
   * @param right the object to copy.
   */
  MccInput (const MccInput &right) { }

  /**
   * Assigns the rights content in the object.  It must not be used.
   * @param right the object to copy.
   * @return itself.
   */
  virtual const MccInput& operator= (const MccInput &right) { return *this; }

public:

  // LIFECYCLE ------------------------------------------------------------

  /**
   * Initializes the object.
   */
  MccInput () { }

  /**
   * Destroys the object.
   */
  virtual ~MccInput () { }

  // OPERATORS -------------------------------------------------------------

  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Gets the next character in the buffer.  If the buffer gets to the end,
   * it waits for user input.
   * @return the character read.
   */
  virtual int nextchar () { return getc (mccin); }

  // I/O ------------------------------------------------------------------

};



/**
 * The variable containing the input class.
 */
extern MccInput *input_class;


/**
 * @short Parent struct for the mcc parser structures.
 *
 * These structures are used for building an abstract syntax tree for a
 * mcsym script.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccPStruct
{
  // LIFECYCLE ------------------------------------------------------------

  /**
   * Initializes the structure.  Nothing to do.
   */
  MccPStruct () { }

  /**
   * Initializes the struct with the rights contents.
   * @param right the object to copy.
   */
  MccPStruct (const MccPStruct &right) { }
  
  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccPStruct* clone () const = 0;

  /**
   * Destroys the structure.  Nothing to do.
   */
  virtual ~MccPStruct () { }

  //OPERATORS -------------------------------------------------------------

  /**
   * Assigns the rights content in the object.
   * @param right the object to copy.
   * @return itself.
   */
  MccPStruct& operator= (const MccPStruct &right) { return *this; }

  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor) = 0;
  
  // I/O ------------------------------------------------------------------

  /**
   * Displays the script.  It normally never reaches this method.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const = 0;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0)  const = 0;
};

ostream& operator<< (ostream& os, MccPStruct& obj);

/**
 * @short Parent structure for the fragment generator expressions.
 *
 * These structures are used for building an abstract syntax tree for the
 * fg expressions.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccFGExp
{
  // LIFECYCLE ------------------------------------------------------------

  /**
   * Initializes the structure.  Nothing to do.
   */
  MccFGExp () { }

  /**
   * Initializes the struct with the rights contents.
   * @param right the object to copy.
   */
  MccFGExp (const MccFGExp &right) { }
  
  /**
   * Destroys the structure.  Nothing to do.
   */
  virtual ~MccFGExp () { }

  //OPERATORS -------------------------------------------------------------

  /**
   * Assigns the rights content in the object.
   * @param right the object to copy.
   * @return itself.
   */
  MccFGExp& operator= (const MccFGExp &right) { return *this; }

  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccFGExp* clone () const = 0;
  
  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor) = 0;
  
  // I/O ------------------------------------------------------------------

  /**
   * Displays the script.  It normally never reaches this method.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const = 0;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const = 0;
};



/**
 * @short Struct representing the FG reference AST node.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccFragGenStruc
{
  /**
   * The name of the FG.
   */
  string ident;

  /**
   * The variable indicates if the identifier contains any special
   * character.
   */
  bool special_char;
  

  // LIFECYCLE ------------------------------------------------------------

  /**
   * Initializes the object.  It should never be used.
   */
  MccFragGenStruc () 
    : special_char (false) 
  { }

  /**
   * Initializes the object.
   * @param i the name of the FG.
   * @param sp the variable specifying if the ident contains any special char.
   */
  MccFragGenStruc (const string& i, bool sp = false) 
    : ident (i), special_char (sp) 
  { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccFragGenStruc (const MccFragGenStruc &right)
    : ident (right.ident),
      special_char (right.special_char)
  { }

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  MccFragGenStruc* clone () const { return new MccFragGenStruc (*this); }

  /**
   * Destroys the object.
   */
  ~MccFragGenStruc () { }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content in the object.
   * @param right the object to copy.
   * @return itself.
   */
  MccFragGenStruc& operator= (const MccFragGenStruc &right);

  // ACCESS ---------------------------------------------------------------

  // METHODS --------------------------------------------------------------

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  void accept (MccVisitor *visitor);
  
  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  void display (ostream &os) const;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  void ppdisplay (ostream &os, int indent = 0) const 
  { 
    display (os); 
  }
};


/**
 * @short Struct representing the sampling size on AST nodes "residue",
 * "connect" and "pair" 
 *
 * @author Philippe Thibault <thibaup@IRO.UMontreal.CA>
 */
struct MccSamplingSize
{
  /**
   * Contains either an absolute value or a proportion factor
   * of a set size
   */
  union
  {
    int absVal;    // absolute size
    float propFact;    // proportion of whole set
  };

  /**
   * Indicates if the current union value is an absolute size.
   */
  bool discrete;

protected:

  // LIFECYCLE ------------------------------------------------------------

  /**
   * Initializes the object.  It should never be used.
   */
  MccSamplingSize () { }
  
public:
  
  /**
   * Initializes the object.
   * @param ssize  set sampling size, either an absolute value or a
   * proportion (%) of whole set, depending on pflag value.
   * @param pflag flag indicating if ssize is a proportion or an absolute
   * value. 
   */
  MccSamplingSize (float ssize, bool pflag);

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccSamplingSize (const MccSamplingSize &right);

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  MccSamplingSize* clone () const { return new MccSamplingSize (*this); }
    
  /**
   * Destructs the object.
   */
  virtual ~MccSamplingSize () { }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the right struct values to the object.
   * @param right the struct to copy.
   */
  MccSamplingSize& operator= (const MccSamplingSize &right);

  // ACCESS ---------------------------------------------------------------

  /**
   * Tells if the sampling is a fixed size of a proportion.
   * @return true if the sampling is a fixed size.
   */
  bool isDiscrete () const { return discrete; }

  /**
   * Gets the concrete value of the sampling.
   * @return the integer value of the union.
   */
  int getSamplingSize () const { return absVal; }

  /**
   * Gets the proportion value of the sampling.
   * @return the float value of the union.
   */
  float getSamplingFactor () const { return propFact; }
  
  // METHODS --------------------------------------------------------------

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  void display (ostream &os) const;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  void ppdisplay (ostream &os, int indent = 0) const { display (os); }
};



/**
 * @short Struct representing the residue reference.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccResidueName
{
  /**
   * The chain id of the residue.
   */
  char id;

  /**
   * The residue number of the residue.
   */
  int no;

  
  // LIFECYCLE ------------------------------------------------------------

  /**
   * Initializes the object.
   */
  MccResidueName () : id (' '), no (-1) { }

  /**
   * Initializes the object.
   * @param n the residue number of the residue.
   */
  MccResidueName (int n) : id (' '), no (n) { }

  /**
   * Initializes the object.
   * @param i the chain id of the residue.
   * @param n the residue number of the residue.
   */
  MccResidueName (char i, int n) : id (i), no (n) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccResidueName (const MccResidueName &right)
    : id (right.id), no (right.no) { }
  
  /**
   * Destroys the object.
   */
  ~MccResidueName () { }

  // OPERATORS ------------------------------------------------------------

  MccResidueName& operator= (const MccResidueName &right);
  
  bool operator< (const MccResidueName& rn) const
  {
    return this->id == rn.id ? this->no < rn.no : this->id < rn.id;
  }

  // ACCESS ---------------------------------------------------------------

  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  MccResidueName* clone () const { return new MccResidueName (*this); }

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  void accept (MccVisitor *visitor);
  
  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  void display (ostream &os) const { if (id != ' ') os << id; os << no; }

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  void ppdisplay (ostream &os, int indent = 0) const { display (os); }
};



/**
 * @short Struct representing a list of residue reference pair.
 *
 * @author Philippe Thibault <philippe.thibault@umontreal.ca>
 */
struct MccResidueNamePairs
{
  
  list< pair< MccResidueName, MccResidueName > > pairs;
  list< pair< MccResidueName, MccResidueName > > ranges;
  
  // LIFECYCLE ------------------------------------------------------------

public:

  /**
   * Initializes the object.
   */
  MccResidueNamePairs () { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccResidueNamePairs (const MccResidueNamePairs& right)
    : pairs (right.pairs),
      ranges (right.ranges)
  { }
  
  /**
   * Destroys the object.
   */
  ~MccResidueNamePairs () 
  { }

  // OPERATORS ------------------------------------------------------------

  MccResidueNamePairs& operator= (const MccResidueNamePairs& right);
  
  // ACCESS ---------------------------------------------------------------

  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  MccResidueNamePairs* clone () const 
  { 
    return new MccResidueNamePairs (*this); 
  }

  bool empty () const
  {
    return this->pairs.empty () && this->ranges.empty ();
  }

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  void accept (MccVisitor *visitor);
  
  void add (const MccResidueNamePairs& rnp);

  void add (const MccResidueName& rn1, const MccResidueName& rn2);

  void addRange (const MccResidueName& rn1, const MccResidueName& rn2);

  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  void display (ostream &os) const;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  void ppdisplay (ostream &os, int indent = 0) const;
};


/**
 * @short Struct representing a list of residue reference singletons.
 *
 * @author Philippe Thibault <philippe.thibault@umontreal.ca>
 */
struct MccResidueNameSingletons
{
  
  list< MccResidueName > singletons;
  list< pair< MccResidueName, MccResidueName > > ranges;
  
  // LIFECYCLE ------------------------------------------------------------

public:

  /**
   * Initializes the object.
   */
  MccResidueNameSingletons () { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccResidueNameSingletons (const MccResidueNameSingletons& right)
    : singletons (right.singletons),
      ranges (right.ranges)
  { }
  
  /**
   * Destroys the object.
   */
  ~MccResidueNameSingletons () 
  { }

  // OPERATORS ------------------------------------------------------------

  MccResidueNameSingletons& operator= (const MccResidueNameSingletons& right);
  
  // ACCESS ---------------------------------------------------------------

  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  MccResidueNameSingletons* clone () const 
  { 
    return new MccResidueNameSingletons (*this); 
  }

  bool empty () const
  {
    return this->singletons.empty () && this->ranges.empty ();
  }

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  void accept (MccVisitor *visitor);
  
  void add (const MccResidueNameSingletons& rnp);

  void add (const MccResidueName& rn);

  void addRange (const MccResidueName& rn1, const MccResidueName& rn2);

  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  void display (ostream &os) const;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  void ppdisplay (ostream &os, int indent = 0) const;
};


/**
 * @short Structures representing the query expressions.
 *
 * The struct contains a serie of structures describing the test fuctions
 * (NOT, AND, OR, ID) for the query.  We can display to a ostream the
 * original query by using the display method.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccQFunc
{
  // LIFECYCLE ------------------------------------------------------------
  
  /**
   * Initializes the object.
   */
  MccQFunc () { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccQFunc (const MccQFunc &right) { }

  /**
   * Destroys the object.
   */
  virtual ~MccQFunc () { }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content in the object.
   * @param right the object to copy.
   * @return itself.
   */
  MccQFunc& operator= (const MccQFunc &right) { return *this; }
  
  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccQFunc* clone () const = 0;

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor) = 0;
  
  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const = 0;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const = 0;
};



/**
 * @short Struct representing the AST "true" in query statements.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccQTrueFunc : public MccQFunc
{
  // LIFECYCLE ------------------------------------------------------------
  
  /**
   * Initializes the object.
   */
  MccQTrueFunc () { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccQTrueFunc (const MccQTrueFunc &right) { }

  /**
   * Destroys the object.
   */
  virtual ~MccQTrueFunc () { }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content in the object.
   * @param right the object to copy.
   * @return itself.
   */
  MccQTrueFunc& operator= (const MccQTrueFunc &right) { return *this; }
  
  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccQTrueFunc* clone () const { return new MccQTrueFunc (*this); }

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);
  
  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const { }

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const { }
};



/**
 * @short Struct representing the AST identifier in query statements.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccQIdentFunc : public MccQFunc
{
  /**
   * The name of the identifier.
   */
  char *str;

  
  // LIFECYCLE ------------------------------------------------------------

private:
  /**
   * Initializes the object.  It should never be used.
   */
  MccQIdentFunc () { }
public:

  /**
   * Initializes the object.
   * @param s the name of the identifier.
   */
  MccQIdentFunc (char *s) : str (s) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccQIdentFunc (const MccQIdentFunc &right);

  /**
   * Destroys the object.
   */
  virtual ~MccQIdentFunc () { delete[] str; }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content in the object.
   * @param right the object to copy.
   * @return itself.
   */
  MccQIdentFunc& operator= (const MccQIdentFunc &right);
  
  // ACCESS ---------------------------------------------------------------

  /**
   * Sets the name of the identifier.
   * @param s the new name of the identifier.
   */
  void SetStr (char *s) { delete[] str; str = s; }
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccQIdentFunc* clone () const { return new MccQIdentFunc (*this); }

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);
  
  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const { os << str; }

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const { display (os); }
};



/**
 * @short Struct representing the AST not statement in queries.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccQNotFunc : public MccQFunc
{
  /**
   * The query that is negated.
   */
  MccQFunc *fn;

  
  // LIFECYCLE ------------------------------------------------------------

private:
  /**
   * Initializes the object.  It should never be used.
   */
  MccQNotFunc () : fn (0) { }
public:

  /**
   * Initializes the object.
   * @param f the query struct that is negated.
   */
  MccQNotFunc (MccQFunc *f) : fn (f) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccQNotFunc (const MccQNotFunc &right) : fn (right.fn->clone ()) { }

  /**
   * Destroys the object.
   */
  virtual ~MccQNotFunc () { delete fn; }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content in the object.
   * @param right the object to copy.
   * @return itself.
   */
  MccQNotFunc& operator= (const MccQNotFunc &right);
  
  // ACCESS ---------------------------------------------------------------

  /**
   * Gets the negated query function.
   * @return the negated query structure.
   */
  MccQFunc* Getfn () { return fn; }

  /**
   * Sets the negated query structure.
   * @param qf the query structure to negate.
   */
  void Setfn (MccQFunc *qf) { delete fn; fn = qf; }
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccQNotFunc* clone () const { return new MccQNotFunc (*this); }

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const
  { os << "! (";  fn->display (os); os << ")"; }

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const;
};



/**
 * @short Struct representing the AST face statement in queries.
 *
 * @author Patrick Gendron <gendrop@iro.umontreal.ca>
 */
struct MccQFaceFunc : public MccQFunc
{
  /**
   * The left query struc of the operation.
   */
  MccQFunc *left;

  /**
   * The right query struc of the operation.
   */
  MccQFunc *right;


  // LIFECYCLE ------------------------------------------------------------

private:
  /**
   * Initializes the object.  It should never be used.
   */
  MccQFaceFunc () { }
public:

  /**
   * Initializes the object.
   * @param l the left query struc of the operation.
   * @param r the right query struc of the operation.
   */
  MccQFaceFunc (MccQFunc *l, MccQFunc *r) : left (l), right (r) { }

  /**
   * Initializes the object with the rights content.
   * @param right_val the object to copy.
   */
  MccQFaceFunc (const MccQFaceFunc &right_val)
    : left (right_val.left->clone ()), right (right_val.right->clone ()) { }
  
  /**
   * Destroys the object.
   */
  virtual ~MccQFaceFunc () { delete left; delete right; }

  // OPERATORS ------------------------------------------------------------
  
  /**
   * Assigns the rights content in the object.
   * @param right_val the object to copy.
   * @return itself.
   */
  MccQFaceFunc& operator= (const MccQFaceFunc &right_val);
  
  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccQFaceFunc* clone () const { return new MccQFaceFunc (*this); }

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const
  { os << "("; left->display (os); os << " / "; right->display (os);
    os << ")"; }

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const;
};
 


/**
 * @short Struct representing the AST and statement in queries.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccQAndFunc : public MccQFunc
{
  /**
   * The left query struc of the operation.
   */
  MccQFunc *left;

  /**
   * The right query struc of the operation.
   */
  MccQFunc *right;


  // LIFECYCLE ------------------------------------------------------------

private:
  /**
   * Initializes the object.  It should never be used.
   */
  MccQAndFunc () { }
public:

  /**
   * Initializes the object.
   * @param l the left query struc of the operation.
   * @param r the right query struc of the operation.
   */
  MccQAndFunc (MccQFunc *l, MccQFunc *r) : left (l), right (r) { }

  /**
   * Initializes the object with the rights content.
   * @param right_val the object to copy.
   */
  MccQAndFunc (const MccQAndFunc &right_val)
    : left (right_val.left->clone ()), right (right_val.right->clone ()) { }
  
  /**
   * Destroys the object.
   */
  virtual ~MccQAndFunc () { delete left; delete right; }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content in the object.
   * @param right_val the object to copy.
   * @return itself.
   */
  MccQAndFunc& operator= (const MccQAndFunc &right_val);
  
  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccQAndFunc* clone () const { return new MccQAndFunc (*this); }

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const
  { left->display (os); os << " && "; right->display (os); }

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const;
};
  
  

/**
 * @short Struct representing the AST or statement in queries.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccQOrFunc : public MccQFunc
{
  /**
   * The left query struc of the operation.
   */
  MccQFunc *left;

  /**
   * The right query struc of the operation.
   */
  MccQFunc *right;


  // LIFECYCLE ------------------------------------------------------------

private:
  /**
   * Initializes the object.  It should never be used.
   */
  MccQOrFunc () { }
public:

  /**
   * Initializes the object.
   * @param l the left query struc of the operation.
   * @param r the right query struc of the operation.
   */
  MccQOrFunc (MccQFunc *l, MccQFunc *r) : left (l), right (r) { }

  /**
   * Initializes the object with the rights content.
   * @param right_val the object to copy.
   */
  MccQOrFunc (const MccQOrFunc &right_val)
    : left (right_val.left->clone ()), right (right_val.right->clone ()) { }
  
  /**
   * Destroys the object.
   */
  virtual ~MccQOrFunc () { delete left; delete right; }

  // OPERATORS ------------------------------------------------------------
  
  /**
   * Assigns the rights content in the object.
   * @param right_val the object to copy.
   * @return itself.
   */
  MccQOrFunc& operator= (const MccQOrFunc &right_val);
  
  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccQOrFunc* clone () const { return new MccQOrFunc (*this); }

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const
  { os << "("; left->display (os); os << " || "; right->display (os);
    os << ")"; }

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const;
};
  


/**
 * @short Struct representing the AST query statements.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccQueryExpr
{
  /**
   * The file name vector.
   */
  vector< char* > *strs;

  /**
   * The query function expression struct.
   */
  MccQFunc *fn;


  // LIFECYCLE ------------------------------------------------------------
  
  /**
   * Initializes the object.
   */
  MccQueryExpr ()
    : strs (new vector< char*> ()), fn (new MccQTrueFunc ()) { }

  /**
   * Initializes the object.
   * @param fv the file name vector.
   * @param f the query function expression struct.
   */
  MccQueryExpr (vector< char* > *fv, MccQFunc *f) : strs (fv), fn (f) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccQueryExpr (const MccQueryExpr &right);
  
  /**
   * Destroys the object.
   */
  ~MccQueryExpr ();

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content in the object.
   * @param right the object to copy.
   * @return itself.
   */
  MccQueryExpr& operator= (const MccQueryExpr &right_val);
  
  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  MccQueryExpr* clone () const { return new MccQueryExpr (*this); }

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  void accept (MccVisitor *visitor);

  /**
   * Puts the file name in the vector.
   * @param f the file name.
   */
  void GenFileName (char *f) { strs->push_back (f); }

  /**
   * Sets the query function expression struct.
   * @param f the query function expression struct.
   */
  void GenFunc (MccQFunc *f) { if (fn) delete fn; fn = f; }
  
  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  void display (ostream &os) const;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  void ppdisplay (ostream &os, int indent = 0) const;
};


/**
 * @short Structures representing the atomset filter expressions.
 *
 * The struct contains a serie of structures describing the test fuctions
 * (NOT, AND, OR, ID) for the atomset filter.  We can display to a ostream the
 * original filter by using the display method.
 *
 * @author Philippe Thibault <thibaup@iro.umontreal.ca>
 */
struct MccASFunc
{
  // LIFECYCLE ------------------------------------------------------------
  
  /**
   * Initializes the object.
   */
  MccASFunc () { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccASFunc (const MccASFunc &right) { }

  /**
   * Destroys the object.
   */
  virtual ~MccASFunc () { }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content in the object.
   * @param right the object to copy.
   * @return itself.
   */
  MccASFunc& operator= (const MccASFunc &right) { return *this; }
  
  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccASFunc* clone () const = 0;

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor) = 0;
  
  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const = 0;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const = 0;
};


/**
 * @short Struct representing the AST identifier in query statements.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccASIdentFunc : public MccASFunc
{
  /**
   * The name of the identifier.
   */
  char *str;

  
  // LIFECYCLE ------------------------------------------------------------

private:
  /**
   * Initializes the object.  It should never be used.
   */
  MccASIdentFunc () { }
public:

  /**
   * Initializes the object.
   * @param s the name of the identifier.
   */
  MccASIdentFunc (char *s) : str (s) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccASIdentFunc (const MccASIdentFunc &right);

  /**
   * Destroys the object.
   */
  virtual ~MccASIdentFunc () { delete[] str; }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content in the object.
   * @param right the object to copy.
   * @return itself.
   */
  MccASIdentFunc& operator= (const MccASIdentFunc &right);
  
  // ACCESS ---------------------------------------------------------------

  /**
   * Sets the name of the identifier.
   * @param s the new name of the identifier.
   */
  void setIdent (char *s) { delete[] str; str = s; }
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccASIdentFunc* clone () const { return new MccASIdentFunc (*this); }

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);
  
  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const { os << str; }

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const { display (os); }
};



/**
 * @short Struct representing the AST not statement in atomset filter.
 *
 * @author Philippe Thibault <thibaup@iro.umontreal.ca>
 */
struct MccASNotFunc : public MccASFunc
{
  /**
   * The query that is negated.
   */
  MccASFunc *fn;

  
  // LIFECYCLE ------------------------------------------------------------

private:
  /**
   * Initializes the object.  It should never be used.
   */
  MccASNotFunc () : fn (0) { }
public:

  /**
   * Initializes the object.
   * @param f the query struct that is negated.
   */
  MccASNotFunc (MccASFunc *f) : fn (f) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccASNotFunc (const MccASNotFunc &right) : fn (right.fn->clone ()) { }

  /**
   * Destroys the object.
   */
  virtual ~MccASNotFunc () { delete fn; }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content in the object.
   * @param right the object to copy.
   * @return itself.
   */
  MccASNotFunc& operator= (const MccASNotFunc &right);
  
  // ACCESS ---------------------------------------------------------------

  /**
   * Gets the negated query function.
   * @return the negated query structure.
   */
  MccASFunc* Getfn () { return fn; }

  /**
   * Sets the negated query structure.
   * @param qf the query structure to negate.
   */
  void setFunc (MccASFunc *qf) { delete fn; fn = qf; }
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccASNotFunc* clone () const { return new MccASNotFunc (*this); }

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const
  { os << "! (";  fn->display (os); os << ")"; }

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const;
};


/**
 * @short Struct representing the AST and statement in atomset filters.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccASAndFunc : public MccASFunc
{
  /**
   * The left query struc of the operation.
   */
  MccASFunc *left;

  /**
   * The right query struc of the operation.
   */
  MccASFunc *right;


  // LIFECYCLE ------------------------------------------------------------

private:
  /**
   * Initializes the object.  It should never be used.
   */
  MccASAndFunc () { }
public:

  /**
   * Initializes the object.
   * @param l the left query struc of the operation.
   * @param r the right query struc of the operation.
   */
  MccASAndFunc (MccASFunc *l, MccASFunc *r) : left (l), right (r) { }

  /**
   * Initializes the object with the rights content.
   * @param right_val the object to copy.
   */
  MccASAndFunc (const MccASAndFunc &right_val)
    : left (right_val.left->clone ()), right (right_val.right->clone ()) { }
  
  /**
   * Destroys the object.
   */
  virtual ~MccASAndFunc () { delete left; delete right; }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content in the object.
   * @param right_val the object to copy.
   * @return itself.
   */
  MccASAndFunc& operator= (const MccASAndFunc &right_val);
  
  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccASAndFunc* clone () const { return new MccASAndFunc (*this); }

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const
  { left->display (os); os << " && "; right->display (os); }

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const;
};
  
  

/**
 * @short Struct representing the AST or statement in atomset filters.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccASOrFunc : public MccASFunc
{
  /**
   * The left query struc of the operation.
   */
  MccASFunc *left;

  /**
   * The right query struc of the operation.
   */
  MccASFunc *right;


  // LIFECYCLE ------------------------------------------------------------

private:
  /**
   * Initializes the object.  It should never be used.
   */
  MccASOrFunc () { }
public:

  /**
   * Initializes the object.
   * @param l the left query struc of the operation.
   * @param r the right query struc of the operation.
   */
  MccASOrFunc (MccASFunc *l, MccASFunc *r) : left (l), right (r) { }

  /**
   * Initializes the object with the rights content.
   * @param right_val the object to copy.
   */
  MccASOrFunc (const MccASOrFunc &right_val)
    : left (right_val.left->clone ()), right (right_val.right->clone ()) { }
  
  /**
   * Destroys the object.
   */
  virtual ~MccASOrFunc () { delete left; delete right; }

  // OPERATORS ------------------------------------------------------------
  
  /**
   * Assigns the rights content in the object.
   * @param right_val the object to copy.
   * @return itself.
   */
  MccASOrFunc& operator= (const MccASOrFunc &right_val);
  
  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccASOrFunc* clone () const { return new MccASOrFunc (*this); }

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const
  { os << "("; left->display (os); os << " || "; right->display (os);
    os << ")"; }

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const;
};
  


/**
 * @short Struct representing the abstract residue view cache.
 *
 * @author Philippe Thibault <philippe.thibault@umontreal.ca>
 */
struct MccResidueViewCache
{

public:

  // LIFECYCLE ------------------------------------------------------------

  /**
   * Initializes the object.
   */
  MccResidueViewCache () { }
  
private:
  
  MccResidueViewCache (const MccResidueViewCache& mrvc) { }
  
public:

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccResidueViewCache* clone () const = 0;
    
  /**
   * Destroys the object.
   */
  virtual ~MccResidueViewCache () { }

  // OPERATORS ------------------------------------------------------------

private:

  MccResidueViewCache& operator= (const MccResidueViewCache& mrvc) { return *this; }

public:

  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor) = 0;

  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const = 0;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const = 0;
};  


/**
 * @short Struct representing the RMSD filtered residue view cache.
 *
 * @author Philippe Thibault <philippe.thibault@umontreal.ca>
 */
struct MccRMSDResidueViewCache : public MccResidueViewCache
{
  /**
   * Minimal RMSD threshold (Angstroms).
   */
  float threshold;

  /**
   * Optional atom filter expression.
   */
  MccASFunc* atomset;

  /**
   * Optional structure alignment flag.
   */
  bool alignflag;

  // LIFECYCLE ------------------------------------------------------------

public:

  /**
   * Initializes the object.  
   */
  MccRMSDResidueViewCache (float th = 0.5, MccASFunc* asfn = 0, bool af = false);
  
private:
  
  MccRMSDResidueViewCache (const MccRMSDResidueViewCache& mrrvc) { }
  MccRMSDResidueViewCache (const MccResidueViewCache& mrvc) { }
  
public:

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccResidueViewCache* clone () const;
    
  /**
   * Destroys the object.
   */
  virtual ~MccRMSDResidueViewCache ();

  // OPERATORS ------------------------------------------------------------

private:

  MccRMSDResidueViewCache& operator= (const MccRMSDResidueViewCache& mrvc) { return *this; }
  MccRMSDResidueViewCache& operator= (const MccResidueViewCache& mrrvc) { return *this; }

public:

  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const;
};  


/**
 * @short Struct representing the TFOD filtered residue view cache.
 *
 * @author Philippe Thibault <philippe.thibault@umontreal.ca>
 */
struct MccTFODResidueViewCache : public MccResidueViewCache
{
  float threshold;

  // LIFECYCLE ------------------------------------------------------------

public:

  /**
   * Initializes the object.  
   */
  MccTFODResidueViewCache (float th = 0.5);
  
private:
  
  MccTFODResidueViewCache (const MccTFODResidueViewCache& mrrvc) { }
  MccTFODResidueViewCache (const MccResidueViewCache& mrvc) { }
  
public:

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccResidueViewCache* clone () const;
    
  /**
   * Destroys the object.
   */
  virtual ~MccTFODResidueViewCache ();

  // OPERATORS ------------------------------------------------------------

private:

  MccTFODResidueViewCache& operator= (const MccTFODResidueViewCache& mrvc) { return *this; }
  MccTFODResidueViewCache& operator= (const MccResidueViewCache& mrrvc) { return *this; }

public:

  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const;
};  


/**
 * @short Struct representing the AST node "adjacency".
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccBaseAdjacencyCstStat : public MccPStruct
{
  /**
   * The struct identifying the FG.
   */
  MccFragGenStruc *fg_struc;
  
  /**
   * The coverage percentage
   */
  int coverage;
  

  // LIFECYCLE ------------------------------------------------------------

private:
  /**
   * Initializes the object.  It should not be used.
   */
  MccBaseAdjacencyCstStat () { }
public:

  /**
   * Initializes the object.
   * @param f the FG struct.
   * @param percent the coverage percentage.
   */
  MccBaseAdjacencyCstStat (MccFragGenStruc *f, int percent)
    : fg_struc (f), coverage (percent) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccBaseAdjacencyCstStat (const MccBaseAdjacencyCstStat &right);

  /**
   * Destroys the structure.
   */
  virtual ~MccBaseAdjacencyCstStat () { delete fg_struc; }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
   * @param right the object to copy.
   * @return itself.
   */
  MccBaseAdjacencyCstStat& operator= (const MccBaseAdjacencyCstStat &right);

  // ACCESS ---------------------------------------------------------------

  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccBaseAdjacencyCstStat* clone () const
  { return new MccBaseAdjacencyCstStat (*this); }

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  // I/O  -----------------------------------------------------------------

  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const;
};



/**
 * @short Struct representing the "angle" statement.
 *
 * This statement generate a constraint between three atoms to fit in a
 * lower and an upper bound.  This struct owns a local struct _AngleStruc
 * that contains the informations on the constraints.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccAngleCstStat : public MccPStruct
{
  /**
   * @short Sub-struct containing an angle constraint description.
   *
   * @author Martin Larose <larosem@iro.umontreal.ca>
   */
  struct _AngleStruc
  {
    /**
     * The first residue name.
     */
    MccResidueName *res1;

    /**
     * The atom name of the first residue.
     */
    char *at1;

    /**
     * The second residue name.
     */
    MccResidueName *res2;

    /**
     * The atom name of the second residue.
     */
    char *at2;

    /**
     * The third residue name.
     */
    MccResidueName *res3;

    /**
     * The atom name of the third residue.
     */
    char *at3;

    /**
     * The target angle value (in degrees).
     */
    float angle;

    /**
     * The absolute error value (in degrees).
     */
    float error;

    
    // LIFECYCLE ------------------------------------------------------------

  private:
    /**
     * Initializes the object.  It should never be used.
     */
    _AngleStruc () { }
  public:

    /**
     * Initializes the object.
     * @param r1 the first residue name.
     * @param a1 the atom name of the first residue.
     * @param r2 the second residue name.
     * @param a2 the atom name of the second residue.
     * @param r3 the third residue name.
     * @param a3 the atom name of the third residue.
     * @param mn the minimum angle value (in degrees).
     * @param mx the maximum angle value (in degrees).
     */
    _AngleStruc (MccResidueName *r1, char *a1, MccResidueName *r2, char *a2,
		 MccResidueName *r3, char *a3, float a, float e)
      : res1 (r1), at1 (a1), res2 (r2), at2 (a2), res3 (r3), at3 (a3),
	angle (a), error (e)
    { }

    /**
     * Initializes the object with the rights content.
     * @param right the object to copy.
     */
    _AngleStruc (const _AngleStruc &right);

    /**
     * Destroys the residue names and atom names.
     */
    ~_AngleStruc ();

    // OPERATORS ------------------------------------------------------------

    /**
     * Assigns the rights content into the object.
     * @param right the object to copy.
     * @return itself.
     */
    _AngleStruc& operator= (const _AngleStruc &right);
    
    // ACCESS ---------------------------------------------------------------

    // METHODS --------------------------------------------------------------

    /**
     * Replicates the object.
     * @return a copy of the current object.
     */
    _AngleStruc* clone () const { return new _AngleStruc (*this); }

    /**
     * Accepts the visitor and calls it on itself.
     * @param visitor the visitor.
     */
    void accept (MccVisitor *visitor);
    
    // I/O  -----------------------------------------------------------------

    /**
     * Displays the structure.
     * @param os the output stream where the message is displayed.
     */
    void display (ostream &os) const;

    /**
     * Displays the script in human readable form.
     * @param os the output stream used.
     * @param ident the identation level.
     */
    void ppdisplay (ostream &os, int indent = 0) const;
  };

  /**
   * The vector of _AngleStruc.
   */
  vector< _AngleStruc* > *strucs;

  
  // LIFECYCLE ------------------------------------------------------------

  /**
   * Initializes the object.
   */
  MccAngleCstStat () : strucs (new vector< _AngleStruc* > ()) { }

  /**
   * Initializes the object.
   * @param asv the angle sub-structure vector.
   */
  MccAngleCstStat (vector< _AngleStruc* > *asv) : strucs (asv) { }
  
  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccAngleCstStat (const MccAngleCstStat &right);
  
  /**
   * Destroys the vector of sub-structures.
   */
  virtual ~MccAngleCstStat ();

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
     * @param right the object to copy.
     * @return itself.
     */
  MccAngleCstStat& operator= (const MccAngleCstStat &right);

  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccAngleCstStat* clone () const
  { return new MccAngleCstStat (*this); }

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  /**
   * Generates a new sub-structure and puts it in the vector.
   * @param r1 the first residue name.
   * @param a1 the atom name of the first residue.
   * @param r2 the second residue name.
   * @param a2 the atom name of the second residue.
   * @param r3 the third residue name.
   * @param a3 the atom name of the third residue.
   * @param mn the minimum angle value (in degrees).
   * @param mx the maximum angle value (in degrees).
   */
  void GenAngleStruc (MccResidueName *r1, char *a1, MccResidueName *r2,
		      char *a2, MccResidueName *r3, char *a3, float mn,
		      float mx)
  {
    strucs->push_back (new _AngleStruc (r1, a1, r2, a2, r3, a3, mn, mx)); 
  }

  // I/O ------------------------------------------------------------------

  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const;
};



/**
 * @short Struct representing the AST node of the assignment "=" statement.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccAssignStat : public MccPStruct
{
  /**
   * The variable receiving the FG.
   */
  char *ident;

  /**
   * The expression of the FG.
   */
  MccFGExp *expr;


  // LIFECYCLE ------------------------------------------------------------

private:
  /**
   * Initializes the object.  It should never be used.
   */
  MccAssignStat () { }
public:

  /**
   * Initializes the object.
   * @param s the variable receiving the FG.
   * @param exp the expression of the FG.
   */
  MccAssignStat (char *s, MccFGExp *exp) : ident (s), expr (exp) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccAssignStat (const MccAssignStat &right);
  
  /**
   * Destroys the variable and the expression.
   */
  virtual ~MccAssignStat () { delete [] ident; delete expr; }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
   * @param right the object to copy.
   * @return itself.
   */
  MccAssignStat& operator= (const MccAssignStat &right);

  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccAssignStat* clone () const { return new MccAssignStat (*this); }

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  // I/O ------------------------------------------------------------------

  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const;
};



/**
 * @short Struct representing the AST node of the "backtrack" expression.
 *
 * The struct contains a local hierarchy of statements: _GenBTStruc (the
 * parent), _FGStruc (for the fraggen reference), _BTStruc (for the residue
 * enumeration) and _PlaceStruc (for the "place" statement).
 *
 * Usage: first generate a MccBacktrackExpr object and then you can fill its
 * _GenBTStruc* vector using the GenFGStruc, GenBTStruc and GenPlaceStruc
 * methods.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccBacktrackExpr : public MccFGExp
{
  /**
   * @short Parent struct for the MccBacktrackExpr sub-structures.
   *
   * @author Martin Larose <larosem@iro.umontreal.ca>
   */
  struct _GenBTStruc
  {
    /**
     * The name of the reference residue.
     */
    MccResidueName *ref;

    /**
     * The name of the remote residue.
     */
    MccResidueName *res;

    /**
     * The FG.  It is optionnal.
     */
    MccFragGenStruc *fg_struc;

    /**
     * The vector of residu names.
     */
    vector< MccResidueName* > *res_v;

    
    // LIFECYCLE ------------------------------------------------------------

  protected:
    /**
     * Initializes the object.  It should never be used.
     */
    _GenBTStruc () : ref (0), res (0), fg_struc (0), res_v (0) { }
  public:

    /**
     * Initializes the object.
     * @param rf the name of the reference residue.
     * @param rs the name of the remote residue.
     * @param f the FG structure.
     * @param rv the vector of residue names.
     */
    _GenBTStruc (MccResidueName *rf, MccResidueName *rs,
		 MccFragGenStruc *f, vector< MccResidueName* > *rv)
      : ref (rf), res (rs), fg_struc (f), res_v (rv) { }

    /**
     * Initializes the object with the rights content.
     * @param right the object to copy.
     */
    _GenBTStruc (const _GenBTStruc &right);

    /**
     * Destroys the object.  The destruction is controlled by the children
     * of the structure.
     */
    virtual ~_GenBTStruc () { }

    // OPERATORS ------------------------------------------------------------

    /**
     * Assigns the rights content into the object.
     * @param right the object to copy.
     * @return itself.
     */
    _GenBTStruc& operator= (const _GenBTStruc &right);

    // ACCESS ---------------------------------------------------------------
    
    // METHODS --------------------------------------------------------------

    /**
     * Replicates the object.  The replication is controlled by the children.
     * @return a copy of the current object.
     */
    virtual _GenBTStruc* clone () const = 0;
    
    /**
     * Accepts the visitor and calls it on itself.
     * @param visitor the visitor.
     */
    virtual void accept (MccVisitor *visitor) = 0;

    // I/O ------------------------------------------------------------------

    /**
     * Displays the structure.  Nothing to do here, every output is
     * controlled by the children structures.
     * @param os the output stream where the message is displayed.
     */
    virtual void display (ostream &os) const = 0;

    /**
     * Displays the script in human readable form.
     * @param os the output stream used.
     * @param ident the identation level.
     */
    virtual void ppdisplay (ostream &os, int indent = 0) const = 0;
  };
  

  /**
   * @short Struct containing the placement of a FG in the backtrack.
   *
   * @author Martin Larose <larosem@iro.umontreal.ca>
   */
  struct _FGStruc : public _GenBTStruc
  {
    // LIFECYCLE ------------------------------------------------------------

  private:
    /**
     * Initializes the object.  It should never be used.
     */
    _FGStruc () : _GenBTStruc () { }
  public:

    /**
     * Initializes the object.
     * @param f the FG structure.
     */
    _FGStruc (MccFragGenStruc *f) : _GenBTStruc (0, 0, f, 0) { }

    /**
     * Initializes the object with the rights content.
     * @param right the object to copy.
     */
    _FGStruc (const _FGStruc &right) : _GenBTStruc (right) { }
    
    /**
     * Destroys the object.
     */
    virtual ~_FGStruc () { delete fg_struc; }

    // OPERATORS ------------------------------------------------------------

    /**
     * Assigns the rights content into the object.
     * @param right the object to copy.
     * @return itself.
     */
    _FGStruc& operator= (const _FGStruc &right);
    
    // ACCESS ---------------------------------------------------------------
    
    // METHODS --------------------------------------------------------------

    /**
     * Replicates the object.
     * @return a copy of the current object.
     */
    virtual _FGStruc* clone () const { return new _FGStruc (*this); }
    
    /**
     * Accepts the visitor and calls it on itself.
     * @param visitor the visitor.
     */
    virtual void accept (MccVisitor *visitor);

    // I/O ------------------------------------------------------------------
    
    /**
     * Displays the structure.
     * @param os the output stream where the message is displayed.
     */
    virtual void display (ostream &os) const { fg_struc->display (os); }

    /**
     * Displays the script in human readable form.
     * @param os the output stream used.
     * @param ident the identation level.
     */
    virtual void ppdisplay (ostream &os, int indent = 0) const;
  };
  

  /**
   * @short Struct containing the placement of several residues.
   *
   * @author Martin Larose <larosem@iro.umontreal.ca>
   */
  struct _BTStruc : public _GenBTStruc
  {
    // LIFECYCLE ------------------------------------------------------------

  private:
    /**
     * Initializes the object. It should never be used.
     */
    _BTStruc () : _GenBTStruc () { }
  public:

    /**
     * Initializes the object.
     * @param rf the name of the reference residue.
     * @param rv the residue name vector.
     */
    _BTStruc (MccResidueName *rf, vector< MccResidueName* > *rv)
      : _GenBTStruc (rf, 0, 0, rv) { }

    /**
     * Initializes the object with the rights content.
     * @param right the object to copy.
     */
    _BTStruc (const _BTStruc &right) : _GenBTStruc (right) { }
    
    /**
     * Destroys the object.
     */
    virtual ~_BTStruc ();

    // OPERATORS ------------------------------------------------------------

    /**
     * Assigns the rights content into the object.
     * @param right the object to copy.
     * @return itself.
     */
    _BTStruc& operator= (const _BTStruc &right);
    
    // ACCESS ---------------------------------------------------------------
    
    // METHODS --------------------------------------------------------------

    /**
     * Replicates the object.
     * @return a copy of the current object.
     */
    virtual _BTStruc* clone () const { return new _BTStruc (*this); }
    
    /**
     * Accepts the visitor and calls it on itself.
     * @param visitor the visitor.
     */
    virtual void accept (MccVisitor *visitor);

    // I/O ------------------------------------------------------------------
    
    /**
     * Displays the structure.
     * @param os the output stream where the message is displayed.
     */
    virtual void display (ostream &os) const;

    /**
     * Displays the script in human readable form.
     * @param os the output stream used.
     * @param ident the identation level.
     */
    virtual void ppdisplay (ostream &os, int indent = 0) const;
  };
  

  /**
   * @short Struct containing the placement of a FG relative to some residues.
   *
   * @author Martin Larose <larosem@iro.umontreal.ca>
   */
  struct _PlaceStruc : public _GenBTStruc
  {
    // LIFECYCLE ------------------------------------------------------------

  private:
    /**
     * Initializes the object. It should never be used.
     */
    _PlaceStruc () : _GenBTStruc () { }
  public:

    /**
     * Initializes the object.
     * @param rf the name of the reference residue.
     * @param fs the name of the remote residue.
     * @param fg the FG structure.
     */
    _PlaceStruc (MccResidueName *rf, MccResidueName *rs, MccFragGenStruc *fg)
      : _GenBTStruc (rf, rs, fg, 0) { }

    /**
     * Initializes the object with the rights content.
     * @param right the object to copy.
     */
    _PlaceStruc (const _PlaceStruc &right) : _GenBTStruc (right) { }
    
    /**
     * Destroys the object.
     */
    virtual ~_PlaceStruc () { delete ref; delete res; delete fg_struc; }

    // OPERATORS ------------------------------------------------------------

    /**
     * Assigns the rights content into the object.
     * @param right the object to copy.
     * @return itself.
     */
    _PlaceStruc& operator= (const _PlaceStruc &right);
    
    // ACCESS ---------------------------------------------------------------
    
    // METHODS --------------------------------------------------------------

    /**
     * Replicates the object.
     * @return a copy of the current object.
     */
    virtual _PlaceStruc* clone () const { return new _PlaceStruc (*this); }
    
    /**
     * Accepts the visitor and calls it on itself.
     * @param visitor the visitor.
     */
    virtual void accept (MccVisitor *visitor);
    
    // I/O ------------------------------------------------------------------
    
    /**
     * Displays the structure.
     * @param os the output stream where the message is displayed.
     */
    virtual void display (ostream &os) const;

    /**
     * Displays the script in human readable form.
     * @param os the output stream used.
     * @param ident the identation level.
     */
    virtual void ppdisplay (ostream &os, int indent = 0) const;
  };

  /**
   * @short Struct containing the placement of a FG by merging shared residues.
   *
   * @author Philippe Thibault <thibaup@iro.umontreal.ca>
   */
  struct _MergeStruc : public _GenBTStruc
  {
    /**
     * The rms bound.
     */
    float rmsBound;
    
    /**
     * The atomset filter expression.
     */
    MccASFunc* atomset;

    // LIFECYCLE ------------------------------------------------------------

  private:
    /**
     * Initializes the object. It should never be used.
     */
    _MergeStruc () : _GenBTStruc () { }
  public:

    /**
     * Initializes the object.
     * @param fg the FG structure.
     * @param rb the rms bound.
     * @param asfn atomset filter expression.
     */
    _MergeStruc (MccFragGenStruc *fg, float rb, MccASFunc* asfn)
      : _GenBTStruc (0, 0, fg, 0),
	rmsBound (rb), 
	atomset (asfn) 
    { }

    /**
     * Initializes the object with the rights content.
     * @param right the object to copy.
     */
    _MergeStruc (const _MergeStruc &right) 
      : _GenBTStruc (right),
	rmsBound (right.rmsBound),
	atomset (0 == right.atomset ? right.atomset : right.atomset->clone ())
    { }
    
    /**
     * Destroys the object.
     */
    virtual ~_MergeStruc () 
    { 
      delete fg_struc; 
      if (atomset) delete atomset;
    }

    // OPERATORS ------------------------------------------------------------

    /**
     * Assigns the rights content into the object.
     * @param right the object to copy.
     * @return itself.
     */
    _MergeStruc& operator= (const _MergeStruc &right);
    
    // ACCESS ---------------------------------------------------------------
    
    // METHODS --------------------------------------------------------------

    /**
     * Replicates the object.
     * @return a copy of the current object.
     */
    virtual _MergeStruc* clone () const { return new _MergeStruc (*this); }
    
    /**
     * Accepts the visitor and calls it on itself.
     * @param visitor the visitor.
     */
    virtual void accept (MccVisitor *visitor);
    
    // I/O ------------------------------------------------------------------
    
    /**
     * Displays the structure.
     * @param os the output stream where the message is displayed.
     */
    virtual void display (ostream &os) const;

    /**
     * Displays the script in human readable form.
     * @param os the output stream used.
     * @param ident the identation level.
     */
    virtual void ppdisplay (ostream &os, int indent = 0) const;
  };


  /**
   * The vector containing the different backtrack sub-structures.
   */
  vector< _GenBTStruc* > *strucs;

  
  // LIFECYCLE ------------------------------------------------------------

  /**
   * Initializes the object.
   */
  MccBacktrackExpr () : strucs (new vector< _GenBTStruc* > ()) { }

  /**
   * Initializes the object.
   * @param s the vector containing the different backtrack sub-structures.
   */
  MccBacktrackExpr (vector< _GenBTStruc* > *s) : strucs (s) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccBacktrackExpr (const MccBacktrackExpr &right);
  
  /**
   * Destroys the object.  It clears the sub-structures contained in the
   * vector.
   */
  virtual ~MccBacktrackExpr ();

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
   * @param right the object to copy.
   * @return itself.
   */
  MccBacktrackExpr& operator= (const MccBacktrackExpr &right);
  
  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccBacktrackExpr* clone () const
  { return new MccBacktrackExpr (*this); }
    
  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  /**
   * Generates a new FG sub-structure and puts it in the vector.
   * @param f the FG structure.
   */
  void GenFGStruc (MccFragGenStruc *f)
  { strucs->push_back (new _FGStruc (f)); }

  /**
   * Generates a new BT sub-structure and puts it in the vector.
   * @param rf the name of the reference residue.
   * @param rv the residue name vector.
   */
  void GenBTStruc (MccResidueName *rf, vector< MccResidueName* > *rv)
  { strucs->push_back (new _BTStruc (rf, rv)); }

  /**
   * Generates a new Place sub-structure and puts it in the vector.
   * @param rf the name of the reference residue.
   * @param rs the name of the remote residue.
   * @param fg the FG structure.
   */
  void GenPlaceStruc (MccResidueName *rf, MccResidueName *rs,
		      MccFragGenStruc *fg)
  { strucs->push_back (new _PlaceStruc (rf, rs, fg)); }

  /**
   * Adds the vector of backtrack structures in the vector.
   * @param bts the vector of backtrack structures.
   */
  void AddBTStrucs (vector< _GenBTStruc* > *bts)
  { strucs->insert (strucs->end (), bts->begin (), bts->end ()); delete bts; }

  // I/O ------------------------------------------------------------------
    
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const;
};


/**
 * @short Struct representing the AST node of the "cycle" expression.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccCycleExpr : public MccFGExp
{
  /**
   * The vector of residu names.
   */
  vector< MccResidueName * > *resv;
  
  // LIFECYCLE ------------------------------------------------------------

  /**
   * Initializes the object.
   */
  MccCycleExpr () : resv (new vector< MccResidueName * > ()) { }

  /**
   * Initializes the object.
   * @param rv the residues used in the cycle fg.
   */
  MccCycleExpr (vector< MccResidueName * > *rv) : resv (rv) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccCycleExpr (const MccCycleExpr &right);
  
  /**
   * Destroys the object.  It clears the sub-structures contained in the
   * vector.
   */
  virtual ~MccCycleExpr ();

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
   * @param right the object to copy.
   * @return itself.
   */
  MccCycleExpr& operator= (const MccCycleExpr &right);
  
  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccCycleExpr* clone () const
  { return new MccCycleExpr (*this); }
    
  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  // I/O ------------------------------------------------------------------
    
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const;
};



/**
 * @short Struct representing the AST node for the "cache" expression.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccCacheExpr : public MccFGExp
{
  /**
   * The FG structure to cache.
   */
  MccFragGenStruc fg;

  /**
   * The model cache.
   */
  MccResidueViewCache* cache;
  
protected:
  
  // LIFECYCLE ------------------------------------------------------------

  /**
   * Initializes the object.  It should never be used.
   */
  MccCacheExpr () { }

public:

  /**
   * Initializes the object.
   * @param f the FG structure to cache.
   * @param c the model filter structure.
   */
  MccCacheExpr (const MccFragGenStruc& mfgs, MccResidueViewCache* mrvc = 0)
    : fg (mfgs),
      cache (mrvc)
  {
  }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccCacheExpr (const MccCacheExpr &right);
  
  /**
   * Destroys the object.
   */
  virtual ~MccCacheExpr ();

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
   * @param right the object to copy.
   * @return itself.
   */
  MccCacheExpr& operator= (const MccCacheExpr &right);
  
  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccCacheExpr* clone () const { return new MccCacheExpr (*this); }
    
  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  // I/O ------------------------------------------------------------------
    
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const;
};



/**
 * @short Struct representing the "res_clash" AST node.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccClashCstStat : public MccPStruct
{
  /**
   * The FG struct on which the constraint is verified.
   */
  MccFragGenStruc *fg_struc;

  /**
   * The variable specifying if we use Van Der Waals or fixed distance.
   */
  bool VDWDist;

  /**
   * The distance value.
   */
  float distFac;

  /**
   * The atomset filter expression.
   */
  MccASFunc* atomset;
    
  // LIFECYCLE ------------------------------------------------------------

private:
  /**
   * Initializes the object.  It should never be used.
   */
  MccClashCstStat () { }
public:

  /**
   * Initializes the object.
   * @param fg FG struct on which the constraint is verified.
   * @param isVDW flag specifying if we use Van Der Waals or fixed distance.
   * @param cut distance value.
   * @param asfn atomset filter expression.
   */
  MccClashCstStat (MccFragGenStruc *fg, bool isVDW, float cut, MccASFunc* asfn)
    : fg_struc (fg),
      VDWDist (isVDW),
      distFac (cut),
      atomset (asfn)
  {

  }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccClashCstStat (const MccClashCstStat &right);
  
  /**
   * Destroys the object.
   */
  virtual ~MccClashCstStat ();

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
   * @param right the object to copy.
   * @return itself.
   */
  MccClashCstStat& operator= (const MccClashCstStat &right);
  
  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccClashCstStat* clone () const { return new MccClashCstStat (*this); }
    
  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  // I/O ------------------------------------------------------------------
    
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const;
};


/**
 * @short Struct representing the "cycle" constraint statement.
 *
 */
struct MccCycleCstStat : public MccPStruct
{

  struct _CycleStruc
  {
    /**
     * The last residue name of teh cycle.
     */
    MccResidueName *last;

    /**
     * The first residue name of the cycle.
     */
    MccResidueName *first;

    /**
     * The distance threshold
     */
    float threshold;
    
  protected:
    
    // LIFECYCLE ------------------------------------------------------------

    /**
     * Initializes the object.  It should never be used.
     */
    _CycleStruc () { }

  public:

    /**
     * Initializes the object.
     */
    _CycleStruc (MccResidueName *lr, MccResidueName *fr, float th)
      : last (lr), first (fr), threshold (th)
    { }

    /**
     * Initializes the object with the rights content.
     * @param right the object to copy.
     */
    _CycleStruc (const _CycleStruc &right);

    /**
     * Replicates the object.
     * @return a copy of the current object.
     */
    _CycleStruc* clone () const { return new _CycleStruc (*this); }

    /**
     * Destroys the residue names and atom names.
     */
    ~_CycleStruc ();

    // OPERATORS ------------------------------------------------------------

    /**
     * Assigns the rights content into the object.
     * @param right the object to copy.
     * @return itself.
     */
    _CycleStruc& operator= (const _CycleStruc &right);
    
    // ACCESS ---------------------------------------------------------------

    // METHODS --------------------------------------------------------------

    /**
     * Accepts the visitor and calls it on itself.
     * @param visitor the visitor.
     */
    void accept (MccVisitor *visitor);
    
    // I/O  -----------------------------------------------------------------

    /**
     * Displays the structure.
     * @param os the output stream where the message is displayed.
     */
    void display (ostream &os) const;

    /**
     * Displays the script in human readable form.
     * @param os the output stream used.
     * @param ident the identation level.
     */
    void ppdisplay (ostream &os, int indent = 0) const;
  };

  /**
   * The vector of _CycleStruc.
   */
  vector< _CycleStruc* > *strucs;

  
  // LIFECYCLE ------------------------------------------------------------

  /**
   * Initializes the object.
   */
  MccCycleCstStat () : strucs (new vector< _CycleStruc* > ()) { }

  /**
   * Initializes the object.
   * @param asv the relation sub-structure vector.
   */
  MccCycleCstStat (vector< _CycleStruc* > *asv) : strucs (asv) { }
  
  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccCycleCstStat (const MccCycleCstStat &right);
  
  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccCycleCstStat* clone () const
  { return new MccCycleCstStat (*this); }

  /**
   * Destroys the vector of sub-structures.
   */
  virtual ~MccCycleCstStat ();

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
   * @param right the object to copy.
   * @return itself.
   */
  MccCycleCstStat& operator= (const MccCycleCstStat &right);

  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  // I/O ------------------------------------------------------------------

  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const;
};


/**
 * @short Struct representing the AST node "cycle" statement.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccMultimerCstStat : public MccPStruct
{
  /**
   * @short Sub-struct containing an cycle constraint description.
   *
   * @author Martin Larose <larosem@iro.umontreal.ca>
   */
  struct _MultimerStruc
  {
    /**
     * The name of the first residue.
     */
    MccResidueName *res1;
    
    /**
     * The name of the second residue.
     */
    MccResidueName *res2;
    
    /**
     * The distance value.
     */
    float dist;
    
    /**
     * The repeat value.
     */
    int nb;


    // LIFECYCLE ------------------------------------------------------------

  private:
    /**
     * Initializes the object.  It should never be used.
     */
    _MultimerStruc () { }
  public:

    /**
     * Initializes the object.
     * @param r1 the name of the first residue.
     * @param r2 the name of the second residue.
     * @param d the distance value.
     * @param n the repeat value.
     */
    _MultimerStruc (MccResidueName *r1, MccResidueName *r2, float d, int n)
      : res1 (r1), res2 (r2), dist (d), nb (n) { }

    /**
     * Initializes the object with the rights content.
     * @param right the object to copy.
     */
    _MultimerStruc (const _MultimerStruc &right);
    
    /**
     * Destroys the object.
     */
    ~_MultimerStruc () { delete res1; delete res2; }

    // OPERATORS ------------------------------------------------------------

    /**
     * Assigns the rights content into the object.
     * @param right the object to copy.
     * @return itself.
     */
    _MultimerStruc& operator= (const _MultimerStruc &right);
    
    // ACCESS ---------------------------------------------------------------
    
    // METHODS --------------------------------------------------------------

    /**
     * Replicates the object.
     * @return a copy of the current object.
     */
    _MultimerStruc* clone () const { return new _MultimerStruc (*this); }
    
    /**
     * Accepts the visitor and calls it on itself.
     * @param visitor the visitor.
     */
    void accept (MccVisitor *visitor);

    // I/O ------------------------------------------------------------------
    
    /**
     * Displays the structure.
     * @param os the output stream where the message is displayed.
     */
    void display (ostream &os) const;

    /**
     * Displays the script in human readable form.
     * @param os the output stream used.
     * @param ident the identation level.
     */
    void ppdisplay (ostream &os, int indent = 0) const;
  };

  /**
   * The vector containing the cycle sub-structures.
   */
  vector< _MultimerStruc* > *strucs;

  
  // LIFECYCLE ------------------------------------------------------------

  /**
   * Initializes the object.
   */
  MccMultimerCstStat () : strucs (new vector< _MultimerStruc* > ()) { }
  
  /**
   * Initializes the object.
   * @param csv the cycle sub-structure vector.
   */
  MccMultimerCstStat (vector< _MultimerStruc* > *csv) : strucs (csv) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
   MccMultimerCstStat (const MccMultimerCstStat &right);
  
  /**
   * Destroys the object.
   */
  virtual ~MccMultimerCstStat ();

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
   * @param right the object to copy.
   * @return itself.
   */
  MccMultimerCstStat& operator= (const MccMultimerCstStat &right);
    
  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  MccMultimerCstStat* clone () const { return new MccMultimerCstStat (*this); }
    
  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  /**
   * Generates a new cycle sub-structure and puts it in the vector.
   * @param r1 the name of the first residue.
   * @param r2 the name of the second residue.
   * @param d the cutoff value.
   * @param n the repeat value.
   */
  void GenMultimerStruc (MccResidueName *r1, MccResidueName *r2, float d, int n)
  { strucs->push_back (new _MultimerStruc (r1, r2, d, n)); }

  // I/O ------------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const;
};


/**
 * @short Struct representing the AST node "db_display" statement.
 *
 * @author Philippe Thibault <thibaup@iro.umontreal.ca>
 */
struct MccDBDisplayStat : public MccPStruct
{

  // LIFECYCLE ------------------------------------------------------------

  /**
   * Initializes the object.
   */
  MccDBDisplayStat () { }

  /**
   * Initializes the object with the right struct.
   * @param right the struct to copy.
   */
  MccDBDisplayStat (const MccDBDisplayStat &right) { }

  /**
   * Destroys the object.
   */
  virtual ~MccDBDisplayStat () {  }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
   * @param right the object to copy.
   * @return itself.
   */
  MccDBDisplayStat& operator= (const MccDBDisplayStat &right) { return *this; }

  // ACCESS ---------------------------------------------------------------

  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  MccDBDisplayStat* clone () const { return new MccDBDisplayStat (*this); }
    
  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  // I/O  -----------------------------------------------------------------

  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const;
};


/**
 * @short Struct representing the AST node "display_fg" statement.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccDisplayFGStat : public MccPStruct
{
  /**
   * The FG structure to display.
   */
  MccFragGenStruc *fg_struc;

  
  // LIFECYCLE ------------------------------------------------------------

private:
  /**
   * Initializes the object.  It should never be used.
   */
  MccDisplayFGStat () { }
public:

  /**
   * Initializes the object.
   * @param f the FG structure to display.
   */
  MccDisplayFGStat (MccFragGenStruc *f) : fg_struc (f) { }

  /**
   * Initializes the object with the right struct.
   * @param right the struct to copy.
   */
  MccDisplayFGStat (const MccDisplayFGStat &right);

  /**
   * Destroys the object.
   */
  virtual ~MccDisplayFGStat () { delete fg_struc; }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
   * @param right the object to copy.
   * @return itself.
   */
  MccDisplayFGStat& operator= (const MccDisplayFGStat &right);

  // ACCESS ---------------------------------------------------------------

  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  MccDisplayFGStat* clone () const { return new MccDisplayFGStat (*this); }
    
  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  // I/O  -----------------------------------------------------------------

  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const;
};


/**
 * @short Struct representing the AST node "db_filter" statement.
 *
 * @author Philippe Thibault <philippe.thibault@umontreal.ca>
 */
struct MccDBFilterStat : public MccPStruct
{

  /**
   * The identity threshold.
   */
  float threshold;

  bool threaded;
  
  // LIFECYCLE ------------------------------------------------------------

public:

  /**
   * Initializes the object.
   * @param f the FG structure to display.
   */
  MccDBFilterStat (float th = -1.0, bool wth = false) 
    : threshold (th), threaded (wth) { }

  /**
   * Initializes the object with the right struct.
   * @param right the struct to copy.
   */
  MccDBFilterStat (const MccDBFilterStat &right) 
    : threshold (right.threshold), threaded (right.threaded) { }

  /**
   * Destroys the object.
   */
  virtual ~MccDBFilterStat () { }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
   * @param right the object to copy.
   * @return itself.
   */
  MccDBFilterStat& operator= (const MccDBFilterStat &right);

  // ACCESS ---------------------------------------------------------------

  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  MccDBFilterStat* clone () const { return new MccDBFilterStat (*this); }
    
  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  // I/O  -----------------------------------------------------------------

  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const;
};


/**
 * @short Struct representing the "distance" statement.
 *
 * This statement generate a constraint between two atoms with a lower and
 * an upper bound.  This struct owns a local struct _DistStruc that contains
 * the informations on the constraints.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccDistCstStat : public MccPStruct
{
  /**
   * @short Sub-struct containing a distance constraint description.
   *
   * @author Martin Larose <larosem@iro.umontreal.ca>
   */
  struct _DistStruc
  {
    /**
     * The struct of the first residue.
     */
    MccResidueName *res1;

    /**
     * The name of the atom of the first residue.
     */
    char *at1;

    /**
     * The struct of the second residue.
     */
    MccResidueName *res2;

    /**
     * The name of the atom of the second residue.
     */
    char *at2;

    /**
     * The minimum distance.
     */
    float dist_min;

    /**
     * The maximum distance.
     */
    float dist_max;

    
    // LIFECYCLE ------------------------------------------------------------

  private:
    /**
     * Initializes the object.  It should never be used.
     */
    _DistStruc () { }
  public:
    
    /**
     * Initializes the object.
     * @param r1 the struct of the first residue.
     * @param a1 the name of the atom of the first residue.
     * @param r2 the struct of the second residue.
     * @param a2 the name of the atom of the second residue.
     * @param mn the minimum distance.
     * @param mx the maximum distance.
     */
    _DistStruc (MccResidueName *r1, char *a1, MccResidueName *r2, char *a2,
		float mn, float mx)
      : res1 (r1), at1 (a1), res2 (r2), at2 (a2), dist_min (mn), dist_max (mx)
    { }

    /**
     * Initializes the object with the rights content.
     * @param right the object to copy.
     */
    _DistStruc (const _DistStruc &right);

    /**
     * Destroys the object.
     */
    ~_DistStruc () { delete res1; delete[] at1; delete res2; delete[] at2; }

    // OPERATORS ------------------------------------------------------------

    /**
     * Assigns the rights content into the object.
     * @param right the object to copy.
     * @return itself.
     */
    _DistStruc& operator= (const _DistStruc &right);
    
    // ACCESS ---------------------------------------------------------------

    // METHODS --------------------------------------------------------------
    
    /**
     * Replicates the object.
     * @return a copy of the current object.
     */
    _DistStruc* clone () const { return new _DistStruc (*this); }
    
    /**
     * Accepts the visitor and calls it on itself.
     * @param visitor the visitor.
     */
    void accept (MccVisitor *visitor);

    // I/O  -----------------------------------------------------------------
    
    /**
     * Displays the structure.
     * @param os the output stream where the message is displayed.
     */
    void display (ostream &os) const;

    /**
     * Displays the script in human readable form.
     * @param os the output stream used.
     * @param ident the identation level.
     */
    void ppdisplay (ostream &os, int indent = 0) const;
  };

  /**
   * The vector of distance sub-structures.
   */
  vector< _DistStruc* > *strucs;


  // LIFECYCLE ------------------------------------------------------------

  /**
   * Initializes the object.
   */
  MccDistCstStat () : strucs (new vector< _DistStruc* > ()) { }

  /**
   * Initializes the object.
   * @param dsv the distance sub-structure vector.
   */
  MccDistCstStat (vector< _DistStruc* > *dsv) : strucs (dsv) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccDistCstStat (const MccDistCstStat &right);

  /**
   * Destroys the object.  Clears the vector of sub-structures.
   */
  virtual ~MccDistCstStat ();

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
   * @param right the object to copy.
   * @return itself.
   */
  MccDistCstStat& operator= (const MccDistCstStat &right);
    
  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccDistCstStat* clone () const { return new MccDistCstStat (*this); }
    
  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  /**
   * Generates a new distance sub-structure and puts it in the vector.
   * @param r1 the struct of the first residue.
   * @param a1 the name of the atom of the first residue.
   * @param r2 the struct of the second residue.
   * @param a2 the name of the atom of the second residue.
   * @param mn the minimum distance.
   * @param mx the maximum distance.
   */
  void GenDistStruc (MccResidueName *r1, char *a1, MccResidueName *r2, char *a2,
		     float mn, float mx)
  { strucs->push_back (new _DistStruc (r1, a1, r2, a2, mn, mx)); }

  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const;
};


/**
 * @short Struct representing the AST statement "env".
 *
 * Displays mcsym's current environment content.
 *
 * @author Philippe Thibault <philippe.thibault@umontreal.ca>
 */
struct MccEnvStat : public MccPStruct
{
  // LIFECYCLE ------------------------------------------------------------
    
  /**
   * Initializes the object.
   */
  MccEnvStat () { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccEnvStat (const MccEnvStat &right) { }

  /**
   * Destroys the object.
   */
  virtual ~MccEnvStat () { }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
   * @param right the object to copy.
   * @return itself.
   */
  MccEnvStat& operator= (const MccEnvStat &right)
  { return *this; }
  
  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccEnvStat* clone () const { return new MccEnvStat (*this); }
    
  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const { os << "env"; }

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const;
};


/**
 * @short Abstract class for output models.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccOutputMode
{
  
public:
  
  // LIFECYCLE ------------------------------------------------------------

  /**
   * Initializes the object.
   */
  MccOutputMode () { }

  /**
   * Copies the object.
   * @return a clone of itself.
   */
  virtual MccOutputMode* clone () const = 0;

  /**
   * Destroys the object.
   */
  virtual ~MccOutputMode () { }

  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor) = 0;

  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const = 0;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const = 0;
};



/**
 * @short Class representing the file option on AST nodes "explore" and
 * "restore".
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccFilePdbOutput : public MccOutputMode
{
  /**
   * The filename for ouput (without extension)
   */
  string filename;

  /**
   * The boolean indicating if the files will be zipped.
   */
  bool zipped;

  bool mult_file;

  // LIFECYCLE ------------------------------------------------------------

public:
  
  /**
   * Initializes the object.  It should never be used.
   */
  MccFilePdbOutput () 
    : filename ("unamed"),
      zipped (false),
      mult_file (true)
  { }

  /**
   * Initializes the object.
   * @param fn the C form that will be used for file output.
   * @param z the boolean indicating if the files will be zipped.
   */
  MccFilePdbOutput (const string& fn, bool z, bool m) 
    : filename (fn), 
      zipped (z), 
      mult_file (m) 
  { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccFilePdbOutput (const MccFilePdbOutput &right)
    : filename (right.filename),
      zipped (right.zipped),
      mult_file (right.mult_file)
  { }

  /**
   * Copies the object.
   * @return a clone of the current object.
   */
  virtual MccOutputMode* clone () const { return new MccFilePdbOutput (*this); }
    
  /**
   * Destroys the object.
   */
  virtual ~MccFilePdbOutput () { }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the right struct values to the object.
   * @param right the struct to copy.
   */
  MccFilePdbOutput& operator= (const MccFilePdbOutput &right);

  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const { display (os); }
};



/**
 * @short Class representing the binary file option on AST nodes "explore"
 * and "restore" 
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccFileBinaryOutput : public MccOutputMode
{
  /**
   * The filename for ouput (without extension)
   */
  string filename;

  /**
   * The boolean indicating if the files will be zipped.
   */
  bool zipped;

  bool mult_file;


  // LIFECYCLE ------------------------------------------------------------

public:
  
  /**
   * Initializes the object.  It should never be used.
   */
  MccFileBinaryOutput () 
    : filename ("unamed"),
      zipped (false),
      mult_file (true)
  { }

  /**
   * Initializes the object.
   * @param fn the C form that will be used for file output.
   * @param z the boolean indicating if the files will be zipped.
   */
  MccFileBinaryOutput (const string& fn, bool z, bool m) 
    : filename (fn), 
      zipped (z), 
      mult_file (m) 
  { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccFileBinaryOutput (const MccFileBinaryOutput &right)
    : filename (right.filename),
      zipped (right.zipped),
      mult_file (right.mult_file)
  { }

  /**
   * Copies the object.
   * @return a clone of the current object.
   */
  virtual MccOutputMode* clone () const
  { return new MccFileBinaryOutput (*this); }
    
  /**
   * Destroys the object.
   */
  virtual ~MccFileBinaryOutput () {  }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the right struct values to the object.
   * @param right the struct to copy.
   */
  MccFileBinaryOutput& operator= (const MccFileBinaryOutput &right);

  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const { display (os); }
};



/**
 * @short Class representing the socket binary option on AST nodes "explore",
 * "exploreLV" and "restore".
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccSocketBinaryOutput : public MccOutputMode
{
  /**
   * The server name.
   */
  string serverName;

  /**
   * The port number.
   */
  int port;

  /**
   * Model name for identification.
   */
  string modelName;

  bool mult_file;

  // LIFECYCLE ------------------------------------------------------------

public:
  
  /**
   * Initializes the object. 
   */
  MccSocketBinaryOutput () 
    : serverName ("localhost"),
      port (6666),
      modelName ("unamed"),
      mult_file (true)
  { }

  /**
   * Initializes the object.
   * @param sn the server name.
   * @param sp the server port number.
   * @param mn the C form that will be used for the model name.
   */
  MccSocketBinaryOutput (const string& sn, int sp, const string& mn, bool m)
    : serverName (sn), port (sp), modelName (mn), mult_file (m) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccSocketBinaryOutput (const MccSocketBinaryOutput &right)
    : serverName (right.serverName),
      port (right.port),
      modelName (right.modelName),
      mult_file (right.mult_file)
  { }

  /**
   * Copies the object.
   * @return a clone of the current object.
   */
  virtual MccOutputMode* clone () const
  { return new MccSocketBinaryOutput (*this); }
    
  /**
   * Destroys the object.
   */
  virtual ~MccSocketBinaryOutput () { }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the right struct values to the object.
   * @param right the struct to copy.
   */
  MccSocketBinaryOutput& operator= (const MccSocketBinaryOutput &right);

  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const { display (os); }
};


/**
 * @short Class representing the file option on AST nodes "explore" and
 * "restore".
 *
 * @author Martin Larose (<a href="mailto:larosem@iro.umontreal.ca">larosem@iro.umontreal.ca</a>)
 */
struct MccFileRnamlOutput : public MccOutputMode
{
  /**
   * The filename for ouput (without extension)
   */
  string filename;

  /**
   * The boolean indicating if the files will be zipped.
   */
  bool zipped;

  bool mult_file;

  // LIFECYCLE ------------------------------------------------------------

public:
  
  /**
   * Initializes the object.  
   */
  MccFileRnamlOutput () 
    : filename ("unamed"),
      zipped (false),
      mult_file (true)
  { }

  /**
   * Initializes the object.
   * @param name the ouput file name.
   * @param z the boolean indicating if the files will be zipped.
   */
  MccFileRnamlOutput (const string& fn, bool z, bool m) 
    : filename (fn), 
      zipped (z), 
      mult_file (m) 
  { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccFileRnamlOutput (const MccFileRnamlOutput &right)
    : filename (right.filename),
      zipped (right.zipped),
      mult_file (right.mult_file)
  { }

  /**
   * Copies the object.
   * @return a clone of the current object.
   */
  virtual MccOutputMode* clone () const { return new MccFileRnamlOutput (*this); }
    
  /**
   * Destroys the object.
   */
  virtual ~MccFileRnamlOutput () { }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the right struct values to the object.
   * @param right the struct to copy.
   */
  MccFileRnamlOutput& operator= (const MccFileRnamlOutput &right);

  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const { display (os); }
};


/**
 * @short Abstract class for input model modes.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccInputMode
{
  
public:
  
  // LIFECYCLE ------------------------------------------------------------

  /**
   * Initializes the object.
   */
  MccInputMode () { }

  /**
   * Copies the object.
   * @return a clone of itself.
   */
  virtual MccInputMode* clone () const = 0;

  /**
   * Destroys the object.
   */
  virtual ~MccInputMode () { }

  // OPERATORS ------------------------------------------------------------

  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor) = 0;

  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const = 0;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const = 0;
};



/**
 * @short Input model class for pdb files.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccFilePdbInput : public MccInputMode
{
  /**
   * The file name. 
   */
  string filename;
    
  // LIFECYCLE ------------------------------------------------------------

public:

  /**
   * Initializes the object.  It should not be used.
   */
  MccFilePdbInput ()
    : filename ("unamed") 
  { }

  /**
   * Initializes the object with a name.
   * @param n the name of the input file(s).
   */
  MccFilePdbInput (const string& n) : filename (n) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccFilePdbInput (const MccFilePdbInput &right) : filename (right.filename) { }

  /**
   * Copies the object.
   * @return a clone of itself.
   */
  virtual MccFilePdbInput* clone () const
  { return new MccFilePdbInput (*this); }

  /**
   * Destroys the object.
   */
  virtual ~MccFilePdbInput () { }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the right struct values to the object.
   * @param right the struct to copy.
   */
  MccFilePdbInput& operator= (const MccFilePdbInput &right);
  
  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const { display (os); }
};



/**
 * @short Input model class for binary files.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccFileBinaryInput : public MccInputMode
{
  /**
   * The file name.  
   */
  string filename;  
    
  // LIFECYCLE ------------------------------------------------------------

public:

  /**
   * Initializes the object.
   */
  MccFileBinaryInput ()
    : filename ("unamed")
  { }

public:

  /**
   * Initializes the object with a name.
   * @param n the name of the input file(s).
   */
  MccFileBinaryInput (const string& n) : filename (n) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccFileBinaryInput (const MccFileBinaryInput &right) : filename (right.filename) { }

  /**
   * Copies the object.
   * @return a clone of itself.
   */
  virtual MccFileBinaryInput* clone () const
  { return new MccFileBinaryInput (*this); }

  /**
   * Destroys the object.
   */
  virtual ~MccFileBinaryInput () { }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the right struct values to the object.
   * @param right the struct to copy.
   */
  MccFileBinaryInput& operator= (const MccFileBinaryInput &right);
  
  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const { display (os); }
};



/**
 * @short Input model class for binary sockets.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccSocketBinaryInput : public MccInputMode
{
  /**
   * The server name.
   */
  string serverName;

  /**
   * The port number.
   */
  int port;

  /**
   * Model name for identification.
   */
  vector< string > modelNames;
  
  // LIFECYCLE ------------------------------------------------------------

public:

  /**
   * Initializes the object. 
   */
  MccSocketBinaryInput () 
    : serverName ("localhost"),
      port (6666)
  { }

  /**
   * Initializes the object with a name.
   * @param sn the server name.
   * @param p the port number.
   * @param mn the name of the input models.
   */
  MccSocketBinaryInput (const string& sn, int sp, const vector< string >& mns)
    : serverName (sn), port (sp), modelNames (mns) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccSocketBinaryInput (const MccSocketBinaryInput &right)
    : serverName (right.serverName),
      port (right.port),
      modelNames (right.modelNames)
  { }

  /**
   * Copies the object.
   * @return a clone of itself.
   */
  virtual MccSocketBinaryInput* clone () const
  { return new MccSocketBinaryInput (*this); }

  /**
   * Destroys the object.
   */
  virtual ~MccSocketBinaryInput () { }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the right struct values to the object.
   * @param right the struct to copy.
   */
  MccSocketBinaryInput& operator= (const MccSocketBinaryInput &right);
  
  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const { display (os); }
};



/**
 * @short Input model class for rnaml files.
 *
 * @author Martin Larose (<a href="mailto:larosem@iro.umontreal.ca">larosem@iro.umontreal.ca</a>)
 */
struct MccFileRnamlInput : public MccInputMode
{
  /**
   * The file name. 
   */
  string filename;
  
  // LIFECYCLE ------------------------------------------------------------

public:

  /**
   * Initializes the object.  It should not be used.
   */
  MccFileRnamlInput ()
    : filename ("unamed") 
  { }

  /**
   * Initializes the object with a name.
   * @param n the name of the input file(s).
   */
  MccFileRnamlInput (const string& n) : filename (n) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccFileRnamlInput (const MccFileRnamlInput &right) : filename (right.filename) { }

  /**
   * Copies the object.
   * @return a clone of itself.
   */
  virtual MccFileRnamlInput* clone () const
  { return new MccFileRnamlInput (*this); }

  /**
   * Destroys the object.
   */
  virtual ~MccFileRnamlInput () {  }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the right struct values to the object.
   * @param right the struct to copy.
   */
  MccFileRnamlInput& operator= (const MccFileRnamlInput &right);
  
  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const { display (os); }
};


/**
 * @short Struct representing the AST node of the "db_insert" form.
 *
 * @author Philippe Thibault <philippe.thibault@umontreal.ca>
 */
struct MccDBSetVersionStat : public MccPStruct
{
  
  string dbversion;

  // LIFECYCLE ------------------------------------------------------------

protected:

  /**
   * Initializes the object.  It should not be used.
   */
  MccDBSetVersionStat () { }

public:

  /**
   * Initializes the object.
   */
  MccDBSetVersionStat (const string& vstr)
    : dbversion (vstr)
  { }

  /**
   * Initializes the struct with the rights content.
   * @param right the object to copy.
   */
  MccDBSetVersionStat (const MccDBSetVersionStat &right)
    : dbversion (right.dbversion)
  { }
  
  /**
   * Destroys the structure.  Clears the vector of sub-structures.
   */
  virtual ~MccDBSetVersionStat ()
  { }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
   * @param right the object to copy.
   * @return itself.
   */
  MccDBSetVersionStat& operator= (const MccDBSetVersionStat &right);
  
  // ACCESS ---------------------------------------------------------------

  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccDBSetVersionStat* clone () const { return new MccDBSetVersionStat (*this); }

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  // I/O ------------------------------------------------------------

  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const;
};


/**
 * @short Struct representing the AST node of the "db_insert" form.
 *
 * @author Philippe Thibault <philippe.thibault@umontreal.ca>
 */
struct MccDBInsertStat : public MccPStruct
{

  /**
   * The input model mode.
   */
  MccInputMode *inputMode;

  /**
   * The identity threshold
   */
  float threshold;

  // LIFECYCLE ------------------------------------------------------------

protected:

  /**
   * Initializes the object.  It should not be used.
   */
  MccDBInsertStat () { }

public:

  /**
   * Initializes the object.
   */
  MccDBInsertStat (MccInputMode *im, float th = -1.0)
    : inputMode (im),
      threshold (th)
  { 

  }

  /**
   * Initializes the struct with the rights content.
   * @param right the object to copy.
   */
  MccDBInsertStat (const MccDBInsertStat &right);
  
  /**
   * Destroys the structure.  Clears the vector of sub-structures.
   */
  virtual ~MccDBInsertStat ();

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
   * @param right the object to copy.
   * @return itself.
   */
  MccDBInsertStat& operator= (const MccDBInsertStat &right);
  
  // ACCESS ---------------------------------------------------------------

  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccDBInsertStat* clone () const { return new MccDBInsertStat (*this); }

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  // I/O ------------------------------------------------------------

  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const;
};


/**
 * @short Struct representing the AST node "explore".
 *
 * @author Philippe Thibault <thibaup@iro.umontreal.ca>
 */
struct MccExploreStat : public MccPStruct
{
  
  /**
   * The FG struct to explore.
   */
  MccFragGenStruc fg_struc;
  
  /**
   * Parameters to the global exploration mechanism.
   */
  map< string, string > exp_parameters;

  /**
   * The model cache.
   */
  MccResidueViewCache* filter;
  
  /**
   * The explore output file structure.
   */
  MccOutputMode* output;
  
  // LIFECYCLE ------------------------------------------------------------

public:

  /**
   * Initializes the object.
   */
  MccExploreStat () :
    filter (0),
    output (0)
  { }

  MccExploreStat (const MccFragGenStruc& fg,
		  const map< string, string >& exp_par,
		  MccResidueViewCache* mrvc,
		  MccOutputMode* ef)
    : fg_struc (fg),
      exp_parameters (exp_par),
      filter (mrvc),
      output (ef)
  { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccExploreStat (const MccExploreStat &right);

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccExploreStat* clone () const { return new MccExploreStat (*this); }
    
  /**
   * Destroys the object.
   */
  virtual ~MccExploreStat ();

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
   * @param right the object to copy.
   * @return itself.
   */
  MccExploreStat& operator= (const MccExploreStat &right);
  
  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const;
};



/**
 * @short Struct representing a "library" AST node.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccLibraryExpr : public MccFGExp
{
  
  /**
   * The input model modes.
   */
  vector< MccInputMode* > inputModes;

  /**
   * Original residue names to be changed.
   */
  MccResidueNameSingletons res_from;

  /**
   * New residue names.
   */
  MccResidueNameSingletons res_to;

  /**
   * Flag acknowledging if model should be pre-filtered by constraints.
   */
  bool asis;

  /**
   * Specialized library container (NULL if unspecified).
   */
  MccResidueViewCache* library;
  
protected:
  
  // LIFECYCLE ------------------------------------------------------------
    
  /**
   * Initializes the object.  It should not be used.
   */
  MccLibraryExpr () { }

public:

  /**
   * Initializes the object.
   * @param im the input model mode.
   * @param lsv the vector containing the library sub-structures.
   */
  MccLibraryExpr (const vector< MccInputMode* >& imv, 
		  bool asis_flag,
		  MccResidueViewCache* mrvc = 0)
    : inputModes (imv), 
      asis (asis_flag),
      library (mrvc)
  { 
  }

  MccLibraryExpr (const vector< MccInputMode* >& imv,
		  const MccResidueNameSingletons& from,
		  const MccResidueNameSingletons& to,
		  bool asis_flag,
		  MccResidueViewCache* mrvc = 0)
    : inputModes (imv), 
      res_from (from),
      res_to (to),
      asis (asis_flag),
      library (mrvc)
  { 
  }


  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccLibraryExpr (const MccLibraryExpr &right);

  /**
   * Destroys the object.
   */
  virtual ~MccLibraryExpr ();

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
   * @param right the object to copy.
   * @return itself.
   */
  MccLibraryExpr& operator= (const MccLibraryExpr &right);
  
  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccLibraryExpr* clone () const { return new MccLibraryExpr (*this); }
    
  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);
  
  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const;
};



// /**
//  * @short Struct representing a "library" AST node.
//  *
//  * The struct contains a local hierarchy of statements: _LibStruc (the
//  * parent), _StripStruc (for the "strip" substatement) and _ChangeIdStruc
//  * (for the "change_id" substatement).
//  *
//  * @author Martin Larose <larosem@iro.umontreal.ca>
//  */
// struct MccLibraryExpr : public MccFGExp
// {
//   /**
//    * @short Parent struct for the MccLibraryExpr sub-structures.
//    *
//    * @author Martin Larose <larosem@iro.umontreal.ca>
//    */
//   struct _LibStruc
//   {

//     // LIFECYCLE ------------------------------------------------------------
    
//     /**
//      * Initializes the object.  It should never be used.
//      */
//     _LibStruc () { }
    
//     /**
//      * Initializes the object with the rights content.
//      * @param right the object to copy.
//      */
//     _LibStruc (const _LibStruc &right) { }

//     /**
//      * Destroys the object.  The destruction is controlled by the children
//      * of the structure.
//      */
//     virtual ~_LibStruc () { }

//     // OPERATORS ------------------------------------------------------------

//     /**
//      * Assigns the rights content into the object.
//      * @param right the object to copy.
//      * @return itself.
//      */
//     _LibStruc& operator= (const _LibStruc &right) { return *this; }
  
//     // ACCESS ---------------------------------------------------------------
    
//     // METHODS --------------------------------------------------------------

//     /**
//      * Replicates the object.  The replication is controlled by the children.
//      * @return a copy of the current object.
//      */
//     virtual _LibStruc* clone () const = 0;
    
//     /**
//      * Accepts the visitor and calls it on itself.
//      * @param visitor the visitor.
//      */
//     virtual void accept (MccVisitor *visitor) = 0;

//     // I/O  -----------------------------------------------------------------
    
//     /**
//      * Displays the structure.
//      * @param os the output stream where the message is displayed.
//      */
//     virtual void display (ostream &os) const = 0;

//     /**
//      * Displays the script in human readable form.
//      * @param os the output stream used.
//      * @param ident the identation level.
//      */
//     virtual void ppdisplay (ostream &os, int indent = 0) const = 0;
//   };
  
//   /**
//    * @short Struct containing the residue names to strip.
//    *
//    * @author Martin Larose <larosem@iro.umontreal.ca>
//    */
//   struct _StripStruc : public _LibStruc
//   {
//     /**
//      * The residue name vector.
//      */
//     vector< MccResidueName* > *res_vec;
    
//     // LIFECYCLE ------------------------------------------------------------

//   private:
//     /**
//      * Initializes the object.  It should never be used.
//      */
//     _StripStruc ()
//       : _LibStruc (), res_vec (0) { }
    
//   public:

//     /**
//      * Initializes the object.
//      * @param rv the residue name vector
//      */
//     _StripStruc (vector< MccResidueName* > *rv)
//       : _LibStruc (), res_vec (rv) { }

//     /**
//      * Initializes the object with the rights content.
//      * @param right the object to copy.
//      */
//     _StripStruc (const _StripStruc &right);

//     virtual ~_StripStruc ();
    
//     // OPERATORS ------------------------------------------------------------

//     /**
//      * Assigns the rights content into the object.
//      * @param right the object to copy.
//      * @return itself.
//      */
//     _StripStruc& operator= (const _StripStruc &right);
  
//     // ACCESS ---------------------------------------------------------------
    
//     // METHODS --------------------------------------------------------------

//     /**
//      * Replicates the object.
//      * @return a copy of the current object.
//      */
//     virtual _StripStruc* clone () const { return new _StripStruc (*this); }
    
//     /**
//      * Accepts the visitor and calls it on itself.
//      * @param visitor the visitor.
//      */
//     virtual void accept (MccVisitor *visitor);

//     // I/O  -----------------------------------------------------------------
    
//     /**
//      * Displays the structure.
//      * @param os the output stream where the message is displayed.
//      */
//     virtual void display (ostream &os) const;

//     /**
//      * Displays the script in human readable form.
//      * @param os the output stream used.
//      * @param ident the identation level.
//      */
//     virtual void ppdisplay (ostream &os, int indent = 0) const;
//   };

//   /**
//    * @short Struct containing the residue id to change.
//    *
//    * @author Martin Larose <larosem@iro.umontreal.ca>
//    */
//   struct _ChangeIdStruc : public _LibStruc
//   {
//     // LIFECYCLE ------------------------------------------------------------

//     MccResidueName* old_id;
//     MccResidueName* new_id;
    
//   private:
    
//     /**
//      * Initializes the object.  It should never be used.
//      */
//     _ChangeIdStruc () : _LibStruc () { }
    
//   public:

//     /**
//      * Initializes the object.
//      * @param o the old residue ID.
//      * @param n the new residue ID.
//      */
//     _ChangeIdStruc (MccResidueName* o, MccResidueName* n)
//       : _LibStruc (), old_id (o), new_id (n) { }

//     /**
//      * Initializes the object with the rights content.
//      * @param right the object to copy.
//      */
//     _ChangeIdStruc (const _ChangeIdStruc &right)
//       : _LibStruc (right),
// 	old_id (right.old_id->clone ()),
// 	new_id (right.new_id->clone ())
//     { }

//     virtual ~_ChangeIdStruc ();
    
//     // OPERATORS ------------------------------------------------------------

//     /**
//      * Assigns the rights content into the object.
//      * @param right the object to copy.
//      * @return itself.
//      */
//     _ChangeIdStruc& operator= (const _ChangeIdStruc &right);
  
//     // ACCESS ---------------------------------------------------------------
    
//     // METHODS --------------------------------------------------------------

//     /**
//      * Replicates the object.
//      * @return a copy of the current object.
//      */
//     virtual _ChangeIdStruc* clone () const { return new _ChangeIdStruc (*this); }
    
//     /**
//      * Accepts the visitor and calls it on itself.
//      * @param visitor the visitor.
//      */
//     virtual void accept (MccVisitor *visitor);

//     // I/O  -----------------------------------------------------------------
    
//     /**
//      * Displays the structure.
//      * @param os the output stream where the message is displayed.
//      */
//     virtual void display (ostream &os) const;

//     /**
//      * Displays the script in human readable form.
//      * @param os the output stream used.
//      * @param ident the identation level.
//      */
//     virtual void ppdisplay (ostream &os, int indent = 0) const;
//   };

  
//   /**
//    * @short Struct containing the chain id to change.
//    *
//    * @author Martin Larose <larosem@iro.umontreal.ca>
//    */
//   struct _ChangeChainStruc : public _LibStruc
//   {
//     // LIFECYCLE ------------------------------------------------------------

//     char old_chain;
//     char new_chain;
    
//   private:
    
//     /**
//      * Initializes the object.  It should never be used.
//      */
//     _ChangeChainStruc () : _LibStruc () { }
    
//   public:

//     /**
//      * Initializes the object.
//      * @param o the old residue chain ID.
//      * @param n the new residue chain ID.
//      */
//     _ChangeChainStruc (char o, char n)
//       : _LibStruc (), old_chain (o), new_chain (n) { }

//     /**
//      * Initializes the object with the rights content.
//      * @param right the object to copy.
//      */
//     _ChangeChainStruc (const _ChangeChainStruc &right)
//       : _LibStruc (right),
// 	old_chain (right.old_chain),
// 	new_chain (right.new_chain)
//     { }

//     virtual ~_ChangeChainStruc () { }
    
//     // OPERATORS ------------------------------------------------------------

//     /**
//      * Assigns the rights content into the object.
//      * @param right the object to copy.
//      * @return itself.
//      */
//     _ChangeChainStruc& operator= (const _ChangeChainStruc &right);
  
//     // ACCESS ---------------------------------------------------------------
    
//     // METHODS --------------------------------------------------------------

//     /**
//      * Replicates the object.
//      * @return a copy of the current object.
//      */
//     virtual _ChangeChainStruc* clone () const { return new _ChangeChainStruc (*this); }
    
//     /**
//      * Accepts the visitor and calls it on itself.
//      * @param visitor the visitor.
//      */
//     virtual void accept (MccVisitor *visitor);

//     // I/O  -----------------------------------------------------------------
    
//     /**
//      * Displays the structure.
//      * @param os the output stream where the message is displayed.
//      */
//     virtual void display (ostream &os) const;

//     /**
//      * Displays the script in human readable form.
//      * @param os the output stream used.
//      * @param ident the identation level.
//      */
//     virtual void ppdisplay (ostream &os, int indent = 0) const;
//   };

  
//   /**
//    * The input model mode.
//    */
//   MccInputMode *inputMode;

//   /**
//    * The vector containing the sub-structures.
//    */
//   vector< _LibStruc* > *strucs;

//   /**
//    * Flag acknowledging if model should be pre-filtered by constraints.
//    */
//   bool asis;

//   MccResidueViewCache* library;
  
// protected:
  
//   // LIFECYCLE ------------------------------------------------------------
    
//   /**
//    * Initializes the object.  It should not be used.
//    */
//   MccLibraryExpr () { }

// public:

//   /**
//    * Initializes the object.
//    * @param im the input model mode.
//    * @param lsv the vector containing the library sub-structures.
//    */
//   MccLibraryExpr (MccInputMode *im, 
// 		  vector< _LibStruc* > *lsv, 
// 		  bool asis_flag,
// 		  MccResidueViewCache* mrvc = 0)
//     : inputMode (im), 
//       strucs (lsv), 
//       asis (asis_flag),
//       library (mrvc)
//   { 
//   }

//   /**
//    * Initializes the object with the rights content.
//    * @param right the object to copy.
//    */
//   MccLibraryExpr (const MccLibraryExpr &right);

//   /**
//    * Destroys the object.
//    */
//   virtual ~MccLibraryExpr ();

//   // OPERATORS ------------------------------------------------------------

//   /**
//    * Assigns the rights content into the object.
//    * @param right the object to copy.
//    * @return itself.
//    */
//   MccLibraryExpr& operator= (const MccLibraryExpr &right);
  
//   // ACCESS ---------------------------------------------------------------
  
//   // METHODS --------------------------------------------------------------

//   /**
//    * Replicates the object.
//    * @return a copy of the current object.
//    */
//   virtual MccLibraryExpr* clone () const { return new MccLibraryExpr (*this); }
    
//   /**
//    * Accepts the visitor and calls it on itself.
//    * @param visitor the visitor.
//    */
//   virtual void accept (MccVisitor *visitor);

//   /**
//    * Generates a new strip sub-structure and puts it in the vector.
//    * @param rv the residue name vector.
//    */
//   void genStripStruc (vector< MccResidueName* > *rv)
//   { strucs->push_back (new _StripStruc (rv)); }

//   /**
//    * Generates a new ChangeId sub-structure and puts it in the vector.
//    * @param o the old residue ID.
//    * @param n the new residue ID.
//    */
//   void genChangeIdStruc (MccResidueName* o, MccResidueName* n)
//   { strucs->push_back (new _ChangeIdStruc (o, n)); }

//   /**
//    * Generates a new ChangeChain sub-structure and puts it in the vector.
//    * @param o the old residue chain ID.
//    * @param n the new residue chain ID.
//    */
//   void genChangeChainStruc (char o, char n)
//   { strucs->push_back (new _ChangeChainStruc (o, n)); }
  
//   // I/O  -----------------------------------------------------------------
  
//   /**
//    * Displays the structure.
//    * @param os the output stream where the message is displayed.
//    */
//   virtual void display (ostream &os) const;

//   /**
//    * Displays the script in human readable form.
//    * @param os the output stream used.
//    * @param ident the identation level.
//    */
//   virtual void ppdisplay (ostream &os, int indent = 0) const;
// };



/**
 * @short Struct representing the "new_tag" statement.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccNewTagStat : public MccPStruct
{

  /**
   * The name of the tag.
   */
  char *id;

  /**
   * The query expression structure.
   */
  vector< MccQueryExpr* > *exprs;

  
  // LIFECYCLE ------------------------------------------------------------

private:
  /**
   * Initializes the object.  It should never be used.
   */
  MccNewTagStat () { }
public:

  /**
   * Initializes the object.
   * @param i the name of the tag.
   * @param e the query expression structure.
   */
  MccNewTagStat (char *i, vector< MccQueryExpr* > *e)
    : id (i), exprs (e) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccNewTagStat (const MccNewTagStat &right);

  /**
   * Destroys the object.
   */
  virtual ~MccNewTagStat ();

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
   * @param right the object to copy.
   * @return itself.
   */
  MccNewTagStat& operator= (const MccNewTagStat &right);
  
  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccNewTagStat* clone () const { return new MccNewTagStat (*this); }
    
  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const;
};



/**
 * @short Struct representing the "note" statement.
 *
 * Inputs an note in the database.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccNoteStat : public MccPStruct
{
  /**
   * The note to add in the database.
   */
  char *str;

  
  // LIFECYCLE ------------------------------------------------------------

private:
  /**
   * Initializes the object.  It should never be used.
   */
  MccNoteStat () { }
public:

  /**
   * Initializes the object.
   * @param s the note to add in the database.
   */
  MccNoteStat (char *s) : str (s) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccNoteStat (const MccNoteStat &right);

  /**
   * Destroys the object.
   */
  virtual ~MccNoteStat () { delete[] str; }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
   * @param right the object to copy.
   * @return itself.
   */
  MccNoteStat& operator= (const MccNoteStat &right);
  
  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccNoteStat* clone () const { return new MccNoteStat (*this); }
    
  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const
  { os << "note (\"" << str << "\")"; }

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const;
};



/**
 * @short Struct representing the AST statement "notes".
 *
 * Displays the database note on standard output.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccNotesStat : public MccPStruct
{
  // LIFECYCLE ------------------------------------------------------------
    
  /**
   * Initializes the object.
   */
  MccNotesStat () { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccNotesStat (const MccNotesStat &right) { }

  /**
   * Destroys the object.
   */
  virtual ~MccNotesStat () { }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
   * @param right the object to copy.
   * @return itself.
   */
  MccNotesStat& operator= (const MccNotesStat &right)
  { return *this; }
  
  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccNotesStat* clone () const { return new MccNotesStat (*this); }
    
  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const { os << "notes"; }

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const;
};


/**
 * @short Struct representing the AST node "relation" statement.
 *
 * The struct contains a local substatement: _RelationStruc that contains each
 * pairing description.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccRelationStat : public MccPStruct
{
  /**
   * @short Sub-structure containing the pairing informations.
   *
   * @author Martin Larose <larosem@iro.umontreal.ca>
   */
  struct _RelationStruc
  {
    
//     /**
//      * The list of residue reference pairs. Each string represents a pair
//      * of residue references. Two forms are possible:
//      *
//      *  o "<char><int><' '><char><int>" for a single relation specification
//      *  o "<char><int><'-'><char><int>" for a relation range specification
//      *
//      */
//     vector< char* > *resrefs;

    MccResidueNamePairs* rnpairs;

    /**
     * The query expression structure.
     */
    MccQueryExpr *expr;

    /**
     * The sampling size structure.
     */
    MccSamplingSize *ssize;

  protected:

    // LIFECYCLE ------------------------------------------------------------

    /**
     * Initializes the object.  It should never be used.
     */
    _RelationStruc () { }
    
  public:

    /**
     * Initializes the object.
     * @param refs the residue reference string vector.
     * @param e the query expression structure.
     * @param s the sampling size structure.
     */
//     _RelationStruc (vector< char* > *refs, MccQueryExpr *e, MccSamplingSize *s)
//       : resrefs (refs), expr (e), ssize (s) { }
    _RelationStruc (MccResidueNamePairs* rnp, MccQueryExpr *e, MccSamplingSize *s)
      : rnpairs (rnp), expr (e), ssize (s) { }

    /**
     * Initializes the object with the rights content.
     * @param right the object to copy.
     */
    _RelationStruc (const _RelationStruc &right);

    /**
     * Destroys the object.
     */
    ~_RelationStruc ();

    // OPERATORS ------------------------------------------------------------

    /**
     * Assigns the rights content into the object.
     * @param right the object to copy.
     * @return itself.
     */
    _RelationStruc& operator= (const _RelationStruc &right);
  
    // ACCESS ---------------------------------------------------------------
    
    // METHODS --------------------------------------------------------------

    /**
     * Replicates the object.
     * @return a copy of the current object.
     */
    _RelationStruc* clone () const { return new _RelationStruc (*this); }
    
    /**
     * Accepts the visitor and calls it on itself.
     * @param visitor the visitor.
     */
    void accept (MccVisitor *visitor);

    // I/O  -----------------------------------------------------------------
    
    /**
     * Displays the structure.
     * @param os the output stream where the message is displayed.
     */
    void display (ostream &os) const;

    /**
     * Displays the script in human readable form.
     * @param os the output stream used.
     * @param ident the identation level.
     */
    void ppdisplay (ostream &os, int indent = 0) const;
  };

  /**
   * The vector containing the pairing sub-structures.
   */
  vector< _RelationStruc* > *strucs;


  // LIFECYCLE ------------------------------------------------------------
    
  /**
   * Initializes the object.
   */
  MccRelationStat () : strucs (new vector< _RelationStruc* > ()) { }

  /**
   * Initializes the object.
   * @param psv the vector containing the pairing sub-structures.
   */
  MccRelationStat (vector< _RelationStruc* > *psv) : strucs (psv) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccRelationStat (const MccRelationStat &right);

  /**
   * Destroys the object.
   */
  virtual ~MccRelationStat ();

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
   * @param right the object to copy.
   * @return itself.
   */
  MccRelationStat& operator= (const MccRelationStat &right);
  
  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
   virtual MccRelationStat* clone () const { return new MccRelationStat (*this); }
    
  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  /**
   * Generates a new pairing sub-structures and puts it in the vector.
   * @param refs the residue reference string vector.
   * @param e the query expression structure.
   * @param s the sampling size structure.
   */   
  void genRelationStruc (MccResidueNamePairs* rnp,
			 MccQueryExpr *e,
			 MccSamplingSize *s)
  { 
    strucs->push_back (new _RelationStruc (rnp, e, s)); 
  }
  
  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const;
};


/**
 * @short Struct representing the AST node "ribose_rst" statement.
 *
 * @author Philippe Thibault <philippe.thibault@umontreal.ca>
 */
struct MccBacktrackRstStat : public MccPStruct
{
  /**
   * The FG struct on which the restraint applies.
   */
  MccFragGenStruc fg_struc;

  map< string, string > parameters;

  // LIFECYCLE ------------------------------------------------------------
    
  /**
   * Default constructor must not be used.
   */
  MccBacktrackRstStat () { }

  /**
   * Initializes the object.
   */
  MccBacktrackRstStat (const MccFragGenStruc& fg, const map< string, string >& par);

  /**
   * Initializes the object with the rights content.
   * @param rrst the object to copy.
   */
  MccBacktrackRstStat (const MccBacktrackRstStat& rrst);

  /**
   * Destroys the object.
   */
  virtual ~MccBacktrackRstStat () { }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
   * @param rrst the object to copy.
   * @return itself.
   */
  MccBacktrackRstStat& operator= (const MccBacktrackRstStat& rrst);
  
  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccBacktrackRstStat* clone () const 
  { 
    return new MccBacktrackRstStat (*this); 
  }
    
  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);
 
  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const;
};


/**
 * @short Struct representing the AST node "ribose_rst" statement.
 *
 * @author Philippe Thibault <philippe.thibault@umontreal.ca>
 */
struct MccRiboseRstStat : public MccPStruct
{
  /**
   * The FG struct on which the restraint applies.
   */
  MccFragGenStruc fg_struc;

  MccResidueNameSingletons restrictions;

  map< string, string > parameters;

  // LIFECYCLE ------------------------------------------------------------
    
  /**
   * Default constructor must not be used.
   */
  MccRiboseRstStat () { }

  /**
   * Initializes the object.
   */
  MccRiboseRstStat (const MccFragGenStruc& fg, const map< string, string >& par);

  /**
   * Initializes the object.
   */
  MccRiboseRstStat (const MccFragGenStruc& fg, 
		    const MccResidueNameSingletons& res,
		    const map< string, string >& par);



  /**
   * Initializes the object with the rights content.
   * @param rrst the object to copy.
   */
  MccRiboseRstStat (const MccRiboseRstStat& rrst);

  /**
   * Destroys the object.
   */
  virtual ~MccRiboseRstStat () { }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
   * @param rrst the object to copy.
   * @return itself.
   */
  MccRiboseRstStat& operator= (const MccRiboseRstStat& rrst);
  
  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccRiboseRstStat* clone () const 
  { 
    return new MccRiboseRstStat (*this); 
  }
    
  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);
 
  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const;
};


/**
 * @short Struct representing the AST node "implicit_phosphate_rst" statement.
 *
 * @author Philippe Thibault <philippe.thibault@umontreal.ca>
 */
struct MccImplicitPhosphateRstStat : public MccPStruct
{
  /**
   * The FG struct on which the restraint applies.
   */
  MccFragGenStruc fg_struc;

  MccResidueNamePairs restrictions;

  map< string, string > parameters;

  // LIFECYCLE ------------------------------------------------------------
    
  /**
   * Default constructor must not be used.
   */
  MccImplicitPhosphateRstStat () { }

  /**
   * Initializes the object.
   */
  MccImplicitPhosphateRstStat (const MccFragGenStruc& fg, const map< string, string >& par);

  /**
   * Initializes the object.
   */
  MccImplicitPhosphateRstStat (const MccFragGenStruc& fg, 
			       const MccResidueNamePairs& res,
			       const map< string, string >& par);



  /**
   * Initializes the object with the rights content.
   * @param rrst the object to copy.
   */
  MccImplicitPhosphateRstStat (const MccImplicitPhosphateRstStat& rrst);

  /**
   * Destroys the object.
   */
  virtual ~MccImplicitPhosphateRstStat () { }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
   * @param rrst the object to copy.
   * @return itself.
   */
  MccImplicitPhosphateRstStat& operator= (const MccImplicitPhosphateRstStat& rrst);
  
  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccImplicitPhosphateRstStat* clone () const 
  { 
    return new MccImplicitPhosphateRstStat (*this); 
  }
    
  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);
 
  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const;
};


/**
 * @short Struct representing the AST statement "quit".
 *
 * This statement ends the mcsym script.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccQuitStat : public MccPStruct
{
  // LIFECYCLE ------------------------------------------------------------
    
  /**
   * Initializes the object.
   */
  MccQuitStat () { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccQuitStat (const MccQuitStat &right) { }

  /**
   * Destroys the object.
   */
  virtual ~MccQuitStat () { }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
   * @param right the object to copy.
   * @return itself.
   */
  MccQuitStat& operator= (const MccQuitStat &right) { return *this; }
  
  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
   virtual MccQuitStat* clone () const { return new MccQuitStat (*this); }
    
  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const { os << "quit"; }

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const;
};



/**
 * @short Struct representing the "implicit_relation" statement.
 *
 * This statement generate constraints between two residues related together
 * by properties.  This struct owns a local struct _ImplicitRelationStruc
 * that contains the informations on the constraints.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccImplicitRelationCstStat : public MccPStruct
{
  /**
   * @short Sub-struct containing a relation constraint description.
   *
   * @author Martin Larose <larosem@iro.umontreal.ca>
   */
  struct _ImplicitRelationStruc
  {
    /**
     * The first residue name.
     */
    MccResidueName *ref;

    /**
     * The second residue name.
     */
    MccResidueName *res;

    /**
     * The query expression structure.
     */
    MccQueryExpr *expr;
    
  protected:
    
    // LIFECYCLE ------------------------------------------------------------

    /**
     * Initializes the object.  It should never be used.
     */
    _ImplicitRelationStruc () { }

  public:

    /**
     * Initializes the object.
     * @param r1 the first residue name.
     * @param r2 the second residue name.
     * @param e the query expression.
     */
    _ImplicitRelationStruc (MccResidueName *r1, MccResidueName *r2, MccQueryExpr *e)
      : ref (r1), res (r2), expr (e)
    { }

    /**
     * Initializes the object with the rights content.
     * @param right the object to copy.
     */
    _ImplicitRelationStruc (const _ImplicitRelationStruc &right);

    /**
     * Replicates the object.
     * @return a copy of the current object.
     */
    _ImplicitRelationStruc* clone () const { return new _ImplicitRelationStruc (*this); }

    /**
     * Destroys the residue names and atom names.
     */
    ~_ImplicitRelationStruc ();

    // OPERATORS ------------------------------------------------------------

    /**
     * Assigns the rights content into the object.
     * @param right the object to copy.
     * @return itself.
     */
    _ImplicitRelationStruc& operator= (const _ImplicitRelationStruc &right);
    
    // ACCESS ---------------------------------------------------------------

    // METHODS --------------------------------------------------------------

    /**
     * Accepts the visitor and calls it on itself.
     * @param visitor the visitor.
     */
    void accept (MccVisitor *visitor);
    
    // I/O  -----------------------------------------------------------------

    /**
     * Displays the structure.
     * @param os the output stream where the message is displayed.
     */
    void display (ostream &os) const;

    /**
     * Displays the script in human readable form.
     * @param os the output stream used.
     * @param ident the identation level.
     */
    void ppdisplay (ostream &os, int indent = 0) const;
  };

  /**
   * The vector of _ImplicitRelationStruc.
   */
  vector< _ImplicitRelationStruc* > *strucs;

  
  // LIFECYCLE ------------------------------------------------------------

  /**
   * Initializes the object.
   */
  MccImplicitRelationCstStat () : strucs (new vector< _ImplicitRelationStruc* > ()) { }

  /**
   * Initializes the object.
   * @param asv the relation sub-structure vector.
   */
  MccImplicitRelationCstStat (vector< _ImplicitRelationStruc* > *asv) : strucs (asv) { }
  
  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccImplicitRelationCstStat (const MccImplicitRelationCstStat &right);
  
  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccImplicitRelationCstStat* clone () const
  { return new MccImplicitRelationCstStat (*this); }

  /**
   * Destroys the vector of sub-structures.
   */
  virtual ~MccImplicitRelationCstStat ();

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
   * @param right the object to copy.
   * @return itself.
   */
  MccImplicitRelationCstStat& operator= (const MccImplicitRelationCstStat &right);

  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  // I/O ------------------------------------------------------------------

  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const;
};



/**
 * @short Struct representing the AST "remark" statement.
 *
 * This statement inserts a remark for output in the pdb files.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccRemarkStat : public MccPStruct
{
  /**
   * The remark to insert.
   */
  char *str;


  // LIFECYCLE ------------------------------------------------------------

private:
  /**
   * Initializes the object.  It should never be used.
   */
  MccRemarkStat () { }
public:

  /**
   * Initializes the object.
   * @param s the remark to insert.
   */
  MccRemarkStat (char *s) : str (s) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccRemarkStat (const MccRemarkStat &right);

  /**
   * Deletes the object.
   */
  virtual ~MccRemarkStat () { delete[] str; }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
   * @param right the object to copy.
   * @return itself.
   */
  MccRemarkStat& operator= (const MccRemarkStat &right);
  
  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
   virtual MccRemarkStat* clone () const { return new MccRemarkStat (*this); }
    
  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const
  { os << "remark (\"" << str << "\")"; }

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const;
};



/**
 * @short Struct representing the "db_reset" statement in AST form.
 *
 * This statement resets the database file.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccDBResetStat : public MccPStruct
{
  // LIFECYCLE ------------------------------------------------------------
  
  /**
   * Initializes the object.
   */
  MccDBResetStat () { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccDBResetStat (const MccDBResetStat &right) { }

  /**
   * Destroys the object.
   */
  virtual ~MccDBResetStat () { }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
   * @param right the object to copy.
   * @return itself.
   */
  MccDBResetStat& operator= (const MccDBResetStat &right)
  { return *this; }
  
  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccDBResetStat* clone () const { return new MccDBResetStat (*this); }
    
  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const { os << "db_reset"; }

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const;
};



/**
 * @short Struct representing the "reset" statement in AST form.
 *
 * This statement reset the environment of the mcsym program.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccResetStat : public MccPStruct
{
  // LIFECYCLE ------------------------------------------------------------
  
  /**
   * Initializes the object.
   */
  MccResetStat () { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccResetStat (const MccResetStat &right) { }

  /**
   * Destroys the object.
   */
  virtual ~MccResetStat () { }

  // OPERATORS ------------------------------------------------------------
  
  /**
   * Assigns the rights content into the object.
   * @param right the object to copy.
   * @return itself.
   */
  MccResetStat& operator= (const MccResetStat &right)
  { return *this; }
  
  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccResetStat* clone () const { return new MccResetStat (*this); }
    
  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const { os << "reset"; }

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const;
};


/**
 * @short Struct representing the AST node "restore".
 *
 * This statement restores the saved status of an interrupted mcsym
 * exploration.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccRestoreStat : public MccPStruct
{
  /**
   * The file name of the status file.
   */
  char *filename;

  /**
   * The explore output file structure.
   */
  MccOutputMode *expOutput;
  

  // LIFECYCLE ------------------------------------------------------------

private:
  /**
   * Initializes the object.  It should never be used.
   */
  MccRestoreStat () { }
public:

  /**
   * Initializes the object.
   * @param fi the file name of the status file.
   * @param ef the explore output file structure.
   */
  MccRestoreStat (char *fi, MccOutputMode *ef)
    : filename (fi), expOutput (ef) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccRestoreStat (const MccRestoreStat &right);

  /**
   * Deletes the object.
   */
  ~MccRestoreStat () { delete[] filename; if (expOutput) delete expOutput; }

  // OPERATORS -----------------------------------------------------------

  /**
   * Assigns the rights values in the object.
   * @param right the object to copy.
   * @return itself.
   */
  MccRestoreStat& operator= (const MccRestoreStat &right);

  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccRestoreStat* clone () const { return new MccRestoreStat (*this); }
    
  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const;
};




/**
 * @short Struct representing the AST node for the "sequence" statement.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccSequenceStat : public MccPStruct
{

  /**
   * The type of the defined sequence [pdr].
   */
  char type;

  /**
   * The residue name of the first residue in the sequence.
   */
  MccResidueName *res;

  /**
   * The sequence.
   */
  string seq;

protected:

  // LIFECYCLE ------------------------------------------------------------

  /**
   * Initializes the object.  It should never be used.
   */
  MccSequenceStat () { }

public:

  /**
   * Initializes the object.
   * @param t the type of the defined sequence.
   * @param res the residue name of the first residue in the sequence.
   * @param str the sequence.
   */
  MccSequenceStat (char t, MccResidueName *r, const string& s)
    : type (t), res (r), seq (s)
  { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccSequenceStat (const MccSequenceStat &right);

  /**
   * Destroys the object.
   */
  virtual ~MccSequenceStat () { delete res; }

  // OPERATORS -----------------------------------------------------------

  /**
   * Assigns the rights values in the object.
   * @param right the object to copy.
   * @return itself.
   */
  MccSequenceStat& operator= (const MccSequenceStat &right);

  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccSequenceStat* clone () const { return new MccSequenceStat (*this); }
    
  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const;
};



/**
 * @short Struct representing the AST form of the "source" statement.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccSourceStat : public MccPStruct
{
  /**
   * The path and file name of the mcsym script.
   */
  char *str;

protected:

  // LIFECYCLE ------------------------------------------------------------

  /**
   * Initializes the object.  It should never be used.
   */
  MccSourceStat () { }
  
public:

  /**
   * Initializes the object.
   * @param s the path and file name of the mcsym script.
   */
  MccSourceStat (char *s) : str (s) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccSourceStat (const MccSourceStat &right);

  /**
   * Destroys the object.
   */
  virtual ~MccSourceStat () { delete[] str; }

  // OPERATORS -----------------------------------------------------------

  /**
   * Assigns the rights values in the object.
   * @param right the object to copy.
   * @return itself.
   */
  MccSourceStat& operator= (const MccSourceStat &right);

  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccSourceStat* clone () const { return new MccSourceStat (*this); }
    
  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const
  { os << "source (\"" << str << "\")"; }

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const;
};



/**
 * @short Struct representing the "torsion" statement.
 *
 * This statement generate a constraint between four atoms to fit in a lower
 * and an upper bound.  This struct owns a local struct _TorsionStruc that
 * contains the informations on the constraints.
 *
 * Usage: first generate a MccTorsionCstStat object and then you can fill its
 * _TorsionStruc* vector using the GenTorsionStruc method. This vector
 * contains all information about the specifications of the constraint.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccTorsionCstStat : public MccPStruct
{
  /**
   * @short Sub-struct containing a torsion constraint description.
   *
   * @author Martin Larose <larosem@iro.umontreal.ca>
   */
  struct _TorsionStruc
  {
    /**
     * The first residue name.
     */
    MccResidueName *res1;
    
    /**
     * The atom name of the first residue.
     */
    char *at1;
    
    /**
     * The second residue name.
     */
    MccResidueName *res2;
    
    /**
     * The atom name of the second residue.
     */
    char *at2;
    
    /**
     * The third residue name.
     */
    MccResidueName *res3;
    
    /**
     * The atom name of the third residue.
     */
    char *at3;
    
    /**
     * The fourth residue name.
     */
    MccResidueName *res4;
    
    /**
     * The atom name of the fourth residue.
     */
    char *at4;
    
    /**
     * The target torsion angle value (in degrees).
     */
    float angle;

    /**
     * The absolute error value (in degrees).
     */
    float error;
    

    // LIFECYCLE ------------------------------------------------------------

  private:
    /**
     * Initializes the object.  It should never be used.
     */
    _TorsionStruc () { }
  public:

    /**
     * Initializes the object.
     * @param r1 the first residue name.
     * @param a1 the atom name of the first residue.
     * @param r2 the second residue name.
     * @param a2 the atom name of the second residue.
     * @param r3 the third residue name.
     * @param a3 the atom name of the third residue.
     * @param r4 the fourth residue name.
     * @param a4 the atom name of the fourth residue.
     * @param mn the minimum torsion angle value (in degrees).
     * @param mx the maximum torsion angle value (in degrees).
     */
    _TorsionStruc (MccResidueName *r1, char *a1, MccResidueName *r2, char *a2,
		   MccResidueName *r3, char *a3, MccResidueName *r4, char *a4,
		   float a, float e)
      : res1 (r1), at1 (a1), res2 (r2), at2 (a2),
	res3 (r3), at3 (a3), res4 (r4), at4 (a4),
	angle (a), error (e)
    { }

    /**
     * Initializes the object with the rights content.
     * @param right the object to copy.
     */
    _TorsionStruc (const _TorsionStruc &right);

    /**
     * Destroys the residue names and atom names.
     */
    ~_TorsionStruc ();
    
    // OPERATORS ------------------------------------------------------------

    /**
     * Assigns the rights values in the object.
     * @param right the object to copy.
     * @return itself.
     */
    _TorsionStruc& operator= (const _TorsionStruc &right);

    // ACCESS ---------------------------------------------------------------

    // METHODS --------------------------------------------------------------

    /**
     * Replicates the object.
     * @return a copy of the current object.
     */
    _TorsionStruc* clone () const { return new _TorsionStruc (*this); }
    
    /**
     * Accepts the visitor and calls it on itself.
     * @param visitor the visitor.
     */
    void accept (MccVisitor *visitor);

    // I/O  -----------------------------------------------------------------

    /**
     * Displays the structure.
     * @param os the output stream where the message is displayed.
     */
    void display (ostream &os) const;

    /**
     * Displays the script in human readable form.
     * @param os the output stream used.
     * @param ident the identation level.
     */
    void ppdisplay (ostream &os, int indent = 0) const;
  };

  /**
   * The vector of torsion sub-structures.
   */
  vector< _TorsionStruc* > *strucs;

  
  // LIFECYCLE ------------------------------------------------------------

  /**
   * Initializes the object.
   */
  MccTorsionCstStat () : strucs (new vector< _TorsionStruc* > ()) { }

  /**
   * Initializes the object.
   * @param tsv the torsion sub-structure vector.
   */
  MccTorsionCstStat (vector< _TorsionStruc* > *tsv) : strucs (tsv) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccTorsionCstStat (const MccTorsionCstStat &right);

  /**
   * Destroys the vector of sub-structures.
   */
  virtual ~MccTorsionCstStat ();

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights values in the object.
   * @param right the object to copy.
   * @return itself.
   */
   MccTorsionCstStat& operator= (const MccTorsionCstStat &right);

  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccTorsionCstStat* clone () const
  { return new MccTorsionCstStat (*this); }
    
  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  /**
   * Generates a new sub-structure and puts it in the vector.
   * @param r1 the first residue name.
   * @param a1 the atom name of the first residue.
   * @param r2 the second residue name.
   * @param a2 the atom name of the second residue.
   * @param r3 the third residue name.
   * @param a3 the atom name of the third residue.
   * @param r4 the fourth residue name.
   * @param a4 the atom name of the fourth residue.
   * @param mn the minimum torsion angle value (in degrees).
   * @param mx the maximum torsion angle value (in degrees).
   */
  void GenTorsionStruc (MccResidueName *r1, char *a1,
			MccResidueName *r2, char *a2,
			MccResidueName *r3, char *a3,
			MccResidueName *r4, char *a4,
			float mn, float mx)
  {
    strucs->push_back (new _TorsionStruc (r1, a1, r2, a2, r3, a3, r4, a4, mn, mx));
  }
  
  // I/O ------------------------------------------------------------------

  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const;
};



/**
 * @short Struct representing the AST form of the "version" statement.
 *
 * This statement displays the version number of mcsym and its database to
 * the standard output.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccVersion : public MccPStruct
{
  // LIFECYCLE ------------------------------------------------------------

  /**
   * Initializes the object.
   */
  MccVersion () { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccVersion (const MccVersion &right) { }

  /**
   * Destroys the object.
   */
  virtual ~MccVersion () { }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights values in the object.
   * @param right the object to copy.
   * @return itself.
   */
  MccVersion& operator= (const MccVersion &right) { return *this; }

  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccVersion* clone () const { return new MccVersion (*this); }
    
  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);

  // I/O ------------------------------------------------------------------

  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const { os << "version"; }

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const;
};


/**
 * @short Struct representing the AST node "report_implicit_phosphate" statement.
 *
 * @author Philippe Thibault <philippe.thibault@umontreal.ca>
 */
struct MccReportImplicitPhosphateStat : public MccPStruct
{
  /**
   * The FG struct on which the restraint applies.
   */
  MccFragGenStruc fg_struc;

  // LIFECYCLE ------------------------------------------------------------
    
  /**
   * Default constructor must not be used.
   */
  MccReportImplicitPhosphateStat () { }

  /**
   * Initializes the object.
   */
  MccReportImplicitPhosphateStat (const MccFragGenStruc& fg);

  /**
   * Initializes the object with the rights content.
   * @param rrst the object to copy.
   */
  MccReportImplicitPhosphateStat (const MccReportImplicitPhosphateStat& obj);

  /**
   * Destroys the object.
   */
  virtual ~MccReportImplicitPhosphateStat () { }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
   * @param rrst the object to copy.
   * @return itself.
   */
  MccReportImplicitPhosphateStat& operator= (const MccReportImplicitPhosphateStat& obj);
  
  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccReportImplicitPhosphateStat* clone () const 
  { 
    return new MccReportImplicitPhosphateStat (*this); 
  }
    
  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);
 
  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const;
};


/**
 * @short Struct representing the AST node "report_ribose" statement.
 *
 * @author Philippe Thibault <philippe.thibault@umontreal.ca>
 */
struct MccReportRiboseStat : public MccPStruct
{
  /**
   * The FG struct on which the restraint applies.
   */
  MccFragGenStruc fg_struc;

  // LIFECYCLE ------------------------------------------------------------
    
  /**
   * Default constructor must not be used.
   */
  MccReportRiboseStat () { }

  /**
   * Initializes the object.
   */
  MccReportRiboseStat (const MccFragGenStruc& fg);

  /**
   * Initializes the object with the rights content.
   * @param rrst the object to copy.
   */
  MccReportRiboseStat (const MccReportRiboseStat& obj);

  /**
   * Destroys the object.
   */
  virtual ~MccReportRiboseStat () { }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
   * @param rrst the object to copy.
   * @return itself.
   */
  MccReportRiboseStat& operator= (const MccReportRiboseStat& obj);
  
  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccReportRiboseStat* clone () const 
  { 
    return new MccReportRiboseStat (*this); 
  }
    
  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void accept (MccVisitor *visitor);
 
  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const;

  /**
   * Displays the script in human readable form.
   * @param os the output stream used.
   * @param ident the identation level.
   */
  virtual void ppdisplay (ostream &os, int indent = 0) const;
};


/**
 * @short Basic visitor class for the parser structures.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
class MccVisitor
{
public:

  // LIFECYCLE ------------------------------------------------------------

  /**
   * Initializes the object.
   */
  MccVisitor () { }

  /**
   * Initializes the object with the right's content.
   * @param right the object to copy.
   */
  MccVisitor (const MccVisitor &right) { }

  /**
   * Destroys the object.
   */
  virtual ~MccVisitor () { }

  // OPERATORS ----------------------------------------------------------

  /**
   * Assigns the rights content in the object.
   * @param right the object to copy.
   * @return itself.
   */
  MccVisitor& operator= (const MccVisitor &right) { return *this; }

  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Visits the MccFragGenStruc structure.
   * @param struc the MccFragGenStruc structure.
   */
  virtual void visit (MccFragGenStruc *struc) = 0;

  /**
   * Visits the MccResidueName structure.
   * @param struc the MccResidueName structure.
   */
  virtual void visit (MccResidueName *struc) = 0;

  /**
   * Visits the MccResidueNamePairs structure.
   * @param struc the MccResidueNamePairs structure.
   */
  virtual void visit (MccResidueNamePairs *struc) = 0;

  /**
   * Visits the MccResidueNameSingletons structure.
   * @param struc the MccResidueNameSingletons structure.
   */
  virtual void visit (MccResidueNameSingletons *struc) = 0;

  /**
   * Visits the MccQTrueFunc structure.
   * @param struc the MccQTrueFunc structure.
   */
  virtual void visit (MccQTrueFunc *struc) = 0;

  /**
   * Visits the MccQIdentFunc structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccQIdentFunc *struc) = 0;

  /**
   * Visits the MccQNotFunc structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccQNotFunc *struc) = 0;

  /**
   * Visits the MccQFaceFunc structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccQFaceFunc *struc) = 0;

  /**
   * Visits the MccQAndFunc structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccQAndFunc *struc) = 0;

  /**
   * Visits the MccQOrFunc structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccQOrFunc *struc) = 0;

  /**
   * Visits the MccQueryExpr structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccQueryExpr *struc) = 0;

  /**
   * Visits the MccASIdentFunc structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccASIdentFunc *struc) = 0;

  /**
   * Visits the MccASNotFunc structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccASNotFunc *struc) = 0;

  /**
   * Visits the MccASAndFunc structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccASAndFunc *struc) = 0;

  /**
   * Visits the MccASOrFunc structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccASOrFunc *struc) = 0;

  /**
   * Visits the MccRMSDResidueViewCache structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccRMSDResidueViewCache* struc) = 0;

  /**
   * Visits the MccTFODResidueViewCache structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccTFODResidueViewCache* struc) = 0;

  /**
   * Visits the MccDBSetVersionStat structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccDBSetVersionStat *struc) = 0;

  /**
   * Visits the MccDBInsertStat structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccDBInsertStat *struc) = 0;

  /**
   * Visits the MccBaseAdjacencyCstStat structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccBaseAdjacencyCstStat *struc) = 0;
  
  /**
   * Visits the MccAngleCstStat::_AngleStruc structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccAngleCstStat::_AngleStruc *struc) = 0;
  
  /**
   * Visits the MccAngleCstStat structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccAngleCstStat *struc) = 0;
  
  /**
   * Visits the MccAssignStat assignment statement node.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccAssignStat *struc) = 0;

  /**
   * Visits the MccBacktrackExpr::_FGStruc sub-structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccBacktrackExpr::_FGStruc *struc) = 0;
  
  /**
   * Visits the MccBacktrackExpr::_BTStruc sub-structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccBacktrackExpr::_BTStruc *struc) = 0;

  /**
   * Visits the MccBacktrackExpr::_PlaceStruc sub-structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccBacktrackExpr::_PlaceStruc *struc) = 0;

  /**
   * Visits the MccBacktrackExpr::_MergeStruc sub-structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccBacktrackExpr::_MergeStruc *struc) = 0;
  
  /**
   * Visits the MccBacktrackExpr structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccBacktrackExpr *struc) = 0;
  
  /**
   * Visits the MccCycleExpr structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccCycleExpr *struc) = 0;

  /**
   * Visits the MccCacheExpr structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccCacheExpr *struc) = 0;
  
  /**
   * Visits the MccClashCstStat structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccClashCstStat *struc) = 0;
  
  virtual void visit (MccCycleCstStat::_CycleStruc *struc) = 0;
  
  virtual void visit (MccCycleCstStat *struc) = 0;

  /**
   * Visits the MccMultimerCstStat::_MultimerStruc sub-structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccMultimerCstStat::_MultimerStruc *struc) = 0;
  
  /**
   * Visits the MccMultimerCstStat structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccMultimerCstStat *struc) = 0;
  
  /**
   * Visits the MccDBDisplayStat structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccDBDisplayStat *struc) = 0;

  /**
   * Visits the MccDisplayFGStat structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccDisplayFGStat *struc) = 0;

  /**
   * Visits the MccDBFilterStat structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccDBFilterStat *struc) = 0;

  /**
   * Visits the MccDistCstStat::_DistStruc sub-structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccDistCstStat::_DistStruc *struc) = 0;

  /**
   * Visits the MccDistCstStat structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccDistCstStat *struc) = 0;

  /**
   * Evaluates the MccEnvStat structure. It displays the environment's current content.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccEnvStat *struc) = 0;
  
  /**
   * Visits the MccFilePdbOutput structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccFilePdbOutput *struc) = 0;
  
  /**
   * Visits the MccFileBinaryOutput structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccFileBinaryOutput *struc) = 0;
  
  /**
   * Visits the MccSocketBinaryOutput structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccSocketBinaryOutput *struc) = 0;
    
  /**
   * Visits the MccFileRnamlOutput structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccFileRnamlOutput *struc) = 0;
  
  /**
   * Visits the MccFilePdbInput structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccFilePdbInput *struc) = 0;
  
  /**
   * Visits the MccFileBinaryInput structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccFileBinaryInput *struc) = 0;
  
  /**
   * Visits the MccSocketBinaryInput structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccSocketBinaryInput *struc) = 0;
  
  /**
   * Visits the MccFileRnamlInput structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccFileRnamlInput *struc) = 0;
  
  /**
   * Visits the MccExploreStat structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccExploreStat *struc) = 0;
  
  /**
   * Visits the local MccLibraryExpr sub-structure _StripStruc.
   * @param struc the evaluated structure.
   */
  //virtual void visit (MccLibraryExpr::_StripStruc *struc) = 0;
  
  /**
   * Visits the local MccLibraryExpr sub-structure _ChangeIdStruc.
   * @param struc the evaluated structure.
   */
  //virtual void visit (MccLibraryExpr::_ChangeIdStruc *struc) = 0;

  /**
   * Visits the local MccLibraryExpr sub-structure _ChangeChainStruc.
   * @param struc the evaluated structure.
   */
  //virtual void visit (MccLibraryExpr::_ChangeChainStruc *struc) = 0;
  
  /**
   * Visits the MccMultimerCstStat structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccLibraryExpr *struc) = 0;

  /**
   * Visits the MccNewTagStat structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccNewTagStat *struc) = 0;

  /**
   * Visits the MccNoteStat structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccNoteStat *struc) = 0;
  
  /**
   * Visits the MccNotesStat structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccNotesStat *struc) = 0;

  /**
   * Visits the local MccImplicitRelationStat sub-structure _ImplicitRelationStruc.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccRelationStat::_RelationStruc *struc) = 0;
  
  /**
   * Visits the MccRelationStat structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccRelationStat *struc) = 0;

  /**
   * Visits the MccBacktrackRstStat structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccBacktrackRstStat *struc) = 0;

  /**
   * Visits the MccImplicitPhosphateRstStat structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccImplicitPhosphateRstStat *struc) = 0;

  /**
   * Visits the MccRiboseRstStat structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccRiboseRstStat *struc) = 0;

  /**
   * Visits the MccQuitStat structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccQuitStat *struc) = 0;

  /**
   * Visits the MccImplicitRelationCstStat::_ImplicitRelationStruc structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccImplicitRelationCstStat::_ImplicitRelationStruc *struc) = 0;
  
  /**
   * Visits the MccImplicitRelationCstStat structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccImplicitRelationCstStat *struc) = 0;
  
  /**
   * Visits the MccRemarkStat structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccRemarkStat *struc) = 0;

  /**
   * Visits the MccDBResetStat structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccDBResetStat *struc) = 0;
  
  /**
   * Visits the MccResetStat structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccResetStat *struc) = 0;
  
  /**
   * Visits the MccRestoreStat structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccRestoreStat *struc) = 0;
  
  /**
   * Visits the MccSamplingSize structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccSamplingSize *struc) = 0;

  /**
   * Visits the MccSequenceStat structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccSequenceStat *struc) = 0;

  /**
   * Visits the MccSourceStat structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccSourceStat *struc) = 0;
  
  /**
   * Visits the MccTorsionCstStat local sub-structure _TorsionStruc.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccTorsionCstStat::_TorsionStruc *struc) = 0;
  
  /**
   * Visits the MccTorsionCstStat structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccTorsionCstStat *struc) = 0;

  /**
   * Visits the MccVersion structure.
   * @param struc the evaluated structure.
   */
  virtual void visit (MccVersion *struc) = 0;

  virtual void visit (MccReportImplicitPhosphateStat *struc) = 0;

  virtual void visit (MccReportRiboseStat *struc) = 0;

  // I/O  -----------------------------------------------------------------

};


/**
 * @short Exception thrown by the lexer.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
class LexerException
{
  /**
   * The message.
   */
  char *str;

  /**
   * Initializes the object.  It should never be used.
   */
  LexerException () { }

public:

  // LIFECYCLE ------------------------------------------------------------

  /**
   * Initializes the object.
   * @param s the message.
   */
  LexerException (const char *s);

  /**
   * Initializes the object with the rights contents.
   * @param right the object to copy.
   */
  LexerException (const LexerException &right);

  /**
   * Destroys the object.
   */
  ~LexerException () { delete[] str; }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights contents in the object.
   * @param right the object to copy.
   * @return itself.
   */
  LexerException& operator= (const LexerException &right);

  /**
   * Concatenates a character in the message.
   * @param c the character to concatenate.
   * @return itself.
   */
  LexerException& operator<< (char c);

  /**
   * Concatenates an integer in the message.
   * @param n the integer to concatenate.
   * @return itself.
   */
  LexerException& operator<< (int n);

  /**
   * Concatenates a float in the message.
   * @param x the float to concatenate.
   * @return itself.
   */
  LexerException& operator<< (float x);

  /**
   * Concatenates a string in the message.
   * @param s the string to concatenate.
   * @return itself.
   */
  LexerException& operator<< (const char *s);

  // ACCESS ---------------------------------------------------------------

  /**
   * Gets the exception string.
   * @return the message string.
   */
  const char * GetMessage () const { return str; }

  // METHODS --------------------------------------------------------------

  // I/O ------------------------------------------------------------------
};

/**
 * Displays the message to the output stream.
 * @param os the output stream where the message is displayed.
 * @param ex the exception to display.
 * @return the output stream used.
 */
ostream& operator<< (ostream &os, const LexerException &ex);



/**
 * @short Exception thrown by the parser.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
class ParserException
{
  /**
   * The message.
   */
  char *str;

  /**
   * Initializes the object.  It should never be used.
   */
  ParserException () { }

public:

  // LIFECYCLE ------------------------------------------------------------

  /**
   * Initializes the object.
   * @param s the message.
   */
  ParserException (const char *s);

  /**
   * Initializes the object with the rights contents.
   * @param right the object to copy.
   */
  ParserException (const ParserException &right);

  /**
   * Destroys the object.
   */
  ~ParserException () { delete[] str; }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights contents in the object.
   * @param right the object to copy.
   * @return itself.
   */
  ParserException& operator= (const ParserException &right);

  /**
   * Concatenates a character in the message.
   * @param c the character to concatenate.
   * @return itself.
   */
  ParserException& operator<< (char c);

  /**
   * Concatenates an integer in the message.
   * @param n the integer to concatenate.
   * @return itself.
   */
  ParserException& operator<< (int n);

  /**
   * Concatenates a float in the message.
   * @param x the float to concatenate.
   * @return itself.
   */
  ParserException& operator<< (float x);

  /**
   * Concatenates a string in the message.
   * @param s the string to concatenate.
   * @return itself.
   */
  ParserException& operator<< (const char *s);

  // ACCESS ---------------------------------------------------------------

  /**
   * Gets the exception string.
   * @return the message string.
   */
  const char * GetMessage () const { return str; }

  // METHODS --------------------------------------------------------------

  // I/O ------------------------------------------------------------------
};

/**
 * Displays the message to the output stream.
 * @param os the output stream where the message is displayed.
 * @param ex the exception to display.
 * @return the output stream used.
 */
ostream& operator<< (ostream &os, const ParserException &ex);


#endif
