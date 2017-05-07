#pragma once
#include "ICommand.h"

class NullCommand : public ICommand
{
public:
	NullCommand();
	~NullCommand() override = default;
	void Shutdown() override;
	void Execute() override;
};