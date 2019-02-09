/******************************************************************************
 * STRUCT NAME:                 TokenLexPair                                  *
 * CLASS NAME:                  Lexer                                         *
 * FOR COURSE:                  CPSC 323 - Compilers and Program Languages    *
 *                              MW 11:30AM – 12:45PM                          *
 *****************************************************************************/
/*============================================================================
PURPOSE OF THE STRUCT:
 * The object of struct TokenLexPair will keep track
 * of all types of tokens and their lexemes.
PURPOSE OF THE CLASS:
 * The objects of this class will keep track of legal lexemes
 * into defined token groups as following:
 * Identifier, Keyword, Separator, Operator, Integer, Real, Unknown.
MEMBER FUNCTIONS OF THE CLASS:
 * Lexer(): IDstate(0),IntRealState(0)
 *    This constructor function will accept from the calling program and
 *    initial values for Identifier's and IntReal's state are all all zeros.
 * Lexer(std::istream &stream)
 *    This parameterized constructor will receive the inputs
 *    as the strings of data from the external file.
 * ~Lexer()
 *    This destructor will be called automatically
 *    when an object of Lexer is out of scope.
 * void setStream(std::istream &stream)
 *
 * std::vector<TokenLexPair> getTokens()
 *     This function will get the legal/illegal tokens and their corresponding lexemes,
 *     wrap into TokenLexPair objects, and stores those objects into a vector.
 * TokenLexPair getPair();
 *     This helper function will return a pair of token and lexemme
 *
 * void ID_FSM(const char &c)
 *      A mutator for changing values inside ID_FSM_TABLE_
 * void INT_REAL_FSM(const char &c)
 *      A mutator for changing values inside INT_REAL_FSM_TABLE_
 * void updateFSM(const char &c)
 * void resetFSM()
 * unsigned int Peek_INT_REAL_FSM(const char&c)
 * unsigned int Peek_ID_FSM(const char&c)
 *
 * int isSeparator(const char &c)
 *     Is inputted character a legal one-character Seperator?
 * int isSeparator(const std::string &s)
 *     Is inputted character a legal double-character Seperator?
 * int isOperator(const char &c)
 *     Is inputted character a legal one-character Operator?
 * int isOperator(const std::string &s)
 *     Is inputted character a legal double-character Operator?
 * bool isKeyword(const std::string &s)
 *     Is inputted character a keyword?
 * bool isTerminator(const char &c)
 *     Is inputted character a legal Terminator?
 *
MEMBER DATA OF THE CLASS:
 * const unsigned int ID_FSM_TABLE[4][3]
 *   A 4x3 matrix with fixed values that represent states for Identifier
 * const unsigned int INT_REAL_FSM_TABLE[5][3]
 *   A 5x3 matrix with fixed values that represent states for Int and Real
 * IDstate
 *   Stores the state of the FSM that represents the Identifiers.
 *   The only valid state is state 2,
 *   or in this programs representation only when ISstate == 1
 * *Input
 *   is a pointer to other istream objects.
 *   This allows all child objects of istream to interface with this class.
 * IntRealState
 *   stores the state of the FSM that represents Integers or Reals.
 *   The only valid states is state 2 or 4, or in this program representation.
 *   When IntRealState == state 1 ->INTEGERS
 *   When IntRealState == state 3 -> REALS
 *
 * const std::unordered_set<std::string> keyword_set
 *   The set of defined keywords.
 * const std::unordered_set<std::string> separator_set
 *    The set of defined separators.
 * const std::unordered_set<std::string> operator_set
 *    The set of defined operators.
 *===========================================================================*/
#ifndef Lexer_hpp_
#define Lexer_hpp_

