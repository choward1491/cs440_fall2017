//
//  Created by Christian J Howard
//


#include "commandline_parser.hpp"
#include <stdio.h>


namespace parser {

    commandline::commandline(int argc, char** argv) :null_string(""), input_list() {
        build_list(argc, argv);
    }

    const std::string & commandline::operator[](const std::string & key) const {
        if (input_list.count(key) != 0) {
            return input_list.find(key)->second;
        }
        else {
            return null_string;
        }
    }

    void commandline::build_list(int argc, char** argv) {
        int num_args = argc - 1;
        if (num_args % 2 == 0 && num_args != 0) {

            for (int i = 1; i < argc; i += 2) {
                input_list[std::string(argv[i])] = std::string(argv[i + 1]);
            }

        }
        else if( num_args != 0 ){
            throw expn_odd_number_inputs();
        }
    }


    template<>
    inline float commandline::convert(const std::string & key) const
    {
        float out = 0.0;
        sscanf(key.c_str(), "%f", &out);
        return out;
    }

    template<>
    inline double commandline::convert(const std::string & key) const
    {
        double out = 0.0;
        sscanf(key.c_str(), "%lf", &out);
        return out;
    }

    template<>
    inline int commandline::convert(const std::string & key) const
    {
        int out = 0.0;
        sscanf(key.c_str(), "%i", &out);
        return out;
    }

    template<>
    inline unsigned int commandline::convert(const std::string & key) const
    {
        unsigned int out = 0.0;
        sscanf(key.c_str(), "%u", &out);
        return out;
    }

}
