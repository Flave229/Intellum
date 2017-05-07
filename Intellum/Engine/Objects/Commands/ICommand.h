#pragma once

class ICommand
{
public:
	ICommand() {}
	virtual ~ICommand() {}
	virtual void Shutdown() = 0;

	virtual void Execute() = 0;
};