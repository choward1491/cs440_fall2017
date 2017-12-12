//
// Author: C.Howard
// Date  : 3/2017
//


#include "vector_pattern.hpp"
#include <algorithm>

namespace pattern {

	void increment(double start, double end, double incr, std::vector<double> & vec ){
		double current = start;
		if( incr < 0.0 ){
			while( current - end > -1e-8 ){ vec.push_back(current); current += incr; } 
		}else if( incr > 0.0 ){
			while( current - end < 1e-8 ){ vec.push_back(current); current += incr; }
		}
	}

	void vector::getVectorFromString( const std::string & vec_string, std::vector<double> & vec ){
		getVectorFromString( vec_string.c_str(), vec );
	}
	void vector::getVectorFromString( const char * vec_string, std::vector<double> & vec ){

		double nums[3] = {0};
		char num_s[64] = {'\0'};
		int cidx = 0, sidx = 0, count = 0;
		while( vec_string[cidx] != '\0' ){

			switch(vec_string[cidx]){
			case '[':
			case ']':
			case ' ':
			case '\n': ++cidx; break;
			case ',': {

				if( sidx != 0 ){
					num_s[sidx] = '\0'; sidx = 0;
					sscanf(num_s,"%lf",&nums[count++]);
				}
				if( count != 0 ){
					if( count == 1 ){
						vec.push_back(nums[0]);
					}else if( count == 2 ){
						increment(nums[0],nums[1],1.0,vec);
					}else if( count == 3 ){
						increment(nums[0],nums[2],nums[1],vec);
					}
					count = 0;
				}
				++cidx;
				break;
			}
			case ':': {
				num_s[sidx] = '\0'; sidx = 0;
				sscanf(num_s,"%lf",&nums[count++]);
				++cidx;
				break;
			}
			default: num_s[sidx++] = vec_string[cidx++];
			}

		}

		// cleanup anything remaining
		if( sidx != 0 ){
			num_s[sidx] = '\0'; sidx = 0;
			sscanf(num_s,"%lf",&nums[count++]);
		}
		if( count != 0 ){
			if( count == 1 ){
				vec.push_back(nums[0]);
			}else if( count == 2 ){
				increment(nums[0],nums[1],1.0,vec);
			}else if( count == 3 ){
				increment(nums[0],nums[2],nums[1],vec);
			}
		}

		// sort result
		std::sort(vec.begin(),vec.end());
		

	}

}
