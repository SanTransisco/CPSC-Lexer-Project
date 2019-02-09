/******************************************************************************
 * FOR COURSE:                  CPSC 323 - Compilers and Program Languages    *
 *                              MW 11:30AM – 12:45PM                          *
 * CLASS:                       Functions for class Parser.                   *
 *****************************************************************************/

#include "Parser.h"

// Global variables
#define NIL -9999
enum EXPRESSION_EXCEPTION { BOOL, INT, EPSILON  };
unsigned MEMORY_ADDRESS = 5000;
bool IS_FROM_SCAN = false;
bool IS_FROM_DECLARATION = false;
bool IS_FROM_PRINT =false;
string prevLexeme;
string tempSymbol;
//=================================================================================
// Parameterized constructor
Parser::Parser(vector<TokenLexPair> &tok_lex_list, bool comments, bool char_output) {
    //top down parser, start at RAT
    comment_switch = comments;          // use to turn on/off printing syntax rules
    character_output = char_output;
     instructionIndex = 1;              // instruction count starting from 1
    iter = tok_lex_list.begin();
    
    try{
        RAT(iter);
         // print Congratulations on the console
        cout << "Compilation Succesful\n";
        cout<<"Congratulation! Your syntax is 100% correct!\n";
        
        // print congratulations on a text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream << "Compilation Succesful\n";
            outputStream<<"Congratulations! Your syntax is 100% correct!\n";
        }
        // close the output text file
        outputStream.close();
        
        
    }
    // catch and print out all types of errors
    catch(Parser_Exception &e) {
        // error message on console
        cerr<<e<<endl;
        cerr << "Our program will exit due to encountering syntax error in source code.\n";
        // error message on the txt file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<<e<<endl;
            outputStream << "Our program will exit due to encountering syntax error in source code.\n";
        }
        // close the text file
        outputStream.close();
        
        // exit the program due to erro in parsing process
        exit(1);
    }
}

// print outputs onto the console and into a text file
void Parser::print_tok_lexeme(std::vector<TokenLexPair>::iterator &iter) {
    if(comment_switch){
        // print outputs on the console
        cout << string(80, '*') << endl;
        cout << "*** FOUND *** Token: " << iter->token << " | " << "Lexeme: " << iter->lexeme << endl;
        cout << string(80, '*') << endl;
        
        // print outputs on a text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream << string(80, '*') << endl;
            outputStream << "*** FOUND *** Token: " << iter->token << " | " << "Lexeme: " << iter->lexeme << endl;
            outputStream << string(80, '*') << endl;
        }
        // close the text file
        outputStream.close();
    }
}

// R1. <Rat18F>  ::=   <Opt Function Definitions>   $$  <Opt Declaration List>  <Statement List>  $$
void Parser::RAT(std::vector<TokenLexPair>::iterator &it){
    
    //switched print statement
    if(comment_switch) {
         // print production rule on the console
        cout << "<Rat18F>  ::=   <Opt Function Definitions>   $$  <Opt Declaration List>  <Statement List>  $$" << endl << endl;
        
        // print production rule on a text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream << "<Rat18F>  ::=   <Opt Function Definitions>   $$  <Opt Declaration List>  <Statement List>  $$" << endl << endl;
        }
        // close the output text file
        outputStream.close();
    }
    OFD(it);
    if(it->lexeme == "$$") {
        ++it;
        print_tok_lexeme(it);
        ODL(it);
        SL(it);
        if(it->lexeme == "$$") {
            print_tok_lexeme(it);
            return;
        }
        else{
            // error message
            Parser_Exception e= "Expecting a $$ at the end of main.\n";
            throw e;
        }
    }
    // error message
    else{
        Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
        +":"+"Fatal Error, Expecting \"$$\" before \""+it->lexeme+"\"";
        throw e;
    }
}
// R2. <Opt Function Definitions> ::= <Function Definitions> |  <Empty>
void Parser::OFD(std::vector<TokenLexPair>::iterator &it){
    //switched print statement
    if(comment_switch) {
        // print production rule on console
        cout << "<Opt Function Definitions> ::= <Function Definitions> | <Empty>" << endl << endl;
        
       // print production rule on a text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Opt Function Definitions> ::= <Function Definitions> | <Empty>" << endl << endl;
        }
        // close the output text file
        outputStream.close();
    }
    try{
        FD(it);
    }
    catch(NON_FATAL_ERROR &e) {
        //It was empty
        //or we were at the end of the OPL
        return;
    }
}

// R3. <Function Definitions>  ::= <Function> <Function Definition Prime>
void Parser::FD(std::vector<TokenLexPair>::iterator &it){
     //switched print statement
    if(comment_switch) {
        // print production rule on console
        cout << "<Function Definitions>  ::= <Function> <Function Definition Prime>" << endl << endl;
        
         // print production rule on a text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Function Definitions>  ::= <Function> <Function Definition Prime>" << endl << endl;
        }
        // close the output text file
        outputStream.close();
    }
    F(it);
    FD_Prime(it);
}

// R4. <Function Definition Prime> ::= <Function Definition> | epsilon
void Parser::FD_Prime(std::vector<TokenLexPair>::iterator &it){
     //switched print statement
    if(comment_switch) {
        // print production rule on console
        cout << "<Function Definition Prime> ::= <Function Definition> | \u03B5" << endl << endl;
        
        // print production rule on a text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Function Definition Prime> ::= <Function Definition> | \u03B5" << endl << endl;
        }
        // close the output text file
        outputStream.close();
        
    }
    FD(it);
    // do nothing for epsilon case
    
}

// R5. <Function> ::= function <Identifier> (<Opt Parameter List>) <Opt Declaration List> <Body>
void Parser::F(std::vector<TokenLexPair>::iterator &it){
     //switched print statement
    if(comment_switch) {
        // print production rule on console
        cout << "<Function> ::= function <Identifier> (<Opt Parameter List>) <Opt Declaration List> <Body>" << endl << endl;
        
         // print production rule on a text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Function> ::= function <Identifier> (<Opt Parameter List>) <Opt Declaration List> <Body>" << endl << endl;
        }
        // close the output text file
        outputStream.close();
    }
    print_tok_lexeme(it);
    if(it->lexeme == "function"){
        //cout << "inside if for function..." << endl;
        print_tok_lexeme(it);
        ++it;
        //I(it);
        if(it->token == "Identifier") {
            //cout << "inside if for function..." << endl;
            print_tok_lexeme(it);
            ++it;
            if(it->lexeme == "("){
                //  cout << "inside if for function..." << endl;
                print_tok_lexeme(it);
                ++it;
                OPL(it);
                //cout << "*** Token to verify -> Token: " << it->token << " " << "Lexeme: " << it->lexeme << endl << endl;
                if(it->lexeme ==")"){
                    //cout << "**** inside if for function..." << endl;
                    print_tok_lexeme(it);
                    ++it;
                    ODL(it);
                    B(it);
                }
                else{
                    Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
                    +":"+"Fatal Error, Expecting \")\" before \""+it->lexeme+"\"";
                    throw e;
                }
            }
            else{
                //ERROR HERE WITH (
                Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
                +":"+"Fatal Error, Expecting \"(\" before \""+it->lexeme+"\"";
                throw e;
            }
        }
        else {
            Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
            +":"+"Fatal Error, \""+it->lexeme+"\" is not a valid identifier for function definition";
            throw e;
        }
    }
    else{
        //This is because if it is no longer a function definition it should  be $$
        //which is not a fatal error so we can go back up the stack.
        NON_FATAL_ERROR e = true;
        throw e;
    }
}

