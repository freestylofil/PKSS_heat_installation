#pragma once

enum class Result
{
	success = 0,
	connectionReset = 1,
	genericError = 2,
	wouldBlock = 3
};

Result errorCodeToResult(int errorCode);