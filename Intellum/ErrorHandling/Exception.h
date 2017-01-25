#pragma once
#include <vector>
#include <string>
#include <Windows.h>
#include <winbase.h>
#include <DbgHelp.h>

using namespace std;

class Exception
{
private:
	static const int _stackTraceHistory = 48;
	void* _stackTrace[_stackTraceHistory];

public:
	Exception* _innerException;
	string _message;

public:
	Exception();
	Exception(const Exception&);
	Exception(string message);
	Exception(string message, Exception innerException);
	~Exception();

	void Shutdown();

	string PrintFullMessage();
	string PrintStackTrace();
};