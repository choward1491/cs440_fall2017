//
//  Token.hpp
//  ConfigGen
//
//  Created by Christian J Howard on 8/6/16.
//
//
//

#ifndef Token_hpp
#define Token_hpp

#include "TokenTypes.hpp"

// class representing a token
// includes token type and token (character) value
class Token {
public:
    typedef _token::Type Type;
    Token( Type type, const char & c );
    Type getType() const;
    char getChar() const;
    void print() const;
    
private:
    Type type;
    char character;
};

#endif /* Token_hpp */
