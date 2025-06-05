#include "nodes.h"

bool printDelete = false; //whether to print deleting

#define EPSILON 0.001
static bool truth(float F) {
    return !(EPSILON > F) && (F > -EPSILON);
}

// ---------------------------------------------------------------------
// Indent according to tree level
static void indent(int level) {
  for (int i = 0; i < level; i++)
    cout << ("|  ");
}
// ---------------------------------------------------------------------
ProgramNode::ProgramNode(int level) {
  _level = level;
}

ostream& operator<<(ostream& os, ProgramNode& pn) {
  os << endl; indent(pn._level); os << "(program ";
	
  os << *(pn.Block); //Call block print method
    
  os << endl; indent(pn._level); os << "program) ";
	return os;
}
ProgramNode::~ProgramNode() {
  if (printDelete) cout << "Deleting ProgramNode" << endl;
	
  delete Block; //Call block destructor
	Block = nullptr;
}
void ProgramNode::interpret() {
  //Call block interpret method
  Block->interpret();
}
// ---------------------------------------------------------------------
BlockNode::BlockNode(int level) {
  _level = level;
}
ostream& operator<<(ostream& os, BlockNode& bn) {
  os << endl; indent(bn._level); os << "(block ";

  //Call compound statement print method
  os << *(bn.compoundStatement);

  os << endl; indent(bn._level); os << "block) ";
	return os;
}
BlockNode::~BlockNode() {
  if (printDelete) cout << "Deleting BlockNode" << endl;

  //Call compound statement destructor
	delete compoundStatement;
	compoundStatement = nullptr;
}
void BlockNode::interpret() {
  //Call statement method
  compoundStatement->interpret();
}
// ---------------------------------------------------------------------
StatementNode::~StatementNode() {}
ostream& operator<<(ostream& os, StatementNode& sn) {
  //Call all statement printTo methods
  sn.printTo(os);
	return os;
}
void StatementNode::interpret(){
  //Call all statement interpret methods
  this->interpret();
}
// ---------------------------------------------------------------------
AssignmentStmtNode::AssignmentStmtNode(int level, string yytext) {
  _level = level;
  text = yytext;
}

AssignmentStmtNode::~AssignmentStmtNode() {
  if (printDelete) cout << "Deleting StatementNode:AssignmentStmtNode" << endl;

  //Delete all expressions
  int length = Expression.size();
	for (int i = 0; i < length; ++i) {
		delete Expression[i];
    Expression[i] = nullptr;
	}
}

void AssignmentStmtNode::printTo(ostream &os)
{
  os << endl; indent(_level - 1); os << "(assignment_stmt ( " << text << " := ) ";
      
  //Print all expressions
  int length = Expression.size();
	for (int i = 0; i < length; ++i) os << *(Expression[i]);

  os << endl; indent(_level - 1); os << "assignment_stmt) ";
}

void AssignmentStmtNode::interpret() {
  float value;
  
  int length = Expression.size();
	for (int i = 0; i < length; ++i)
  {
    //Update symbol table with expression value
    value = Expression[i]->interpret();
    auto it = symbolTable.find(text);
    if (it != symbolTable.end()) it->second = value;
  }
}
// ---------------------------------------------------------------------
CompoundStmtNode::CompoundStmtNode(int level) {
    _level = level;
}

CompoundStmtNode::~CompoundStmtNode() {
  if (printDelete) cout << "Deleting StatementNode:CompoundStmtNode" << endl;

  //Delete all statements
	int length = Statement.size();
	for (int i = 0; i < length; ++i) {
	  delete Statement[i];
    Statement[i] = nullptr;
	}
}

void CompoundStmtNode::printTo(ostream &os)
{
  os << endl; indent(_level); os << "(compound_stmt ";

  //Print all statements
  int length = Statement.size();
	for (int i = 0; i < length; ++i) os << *(Statement[i]);

  os << endl; indent(_level); os << "compound_stmt) ";
}

void CompoundStmtNode::interpret() {
  //Interpret all statements
  int length = Statement.size();
	for (int i = 0; i < length; ++i) Statement[i]->interpret();
}
// ---------------------------------------------------------------------
IfStmtNode::IfStmtNode(int level) {
    _level = level;
}

IfStmtNode::~IfStmtNode() {
  if (printDelete) cout << "Deleting StatementNode:IfStmtNode" << endl;
    
  //Delete expression
  delete Expression;
  Expression = nullptr;

  //Delete first statement
  delete firstStatement;
  firstStatement = nullptr;

  //Delete second statement if applicable
  if (secondStatement != nullptr)
  {
    delete secondStatement;
    secondStatement = nullptr;
  }
}