// R6. <Opt Parameter List> ::=  <Parameter List>    |     <Empty>
void Parser::OPL(std::vector<TokenLexPair>::iterator &it){
     //switched print statement
    if(comment_switch) {
        // print production rule on console
        cout << "<Opt Parameter List> ::=  <Parameter List> | <Empty>" << endl << endl;
        
         // print production rule on a text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Opt Parameter List> ::=  <Parameter List> | <Empty>" << endl << endl;
        }
        // close the output text file
        outputStream.close();
    }
    try{
        PL(it);
    }
    catch(NON_FATAL_ERROR &e){
        //It was empty
        //Or we were at the end of the OPL
        return;
    }
    
}

// R7. <Parameter List>  ::=  <Parameter>    |     <Parameter> , <Parameter List>
void Parser::PL(std::vector<TokenLexPair>::iterator &it){
     //switched print statement
    if(comment_switch) {
        // print production rule on console
        cout << "<Parameter List>  ::=  <Parameter> | <Parameter> , <Parameter List>" << endl << endl;
        
         // print production rule on a text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Parameter List>  ::=  <Parameter>    |     <Parameter> , <Parameter List>" << endl << endl;
        }
        // close the output text file
        outputStream.close();
    }
    P(it);
    if(it->lexeme == ",") {
        print_tok_lexeme(it);
        ++it;
        PL(it);
        return;
    }
}

// R8. <Parameter> ::=  <IDs > : <Qualifier>
void Parser::P(std::vector<TokenLexPair>::iterator &it){
     //switched print statement
    if(comment_switch) {
        // print production rule on console
        cout << "<Parameter> ::=  <IDs > : <Qualifier>" << endl << endl;
        
         // print production rule on a text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Parameter> ::=  <IDs > : <Qualifier>" << endl << endl;
        }
        // close the output text file
        outputStream.close();
    }
    try{
        IDs(it);
    }
    catch(Parser_Exception &e) {
        NON_FATAL_ERROR f = true;
        throw f;
    }
    if(it->lexeme == ":") {
        print_tok_lexeme(it);
        ++it;
        Q(it);
    }
    else{
        --it;
        // error message
        Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
        +":"+"In Parameter list: Fatal Error, Expecting \":\" before \""+it->lexeme+"\"";
        ++it;
        throw e;
    }
}

// R9. <Qualifier> ::= int     |    boolean    |  real
void Parser::Q(std::vector<TokenLexPair>::iterator &it){
     //switched print statement
    if(comment_switch) {
        // print production rule on console
        cout << "<Qualifier> ::= int     |    boolean    |  real" << endl << endl;
        
        // print production rule on a text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Qualifier> ::= int     |    boolean    |  real" << endl << endl;
        }
        // close the output text file
        outputStream.close();
        
    }
    // current lexeme type is integer
    if(it->lexeme == "int"){
        currentLexemeType = it->lexeme;
        if(comment_switch){
            cout << "<Qualifier> -> integer\n";
        }
        print_tok_lexeme(it);
        ++it;
        return;
    }
    // current lexeme type is boolean
    else if(it->lexeme == "boolean" ){
        currentLexemeType = it->lexeme;
        if(comment_switch){
            cout << "<Qualifier> -> boolean\n";
        }
        print_tok_lexeme(it);
        ++it;
        return;
    }
    // current lexeme type is real, not used due to simplicity
    else if(it->lexeme == "real"){
        print_tok_lexeme(it);
        ++it;
        return;
    }
    // error messege
    else{
        Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
        +":"+"Fatal Error, \""+it->lexeme+"\" is not a valid qualifier";
        throw e;
    }
}


// R10. <Body>  ::=  {  < Statement List>  }
void Parser::B(std::vector<TokenLexPair>::iterator &it){
     //switched print statement
    if(comment_switch) {
        // print production rule on console
        cout << "<Body>  ::=  {  < Statement List>  }" << endl << endl;
        
         // print production rule on a text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Body>  ::=  {  < Statement List>  }" << endl << endl;
        }
        // close the output text file
        outputStream.close();
    }
    if(it->lexeme == "{") {
        print_tok_lexeme(it);
        ++it;
        SL(it);
        if(it->lexeme == "}") {
            print_tok_lexeme(it);
            ++it;
            return;
        }
        else{
            // error message
            Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
            +":"+"In Body: Fatal Error, Expecting \"}\" before \""+it->lexeme+"\"";
            throw e;
        }
    }
    else{
        // error message
        Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
        +":"+"In Body: Fatal Error, Expecting \"{\" before \""+it->lexeme+"\"";
        throw e;
    }
}

// R11. <Opt Declaration List> ::= <Declaration List>   |    <Empty>
void Parser::ODL(std::vector<TokenLexPair>::iterator &it){
     //switched print statement
    if(comment_switch) {
        // print production rule on console
        cout << "<Opt Declaration List> ::= <Declaration List>   |    <Empty>" << endl << endl;
        
         // print production rule on a text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Opt Declaration List> ::= <Declaration List>   |    <Empty>" << endl << endl;
        }
        // close the output text file
        outputStream.close();
    }
    try{
        DL(it);
    }
    catch(NON_FATAL_ERROR &e) {
        //IT was empty
        //Or we were at the end of the declaration list.
        return;
    }
}

// R12. <Declaration List>  ::= <Declaration> ; <Declaration List Prime>
void Parser::DL(std::vector<TokenLexPair>::iterator &it){
    IS_FROM_DECLARATION = true;
     //switched print statement
    if(comment_switch) {
        // print production rule on console
        cout << "<Declaration List>  ::= <Declaration> ; <Declaration List Prime>" << endl << endl;
        
         // print production rule on a text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Declaration List>  ::= <Declaration> ; <Declaration List Prime>" << endl << endl;
        }
        // close the output text file
        outputStream.close();
    }
    D(it);
    if(it->lexeme == ";") {
        print_tok_lexeme(it);
        ++it;
        DL_Prime(it);
    }
    // error messages
    else{
        Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
        +":"+"In Declaration List: Fatal Error, Expecting \";\" before \"" +it->lexeme+ "\"";
        throw e;
    }
    IS_FROM_DECLARATION = false;
}

