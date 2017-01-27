#pragma once
#include <map>
#include <dinput.h>
#include <vector>
#include "Controls.h"

using namespace std;

class KeyboardMappings
{

private:
	vector<Controls> _controls = vector<Controls>();
	map<Controls, unsigned char> _keyboardMappings = map<Controls, unsigned char>();

public:
	KeyboardMappings();
	~KeyboardMappings();

	unsigned char GetControlMappingFor(Controls control);
};