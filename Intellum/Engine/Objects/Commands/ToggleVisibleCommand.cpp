#include "ToggleVisibleCommand.h"

ToggleVisibleCommand::ToggleVisibleCommand(AppearanceComponent* appearance) : _appearance(appearance)
{
}

void ToggleVisibleCommand::Shutdown()
{
}

void ToggleVisibleCommand::Execute()
{
	_appearance->RenderEnabled = !_appearance->RenderEnabled;
}