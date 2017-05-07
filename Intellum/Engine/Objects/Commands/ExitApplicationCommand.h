#pragma once
#include "ICommand.h"

class ExitApplicationCommand : public ICommand
{
public:
	ExitApplicationCommand();
	~ExitApplicationCommand() override = default;

	void Shutdown() override;
	void Execute() override;
};