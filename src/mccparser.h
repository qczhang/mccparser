//                              -*- Mode: C++ -*- 
// mccparser.h
// Copyright © 2000-01 Laboratoire de Biologie Informatique et Théorique.
// Author           : Martin Larose
// Created On       : Thu Aug 24 12:14:42 2000
// Last Modified By : Martin Larose
// Last Modified On : Fri Feb  9 10:51:42 2001
// Update Count     : 14
// Status           : Ok.
// 


#ifndef _mccparser_h_
#define _mccparser_h_


#include <iostream.h>
#include <vector.h>
#include <stdio.h>



typedef struct yy_buffer_state *YY_BUFFER_STATE;
YY_BUFFER_STATE mcc_scan_string (const char*);
void mcc_delete_buffer (YY_BUFFER_STATE);
void mcc_switch_to_buffer (YY_BUFFER_STATE);

class MccPStruct;
class MccVisitor;



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
   * Destructs the object.
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
   * Destructs the structure.  Nothing to do.
   */
  virtual ~MccPStruct () { }

  //OPERATORS -------------------------------------------------------------

  /**
   * Assigns the rights content in the object.
   * @param right the object to copy.
   * @return itself.
   */
  virtual MccPStruct& operator= (const MccPStruct &right) { return *this; }

  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccPStruct* Copy () const = 0;

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
   * Destructs the structure.  Nothing to do.
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
  virtual MccFGExp* Copy () const = 0;
  
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
   * Destructs the object.
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
   * Replicates the object.
   * @return a copy of the current object.
   */
  MccFragGenStruc* Copy () const { return new MccFragGenStruc (*this); }

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
   * Destructs the object.
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
  MccResidueName* Copy () const { return new MccResidueName (*this); }

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
   * Destructs the object.
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
  virtual MccQFunc* Copy () const = 0;

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
   * Destructs the object.
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
  virtual MccQTrueFunc* Copy () const { return new MccQTrueFunc (*this); }

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
   * Destructs the object.
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
  virtual MccQIdentFunc* Copy () const { return new MccQIdentFunc (*this); }

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
  MccQNotFunc (const MccQNotFunc &right) : fn (right.fn->Copy ()) { }

  /**
   * Destructs the object.
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
  virtual MccQNotFunc* Copy () const { return new MccQNotFunc (*this); }

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
    : left (right_val.left->Copy ()), right (right_val.right->Copy ()) { }
  
  /**
   * Destructs the object.
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
  virtual MccQAndFunc* Copy () const { return new MccQAndFunc (*this); }

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
    : left (right_val.left->Copy ()), right (right_val.right->Copy ()) { }
  
  /**
   * Destructs the object.
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
  virtual MccQOrFunc* Copy () const { return new MccQOrFunc (*this); }

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
   * Destructs the object.
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
  MccQueryExpr* Copy () const { return new MccQueryExpr (*this); }

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
     * Destructs the structure.  Clears the vector of file names.
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
    _AddPdbStruc* Copy () const { return new _AddPdbStruc (*this); }

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
   * Destructs the structure.  Clears the vector of sub-structures.
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
  virtual MccAddPdbStat* Copy () const { return new MccAddPdbStat (*this); }

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
   * Destructs the structure.
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
  virtual MccAdjacencyCstStat* Copy () const
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
     * Destructs the residue names and atom names.
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
    _AngleStruc* Copy () const { return new _AngleStruc (*this); }

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
   * Destructs the vector of sub-structures.
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
  virtual MccAngleCstStat* Copy () const
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
   * Destructs the variable and the expression.
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
  virtual MccAssignStat* Copy () const { return new MccAssignStat (*this); }

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
     * Destructs the object.  The destruction is controlled by the children
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
    virtual _GenBTStruc* Copy () const = 0;
    
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
     * Destructs the object.
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
    virtual _FGStruc* Copy () const { return new _FGStruc (*this); }
    
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
     * Destructs the object.
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
    virtual _BTStruc* Copy () const { return new _BTStruc (*this); }
    
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
     * Destructs the object.
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
    virtual _PlaceStruc* Copy () const { return new _PlaceStruc (*this); }
    
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
   * Destructs the object.  It clears the sub-structures contained in the
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
  virtual MccBacktrackExpr* Copy () const
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
   * The rms bound under wich a FG is not kept.
   */
  float rms_bound;

  /**
   * The set of atoms used to calculate the rms.
   */
  MccAS atom_set;

  /**
   * The options on the atom set.
   */
  MccAS atom_set_opt;

  /**
   * The variable specifying if Align is made.
   */
  bool align;

  
  // LIFECYCLE ------------------------------------------------------------

