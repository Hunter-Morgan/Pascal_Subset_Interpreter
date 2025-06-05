#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdlib.h>
#include <iostream>
#include "lexer.h"
#include "parser.h"
 
using namespace std;

int nextToken = 0; //Hold nextToken returned by lex

bool printParse = false; //Whether to output during parsing

// Which tree level are we currently in?  
static int level = 0;

// Feel free to use a different data structure for the symbol table (list of
// variables declared in the program) but you will have to adjust the code in
// main() to print out the symbol table after a successful parse

// Symbol table
symbolTableT symbolTable;
// Determine if a symbol is in the symbol table
bool inSymbolTable(string idName) {
  symbolTableT::iterator it;
  it = symbolTable.find( idName );
  // If idName is missing, will be set to the end
  return !(it == symbolTable.end());
}

//*****************************************************************************
// Indent to reveal tree structure
string psp(void) { // Stands for p-space, but I want the name short
  string str("");
  for(int i = 0; i < level; i++)
    str += "|  ";
  return str;
}
//*****************************************************************************
// Get the next lexeme (word in sentence)
int lex() 
{
  char* token_str;  // point to the logical name of the token

  nextToken = yylex();
  if( nextToken == TOK_EOF ) {
    yytext[0] = 'E'; yytext[1] = 'O'; yytext[2] = 'F'; yytext[3] = 0;   
  }

  // Determine the logical name of the lexeme
  switch( nextToken )
  {
    // Keyword Lexemes
    case TOK_BEGIN:           token_str = (char*)"BEGIN"; break;
    case TOK_BREAK:           token_str = (char*)"BREAK"; break;
    case TOK_CONTINUE:        token_str = (char*)"CONTINUE"; break;
    case TOK_DOWNTO:          token_str = (char*)"DOWNTO"; break;
    case TOK_ELSE:            token_str = (char*)"ELSE"; break;
    case TOK_END:             token_str = (char*)"END"; break;
    case TOK_FOR:             token_str = (char*)"FOR"; break;
    case TOK_IF:              token_str = (char*)"IF"; break;
    case TOK_LET:             token_str = (char*)"LET"; break;
    case TOK_PROGRAM:         token_str = (char*)"PROGRAM"; break;
    case TOK_READ:            token_str = (char*)"READ"; break;
    case TOK_THEN:            token_str = (char*)"THEN"; break;
    case TOK_TO:              token_str = (char*)"TO"; break;
    case TOK_VAR:             token_str = (char*)"VAR"; break;
    case TOK_WHILE:           token_str = (char*)"WHILE"; break;
    case TOK_WRITE:           token_str = (char*)"WRITE"; break;

    // Datatype Specifier Lexemes
    case TOK_INTEGER:         token_str = (char*)"INTEGER"; break;
    case TOK_REAL:            token_str = (char*)"REAL"; break;

    // Punctuation Lexemes
    case TOK_SEMICOLON:       token_str = (char*)"SEMICOLON"; break;
    case TOK_COLON:           token_str = (char*)"COLON"; break;
    case TOK_OPENPAREN:       token_str = (char*)"OPENPAREN"; break;
    case TOK_CLOSEPAREN:      token_str = (char*)"CLOSEPAREN"; break;

    // Operator Lexemes
    case TOK_PLUS:            token_str = (char*)"PLUS"; break;
    case TOK_MINUS:           token_str = (char*)"MINUS"; break;
    case TOK_MULTIPLY:        token_str = (char*)"MULTIPLY"; break;
    case TOK_DIVIDE:          token_str = (char*)"DIVIDE"; break;
    case TOK_ASSIGN:          token_str = (char*)"ASSIGN"; break;
    case TOK_EQUALTO:         token_str = (char*)"EQUALTO"; break;
    case TOK_LESSTHAN:        token_str = (char*)"LESSTHAN"; break;
    case TOK_GREATERTHAN:     token_str = (char*)"GREATERTHAN"; break;
    case TOK_NOTEQUALTO:      token_str = (char*)"NOTEQUALTO"; break;
    case TOK_MOD:             token_str = (char*)"MOD"; break;
    case TOK_NOT:             token_str = (char*)"NOT"; break;
    case TOK_OR:              token_str = (char*)"OR"; break;
    case TOK_AND:             token_str = (char*)"AND"; break;

    // Useful Abstraction Lexemes
    case TOK_IDENT:           token_str = (char*)"IDENTIFIER"; break;
    case TOK_INTLIT:          token_str = (char*)"INTLIT"; break;
    case TOK_FLOATLIT:        token_str = (char*)"FLOATLIT"; break;
    case TOK_STRINGLIT:       token_str = (char*)"STRINGLIT"; break;
    

    case TOK_UNKNOWN:         token_str = (char*)"TOK_UNKNOWN"; break;

    default:                  token_str = (char*)"=== unmapped token name ===";
  }

  return nextToken;
}
//*****************************************************************************
// Report what we found
void output(string what) {
  cout << psp() << "found |" << yytext << "| " << what << endl;
}
//*****************************************************************************

