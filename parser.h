#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <set>
#include <map>
#include <stdlib.h>
#include <iostream>
#include "nodes.h"

using namespace std;

extern int nextToken;        // next token returned by lexer

extern "C" {
	// Instantiate global variables used by flex
	extern int   yylex();      // the generated lexical analyzer
	extern char* yytext;       // text of current lexeme
}

//Declare lex
int lex();

// Forward declarations of FIRST_OF functions.  These check whether the current 
// token is in the FIRST set of a production rule.
bool first_of_program(void);
bool first_of_block(void);
bool first_of_statement(void);
bool first_of_compoundStatement(void);
bool first_of_expression(void);
bool first_of_simpleExpression(void);
bool first_of_term(void);
bool first_of_factor(void);

// Forward declarations of functions
ProgramNode* program();
BlockNode* block();
StatementNode* statement();
AssignmentStmtNode* assignmentStatement();
CompoundStmtNode* compoundStatement();
IfStmtNode* ifStatement();
WhileStmtNode* whileStatement();
ReadStmtNode* readStatement();
WriteStmtNode* writeStatement();
ExpressionNode* expression();
SimpleExpNode* simpleExpression();
TermNode* term();
FactorNode* factor();

#endif /* PARSER_H */