void IfStmtNode::printTo(ostream &os)
{
  os << endl; indent(_level); os << "(if_stmt ";

  //Print all expressions
  os << *(Expression);

  os << endl; indent(_level); os << "(then "; //Print then

  //Print first statement
  os << *(firstStatement);

  os << endl; indent(_level); os << "then) ";

  //Print else and second statement if applicable
  if (secondStatement != nullptr)
  {
    os << endl; indent(_level); os << "(else ";
    os << *(secondStatement);
    os << endl; indent(_level); os << "else) ";
  }

  os << endl; indent(_level); os << "if_stmt) ";
}

void IfStmtNode::interpret() {
  //Interpret expression and interpret statement if applicable
  if (Expression->interpret() == 1.0) firstStatement->interpret();

  //Interpret else statement if applicable
  else if (secondStatement != nullptr) secondStatement->interpret();

}
// ---------------------------------------------------------------------
WhileStmtNode::WhileStmtNode(int level) {
    _level = level;
}

WhileStmtNode::~WhileStmtNode() {
  if (printDelete) cout << "Deleting StatementNode:WhileStmtNode" << endl;
    
  //Delete expression
	delete Expression;
  Expression = nullptr;

  //Delete statement
  delete Statement;
  Statement = nullptr;
}

void WhileStmtNode::printTo(ostream &os)
{
  os << endl; indent(_level); os << "(while_stmt ";

  //Print expression 
  os << *(Expression);
    
  //Print statement
  os << *(Statement);

  os << endl; indent(_level); os << "while_stmt) ";
}

void WhileStmtNode::interpret() {
  //Interpret statement while expression is true
  while(Expression->interpret() == 1.0) Statement->interpret();
}
// ---------------------------------------------------------------------
ReadStmtNode::ReadStmtNode(int level, string yytext) {
    _level = level;
    text = yytext;
}

ReadStmtNode::~ReadStmtNode() {
  if (printDelete) cout << "Deleting StatementNode:ReadStmtNode" << endl;
  //Nothing else to delete
}

void ReadStmtNode::printTo(ostream &os)
{
  //Print read statement and text read
  os << endl; indent(_level - 1); os << "(read_stmt ( " << text << " ) ";

  os << endl; indent(_level - 1); os << "read_stmt) ";
}

void ReadStmtNode::interpret() {
  //Read user input
  float input;
  cin >> input;

  //Update symbol table accordingly
  auto it = symbolTable.find(text);
  if (it != symbolTable.end()) it->second = input;
}
// ---------------------------------------------------------------------
WriteStmtNode::WriteStmtNode(int level, string yytext) {
    _level = level;
    text = yytext;
}

WriteStmtNode::~WriteStmtNode() {
  if (printDelete) cout << "Deleting StatementNode:WriteStmtNode" << endl;
  //Nothing else to delete
}

void WriteStmtNode::printTo(ostream &os)
{
  //Print write statement and text written
  os << endl; indent(_level - 1); os << "(write_stmt ( " << text << " ) ";

  os << endl; indent(_level - 1); os << "write_stmt) ";
}

void WriteStmtNode::interpret() {
  //Find and print symbol table value if applicable
  auto it = symbolTable.find(text);
  if (it != symbolTable.end()) cout << it->second << endl;
  
  //Otherwise print contents without ''
  else cout << text.substr(1, text.length() - 2) << endl;
}
// ---------------------------------------------------------------------
ExpressionNode::ExpressionNode(int level) {
  _level = level;
}

ostream& operator<<(ostream& os, ExpressionNode& en) {
  os << endl; indent(en._level); os << "(expression ";

  //Print first simple expression
  os << *(en.firstSimpleExp);

  //Print operations and remaining simple expressions
  int length = en.restSimpleExp.size();
  for (int i = 0; i < length; i++)
  {
    int op = en.restSimpleExpOps[i];
    if (op == TOK_EQUALTO) {
      os << endl; indent(en._level); os << "= ";
    } 
    if (op == TOK_LESSTHAN) {
      os << endl; indent(en._level); os << "< ";
    }
    if (op == TOK_GREATERTHAN) {
      os << endl; indent(en._level); os << "> ";
    }
    if (op == TOK_NOTEQUALTO) {
      os << endl; indent(en._level); os << "<> ";
    }
    
	  os << *(en.restSimpleExp[i]);
  }

  os << endl; indent(en._level); os << "expression) ";
	return os;
}

ExpressionNode::~ExpressionNode() {
  if (printDelete) cout << "Deleting ExpressionNode" << endl;

  //Delete first simple expression
  delete firstSimpleExp;
  firstSimpleExp = nullptr;
	
  //Delete remaining simple expressions
  int length = restSimpleExp.size();
  for (int i = 0; i < length; i++)
  {
    delete restSimpleExp[i];
	  restSimpleExp[i] = nullptr;
  }
}

