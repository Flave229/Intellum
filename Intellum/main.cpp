#include <windows.system.h>
#include "DXSystem.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	DXSystem* system;

	system = new DXSystem;
	if (!system) return 0;

	system->Run();

	system->Shutdown();
	delete system;
	system = nullptr;

	return 0;
}
