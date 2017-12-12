//
// Author: C.Howard
// Date  : 3/2017
//


#ifndef _message_exception_hpp_
#define _message_exception_hpp_

#include <stdio.h>

// exception that should allow for printing
// formatted text strings (with colors and whatnot)
// relative to typical std::exceptions
class MessageException {
public:
	void msg(){ custom_message(); }
private:
	virtual void custom_message() = 0;
};



#endif