float ExpressionNode::interpret() {
  //Set return value to first simple expression
  float returnValue = firstSimpleExp->interpret();

  int length = restSimpleExp.size();
  for (int i = 0; i < length; i++)
  {
    //Check for true/false conditions and return 1.0/0.0 accordingly
    int op = restSimpleExpOps[i];
    if (op == TOK_EQUALTO) {
      if (abs(returnValue - restSimpleExp[i]->interpret()) <= EPSILON)
        return 1.0;
      else return 0.0;
    } 
    if (op == TOK_LESSTHAN) {
      if (returnValue < restSimpleExp[i]->interpret()) return 1.0;
      else return 0.0;
    }
    if (op == TOK_GREATERTHAN) {
      if (returnValue > restSimpleExp[i]->interpret()) return 1.0;
      else return 0.0;
    }
    if (op == TOK_NOTEQUALTO) {
      if (abs(returnValue - restSimpleExp[i]->interpret()) > EPSILON)
        return 1.0;
      else return 0.0;
    }
    
  }

  //Otherwise return simple expression value
  return returnValue;
}
// ---------------------------------------------------------------------
SimpleExpNode::SimpleExpNode(int level) {
  _level = level;
}

ostream& operator<<(ostream& os, SimpleExpNode& sen) {
  os << endl; indent(sen._level); os << "(simple_exp ";
  
  //Print first term
  os << *(sen.firstTerm);

  //Print operations and remaining terms
  int length = sen.restTerm.size();
  for (int i = 0; i < length; i++)
  {
    int op = sen.restTermOps[i];
    if (op == TOK_PLUS) {
      os << endl; indent(sen._level); os << "+ ";
    } 
    if (op == TOK_MINUS) {
      os << endl; indent(sen._level); os << "- ";
    }
    if (op == TOK_OR) {
      os << endl; indent(sen._level); os << "OR ";
    }
    
	os << *(sen.restTerm[i]);
  }

  os << endl; indent(sen._level); os << "simple_exp) ";
	return os;
}

SimpleExpNode::~SimpleExpNode() {
  if (printDelete) cout << "Deleting SimpleExpNode" << endl;

  //Delete first term
  delete firstTerm;
  firstTerm = nullptr;

  //Delete remaining terms
	int length = restTerm.size();
  for (int i = 0; i < length; i++)
  {
    delete restTerm[i];
	  restTerm[i] = nullptr;
  }
}

float SimpleExpNode::interpret() {
  //Set return value to first term
  float returnValue = firstTerm->interpret();

  int length = restTerm.size();
  for (int i = 0; i < length; i++)
  {
    //Evaluate operations and update return value
    int op = restTermOps[i];
    if (op == TOK_PLUS) {
      return returnValue + restTerm[i]->interpret();
    } 
    if (op == TOK_MINUS) {
      return returnValue - restTerm[i]->interpret();
    }
    //Evaluate OR. Must have nested expressions on either
    //side of OR to interpret properly
    if (op == TOK_OR) {
      if (returnValue == 1.0 || restTerm[i]->interpret() == 1.0) return 1.0;
      else return 0.0;
    }
  }

  return returnValue;
}
// ---------------------------------------------------------------------
TermNode::TermNode(int level) {
  _level = level;
}

ostream& operator<<(ostream& os, TermNode& tn) {
  os << endl; indent(tn._level); os << "(term ";

  //Print first factor
  os << *(tn.firstFactor);

  //Print operations and remaining factors
  int length = tn.restFactor.size();
  for (int i = 0; i < length; i++)
  {
    int op = tn.restFactorOps[i];
    if (op == TOK_MULTIPLY) {
      os << endl; indent(tn._level); os << "* ";
    } 
    if (op == TOK_DIVIDE) {
      os << endl; indent(tn._level); os << "/ ";
    }
    //Evaluate AND. Must have nested expressions on either
    //side of AND to interpret properly
    if (op == TOK_AND) {
      os << endl; indent(tn._level); os << "AND ";
    }
    
	os << *(tn.restFactor[i]);
  }
  os << endl; indent(tn._level); os << "term) ";
	return os;
}

TermNode::~TermNode() {
  if (printDelete) cout << "Deleting TermNode" << endl;

  //Delete first Factor
  delete firstFactor;
  firstFactor = nullptr;
	
  //Delete remaining factors
  int length = restFactor.size();
  for (int i = 0; i < length; i++)
  {
    delete restFactor[i];
	  restFactor[i] = nullptr;
  }
}

