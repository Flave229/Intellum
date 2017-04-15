#pragma once
#include <d3d11.h>
#include "IComponent.h"
#include "../../Observer/IObserver.h"

class TextComponent : public IComponent, public IObserver
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

	void Notify(ObserverEvent<XMFLOAT2> observerEvent) override
	{
		if (observerEvent.EventType == MOVED_MOUSE)
		{
			XMFLOAT2* mousePosition = observerEvent.GetObservableData();
			Text = "Victoria Grump: X: " + to_string(mousePosition->x) + " Y: " + to_string(mousePosition->y);
		}
	}
};