// R13. <Declaration List Prime> ::=     <Declaration List> | epsilon
void Parser::DL_Prime(std::vector<TokenLexPair>::iterator &it){
     //switched print statement
    if(comment_switch) {
        // print production rule on console
        cout << "<Declaration List Prime> ::=     <Declaration List> | \u03B5" << endl << endl;
        
         // print production rule on a text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Declaration List Prime> ::=     <Declaration List> | \u03B5" << endl << endl;
        }
        // close the output text file
        outputStream.close();
    }
    DL(it);
    // do nothing for epsilon case
}

// R14. <Declaration> ::=   <Qualifier> <IDs>
void Parser::D(std::vector<TokenLexPair>::iterator &it){
     //switched print statement
    if(comment_switch) {
        // print production rule on console
        cout << "<Declaration> ::=   <Qualifier> <IDs>" << endl << endl;
        
         // print production rule on a text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream << "<Declaration> ::=   <Qualifier> <IDs>" << endl << endl;
        }
        // close the output text file
        outputStream.close();
    }
    try{
        Q(it);
    }
    catch(Parser_Exception &e) {
        NON_FATAL_ERROR f = true;
        throw f;
    }
    IDs(it);
}

// R15. <IDs> ::=     <Identifier> <IDs_Prime>
void Parser::IDs(std::vector<TokenLexPair>::iterator &it){
     //switched print statement
    if(comment_switch) {
        // print production rule on console
        cout << "<IDs> ::=     <Identifier> <IDs_Prime>" << endl << endl;
        
        // print production rule on a text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<IDs> ::=     <Identifier> <IDs_Prime>" << endl << endl;
        }
        // close the output text file
        outputStream.close();
    }
    if(it->token == "Identifier") {
        print_tok_lexeme(it);
        //IS_FROM_DECLARATION
        //IF it is NOT on the table AND we are calling IDs from the declaration production
        if(!checkSymbol(it->lexeme)&&IS_FROM_DECLARATION){
            // store a new identifier with its corresponding type into table
            cout << "The identifer was not found in the symbol table, creating it..." << endl;
            genSymbol(it, currentLexemeType);
        }
        //else if it IS on the table AND we are calling IDs from the scan production
        else if(checkSymbol(it->lexeme)&&IS_FROM_SCAN){
            genInstruction("STDIN", NIL);
            genInstruction("POPM",getSymbolAddress(it->lexeme));
        }
        //else if it IS on the table AND we are calling IDs from the print production
        else if(checkSymbol(it->lexeme)&&IS_FROM_PRINT) {
            genInstruction("STDOUT", NIL);
        }
        // else if it IS NOT on the table
        else if(!checkSymbol(it->lexeme)){
            cerr << "Identifier " << it->lexeme << " has not been declared yet.\n";
            exit(1);
            //Alternatively, you can throw a PARSER EXCEPTION HERE.
        }
        ++it;
        try{
            IDs_Prime(it);
        }
        catch(EXPRESSION_EXCEPTION &e){
            if(e == EPSILON){
                // DO NOTHING
            }
        }
    }
    // error message
    else{
        Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
        +":"+"Fatal Error,\""+it->lexeme +"\" is not a valid Identifier";
        throw e;
    }
}

// R16. <IDs_Prime> ::=  ,<IDs> | epsilon
void Parser::IDs_Prime(std::vector<TokenLexPair>::iterator &it){
     //switched print statement
    if(comment_switch) {
        // print production rule on console
        cout << "<IDs_Prime> ::= ,<IDs> | \u03B5" << endl << endl;
        
         // print production rule on a text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<IDs_Prime> ::= ,<IDs> | \u03B5" << endl << endl;
        }
        // close the output text file
        outputStream.close();
    }
    if(it->lexeme == ",") {
        print_tok_lexeme(it);
        ++it;
        IDs(it);
    }
    // epsilon case
    else{
        EMPT(it);
        
    }
}

// R17. <Statement List> ::=   <Statement> <Statement List Prime>
void Parser::SL(std::vector<TokenLexPair>::iterator &it) {
     //switched print statement
    if(comment_switch) {
        // print production rule on console
        cout << "<Statement List> ::=   <Statement> <Statement List Prime>" << endl << endl;
        
        
         // print production rule on a text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Statement List> ::=   <Statement> <Statement List Prime>" << endl << endl;
        }
        // close the output text file
        outputStream.close();
    }
    S(it);
    try{
         SL_Prime(it);
    }
    catch( EXPRESSION_EXCEPTION &e){
        if(e == EPSILON){
            // DO NOTHING
        }
    }
    
}

// R18. <Statement List Prime> ::=   <Statement List> | epsilon
void Parser::SL_Prime(std::vector<TokenLexPair>::iterator &it){
     //switched print statement
    if(comment_switch) {
        // print production rule on console
        cout << "<Statement List Prime> ::=   <Statement List> | \u03B5" << endl << endl;
        
         // print production rule on a text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream << "<Statement List Prime> ::=   <Statement List> | \u03B5" << endl << endl;
        }
        // close the output text file
        outputStream.close();
    }
    try{
        SL(it);
    }
    catch(NON_FATAL_ERROR &e){
        // do nothing for nonfatal error
        EMPT(it);    // epsilon case
    }
}

// R19. <Statement> ::=   <Compound>  |  <Assign>  |   <If>  |  <Return>   | <Print>   |   <Scan>   |  <While>
void Parser::S(std::vector<TokenLexPair>::iterator &it){
     //switched print statement
    if(comment_switch) {
        // print production rule on console
        cout << "<Statement> ::=   <Compound>  |  <Assign>  |   <If>  |  <Return>   | <Print>   |   <Scan>   |  <While>" << endl << endl;
        
         // print production rule on a text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Statement> ::=   <Compound>  |  <Assign>  |   <If>  |  <Return>   | <Print>   |   <Scan>   |  <While>" << endl << endl;
        }
        // close the output text file
        outputStream.close();
    }
    //**-> <Compound>
    try{
        COM(it);
        return;
    }
    catch(NON_FATAL_ERROR &e){
        //DO nothing
    }
    //**-> <Assign>
    try{
        A(it);
        return;
    }
    catch(NON_FATAL_ERROR&e){
        //DO nothing
    }
    //**-> <If>
    try{
        IF(it);
        return;
    }
    catch(NON_FATAL_ERROR &e){
        //DO nothing
    }
    //**-> <Return>
    try{
        RETURN(it);
        return;
    }
    catch(NON_FATAL_ERROR&e){
        //DO nothing
    }
    //**-> <Print>
    try{
        PRINT(it);
        return;
    }
    catch(NON_FATAL_ERROR &e){
        //DO nothing
    }
    //**-> <Scan>
    try{
        SCAN(it);
        return;
    }
    catch(NON_FATAL_ERROR &e){
        //DO nothing
    }
    //**-> <While>
    try{
        WHILE(it);
        return;
    }
    catch(NON_FATAL_ERROR &e){
        //DO NOTHING
    }
    
    if(it->lexeme == "$$" || it-> lexeme == "}"||it->lexeme == "") {
        NON_FATAL_ERROR e = true;
        throw e;
    }
    // error message
    Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)+":"+"Fatal Error, Statement beginning with "+it->lexeme +" not recognized";
    throw e;
}

