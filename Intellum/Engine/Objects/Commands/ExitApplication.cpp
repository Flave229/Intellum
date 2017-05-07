#include "ExitApplication.h"
#include "../../../DXSystem.h"

ExitApplication::ExitApplication()
{
}

void ExitApplication::Shutdown()
{
}

void ExitApplication::Execute()
{
	DXSystem::ShutdownApplication();
}