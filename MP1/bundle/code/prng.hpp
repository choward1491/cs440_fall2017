//
//  prng.hpp
//
//  Created by Christian J Howard on 8/9/15.
//

#ifndef RandomNumberGenerator_hpp
#define RandomNumberGenerator_hpp


#include <stdio.h>
#include "stdint.h"


// random number generator
class prng {

public:
	prng(int seed = 173);
	void setSeed(int seed);
	double rand();
	uint64_t randInt();
	double gaussRand();
	double gaussRand(double mean, double sigma);
	void print() const;

private:
	uint64_t currentNumber;
	uint64_t s[16];
	int p;
	const uint64_t MAX_NUM_INT = 0xFFFFFFFFFFFFFFFF;
	const double INV_MAX_NUM_DBL = 1.0 / (double)MAX_NUM_INT;

	double gaussGetXFromCDF(double cdfValue);
	int binarySearchForIndex(double value, double * cdf, int size);
};



#endif /* RandomNumberGenerator_cpp */
