#include <windows.system.h>
#include "DXSystem.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	DXSystem* system;
	bool result;

	system = new DXSystem;

	if (!system) return 0;

	result = system->Initialise();
	if (result) system->Run();

	system->Shutdown();
	delete system;
	system = nullptr;

	return 0;
}
