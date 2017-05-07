#include "ExitApplicationCommand.h"
#include "../../../DXSystem.h"

ExitApplicationCommand::ExitApplicationCommand()
{
}

void ExitApplicationCommand::Shutdown()
{
}

void ExitApplicationCommand::Execute()
{
	DXSystem::ShutdownApplication();
}