//first_of functions
bool first_of_program(void)
{
  return nextToken == TOK_PROGRAM;
}

bool first_of_block(void)
{
  return nextToken == TOK_VAR;
}

bool first_of_statement(void)
{
  switch( nextToken )
  {
    case TOK_IDENT:           break;
    case TOK_BEGIN:           break;
    case TOK_IF:              break;
    case TOK_WHILE:           break;
    case TOK_READ:            break;
    case TOK_WRITE:           break;

    default:                  throw "900: illegal type of statement";
  }

  return nextToken;

}

bool first_of_compoundStatement(void)
{
  return nextToken == TOK_BEGIN;
}

bool first_of_expression(void)
{
  switch( nextToken )
  {
    case TOK_INTLIT:          break;
    case TOK_FLOATLIT:        break;
    case TOK_IDENT:           break;
    case TOK_OPENPAREN:       break;
    case TOK_NOT:             break;
    case TOK_MINUS:           break;
    
    default:                  throw "144: illegal type of expression";
  }
  
  return nextToken;

}

bool first_of_simpleExpression(void)
{
  switch( nextToken )
  {
    case TOK_INTLIT:          break;
    case TOK_FLOATLIT:        break;
    case TOK_IDENT:           break;
    case TOK_OPENPAREN:       break;
    case TOK_NOT:             break;
    case TOK_MINUS:           break;
    
    default:                  throw "901: illegal type of simple expression";
  }

  return nextToken;

}

bool first_of_term(void)
{
  switch( nextToken )
  {
    case TOK_INTLIT:          break;
    case TOK_FLOATLIT:        break;
    case TOK_IDENT:           break;
    case TOK_OPENPAREN:       break;
    case TOK_NOT:             break;
    case TOK_MINUS:           break;
    
    default:                  throw "902: illegal type of term";
  }

  return nextToken;

}

bool first_of_factor(void)
{
  switch( nextToken )
  {
    case TOK_INTLIT:          break;
    case TOK_FLOATLIT:        break;
    case TOK_IDENT:           break;
    case TOK_OPENPAREN:       break;
    case TOK_NOT:             break;
    case TOK_MINUS:           break;
    
    default:                  throw "903: illegal type of factor";
  }

  return nextToken;
}

// Parses strings in the language generated by the rule:
// <program> → TOK_PROGRAM TOK_IDENT TOK_SEMICOLON <block>
ProgramNode* program() 
{
    //Create Node
    ProgramNode* newProgramNode = new ProgramNode(level);

    if (!first_of_program()) // Check for PROGRAM
        throw "3: 'PROGRAM' expected";
        
    if (printParse) output("PROGRAM");
    lex();

    if (printParse) cout << psp() << "enter <program>" << endl;
    ++level;

    //Check for identifier
    if (printParse)
    {
      if (nextToken == TOK_IDENT) output("IDENTIFIER");
      else throw "2: identifier expected";
    }
    lex();

    //Check for semicolon
    if (printParse)
    {
      if (nextToken == TOK_SEMICOLON) output("SEMICOLON");
      else throw "14: ';' expected";
    }
    lex();
    
    //Call block and assign pointer
    newProgramNode->Block = block();

    --level;
    if (printParse) cout << psp() << "exit <program>" << endl;

    return newProgramNode;
}

