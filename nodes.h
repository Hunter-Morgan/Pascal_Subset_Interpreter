#ifndef NODES_H
#define NODES_H

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "lexer.h"

using namespace std;

// ---------------------------------------------------------------------
// Forward declaration of node types
class ProgramNode;
class BlockNode;
class StatementNode;
class AssignmentStmtNode;
class CompoundStmtNode;
class IfStmtNode;
class WhileStmtNode;
class ReadStmtNode;
class WriteStmtNode;
class ExpressionNode;
class SimpleExpNode;
class TermNode;
class FactorNode;
class IntLitNode;
class FloatLitNode;
class IdNode;
class NestedExprNode;
class NotNode;
class MinusNode;

// Holds the symbols in the interpreted program
typedef map<string, float> symbolTableT;
extern symbolTableT symbolTable;

class ProgramNode {
public:
  int _level = 0; //Recursion level

  BlockNode* Block = nullptr; //Pointer to  block node
  
  //Constructors and destructors
  ProgramNode(int level);
  ~ProgramNode();
  void interpret();
};
ostream& operator<<(ostream&, ProgramNode&); //Node print operator
// ---------------------------------------------------------------------
class BlockNode {
public:
  int _level = 0; //Recursion level
  
  //Pointer to Compound Statement
  CompoundStmtNode* compoundStatement = nullptr;

  //Constructors and destructors
  BlockNode(int level);
  ~BlockNode();

  //Interpret function
  void interpret();
};
ostream& operator<<(ostream&, BlockNode&); //Node print operator
// ---------------------------------------------------------------------
class ExpressionNode {
public:
  int _level = 0; //Recursion level

  SimpleExpNode* firstSimpleExp; //Pointer to first simple expression
  vector <SimpleExpNode*> restSimpleExp; //Pointer to remaining SE
  vector<int> restSimpleExpOps; //Operations (=, <, >, <>)

  //Constructors and destructors
  ExpressionNode(int level);
  ~ExpressionNode();

  //Interpret function
  float interpret();
};
ostream& operator<<(ostream&, ExpressionNode&); // Node print operator
// ---------------------------------------------------------------------
class SimpleExpNode {
public:
  int _level = 0; //Recursion level

  TermNode* firstTerm = nullptr; //Pointer to first term
  vector<TermNode*> restTerm; //Pointer to remaining terms
  vector<int> restTermOps; //Operations (+, -, OR)

  //Constructors and destructors
  SimpleExpNode(int level);
  ~SimpleExpNode();

  //Interpret function
  float interpret();
};
ostream& operator<<(ostream&, SimpleExpNode&); //Node print operator
// ---------------------------------------------------------------------
class TermNode {
public:
  int _level = 0; //Recursion level

  FactorNode* firstFactor = nullptr; //Pointer to first factor
  vector<FactorNode*> restFactor; //Pointer to remaining factors
  vector<int> restFactorOps;  //Operations (*, /, AND)

  //Constructors and destructors
  TermNode(int level);
  ~TermNode();

  //Interpret function
  float interpret();
};
ostream& operator<<(ostream&, TermNode&); //Node print operator
// ---------------------------------------------------------------------
// Abstract class. Base class for statement functions
class StatementNode {
public:
    virtual void interpret() = 0;          //Interpret function. Also pure virtual
    virtual void printTo(ostream &os) = 0; //Pure virtual method, makes the class Abstract
    
    virtual ~StatementNode();              //Labeling the destructor as virtual allows 
	                                       //the subclass destructors to be called
};
ostream& operator<<(ostream&, StatementNode&);  //Print method
// ---------------------------------------------------------------------
class AssignmentStmtNode : public StatementNode {
public:
    int _level = 0; //Recursion level

    string text; //Text within expression

    vector<ExpressionNode*> Expression; //Pointer to expression

    // Add constructor, destructor, and printTo methods
    AssignmentStmtNode(int level, string yytext);

    ~AssignmentStmtNode();

    //Declare the printTo method for node
    void printTo(ostream &os) override;

    //Interpret function
    void interpret();
};
// ---------------------------------------------------------------------
class CompoundStmtNode : public StatementNode {
public:
    int _level = 0; //Recursion Level

    vector<StatementNode*> Statement; //Pointer to statement

    // Add constructor, destructor, and printTo methods
    CompoundStmtNode(int level);

    ~CompoundStmtNode();

    //Declare the printTo method for node
    void printTo(ostream &os) override;

