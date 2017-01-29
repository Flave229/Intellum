#include "InputControl.h"

InputControl::InputControl(): _controlType(), _input(0)
{
}

InputControl::InputControl(ControlType controlType, unsigned char input) : _controlType(controlType), _input(input)
{
}

InputControl::~InputControl()
{
}

ControlType InputControl::GetControlType() const
{
	return _controlType;
}

unsigned char InputControl::GetInput() const
{
	return _input;
}
