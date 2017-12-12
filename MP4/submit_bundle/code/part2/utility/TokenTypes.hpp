//
//  TokenTypes.hpp
//  ConfigGen
//
//  Created by Christian J Howard on 8/6/16.
//
//
//

#ifndef TokenTypes_h
#define TokenTypes_h

#include <string>

// list of token types
namespace _token {
    
    enum Type {
        LBRACKET=0,
        RBRACKET,
		LBRACE,
		RBRACE,
        LPAR,
        RPAR,
        COMMA,
		COLON,
        SEMICOLON,
        LETTER,
        NUMBER,
        EQUALS,
        PLUS,
        MINUS,
        STAR,
        BSLASH,
		FSLASH,
        PERIOD,
        EXCLAM,
        QUOTE,
		APOST,
        PERCENT,
		AMPERSAND,
		POUND,
		SPACE,
		NEXTLINE,
		UNDERSCORE,
        EOF_,
        UNKNOWN
    };

    extern std::string String [];

}

#endif /* TokenTypes_h */
