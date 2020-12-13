#include "NetworkException.h"
#include <Windows.h>
#include <sstream>
#include <winbase.h>

NetworkException::NetworkException(int line, const char* file, int errorCode)
	: line(line), file(file), errorCode(errorCode)
{
}

const char* NetworkException::what() const
{
	std::ostringstream oss;
	oss << getType() << std::endl
		<< "[Error code] " << getErrorCode() << std::endl
		<< "[Description] " << getErrorString() << std::endl
		<< getOriginString() << std::endl;
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* NetworkException::getType() const
{
	return "Network Exception";
}

int NetworkException::getLine() const
{
	return line;
}

std::string NetworkException::getFile() const
{
	return file;
}

std::string NetworkException::getOriginString() const
{
	std::ostringstream oss;
	oss << "[File] " << file << std::endl
		<< "[Line] " << line;
	return oss.str();
}

int NetworkException::getErrorCode() const
{
	return errorCode;
}

std::string NetworkException::getErrorString() const
{
	return translateErrorCode(errorCode);
}

std::string NetworkException::translateErrorCode(int errorCode)
{
	char* msgBuf;
	DWORD msgLength = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		errorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPTSTR>(&msgBuf),
		0,
		nullptr);
	if (msgLength == 0)
	{
		return "Unidentified error code";
	}
	std::string errorString = msgBuf;
	LocalFree(msgBuf);
	return errorString;
}