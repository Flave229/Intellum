#pragma once
#include "../Objects/Commands/ICommand.h"
#include "Controls.h"

class ControlCommand
{
public:
	Controls Control;
	ICommand* Command;
	float Cooldown;
	float CurrentCooldown;
};