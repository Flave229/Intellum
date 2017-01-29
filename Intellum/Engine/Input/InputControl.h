#pragma once
#include "ControlType.h"

class InputControl
{
private:
	ControlType _controlType;
	unsigned char _input;

public:
	InputControl();
	InputControl(ControlType controlType, unsigned char input);
	~InputControl();

	ControlType GetControlType() const;
	unsigned char GetInput() const;
};