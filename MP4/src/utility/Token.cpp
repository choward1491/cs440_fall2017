//
//  Token.cpp
//  ConfigGen
//
//  Created by Christian J Howard on 8/6/16.
//

#include "Token.hpp"


Token::Token( Type type_, const char & c):type(type_),character(c) {
    
}
Token::Type Token::getType() const {
    return type;
}
char Token::getChar() const {
    return character;
}
void Token::print() const {
    printf("(%s, %c)\n",_token::String[type].c_str(),character);
}
