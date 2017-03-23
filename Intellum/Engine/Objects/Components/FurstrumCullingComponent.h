#pragma once
#include "IComponent.h"
#include "../../Camera/FrustrumCullingType.h"

class FrustrumCullingComponent : public IComponent
{
public:
	FrustrumCullingType CullingType;

	FrustrumCullingComponent() 
		: IComponent(FRUSTRUM_CULLING), CullingType(FRUSTRUM_CULL_RECTANGLE) {}

	void Shutdown() override {};
};