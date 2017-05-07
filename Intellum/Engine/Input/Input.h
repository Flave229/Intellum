#ifndef _INPUT_H_
#define _INPUT_H_
#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>
#include <DirectXMath.h>
#include "../../ErrorHandling/Exception.h"
#include "../../common/Box.h"
#include "ControlMappings.h"
#include "../Observer/IObserver.h"
#include "../Observer/Observable.h"

using namespace DirectX;

class Input : public Observable
{
private:
	IDirectInput8* _directInput;
	IDirectInputDevice8* _keyboard;
	IDirectInputDevice8* _mouse;

	unsigned char _keyboardState[256];
	DIMOUSESTATE _mouseState;

	Box _screen;
	XMFLOAT2 _previousMousePosition;
	
	ControlMappings _keyboardMappings;
private:
	void Initialise(HINSTANCE hInstance, HWND hwnd, Box screenSize);

	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

public:
	XMFLOAT2 MousePosition;

	Input(HINSTANCE hInstance, HWND hwnd, Box screenSize);
	Input(const Input& other);
	~Input();
	void Shutdown();
	void Update();

	bool IsControlPressed(Controls control);
	bool ProcessMouseControl(InputControl input);

	void AddObserver(IObserver* observer) override;
};

#endif