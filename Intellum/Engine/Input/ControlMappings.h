#pragma once
#include <map>
#include <dinput.h>
#include <vector>
#include "Controls.h"
#include "InputControl.h"

#define MOUSE_LEFT			0x01
#define MOUSE_UP			0x02
#define MOUSE_RIGHT			0x03
#define MOUSE_DOWN			0x04
#define MOUSE_LEFT_CLICK	0x05

using namespace std;

class ControlMappings
{
private:
	vector<Controls> _controls = vector<Controls>();
	map<Controls, InputControl> _keyboardMappings = map<Controls, InputControl>();

public:
	ControlMappings();
	~ControlMappings();

	InputControl GetControlMappingFor(Controls control);
};