#include "Input.h"

Input::Input()
{
}

Input::Input(const Input&)
{
}

Input::~Input()
{
}

void Input::Initialise()
{
	for (int i = 0; i < 256; i++)
	{
		_keys[i] = false;
	}
}

void Input::KeyDown(unsigned int input)
{
	_keys[input] = true;
}

void Input::KeyUp(unsigned int input)
{
	_keys[input] = false;
}

bool Input::IsKeyDown(unsigned int input)
{
	return _keys[input];
}
