#include "KeyboardMappings.h"

KeyboardMappings::KeyboardMappings()
{
	_keyboardMappings = map<Controls, InputControl>
	{
		{ ESCAPE, InputControl(KEYBOARD_CONTROL, DIK_ESCAPE) },
		{ CAMERA_MOVE_LEFT, InputControl(KEYBOARD_CONTROL, DIK_A) },
		{ CAMERA_MOVE_RIGHT, InputControl(KEYBOARD_CONTROL, DIK_D) },
		{ CAMERA_MOVE_UP, InputControl(KEYBOARD_CONTROL, DIK_W) },
		{ CAMERA_MOVE_DOWN, InputControl(KEYBOARD_CONTROL, DIK_S) },
		{ CAMERA_LOOK_LEFT, InputControl(MOUSE_CONTROL, MOUSE_LEFT) },
		{ CAMERA_LOOK_RIGHT, InputControl(MOUSE_CONTROL, MOUSE_RIGHT) },
		{ CAMERA_LOOK_UP, InputControl(MOUSE_CONTROL, MOUSE_UP) },
		{ CAMERA_LOOK_DOWN, InputControl(MOUSE_CONTROL, MOUSE_DOWN) },
	};
}

KeyboardMappings::~KeyboardMappings()
{
}

InputControl KeyboardMappings::GetControlMappingFor(Controls control)
{
	return _keyboardMappings[control];
}
