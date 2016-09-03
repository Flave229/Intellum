#pragma once
#include <vector>
#include <string>

using namespace std;

class Exception
{
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
};

