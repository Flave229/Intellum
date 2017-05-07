#pragma once
#include "IComponent.h"
#include "../Commands/ICommand.h"
#include "../Commands/NullCommand.h"

using namespace DirectX;

class ButtonComponent : public IComponent
{
public:
	ICommand* OnClickCommand;

	ButtonComponent() : IComponent(BUTTON), OnClickCommand(new NullCommand)
	{
	}

	~ButtonComponent() override = default;

	void Shutdown() override
	{ 
		if (OnClickCommand)
		{
			OnClickCommand->Shutdown();
			delete OnClickCommand;
			OnClickCommand = nullptr;
		}
	}
};