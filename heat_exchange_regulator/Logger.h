#pragma once
#include <iostream>
#include <sstream>

#ifdef NDEBUG
	#define LOG passingStream
#else
	#define LOG std::cout
#endif

class PassingStream : public std::ostream
{
public:
	PassingStream();
};

extern PassingStream passingStream;