private:
  /**
   * Initializes the object.  It should never be used.
   */
  MccCacheExpr () { }
public:

  /**
   * Initializes the object.
   * @param f the FG structure to cache.
   * @param rms the rms bound under wich a FG is not kept.
   * @param as the atom set used to calculate the rms.
   * @param aso the options on the atom set.
   * @param al the variable specifying if Align is made.
   */
  MccCacheExpr (MccFragGenStruc *f, float rms, MccAS as, MccAS aso,
	      bool al)
    : fgref (f), rms_bound (rms), atom_set (as), atom_set_opt (aso),
      align (al) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccCacheExpr (const MccCacheExpr &right);
  
  /**
   * Destructs the object.
   */
  virtual ~MccCacheExpr () { delete fgref; }

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
  virtual MccCacheExpr* Copy () const { return new MccCacheExpr (*this); }
    
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
   * Destructs the object.
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
  virtual MccClashCstStat* Copy () const { return new MccClashCstStat (*this); }
    
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
     * The query expression structure for properties related to res1.
     */
    MccQueryExpr *expr_res1;

    /**
     * The name of the second residue in the relation.
     */
    MccResidueName *res2;

    /**
     * The query expression structure for properties related to res2.
     */
    MccQueryExpr *expr_res2;

    /**
     * The query expression structure.
     */
    MccQueryExpr *expr;

    /**
     * The size of the sampling.
     */
    int ssize;

    
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
     * @param e1 the query expression for properties related to res1.
     * @param r2 the name of the second residue in the relation.
     * @param e2 the query expression for properties related to res2.
     * @param e the query expression structure.
     * @param s the size of the sampling.
     */
    _ConnectStruc (MccResidueName *r1, MccQueryExpr *e1, 
		   MccResidueName *r2, MccQueryExpr *e2, MccQueryExpr *e,
		   int s)
      : res1 (r1), expr_res1 (e1), res2 (r2), expr_res2 (e2), expr (e), ssize (s) { }

    /**
     * Initializes the object with the rights content.
     * @param right the object to copy.
     */
    _ConnectStruc (const _ConnectStruc &right);
  
    /**
     * Destructs the object.
     */
    ~_ConnectStruc () { delete res1; delete expr_res1; delete res2; 
                        delete expr_res2; delete expr; }

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
    _ConnectStruc* Copy () const { return new _ConnectStruc (*this); }
    
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
   * Destructs the object.  Clears the vector of sub-structures.
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
  virtual MccConnectStat* Copy () const { return new MccConnectStat (*this); }
    
  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void Accept (MccVisitor *visitor);

  /**
   * Generates a new connect sub-structure and puts it in the vector.
   * @param r1 the name of the first residue.
   * @param r2 the name of the second residue.
   * @param e2 the query expression for properties related to res2.
   * @param e the query expression structure.
   * @param s the size of the sampling.
   */
  void GenConnectStruc (MccResidueName *r1, MccQueryExpr *e1, MccResidueName *r2,
			MccQueryExpr *e2, MccQueryExpr *e, int s)
  { strucs->push_back (new _ConnectStruc (r1, e1, r2, e2, e, s)); }

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
     * Destructs the object.
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
    _CycleStruc* Copy () const { return new _CycleStruc (*this); }
    
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
   * Destructs the object.
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
  MccCycleCstStat* Copy () const { return new MccCycleCstStat (*this); }
    
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
   * Destructs the object.
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
  MccDisplayFGStat* Copy () const { return new MccDisplayFGStat (*this); }
    
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
     * Destructs the object.
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
    _DistStruc* Copy () const { return new _DistStruc (*this); }
    
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
   * Destructs the object.  Clears the vector of sub-structures.
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
  virtual MccDistCstStat* Copy () const { return new MccDistCstStat (*this); }
    
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
 * @short Struct representing the file option on AST nodes "explore" and
 * "restore" 
 *
 * @author Martin Larose <larosem@iro.umontreal.ca>
 */
struct MccExpfile
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

private:
  /**
   * Initializes the object.  It should never be used.
   */
  MccExpfile () { }