// R20. <Compound> ::=   {  <Statement List>  }
void Parser::COM(std::vector<TokenLexPair>::iterator &it){
     //switched print statement
    if(comment_switch) {
        // print production rule on console
        cout << "<Compound> ::=   {  <Statement List>  }" << endl << endl;
        
         // print production rule on a text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Compound> ::=   {  <Statement List>  }" << endl << endl;
        }
        // close the output text file
        outputStream.close();
    }
    
    if(it->lexeme == "{"){
        print_tok_lexeme(it);
        ++it;
        SL(it);
        if(it->lexeme == "}"){
            print_tok_lexeme(it);
            ++it;
            return;
        }
        // error message
        else{
            Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
            +":"+"In Compound: Fatal Error, Expecting \"}\" before \""+it->lexeme+"\"";
            throw e;
        }
    }
    // error message
    else{
        NON_FATAL_ERROR f = true;
        throw f;
    }
}

// R21. <Assign> ::=     <Identifier> = <Expression> ;
void Parser::A(std::vector<TokenLexPair>::iterator &it){
     //switched print statements
    if(comment_switch) {
        // print production rule on console
        cout << "<Assign> ::=     <Identifier> = <Expression> ;" << endl << endl;
        
        // print production rule on a text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Assign> ::=     <Identifier> = <Expression> ;" << endl << endl;
        }
        // close the output text file
        outputStream.close();
    }
    // if the token is Identifier,
    if(it->token == "Identifier") {
        // then check if its lexeme is in the Symbol table
        if(!checkSymbol(it->lexeme) ){
            cerr << "Identifier " << it->lexeme << " has not been declared yet.\n";
            exit(1);
        }
        // save the type of the symbol
        string symbolTypeSaved = getSymbolType(it->lexeme);
        // save the symbol for address generation
        string symbolSaved = it->lexeme;
        // assign temp to the symbol for future comparision
        tempSymbol = it->lexeme;
        
        print_tok_lexeme(it);
        ++it;
        if(it->lexeme == "="){
            print_tok_lexeme(it);
            ++it;
            try{
                E(it);
            }
            catch(EXPRESSION_EXCEPTION &e){
                cout << (e==INT?"<Expression> throw back an int":"<Expression> throw back a boolean") <<endl;
                // checking type compatibility of 2 symbols
                if((e == BOOL && symbolTypeSaved == "int")||(e == INT && symbolTypeSaved == "boolean")){
                    cerr << "Type mismatch in <Assign>\n";
                    exit(1);
                }
            }
            // get the memory address of the identifier symbol
            int address = getSymbolAddress(symbolSaved);
            // generating the instruction POPM for the symbol
            genInstruction("POPM", address);
            
            if(it->lexeme == ";"){
                print_tok_lexeme(it);
                ++it;
                return;
            }
            // error message
            else{
                Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
                +":"+"In Assignment: Fatal Error, Expecting \";\" before \""+it->lexeme+"\"";
                throw e;
            }
        }
        // error message
        else{
            Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
            +":"+"In Assignment: Fatal Error, Expecting \"=\" before \""+it->lexeme+"\"";
            throw e;
        }
    }
    //non-fatal error
    else {
        NON_FATAL_ERROR e = true;
        throw e;
    }
}

// R22. <If> ::=     if  ( <Condition>  ) <Statement> <If Prime>
void Parser::IF(std::vector<TokenLexPair>::iterator &it){
    //switched print statement
    if(comment_switch) {
        // print production rule on console
        cout << "<If> ::=     if  ( <Condition>  ) <Statement> <If Prime>" << endl << endl;
        
        // print production rule on a text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream << "<If> ::=     if  ( <Condition>  ) <Statement> <If Prime>" << endl << endl;
        }
        // close the output text file
        outputStream.close();
    }
    
    if(it->lexeme == "if"){
        //int address = instructionIndex;
        print_tok_lexeme(it);
        ++it;
        if(it->lexeme == "("){
            print_tok_lexeme(it);
            ++it;
            C(it);
            if(it->lexeme == ")"){
                print_tok_lexeme(it);
                ++it;
                S(it);
                IF_Prime(it);
            }
            // error message
            else{
                --it;
                Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
                +":"+"In IF-statement: Fatal Error, Expecting \")\" before \""+it->lexeme+"\"";
                throw e;
            }
        }
        // error message
        else{
            --it;
            Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
            +":"+"in IF-statement: Fatal Error, Expecting \"(\" before \""+it->lexeme+"\"";
            throw e;
        }
    }
    else{
        NON_FATAL_ERROR f = true;
        throw f;
    }
}

// R23. <If prime > ::=     ifend | else <Statement> ifend
void Parser::IF_Prime(std::vector<TokenLexPair>::iterator &it){
    //switched print statement
    if(comment_switch) {
        // print production rule on console
        cout << "<If prime > ::=     ifend | else <Statement> ifend" << endl << endl;
        
        // print production rule on a text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<If prime > ::=     ifend | else <Statement> ifend" << endl << endl;
        }
        // close the output text file
        outputStream.close();
    }
    if(it->lexeme == "ifend"){
        backPatch(instructionIndex);
        // LABEL for ifend
        genInstruction("LABEL",NIL);
        print_tok_lexeme(it);
        ++it;
    }
    else if( it->lexeme == "else"){
        genInstruction("JUMP", NIL);
        backPatch(instructionIndex);// this will back patch to line 10's Jumpz
        jumpstack.push(instructionIndex-1);
        // LABEL for else
        genInstruction("LABEL", NIL);
        
        print_tok_lexeme(it);
        ++it;
        S(it);
        backPatch(instructionIndex);
        genInstruction("LABEL", NIL);
        if(it->lexeme == "ifend"){
            print_tok_lexeme(it);
            it++;
        }
        //error message
        else{
            Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
            +":"+"In IF-statement: Fatal Error, Expecting \"ifend\" before \""+it->lexeme+"\"";
            throw e;
        }
    }
    // error message
    else{
        Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
        +":"+"In IF-statement: Fatal Error, Expecting \"ifend\" or \"else\" before \""+it->lexeme+"\"";
        throw e;
    }
}

