//
// Author: C.Howard
// Date  : 3/2017
//


#include "text_color.hpp"
#include <stdio.h>
#include <stdarg.h>

#ifdef _WIN32
#include <windows.h>
#else
	static std::string black_s		= "\x1b[30m";
	static std::string red_s		= "\x1b[31;1m";
	static std::string green_s		= "\x1b[32;1m";
	static std::string yellow_s		= "\x1b[33;1m";
	static std::string blue_s		= "\x1b[34;1m";
	static std::string magenta_s	= "\x1b[35;1m";
	static std::string cyan_s		= "\x1b[36;1m";
	static std::string white_s		= "\x1b[37;1m";
	static std::string reset_s		= "\x1b[0m";
#endif

namespace text {

	//enum Color { Red = 0, Green, Yellow, Blue, Magenta, Cyan, Black, White };

#ifdef _WIN32
	unsigned int getColor( enum Color c ){
		unsigned char black = 0, blue = 1, green = 2, red = 4, a = 8, white = 15;
		switch(c){
		case Red:		return red | a;
		case Green:		return green | a;
		case Yellow:	return red | green | a;
		case Blue:		return blue;
		case Magenta:	return red | blue | a;
		case Cyan:		return green | blue | a;
		case Black:		return black;
		case White:		return white;
		}
	}
#else
	const std::string & getColor( enum Color c ){
		switch(c){
		case Red:		return red_s;
		case Green:		return green_s;
		case Yellow:	return yellow_s;
		case Blue:		return blue_s;
		case Magenta:	return magenta_s;
		case Cyan:		return cyan_s;
		case Black:		return black_s;
		case White:		return white_s;
		}
	}
#endif

	void printf_color( enum Color c, const std::string & format, ... ){

#ifdef _WIN32
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
		WORD saved_attributes;
		GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
		saved_attributes = consoleInfo.wAttributes;
		SetConsoleTextAttribute(hConsole, getColor( c ));
		GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
#else
		printf("%s",getColor(c).c_str());
#endif
		const char* format_ = format.c_str();
		va_list args;
		va_start(args, format_);
		vprintf(format_, args);
		va_end(args);

#ifdef _WIN32
		SetConsoleTextAttribute(hConsole, saved_attributes);
#else
		printf("%s",reset_s.c_str());
#endif
	}

	void printf_color( enum Color c, const char * format, ... ){

#ifdef _WIN32
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
		WORD saved_attributes;
		GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
		saved_attributes = consoleInfo.wAttributes;
		SetConsoleTextAttribute(hConsole, getColor( c ));
		GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
#else
		printf("%s",getColor(c).c_str());
#endif
		va_list args;
		va_start (args, format);
		vprintf (format, args);
		va_end (args);

#ifdef _WIN32
		SetConsoleTextAttribute(hConsole, saved_attributes);
#else
		printf("%s",reset_s.c_str());
#endif
	}

}
