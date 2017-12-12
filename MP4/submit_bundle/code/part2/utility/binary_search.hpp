//
// Author: C. Howard
//

#ifndef _binary_search_hpp_
#define _binary_search_hpp_


namespace algos {

	// nominal comparison class
	// assumes the type T already has comparison operators defined
	template<typename T>
	class nominal_compare {
	public:
		static bool leq(const T& v1, const T& v2) {
			return (v1 <= v2);
		}
		static bool geq(const T& v1, const T& v2) {
			return (v1 >= v2);
		}
	};

	// binary search algorithm
	// returns the index of the element within fdata that is the lower
	// bound element bounding the variable value. Thus, the following 
	// inequality should hold: fdata[result] <= value <= fdata[result+1]
	template<typename T, template<typename> class compare = nominal_compare>
	int binary_search(const T & value, const T* fdata, int size_data) {

		// init useful temp variables
		int end = size_data - 1, li = 0, ri = end, ci;

		// check to see if the value is out of bounds of the input
		// array fdata. Assumes fdata is sorted from smallest to largest
		// based on the comparison operator compare<T>
		if (compare<T>::leq(value, fdata[0]) )		{ return 0; }
		if (compare<T>::geq(value, fdata[end]) )	{ return end; }

		// Do binary search iteratively (avoids stack calls)
		while ( ri != li ) {
			ci = (li + ri) / 2;
			if (compare<T>::leq(value,fdata[ci])) {
				ri = ci;
			}
			else {
				li = ci + 1;
			}
		}

		// return lower bound index
        if( value == fdata[ci] ){
            return li;
        }else{
            return li-1;
        }
	}
}



#endif

