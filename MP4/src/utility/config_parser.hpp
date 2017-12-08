/*
 *  config_parser.hpp
 *
 *  Project: meshing
 *  Author : Christian J Howard
 *  Date   : 11/20/17
 *  Purpose:
 *
 */

#ifndef config_parser_hpp
#define config_parser_hpp

// standard headers
#include <map>
#include <vector>

// custom headers
#include "FileWrap.hpp"

// declare some helper classes
class Token;


namespace parser {
    
    /*!
     * \brief Class used for parsing and retrieving data from config files. All variables in a config file can only have one unique value.
     */
    class config {
    public:
        
        // default ctor/dtor
        config()    = default;
        ~config()   = default;
        
        /*!
         * \brief Method used to parse some file contents and retrieve config file data.
         */
        void parse( const std::string & filename );
        
        /*!
         * \brief Method used to retrieve data stored by some variable in the config file
         *        such that the output data is in a desired form
         */
        template<typename type>
        type retrieve( const std::string & varname) const ;
        
        
        /*!
         * \brief Method to print key-value pairs in the internal storage
         */
        void print() const;
        
    private:
        
        // map used to store key-value pairs
        unsigned int idx;
        const std::vector<Token>* ptokens;
        std::map<std::string,std::string> keyval_map;
        std::string tmp;
        
        // helper methods
        void interpreter(std::map<std::string,std::string> & kvlist );
        const Token & getToken(int del = 0) const;
        bool reachedEndFile() const;
        void updateIndex();
        void skipAllSpace();
        void skipAllSpaceAndNextline();
        void skipComments();
        void readString();
        void readStringWithSpaces();
        void readKeyValuePair( std::map<std::string,std::string> & kvlist );
        
    };
}

#endif /* config_parser_hpp */
