#pragma once
#include "IComponent.h"
#include "../../Input/ControlCommand.h"

using namespace std;

class InputComponent : public IComponent
{
public:
	vector<ControlCommand> ControlCommands;

	InputComponent() : IComponent(INPUT_COMPONENT) {}
	~InputComponent() override = default;

	void Shutdown() override {}
};