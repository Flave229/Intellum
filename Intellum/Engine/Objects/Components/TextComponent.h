#pragma once
#include <d3d11.h>
#include "IComponent.h"

class TextComponent : public IComponent
{
public:
	string Text;
	int FontSize;
	XMFLOAT2 FontPosition;
	XMFLOAT4 Color;

	TextComponent()
		: IComponent(TEXT), Text(""), FontSize(0), FontPosition(0, 0), Color(XMFLOAT4(0, 0, 0, 0)) {}

	~TextComponent() override = default;

	void Shutdown() override {}
};