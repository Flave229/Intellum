#ifndef _SYSTEM_H_
#define _SYSTEM_H_
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include "Input.h"
#include "Graphics.h"
#include "engine/GameTimer.h"

class DXSystem
{
// Member-Level Variables
private:
	LPCWSTR _applicationName;
	HINSTANCE _hInstance;
	HWND _hwnd;

	GameTimer* _timer;

	Input* _input;
	Graphics* _graphics;
// Function Declarations
private:
	bool Frame(float delta);
	void InitialiseWindows(int&, int&);
	void ShutdownWindows();
public:
	DXSystem();
	DXSystem(const DXSystem&);
	~DXSystem();

	bool Initialise();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
};

static LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
static DXSystem* ApplicationHandle = nullptr;

#endif