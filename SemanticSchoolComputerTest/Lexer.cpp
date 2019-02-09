/******************************************************************************
 * FOR COURSE:                  CPSC 323 - Compilers and Program Languages    *
 *                              MW 11:30AM – 12:45PM                          *
 * CLASS:                       Functions for class Lexer.                    *
 *****************************************************************************/
#include "Lexer.h"

void Lexer::getFile(string &fileName){
    inputStream.open(fileName);
    if(inputStream.is_open())
        cout << "Input file opened." << endl;
    else{
        cerr << "Error: Cannot open input file." << endl;
        exit(EXIT_FAILURE);
    }
    
    // pointing to the first char in the text file
    Input_ = &inputStream;
    
    // get the tokens from the input text file
    getTokens();
    
    // close the input text file
    inputStream.close();
}

void Lexer::printTokenPairsToConsole(){
    //**-> Output the result on the console
    cout<<setw(15)<<left<< "Token: "<<setw(13)<< "Lexemme: "<<setw(10)<< "Line #:"<< "char position:"<<endl;
    cout << string(50, '-') << '\n';
    for(vector<TokenLexPair>::iterator i=list.begin(); i != list.end(); i++){
        cout<<setw(15)<<i->token <<setw(15)<< i->lexeme << setw(13)<< i->lineNum<< i->charNum<<endl;
    }
}
void Lexer::printTokenPairsToTxtFile(){
    outputStream.open("Lexical_Analysis_Result.txt");
    if(outputStream.is_open()) {
        // Formating the header for the output file
        cout << "Output file opened." << endl;
        outputStream << left<< setw(25) << "Output:" << '\n';
        outputStream<<setw(15)<<left<< "Token: "<<setw(13)<< "Lexemme: "<<setw(10)<< "Line #:"<< "char position:"<<endl;
        outputStream << string(50, '-') << '\n';
        
        // Writing the legal tokens on to the .txt file
        for(vector<TokenLexPair>::iterator i=list.begin(); i != list.end(); i++){
            outputStream<<setw(15)<<i->token <<setw(15)<< i->lexeme << setw(13)<< i->lineNum<< i->charNum<<endl;
        }
        
        cout << "Succefully write onto the output .txt file!\n" << endl;
    }
    else {
        cerr << "Error: Cannot Open output file." << endl;
    }
    
    // close the output text file
    outputStream.close();
}

void Lexer::updateLinePosition(char c){
    if(c == '\n') {
        ++lineNum;
        charNum=1;
    }
    else if( c== '\t') {
        charNum+=3;
    }
    else{
        ++charNum;
    }
}

bool Lexer::isspace_string(std::string s){
    for(int i=0;i<s.size();i++) {
        if(!isspace(s[i])) {
            return false;
        }
    }
    return true;
}
//=============================================================================
// The implementation of mutator for 2 tables
//=============================================================================
// Mutator for ID_FSM_TABLE_
void Lexer::ID_FSM(const char &c){
    // If the inputted char is a letter then its state is 0,
    // a digit then its state is 1, otherwise its state is 2
    unsigned int state =(isalpha(c)? 0 : (isdigit(c)? 1 : 2 ));
    
    // Update the new position on the ID_FSM_TABLE_
    // according to the inputted character being represented
    IDState_ = ID_FSM_TABLE_[IDState_][state];
}
// Mutator for INT_REAL_FSM_TABLE_
void Lexer::INT_REAL_FSM(const char&c){
    // If the inputted character is a digit then its state is 0,
    // a '.' then its state is 1, otherwise its state is 2.
    unsigned int state =(isdigit(c) ? 0 :(c == '.'? 1 : 2));
    
    // Update the new position on the INT_REAL_FSM_TABLE_
    // according to the inputted character being represented
    IntRealState_ = INT_REAL_FSM_TABLE_[IntRealState_][state];
}