//Grammar functions
BlockNode* block()
{
  //Local variables to store VAR names and types
  string idName;
  string idType;

  //Ensure block starts with a VAR or BEGIN
  if (!first_of_block() && !first_of_compoundStatement())
    throw "18: error in declaration part OR 17: 'BEGIN' expected";

  if (printParse)
  {
    output("BLOCK");
    cout << psp() << "enter <block>" << endl;
  }

  //Create Node
  BlockNode* newBlockNode = new BlockNode(level);
  
  ++level;

  //Handle VAR
  if (first_of_block())
  {
    
    lex();

    //Loop while an identifier is found
    while (nextToken == TOK_IDENT)
    {
      //Check for identifier
      if (nextToken == TOK_IDENT)
      {
        if (printParse) output("IDENTIFIER");
        idName = yytext;
        
      }
      else throw "2: identifier expected";
      lex();

      //Check for colon
      if (nextToken == TOK_COLON)
      {
        if (printParse) output("COLON");
      }
      else throw "5: ':' expected";

      lex();

      //Check for type
      if (nextToken == TOK_INTEGER || nextToken == TOK_REAL)
      {
        if (printParse) output("TYPE");
        idType = yytext;
      }
      else throw "10: error in type";
      lex();

      //Check for semicolon
      if (nextToken == TOK_SEMICOLON)
      {
        if (printParse) output("SEMICOLON");
      }
      else throw "14: ';' expected";
      

      //Output VAR name and type
      if (printParse) cout << psp() << "-- idName: |" << idName << "| idType: |" << idType << "| --\n";

      //Check for double declaration and add to symbol table if new
      if (inSymbolTable(idName)) throw "101: identifier declared twice";
      else symbolTable.insert( pair<string, float>(idName, 0.0));

      lex();
    }

    //Call compound statement and assign pointer after variables are handled
    newBlockNode->compoundStatement = compoundStatement();
  }

  //Call compound statement and assign pointer if block starts with BEGIN
  else newBlockNode->compoundStatement = compoundStatement();

  --level;
  if (printParse) cout << psp() << "exit <block>" << endl;

  return newBlockNode;
}

StatementNode* statement()
{
  //Check for allowed token
  if (printParse && first_of_statement) output("STATEMENT");

  //Create Node
  StatementNode* newStatement = nullptr;

  //Go to appropriate function based on start token
  switch( nextToken )
  {                           //Assign correct statement to newStatement
    case TOK_IDENT:           newStatement = assignmentStatement(); break;
    case TOK_BEGIN:           newStatement = compoundStatement(); break;
    case TOK_IF:              newStatement = ifStatement(); break;
    case TOK_WHILE:           newStatement = whileStatement(); break;
    case TOK_READ:            newStatement = readStatement(); break;
    case TOK_WRITE:           newStatement = writeStatement(); break;
  }

  return newStatement;
}

AssignmentStmtNode* assignmentStatement()
{
  if (printParse) cout << psp() << "enter <assignment>" << endl;
  ++level;
  
  //Check for identifier
  if (nextToken == TOK_IDENT)
  {
    if (printParse) output("IDENTIFIER");
  }
  else throw "2: identifier expected";

  //Output identifier
  if (printParse) cout << psp() << yytext << endl;

  //Create Node
  AssignmentStmtNode* newAssignmentStatement = nullptr;
  newAssignmentStatement = new AssignmentStmtNode(level, yytext);

  lex();

  //Check for assignment character
    if (nextToken == TOK_ASSIGN)
    {
      if (printParse) output("ASSIGN");
    }
    else throw "51: ':=' expected";

  lex();

  //Call expression and assign pointer
  newAssignmentStatement->Expression.push_back(expression());

  --level;
  if (printParse) cout << psp() << "exit <assignment>" << endl;

  return newAssignmentStatement;
}

