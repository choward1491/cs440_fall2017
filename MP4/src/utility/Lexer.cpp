//
// Author: C.Howard
// Date  : 3/2017
//



#include "Lexer.hpp"


//std::vector<Token>         tokens;
const char Lexer::doneChar = (char)-1;

Lexer::Lexer():tokens(),c(),file(0),isFileSource(true){
    
}
Lexer::Lexer( wrap::file & file_ ):tokens(),c(),file(0),isFileSource(true) {
    obtainTokensFromSource(file_);
}
Lexer::~Lexer(){
    file = 0;
}

void Lexer::obtainTokensFromSource( wrap::file & file_ ){
	isFileSource = true;
    file = &file_;
	tokens.clear();
    consume();
    while(true){
        Token tmp = getNextToken();
        tokens.push_back(tmp);
        if( tmp.getType() == Token::Type::EOF_ ){ break; }
    }
}

void Lexer::obtainTokensFromString(const std::string & s)
{
	isFileSource = false;
	it_s = s.begin();
	it_e = s.end();
	tokens.clear();
	consume();
	while (true) {
		Token tmp = getNextToken();
		tokens.push_back(tmp);
		if (tmp.getType() == Token::Type::EOF_) { break; }
	}
}

void Lexer::printTokens(){
    for(int i = 0; i < tokens.size(); ++i ){
        tokens[i].print();
    }
}
const std::vector<Token> & Lexer::getTokens() const{
    return tokens;
}


Token Lexer::getNextToken(){
    while( c != doneChar ){
        switch(c){
            case '\t':case '\r': //passWhiteSpace(); continue;
			case ' ': consume(); return Token(Token::Type::SPACE, ' ');
			case '\n': consume(); return Token(Token::Type::NEXTLINE, '\n');
            case '[': consume(); return Token(Token::Type::LBRACKET,'[');
            case ']': consume(); return Token(Token::Type::RBRACKET,']');
			case '{': consume(); return Token(Token::Type::LBRACE, '{');
			case '}': consume(); return Token(Token::Type::RBRACE, '}');
            case '(': consume(); return Token(Token::Type::LPAR,'(');
            case ')': consume(); return Token(Token::Type::RPAR,')');
            case '=': consume(); return Token(Token::Type::EQUALS,'=');
            case '+': consume(); return Token(Token::Type::PLUS,'+');
            case '-': consume(); return Token(Token::Type::MINUS,'-');
            case '*': consume(); return Token(Token::Type::STAR,'*');
			case '\\': consume(); return Token(Token::Type::BSLASH, '\\');
            case '/': consume(); return Token(Token::Type::FSLASH,'/');
            case '.': consume(); return Token(Token::Type::PERIOD,'.');
			case ',': consume(); return Token(Token::Type::COMMA, ',');
            case ':': consume(); return Token(Token::Type::COLON,':');
			case ';': consume(); return Token(Token::Type::SEMICOLON, ';');
			case '!': consume(); return Token(Token::Type::EXCLAM, '!');
            case '"': consume(); return Token(Token::Type::QUOTE,'"');
			case '\'': consume(); return Token(Token::Type::APOST, '\'');
            case '%': consume(); return Token(Token::Type::PERCENT,'%');
			case '&': consume(); return Token(Token::Type::AMPERSAND, '&');
			case '#': consume(); return Token(Token::Type::POUND, '#');
			case '_': consume(); return Token(Token::Type::UNDERSCORE, '_');
            case '0': case '1': case '2': case '3': case '4': case '5':
            case '6': case '7': case '8': case '9': return getNumber();
            default:
                if( isLetter(c) ){
					Token t(Token::Type::LETTER, c);
					consume();
                    return t;
                }else{
					Token t(Token::Type::UNKNOWN, c);
					consume();
                    return t;
                }
        }
        
    }
    
    return Token(Token::Type::EOF_,doneChar);
    
}

void Lexer::consume() {
	if (isFileSource) {
		if (fscanf(file->ref(), "%c", &c) == EOF) {
			c = doneChar;
		}
	}
	else {
		if (it_s == it_e) { c = doneChar; }
		else {
			c = *it_s; ++it_s;
		}
	}
}

bool Lexer::isLetter( char c ) const {
    return ( ( c >= 'a' && c <= 'z' ) || ( c >= 'A' && c <= 'Z' ) );
}

void Lexer::passWhiteSpace() {
    while( ( c == '\t' || c == '\r' ) ){ consume(); }
}
void Lexer::passComment() {
    while( c != '\n' ){ consume(); }
}

Token Lexer::getNumber() {
    Token t(Token::Type::NUMBER, c);
	consume();
	return t;
}

