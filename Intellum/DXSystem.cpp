#include "DXSystem.h"
#include "ErrorHandling/Exception.h"

DXSystem::DXSystem(): _applicationName(nullptr), _hInstance(nullptr), _hwnd(nullptr), _framesPerSecond(nullptr), _cpu(nullptr), _timer(nullptr), _input(nullptr), _graphics(nullptr)
{
	Initialise();
}

DXSystem::DXSystem(const DXSystem& other): _applicationName(other._applicationName), _hInstance(other._hInstance), _hwnd(other._hwnd), _framesPerSecond(other._framesPerSecond), _cpu(other._cpu), _timer(other._timer), _input(other._input), _graphics(other._graphics)
{
}

DXSystem::~DXSystem()
{
}

void DXSystem::Initialise()
{
	try
	{
		Box* screenSize = new Box(1280, 720);

		InitialiseWindows(*screenSize);

		_framesPerSecond = new FramesPerSecond();
		_cpu = new Cpu();

		_input = new Input(_hInstance, _hwnd, *screenSize);
		_graphics = new Graphics(_input, screenSize, _hwnd, _framesPerSecond, _cpu);
	}
	catch(Exception& exception)
	{
		string exceptionMessage = exception.PrintFullMessage();
		exceptionMessage.append("\n\nStack Trace:\n").append(exception.PrintStackTrace());

		MessageBox(_hwnd, wstring(exceptionMessage.begin(), exceptionMessage.end()).c_str(), L"Error", MB_OK);

		exception.Shutdown();
	}
	catch(...)
	{
		MessageBox(_hwnd, L"An unknown error occured while initialising the system.", L"Error", MB_OK);
	}
}

void DXSystem::Shutdown()
{
	if(_graphics)
	{
		_graphics->Shutdown();
		delete _graphics;
		_graphics = nullptr;
	}

	if (_input)
	{
		_input->Shutdown();
		delete _input;
		_input = nullptr;
	}

	if (_framesPerSecond)
	{
		delete _framesPerSecond;
		_framesPerSecond = nullptr;
	}

	if (_cpu)
	{
		delete _cpu;
		_cpu = nullptr;
	}

	ShutdownWindows();
}

void DXSystem::Run()
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

		if (_input->IsControlPressed(ESCAPE))
		{
			done = true;
		}
	}
}

bool DXSystem::Frame(float delta) const
{
	try
	{
		_input->Update();

		_framesPerSecond->Frame(delta);
		_cpu->Frame();
		
		_graphics->Update(delta);

		XMFLOAT2 mousePoint;
		_input->MapMouseLocationInto(mousePoint);

		_graphics->Render(mousePoint);

		return true;
	}
	catch(Exception& exception)
	{
		string exceptionMessage = exception.PrintFullMessage();
		exceptionMessage.append("\n\nStack Trace:\n").append(exception.PrintStackTrace());

		MessageBox(_hwnd, wstring(exceptionMessage.begin(), exceptionMessage.end()).c_str(), L"Error", MB_OK);

		exception.Shutdown();

		return false;
	}
}

LRESULT DXSystem::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, umsg, wparam, lparam);
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

void DXSystem::InitialiseWindows(Box& screenSize)
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

void DXSystem::ShutdownWindows()
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