// R24. <Return> ::=  return <Return Prime>
void Parser::RETURN(std::vector<TokenLexPair>::iterator &it){
     //switched print statement
    if(comment_switch) {
        cout << "<Return> ::=  return <Return Prime>" << endl << endl;
        
         // print production rule on a text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Return> ::=  return <Return Prime>" << endl << endl;
        }
        // close the output text file
        outputStream.close();
    }
    if(it->lexeme == "return"){
        print_tok_lexeme(it);
        ++it;
        RETURN_Prime(it);
    }
    // non-fatal error
    else{
        NON_FATAL_ERROR f = true;
        throw f;
    }
}

// R25. <Return Prime> ::=  ; | < Expression> ;
void Parser::RETURN_Prime(std::vector<TokenLexPair>::iterator &it){
     //switched print statement
    if(comment_switch) {
        // print production rule on console
        cout << "<Return Prime> ::=  ; | < Expression> ;" << endl << endl;
        
         // print production rule on a text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Return Prime> ::=  ; | < Expression> ;" << endl << endl;
        }
        // close the output text file
        outputStream.close();
    }
    if(it->lexeme == ";"){
        print_tok_lexeme(it);
        ++it;
        return;
    }
    //**-> < Expression> ;
    else{
        E(it);
        if(it->lexeme == ";"){
            print_tok_lexeme(it);
            ++it;
            return;
        }
        // error message
        else{
            Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
            +":"+"In Return: Fatal Error, Expecting \";\" before \""+it->lexeme+"\"";
            throw e;
        }
    }
}

// R26. <Print> ::=    put ( <Expression>);
void Parser::PRINT(std::vector<TokenLexPair>::iterator &it){
     //switched print statement
    if(comment_switch) {
        // print production rule on console
        cout << "<Print> ::=    put ( <Expression>);" << endl << endl;
        
         // print production rule on a text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream << "<Print> ::=    put ( <Expression>);" << endl << endl;
        }
        // close the output text file
        outputStream.close();
    }
    if(it->lexeme == "put"){
        IS_FROM_PRINT = true;
        
        print_tok_lexeme(it);
        ++it;
        if(it->lexeme == "("){
            print_tok_lexeme(it);
            ++it;
            try{
                E(it);
            }
            catch( EXPRESSION_EXCEPTION &e){
                // DO NOTHING HERE
            }
            if(it->lexeme == ")"){
                // generating the instruction STDOUT
                genInstruction("STDOUT", NIL);
                print_tok_lexeme(it);
                ++it;
                if(it->lexeme == ";"){
                    print_tok_lexeme(it);
                    ++it;
                    IS_FROM_PRINT = false;
                    return;
                }
                // error message
                else{
                    Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
                    +":"+"In Print: Fatal Error, Expecting \";\" before \""+it->lexeme+"\"";
                    throw e;
                }
            }
            // error message
            else{
                Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
                +":"+"In Print: Fatal Error, Expecting \")\" before \""+it->lexeme+"\"";
                throw e;
            }
        }
        // error message
        else{
            Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
            +":"+"In Print: Fatal Error, Expecting \"()\" before \""+it->lexeme+"\"";
            throw e;
        }
    }
    // non-fatal error
    else{
        NON_FATAL_ERROR f = true;
        throw f;
    }
    
}

// R27. <Scan> ::=    get ( <IDs> );
void Parser::SCAN(std::vector<TokenLexPair>::iterator &it) {
    IS_FROM_SCAN = true;
     //switched print statement
    if(comment_switch) {
        // print production rule on console
        cout << "<Scan> ::=    get ( <IDs> );" << endl << endl;
        
         // print production rule on a text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream << "<Scan> ::=    get ( <IDs> );" << endl << endl;
        }
        // close the output text file
        outputStream.close();
    }
    if(it->lexeme == "get"){
        print_tok_lexeme(it);
        ++it;
        if(it->lexeme == "("){
            print_tok_lexeme(it);
            ++it;
            IDs(it);
            if(it->lexeme == ")"){
                print_tok_lexeme(it);
                
                ++it;
                if(it->lexeme == ";"){
                    print_tok_lexeme(it);
                    
                    ++it;
                    return;
                }
                // error message
                else{
                    Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
                    +":"+"In Scan: Fatal Error, Expecting \";\" before \""+it->lexeme+"\"";
                    throw e;
                }
            }
            // error message
            else{
                Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
                +":"+"In Scan: Fatal Error, Expecting \")\" before \""+it->lexeme+"\"";
                throw e;
            }
        }
        // error message
        else{
            Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
            +":"+"In Scan: Fatal Error, Expecting \")\" before \""+it->lexeme+"\"";
            throw e;
        }
    }
    else{
        NON_FATAL_ERROR f = true;
        throw f;
    }
}

// R28. <While> ::=  while ( <Condition>  )  <Statement>  whileend
void Parser::WHILE(std::vector<TokenLexPair>::iterator &it) {
     //switched print statement
    if(comment_switch) {
        // print production rule on console
        cout << "<While> ::=  while ( <Condition>  )  <Statement>  whileend" << endl << endl;
        
         // print production rule on a text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<While> ::=  while ( <Condition>  )  <Statement>  whileend" << endl << endl;
        }
        // close the output text file
        outputStream.close();
    }
    if(it->lexeme == "while") {
        
        int address = instructionIndex;
        genInstruction("LABEL", NIL);  // generating the instruction LABEL
        
        print_tok_lexeme(it);
        ++it;
        if(it->lexeme == "(") {
            print_tok_lexeme(it);
            ++it;
            C(it);
            
            if(it->lexeme == ")") {
                print_tok_lexeme(it);
                ++it;
                S(it);
                
                genInstruction("JUMP", address); // generating the instruction JUMP
                genInstruction("LABEL", NIL); // This adds the label to the end of the while loopThe following two lines introduce the label
                backPatch(instructionIndex-1);// remove them if label is not needed
                
                
                if(it->lexeme == "whileend") {
                    print_tok_lexeme(it);
                    ++it;
                    return;
                }
                // error messeges
                else{
                    Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
                    +":"+"In While: Fatal Error, Expecting \"whileend\" before \""+it->lexeme+"\"";
                    throw e;
                }
            }
            // error messeges
            else{
                Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
                +":"+"In While: Fatal Error, Expecting \")\" before \""+it->lexeme+"\"";
                throw e;
            }
        }
        // error messeges
        else{
            Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
            +":"+"In While: Fatal Error, Expecting \"(\" before \""+it->lexeme+"\"";
            throw e;
        }
    }
    // error messeges
    else{
        NON_FATAL_ERROR f = true;
        throw f;
    }
}

