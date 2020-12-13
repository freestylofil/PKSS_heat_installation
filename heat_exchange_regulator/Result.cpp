#include "Result.h"
#include <WinSock2.h>

Result errorCodeToResult(int errorCode)
{
	switch (errorCode)
	{
	case WSAECONNRESET:
		return Result::connectionReset;
	case WSAEWOULDBLOCK:
		return Result::wouldBlock;
	default:
		return Result::genericError;
	}
}