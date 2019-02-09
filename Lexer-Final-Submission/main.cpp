/*****************************************************************************
 * FILENAME:                    main.cpp                                     *
 * TITLE:                       Lexical Analyzer (Lexer)                     *
 * SUBMITTED BY TEAM:           Cong Le, Dion Pieterse, San Tran             *
 * FOR COURSE:                  CPSC 323 - Compilers and Program Languages   *
 *                              MW 11:30AM - 12:45PM                         *
 * PROFESSOR:                   Song-James Choi, Ph.D.                       *
 * PROJECT NUMBER:              1                                            *
 * DUE DATE:                    Softcopy on Sunday, October 7, 2018          *
 *                              Hardcopy on Thursday, October 11, 2018       *
 ******************************************************************************/
/*============================================================================
 * PURPOSE OF THE PROGRAM:
 * This program will contruct 2 Finite State Machines
 * (one for Identifier Token group and one for Integer and Real Token group)
 * The program will read a .txt file contanining
 * the source code of Rat18F to generate legal tokens.
 * Then, it can display the results(token groups and lexems instances) on the console,
 * including legal and illegal cases.
 * Finally, it writes out the results to an output as .txt files.
 *
 * FUNCTIONS:
 * Lexer()
 *  will read characters from the source .txt file into two Finite State Machines
 *  and, upon exit, return legal and illegal lexemes with defined Token Groups.
 *
 * INCLUDED FILES:
 *     Lexor.hpp
 *
 * DATA FILES:
 *     TestCase1.txt
 *     TestCase2.txt
 *     TestCase3.txt
  * OUTPUT FILES:
 *     ResultCase1.txt
 *     ResultCase2.txt
 *     ResultCase3.txt
==============================================================================*/
#include "Lexer.hpp"
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>
using namespace std;

int main() {
//=============================================================================
//**-> Declare some nessesary variables
//=============================================================================
ifstream istrm;
ofstream ostrm;
//=============================================================================
//**-> Open the input from a text file
//=============================================================================
istrm.open("TestCase1.txt");
if(istrm.is_open())
    cout << "Input file opened." << endl;
else
    cerr << "Error: Cannot open input file." << endl;
//=============================================================================
/**-> Run the Lexor() over the input file,
 and collect legal tokens into a list */
//=============================================================================
Lexer lex(istrm);
vector<TokenLexPair> list= lex.getTokens();
//=============================================================================
//**-> Output the result on the console
//=============================================================================
cout<<setw(25)<<left<< "Token: "<<setw(25)<< "Lexemme: "<<endl;
for(vector<TokenLexPair>::iterator i=list.begin();i!=list.end();i++){
    cout<<setw(25)<<i->token <<setw(25)<< i->lexeme<<endl;
}
//=============================================================================
//**-> Open the output file an write on it
//=============================================================================
ostrm.open("ResultCase1.txt");
if(ostrm.is_open()) {
    // Formating the header for the output file
    cout << "Output file opened." << endl;
    ostrm << left<< setw(25) << "Output:" << '\n';
    ostrm << setw(25) << "Token" << setw(25) << "Lexeme" << '\n';
    ostrm << string(50, '-') << '\n';

    // Writing the legal tokens on to the .txt file
    for(vector<TokenLexPair>::iterator i=list.begin();i!= list.end();i++){
        ostrm<<setw(25)<<i->token <<setw(25)<< i->lexeme<<endl;
    }
    cout << "Succefully write onto the output .txt file!" << endl;
    }
else {
    cerr << "Error: Cannot Open output file." << endl;
}
//=============================================================================
//**-> Closing the input and output text files and exit.
//=============================================================================
istrm.close();
ostrm.close();
return 0;
}
