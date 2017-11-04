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
            for(unsigned int i = 0; i < argc; ++i){
                printf("arg(%i) = %s\n",i,argv[i]);
            }
            throw expn_odd_number_inputs();
        }
    }


    template<>
    float commandline::convert<float>(const std::string & key) const
    {
        float out = 0.0;
        auto ref = input_list.find(key);
        if( ref != input_list.end() ) {
            sscanf(ref->second.c_str(), "%f", &out);
        }
        return out;
    }

    template<>
    double commandline::convert<double>(const std::string & key) const
    {
        double out = 0.0;
        auto ref = input_list.find(key);
        if( ref != input_list.end() ) {
            sscanf(ref->second.c_str(), "%lf", &out);
        }
        return out;
    }

    template<>
    int commandline::convert<int>(const std::string & key) const
    {
        int out = 0.0;
        auto ref = input_list.find(key);
        if( ref != input_list.end() ) {
            sscanf(ref->second.c_str(), "%i", &out);
        }
        return out;
    }

    template<>
    unsigned int commandline::convert<unsigned int>(const std::string & key) const
    {
        unsigned int out = 0.0;
        auto ref = input_list.find(key);
        if( ref != input_list.end() ) {
            sscanf(ref->second.c_str(), "%u", &out);
        }
        return out;
    }

}