CompoundStmtNode* compoundStatement()
{
  //Check for BEGIN
  if (first_of_compoundStatement())
  {
    if (printParse) output("BEGIN");
  }
  else throw "17: 'BEGIN' expected";
    
  if (printParse) cout << psp() << "enter <compound_stmt>" << endl;

  //Create Node
  CompoundStmtNode* newCompoundStatement = nullptr;
  newCompoundStatement = new CompoundStmtNode(level);

  ++level;
  lex();

  //Loop while END is not reached
  int i = 0;
  while (1)
  {
    //Call statement and assign pointer
    newCompoundStatement->Statement.push_back(statement());

    //Break if END is reached
    if (nextToken == TOK_END) break;

    //Check for semicolon
      if (nextToken == TOK_SEMICOLON)
      {
        if (printParse) output("SEMICOLON");
      }
      else throw "14: ';' expected";

    lex();
  }

    --level;

    if (printParse)
    {
      output("END");
      cout << psp() << "exit <compound_stmt>" << endl;
    }

    lex();

    return newCompoundStatement;
}

IfStmtNode* ifStatement()
{
  //Create Node
  IfStmtNode* newIfStatement = nullptr;
  newIfStatement = new IfStmtNode(level);

  if (printParse) cout << psp() << "enter <if>" << endl;
  ++level;
  lex();

  //Call expression and assign pointer
  newIfStatement->Expression = expression();

  --level;
  
  //Check for THEN
  if (nextToken == TOK_THEN)
  {
    if (printParse) output("THEN");
  }
  else throw "52: 'THEN' expected";

  if (printParse) cout << psp() << "enter <then>" << endl;

  ++level;
  lex();

  //Call statement and assign pointer
  newIfStatement->firstStatement = statement();

  --level;
  if (printParse) cout << psp() << "exit <then>" << endl;

  //Check for ELSE (optional so no error is thrown)
  if (nextToken == TOK_ELSE)
  {
    if (printParse)
    {
      output("ELSE");
      cout << psp() << "enter <else>" << endl;
    }
    ++level;
    lex();

    //Call statement and assign pointer
    newIfStatement->secondStatement = statement();
    
    --level;
    if (printParse) cout << psp() << "exit <else>" << endl;
  }

  if (printParse) cout << psp() << "exit <if>" << endl;

  return newIfStatement;
}

WhileStmtNode* whileStatement()
{
  //Create Node
  WhileStmtNode* newWhileStatement = nullptr;
  newWhileStatement = new WhileStmtNode(level);

  if (printParse) cout << psp() << "enter <while>" << endl;
  ++level;
  lex();

  //Call expression and assign pointer
  newWhileStatement->Expression = expression();

  //Call statement and assign pointer
  newWhileStatement->Statement = statement();

  --level;
  if (printParse) cout << psp() << "exit <while>" << endl;

  return newWhileStatement;
}

ReadStmtNode* readStatement()
{
  if (printParse) cout << psp() << "enter <read>" << endl;
  ++level;
  lex();

  //Check for (
  if (nextToken == TOK_OPENPAREN)
  {
    if (printParse) output("OPENPAREN");
  }
  else throw "9: '(' expected";
  lex();

  //Check for identifier
  if (nextToken == TOK_IDENT)
  {
    if (printParse) output("IDENTIFIER");
  }
  else throw "2: identifier expected";

  //Output identifier
  if (printParse) cout << psp() << yytext << endl;

  //Create Node
  ReadStmtNode* newReadStatement = nullptr;
  newReadStatement = new ReadStmtNode(level, yytext);

  lex();

  //Check for )
    if (nextToken == TOK_CLOSEPAREN)
    {
      if (printParse) output("CLOSEPAREN");
    }
    else throw "4: ')' expected";
  lex();

  --level;
  if (printParse) cout << psp() << "exit <read>" << endl;

  return newReadStatement;
}

WriteStmtNode* writeStatement()
{
  if (printParse) cout << psp() << "enter <write>" << endl;
  ++level;
  lex();

  //Check for (
  if (nextToken == TOK_OPENPAREN)
  {
    if (printParse) output("OPENPAREN");
  }
  else throw "9: '(' expected";
  
  lex();

  //Check for identifier of string literal
  if (nextToken == TOK_IDENT || nextToken == TOK_STRINGLIT)
  {
    if (printParse) output("WRITE");
  }
  else throw "134: illegal type of operand(s)";

  //Output identifier/stringlit
  if (printParse) cout << psp() << yytext << endl;

  //Create Node
  WriteStmtNode* newWriteStatement = nullptr;
  newWriteStatement = new WriteStmtNode(level, yytext);

  lex();

  //Check for )
    if (nextToken == TOK_CLOSEPAREN)
    {
      if (printParse) output("CLOSEPAREN");
    }
    else throw "4: ')' expected";

  lex();

  --level;
  if (printParse) cout << psp() << "exit <write>" << endl;

  return newWriteStatement;
}

