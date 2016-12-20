#pragma once


#include <iostream>


#define B(msg)			"\033[1m" << msg << "\033[0m"
#define U(msg)			"\033[4m" << msg << "\033[0m"
#define LOG(msg)		std::cout << msg << std::endl

#define WARNING(msg)	std::cerr << "WARNING: " << msg << std::endl
#define ERROR(msg)		std::cerr << "ERROR: " << msg << std::endl

#define OUT_HEX(num)    std::hex << (num) << std::dec

#define __STR_AUX(x)	#x
#define __STR(x)		__STR_AUX(x)
#define STR__LINE__		__STR(__LINE__)
#define ASSERT(X)       if(!(X)) throw std::logic_error(#X " @ " __FILE__ ":" STR__LINE__)

