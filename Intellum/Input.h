#ifndef _INPUT_H_
#define _INPUT_H_
#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>
#include <DirectXMath.h>
#include "common/Box.h"
#include "ErrorHandling/Exception.h"

using namespace DirectX;

class Input
{
private:
	IDirectInput8* _directInput;
	IDirectInputDevice8* _keyboard;
	IDirectInputDevice8* _mouse;

	unsigned char _keyboardState[256];
	DIMOUSESTATE _mouseState;

	Box _screen;
	XMFLOAT2 _mousePosition;

private:
	void Initialise(HINSTANCE hInstance, HWND hwnd, Box screenSize);

	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

public:
	Input(HINSTANCE hInstance, HWND hwnd, Box screenSize);
	Input(const Input& other);
	~Input();
	void Shutdown();
	void Update();

	bool IsEscapePressed() const;
	void MapMouseLocationInto(XMFLOAT2& point) const;
};

#endif