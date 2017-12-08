//
// Author: C.Howard
// Date  : 3/2017
//


#ifndef _text_color_hpp_
#define _text_color_hpp_

#include <string>

namespace text {

	// enum with color values that are useable
	enum Color { Red = 0, Green, Yellow, Blue, Magenta, Cyan, Black, White };
	
	// cross platform methods to make colored text in the terminal to help emphasize things
	void printf_color( enum Color c, const std::string & format, ... );
	void printf_color( enum Color c, const char * format, ... );

}

#endif