//=============================================================================
// The implementation of helper functions used by both FSMs
//=============================================================================
// Updating the state of the lexeme having multiple characrters in FSMs.
void Lexer::updateFSM(const char &c){
    // Getting the passed one-char argument
    // and send it to 2 FSMs for concurrently cheking its validity.
    ID_FSM(c);
    INT_REAL_FSM(c);
}
// Peeking to the next character in the inputted stream
unsigned int Lexer::Peek_ID_FSM(const char& c){
    unsigned int temp_ID_state = IDState_;
    // If the next character in the inputted stream is a letter,
    // then its state is 0, a digit then its state is 1,
    // otherwise its state is 2.
    unsigned int state =(isalpha(c)? 0 : (isdigit(c)? 1 : 2 ));
    
    // Update the new position on the ID_FSM_TABLE_
    // according to the inputted character being represented
    return ID_FSM_TABLE_[temp_ID_state][state];
}
// Peeking to the next character in the inputted stream
unsigned int Lexer::Peek_INT_REAL_FSM(const char& c){
    unsigned int temp_INT_REAL_state = IntRealState_;
    // If the next character in the inputted stream is a digit,
    // then its state is 0, a '.' then its state is 1,
    // otherwise its state is 2.
    unsigned int state =(isdigit(c) ? 0 :(c=='.'? 1 : 2));
    
    // Update the new position on the INT_REAL_FSM_TABLE_
    // according to the inputted character being represented
    return INT_REAL_FSM_TABLE_[temp_INT_REAL_state][state];
}
// Reset all the FSMs back to the starting state, which is zero.
void Lexer::resetFSM(){
    IDState_ = 0;
    IntRealState_ = 0;
}
/*=============================================================================
 // The list of implementations of functions
 that check the legality of the inputted characters against the defined sets.
 // Note: The int-return type functions will have the following expecting values
 and their corresponding meanings:
 -1 no match
 0 partial match
 1 perfect match
 //===========================================================================*/
// Is the passed string argument in the set of defined keywords?
bool Lexer::isKeyword(const std::string& s){
    // Loop through the set of defined keywords and find a match
    std::unordered_set<std::string>::const_iterator search_for = KeywordSet_.find(s);
    if(search_for == KeywordSet_.end()) {
        // if it reaches the end of the set and nothing is matched, then
        return false;
    }
    else { return true; }
}
// Is the passed character argument a terminator?
bool Lexer::isTerminator(const char &c){
    //It is considered a terminator as long as the follwing is true
    //It is a partial or perfect match for isSeparator, aka isSeparator does not return -1
    //OR, it is a partial or perfect match for isOpeartor, aka isOperator does not return -1
    //OR, it is a space character, aka isspace does not return 0
    //OR, if it encountered a character that makes both IDstate and IntFloatState enter the deadState
    return (isSeparator(c)!=-1||isOperator(c)!=-1||isspace(c)!=0||(Peek_ID_FSM(c)==3 && Peek_INT_REAL_FSM(c)==4));
}
// Is the passed character argument in the set of defined one-char separators?
int Lexer::isSeparator(const char &c){
    // Assume the character does not exist in the list, then
    int matcher_state = -1;
    // A temporary string for checking the validity of the inputted charaters.
    std::string temp;
    // Geting the passed character argument
    temp.push_back(c);
    // Loop through the set of defined one-char separators and find a match
    for(std::unordered_set<std::string>::const_iterator iter = SeparatorSet_.begin(); iter != SeparatorSet_.end(); iter++){
        // When it is not until the end of the string,
        if(iter->find(temp)!=std::string::npos){
            if(*iter == temp){
                // it was an EXACT MATCH for an one-char separator, then
                return 1;
            } else{
                // it was a PARTIAL MATCH,
                // we keep looking for the potential EXACT MATCH, and
                matcher_state = 0;
            }
        }
    }
    // it will return npos = -1 if it was a NO MATCH
    // and 0 if it was a PARTIAL MATCH.
    return matcher_state;
}
// Is the passed string argument in the set of defined double-char separators?
int Lexer::isSeparator(const std::string &s){
    // Assume the string does not exist in the list, then
    int matcher_state = -1;
    // Loop through the set of defined double-char separators and find a match
    for(std::unordered_set<std::string>::const_iterator iter = SeparatorSet_.begin(); iter != SeparatorSet_.end(); iter++){
        // When it is not until the end of the string,
        if(iter->find(s)!=std::string::npos){
            if(*iter == s){
                // it was an EXACT MATCH for an double-char separator, then
                return 1;
            } else{
                // it was a PARTIAL MATCH,
                // we keep looking for the potential EXACT MATCH, and
                matcher_state = 0;
            }
        }
    }
    // it will return npos = -1 if it was a NO MATCH
    // and 0 if it was a PARTIAL MATCH.
    return matcher_state;
}
// Is the passed character argument in the set of defined one-char operators?
int Lexer::isOperator(const char &c){
    // Assume the character does not exist in the list, then
    int matcher_state = -1;
    // A temporary string for checking the validity of the inputted charaters.
    std::string temp;
    // Geting the passed character argument
    temp.push_back(c);
    // Loop through the set of defined one-char operators and find a match
    for(std::unordered_set<std::string>::const_iterator iter = OperatorSet_.begin(); iter != OperatorSet_.end(); iter++){
        // When the iterator is not until the end of the temp string,
        if(iter->find(temp)!= std::string::npos){
            if(*iter == temp) {
                // it was an EXACT MATCH for an one-char operator, then
                return 1;
            } else {
                // it was a PARTIAL MATCH,
                // we keep looking for the potential EXACT MATCH, and
                matcher_state = 0;
            }
        }
    }
    // It will return npos = -1 if it was a NO MATCH
    // and 0 if it was a PARTIAL MATCH.
    return matcher_state;
}
// Is the passed string argument in the set of defined double-char operators?
int Lexer::isOperator(const std::string &s){
    // Assume the character does not exist in the list, then
    int matcher_state = -1;
    // Loop through the set of defined double-char operators and find a match
    for(std::unordered_set<std::string>::const_iterator iter = OperatorSet_.begin(); iter != OperatorSet_.end(); iter++){
        // When the iterator is not until the end of the temp string,
        if(iter->find(s)!=std::string::npos){
            if(*iter == s){
                // it was an EXACT MATCH for an double-char operator, then
                return 1;
            } else{
                // it was a PARTIAL MATCH,
                // we keep looking for the potential EXACT MATCH, and
                matcher_state = 0;
            }
        }
    }
    // it will return npos = -1 if it was a NO MATCH
    // and 0 if it was a PARTIAL MATCH.
    return matcher_state;
}
/*============================================================================
 // The implementation of getPair() that used to read the inputted stream
 of characters and categorize them into valid tokens/lexeme.
 //===========================================================================*/
