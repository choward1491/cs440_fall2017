//
// Author: C.Howard
// Date  : 3/2017
//



#ifndef Lexer_hpp
#define Lexer_hpp

#include <stdio.h>
#include <vector>
#include <string>
#include "Token.hpp"
#include "FileWrap.hpp"



/*!
	This class acts as a Lexical Analyzer for a stream of characters.
	This class is capable of analyzing a stream of characters from both
	a file and a string. The goal is for this class to generate a token
	list output that can be used by an interpreter to make sense of the
	things written.
*/
class Lexer {
public:
    
	typedef std::vector<Token> Tokens;

	//ctor/dtor
    Lexer();
    Lexer( wrap::file & file );
    ~Lexer();
    
	// main functions to get token streams
    void obtainTokensFromSource( wrap::file & file );
	void obtainTokensFromString( const std::string & s);

	// method to print tokens
    void printTokens();

	// method to get reference to list of tokens
    const std::vector<Token> & getTokens() const;
    
private:
    static const char doneChar;
    char c; // current character from stream
	bool isFileSource;
    Tokens  tokens;
    wrap::file * file;
	std::string::const_iterator it_s;
	std::string::const_iterator it_e;
    
    Token getNextToken();
    bool isLetter( char c ) const;
    Token getNumber();
    void consume();
    void passWhiteSpace();
    void passComment();
    
};


#endif /* Lexer_hpp */
