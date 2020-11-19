#pragma once


enum class Result
{
	Success,
	ConnectionReset,
	GenericError
};

Result errorCodeToResult(int errorCode);
