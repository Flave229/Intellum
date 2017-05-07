#pragma once
#include "ICommand.h"

class ExitApplication : public ICommand
{
public:
	ExitApplication();
	~ExitApplication() override = default;

	void Shutdown() override;
	void Execute() override;
};