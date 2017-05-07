#pragma once
#include "IComponent.h"
#include "../../Observer/IObserver.h"
#include "../Commands/ICommand.h"

using namespace DirectX;

class ButtonComponent : public IComponent
{
public:
	ICommand* OnClickCommand;

	ButtonComponent() : IComponent(BUTTON), OnClickCommand(nullptr)
	{
	}

	~ButtonComponent() override = default;

	void Shutdown() override
	{ }
};