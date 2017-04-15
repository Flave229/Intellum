#include "Input.h"

Input::Input(HINSTANCE hInstance, HWND hwnd, Box screenSize) : _directInput(nullptr), _keyboard(nullptr), _mouse(nullptr)
{
	Initialise(hInstance, hwnd, screenSize);
}

Input::Input(const Input& other): _directInput(other._directInput), _keyboard(other._keyboard), _mouse(other._mouse)
{
}

Input::~Input()
{
}

void Input::Initialise(HINSTANCE hInstance, HWND hwnd, Box screenSize)
{
	_screen = screenSize;
	_previousMousePosition = XMFLOAT2(0, 0);
	_mousePosition = XMFLOAT2(0, 0);
	
	HRESULT result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<void**>(&_directInput), nullptr);
	if (FAILED(result)) throw Exception("Failed to create DirectInput.");

	result = _directInput->CreateDevice(GUID_SysKeyboard, &_keyboard, nullptr);
	if (FAILED(result)) throw Exception("Failed to create the keyboard device for DirectInput.");

	result = _keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result)) throw Exception("Failed to set the data format for the keyboard device.");

	result = _keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result)) throw Exception("Failed to set the cooperative level for the keyboard device");

	result = _keyboard->Acquire();
	if (FAILED(result)) throw Exception("Failed to acquire the keyboard device.");

	result = _directInput->CreateDevice(GUID_SysMouse, &_mouse, nullptr);
	if (FAILED(result)) throw Exception("Failed to create the mouse device for DirectInput.");

	result = _mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result)) throw Exception("Failed to set the data format for the mouse device.");

	result = _mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result)) throw Exception("Failed to set the cooperative level for the mouse device");

	result = _mouse->Acquire();
	if (FAILED(result)) throw Exception("Failed to acquire the mouse device.");

	_keyboardMappings = ControlMappings();
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

void Input::Update()
{
	bool result = ReadKeyboard();
	if (!result) throw Exception("Failed to read keyboard input");

	result = ReadMouse();
	if (!result) throw Exception("Failed to read mouse input");;

	ProcessInput();
}

bool Input::IsControlPressed(Controls control)
{
	InputControl input = _keyboardMappings.GetControlMappingFor(control);

	if (input.GetControlType() == KEYBOARD_CONTROL && _keyboardState[input.GetInput()] & 0x80)
		return true;
	
	if (input.GetControlType() == MOUSE_CONTROL)
		return ProcessMouseControl(input);

	return false;
}

bool Input::ProcessMouseControl(InputControl input)
{
	if (input.GetInput() == MOUSE_LEFT && _previousMousePosition.x > _mousePosition.x)
		return true;

	if (input.GetInput() == MOUSE_RIGHT && _previousMousePosition.x < _mousePosition.x)
		return true;

	if (input.GetInput() == MOUSE_UP && _previousMousePosition.y < _mousePosition.y)
		return true;

	if (input.GetInput() == MOUSE_DOWN && _previousMousePosition.y > _mousePosition.y)
		return true;

	return false;
}

void Input::MapMouseLocationInto(XMFLOAT2& point) const
{
	point = _mousePosition;
}

bool Input::ReadKeyboard()
{
	HRESULT result = _keyboard->GetDeviceState(sizeof _keyboardState, static_cast<LPVOID>(&_keyboardState));
	
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
	_previousMousePosition.x = _mousePosition.x;
	_previousMousePosition.y = _mousePosition.y;

	_mousePosition.x += _mouseState.lX;
	_mousePosition.y += _mouseState.lY;

	if (_previousMousePosition.x != _mousePosition.x || _previousMousePosition.y != _mousePosition.y)
	{
		for (int i = 0; i < Observers.size(); i++)
		{
			ObserverEvent<XMFLOAT2> observerEvent = ObserverEvent<XMFLOAT2>();
			observerEvent.EventType = MOVED_MOUSE;
			observerEvent.SetObservableData(new XMFLOAT2(_mousePosition.x, _mousePosition.y));
			Observers.at(i)->Notify(observerEvent);
		}
	}
}

void Input::AddObserver(IObserver* observer)
{
	Observers.push_back(observer);
}