#include <string>
#include <fstream>       // for open external files for reading and writing
#include <sstream>       // for I/O string stream
#include <unordered_set>
#include <vector>
#include <iostream>     // for I/O output on console
//=============================================================================
// TokenLexPair Struct: Keep track of all types of tokens and their lexemes
//=============================================================================
struct TokenLexPair {
    std::string token;
    std::string lexeme;
};
//=============================================================================
// Lexor class and its methods
//============================================================================
class Lexer {
    public:
//**-> Default constructor with default state is 0 all inputted characters
    Lexer():IDState_(0),IntRealState_(0){ Input_ = nullptr; }
//**-> Parameterized constructor
    Lexer(std::istream &stream):Input_(&stream),IDState_(0),IntRealState_(0){}
//**-> Destructor
~Lexer(){ Input_ = nullptr; }
void setStream(std::istream &stream){ Input_ = &stream; }
//**-> Vector of tokens and their lexemes
std::vector<TokenLexPair> getTokens();
    private:
//=============================================================================
// The list of functions that check the validity of the inputted characters
// against the defined sets.
/*=============================================================================
 Note: The int-return type functions will have the following expecting values
       and the corresponding meanings:
        -1 no match
        0 partial match
        1 perfect match
=============================================================================*/
    int isSeparator(const char &c);
    int isSeparator(const std::string &s);
    int isOperator(const char &c);
    int isOperator(const std::string &s);
    
    bool isKeyword(const std::string &s);
    bool isTerminator(const char &c);
/*===========================================================================
getPair()
//===========================================================================
- This function will either try to read til it hits EOF
  or it will read til a valid terminator.
 - Valid Terminators: Separators, Operators, Whitespace,
                      or whatever makes isTerminator return true.
- It will loop through the stream getting character by character:
  + If the characters inputted are not terminators,
    then getPair() will start building a lexeme string.
  + Every time we input another character, the finite state machines update.
  + Once it hits a character it considers a terminator,
    then the function do the following:
   -> By the end of it all, either one of the finite machines
      are in a valid state, or none of them are in a valid state.
   -> Based on the state of the machine, we can determine what kind of lexemme
      it is under the the sets of: INTS, REALS, UNKNOWN STRINGS.
//===========================================================================*/
    TokenLexPair getPair();
//=============================================================================
// Private Memeber Variables
//=============================================================================
    unsigned int IDState_;
    std::istream *Input_;
    unsigned int IntRealState_;
    //=========================================================================
    // The sets of legal tokens
    //=========================================================================
    // The set of defined keywords
    const std::unordered_set<std::string> KeywordSet_ = { "while",
         "whileend","return","if","ifend","true","false","int","boolean",
         "real","else","function","put","get" };
    
    // The set of defined Separators
    const std::unordered_set<std::string> SeparatorSet_ = {";","{",
         "}", "(",")", ",", "[", "]", ":", "$$"};
    
    // The set of defined Operators
    const std::unordered_set<std::string> OperatorSet_ = {"+","-","*",
         "/","=", "==", "^=", ">", "<", "=>", "=<"};
    //=========================================================================
    // 2 Finite State Machines(FSMs)
    //=========================================================================
    // THE TABLES WE REFERENCE ALL OF THESE ARE CONSTANTS
    const unsigned int INT_REAL_FSM_TABLE_[5][3] ={  /* d . ?*/
        /*1*/ {1,4,4},
        /*V2*/{1,2,4},
        /*3*/ {3,4,4},
        /*V4*/{3,4,4},
        /*DeadState*/    /*5*/ {4,4,4}};
    const unsigned int ID_FSM_TABLE_[4][3]= {       /* l d ? */
        /*1*/  {  1,3,3  },
        /*V2*/ {  1,2,3  },
        /*3*/  {  1,2,3  },
        /*DeadState*/    /*4*/  {  3,3,3  }};
    //END THE TABLES WE LOOK TO WHEN TRYING TO IDENTIFY A POTENTIAL LEXEMME
    //========================================================================
    // Mutators for both FSMs
    //========================================================================
    void ID_FSM(const char &c);
    void INT_REAL_FSM(const char &c);
    //========================================================================
    // Helper functions for the two FSMs
    //========================================================================
    void updateFSM(const char &c);
    void resetFSM();
    
    unsigned int Peek_INT_REAL_FSM(const char&c);
    unsigned int Peek_ID_FSM(const char&c);
};
#endif /* Lexer_hpp_ */
