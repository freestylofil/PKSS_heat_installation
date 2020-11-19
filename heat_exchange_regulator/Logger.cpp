#include "Logger.h"

PassingStream passingStream;

PassingStream::PassingStream()
	: std::ostream(nullptr)
{
}