float TermNode::interpret() {
  //Set return value to first factor
  float returnValue = firstFactor->interpret();

  int length = restFactor.size();
  for (int i = 0; i < length; i++)
  {
    //Evaluate operations and update return value
    int op = restFactorOps[i];
    if (op == TOK_MULTIPLY) {
      returnValue = returnValue * restFactor[i]->interpret();
    } 
    if (op == TOK_DIVIDE) {
      returnValue = returnValue / restFactor[i]->interpret();
    }
    if (op == TOK_AND) {
      if (returnValue == 1.0 && restFactor[i]->interpret() == 1.0) return 1.0;
      else return 0.0;
    }
  }

  return returnValue;
}
// ---------------------------------------------------------------------
FactorNode::~FactorNode() {}
// Uses double dispatch to call the overloaded method printTo in the 
// FactorNodes
ostream& operator<<(ostream& os, FactorNode& fn) {
  //Call all factor printTo methods
  fn.printTo(os);
  return os;
}
float FactorNode::interpret()
{
  //Interpret all factors
  return this->interpret();
}
// ---------------------------------------------------------------------
IdNode::IdNode(int level, string name) {
  _level = level;
  id = name;
}
IdNode::~IdNode() { 
  if (printDelete) cout << "Deleting FactorNode:IdNode" << endl;
  //Nothing else to delete
}
void IdNode::printTo(ostream& os) {
  //Print identifier
	os << endl; indent(_level - 1); os << "(factor ( IDENT: " << id << " ) ";
  os << endl; indent(_level - 1); os << "factor) ";
}

float IdNode::interpret() {
  float returnValue = 0.0;

  //Find identifier value in symbol table and assign it to return value
  auto it = symbolTable.find(id);
  if (it != symbolTable.end()) returnValue = it->second;

  return returnValue;
}
// ---------------------------------------------------------------------
IntLitNode::IntLitNode(int level, float value) {
  _level = level - 1;
  int_lit = value;
}
IntLitNode::~IntLitNode() {
  if (printDelete) cout << "Deleting FactorNode:IntLitNode" << endl;
  //Nothing else to delete
}
void IntLitNode::printTo(ostream& os) {
  //Print Intlit
	os << endl; indent(_level); os << "(factor ( INTLIT: " << int_lit << " ) ";
  os << endl; indent(_level); os << "factor) ";
}

float IntLitNode::interpret() {
  //Return Intlit value as float
  return int_lit;
}
// ---------------------------------------------------------------------
FloatLitNode::FloatLitNode(int level, float value) {
  _level = level - 1;
  float_lit = value;
}
FloatLitNode::~FloatLitNode() {
  if (printDelete) cout << "Deleting FactorNode:FloatLitNode" << endl;
  //Nothing else to delete
}
void FloatLitNode::printTo(ostream& os) {
  //Print floatlit
	os << endl; indent(_level); os << "(factor ( FLOATLIT: " << float_lit << " ) ";
  os << endl; indent(_level); os << "factor) ";
}
float FloatLitNode::interpret() {
  //Return Floatlit value
  return float_lit;
}
// ---------------------------------------------------------------------
NestedExprNode::NestedExprNode(int level) {
  _level = level;
}
void NestedExprNode::printTo(ostream& os) {
  //Print Nested Expression                             Call expression
	os << endl; indent(_level - 1); os << "(factor ( " << *(Expression) << ") ";
  os << endl; indent(_level - 1); os << "factor) ";
}
NestedExprNode::~NestedExprNode() {
  if (printDelete) cout << "Deleting FactorNode:NestedExprNode" << endl;

  //Delete expression used
  delete Expression;
  Expression = nullptr;
}
float NestedExprNode::interpret() {
  //Interpret expression and assign it to return value
  float returnValue = Expression->interpret();

  return returnValue;
}
// ---------------------------------------------------------------------
NotNode::NotNode(int level) {
  _level = level;
}
NotNode::~NotNode() { 
  if (printDelete) cout << "Deleting FactorNode:NotNode" << endl;

  //Delete second call of factor
  delete Factor;
}
void NotNode::printTo(ostream& os) {
	os << endl; indent(_level - 1); os << "(factor (NOT ";
    
  //Call factor again
  os << *(Factor);

  os << ") " << endl; indent(_level - 1); os << "factor) ";
}
float NotNode::interpret() {
  //Assign !factor to return value
  float returnValue = !(Factor->interpret());

  return returnValue;
}
// ---------------------------------------------------------------------
MinusNode::MinusNode(int level) {
  _level = level;
}
MinusNode::~MinusNode() { 
  if (printDelete) cout << "Deleting FactorNode:MinusNode" << endl;

  //Delete second call of factor
  delete Factor;
}
void MinusNode::printTo(ostream& os) {
	os << endl; indent(_level - 1); os << "(factor (- ";

  //Call factor again
  os << *(Factor);

  os << ") " << endl; indent(_level - 1); os << "factor) ";
}
float MinusNode::interpret() {
  //Assign -factor to return value
  float returnValue = -(Factor->interpret());

  return returnValue;
}