#include "Result.h"
#include <WinSock2.h>

Result errorCodeToResult(int errorCode)
{
	switch (errorCode)
	{
	case WSAECONNRESET:
		return Result::ConnectionReset;
	default:
		return Result::GenericError;
	}
}
