#ifndef _SYSTEM_H_
#define _SYSTEM_H_
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include "Input.h"
#include "Engine/Graphics/Graphics.h"
#include "Engine/GameTimer.h"
#include "Engine/SystemMetrics/FramesPerSecond.h"
#include "Engine/SystemMetrics/Cpu.h"

class DXSystem
{
private:
	LPCWSTR _applicationName;
	HINSTANCE _hInstance;
	HWND _hwnd;

	FramesPerSecond* _framesPerSecond;
	Cpu* _cpu;
	GameTimer* _timer;

	Input* _input;
	Graphics* _graphics;

private:
	void Initialise();
	void InitialiseWindows(Box& screenSize);

	bool Frame(float delta) const;
	void ShutdownWindows();
public:
	DXSystem();
	DXSystem(const DXSystem&);
	~DXSystem();

	void Shutdown();
	void Run();

	static LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
};

static LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
static DXSystem* ApplicationHandle = nullptr;

#endif