TokenLexPair Lexer::getPair() {
    
    char c;
    TokenLexPair output;
    output.lineNum = lineNum;
    output.charNum = charNum;
    // Continuously read in the text data file until reach EOF.
    while(Input_->get(c)){
        updateLinePosition(c);
        //============================================================================
        // If we just encountered a terminator, and the string is empty,
        if(!isTerminator(c)){
            // Then the next lexeme we're about to construct
            // will be under the terminator domain.
            updateFSM(c);
            // store the inputted character as a lexeme into a TokenLexPair object.
            output.lexeme.push_back(c);
        }
        //============================================================================
        else if(output.lexeme.empty()){
            std::string potentialSeparator;
            
            // We will peek into the next character in the stream,
            char peeker;
            peeker = Input_->peek();
            // Store the inputted characters to construct a potential
            // two-character separator. We will reference against the tables.
            potentialSeparator.push_back(c);
            potentialSeparator.push_back(peeker);
            //============================================================================
            //If it is the operator this statements execute
            //This looks for exact and partial match.
            //This brings us within the realm of possibilities that it will be an operator.
            // Case 1 for Operator checking:
            if(isOperator(c)!= -1||isOperator(potentialSeparator)!= -1){
                //if the conditions above is matched, then we know for sure the inputted character is at least an one-character operator.
                output.token="Operator";
                //Check if the potential two-character operater is really a valid defined operator.
                //If it is, then we know that new two-character operator is our legal lexemme.
                if(isOperator(potentialSeparator) == 1){
                    //if the conditions above is matched, then we have a two-character operator
                    output.lexeme = potentialSeparator;
                    //Get peeker to take it out of the buffer
                    Input_->get(peeker);
                    updateLinePosition(peeker);
                    resetFSM();
                    return output;
                }
                // Case 2 for Operator checking:
                //else we check if it it was only one character
                else if(isOperator(c) == 1) {
                    output.lexeme.push_back(c);
                    resetFSM();
                    return output; //return out of this function because we know it is a one-character operator.
                }
                // Case 3 for Operator checking:
                else {
                    //If it made it here, it was a partial match for an operator, but it never made an exact match with an operator
                    //so it must be unknown.
                    output.token = "Unknown";
                    output.lexeme.push_back(c);
                    resetFSM();
                    return output;
                }
            }//End isOperator else-if block
            //============================================================================
            //Begin isSeparator else-if block
            //Performs a check to look for partial match with the set of separators
            //same methodology as the operator block
            else if(isSeparator(c)!= -1||isSeparator(potentialSeparator)!= -1) {
                //if it is a comment
                if(c=='[' && peeker== '*'){
                    Input_->get(peeker);
                    updateLinePosition(peeker);
                    char last;
                    char secondLast;
                    output.token ="Comment";
                    while(Input_->get(last)){
                        updateLinePosition(last);
                        if(last == '*'){
                            //If are in here last is a '*'
                            secondLast=Input_->peek();
                            //Now we gotta check if the character after the * is a ]
                            if(secondLast==']'){
                                Input_->get(secondLast);
                                updateLinePosition(secondLast);
                                output.token = "Comment";
                                break;
                            }
                        }
                    }
                }
                //if it is a multi-character separator EXACT MATCH
                else if(isSeparator(potentialSeparator) == 1){
                    //if this is true we have a 2 character operator
                    output.token = "Separator";
                    output.lexeme = potentialSeparator;
                    //get peeker to take it out of the buffer
                    Input_->get(peeker);
                    updateLinePosition(peeker);
                }
                //if it is a single character separator EXACT MATCH
                else if(isSeparator(c) == 1) {
                    //the one-character separator checker.
                    //it looks for exact match.
                    output.token = "Separator";
                    output.lexeme.push_back(c);
                }
                else{
                    //If it doesnt match a comment,
                    //If it doesnt match a two-character separator exactly
                    //If it doesnt match a one-character separator exactly
                    //then it must be an UNKNOWN
                    output.token="Unknown";
                    output.lexeme.push_back(c);
                }
                resetFSM();
                return output;
            }//End isSeparator else-if block
            //============================================================================
            else if(isspace(c) != 0){
                // If this was true, then the string was empty,
                // and we just read in whitespace, so we will ignore it.
                // Do nothing here.
                output.lineNum = lineNum;
                output.charNum = charNum;
            }
            //============================================================================
            else {
                output.lexeme.push_back(c);
                output.token = "Unknown";
                resetFSM();
                // If it is not a separator, if it is not an operator,
                // it must be an unknown single character ex.) %, or #.
                return output;
            }
        }
        //============================================================================
        else{
            //It is terminator, and the string is not empty
            //so we unget the terminator, and we check the state of the finite state machines by breaking out of the while loop
            //that constructs the multi-character lexemmes.
            Input_->unget();
            if(c == '\n')
            {
                lineNum--;
            }
            else if(c =='\t')
            {
                charNum-=3;
            }
            else{
                charNum--;
            }
            break;
        }
    }//END WHILE - multi-character lexemmes (3+characters) have been constructed, check the machines to figure out what it is.
    //============================================================================
    // If it is at the end of the stream, and the lexemme is empty,
    // then we are at the end of the file.
    if(Input_->eof() && output.lexeme.empty()) {
        // Stop read the stream. Nothing to do here.
    }
    //============================================================================
    // Checking the lexeme against the 2 FSMs
    //============================================================================
    else if(IDState_ == 1 && (IntRealState_ != 1||IntRealState_ != 3)) {
        if(isKeyword(output.lexeme)){
            output.token = "Keyword";
        }
        else{
            output.token = "Identifier";
        }
    }
    else if(IDState_ != 1 && IntRealState_ == 1) {
        output.token = "Int";
    }
    else if(IDState_ != 1 && IntRealState_ == 3) {
        output.token = "Real";
    }
    else{
        //Illegal set of strings.
        output.token="Unknown";
    }
    resetFSM();
    return output;
}

/*============================================================================
 // The implementation of getTokens() that used to store data
 into a vector of TokenLexPair objects
 //===========================================================================*/
std::vector<TokenLexPair> Lexer::getTokens(){
    // std::vector<TokenLexPair> output;
    int counter = 0;
    while(!Input_->eof()) {
        TokenLexPair temp = getPair();
        if(temp.token != "Comment"&& !isspace_string(temp.lexeme)) {
            ++counter;
            list.push_back(temp);
        }
    }
    return list;
}
