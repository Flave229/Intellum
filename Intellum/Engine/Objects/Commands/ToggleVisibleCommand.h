#pragma once
#include "ICommand.h"
#include "../Components/AppearanceComponent.h"

class ToggleVisibleCommand : public ICommand
{
private:
	AppearanceComponent* _appearance;
public:
	ToggleVisibleCommand(AppearanceComponent* appearance);
	~ToggleVisibleCommand() override = default;
	void Shutdown() override;
	void Execute() override;
};