ExpressionNode* expression()
{
  //Ensure expression starts with an accepted token
  if (first_of_expression)
  {
    if (printParse) output("EXPRESSION");
  }
  else throw "144: illegal type of expression";

  if (printParse) cout << psp() << "enter <expression>" << endl;

  //Create Node
  ExpressionNode* newExpressionNode = nullptr;
  newExpressionNode = new ExpressionNode(level);

  ++level;


  //Call first simple expression and assign pointer
  newExpressionNode->firstSimpleExp = simpleExpression();
  
  //Loop while nextToken is an expression operator
  while (nextToken == TOK_EQUALTO || nextToken == TOK_LESSTHAN || nextToken == TOK_GREATERTHAN || nextToken == TOK_NOTEQUALTO)
  {
    //Check for type of operator, output it, and call simpleExpression again to loop
    switch( nextToken )
    { //Assign operators and remaining simple expressions
      case TOK_EQUALTO:          
        if (printParse){output("EQUALTO"); cout << psp() << yytext << endl;}
        newExpressionNode->restSimpleExpOps.push_back(nextToken); lex();
        newExpressionNode->restSimpleExp.push_back(simpleExpression()); break;
      
      case TOK_LESSTHAN:        
        if (printParse){output("LESSTHAN"); cout << psp() << yytext << endl;}
        newExpressionNode->restSimpleExpOps.push_back(nextToken); lex();
        newExpressionNode->restSimpleExp.push_back(simpleExpression()); break;
      
      case TOK_GREATERTHAN:           
        if (printParse){output("GREATERTHAN"); cout << psp() << yytext << endl;}
        newExpressionNode->restSimpleExpOps.push_back(nextToken); lex();
        newExpressionNode->restSimpleExp.push_back(simpleExpression()); break;
      
      case TOK_NOTEQUALTO:           
        if (printParse){output("NOTEQUALTO"); cout << psp() << yytext << endl;}
        newExpressionNode->restSimpleExpOps.push_back(nextToken); lex();
        newExpressionNode->restSimpleExp.push_back(simpleExpression()); break;
    }
  }

  --level;
  if (printParse) cout << psp() << "exit <expression>" << endl;

  return newExpressionNode;
}

SimpleExpNode* simpleExpression()
{
  //Ensure simple expression starts with an accepted token
  if (first_of_simpleExpression)
  {
    if (printParse) output("SIMPLE_EXP");
  }
  else throw "901: illegal type of simple expression";

  if (printParse) cout << psp() << "enter <simple_exp>" << endl;

  //Create Node
  SimpleExpNode* newSimpleExp = nullptr;
  newSimpleExp = new SimpleExpNode(level);

  ++level;

  //Call term and assign pointer
  newSimpleExp->firstTerm = term();

  //Loop while nextToken is a simple expression operator
  while (nextToken == TOK_PLUS || nextToken == TOK_MINUS || nextToken == TOK_OR)
  {
    //Check for type of operator, output it, and call term again to loop
    switch( nextToken )
    { //Assign operators and remaining terms
      case TOK_PLUS:          
        if (printParse){output("PLUS"); cout << psp() << yytext << endl;}
        newSimpleExp->restTermOps.push_back(nextToken); lex();
        newSimpleExp->restTerm.push_back(term()); break;
      
      case TOK_MINUS:        
        if (printParse){output("MINUS"); cout << psp() << yytext << endl;}
        newSimpleExp->restTermOps.push_back(nextToken); lex();
        newSimpleExp->restTerm.push_back(term()); break;
      
      case TOK_OR:           
        if (printParse){output("OR"); cout << psp() << yytext << endl;}
        newSimpleExp->restTermOps.push_back(nextToken); lex();
        newSimpleExp->restTerm.push_back(term()); break;
    }
  }

  --level;
  if (printParse) cout << psp() << "exit <simple_exp>" << endl;

  return newSimpleExp;
}