public:

  /**
   * Initializes the object.
   * @param f the C form that will be used for file output.
   * @param z the boolean indicating if the files will be zipped.
   */
  MccExpfile (char *f, bool z) : form (f), zipped (z) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccExpfile (const MccExpfile &right);

  /**
   * Destructs the object.
   */
  ~MccExpfile () { delete[] form; }

  // OPERATORS ------------------------------------------------------------

  /**
   * Assigns the right struct values to the object.
   * @param right the struct to copy.
   */
  MccExpfile& operator= (const MccExpfile &right);

  // ACCESS ---------------------------------------------------------------
  
  // METHODS --------------------------------------------------------------

  /**
   * Replicates the object.
   * @return a copy of the current object.
   */
  MccExpfile* Copy () const { return new MccExpfile (*this); }
    
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
   * The explore output file structure.
   */
  MccExpfile *efile;
  

  // LIFECYCLE ------------------------------------------------------------

private:
  /**
   * Initializes the object.  It should never be used.
   */
  MccExploreStat () { }
public:

  /**
   * Initializes the object.
   * @param fg the FG struct to explore.
   * @param ef the explore output file structure.
   */
  MccExploreStat (MccFragGenStruc* fg, MccExpfile *ef)
    : fg_struc (fg), efile (ef) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccExploreStat (const MccExploreStat &right);

  /**
   * Destructs the object.
   */
  virtual ~MccExploreStat () { delete fg_struc; if (efile) delete efile; }

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
   * Replicates the object.
   * @return a copy of the current object.
   */
  virtual MccExploreStat* Copy () const { return new MccExploreStat (*this); }
    
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
     * Destructs the object.  The destruction is controlled by the children
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
    virtual _LibStruc* Copy () const = 0;
    
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
    virtual _StripStruc* Copy () const { return new _StripStruc (*this); }
    
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
    virtual _ChangeIdStruc* Copy () const { return new _ChangeIdStruc (*this); }
    
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
   * The C form representing the name and path of the library.
   */
  char *str;

  /**
   * The vector containing the sub-structures.
   */
  vector< _LibStruc* > *strucs;

  
  // LIFECYCLE ------------------------------------------------------------
    
  /**
   * Initializes the object.
   * @param s the C form representing the name and path of the library.
   */
  MccLibraryExpr (char *s) : str (s), strucs (new vector< _LibStruc* > ()) { }

  /**
   * Initializes the object.
   * @param s the C form representing the name and path of the library.
   * @param lsv the vector containing the library sub-structures.
   */
  MccLibraryExpr (char *s, vector< _LibStruc* > *lsv)
    : str (s), strucs (lsv) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccLibraryExpr (const MccLibraryExpr &right);

  /**
   * Destructs the object.
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
  virtual MccLibraryExpr* Copy () const { return new MccLibraryExpr (*this); }
    
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
   * Destructs the object.
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
  virtual MccNewTagStat* Copy () const { return new MccNewTagStat (*this); }
    
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
   * Destructs the object.
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
  virtual MccNoteStat* Copy () const { return new MccNoteStat (*this); }
    
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
   * Destructs the object.
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
  virtual MccNotesStat* Copy () const { return new MccNotesStat (*this); }
    
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
     * The query expression for properties related to res1.
     */
    MccQueryExpr *expr_res1;

    /**
     * The name of the second residue in the relation.
     */
    MccResidueName *res2;

    /**
     * The query expression for properties related to res2.
     */
    MccQueryExpr *expr_res2;

    /**
     * The query expression structure.
     */
    MccQueryExpr *expr;

    /**
     * The sampling size.
     */
    int ssize;


    // LIFECYCLE ------------------------------------------------------------

  private:
    /**
     * Initializes the object.  It should never be used.
     */
    _PairStruc () { }
  public:

    /**
     * Initializes the object.
     * @param r1 the name of the first residue.
     * @param e1 the query expression for properties related to res1.
     * @param r2 the name of the second residue.
     * @param e2 the query expression for properties related to res2.
     * @param e the query expression structure.
     * @param s the sampling size.
     */
    _PairStruc (MccResidueName *r1, MccQueryExpr *e1, 
		MccResidueName *r2, MccQueryExpr *e2, MccQueryExpr *e, int s)
      : res1 (r1), expr_res1 (e1), res2 (r2), expr_res2 (e2), expr (e), ssize (s) { }

    /**
     * Initializes the object with the rights content.
     * @param right the object to copy.
     */
    _PairStruc (const _PairStruc &right);

    /**
     * Destructs the object.
     */
    ~_PairStruc () { delete res1; delete expr_res1; delete res2; 
                     delete expr_res2; delete expr; }

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
    _PairStruc* Copy () const { return new _PairStruc (*this); }
    
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
   * Destructs the object.
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
   virtual MccPairStat* Copy () const { return new MccPairStat (*this); }
    
  /**
   * Accepts the visitor and calls it on itself.
   * @param visitor the visitor.
   */
  virtual void Accept (MccVisitor *visitor);

  /**
   * Generates a new pairing sub-structures and puts it in the vector.
   * @param r1 the name of the first residue.
   * @param e1 the query expression for properties related to res1.
   * @param r2 the name of the second residue.
   * @param e2 the query expression for properties related to res2.
   * @param e the query expression structure.
   * @param s the sampling size.
   */   
  void GenPairStruc (MccResidueName *r1, MccQueryExpr *e1, MccResidueName *r2,
		     MccQueryExpr *e2, MccQueryExpr *e, int s)
  { strucs->push_back (new _PairStruc (r1, e1, r2, e2, e, s)); }
  
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
   * Destructs the object.
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
   virtual MccQuitStat* Copy () const { return new MccQuitStat (*this); }
    
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
   virtual MccRemarkStat* Copy () const { return new MccRemarkStat (*this); }
    
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
   * Destructs the object.
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
  virtual MccResetDBStat* Copy () const { return new MccResetDBStat (*this); }
    
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
   * Destructs the object.
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
  virtual MccResetStat* Copy () const { return new MccResetStat (*this); }
    
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
     * The sampling size.
     */
    int ssize;


    // LIFECYCLE ------------------------------------------------------------

  private:
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
     * @param ss the sampling size.
     */
    _ResidueStruc (MccResidueName *r1, MccResidueName *r2,
		   MccQueryExpr *exp, int ss)
      : res1 (r1), res2 (r2), expr (exp), ssize (ss) { }

    /**
     * Initializes the object with the rights content.
     * @param right the object to copy.
     */
    _ResidueStruc (const _ResidueStruc &right);

    /**
     * Destructs the object.
     */
    ~_ResidueStruc () { delete res1; if (res2) delete res2; delete expr; }

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
    _ResidueStruc* Copy () const { return new _ResidueStruc (*this); }
    
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
   * Destructs the object.  Clears the vector of sub-structures.
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
  virtual MccResidueStat* Copy () const { return new MccResidueStat (*this); }
    
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
   * @param ss the sampling size.
   */   
  void GenResidueStruc (MccResidueName *r1, MccResidueName *r2,
			MccQueryExpr *exp, int ss)
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
  MccExpfile *efile;
  

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
  MccRestoreStat (char *fi, MccExpfile *ef) : filename (fi), efile (ef) { }

  /**
   * Initializes the object with the rights content.
   * @param right the object to copy.
   */
  MccRestoreStat (const MccRestoreStat &right);

  /**
   * Deletes the object.
   */
  ~MccRestoreStat () { delete[] filename; if (efile) delete efile; }

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
  virtual MccRestoreStat* Copy () const { return new MccRestoreStat (*this); }
    
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


  // LIFECYCLE ------------------------------------------------------------

private:
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
   * Destructs the object.
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
  virtual MccSequenceStat* Copy () const { return new MccSequenceStat (*this); }
    
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


  // LIFECYCLE ------------------------------------------------------------

private:
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
   * Destructs the object.
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
  virtual MccSourceStat* Copy () const { return new MccSourceStat (*this); }
    
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
     * Destructs the residue names and atom names.
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
    _TorsionStruc* Copy () const { return new _TorsionStruc (*this); }
    
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
   * Destructs the vector of sub-structures.
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
  virtual MccTorsionCstStat* Copy () const
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
   * Destructs the object.
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
  virtual MccVersion* Copy () const { return new MccVersion (*this); }
    
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
   * Destructs the object.
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
   * Visits the MccExpfile structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccExpfile *struc) = 0;
  
  /**
   * Visits the MccExploreStat structure.
   * @param struc the evaluated structure.
   */
  virtual void Visit (MccExploreStat *struc) = 0;
  
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
   * Destructs the object.
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
   * Destructs the object.
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
