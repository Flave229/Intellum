#include "KeyboardMappings.h"

KeyboardMappings::KeyboardMappings()
{
	_keyboardMappings = map<Controls, unsigned char> 
	{
		{ ESCAPE, DIK_ESCAPE },
		{ CAMERA_LOOK_LEFT, DIK_A },
		{ CAMERA_LOOK_RIGHT, DIK_D },
		{ CAMERA_LOOK_UP, DIK_W },
		{ CAMERA_LOOK_DOWN, DIK_S },
	};
}

KeyboardMappings::~KeyboardMappings()
{
}

unsigned char KeyboardMappings::GetControlMappingFor(Controls control)
{
	return _keyboardMappings[control];
}
