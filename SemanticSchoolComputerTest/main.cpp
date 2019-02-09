/*****************************************************************************
 * FILENAME:                    main.cpp                                     *
 * TITLE:                       Syntax Analyzer                              *
 * SUBMITTED BY TEAM:           Cong Le, Dion Pieterse, San Tran             *
 * FOR COURSE:                  CPSC 323 - Compilers and Program Languages   *
 *                              MW 11:30AM - 12:45PM                         *
 * PROFESSOR:                   Song-James Choi, Ph.D.                       *
 * PROJECT NUMBER:              3                                            *
 * DUE DATE:                    Softcopy on Monday, November 12, 2018        *
 *                              Hardcopy on Wednesday, November 14, 2018     *
 ******************************************************************************/
/*============================================================================
 * PURPOSE OF THE PROGRAM:
 * This program will contruct 2 Finite State Machines
 * (one for Identifier Token group and one for Integer and Real Token group)
 * The program will read a .txt file contanining
 * the source code of Rat18F to generate tokens and their correspoding lexeme.
 * This program also uses top-down parser to analyze the source code syntax.
 * Then, it can display the results(token groups, lexems instances, production
 * rules used) on the console, including legal and illegal cases.
 * Finally, it writes out the results onto 2 text files.
 * This program may exit upon any syntax errors occured during parsing process.
 * FUNCTIONS:
 * Lexer()
 *  will read characters from the source .txt file into 2 Finite State Machines
 *  and, upon exit, return legal and illegal lexemes with defined Token Groups.
 * Parser()
 *  will analysize the source code for correct syntax constructions.
 *  should write the token and lexeme found,
 *  and print out all productions rules used for analyzing the found token.
 *  will generate a meaningful error message, such as:
 *  token, lexeme, line number, and error type etc.
 * INCLUDED FILES:
 *     Lexor.h
 *     Parser.h
 * DATA FILES:
 *     TestCase1.txt
 *     TestCase2.txt
 *     TestCase3.txt
 * OUTPUT FILES:
 *     Lexical_Analysis_Result.txt
 *     Syntactic_Analysis_Result.txt
 ==============================================================================*/
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>
#include "Lexer.h"
#include "Parser.h"
using namespace std;

int main(int argc, char** argv) {
    //checking for 2 arguments
    if(argc != 2){
        cerr <<  "Invalid number of arguments. Please try again." << endl;
        exit(EXIT_FAILURE);
    }
    else if(argc == 2 ){
        string test_file = argv[1];//convert cstr to string
        //**-> Run the Lexor() over the input file, and collect legal tokens into a list inside Lexer
        Lexer lex;
        lex.getFile(test_file);
        lex.printTokenPairsToConsole();       // print out pairs of token and corresponding lexeme onto console
        lex.printTokenPairsToTxtFile();     // print out pairs of token and corresponding lexeme into a txt file
        
        //**-> Parsing the list of token and its coresponding lexeme, then
        // print out the syntax production rules onto the console and into a txt file
        Parser par(lex.list,true,false);
        
        par.printSymbolOntoConsole();       // print Symbol table onto the console
        par.printInstructionOntoConsole();  // print Instruction table onto the console
        
        par.printSymbolIntoTxtFile();       // print Symbol table into a text file
        par.printInstructionIntoTxtFile();   // print Instruction table into a text file
    }
    return 0;
}
