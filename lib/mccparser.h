//                              -*- Mode: C++ -*- 
// mccparser.h
// Copyright © 2000-01 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Thu Aug 24 12:14:42 2000
// Last Modified By : Philippe Thibault
// Last Modified On : Wed Oct 23 09:21:00 2002
// Update Count     : 21
// Status           : Ok.
// 


#ifndef _mccparser_h_
#define _mccparser_h_


#include <iostream.h>
#include <vector.h>
#include <pair.h>
#include <stdio.h>



typedef struct yy_buffer_state *YY_BUFFER_STATE;
YY_BUFFER_STATE mcc_scan_string (const char*);
void mcc_delete_buffer (YY_BUFFER_STATE);
void mcc_switch_to_buffer (YY_BUFFER_STATE);

class MccPStruct;
class MccVisitor;
class MccQueryExpr;


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
 * The atom set type.
 */
enum MccAS { MCC_ALL_AS, MCC_BASE_AS, MCC_BACKBONE_AS, MCC_PSE_AS,
	     MCC_NO_OPTION, MCC_NO_HYDROGEN };



/**
 * @short This class is used for reading input from terminal.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
class CMccInput
{
  /**
   * Initializes the object with the rights contents.  It must not be used.
   * @param right the object to copy.
   */
  CMccInput (const CMccInput &right) { }

  /**
   * Assigns the rights content in the object.  It must not be used.
   * @param right the object to copy.
   * @return itself.
   */
  virtual const CMccInput& operator= (const CMccInput &right) { return *this; }

public:

  // LIFECYCLE ------------------------------------------------------------

  /**
   * Initializes the object.
   */
  CMccInput () { }

  /**
   * Destroys the object.
   */
  virtual ~CMccInput () { }

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
extern CMccInput *input_class;



/**
 * @short Structure used in the parser for cutoffs.
 *
 * The default values are 0.5.  This default is also hardcoded in the parser
 * (in rules cfo_opt and tfo_opt) due to the grammar specifications.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct cutoffs
{
  /**
   * The transfo cutoff value.
   */
  float ctc;

  /**
   * The confo cutoff value.
   */
  float ccc;

  /**
   * Initializes the structure.
   * @param t the transfo cutoff value.
   * @param c the confo cutoff value.
   */
  cutoffs (float t = 0.5, float c = 0.5) : ctc (t), ccc (c) { }
};


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
  virtual void Accept (MccVisitor *visitor) = 0;
  
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
  virtual MccFGExp& operator= (const MccFGExp &right) { return *this; }

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
  virtual void Accept (MccVisitor *visitor) = 0;
  
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
  char *ident;

  /**
   * The variable indicates if the identifier contains any special
   * character.
   */
  bool special_char;
  

  // LIFECYCLE ------------------------------------------------------------

private:
  /**
   * Initializes the object.  It should never be used.
   */
  MccFragGenStruc () : ident (0), special_char (false) { }
public:

  /**
   * Initializes the object.
   * @param i the name of the FG.
   * @param sp the variable specifying if the ident contains any special char.
   */
  MccFragGenStruc (char *i, bool sp = false) : ident (i), special_char (sp) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccFragGenStruc (const MccFragGenStruc &right);

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  MccFragGenStruc* clone () const { return new MccFragGenStruc (*this); }

  /**
   * Destroys the object.
   */
  ~MccFragGenStruc () { delete[] ident; }

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
  void Accept (MccVisitor *visitor);
  
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
  virtual void Accept (MccVisitor *visitor);

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
 * @short Wrapper for query expressions specified in AST node moses. 
 *
 * @author Philippe Thibault <thibaup@IRO.UMontreal.CA>
 */
struct MccMosesQueries
{
  pair< MccQueryExpr*, MccSamplingSize* >* stem_cg_au_res;
  pair< MccQueryExpr*, MccSamplingSize* >* stem_gu_res;
  pair< MccQueryExpr*, MccSamplingSize* >* loop_res;
  pair< MccQueryExpr*, MccSamplingSize* >* stem_connect;
  pair< MccQueryExpr*, MccSamplingSize* >* loop_connect;
  pair< MccQueryExpr*, MccSamplingSize* >* stem_cg_au_pair;
  pair< MccQueryExpr*, MccSamplingSize* >* stem_gu_pair;

protected:

  // LIFECYCLE ------------------------------------------------------------

  /**
   * Initializes the object.  It should never be used.
   */
  MccMosesQueries () { }
  
public:
  
