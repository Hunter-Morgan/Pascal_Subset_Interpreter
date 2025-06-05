#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <cstring>
#include "lexer.h"
#include "parser.h"
#include "nodes.h"

using namespace std;

extern "C" {
    // Instantiate global variables
    extern FILE *yyin;     // input stream
    extern FILE *yyout;    // output stream
    extern int   yyleng;   // length of current lexeme
    extern char *yytext;   // text of current lexeme
    extern int   yylineno; // line number for current lexeme
    extern int   yylex();  // the generated lexical analyzer
    extern int   yylex_destroy(); // deletes memory allocated by yylex
}

// Needed global variables
extern int nextToken; // token returned from yylex
extern bool printParse; // whether to print while parsing
bool printTree; // whether to print the parse tree
extern bool printDelete; // whether to delete the parse tree
bool printSymbol;

//*****************************************************************************
// The main processing loop
//
int main(int argc, char* argv[]) {

    printTree = false; // whether to print parse tree
    printSymbol = false; // whether to print symbol table

    // Process any command-line switches
    for(int i = 1; i < argc; i++) {
        // -p flag: if requested, reverse printParse variable
        if(strcmp(argv[i], "-p") == 0) {
        printParse = !printParse;
        }
        // -t flag: if requested, reverse printTree variable
        if(strcmp(argv[i], "-t") == 0) {
        printTree = !printTree;
        }
        // -d flag: if requested, reverse printDelete variable
        if(strcmp(argv[i], "-d") == 0) {
        printDelete = !printDelete;
        }
        // -s flag: if requested, reverse printSymbol variable
        if(strcmp(argv[i], "-s") == 0) {
        printSymbol = !printSymbol;
        }
    }
    
    // Set the input stream
    if (argc > 1) {
        if (printParse) printf("INFO: Using the %s file for input\n", argv[1]);
        yyin = fopen(argv[1], "r"); 
    }
    else {
        if (printParse) printf("INFO: Using the sample.pas file for input\n");
        yyin = fopen("sample.pas", "r");
    }
  
    if (!yyin) {
        if (printParse) printf("ERROR: input file not found\n");
        return EXIT_FAILURE;
    }

    // Set the output stream
    yyout = stdout;

    // Get the first token
    nextToken = yylex();

    // Set the root
    ProgramNode* root = nullptr;

    // Run parser
        try {
            // Call program
            root = program();

            if (nextToken != TOK_EOF)
                throw "End of file expected";

          // Print error message  
        } catch (char const *errmsg) {
            cout << endl << "***ERROR:" << endl;
            cout << "On line number " << yylineno << ", near |" << yytext << "|, error type ";
            cout << errmsg << endl;
            return EXIT_FAILURE;
        }

  if (yyin)
    fclose(yyin); // close input stream

  yylex_destroy(); // delete allocated memory

  // Printing and Deleting the tree all result in 
  // the same in-order traversal of the tree as parsing.  All
  // use the call stack.

  if(printTree) { // whether to print the tree
    cout << endl << "*** Print the Tree ***" << endl;

    //Start printing at pointer to root
    cout << *root << endl << endl;
  }

  //Run interpreter
  root->interpret();
  
  if (printSymbol)
  {
    // Print out the symbol table
    cout << endl << "*** User Defined Symbols ***" << endl;
    symbolTableT::iterator it;
    for(it = symbolTable.begin(); it != symbolTable.end(); ++it ) {
      cout << setw(8) << it->first << ": " << it->second << endl;
    }
  }

  if (printDelete) cout << endl << "*** Delete the Tree ***" << endl;

  //Delete allocated memory
  delete root;
  root = nullptr;
//********************************************************************************
    return EXIT_SUCCESS;
}