// R29. <Condition> ::=     <Expression>  <Relop>   <Expression>
void Parser::C(std::vector<TokenLexPair>::iterator &it){
     //switched print statement
    if(comment_switch) {
        // print production rule on console
        cout << "<Condition> ::=     <Expression>  <Relop>   <Expression>" << endl << endl;
        
        // print production rule on a text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Condition> ::=     <Expression>  <Relop>   <Expression>" << endl << endl;
        }
        // close the output text file
        outputStream.close();
    }
    bool expression1Bool = false, expression2Bool = false;
    // check the 1st <Expression>
    try{
        E(it);
    }
    catch (EXPRESSION_EXCEPTION &e){
        e = INT;
        expression1Bool = true;
    }
     // current relational operator
    string currentOperator = it->lexeme;
    R_op(it);
    
     // check the 2nd <Expression>
    try{
        E(it);
    }
   catch(EXPRESSION_EXCEPTION &e){
        e = INT;
       expression2Bool = true;
    }
    // check type compatibility between 2 <Expression>,
    // both <Expression> MUST BE 2 int for arithmetic operations
    if((!expression1Bool && expression2Bool)||(expression1Bool&&!expression2Bool)){
        cerr << "Type mismatch in comparison\n";
        exit(1);
    }
    // Relational operator cases
    if(currentOperator == "=="){
        genInstruction("EQU", NIL);        // generating the instruction EQU
        jumpstack.push(instructionIndex);
        genInstruction("JUMPZ", NIL);      // generating the instruction JUMPZ
    }
    else if(currentOperator == "^=" ){
        genInstruction("NEQ", NIL);        // generating the instruction NEQ
        jumpstack.push(instructionIndex);
        genInstruction("JUMPZ", NIL);      // generating the instruction JUMPZ
    }
    else if(currentOperator == "=>" ){
        genInstruction("GEQ", NIL);        // generating the instruction GEQ
        jumpstack.push(instructionIndex);
        genInstruction("JUMPZ", NIL);      // generating the instruction JUMPZ
    }
    else if(currentOperator == "=<" ){
        genInstruction("LEQ", NIL);        // generating the instruction LEQ
        jumpstack.push(instructionIndex);
        genInstruction("JUMPZ", NIL);      // generating the instruction JUMPZ
    }
    else if(currentOperator == "<" ){
        genInstruction("LES", NIL);        // generating the instruction LES
        jumpstack.push(instructionIndex);
        genInstruction("JUMPZ", NIL);      // generating the instruction JUMPZ
    }
    else if(currentOperator == ">" ){
        genInstruction("GRT", NIL);        // generating the instruction GRT
        jumpstack.push(instructionIndex);
        genInstruction("JUMPZ", NIL);      // generating the instruction JUMPZ
    }
    // error messeges
    else{
        cerr << "Relop syntax error at line "<<to_string(it->lineNum) +":"+to_string(it->charNum)<<endl;
        cerr << "Invalid operator\n"<< "'=', '/=', '>', '<', '=>' or '<=' is expected.\n";
        exit(1);
    }
}

// R30. <Relop> ::=        ==   |   ^=    |   >     |   <    |   =>    |   =<
void Parser::R_op(std::vector<TokenLexPair>::iterator &it){
     //switched print statement
    if(comment_switch) {
        // print production rule on console
        cout << "<Relop> ::=        ==   |   ^=    |   >     |   <    |   =>    |   =<" << endl << endl;
        
         // print production rule on a text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Relop> ::=        ==   |   ^=    |   >     |   <    |   =>    |   =<" << endl << endl;
        }
        // close the output text file
        outputStream.close();
    }
    if(it->lexeme != "=="&&
       it->lexeme != "^="&&
       it->lexeme != ">"&&
       it->lexeme != "<"&&
       it->lexeme != "=>"&&
       it->lexeme != "=<") {
        --it;
        // error message
        Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
        +":"+"Fatal Error, Unrecognized relational operator \"" + it->lexeme+"\"";
        throw e;
    }
    ++it;
}

// R31. <Expression>  ::=   <Term><Expression Prime>
void Parser::E(std::vector<TokenLexPair>::iterator &it){
     //switched print statement
    if(comment_switch) {
        // print production rule on console
        cout << "<Expression>  ::=   <Term><Expression Prime>" << endl << endl;
        
         // print production rule on a text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Expression>  ::=   <Term><Expression Prime>" << endl << endl;
        }
        // close the output text file
        outputStream.close();
    }
    
    bool T_Is_BOOL = false;
    try{
       T(it);
    }
    catch (EXPRESSION_EXCEPTION & e){
        if(e == BOOL){
            cout << "we confirm that test is a boolean\n";
            T_Is_BOOL = true;
        }
    }
    try{
        E_Prime(it);
    }
    catch(EXPRESSION_EXCEPTION & e){
        // <Expression Prime> returns EPSILON and <Term> returns BOOL value
        if(e == EPSILON && T_Is_BOOL){
            throw BOOL;
            return;
        }
        // <Expression Prime> returns EPSILON or INT and <Term> does not return BOOL value
        else if((e == EPSILON || e == INT)   && !T_Is_BOOL  ){
            throw INT;
            return;
        }
        // error message
        else{
            cerr<<"We have a mismatched pair of variable type"<<endl;
            exit(1);
        }
    }
}

// R32. <Expression Prime>  ::=   + <Term> <Expression Prime> | - <Term> <Expression Prime> | epsilon
void Parser::E_Prime(std::vector<TokenLexPair>::iterator &it){
     //switched print statement
    if(comment_switch) {
        // print production rule on console
        cout << "<Expression Prime>  ::=   + <Term> <Expression Prime> | - <Term> <Expression Prime> |  \u03B5" << endl << endl;
        
         // print production rule on a text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Expression Prime>  ::=   + <Term> <Expression Prime> | - <Term> <Expression Prime> |  \u03B5" << endl << endl;
        }
        // close the output text file
        outputStream.close();
    }
    //**-> + <Term> <Expression Prime>
    if(it->lexeme == "+"){
        // checking type compatibility
        checkTypeMatch(prevLexeme, it->lexeme);
        print_tok_lexeme(it);
        ++it;
        T(it);
        // generating the instruction ADD
        genInstruction("ADD", NIL);
        
        E_Prime(it);
    }
    //**-> - <Term> <Expression Prime>
    else if(it->lexeme == "-"){
        // checking type compatibility
        checkTypeMatch(prevLexeme, it->lexeme);
        print_tok_lexeme(it);
        ++it;
        T(it);
         // generating the instruction SUB
        genInstruction("SUB", NIL);
        
        E_Prime(it);
    }
    // epsilon case
    else {
        EMPT(it);
    }
}

