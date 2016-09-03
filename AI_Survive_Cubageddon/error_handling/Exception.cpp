#include "Exception.h"

Exception::Exception(): _innerException(nullptr), _message("")
{
}

Exception::Exception(const Exception& other): _innerException(other._innerException), _message(other._message)
{
}

Exception::Exception(string message): _innerException(nullptr), _message(message)
{
}

Exception::Exception(string message, Exception innerException): _innerException(new Exception(innerException)), _message(message)
{
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
