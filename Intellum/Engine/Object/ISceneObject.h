#pragma once
#include "Transform.h"

class ISceneObject
{
public:
	ISceneObject() {}
	virtual ~ISceneObject() {}
	virtual void Shutdown() = 0;

	virtual void Update(float delta) = 0;
	virtual void Render() = 0;

	virtual Transform* GetTransform() = 0;
};