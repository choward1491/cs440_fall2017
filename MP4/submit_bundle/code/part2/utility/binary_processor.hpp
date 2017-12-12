
#ifndef _binary_processor_
#define _binary_processor_

#include <vector>
#include <stdio.h>
#include "FileWrap.hpp"

namespace binary {

	class processor {
	public:

		/* Methods to write data to file*/

		// method to write a single instance of some type to file
		template<typename num_type>
		static void write(const num_type & num, wrap::file & file);

		// method to write a vector of some type to file
		template<typename num_type>
		static void write(const std::vector<num_type> & num, wrap::file & file);





		/* Methods to read data from file*/

		// method to read a single value from file and store it in input `num`
		template<typename num_type>
		static bool read(wrap::file & file, num_type & num);

		// method to read a vector of numbers from file that are the same size
		// as the vetor input `numbers`
		template<typename num_type>
		static bool read(wrap::file & file, std::vector<num_type> & numbers);

		// method to read a vector of `num_get` numbers from a binary file and store them
		// in the `numbers` vector input starting at index `start_idx` in the `numbers` array
		template<typename num_type>
		static bool read(wrap::file & file, int num_get, std::vector<num_type> & numbers, int start_idx = 0);

	};




	template<typename num_type>
	inline void processor::write(const num_type & num, wrap::file & file)
	{
		typedef union { unsigned char buf[16]; num_type value; } map;	// define useful union var
		const unsigned int numbytes = sizeof(num_type) / sizeof(char);	// define number of bytes for type
		map tmp; tmp.value = num;										// init union type instance with passed in `num`
		fwrite(tmp.buf, numbytes, 1, file);								// write data to file with fwrite
	}

	template<typename num_type>
	inline void processor::write(const std::vector<num_type>& nums, wrap::file & file)
	{
		typedef union { unsigned char buf[16]; num_type value; } map;	// define useful union var
		const unsigned int numbytes = sizeof(num_type) / sizeof(char);	// define number of bytes for type
		map tmp; tmp.value = 0;											// init union type instance to null
		unsigned char * data = new unsigned char[numbytes*nums.size()];	// init heap-based buffer of the appropriate size
		for (unsigned int i = 0; i < nums.size(); ++i) {				// loop through vector values
			tmp.value = nums[i];										// set union `value` to ith vector value
			for (int j = 0; j < numbytes; ++j) {						// write bytes of `value` to buffer
				data[i*numbytes + j] = tmp.buf[j];
			}
		}

		fwrite(data, numbytes, nums.size(), file);						// write buffer to file
		delete[] data; data = nullptr;									// free buffer data
	}






	template<typename num_type>
	inline bool processor::read(wrap::file & file, num_type & num)
	{
		typedef union { unsigned char buf[16]; num_type value; } map;	// define useful union var
		const unsigned int numbytes = sizeof(num_type) / sizeof(char);	// define number of bytes for type
		map tmp; tmp.value = 0;											// init union type instance to null
		size_t cnt = fread(tmp.buf, numbytes, 1, file);					// read bytes from file into buffer
		num = tmp.value;												// set output `num` value to `value` in union instance
		return (1 == cnt);
	}

	template<typename num_type>
	inline bool processor::read(wrap::file & file, std::vector<num_type>& nums)
	{
		typedef union { unsigned char buf[16]; num_type value; } map;	// define useful union var
		const unsigned int numbytes = sizeof(num_type) / sizeof(char);	// define number of bytes for type
		unsigned char * data = new unsigned char[numbytes*nums.size()];	// init heap-based buffer of the appropriate size
		map tmp; tmp.value = 0;											// init union type instance to null
		size_t cnt = fread(data, numbytes, nums.size(), file);			// read chunk of bytes from file into buffer
		for (int i = 0; i < nums.size(); ++i) {							// loop through buffer to set ith value from bytes
			for (int j = 0; j < numbytes; ++j) {
				tmp.buf[j] = data[i*numbytes + j];
			}
			nums[i] = tmp.value;
		}
		delete[] data; data = nullptr;									// free buffer
		return cnt == nums.size();
	}

	template<typename num_type>
	inline bool processor::read(wrap::file & file, int num_get, std::vector<num_type>& nums, int start_idx)
	{
		typedef union { unsigned char buf[16]; num_type value; } map;	// define useful union var
		const unsigned int numbytes = sizeof(num_type) / sizeof(char);	// define number of bytes for type
		if (nums.size() < (unsigned int)(num_get + start_idx)) {		// resize input array `nums` if needed
			nums.resize(num_get + start_idx); 
		}	
		unsigned char * data = new unsigned char[numbytes*num_get];		// init heap-based buffer of the appropriate size
		map tmp; tmp.value = 0;											// init union type instance to null
		size_t cnt = fread(data, numbytes, num_get, file);				// read chunk of bytes from file into buffer
		for (int i = 0; i < num_get; ++i) {								// loop through buffer to set ith value from bytes
			for (int j = 0; j < numbytes; ++j) {
				tmp.buf[j] = data[i*numbytes + j];
			}
			nums[start_idx + i] = tmp.value;
		}
		delete[] data; data = nullptr;									// free buffer
		return cnt == (size_t)num_get;
	}

}


#endif
