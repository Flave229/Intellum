#ifndef _SYSTEM_H_
#define _SYSTEM_H_
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include "Input.h"
#include "Graphics.h"
#include "engine/GameTimer.h"

class System
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
	System();
	System(const System&);
	~System();

	bool Initialise();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static System* ApplicationHandle = nullptr;

#endif