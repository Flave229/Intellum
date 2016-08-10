#include "System.h"

System::System(): _applicationName(nullptr), _hInstance(nullptr), _hwnd(nullptr), _input(nullptr), _graphics(nullptr)
{
}

System::System(const System& other): _applicationName(nullptr), _hInstance(nullptr), _hwnd(nullptr), _input(nullptr), _graphics(nullptr) 
{
}

System::~System()
{
}

bool System::Initialise()
{
	bool result;
	auto screenWidth = 1280;
	auto screenHeight = 720;

	InitialiseWindows(screenWidth, screenHeight);

	_input = new Input();
	if (!_input) return false;
	_input->Initialise();

	_graphics = new Graphics();
	if (!_graphics) return false;
	result = _graphics->Initialise(screenWidth, screenHeight, _hwnd);
	if (!result) return false;

	return true;
}

void System::Shutdown()
{
	if(_graphics)
	{
		_graphics->Shutdown();
		delete _graphics;
		_graphics = nullptr;
	}

	if (_input)
	{
		delete _input;
		_input = nullptr;
	}

	ShutdownWindows();
}

void System::Run()
{
	MSG msg;
	auto done = false;
	bool result;

	ZeroMemory(&msg, sizeof(MSG));
	GameTimer* timer = new GameTimer();

	timer->Reset();

	while(!done)
	{
		if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// Handles application exit if Windows ends application
		if (msg.message != WM_QUIT)
		{
			timer->Tick();

			result = Frame(timer->DeltaTime());
			if (!result) done = true;
		}
		else
		{
			done = true;
		}
	}
}

bool System::Frame(float delta)
{
	bool result;

	if (_input->IsKeyDown(VK_ESCAPE)) return false;

	result = _graphics->Frame(delta);

	if (!result) return false;

	return true;
}

LRESULT System::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch(umsg)
	{
		case WM_KEYDOWN:
		{
			_input->KeyDown(static_cast<unsigned int>(wparam));
			return 0;
		}
		case WM_KEYUP:
		{
			_input->KeyUp(static_cast<unsigned int>(wparam));
			return 0;
		}
		default:
			return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
}

LRESULT __stdcall WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch(umessage)
	{
		// Window Destroyed
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		// Window closed
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}

void System::InitialiseWindows(int&, int&)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX;
	int posY;

	ApplicationHandle = this;
	_hInstance = GetModuleHandle(nullptr);
	_applicationName = L"Survive Cubageddon";

	// General setup required for windows class
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = _hInstance;
	wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = _applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	auto screenWidth = GetSystemMetrics(SM_CXSCREEN);
	auto screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if(FULL_SCREEN)
	{
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = static_cast<unsigned long>(screenWidth);
		dmScreenSettings.dmPelsHeight = static_cast<unsigned long>(screenHeight);
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		posX = 0;
		posY = 0;
	}
	else
	{
		// Else statement sets window to windowed mode
		screenWidth = 1280;
		screenHeight = 720;

		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	_hwnd = CreateWindowEx(WS_EX_APPWINDOW, _applicationName, _applicationName, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, 
		posX, posY, screenWidth, screenHeight, nullptr, nullptr, _hInstance, nullptr);

	ShowWindow(_hwnd, SW_SHOW);
	SetForegroundWindow(_hwnd);
	SetFocus(_hwnd);

	ShowCursor(false);
}

void System::ShutdownWindows()
{
	ShowCursor(true);

	if(FULL_SCREEN)
	{
		ChangeDisplaySettings(nullptr, 0);
	}

	DestroyWindow(_hwnd);
	_hwnd = nullptr;

	UnregisterClass(_applicationName, _hInstance);
	_hInstance = nullptr;

	ApplicationHandle = nullptr;
}