TermNode* term()
{
  //Ensure term starts with an accepted token
  if (first_of_term)
  {
    if (printParse) output("TERM");
  }
  else throw "902: illegal type of term";

  if (printParse) cout << psp() << "enter <term>" << endl;

  //Create Node
  TermNode* newTerm = nullptr;
  newTerm = new TermNode(level);

  ++level;

  //Call factor and assign pointer
  newTerm->firstFactor = factor();
  
  //Loop while nextToken is a term operator
  while (nextToken == TOK_MULTIPLY || nextToken == TOK_DIVIDE || nextToken == TOK_AND)
  {
    //Check for type of operator, output it, and call factor again to loop
    switch( nextToken )
    { //Assign operators and remaining factors
      case TOK_MULTIPLY:          
        if (printParse){output("MULTIPLY"); cout << psp() << yytext << endl;}
        newTerm->restFactorOps.push_back(nextToken); lex();
        newTerm->restFactor.push_back(factor()); break;
      
      case TOK_DIVIDE:        
        if (printParse){output("DIVIDE"); cout << psp() << yytext << endl;}
        newTerm->restFactorOps.push_back(nextToken); lex();
        newTerm->restFactor.push_back(factor()); break;
      
      case TOK_AND:           
        if (printParse){output("AND"); cout << psp() << yytext << endl;}
        newTerm->restFactorOps.push_back(nextToken); lex();
        newTerm->restFactor.push_back(factor()); break;
    }
  }

  --level;
  if (printParse) cout << psp() << "exit <term>" << endl;

  return newTerm;
}

FactorNode* factor()
{
  //Ensure factor starts with an accepted token
  if (first_of_factor)
  {
    if (printParse) output("FACTOR");
  }
  else throw "903: illegal type of factor";

  if (printParse) cout << psp() << "enter <factor>" << endl;
  ++level;
  
  //Create necessary nodes
  FactorNode* newFactor = nullptr;
  NestedExprNode* newNestedExp = nullptr;
  NotNode* newNot = nullptr;
  MinusNode* newMinus = nullptr;

  //Check for type of operator and output it
  switch( nextToken )
  { //Assign correct nodes to newFactor
    case TOK_INTLIT:                                                   //Cast string to float
      if (printParse){output("INTLIT");} newFactor = new IntLitNode(level, atof(yytext));
       if (printParse){cout << psp() << yytext << endl;} lex(); break;
    
    case TOK_FLOATLIT:                                                 //Cast string to float
      if (printParse){output("FLOATLIT");} newFactor = new FloatLitNode(level, atof(yytext));
       if (printParse){cout << psp() << yytext << endl;} lex(); break;
    
    case TOK_IDENT:           
      if (printParse){output("IDENTIFIER"); cout << psp() << yytext << endl;}

      newFactor = new IdNode(level, yytext);

      //Ensure identifier has been declared
      if(!inSymbolTable( string(yytext) )) throw "104: identifier not declared";
      lex(); break;
    
    case TOK_OPENPAREN:
      if (printParse){output("OPENPAREN"); cout << psp() << yytext << endl;} lex();

      //Call expression and assign pointer
      newNestedExp = new NestedExprNode(level);
      newNestedExp->Expression = expression();
      newFactor = newNestedExp;

      if (nextToken == TOK_CLOSEPAREN){ if (printParse) output("CLOSEPAREN");}
      else throw "4: ')' expected"; lex(); break;

    //Call factor again since these negate the function
    case TOK_NOT:             
      if (printParse){output("NOT"); cout << psp() << yytext << endl;} lex();

      //Call factor and assign pointer
      newNot = new NotNode(level);
      newNot->Factor = factor();
      newFactor = newNot; break;
    
    case TOK_MINUS:           
      if (printParse){output("MINUS"); cout << psp() << yytext << endl;} lex();

      //Call factor and assign pointer
      newMinus = new MinusNode(level);
      newMinus->Factor = factor();
      newFactor = newMinus; break;
  }

  --level;
  if (printParse) cout << psp() << "exit <factor>" << endl;

  return newFactor;
}