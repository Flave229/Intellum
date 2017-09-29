#pragma once
#include "IComponent.h"
#include "../Commands/ICommand.h"
#include "../Commands/NullCommand.h"

using namespace DirectX;

enum CollisionType
{
	CURSOR
};

class CollisionComponent : public IComponent
{
public:
	ICommand* OnCollisionCommand;
	CollisionType CollisionType;

	CollisionComponent() : IComponent(COLLISION), OnCollisionCommand(new NullCommand)
	{
	}

	~CollisionComponent() override = default;

	void Shutdown() override
	{
		if (OnCollisionCommand)
		{
			OnCollisionCommand->Shutdown();
			delete OnCollisionCommand;
			OnCollisionCommand = nullptr;
		}
	}
};