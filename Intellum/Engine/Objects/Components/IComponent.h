#pragma once
#include <string>
#include <DirectXMath.h>

class IComponent
{
public:
	std::string Name;
	IComponent(std::string name) : Name(name) {}
	virtual ~IComponent() {}
};