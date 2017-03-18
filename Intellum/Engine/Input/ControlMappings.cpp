#include "ControlMappings.h"

ControlMappings::ControlMappings()
{
	_keyboardMappings = map<Controls, InputControl>
	{
		{ ESCAPE, InputControl(KEYBOARD_CONTROL, DIK_ESCAPE) },
		{ CAMERA_MOVE_LEFT, InputControl(KEYBOARD_CONTROL, DIK_A) },
		{ CAMERA_MOVE_RIGHT, InputControl(KEYBOARD_CONTROL, DIK_D) },
		{ CAMERA_MOVE_FORWARD, InputControl(KEYBOARD_CONTROL, DIK_W) },
		{ CAMERA_MOVE_BACKWARD, InputControl(KEYBOARD_CONTROL, DIK_S) },
		{ CAMERA_LOOK_LEFT, InputControl(MOUSE_CONTROL, MOUSE_LEFT) },
		{ CAMERA_LOOK_RIGHT, InputControl(MOUSE_CONTROL, MOUSE_RIGHT) },
		{ CAMERA_LOOK_UP, InputControl(MOUSE_CONTROL, MOUSE_UP) },
		{ CAMERA_LOOK_DOWN, InputControl(MOUSE_CONTROL, MOUSE_DOWN) },
		{ TOGGLE_RASTERIZER_STATE, InputControl(KEYBOARD_CONTROL, DIK_F8) },
	};
}

ControlMappings::~ControlMappings()
{
}

InputControl ControlMappings::GetControlMappingFor(Controls control)
{
	return _keyboardMappings[control];
}
