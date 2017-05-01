#pragma once
#include "../Entity.h"

class ISystem
{
public:
	ISystem() {}
	virtual ~ISystem() {}
	virtual void Shutdown() = 0;

	virtual void Update(vector<Entity*>& entities, float delta) = 0;
	virtual void Render(vector<Entity*>& entities) = 0;
};