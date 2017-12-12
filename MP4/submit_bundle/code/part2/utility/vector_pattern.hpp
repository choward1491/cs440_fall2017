//
// Author: C.Howard
// Date  : 3/2017
//


#ifndef _vector_pattern_hpp_
#define _vector_pattern_hpp_

#include <string>
#include <vector>

namespace pattern {

	// class for parsing matlab like numeric array specifications and placing them in a sorted std::vector<double>. Examples include parsing:
	// 1) [1:5,10] -> [1,2,3,4,5,10]
	// 2) [0:0.1:0.9,-1] -> [-1, 0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9]
	class vector {
	public:
		static void getVectorFromString( const std::string & vec_string, std::vector<double> & vec );
		static void getVectorFromString( const char * vec_string, std::vector<double> & vec );
	};

}

#endif