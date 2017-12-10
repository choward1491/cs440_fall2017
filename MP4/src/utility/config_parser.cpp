/*
 *  config_parser.cpp
 *
 *  Project: meshing
 *  Author : Christian J Howard
 *  Date   : 11/20/17
 *  Purpose:
 *
 */

#include "config_parser.hpp"
#include "Lexer.hpp"
#include "vector_pattern.hpp"

namespace parser {
    
    // typedef the token type
    typedef Token::Type token_t;
    
    /*!
     * \brief Method used to parse some file contents and retrieve config file data.
     */
    void config::parse( const std::string & filename ) {
        wrap::file cfile(filename,wrap::file::Read);
        Lexer lex(cfile);
        ptokens = &lex.getTokens();
        if( ptokens->empty() ){ return; }
        interpreter(keyval_map);
    }
    
    
    /*!
     * \brief Method to see if some string exists
     */
    bool config::exist( const std::string & varname ) const {
        auto it = keyval_map.find(varname);
        return ( it != keyval_map.end() );
    }
    
    
    /*!
     * \brief Method used to retrieve data stored by some variable in the config file
     *        such that the output data is in a desired form
     */
    template<>
    std::string config::retrieve( const std::string & varname) const {
        auto it = keyval_map.find(varname);
        if( it != keyval_map.end() ){ return it->second; }
        else{ return ""; }
    }
    
    template<>
    double config::retrieve( const std::string & varname) const {
        auto it = keyval_map.find(varname);
        if( it != keyval_map.end() ){
            double out = 0.0;
            sscanf(it->second.c_str(), "%lf", &out);
            return out;
        }
        else{ return 0.0; }
    }
    
    template<>
    float config::retrieve( const std::string & varname) const {
        auto it = keyval_map.find(varname);
        if( it != keyval_map.end() ){
            float out = 0.0;
            sscanf(it->second.c_str(), "%f", &out);
            return out;
        }
        else{ return 0.0; }
    }
    
    template<>
    int config::retrieve( const std::string & varname) const {
        auto it = keyval_map.find(varname);
        if( it != keyval_map.end() ){
            int out = 0;
            sscanf(it->second.c_str(), "%i", &out);
            return out;
        }
        else{ return 0; }
    }
    
    template<>
    unsigned int config::retrieve( const std::string & varname) const {
        auto it = keyval_map.find(varname);
        if( it != keyval_map.end() ){
            unsigned int out = 0;
            sscanf(it->second.c_str(), "%u", &out);
            return out;
        }
        else{ return 0; }
    }
    
    template<>
    bool config::retrieve( const std::string & varname) const {
        auto it = keyval_map.find(varname);
        if( it != keyval_map.end() ){
            int out = 0;
            sscanf(it->second.c_str(), "%i", &out);
            return out;
        }
        else{ return false; }
    }
    
    template<>
    std::vector<double> config::retrieve( const std::string & varname) const {
        auto it = keyval_map.find(varname);
        if( it != keyval_map.end() ){
            std::vector<double> out;
            pattern::vector::getVectorFromString(it->second, out);
            return out;
        }
        else{ return std::vector<double>(); }
    }
    
    void config::interpreter(std::map<std::string,std::string> & kvlist ) {
        token_t type = token_t::SPACE;
        idx = 0;
        while( !reachedEndFile() ){
            type = getToken().getType();
            switch(type){
                case token_t::LETTER:
                case token_t::UNDERSCORE: readKeyValuePair(kvlist); break;
                case token_t::POUND: skipComments(); break;
                default: updateIndex();
            }
        }// end while loop
    }
    
    const Token & config::getToken(int del) const {
        return (*ptokens)[idx + del];
    }
    bool config::reachedEndFile() const {
        return idx >= ptokens->size() || (*ptokens)[idx].getType() == token_t::EOF_ ;
    }
    void config::updateIndex() {
        idx++;
    }
    void config::skipAllSpace() {
        token_t type = token_t::SEMICOLON;
        while( (type = getToken().getType()) == token_t::SPACE ){
            updateIndex();
        }
    }
    void config::skipAllSpaceAndNextline() {
        token_t type = token_t::SEMICOLON;
        while( ((type = getToken().getType()) == token_t::SPACE ||
              type == token_t::NEXTLINE) )
        {
            updateIndex();
        }
    }
    void config::skipComments() {
        token_t type = token_t::POUND;
        while( (type = getToken().getType()) != token_t::NEXTLINE &&
              type != token_t::EOF_)
        {
            updateIndex();
        }
        if( type == token_t::NEXTLINE ){ updateIndex(); }
    }
    
    void config::readString() {
        tmp.clear();
        token_t type = token_t::POUND;
        while( (type = getToken().getType()) != token_t::NEXTLINE &&
              type != token_t::SPACE && type != token_t::COLON &&
              type != token_t::POUND && type != token_t::EOF_ )
        {
            tmp.push_back(getToken().getChar());
            updateIndex();
        }
    }
    void config::readStringWithSpaces() {
        tmp.clear();
        token_t type = token_t::POUND;
        while( (type = getToken().getType()) != token_t::NEXTLINE &&
                type != token_t::POUND && type != token_t::EOF_ )
        {
            tmp.push_back(getToken().getChar());
            updateIndex();
        }
    }
    
    void config::readKeyValuePair( std::map<std::string,std::string> & kvlist ) {
        std::string key;
        readString(); key = tmp;
        skipAllSpaceAndNextline();
        if( getToken().getType() == token_t::COLON ){ updateIndex(); }
        else{ /* throw error */ }
        skipAllSpaceAndNextline();
        //readStringWithSpaces();
        readString();
        skipAllSpace();
        if( getToken().getType() == token_t::POUND ){ skipComments(); }
        else{ skipAllSpaceAndNextline(); }
        kvlist[key] = tmp;
    }
    
    void config::print() const {
        for(auto it = keyval_map.begin(); it != keyval_map.end(); ++it){
            printf("(%s, %s)\n",it->first.c_str(), it->second.c_str());
        }
    }
    
    // map used to store key-value pairs
    //std::map<std::string,std::string> keyval_map;}
}