    //Interpret function
    void interpret();
};
// ---------------------------------------------------------------------
class IfStmtNode : public StatementNode {
public:
    int _level = 0; //Recursion level

    //Pointer to first statement (after THEN)
    StatementNode* firstStatement = nullptr;

    //Pointer to second statement (after ELSE) 
    StatementNode* secondStatement = nullptr;

    //Pointer to expression
    ExpressionNode* Expression;

    // Add constructor, destructor, and printTo methods
    IfStmtNode(int level);

    ~IfStmtNode();

    //Declare the printTo method for node
    void printTo(ostream &os) override;

    //Interpret function
    void interpret();

};
// ---------------------------------------------------------------------
class WhileStmtNode : public StatementNode {
public:
    int _level = 0; //Recursion Level

    StatementNode* Statement = nullptr; //Pointer to statement

    ExpressionNode* Expression; //Pointer to Expression(s)

    // Add constructor, destructor, and printTo methods
    WhileStmtNode(int level);

    ~WhileStmtNode();

    //Declare the printTo method for node
    void printTo(ostream &os) override;

    //Interpret function
    void interpret();

};
// ---------------------------------------------------------------------
class ReadStmtNode : public StatementNode {
public:
    int _level = 0; //Recursion level

    string text; //Contents of Read

    // Add constructor, destructor, and printTo methods
    ReadStmtNode(int level, string yytext);

    ~ReadStmtNode();

    //Declare the printTo method for node
    void printTo(ostream &os) override;

    //Interpret function
    void interpret();

};
// ---------------------------------------------------------------------
class WriteStmtNode : public StatementNode {
public:
    int _level = 0; //Recurstion level

    string text; //Contents of Write

    // Add constructor, destructor, and printTo methods
    WriteStmtNode(int level, string yytext);

    ~WriteStmtNode();

    //Declare the printTo method for node
    void printTo(ostream &os) override;

    //Interpret function
    void interpret();
};
// ---------------------------------------------------------------------
// Abstract class. Base class for factor functions
class FactorNode {
public:
  int _level = 0;                        //Recursion Level

  virtual float interpret() = 0;         //Interpret function. Also pure virtual
  virtual void printTo(ostream &os) = 0; //Pure virtual method, makes the class Abstract
  virtual ~FactorNode();                 //Labeling the destructor as virtual allows 
	                                     //The subclass destructors to be called
};
ostream& operator<<(ostream&, FactorNode&); // Node print operator
// ---------------------------------------------------------------------
// class IdNode (Identifier Node)
class IdNode : public FactorNode {
public:
    string id; //identifier

    //Constructor, destructor and printTo method
    IdNode(int level, string name);
    ~IdNode();
    void printTo(ostream & os);

    //Interpret function
    float interpret();
};
// ---------------------------------------------------------------------
// class IntLitNode (Integer Literal Node)
class IntLitNode : public FactorNode {
public:
    float int_lit; //integer literal

    //Constructor, destructor and printTo method
    IntLitNode(int level, float value);
    ~IntLitNode();
    void printTo(ostream & os);

    //Interpret function
    float interpret();
};
// ---------------------------------------------------------------------
class FloatLitNode : public FactorNode {
public:
    float float_lit; //float literal

    //Constructor, destructor and printTo method
    FloatLitNode(int level, float value);
    ~FloatLitNode();
    void printTo(ostream & os);

    //Interpret function
    float interpret();
};
// ---------------------------------------------------------------------
// class NestedExprNode (Nested Expression Node)
class NestedExprNode : public FactorNode {
public:
    //Pointer to expression
    ExpressionNode* Expression = nullptr;

    //Constructor, destructor and printTo method
    NestedExprNode(int level);
    void printTo(ostream & os);
    ~NestedExprNode();

    //Interpret function
    float interpret();
};
// ---------------------------------------------------------------------
class NotNode : public FactorNode {
public:
    //Pointer to factor
    FactorNode* Factor = nullptr;

    //Constructor, destructor and printTo method
    NotNode(int level);
    void printTo(ostream & os);
    ~NotNode();

    //Interpret function
    float interpret();
};
// ---------------------------------------------------------------------
class MinusNode : public FactorNode {
public:
    //Pointer to factor
    FactorNode* Factor = nullptr;

    //Constructor, destructor and printTo method
    MinusNode(int level);
    void printTo(ostream & os);
    ~MinusNode();

    //Interpret function
    float interpret();
};
// ---------------------------------------------------------------------
#endif /* NODES_H */
