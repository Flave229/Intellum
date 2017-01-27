#include "KeyboardMappings.h"

KeyboardMappings::KeyboardMappings()
{
	_keyboardMappings = map<Controls, unsigned char> 
	{
		{ ESCAPE, DIK_ESCAPE },
		{ CAMERA_MOVE_LEFT, DIK_A },
		{ CAMERA_MOVE_RIGHT, DIK_D },
		{ CAMERA_MOVE_UP, DIK_W },
		{ CAMERA_MOVE_DOWN, DIK_S },
	};
}

KeyboardMappings::~KeyboardMappings()
{
}

unsigned char KeyboardMappings::GetControlMappingFor(Controls control)
{
	return _keyboardMappings[control];
}
