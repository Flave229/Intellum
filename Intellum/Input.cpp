#include "Input.h"

Input::Input() : _directInput(nullptr), _keyboard(nullptr), _mouse(nullptr)
{
}

Input::Input(const Input&)
{
}

Input::~Input()
{
}

bool Input::Initialise(HINSTANCE hInstance, HWND hwnd, int screenWidth, int screenHeight)
{
	_screenWidth = screenWidth;
	_screenHeight = screenHeight;
	_mouseX = 0;
	_mouseY = 0;

	HRESULT result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<void**>(&_directInput), nullptr);
	if (FAILED(result)) return false;

	result = _directInput->CreateDevice(GUID_SysKeyboard, &_keyboard, nullptr);
	if (FAILED(result)) return false;

	result = _keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result)) return false;

	result = _keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result)) return false;

	result = _keyboard->Acquire();
	if (FAILED(result)) return false;

	result = _directInput->CreateDevice(GUID_SysMouse, &_mouse, nullptr);
	if (FAILED(result)) return false;

	result = _mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result)) return false;

	result = _mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result)) return false;

	result = _mouse->Acquire();
	if (FAILED(result)) return false;

	return true;
}

void Input::Shutdown()
{
	if (_mouse)
	{
		_mouse->Unacquire();
		_mouse->Release();
		_mouse = nullptr;
	}

	if (_keyboard)
	{
		_keyboard->Unacquire();
		_keyboard->Release();
		_keyboard = nullptr;
	}

	if (_directInput)
	{
		_directInput->Release();
		_directInput = nullptr;
	}
}

bool Input::Frame()
{
	bool result = ReadKeyboard();
	if (!result) return false;

	result = ReadMouse();
	if (!result) return false;

	ProcessInput();

	return true;
}

bool Input::IsEscapePressed()
{
	if (_keyboardState[DIK_ESCAPE] & 0x80)
		return true;

	return false;
}

void Input::MapMouseLocationInto(XMFLOAT2& point)
{
	point = XMFLOAT2(_mouseX, _mouseY);
}

bool Input::ReadKeyboard()
{
	HRESULT result = _keyboard->GetDeviceState(sizeof(_keyboardState), static_cast<LPVOID>(&_keyboardState));
	
	if (FAILED(result))
	{
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
			_keyboard->Acquire();
		else
			return false;
	}

	return true;
}

bool Input::ReadMouse()
{
	HRESULT result = _mouse->GetDeviceState(sizeof(DIMOUSESTATE), static_cast<LPVOID>(&_mouseState));
	if (FAILED(result))
	{
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
			_mouse->Acquire();
		else
			return false;
	}

	return true;
}

void Input::ProcessInput()
{
	_mouseX += _mouseState.lX;
	_mouseY += _mouseState.lY;

	if (_mouseX < 0)
		_mouseX = 0;
	if (_mouseY < 0)
		_mouseY = 0;
	if (_mouseX > _screenWidth)
		_mouseX = _screenWidth;
	if (_mouseY > _screenHeight)
		_mouseY = _screenHeight;
}