// R33. <Term>    ::= <Factor> <Term Prime>
void Parser::T(std::vector<TokenLexPair>::iterator &it){
     //switched print statement
    if(comment_switch) {
        // print production rule on console
        cout << "<Term>    ::= <Factor> <Term Prime>" << endl << endl;
        
         // print production rule on a text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Term>    ::= <Factor> <Term Prime>" << endl << endl;
        }
        // close the output text file
        outputStream.close();
    }
    FAC(it);
    try{
        T_Prime(it);
    }
    catch(EXPRESSION_EXCEPTION &e  ){
        if((e == EPSILON)|| e == INT  ) {
            return;
        }
        else{
            throw e;
        }
    }
}

// R34. <Term Prime>    ::= * <Factor> <Term Prime> | / <Factor> <Term Prime> | epsilon
void Parser::T_Prime(std::vector<TokenLexPair>::iterator &it) {
     //switched print statement
    if(comment_switch) {
        // print production rule on console
        cout << "<Term Prime>    ::= * <Factor> <Term Prime> | / <Factor> <Term Prime> |  \u03B5" << endl << endl;
        
        // print production rule on a text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream << "<Term Prime>    ::= * <Factor> <Term Prime> | / <Factor> <Term Prime> |  \u03B5" << endl << endl;
        }
        // close the output text file
        outputStream.close();
    }
    
    //**-> * <Factor> <Term Prime>
    if(it->lexeme == "*"){
        // checking type compatibility
        checkTypeMatch(prevLexeme, it->lexeme);
        print_tok_lexeme(it);
        it++;
        FAC(it);
        // generating the instruction MUL
        genInstruction("MUL", NIL);
        
        T_Prime(it);
    }
    //**-> / <Factor> <Term Prime>
    else if(it->lexeme == "/"){
        // checking type compatibility
        checkTypeMatch(prevLexeme, it->lexeme);
        print_tok_lexeme(it);
        it++;
        FAC(it);
        // generating the instruction DIV
        genInstruction("DIV", NIL);
        
        T_Prime(it);
    }
    // epsilon case
    else{
        EMPT(it);
    }
}

// R35. <Factor> ::=      -  <Primary>    |    <Primary>
void Parser::FAC(std::vector<TokenLexPair>::iterator &it) {
     //switched print statement
    if(comment_switch) {
        // print production rule on console
        cout << "<Factor> ::=      -  <Primary>    |    <Primary>" << endl << endl;
        
         // print production rule on a text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Factor> ::=      -  <Primary>    |    <Primary>" << endl << endl;
        }
        // close the output text file
        outputStream.close();
    }
     //**-> - <Primary>
    if(it->lexeme == "-"){
        print_tok_lexeme(it);
        prevLexeme = it->lexeme;        // for futute comparision
        ++it;
        PRIM(it);
        if(tempSymbol == "boolean") {
            cerr << "Error- cannot have a negative boolean\n";
            exit(1);
        }
    }
    //**-> <Primary>
    else{
        PRIM(it);
    }
}


// R36. <Primary> ::=     <Identifier>  |  <Integer>  |   <Identifier>  ( <IDs> )   |   ( <Expression> )   | <Real>  |   true   |  false
void Parser::PRIM(std::vector<TokenLexPair>::iterator &it){
     //switched print statement
    if(comment_switch) {
        // print production rule on console
        cout << "<Primary> ::=     <Identifier>  |  <Integer>  |   <Identifier>  ( <IDs> )   |   ( <Expression> )   | <Real>  |   true   |  false" << endl << endl;
        
         // print production rule on a text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Primary> ::=     <Identifier>  |  <Integer>  |   <Identifier>  ( <IDs> )   |   ( <Expression> )   | <Real>  |   true   |  false" << endl << endl;
        }
        // close the output text file
        outputStream.close();
    }
   
    //***-><Identifier>
    // INSTRUCTION GENERATION for Identifier case
    if(it->token== "Identifier") {
        // check if the symbol is in the table
        if(!checkSymbol(it->lexeme)){
            cerr << "Identifier " << it->lexeme << " has not been declared yet.\n";
            exit(1);
        }
         //  getting the memory address of the symbol
        int address = getSymbolAddress(it->lexeme);
        // generating the instruction PUSHM
        genInstruction("PUSHM", address);
        // determine the type of the symbol
        string type_of_id = getSymbolType(it->lexeme);
        // if the symbol is a boolean
        if(type_of_id == "boolean"){
            ++it;
            throw BOOL;
        }
         // else the symbol is an int
        else if(type_of_id == "int"){
            ++it;
            return;
        }
        print_tok_lexeme(it);
        ++it;
        
        //MAYBE THIS BLOCK FOR THE PARENTHESIS CAN BE TAKE OUT
        //AS TYPE CASTING IS NOT ALLOWED. BUT WE WILL KEEP IT IN FOR NOW.
        //***-><Identifier>  ( <IDs> )
        if(it->lexeme == "(") {
            print_tok_lexeme(it);
            ++it;
            IDs(it);
            ++it;
            if(it->lexeme == ")"){
                print_tok_lexeme(it);
                return;
            }
            // error messege
            else{
                Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
                +":"+"In Primary: Fatal Error, Expecting \")\" before \""+it->lexeme+"\"";
                throw e;
            }
        }
    }
     //***-><Integer>
    // INSTRUCTION GENERATION for Integer case
    else if(it->token == "Int"){
        // some conversions
        int inputedInt = 0;
        
        // convert inputed string into negative int value
        if(prevLexeme == "-"){
            inputedInt = 0 - stoi(it->lexeme);
        }
        // convert inputed string into positive int value
        else{
            inputedInt = stoi(it->lexeme);
        }
        // PUSH int value on top of the stack
        genInstruction("PUSHI", inputedInt);
        tempSymbol = "Integer";
        print_tok_lexeme(it);
        ++it;
        return;
    }
    //***-> <Real>
    // NO INSTRUCTION GENERATION IN THIS CASE
    else if(it->token == "Real"){
        print_tok_lexeme(it);
        ++it;
        return;
    }
    //***-> true | false
    // INSTRUCTION GENERATION for boolean cases
    else if(it->lexeme == "true" || it->lexeme == "false"){
        // determine if the the 2 variables having the same type and value
        if( (!(getSymbolType(tempSymbol) == "boolean")  && !(tempSymbol == "")) || prevLexeme == "-" ){
            // comparing a negative int and a boolen value
            if (prevLexeme == "-"){
                cerr << "Cannot assign " << tempSymbol << " to " << prevLexeme + it->lexeme << endl;
            }
            // comparing a positive int and a boolean value
            else {
                cerr << "The type of " << tempSymbol << " and " << it->lexeme << " must match" << endl;
            }
            exit(1);
        }
        // VALID BOOLEAN VALUES
        // boolean as true value
        if(it->lexeme == "true"){
            // generating the instruction PUSHI for true value as 1
            genInstruction("PUSHI", 1);
            print_tok_lexeme(it);
            ++it;
            throw BOOL;
        }
        // boolean as false value
        else {
            // generating the instruction PUSHI for false value as 0
            genInstruction("PUSHI", 0);
            print_tok_lexeme(it);
            ++it;
            throw BOOL;
        }
    }
    //***-> ( <Expression> )
    else if(it->lexeme == "(") {
        print_tok_lexeme(it);
        ++it;
        E(it);
        if(it->lexeme == ")"){
            print_tok_lexeme(it);
            ++it;
            return;
        }
        // error message
        else{
            Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
            +":"+"In Primary: Fatal Error, Expecting \")\" before \""+it->lexeme+"\"";
            throw e;
        }
    }
    // error message
    else{
        Parser_Exception e = to_string(it->lineNum) +":"+to_string(it->charNum)
        +":"+"In Primary: Fatal Error, "+it->lexeme+" is an unrecognized primary";
        throw e;
    }
}

