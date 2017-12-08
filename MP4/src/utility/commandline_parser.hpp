//
//  Created by Christian J Howard
//


#ifndef _commandline_parser_hpp_
#define _commandline_parser_hpp_

#include <exception>
#include <string>
#include <map>


namespace parser {

	// general class used to parse a collection of input parameters
	//
	// The code assumes the parameters will come in the following form:
	// param_name1 param_value1 param_name2 param_value2 ...
	class commandline {
	public:

		// constructor for commandline parser
		commandline(int argc, char** argv);

		// method to get raw param_value string from list
		const std::string & operator[](const std::string & key) const;


		// method to convert input param to some number
		template<typename precision_type>
		precision_type convert(const std::string & key) const;

	private:

		// helper function to build param list
		void build_list(int argc, char** argv);

		// list storage
		std::map<std::string, std::string> input_list;

		// null string to return in rare cases
		std::string null_string;
	};


	// exception to be thrown if commandline inputs are invalid
	class expn_odd_number_inputs : public std::exception {
	public:
		virtual const char* what() const throw() {
			return "Passed odd number of command-line inputs, which isn't allowed. Make sure there's a parameter flag before each input";
		}
	};
		
}


#endif