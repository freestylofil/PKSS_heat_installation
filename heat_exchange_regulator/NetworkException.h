#pragma once
#include <exception>
#include <string>

class NetworkException : public std::exception
{
public:
	NetworkException(int line, const char* file, int errorCode);
	const char* what() const override;
	virtual const char* getType() const;
	int getLine() const;
	std::string getFile() const;
	std::string getOriginString() const;
	int getErrorCode() const;
	std::string getErrorString() const;
	static std::string translateErrorCode(int errorCode);
private:
	int line;
	std::string file;
	mutable std::string whatBuffer;
	int errorCode;
};

#define NETWORK_EXCEPTION(errorCode) NetworkException(__LINE__, __FILE__, errorCode)
