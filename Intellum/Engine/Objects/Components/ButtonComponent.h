#pragma once
#include "IComponent.h"
#include "../../Observer/IObserver.h"

using namespace DirectX;

class ButtonComponent : public IComponent
{
public:
	ButtonComponent() : IComponent(BUTTON)
	{ }

	~ButtonComponent() override = default;

	void Shutdown() override
	{ }
};