// R37. <Empty> ::= epsilon
void Parser::EMPT(std::vector<TokenLexPair>::iterator &it){
     //switched print statement
    if(comment_switch) {
        // print production rule on console
        cout << "<Empty> ::=  \u03B5" << endl << endl;
        
         // print production rule on a text file
        outputStream.open("Syntactic_Analysis_Result.txt", std::ofstream::out | std::ofstream::app);
        if(outputStream.is_open()) {
            outputStream<< "<Empty> ::=  \u03B5" << endl << endl;
        }
        // close the output text file
        outputStream.close();
    }
    throw EPSILON;
    
}
//=================================================================================
// Functions used for Semantic rules
//=================================================================================
// Symbol Table Handling functions

// determine if lexeme is already existed symbol in the table
bool Parser::checkSymbol(string symbol){
    for(size_t i=0; i< symbolIndex; i++){
        if(symbol ==  symbolTable[i].id){
            return true;
        }
    }
    return false;
}
// generating Symbol Table and its elements with corresponsing values
void Parser::genSymbol(std::vector<TokenLexPair>::iterator &it, string id_type){
    symbolTable[symbolIndex].id = it->lexeme;
    symbolTable[symbolIndex].memoryLocation = MEMORY_ADDRESS;
    symbolTable[symbolIndex].idType = id_type;
    symbolIndex++;
    MEMORY_ADDRESS++;
    
}
// Print contents of Symbol Table onto the console
void Parser::printSymbolOntoConsole( ){
    cout<< "\nSYMBOL TABLE\n";
    cout<<setw(15)<<left<<"Identifier"<<setw(20)<<"Memory Location"<<setw(15)<<"Type"<<setw(20)<<left<<endl;
    for(size_t i= 0; i<symbolIndex; i++){
        cout<<setw(20)<<symbolTable[i].id <<setw(15) << symbolTable[i].memoryLocation <<setw(10) << symbolTable[i].idType <<endl;
    }
}
// Print contents of Symbol Table into a text file
void Parser::printSymbolIntoTxtFile(){
    // open the text file
    outputStream.open("Symbol_And_Instruction_Tables.txt");
    if(outputStream.is_open()){
         outputStream<< "Output:" <<endl;
        outputStream<< "\nSYMBOL TABLE\n";
        outputStream<<setw(15)<<left<<"Identifier"<<setw(20)<<"Memory Location"<<setw(15)<<"Type"<<setw(20)<<left<<endl;
        for(size_t i= 0; i<symbolIndex; i++){
            outputStream<<setw(20)<<symbolTable[i].id <<setw(15) << symbolTable[i].memoryLocation <<setw(10) << symbolTable[i].idType <<endl;
        }
    }
    // close the text file
    outputStream.close();
}

// access the memory address of the saved variable
int Parser::getSymbolAddress(string saved) const{
    int address = 0;
    for(size_t i = 0; i < symbolIndex; i++){
        if(symbolTable[i].id == saved ){
            address = symbolTable[i].memoryLocation;
        }
    }
    return address;
}
// access the type of already declared items in the table
string Parser::getSymbolType(string input) const {
    string a = "";
    for(size_t i = 0; i< symbolIndex; i++){
        if(symbolTable[i].id == input){
            a = symbolTable[i].idType;
        }
    }
    
    if(input == "true" || input == "false" ){
        a = "boolean";
    }
    
    return a;
}
// no arithmetic operation for boolean values
void Parser::checkTypeMatch(string prevLexeme, string lexeme ){
    if(getSymbolType(prevLexeme) == "boolean" || getSymbolType(lexeme) == "boolean" ){
        cerr<< "No arithmetic operation are allowed for boolean."<<endl;
        exit(1);
    }
}
//=================================================================================
// Functions Related to generating the Assembly Code

void Parser::genInstruction(string op, int oprnd){
    instructionTable[instructionIndex].instructionStep = instructionIndex;
    instructionTable[instructionIndex].op = op;
    instructionTable[instructionIndex].memoryAddress = oprnd;
    instructionIndex++;
}
// save the address memory for JUMPZ
void Parser::backPatch(int jump_addr){
    int address = jumpstack.top();
    instructionTable[address].memoryAddress = jump_addr;
    jumpstack.pop();
}
// Print contents of Instruction Table onto the console
void Parser::printInstructionOntoConsole( ){
    cout<< "\nINSTRUCTION TABLE\n";
    string nil;
    cout<<setw(10)<<left<<"Address"<<setw(10)<<"Operator"<<setw(7)<<"Operand"<<setw(20)<<left<<endl;
    for(int i = 1; i< instructionIndex; i++){
        if(instructionTable[i].memoryAddress == NIL ){
            nil = "";
            cout<<setw(12)<<left<<instructionTable[i].instructionStep<< setw(10)<< instructionTable[i].op <<nil<<endl;
        }else{
            cout<<setw(12)<<left<<instructionTable[i].instructionStep<<setw(10)<< instructionTable[i].op <<instructionTable[i].memoryAddress<<endl;
        }
    }
}
// Print contents of Instruction Table into the text file
void Parser::printInstructionIntoTxtFile(){
    // open the text file
    outputStream.open("Symbol_And_Instruction_Tables.txt",std::ofstream::app);
    
    if(outputStream.is_open()){
        outputStream<< "\nINSTRUCTION TABLE\n";
        string nil;
        outputStream<<setw(10)<<left<<"Address"<<setw(10)<<"Operator"<<setw(7)<<"Operand"<<setw(20)<<left<<endl;
        for(int i = 1; i< instructionIndex; i++){
            if(instructionTable[i].memoryAddress == NIL ){
                nil = "";
                outputStream<<setw(12)<<left<<instructionTable[i].instructionStep<< setw(10)<< instructionTable[i].op <<nil<<endl;
            }else{
                outputStream<<setw(12)<<left<<instructionTable[i].instructionStep<<setw(10)<< instructionTable[i].op <<instructionTable[i].memoryAddress<<endl;
            }
        }
    }
    // close the text file
    outputStream.close();
   
}

