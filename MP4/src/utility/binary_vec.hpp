
#ifndef _binary_vec_hpp_
#define _binary_vec_hpp_

#include "binary_processor.hpp"

namespace binary {

	class vec {
	public:

		// save vector data to file, defaulted to writing a new binary file
		template<typename num_type>
		static void save_vector( const std::string & filename, const std::vector<num_type> & data);
		
		// method to load a vector from a file, assuming the file only contains one vector saved to it
		template<typename num_type>
		static void load_vector( const std::string & filename, std::vector<num_type> & data);

		// method to load a vector from a file handle. Allows one to read multiple vectors from the
		// same file handle if that makes sense
		template<typename num_type>
		static void load_vector( wrap::file & file, std::vector<num_type> & data);

	};

	template<typename num_type>
	void vec::save_vector(const std::string & filename, const std::vector<num_type>& data)
	{
		wrap::file file(filename, wrap::file::WriteBinary);
		if (file.isOpen()) {
			binary::processor::write(static_cast<int>(data.size()), file);	// write number of elements in data first to file
			binary::processor::write(data, file);							// write the data elements to file
		}
	}

	template<typename num_type>
	void vec::load_vector(const std::string & filename, std::vector<num_type>& data)
	{
		wrap::file file(filename, wrap::file::ReadBinary);	// open file in binary read mode
		if (file.isOpen()) {								// check if file is open
			int num_elements = 0;							// init var for storing number of elements in data
			binary::processor::read(file, num_elements);	// get the number of elements
			if ((int)data.size() != num_elements) { 
				data.resize(num_elements); 
			}
			binary::processor::read(file,data);
		}
	}
	template<typename num_type>
	void vec::load_vector(wrap::file & file, std::vector<num_type>& data)
	{
		if (file.isOpen()) {
			int num_elements = 0;
			binary::processor::read(file, num_elements);
			if ((int)data.size() != num_elements) { data.resize(num_elements); }
			binary::processor::read(file, data);
		}
	}
}


#endif
