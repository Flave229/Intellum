#pragma once
#include "IComponent.h"
#include <map>
#include "../../Input/Controls.h"
#include "../Commands/ICommand.h"

using namespace std;

class InputComponent : public IComponent
{
public:
	map<Controls, ICommand*> ControlCommands;

	InputComponent() : IComponent(INPUT_COMPONENT) {}
	~InputComponent() override = default;

	void Shutdown() override {}
};