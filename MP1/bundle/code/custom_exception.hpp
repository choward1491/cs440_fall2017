
#ifndef _custom_exception_hpp_
#define _custom_exception_hpp_

#include <exception>
#include <string>

namespace custom {

    // exception for returning a basic string message
	class exception : public std::exception {
	public:
		exception(const std::string & s_) :s(s_) {}
		virtual const char* what() const throw() { return s.c_str(); }
	private:
		std::string s;
	};

}

#endif
