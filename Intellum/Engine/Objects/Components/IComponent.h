#pragma once
#include <string>
#include <DirectXMath.h>

enum ComponentType
{
	APPEARANCE,
	TRANSFORM,
	RASTERIZER,
	FRUSTRUM_CULLING,
	USER_INTERFACE,
	TEXT,
	BUTTON
};

class IComponent
{
public:
	ComponentType Name;
	IComponent(ComponentType component) : Name(component) {}
	virtual ~IComponent() {}

	virtual void Shutdown() = 0;
};