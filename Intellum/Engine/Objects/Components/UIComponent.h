#pragma once
#include "IComponent.h"

using namespace DirectX;

class UIComponent : public IComponent
{
public:
	XMFLOAT2 BitmapSize;
	XMFLOAT2 PreviousBitmapSize;

	XMFLOAT2 Position;
	XMFLOAT2 PreviousPosition;

	UIComponent() : IComponent(USER_INTERFACE) { }

	~UIComponent() override = default;

	void Shutdown() override { }
};