  /**
   * Initializes the object.
   *
   */
  MccMosesQueries (pair< MccQueryExpr*, MccSamplingSize* >* qsp1,
		   pair< MccQueryExpr*, MccSamplingSize* >* qsp2,
		   pair< MccQueryExpr*, MccSamplingSize* >* qsp3,
		   pair< MccQueryExpr*, MccSamplingSize* >* qsp4,
		   pair< MccQueryExpr*, MccSamplingSize* >* qsp5,
		   pair< MccQueryExpr*, MccSamplingSize* >* qsp6,
		   pair< MccQueryExpr*, MccSamplingSize* >* qsp7)
    : stem_cg_au_res (qsp1),
      stem_gu_res (qsp2),
      loop_res (qsp3),
      stem_connect (qsp4),
      loop_connect (qsp5),
      stem_cg_au_pair (qsp6),
      stem_gu_pair (qsp7)
  { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccMosesQueries (const MccMosesQueries &right);
		      

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  MccMosesQueries* clone () const { return new MccMosesQueries (*this); }
    
  /**
   * Destructs the object.
   */
  virtual ~MccMosesQueries ();

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the right struct values to the object.
   * @param right the struct to copy.
   */
  MccMosesQueries& operator= (const MccMosesQueries &right);

  // ACCESS ---------------------------------------------------------------

 
  
  // METHODS --------------------------------------------------------------

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void Accept (MccVisitor *visitor);

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
 * @short Struct representing the abstract model filter strategy.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccModelFilterStrategy
{

public:

  // LIFECYCLE ------------------------------------------------------------

  /**
   * Initializes the object.  It should never be used.
   */
  MccModelFilterStrategy () { }
  
  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccModelFilterStrategy* clone () const = 0;
    
  /**
   * Destroys the object.
   */
  virtual ~MccModelFilterStrategy () { }

  // OPERATORS ------------------------------------------------------------

  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void Accept (MccVisitor *visitor) = 0;

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
 * @short Struct representing the abstract model sorter strategy.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccModelSorterStrategy
{

public:

  // LIFECYCLE ------------------------------------------------------------

  /**
   * Initializes the object.  It should never be used.
   */
  MccModelSorterStrategy () { }
  
  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccModelSorterStrategy* clone () const = 0;
    
  /**
   * Destroys the object.
   */
  virtual ~MccModelSorterStrategy () { }

  // OPERATORS ------------------------------------------------------------

  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void Accept (MccVisitor *visitor) = 0;

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

private:
  /**
   * Initializes the object.  It should never be used.
   */
  MccResidueName () { }
public:

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
  void Accept (MccVisitor *visitor);
  
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
  virtual MccQFunc& operator= (const MccQFunc &right) { return *this; }
  
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
  virtual void Accept (MccVisitor *visitor) = 0;
  
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
  virtual MccQTrueFunc& operator= (const MccQTrueFunc &right) { return *this; }
  
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
  virtual void Accept (MccVisitor *visitor);
  
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
  virtual MccQIdentFunc& operator= (const MccQIdentFunc &right);
  
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
  virtual void Accept (MccVisitor *visitor);
  
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
  virtual MccQNotFunc& operator= (const MccQNotFunc &right);
  
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
  virtual void Accept (MccVisitor *visitor);

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
  virtual MccQFaceFunc& operator= (const MccQFaceFunc &right_val);
  
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
  virtual void Accept (MccVisitor *visitor);

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
  virtual MccQAndFunc& operator= (const MccQAndFunc &right_val);
  
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
  virtual void Accept (MccVisitor *visitor);

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
  virtual MccQOrFunc& operator= (const MccQOrFunc &right_val);
  
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
  virtual void Accept (MccVisitor *visitor);

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
  void Accept (MccVisitor *visitor);

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
 * @short Struct representing the AST node of the "add_pdb" form.
 *
 * This structure is a vector of sub-structures _AddPdbStruc.
 * Sub-structures can be added via the GenPdbStruc method.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccAddPdbStat : public MccPStruct
{
  /**
   * @short Sub-structure containing the pdb files associated with the cutoffs.
   *
   * @author Martin Larose <larosem@iro.umontreal.ca>
   */
  struct _AddPdbStruc
  {
    /**
     * The cutoff value for the tranfos.
     */
    float current_tfo_cutoff;
    
    /**
     * The cutoff value for the confos.
     */
    float current_confo_cutoff;
  
    /**
     * Vector of pdb file names.
     */
    vector < char* > *strs;


    // LIFECYCLE ------------------------------------------------------------

  private:
    /**
     * Initializes the struct.  It should not be used.
     */
    _AddPdbStruc () { }
  public:

    /**
     * Initializes the struct.
     * @param cos the cutoffs values.
     * @param s the vector of pdb file names.
     */
    _AddPdbStruc (float ctc, float ccc, vector< char* > *s)
      : current_tfo_cutoff (ctc), current_confo_cutoff (ccc), strs (s) { }

    /**
     * Initializes the struct with the rights content.
     * @param right the object to copy.
     */
    _AddPdbStruc (const _AddPdbStruc &right);
     
    /**
     * Destroys the structure.  Clears the vector of file names.
     */
    ~_AddPdbStruc ();

    // OPERATORS ------------------------------------------------------------

    /**
     * Assigns the rights content into the object.
     * @param right the object to copy.
     * @return itself.
     */
    _AddPdbStruc& operator= (const _AddPdbStruc &right);

    // ACCESS ---------------------------------------------------------------

    // METHODS --------------------------------------------------------------

    /**
     * Replicates the object.
     * @return a copy of the current object.
     */
    _AddPdbStruc* clone () const { return new _AddPdbStruc (*this); }

    /**
     * Accepts the visitor and calls it on itself.
     * @param visitor the visitor.
     */
    void Accept (MccVisitor *visitor);

    // I/O ------------------------------------------------------------

    /**
     * Displays the substructure.
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
   * The vector containing the sub-structures.
   */
  vector< _AddPdbStruc* > *strucs;


  // LIFECYCLE ------------------------------------------------------------

  /**
   * Initializes the object.
   */
  MccAddPdbStat () : strucs (new vector< _AddPdbStruc* > ()) { }

  /**
   * Initializes the object.
   * @param apsv the add_pdb sub-structure vector.
   */
  MccAddPdbStat (vector< _AddPdbStruc* > *apsv) : strucs (apsv) { }

  /**
   * Initializes the struct with the rights content.
   * @param right the object to copy.
   */
  MccAddPdbStat (const MccAddPdbStat &right);
  
  /**
   * Destroys the structure.  Clears the vector of sub-structures.
   */
  virtual ~MccAddPdbStat ();

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
   * @param right the object to copy.
   * @return itself.
   */
  virtual MccAddPdbStat& operator= (const MccAddPdbStat &right);
  
  // ACCESS ---------------------------------------------------------------

  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccAddPdbStat* clone () const { return new MccAddPdbStat (*this); }

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void Accept (MccVisitor *visitor);

  /**
   * Generates a new sub-structure and puts it in the vector.
   * @param ctc the transfo cutoff value.
   * @param ccc the confo cutoff value.
   * @param s the vector of pdb file names.
   */
  void GenPdbStruc (float ctc, float ccc, vector< char* > *s)
    { strucs->push_back (new _AddPdbStruc (ctc, ccc, s)); }

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
 * @short Struct representing the AST node "adjacency".
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccAdjacencyCstStat : public MccPStruct
{
  /**
   * The struct identifying the FG.
   */
  MccFragGenStruc *fg_struc;
  
  /**
   * The minimum adjacency value.
   */
  float the_min;
  
  /**
   * The maximum adjacency value.
   */
  float the_max;
  

  // LIFECYCLE ------------------------------------------------------------

private:
  /**
   * Initializes the object.  It should not be used.
   */
  MccAdjacencyCstStat () { }
public:

  /**
   * Initializes the object.
   * @param f the FG struct.
   * @param mn the minimum adjacency value.
   * @param mx the maximum adjacency value.
   */
  MccAdjacencyCstStat (MccFragGenStruc *f, float mn, float mx)
    : fg_struc (f), the_min (mn), the_max (mx) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccAdjacencyCstStat (const MccAdjacencyCstStat &right);

  /**
   * Destroys the structure.
   */
  virtual ~MccAdjacencyCstStat () { delete fg_struc; }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
   * @param right the object to copy.
   * @return itself.
   */
  virtual MccAdjacencyCstStat& operator= (const MccAdjacencyCstStat &right);

  // ACCESS ---------------------------------------------------------------

  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccAdjacencyCstStat* clone () const
  { return new MccAdjacencyCstStat (*this); }

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void Accept (MccVisitor *visitor);

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
     * The minimum angle value (in degrees).
     */
    float dist_min;

    /**
     * The maximum angle value (in degrees).
     */
    float dist_max;

    
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
		 MccResidueName *r3, char *a3, float mn, float mx)
      : res1 (r1), at1 (a1), res2 (r2), at2 (a2), res3 (r3), at3 (a3),
	dist_min (mn), dist_max (mx)
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
    void Accept (MccVisitor *visitor);
    
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
  virtual MccAngleCstStat& operator= (const MccAngleCstStat &right);

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
  virtual void Accept (MccVisitor *visitor);

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
  virtual MccAssignStat& operator= (const MccAssignStat &right);

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
  virtual void Accept (MccVisitor *visitor);

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
    virtual _GenBTStruc& operator= (const _GenBTStruc &right);

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
    virtual void Accept (MccVisitor *visitor) = 0;

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
    virtual _FGStruc& operator= (const _FGStruc &right);
    
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
    virtual void Accept (MccVisitor *visitor);

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
    virtual _BTStruc& operator= (const _BTStruc &right);
    
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
    virtual void Accept (MccVisitor *visitor);

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
    virtual _PlaceStruc& operator= (const _PlaceStruc &right);
    
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
    virtual void Accept (MccVisitor *visitor);
    
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
  virtual MccBacktrackExpr& operator= (const MccBacktrackExpr &right);
  
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
  virtual void Accept (MccVisitor *visitor);

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
 * @short Struct representing the AST node for the "cache" expression.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccCacheExpr : public MccFGExp
{
  /**
   * The FG structure to cache.
   */
  MccFragGenStruc *fgref;

  /**
   * The model cache.
   */
  MccModelFilterStrategy *filter;
  
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
  MccCacheExpr (MccFragGenStruc *f, MccModelFilterStrategy *c)
    : fgref (f), filter (c) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccCacheExpr (const MccCacheExpr &right);
  
  /**
   * Destroys the object.
   */
  virtual ~MccCacheExpr () { delete fgref; delete filter; }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
   * @param right the object to copy.
   * @return itself.
   */
  virtual MccCacheExpr& operator= (const MccCacheExpr &right);
  
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
  virtual void Accept (MccVisitor *visitor);

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
   * The set of atoms used to calculate the distance.
   */
  MccAS as;

  /**
   * The options on the atom set.
   */
  MccAS aso;

  
  // LIFECYCLE ------------------------------------------------------------

private:
  /**
   * Initializes the object.  It should never be used.
   */
  MccClashCstStat () { }
public:

  /**
   * Initializes the object.
   * @param fg the FG struct on which the constraint is verified.
   * @param V the variable specifying if we use Van Der Waals or fixed distance.
   * @param d the distance value.
   * @param a the set of atoms used to calculate the distance.
   * @param ao the options on the atom set.
   */
  MccClashCstStat (MccFragGenStruc *fg, bool V, float d, MccAS a, MccAS ao)
    : fg_struc (fg), VDWDist (V), distFac (d), as (a), aso (ao) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccClashCstStat (const MccClashCstStat &right);
  
  /**
   * Destroys the object.
   */
  virtual ~MccClashCstStat () { delete fg_struc; }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
   * @param right the object to copy.
   * @return itself.
   */
  virtual MccClashCstStat& operator= (const MccClashCstStat &right);
  
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
  virtual void Accept (MccVisitor *visitor);

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
 * @short Struct representing the "connect" AST node.
 *
 * It contains a local struct _ConnectStruc that collects each statement.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccConnectStat : public MccPStruct
{
  /**
   * @short Sub-structure containing the connect informations.
   *
   * @author Martin Larose <larosem@iro.umontreal.ca>
   */
  struct _ConnectStruc
  {
    /**
     * The name of the first residue in the relation.
     */
    MccResidueName *res1;

    /**
     * The name of the second residue in the relation.
     */
    MccResidueName *res2;

    /**
     * The query expression structure.
     */
    MccQueryExpr *expr;

    /**
     * The sampling size structure.
     */
    MccSamplingSize *ssize;

    
    // LIFECYCLE ------------------------------------------------------------

  private:
    /**
     * Initializes the object.  It should never be used.
     */
    _ConnectStruc () { }
  public:

    /**
     * Initializes the object.
     * @param r1 the name of the first residue in the relation.
     * @param r2 the name of the second residue in the relation.
     * @param e the query expression structure.
     * @param s the sampling size structure.
     */
    _ConnectStruc (MccResidueName *r1, MccResidueName *r2, 
		   MccQueryExpr *e, MccSamplingSize *s)
      : res1 (r1), res2 (r2), expr (e), ssize (s) { }

    /**
     * Initializes the object with the rights content.
     * @param right the object to copy.
     */
    _ConnectStruc (const _ConnectStruc &right);
  
    /**
     * Destroys the object.
     */
    ~_ConnectStruc () { delete res1; delete res2; delete expr; delete ssize; }

    // OPERATORS ------------------------------------------------------------

    /**
     * Assigns the rights content into the object.
     * @param right the object to copy.
     * @return itself.
     */
    _ConnectStruc& operator= (const _ConnectStruc &right);
    
    // ACCESS ---------------------------------------------------------------
    
    // METHODS --------------------------------------------------------------

    /**
     * Replicates the object.
     * @return a copy of the current object.
     */
    _ConnectStruc* clone () const { return new _ConnectStruc (*this); }
    
    /**
     * Accepts the visitor and calls it on itself.
     * @param visitor the visitor.
     */
    void Accept (MccVisitor *visitor);

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
   * The vector containing the connect sub-structures.
   */
  vector< _ConnectStruc* > *strucs;

  
  // LIFECYCLE ------------------------------------------------------------

  /**
   * Initializes the object.
   */
  MccConnectStat () : strucs (new vector< _ConnectStruc* > ()) { }

  /**
   * Initializes the object.
   * @param csv the vector containing the connect sub-structures.
   */
  MccConnectStat (vector< _ConnectStruc* > *csv) : strucs (csv) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccConnectStat (const MccConnectStat &right);
  
  /**
   * Destroys the object.  Clears the vector of sub-structures.
   */
  virtual ~MccConnectStat ();

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
   * @param right the object to copy.
   * @return itself.
   */
  virtual MccConnectStat& operator= (const MccConnectStat &right);
    
  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccConnectStat* clone () const { return new MccConnectStat (*this); }
    
  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void Accept (MccVisitor *visitor);

  /**
   * Generates a new connect sub-structure and puts it in the vector.
   * @param r1 the name of the first residue.
   * @param r2 the name of the second residue.
   * @param e the query expression structure.
   * @param s the sampling size structure.
   */
  void GenConnectStruc (MccResidueName *r1, MccResidueName *r2,
			MccQueryExpr *e, MccSamplingSize *s)
  { strucs->push_back (new _ConnectStruc (r1, r2, e, s)); }

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
struct MccCycleCstStat : public MccPStruct
{
  /**
   * @short Sub-struct containing an cycle constraint description.
   *
   * @author Martin Larose <larosem@iro.umontreal.ca>
   */
  struct _CycleStruc
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
    _CycleStruc () { }
  public:

    /**
     * Initializes the object.
     * @param r1 the name of the first residue.
     * @param r2 the name of the second residue.
     * @param d the distance value.
     * @param n the repeat value.
     */
    _CycleStruc (MccResidueName *r1, MccResidueName *r2, float d, int n)
      : res1 (r1), res2 (r2), dist (d), nb (n) { }

    /**
     * Initializes the object with the rights content.
     * @param right the object to copy.
     */
    _CycleStruc (const _CycleStruc &right);
    
    /**
     * Destroys the object.
     */
    ~_CycleStruc () { delete res1; delete res2; }

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
     * Replicates the object.
     * @return a copy of the current object.
     */
    _CycleStruc* clone () const { return new _CycleStruc (*this); }
    
    /**
     * Accepts the visitor and calls it on itself.
     * @param visitor the visitor.
     */
    void Accept (MccVisitor *visitor);

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
  vector< _CycleStruc* > *strucs;

  
  // LIFECYCLE ------------------------------------------------------------

  /**
   * Initializes the object.
   */
  MccCycleCstStat () : strucs (new vector< _CycleStruc* > ()) { }
  
  /**
   * Initializes the object.
   * @param csv the cycle sub-structure vector.
   */
  MccCycleCstStat (vector< _CycleStruc* > *csv) : strucs (csv) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
   MccCycleCstStat (const MccCycleCstStat &right);
  
  /**
   * Destroys the object.
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
   * Replicates the object.
   * @return a copy of the current object.
   */
  MccCycleCstStat* clone () const { return new MccCycleCstStat (*this); }
    
  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void Accept (MccVisitor *visitor);

  /**
   * Generates a new cycle sub-structure and puts it in the vector.
   * @param r1 the name of the first residue.
   * @param r2 the name of the second residue.
   * @param d the cutoff value.
   * @param n the repeat value.
   */
  void GenCycleStruc (MccResidueName *r1, MccResidueName *r2, float d, int n)
  { strucs->push_back (new _CycleStruc (r1, r2, d, n)); }

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
  virtual MccDisplayFGStat& operator= (const MccDisplayFGStat &right);

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
  virtual void Accept (MccVisitor *visitor);

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
    void Accept (MccVisitor *visitor);

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
  virtual MccDistCstStat& operator= (const MccDistCstStat &right);
    
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
  virtual void Accept (MccVisitor *visitor);

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
  virtual void Accept (MccVisitor *visitor) = 0;

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
   * The C form that will be used for file output.
   */
  char *form;

  /**
   * The boolean indicating if the files will be zipped.
   */
  bool zipped;


  // LIFECYCLE ------------------------------------------------------------

protected:
  
  /**
   * Initializes the object.  It should never be used.
   */
  MccFilePdbOutput () { }
  
public:

  /**
   * Initializes the object.
   * @param f the C form that will be used for file output.
   * @param z the boolean indicating if the files will be zipped.
   */
  MccFilePdbOutput (char *f, bool z) : form (f), zipped (z) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccFilePdbOutput (const MccFilePdbOutput &right);

  /**
   * Copies the object.
   * @return a clone of the current object.
   */
  virtual MccOutputMode* clone () const { return new MccFilePdbOutput (*this); }
    
  /**
   * Destroys the object.
   */
  virtual ~MccFilePdbOutput () { delete[] form; }

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
  virtual void Accept (MccVisitor *visitor);

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
   * The C form that will be used for file output.
   */
  char *form;

  /**
   * The boolean indicating if the files will be zipped.
   */
  bool zipped;


  // LIFECYCLE ------------------------------------------------------------

protected:
  
  /**
   * Initializes the object.  It should never be used.
   */
  MccFileBinaryOutput () { }
  
public:

  /**
   * Initializes the object.
   * @param f the C form that will be used for file output.
   * @param z the boolean indicating if the files will be zipped.
   */
  MccFileBinaryOutput (char *f, bool z) : form (f), zipped (z) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccFileBinaryOutput (const MccFileBinaryOutput &right);

  /**
   * Copies the object.
   * @return a clone of the current object.
   */
  virtual MccOutputMode* clone () const
  { return new MccFileBinaryOutput (*this); }
    
  /**
   * Destroys the object.
   */
  virtual ~MccFileBinaryOutput () { delete[] form; }

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
  virtual void Accept (MccVisitor *visitor);

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
  char *serverName;

  /**
   * The port number.
   */
  int port;

  /**
   * The C form that will be used for the model name.
   */
  char *modelName;

protected:
  
  // LIFECYCLE ------------------------------------------------------------
  
  /**
   * Initializes the object.  It should never be used.
   */
  MccSocketBinaryOutput () { }
  
public:

  /**
   * Initializes the object.
   * @param n the server name.
   * @param p the server port number.
   * @param m the C form that will be used for the model name.
   */
  MccSocketBinaryOutput (char *n, int p, char *m)
    : serverName (n), port (p), modelName (m) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccSocketBinaryOutput (const MccSocketBinaryOutput &right);

  /**
   * Copies the object.
   * @return a clone of the current object.
   */
  virtual MccOutputMode* clone () const
  { return new MccSocketBinaryOutput (*this); }
    
  /**
   * Destroys the object.
   */
  virtual ~MccSocketBinaryOutput ()
  { delete[] serverName; delete[] modelName; }

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
  virtual void Accept (MccVisitor *visitor);

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
 * @short Struct representing the backtrack size option on AST nodes "exploreLV"
 *
 * @author Philippe Thibault  <thibaup@IRO.UMontreal.CA>
 */
struct MccBacktrackSize
{

  /**
   * The maximum number of backtracked levels.
   */
  int maxBT;

  /**
   * The maximum number of retries on same level.
   */
  int maxLR;

protected:

  // LIFECYCLE ------------------------------------------------------------

  /**
   * Initializes the object.  It should never be used.
   */
  MccBacktrackSize () { }
  
public:

  /**
   * Initializes the object.
   * @param bt max number of backtracked levels
   * @param lr max number of retries on same level
   */
  MccBacktrackSize (int bt, int lr) : maxBT (bt), maxLR (lr) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccBacktrackSize (const MccBacktrackSize &right) 
    : maxBT (right.maxBT), maxLR (right.maxLR) { }

  /**
   * Destructs the object.
   */
  ~MccBacktrackSize () { }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the right struct values to the object.
   * @param right the struct to copy.
   */
  const MccBacktrackSize& operator= (const MccBacktrackSize &right);

  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  MccBacktrackSize* clone () const { return new MccBacktrackSize (*this); }
    
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
  virtual void Accept (MccVisitor *visitor) = 0;

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
   * The file name.  It is usually in C form.
   */
  char *name;  
  
protected:
  
  // LIFECYCLE ------------------------------------------------------------

  /**
   * Initializes the object.  It should not be used.
   */
  MccFilePdbInput () { }

public:

  /**
   * Initializes the object with a name.
   * @param n the name of the input file(s).
   */
  MccFilePdbInput (char *n) : name (n) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccFilePdbInput (const MccFilePdbInput &right);

  /**
   * Copies the object.
   * @return a clone of itself.
   */
  virtual MccFilePdbInput* clone () const
  { return new MccFilePdbInput (*this); }

  /**
   * Destroys the object.
   */
  virtual ~MccFilePdbInput () { delete[] name; }

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
  virtual void Accept (MccVisitor *visitor);

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
   * The file name.  It is usually in C form.
   */
  char *name;  
  
protected:
  
  // LIFECYCLE ------------------------------------------------------------

  /**
   * Initializes the object.  It should not be used.
   */
  MccFileBinaryInput () { }

public:

  /**
   * Initializes the object with a name.
   * @param n the name of the input file(s).
   */
  MccFileBinaryInput (char *n) : name (n) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccFileBinaryInput (const MccFileBinaryInput &right);

  /**
   * Copies the object.
   * @return a clone of itself.
   */
  virtual MccFileBinaryInput* clone () const
  { return new MccFileBinaryInput (*this); }

  /**
   * Destroys the object.
   */
  virtual ~MccFileBinaryInput () { delete[] name; }

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
  virtual void Accept (MccVisitor *visitor);

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
  char *serverName;

  /**
   * The server port number.
   */
  int port;
  
  /**
   * The file name.  It is usually in C form.
   */
  char *modelName;  
  
protected:
  
  // LIFECYCLE ------------------------------------------------------------

  /**
   * Initializes the object.  It should not be used.
   */
  MccSocketBinaryInput () { }

public:

  /**
   * Initializes the object with a name.
   * @param sn the server name.
   * @param p the port number.
   * @param mn the name of the input models.
   */
  MccSocketBinaryInput (char *sn, int p, char *mn)
    : serverName (sn), port (p), modelName (mn) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccSocketBinaryInput (const MccSocketBinaryInput &right);

  /**
   * Copies the object.
   * @return a clone of itself.
   */
  virtual MccSocketBinaryInput* clone () const
  { return new MccSocketBinaryInput (*this); }

  /**
   * Destroys the object.
   */
  virtual ~MccSocketBinaryInput () { delete[] serverName; delete[] modelName; }

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
  virtual void Accept (MccVisitor *visitor);

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
 * @short Struct representing the AST node "explore".
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccExploreStat : public MccPStruct
{
  /**
   * The FG struct to explore.
   */
  MccFragGenStruc *fg_struc;

  /**
   * The model cache.
   */
  MccModelFilterStrategy *filter;

  /**
   * The explore output file structure.
   */
  MccOutputMode *expOutput;
  
protected:
  
  // LIFECYCLE ------------------------------------------------------------

  /**
   * Initializes the object.  It should never be used.
   */
  MccExploreStat () { }
  
public:

  /**
   * Initializes the object.
   * @param fg the FG struct to explore.
   * @param mc the model cache.
   * @param ef the explore output file structure.
   */
  MccExploreStat (MccFragGenStruc* fg, MccModelFilterStrategy *f,
		  MccOutputMode *ef)
    : fg_struc (fg), filter (f), expOutput (ef) { }

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
  virtual ~MccExploreStat ()
  {
    delete fg_struc;
    if (filter)
      delete filter;
    if (expOutput)
      delete expOutput;
  }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
   * @param right the object to copy.
   * @return itself.
   */
  virtual MccExploreStat& operator= (const MccExploreStat &right);
  
  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void Accept (MccVisitor *visitor);

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
 * @short Struct representing the AST node "exploreLV".
 *
 * @author Philippe Thibault <thibaup@iro.umontreal.ca>
 */
struct MccExploreLVStat : public MccPStruct
{
  /**
   * The FG struct to explore.
   */
  MccFragGenStruc *fg_struc;

  /**
   * The model cache.
   */
  MccModelFilterStrategy *filter;

  /**
   * The explore output file structure.
   */
  MccOutputMode *expOutput;

  /**
   * vector of time limits (sec)
   */
  vector< int > *vtlimits;

  /**
   * Exploration time limit (sec)
   */
  int tlimit;

  /**
   * Size of the backtrack phase
   */
  MccBacktrackSize *btsize;

protected:
  
  // LIFECYCLE ------------------------------------------------------------

  /**
   * Initializes the object.  It should never be used.
   */
  MccExploreLVStat () : MccPStruct () { }
  
public:

  /**
   * Initializes the object.
   * @param fg the FG struct to explore.
   * @param mc the model cache.
   * @param ef the output mode structure.
   * @param bs size of the backtrack phase
   * @param tl exploration time limits
   */
  MccExploreLVStat (MccFragGenStruc *fg,
		    MccModelFilterStrategy *f,
		    MccOutputMode *ef, 
		    vector< int > *tl,
		    MccBacktrackSize *bs);

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccExploreLVStat (const MccExploreLVStat &right);

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccExploreLVStat* clone () const
  { return new MccExploreLVStat (*this); }
    
  /**
   * Destructs the object.
   */
  virtual ~MccExploreLVStat () 
  {
    delete fg_struc;
    if (filter)
      delete filter;
    if (expOutput)
      delete expOutput; 
    if (btsize)
      delete btsize;
    if (vtlimits)
      delete vtlimits;
  }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
   * @param right the object to copy.
   * @return itself.
   */
  virtual MccExploreLVStat& operator= (const MccExploreLVStat &right);
  
  // ACCESS ---------------------------------------------------------------
 
  // METHODS --------------------------------------------------------------
  
  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void Accept (MccVisitor *visitor);

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
 * The struct contains a local hierarchy of statements: _LibStruc (the
 * parent), _StripStruc (for the "strip" substatement) and _ChangeIdStruc
 * (for the "change_id" substatement).
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccLibraryExpr : public MccFGExp
{
  /**
   * @short Parent struct for the MccLibraryExpr sub-structures.
   *
   * @author Martin Larose <larosem@iro.umontreal.ca>
   */
  struct _LibStruc
  {
    /**
     * The residue name vector.
     */
    vector< MccResidueName* > *res_vec;

    /**
     * The chain id to change.
     */
    char from;

    /**
     * The new chain id.
     */
    char to;
    

    // LIFECYCLE ------------------------------------------------------------

  protected:
    /**
     * Initializes the object.  It should never be used.
     */
    _LibStruc () : res_vec (0) { }
  public:

    /**
     * Initializes the object.
     * @param rv the residue name vector.
     * @param f the chain id to change.
     * @param t the new chain id.
     */
    _LibStruc (vector< MccResidueName* > *rv, char f, char t)
      : res_vec (rv), from (f), to (t) { }

    /**
     * Initializes the object with the rights content.
     * @param right the object to copy.
     */
    _LibStruc (const _LibStruc &right);

    /**
     * Destroys the object.  The destruction is controlled by the children
     * of the structure.
     */
    virtual ~_LibStruc ();

    // OPERATORS ------------------------------------------------------------

    /**
     * Assigns the rights content into the object.
     * @param right the object to copy.
     * @return itself.
     */
    virtual _LibStruc& operator= (const _LibStruc &right);
  
    // ACCESS ---------------------------------------------------------------
    
    // METHODS --------------------------------------------------------------

    /**
     * Replicates the object.  The replication is controlled by the children.
     * @return a copy of the current object.
     */
    virtual _LibStruc* clone () const = 0;
    
    /**
     * Accepts the visitor and calls it on itself.
     * @param visitor the visitor.
     */
    virtual void Accept (MccVisitor *visitor) = 0;

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
   * @short Struct containing the residue names to strip.
   *
   * @author Martin Larose <larosem@iro.umontreal.ca>
   */
  struct _StripStruc : public _LibStruc
  {
    // LIFECYCLE ------------------------------------------------------------

  private:
    /**
     * Initializes the object.  It should never be used.
     */
    _StripStruc () : _LibStruc () { }
  public:

    /**
     * Initializes the object.
     * @param rv the residue name vector
     */
    _StripStruc (vector< MccResidueName* > *rv) : _LibStruc (rv, 0, 0) { }

    /**
     * Initializes the object with the rights content.
     * @param right the object to copy.
     */
    _StripStruc (const _StripStruc &right) : _LibStruc (right) { }

    // OPERATORS ------------------------------------------------------------

    /**
     * Assigns the rights content into the object.
     * @param right the object to copy.
     * @return itself.
     */
    virtual _StripStruc& operator= (const _StripStruc &right);
  
    // ACCESS ---------------------------------------------------------------
    
    // METHODS --------------------------------------------------------------

    /**
     * Replicates the object.
     * @return a copy of the current object.
     */
    virtual _StripStruc* clone () const { return new _StripStruc (*this); }
    
    /**
     * Accepts the visitor and calls it on itself.
     * @param visitor the visitor.
     */
    virtual void Accept (MccVisitor *visitor);

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
   * @short Struct containing the chain id to change.
   *
   * @author Martin Larose <larosem@iro.umontreal.ca>
   */
  struct _ChangeIdStruc : public _LibStruc
  {
    // LIFECYCLE ------------------------------------------------------------

  private:
    /**
     * Initializes the object.  It should never be used.
     */
    _ChangeIdStruc () : _LibStruc () { }
  public:

    /**
     * Initializes the object.
     * @param f the chain id to change.
     * @param t the new chain id.
     */
    _ChangeIdStruc (char f, char t) : _LibStruc (0, f, t) { }

    /**
     * Initializes the object with the rights content.
     * @param right the object to copy.
     */
    _ChangeIdStruc (const _ChangeIdStruc &right) : _LibStruc (right) { }

    // OPERATORS ------------------------------------------------------------

    /**
     * Assigns the rights content into the object.
     * @param right the object to copy.
     * @return itself.
     */
    virtual _ChangeIdStruc& operator= (const _ChangeIdStruc &right);
  
    // ACCESS ---------------------------------------------------------------
    
    // METHODS --------------------------------------------------------------

    /**
     * Replicates the object.
     * @return a copy of the current object.
     */
    virtual _ChangeIdStruc* clone () const { return new _ChangeIdStruc (*this); }
    
    /**
     * Accepts the visitor and calls it on itself.
     * @param visitor the visitor.
     */
    virtual void Accept (MccVisitor *visitor);

    // I/O  -----------------------------------------------------------------
    
    /**
     * Displays the structure.
     * @param os the output stream where the message is displayed.
     */
    virtual void display (ostream &os) const
    { os << "change_id (\"" << from << "\" , \"" << to << "\")"; }

    /**
     * Displays the script in human readable form.
     * @param os the output stream used.
     * @param ident the identation level.
     */
    virtual void ppdisplay (ostream &os, int indent = 0) const;
  };

  /**
   * The input model mode.
   */
  MccInputMode *inputMode;

  /**
   *  The model sorted cache.
   */
  MccModelSorterStrategy *sorter;

  /**
   * The vector containing the sub-structures.
   */
  vector< _LibStruc* > *strucs;

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
  MccLibraryExpr (MccInputMode *im, MccModelSorterStrategy *mss, 
		  vector< _LibStruc* > *lsv)
    : inputMode (im), sorter (mss), strucs (lsv) { }

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
  virtual MccLibraryExpr& operator= (const MccLibraryExpr &right);
  
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
  virtual void Accept (MccVisitor *visitor);

  /**
   * Generates a new strip sub-structure and puts it in the vector.
   * @param rv the residue name vector.
   */
  void GenStripStruc (vector< MccResidueName* > *rv)
  { strucs->push_back (new _StripStruc (rv)); }

  /**
   * Generates a new ChangeId sub-structure and puts it in the vector.
   * @param f the chain id to change.
   * @param t the new chain id.
   */
  void GenChangeIdStruc (char f, char t)
  { strucs->push_back (new _ChangeIdStruc (f, t)); }
  
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
 * @short Struct representing the AST node "moses".
 *
 * @author Philippe Thibault <thibaup@iro.umontreal.ca>
 */
struct MccMosesExpr : public MccFGExp
{
  /**
   * Residue range in the sequence in CResIdSet format
   */
  char* range;

  /**
   * CT file id to load
   */
  int ctid;

  /**
   * Queries expression used to override moses' default.
   */
  MccMosesQueries* queries;

  /**
   * mfold energy parameter (%)
   */
  int mfold_epc;

  /**
   * mfold window length
   */
  int mfold_win;
  
  /**
   * Directory in current directory where all ouput files from mfold will be placed.
   */
  char* mfold_dir;

protected:
  
  // LIFECYCLE ------------------------------------------------------------

  /**
   * Initializes the object.  It should never be used.
   */
  MccMosesExpr () { }
  
public:

  /**
   * Initializes the object.
   * @param r the residue range defining the sequence.
   * @param cid the CT file id to load.
   * @param epc the energy parameter in mFold.
   * @param win the window length in mFold.
   */
  MccMosesExpr (char *r, int cid, MccMosesQueries* q, int epc, int win, char* dir);

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccMosesExpr (const MccMosesExpr &right);

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccMosesExpr* clone () const { return new MccMosesExpr (*this); }
    
  /**
   * Destroys the object.
   */
  virtual ~MccMosesExpr ();

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
   * @param right the object to copy.
   * @return itself.
   */
  virtual MccMosesExpr& operator= (const MccMosesExpr &right);
  
  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void Accept (MccVisitor *visitor);

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
 * @short Struct representing the "new_tag" statement.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccNewTagStat : public MccPStruct
{
  /**
   * The boolean indicating if the tag applies to a residue label or a
   * tranfo label.
   */
  bool resq_opt;

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
   * @param o the boolean indicating if the tag applies to a residue label.
   * @param i the name of the tag.
   * @param e the query expression structure.
   */
  MccNewTagStat (bool o, char *i, vector< MccQueryExpr* > *e)
    : resq_opt (o), id (i), exprs (e) { }

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
  virtual MccNewTagStat& operator= (const MccNewTagStat &right);
  
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
  virtual void Accept (MccVisitor *visitor);

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
  virtual MccNoteStat& operator= (const MccNoteStat &right);
  
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
  virtual void Accept (MccVisitor *visitor);

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
  virtual MccNotesStat& operator= (const MccNotesStat &right)
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
  virtual void Accept (MccVisitor *visitor);

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
 * @short Struct representing the AST node "pair" statement.
 *
 * The struct contains a local substatement: _PairStruc that contains each
 * pairing description.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccPairStat : public MccPStruct
{
  /**
   * @short Sub-structure containing the pairing informations.
   *
   * @author Martin Larose <larosem@iro.umontreal.ca>
   */
  struct _PairStruc
  {
    /**
     * The name of the first residue in the relation.
     */
    MccResidueName *res1;

    /**
     * The name of the second residue in the relation.
     */
    MccResidueName *res2;

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
    _PairStruc () { }
    
  public:

    /**
     * Initializes the object.
     * @param r1 the name of the first residue.
     * @param r2 the name of the second residue.
     * @param e the query expression structure.
     * @param s the sampling size structure.
     */
    _PairStruc (MccResidueName *r1, 
		MccResidueName *r2, MccQueryExpr *e, MccSamplingSize *s)
      : res1 (r1), res2 (r2), expr (e), ssize (s) { }

    /**
     * Initializes the object with the rights content.
     * @param right the object to copy.
     */
    _PairStruc (const _PairStruc &right);

    /**
     * Destroys the object.
     */
    ~_PairStruc () { delete res1; delete res2; delete expr; delete ssize; }

    // OPERATORS ------------------------------------------------------------

    /**
     * Assigns the rights content into the object.
     * @param right the object to copy.
     * @return itself.
     */
    _PairStruc& operator= (const _PairStruc &right);
  
    // ACCESS ---------------------------------------------------------------
    
    // METHODS --------------------------------------------------------------

    /**
     * Replicates the object.
     * @return a copy of the current object.
     */
    _PairStruc* clone () const { return new _PairStruc (*this); }
    
    /**
     * Accepts the visitor and calls it on itself.
     * @param visitor the visitor.
     */
    void Accept (MccVisitor *visitor);

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
  vector< _PairStruc* > *strucs;


  // LIFECYCLE ------------------------------------------------------------
    
  /**
   * Initializes the object.
   */
  MccPairStat () : strucs (new vector< _PairStruc* > ()) { }

  /**
   * Initializes the object.
   * @param psv the vector containing the pairing sub-structures.
   */
  MccPairStat (vector< _PairStruc* > *psv) : strucs (psv) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccPairStat (const MccPairStat &right);

  /**
   * Destroys the object.
   */
  virtual ~MccPairStat ();

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
   * @param right the object to copy.
   * @return itself.
   */
  virtual MccPairStat& operator= (const MccPairStat &right);
  
  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
   virtual MccPairStat* clone () const { return new MccPairStat (*this); }
    
  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void Accept (MccVisitor *visitor);

  /**
   * Generates a new pairing sub-structures and puts it in the vector.
   * @param r1 the name of the first residue.
   * @param r2 the name of the second residue.
   * @param e the query expression structure.
   * @param s the sampling size structure.
   */   
  void GenPairStruc (MccResidueName *r1, MccResidueName *r2,
		     MccQueryExpr *e, MccSamplingSize *s)
  { strucs->push_back (new _PairStruc (r1, r2, e, s)); }
  
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
  virtual MccQuitStat& operator= (const MccQuitStat &right) { return *this; }
  
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
  virtual void Accept (MccVisitor *visitor);

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
 * @short Struct representing the "relation" statement.
 *
 * This statement generate constraints between two residues related together
 * by properties.  This struct owns a local struct _RelationStruc
 * that contains the informations on the constraints.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccRelationCstStat : public MccPStruct
{
  /**
   * @short Sub-struct containing a relation constraint description.
   *
   * @author Martin Larose <larosem@iro.umontreal.ca>
   */
  struct _RelationStruc
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
    _RelationStruc () { }

  public:

    /**
     * Initializes the object.
     * @param r1 the first residue name.
     * @param r2 the second residue name.
     * @param e the query expression.
     */
    _RelationStruc (MccResidueName *r1, MccResidueName *r2, MccQueryExpr *e)
      : ref (r1), res (r2), expr (e)
    { }

    /**
     * Initializes the object with the rights content.
     * @param right the object to copy.
     */
    _RelationStruc (const _RelationStruc &right);

    /**
     * Replicates the object.
     * @return a copy of the current object.
     */
    _RelationStruc* clone () const { return new _RelationStruc (*this); }

    /**
     * Destroys the residue names and atom names.
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
     * Accepts the visitor and calls it on itself.
     * @param visitor the visitor.
     */
    void Accept (MccVisitor *visitor);
    
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
   * The vector of _RelationStruc.
   */
  vector< _RelationStruc* > *strucs;

  
  // LIFECYCLE ------------------------------------------------------------

  /**
   * Initializes the object.
   */
  MccRelationCstStat () : strucs (new vector< _RelationStruc* > ()) { }

  /**
   * Initializes the object.
   * @param asv the relation sub-structure vector.
   */
  MccRelationCstStat (vector< _RelationStruc* > *asv) : strucs (asv) { }
  
  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccRelationCstStat (const MccRelationCstStat &right);
  
  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccRelationCstStat* clone () const
  { return new MccRelationCstStat (*this); }

  /**
   * Destroys the vector of sub-structures.
   */
  virtual ~MccRelationCstStat ();

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
   * @param right the object to copy.
   * @return itself.
   */
  virtual MccRelationCstStat& operator= (const MccRelationCstStat &right);

  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void Accept (MccVisitor *visitor);

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
  virtual MccRemarkStat& operator= (const MccRemarkStat &right);
  
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
  virtual void Accept (MccVisitor *visitor);

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
 * @short Struct representing the "reset_db" statement in AST form.
 *
 * This statement resets the database file.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccResetDBStat : public MccPStruct
{
  // LIFECYCLE ------------------------------------------------------------
  
  /**
   * Initializes the object.
   */
  MccResetDBStat () { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccResetDBStat (const MccResetDBStat &right) { }

  /**
   * Destroys the object.
   */
  virtual ~MccResetDBStat () { }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
   * @param right the object to copy.
   * @return itself.
   */
  virtual MccResetDBStat& operator= (const MccResetDBStat &right)
  { return *this; }
  
  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccResetDBStat* clone () const { return new MccResetDBStat (*this); }
    
  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void Accept (MccVisitor *visitor);

  // I/O  -----------------------------------------------------------------
  
  /**
   * Displays the structure.
   * @param os the output stream where the message is displayed.
   */
  virtual void display (ostream &os) const { os << "reset_db"; }

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
  virtual MccResetStat& operator= (const MccResetStat &right)
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
  virtual void Accept (MccVisitor *visitor);

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
 * @short Struct representing the AST node of the "residue" statement.
 *
 * The struct contains a local struct: _ResidueStruc that contains each
 * residue description.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccResidueStat : public MccPStruct
{
  /**
   * @short Sub-structure containing the residue informations.
   *
   * @author Martin Larose <larosem@iro.umontreal.ca>
   */
  struct _ResidueStruc
  {
    /**
     * The first residue of the list.
     */
    MccResidueName *res1;

    /**
     * The last residue of the list.
     */
    MccResidueName *res2;

    /**
     * The query expression struct.
     */
    MccQueryExpr *expr;

    /**
     * The sampling size structure.
     */
    MccSamplingSize *ssize;

    //! theo confo
//     bool theo_flag;
    //!

  protected:
    
    // LIFECYCLE ------------------------------------------------------------

    /**
     * Initializes the object.  It should never be used.
     */
    _ResidueStruc () { }
    
  public:

    /**
     * Initializes the object.
     * @param r1 the first residue of the list.
     * @param r2 the last residue of the list.
     * @param exp the query expression struct.
     * @param ss the sampling size struct.
     */
    _ResidueStruc (MccResidueName *r1, MccResidueName *r2,
		   MccQueryExpr *exp, MccSamplingSize *ss)
      : res1 (r1), res2 (r2), expr (exp), ssize (ss)
      { }

    //! theo confo
//     _ResidueStruc (MccResidueName *r1, MccResidueName *r2,
// 		   MccQueryExpr *exp, MccSamplingSize *ss, bool tf = false)
//       : res1 (r1), res2 (r2), expr (exp), ssize (ss), theo_flag (tf) 
//       { }
    //!

    /**
     * Initializes the object with the rights content.
     * @param right the object to copy.
     */
    _ResidueStruc (const _ResidueStruc &right);

    /**
     * Destroys the object.
     */
    ~_ResidueStruc ()
    { delete res1; if (res2) delete res2; delete expr; delete ssize; }

    

    // OPERATORS ------------------------------------------------------------

    /**
     * Assigns the rights content into the object.
     * @param right the object to copy.
     * @return itself.
     */
    _ResidueStruc& operator= (const _ResidueStruc &right);
  
    // ACCESS ---------------------------------------------------------------
    
    // METHODS --------------------------------------------------------------

    /**
     * Replicates the object.
     * @return a copy of the current object.
     */
    _ResidueStruc* clone () const { return new _ResidueStruc (*this); }
    
    /**
     * Accepts the visitor and calls it on itself.
     * @param visitor the visitor.
     */
    void Accept (MccVisitor *visitor);

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
   * The vector of residue sub-structures.
   */
  vector< _ResidueStruc* > *strucs;


  // LIFECYCLE ------------------------------------------------------------
  
  /**
   * Initializes the object.
   */
  MccResidueStat () : strucs (new vector< _ResidueStruc* > ()) { }

  /**
   * Initializes the object.
   * @param rsv the vector of residue sub-structures.
   */
  MccResidueStat (vector< _ResidueStruc* > *rsv) : strucs (rsv) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccResidueStat (const MccResidueStat &right);

  /**
   * Destroys the object.  Clears the vector of sub-structures.
   */
  virtual ~MccResidueStat ();

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
   * @param right the object to copy.
   * @return itself.
   */
  virtual MccResidueStat& operator= (const MccResidueStat &right);
  
  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccResidueStat* clone () const { return new MccResidueStat (*this); }
    
  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void Accept (MccVisitor *visitor);

  /**
   * Generates a new residue sub-structure and puts it in the vector.
   * @param r1 the first residue of the list.
   * @param r2 the last residue of the list.
   * @param exp the query expression struct.
   * @param ss the sampling size struct.
   */   
  void GenResidueStruc (MccResidueName *r1, MccResidueName *r2,
			MccQueryExpr *exp, MccSamplingSize *ss)
  { strucs->push_back (new _ResidueStruc (r1, r2, exp, ss)); }
  
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
  virtual MccRestoreStat& operator= (const MccRestoreStat &right);

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
  virtual void Accept (MccVisitor *visitor);

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
 * @short Struct representing the rmsd filter cache.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccRmsdModelFilterStrategy : public MccModelFilterStrategy
{
  /**
   * The rms bound.
   */
  float rmsBound;

  /**
   * The align flag.
   */
  bool alignFlag;

  /**
   * The set of atoms used to calculate the rms.
   */
  MccAS atom_set;

  /**
   * The options on the atom set.
   */
  MccAS atom_set_opt;
  
  // LIFECYCLE ------------------------------------------------------------

protected:
  
  /**
   * Initializes the object.  It should never be used.
   */
  MccRmsdModelFilterStrategy () { }
  
public:

  /**
   * Initializes the object.
   * @param rb the rms bound.
   * @param af the align flag.
   * @param as the atom filter.
   * @param aso the atom filter option.
   */
  MccRmsdModelFilterStrategy (float rb, bool af, MccAS as, MccAS aso)
    : rmsBound (rb), alignFlag (af), atom_set (as), atom_set_opt (aso) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccRmsdModelFilterStrategy (const MccRmsdModelFilterStrategy &right)
    : rmsBound (right.rmsBound),
      alignFlag (right.alignFlag),
      atom_set (right.atom_set),
      atom_set_opt (right.atom_set_opt)
  { }

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccModelFilterStrategy* clone () const { return new MccRmsdModelFilterStrategy (*this); }
    
  /**
   * Destroys the object.
   */
  virtual ~MccRmsdModelFilterStrategy () { }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
   * @param right the object to copy.
   * @return itself.
   */
  MccRmsdModelFilterStrategy& operator= (const MccRmsdModelFilterStrategy &right);
  
  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void Accept (MccVisitor *visitor);

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
 * @short Struct representing the clustered sorter cache.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccClusteredModelSorterStrategy : public MccModelSorterStrategy
{
  /**
   * The number of partitions.
   */
  int partitions;

  /**
   * The rms bound.
   */
  float rmsBound;

  /**
   * The set of atoms used to calculate the rms.
   */
  MccAS atom_set;

  /**
   * The options on the atom set.
   */
  MccAS atom_set_opt;
  
  // LIFECYCLE ------------------------------------------------------------

protected:
  
  /**
   * Initializes the object.  It should never be used.
   */
  MccClusteredModelSorterStrategy () { }
  
public:

  /**
   * Initializes the object.
   * @param part the number of partitions.
   * @param af the align flag.
   * @param as the atom filter.
   * @param aso the atom filter option.
   */
  MccClusteredModelSorterStrategy (int part, MccAS as, MccAS aso)
    : partitions (part), rmsBound (-1), atom_set (as), atom_set_opt (aso) { }

  /**
   * Initializes the object.
   * @param rb the rms bound.
   * @param af the align flag.
   * @param as the atom filter.
   * @param aso the atom filter option.
   */
  MccClusteredModelSorterStrategy (float rb, MccAS as, MccAS aso)
    :  partitions (1), rmsBound (rb), atom_set (as), atom_set_opt (aso) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccClusteredModelSorterStrategy (const MccClusteredModelSorterStrategy &right)
    : rmsBound (right.rmsBound),
      atom_set (right.atom_set),
      atom_set_opt (right.atom_set_opt)
  { }

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccModelSorterStrategy* clone () const 
  { return new MccClusteredModelSorterStrategy (*this); }
    
  /**
   * Destroys the object.
   */
  virtual ~MccClusteredModelSorterStrategy () { }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
   * @param right the object to copy.
   * @return itself.
   */
  MccClusteredModelSorterStrategy& operator= (const MccClusteredModelSorterStrategy &right);
  
  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void Accept (MccVisitor *visitor);

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
  char *seq;

protected:

  // LIFECYCLE ------------------------------------------------------------

  /**
   * Initializes the object.  It should never be used.
   */
  MccSequenceStat () { }

public:

  /**
   * Initializes the object.
   * @param typ the type of the defined sequence.
   * @param res the residue name of the first residue in the sequence.
   * @param str the sequence.
   */
  MccSequenceStat (char typ, MccResidueName *r, char *str)
    : type (typ), res (r), seq (str)
  { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccSequenceStat (const MccSequenceStat &right);

  /**
   * Destroys the object.
   */
  virtual ~MccSequenceStat () { delete res; delete[] seq; }

  // OPERATORS -----------------------------------------------------------

  /**
   * Assigns the rights values in the object.
   * @param right the object to copy.
   * @return itself.
   */
  virtual MccSequenceStat& operator= (const MccSequenceStat &right);

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
  virtual void Accept (MccVisitor *visitor);

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
  virtual MccSourceStat& operator= (const MccSourceStat &right);

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
  virtual void Accept (MccVisitor *visitor);

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
     * The minimum torsion angle value (in degrees).
     */
    float min;

    /**
     * The maximum torsion angle value (in degrees).
     */
    float max;
    

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
		   float mn, float mx)
      : res1 (r1), at1 (a1), res2 (r2), at2 (a2),
	res3 (r3), at3 (a3), res4 (r4), at4 (a4),
	min (mn), max (mx)
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
    void Accept (MccVisitor *visitor);

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
   virtual MccTorsionCstStat& operator= (const MccTorsionCstStat &right);

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
  virtual void Accept (MccVisitor *visitor);

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
  virtual MccVersion& operator= (const MccVersion &right) { return *this; }

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
  virtual void Accept (MccVisitor *visitor);

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



//! placer

/**
 * @short Struct representing the AST node for the "placer_sequence" statement.
 *
 * @author Philippe Thibault <thibaup@iro.umontreal.ca>
 */
struct MccPlacerSequenceStat : public MccPStruct
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
  char *seq;

protected:

  // LIFECYCLE ------------------------------------------------------------

  /**
   * Initializes the object.  It should never be used.
   */
  MccPlacerSequenceStat () { }

public:

  /**
   * Initializes the object.
   * @param typ the type of the defined sequence.
   * @param res the residue name of the first residue in the sequence.
   * @param str the sequence.
   */
  MccPlacerSequenceStat (char typ, MccResidueName *r, char *str)
    : type (typ), res (r), seq (str)
  { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccPlacerSequenceStat (const MccPlacerSequenceStat &right);

  /**
   * Destroys the object.
   */
  virtual ~MccPlacerSequenceStat () { delete res; delete[] seq; }

  // OPERATORS -----------------------------------------------------------

  /**
   * Assigns the rights values in the object.
   * @param right the object to copy.
   * @return itself.
   */
  virtual MccPlacerSequenceStat& operator= (const MccPlacerSequenceStat &right);

  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccPlacerSequenceStat* clone () const { return new MccPlacerSequenceStat (*this); }
    
  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void Accept (MccVisitor *visitor);

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
 * @short Struct representing the "placer_connect" AST node.
 *
 * It contains a local struct _PlacerConnectStruc that collects each statement.
 *
 * @author Philippe Thibault <thibaup@iro.umontreal.ca>
 */
struct MccPlacerConnectStat : public MccPStruct
{
  /**
   * @short Sub-structure containing the connect informations.
   *
   * @author Philippe Thibault <thibaup@iro.umontreal.ca>
   */
  struct _PlacerConnectStruc
  {
    /**
     * The name of the first residue in the relation.
     */
    MccResidueName *res1;

    /**
     * The name of the second residue in the relation.
     */
    MccResidueName *res2;

    /**
     * The query expression structure.
     */
    MccQueryExpr *expr;

    /**
     * The sampling size structure.
     */
    MccSamplingSize *ssize;

    
    // LIFECYCLE ------------------------------------------------------------

  private:
    /**
     * Initializes the object.  It should never be used.
     */
    _PlacerConnectStruc () { }
  public:

    /**
     * Initializes the object.
     * @param r1 the name of the first residue in the relation.
     * @param r2 the name of the second residue in the relation.
     * @param e the query expression structure.
     * @param s the sampling size structure.
     */
    _PlacerConnectStruc (MccResidueName *r1, MccResidueName *r2, 
		   MccQueryExpr *e, MccSamplingSize *s)
      : res1 (r1), res2 (r2), expr (e), ssize (s) { }

    /**
     * Initializes the object with the rights content.
     * @param right the object to copy.
     */
    _PlacerConnectStruc (const _PlacerConnectStruc &right);
  
    /**
     * Destroys the object.
     */
    ~_PlacerConnectStruc () { delete res1; delete res2; delete expr; delete ssize; }

    // OPERATORS ------------------------------------------------------------

    /**
     * Assigns the rights content into the object.
     * @param right the object to copy.
     * @return itself.
     */
    _PlacerConnectStruc& operator= (const _PlacerConnectStruc &right);
    
    // ACCESS ---------------------------------------------------------------
    
    // METHODS --------------------------------------------------------------

    /**
     * Replicates the object.
     * @return a copy of the current object.
     */
    _PlacerConnectStruc* clone () const { return new _PlacerConnectStruc (*this); }
    
    /**
     * Accepts the visitor and calls it on itself.
     * @param visitor the visitor.
     */
    void Accept (MccVisitor *visitor);

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
   * The vector containing the connect sub-structures.
   */
  vector< _PlacerConnectStruc* > *strucs;

  
  // LIFECYCLE ------------------------------------------------------------

  /**
   * Initializes the object.
   */
  MccPlacerConnectStat () : strucs (new vector< _PlacerConnectStruc* > ()) { }

  /**
   * Initializes the object.
   * @param csv the vector containing the connect sub-structures.
   */
  MccPlacerConnectStat (vector< _PlacerConnectStruc* > *csv) : strucs (csv) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccPlacerConnectStat (const MccPlacerConnectStat &right);
  
  /**
   * Destroys the object.  Clears the vector of sub-structures.
   */
  virtual ~MccPlacerConnectStat ();

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
   * @param right the object to copy.
   * @return itself.
   */
  virtual MccPlacerConnectStat& operator= (const MccPlacerConnectStat &right);
    
  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccPlacerConnectStat* clone () const { return new MccPlacerConnectStat (*this); }
    
  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void Accept (MccVisitor *visitor);

  /**
   * Generates a new connect sub-structure and puts it in the vector.
   * @param r1 the name of the first residue.
   * @param r2 the name of the second residue.
   * @param e the query expression structure.
   * @param s the sampling size structure.
   */
  void GenPlacerConnectStruc (MccResidueName *r1, MccResidueName *r2,
			MccQueryExpr *e, MccSamplingSize *s)
  { strucs->push_back (new _PlacerConnectStruc (r1, r2, e, s)); }

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
 * @short Struct representing the AST node "placer_pair" statement.
 *
 * The struct contains a local substatement: _PlacerPairStruc that contains each
 * pairing description.
 *
 * @author Philippe Thibault <thibaup@iro.umontreal.ca>
 */
struct MccPlacerPairStat : public MccPStruct
{
  /**
   * @short Sub-structure containing the pairing informations.
   *
   * @author Philippe Thibault <thibaup@iro.umontreal.ca>
   */
  struct _PlacerPairStruc
  {
    /**
     * The name of the first residue in the relation.
     */
    MccResidueName *res1;

    /**
     * The name of the second residue in the relation.
     */
    MccResidueName *res2;

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
    _PlacerPairStruc () { }
    
  public:

    /**
     * Initializes the object.
     * @param r1 the name of the first residue.
     * @param r2 the name of the second residue.
     * @param e the query expression structure.
     * @param s the sampling size structure.
     */
    _PlacerPairStruc (MccResidueName *r1, 
		MccResidueName *r2, MccQueryExpr *e, MccSamplingSize *s)
      : res1 (r1), res2 (r2), expr (e), ssize (s) { }

    /**
     * Initializes the object with the rights content.
     * @param right the object to copy.
     */
    _PlacerPairStruc (const _PlacerPairStruc &right);

    /**
     * Destroys the object.
     */
    ~_PlacerPairStruc () { delete res1; delete res2; delete expr; delete ssize; }

    // OPERATORS ------------------------------------------------------------

    /**
     * Assigns the rights content into the object.
     * @param right the object to copy.
     * @return itself.
     */
    _PlacerPairStruc& operator= (const _PlacerPairStruc &right);
  
    // ACCESS ---------------------------------------------------------------
    
    // METHODS --------------------------------------------------------------

    /**
     * Replicates the object.
     * @return a copy of the current object.
     */
    _PlacerPairStruc* clone () const { return new _PlacerPairStruc (*this); }
    
    /**
     * Accepts the visitor and calls it on itself.
     * @param visitor the visitor.
     */
    void Accept (MccVisitor *visitor);

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
  vector< _PlacerPairStruc* > *strucs;


  // LIFECYCLE ------------------------------------------------------------
    
  /**
   * Initializes the object.
   */
  MccPlacerPairStat () : strucs (new vector< _PlacerPairStruc* > ()) { }

  /**
   * Initializes the object.
   * @param psv the vector containing the pairing sub-structures.
   */
  MccPlacerPairStat (vector< _PlacerPairStruc* > *psv) : strucs (psv) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccPlacerPairStat (const MccPlacerPairStat &right);

  /**
   * Destroys the object.
   */
  virtual ~MccPlacerPairStat ();

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights content into the object.
   * @param right the object to copy.
   * @return itself.
   */
  virtual MccPlacerPairStat& operator= (const MccPlacerPairStat &right);
  
  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
   virtual MccPlacerPairStat* clone () const { return new MccPlacerPairStat (*this); }
    
  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void Accept (MccVisitor *visitor);

  /**
   * Generates a new pairing sub-structures and puts it in the vector.
   * @param r1 the name of the first residue.
   * @param r2 the name of the second residue.
   * @param e the query expression structure.
   * @param s the sampling size structure.
   */   
  void GenPlacerPairStruc (MccResidueName *r1, MccResidueName *r2,
		     MccQueryExpr *e, MccSamplingSize *s)
  { strucs->push_back (new _PlacerPairStruc (r1, r2, e, s)); }
  
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


//!





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
  virtual MccVisitor& operator= (const MccVisitor &right) { return *this; }

  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Visits the MccFragGenStruc structure.
   * @param struc the MccFragGenStruc structure.
   */
  virtual void Visit (MccFragGenStruc *struc) = 0;

  /**
   * Visits the MccResidueName structure.
   * @param struc the MccResidueName structure.
   */
  virtual void Visit (MccResidueName *struc) = 0;

  /**
   * Visits the MccQTrueFunc structure.
   * @param struc the MccQTrueFunc structure.
   */
  virtual void Visit (MccQTrueFunc *struc) = 0;

  /**
   * Visits the MccQIdentFunc structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccQIdentFunc *struc) = 0;

  /**
   * Visits the MccQNotFunc structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccQNotFunc *struc) = 0;

  /**
   * Visits the MccQFaceFunc structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccQFaceFunc *struc) = 0;

  /**
   * Visits the MccQAndFunc structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccQAndFunc *struc) = 0;

  /**
   * Visits the MccQOrFunc structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccQOrFunc *struc) = 0;

  /**
   * Visits the MccQueryExpr structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccQueryExpr *struc) = 0;

  /**
   * Visits the MccAddPdbStat::_AddPdbStruc structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccAddPdbStat::_AddPdbStruc *struc) = 0;

  /**
   * Visits the MccAddPdbStat structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccAddPdbStat *struc) = 0;
  
  /**
   * Visits the MccAdjacencyCstStat structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccAdjacencyCstStat *struc) = 0;
  
  /**
   * Visits the MccAngleCstStat::_AngleStruc structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccAngleCstStat::_AngleStruc *struc) = 0;
  
  /**
   * Visits the MccAngleCstStat structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccAngleCstStat *struc) = 0;
  
  /**
   * Visits the MccAssignStat assignment statement node.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccAssignStat *struc) = 0;

  /**
   * Visits the MccBacktrackExpr::_FGStruc sub-structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccBacktrackExpr::_FGStruc *struc) = 0;
  
  /**
   * Visits the MccBacktrackExpr::_BTStruc sub-structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccBacktrackExpr::_BTStruc *struc) = 0;

  /**
   * Visits the MccBacktrackExpr::_PlaceStruc sub-structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccBacktrackExpr::_PlaceStruc *struc) = 0;
  
  /**
   * Visits the MccBacktrackExpr structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccBacktrackExpr *struc) = 0;
  
  /**
   * Visits the MccCacheExpr structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccCacheExpr *struc) = 0;
  
  /**
   * Visits the MccClashCstStat structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccClashCstStat *struc) = 0;
  
  /**
   * Visits the MccClusteredModelSorterStrategy structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccClusteredModelSorterStrategy *struc) = 0;

  /**
   * Visits the MccConnectStat::_ConnectStruc sub-structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccConnectStat::_ConnectStruc *struc) = 0;
  
  /**
   * Visits the MccConnectStat structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccConnectStat *struc) = 0;
  
  /**
   * Visits the MccCycleCstStat::_CycleStruc sub-structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccCycleCstStat::_CycleStruc *struc) = 0;
  
  /**
   * Visits the MccCycleCstStat structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccCycleCstStat *struc) = 0;
  
  /**
   * Visits the MccDisplayFGStat structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccDisplayFGStat *struc) = 0;

  /**
   * Visits the MccDistCstStat::_DistStruc sub-structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccDistCstStat::_DistStruc *struc) = 0;

  /**
   * Visits the MccDistCstStat structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccDistCstStat *struc) = 0;
  
  /**
   * Visits the MccFilePdbOutput structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccFilePdbOutput *struc) = 0;
  
  /**
   * Visits the MccFileBinaryOutput structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccFileBinaryOutput *struc) = 0;
  
  /**
   * Visits the MccSocketBinaryOutput structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccSocketBinaryOutput *struc) = 0;
  
  /**
   * Visits the MccFilePdbInput structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccFilePdbInput *struc) = 0;
  
  /**
   * Visits the MccFileBinaryInput structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccFileBinaryInput *struc) = 0;
  
  /**
   * Visits the MccSocketBinaryInput structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccSocketBinaryInput *struc) = 0;
  
  /**
   * Visits the MccExploreStat structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccExploreStat *struc) = 0;
  
  /**
   * Visits the MccExploreLVStat structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccExploreLVStat *struc) = 0;
  
  /**
   * Visits the local MccLibraryExpr sub-structure _StripStruc.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccLibraryExpr::_StripStruc *struc) = 0;
  
  /**
   * Visits the local MccLibraryExpr sub-structure _ChangeIdStruc.
   * @param struc the evaluated structure.
   */
    virtual void Visit (MccLibraryExpr::_ChangeIdStruc *struc) = 0;
  
  /**
   * Visits the MccCycleCstStat structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccLibraryExpr *struc) = 0;

  /**
   * Visits the MccMosesExpr structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccMosesExpr *struc) = 0;

  /**
   * Visits the MccMosesQueries structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccMosesQueries *struc) = 0;

  /**
   * Visits the MccNewTagStat structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccNewTagStat *struc) = 0;

  /**
   * Visits the MccNoteStat structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccNoteStat *struc) = 0;
  
  /**
   * Visits the MccNotesStat structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccNotesStat *struc) = 0;
  
  /**
   * Visits the local MccPairStat sub-structure _PairStruc.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccPairStat::_PairStruc *struc) = 0;
  
  /**
   * Visits the MccPairStat structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccPairStat *struc) = 0;
  
  /**
   * Visits the MccQuitStat structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccQuitStat *struc) = 0;

  /**
   * Visits the MccRelationCstStat::_RelationStruc structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccRelationCstStat::_RelationStruc *struc) = 0;
  
  /**
   * Visits the MccRelationCstStat structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccRelationCstStat *struc) = 0;
  
  /**
   * Visits the MccRemarkStat structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccRemarkStat *struc) = 0;

  /**
   * Visits the MccResetDBStat structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccResetDBStat *struc) = 0;
  
  /**
   * Visits the MccResetStat structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccResetStat *struc) = 0;

  /**
   * Visits the MccResidueStat local sub-structure _ResidueStruc.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccResidueStat::_ResidueStruc *struc) = 0;
  
  /**
   * Visits the MccResidueStat structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccResidueStat *struc) = 0;
  
  /**
   * Visits the MccRestoreStat structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccRestoreStat *struc) = 0;
  
  /**
   * Visits the MccRmsdModelFilterStrategy structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccRmsdModelFilterStrategy *struc) = 0;
  
  /**
   * Visits the MccSamplingSize structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccSamplingSize *struc) = 0;

  /**
   * Visits the MccSequenceStat structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccSequenceStat *struc) = 0;

  /**
   * Visits the MccSourceStat structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccSourceStat *struc) = 0;
  
  /**
   * Visits the MccTorsionCstStat local sub-structure _TorsionStruc.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccTorsionCstStat::_TorsionStruc *struc) = 0;
  
  /**
   * Visits the MccTorsionCstStat structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccTorsionCstStat *struc) = 0;

  /**
   * Visits the MccVersion structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccVersion *struc) = 0;

  //! placer

  /**
   * Visits the MccPlacerSequenceStat structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccPlacerSequenceStat *struc) = 0;
  
  /**
   * Visits the local MccPlacerConnectStat sub-structure _PlacerConnectStruc.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccPlacerConnectStat::_PlacerConnectStruc *struc) = 0;
  
  /**
   * Visits the MccPlacerConnectStat structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccPlacerConnectStat *struc) = 0;
    /**
   * Visits the local MccPlacerPairStat sub-structure _PlacerPairStruc.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccPlacerPairStat::_PlacerPairStruc *struc) = 0;
  
  /**
   * Visits the MccPlacerPairStat structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccPlacerPairStat *struc) = 0;
  
  //!

  
  // I/O  -----------------------------------------------------------------

};



/**
 * @short Exception thrown by the lexer.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
class CLexerException
{
  /**
   * The message.
   */
  char *str;

  /**
   * Initializes the object.  It should never be used.
   */
  CLexerException () { }

public:

  // LIFECYCLE ------------------------------------------------------------

  /**
   * Initializes the object.
   * @param s the message.
   */
  CLexerException (const char *s);

  /**
   * Initializes the object with the rights contents.
   * @param right the object to copy.
   */
  CLexerException (const CLexerException &right);

  /**
   * Destroys the object.
   */
  ~CLexerException () { delete[] str; }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights contents in the object.
   * @param right the object to copy.
   * @return itself.
   */
  CLexerException& operator= (const CLexerException &right);

  /**
   * Concatenates a character in the message.
   * @param c the character to concatenate.
   * @return itself.
   */
  CLexerException& operator<< (char c);

  /**
   * Concatenates an integer in the message.
   * @param n the integer to concatenate.
   * @return itself.
   */
  CLexerException& operator<< (int n);

  /**
   * Concatenates a float in the message.
   * @param x the float to concatenate.
   * @return itself.
   */
  CLexerException& operator<< (float x);

  /**
   * Concatenates a string in the message.
   * @param s the string to concatenate.
   * @return itself.
   */
  CLexerException& operator<< (const char *s);

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
ostream& operator<< (ostream &os, const CLexerException &ex);



/**
 * @short Exception thrown by the parser.
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
class CParserException
{
  /**
   * The message.
   */
  char *str;

  /**
   * Initializes the object.  It should never be used.
   */
  CParserException () { }

public:

  // LIFECYCLE ------------------------------------------------------------

  /**
   * Initializes the object.
   * @param s the message.
   */
  CParserException (const char *s);

  /**
   * Initializes the object with the rights contents.
   * @param right the object to copy.
   */
  CParserException (const CParserException &right);

  /**
   * Destroys the object.
   */
  ~CParserException () { delete[] str; }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the rights contents in the object.
   * @param right the object to copy.
   * @return itself.
   */
  CParserException& operator= (const CParserException &right);

  /**
   * Concatenates a character in the message.
   * @param c the character to concatenate.
   * @return itself.
   */
  CParserException& operator<< (char c);

  /**
   * Concatenates an integer in the message.
   * @param n the integer to concatenate.
   * @return itself.
   */
  CParserException& operator<< (int n);

  /**
   * Concatenates a float in the message.
   * @param x the float to concatenate.
   * @return itself.
   */
  CParserException& operator<< (float x);

  /**
   * Concatenates a string in the message.
   * @param s the string to concatenate.
   * @return itself.
   */
  CParserException& operator<< (const char *s);

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
ostream& operator<< (ostream &os, const CParserException &ex);


#endif
