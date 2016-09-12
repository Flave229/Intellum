#include "Exception.h"

Exception::Exception(): _innerException(nullptr), _message("")
{
	CaptureStackBackTrace(1, 63, _stackTrace, nullptr);
}

Exception::Exception(const Exception& other): _innerException(other._innerException), _message(other._message)
{
	CaptureStackBackTrace(1, 63, _stackTrace, nullptr);
}

Exception::Exception(string message): _innerException(nullptr), _message(message)
{
	CaptureStackBackTrace(1, 63, _stackTrace, nullptr);
}

Exception::Exception(string message, Exception innerException): _innerException(new Exception(innerException)), _message(message)
{
	CaptureStackBackTrace(1, 63, _stackTrace, nullptr);
}

Exception::~Exception()
{
}

void Exception::Shutdown()
{
	if (_innerException)
	{
		_innerException->Shutdown();
		_innerException = nullptr;
	}
}

string Exception::PrintFullMessage()
{
	string message = _message;

	if (_innerException)
		message.append("\n").append(_innerException->PrintFullMessage());

	return message;
}

string Exception::PrintStackTrace()
{
	SymSetOptions(SYMOPT_DEFERRED_LOADS | SYMOPT_INCLUDE_32BIT_MODULES | SYMOPT_UNDNAME);
	if (!SymInitialize(GetCurrentProcess(), "http://msdl.microsoft.com/download/symbols", TRUE)) return "";

	string stackTrace = "";

	for (USHORT i = 0; i < _stackTraceHistory; i++) 
	{
		ULONG64 buffer[(sizeof(SYMBOL_INFO) + 1024 + sizeof(ULONG64) - 1) / sizeof(ULONG64)] = { 0 };
		SYMBOL_INFO *info = reinterpret_cast<SYMBOL_INFO*>(buffer);
		info->SizeOfStruct = sizeof(SYMBOL_INFO);
		info->MaxNameLen = 1024;

		// Attempt to get information about the symbol and add it to our output parameter.
		DWORD64 displacement = 0;
		if (::SymFromAddr(::GetCurrentProcess(), reinterpret_cast<DWORD64>(_stackTrace[i]), &displacement, info)) {
			stackTrace.append(info->Name, info->NameLen);
			stackTrace.append("\n");
		}
	}

	return stackTrace;
}
