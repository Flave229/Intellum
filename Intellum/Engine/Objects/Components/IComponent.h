#pragma once
#include <string>
#include <DirectXMath.h>

enum ComponentType
{
	APPEARANCE,
	TRANSFORM,
	RASTERIZER
};

class IComponent
{
public:
	ComponentType Name;
	IComponent(ComponentType component) : Name(component) {}
	virtual ~IComponent() {}

	virtual void Shutdown() = 0;
};