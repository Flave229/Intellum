#pragma once
#include <d3d11.h>
#include "IComponent.h"
#include "../../Observer/IObserver.h"
#include "../../FontEngine/TextTexture.h"

class TextComponent : public IComponent, public IObserver
{
public:
	string PreviousText;
	string Text;
	int FontSize;
	XMFLOAT2 FontPosition;
	XMFLOAT4 Color;
	vector<TextTexture> TextEntity;

	TextComponent()
		: IComponent(TEXT), Text(""), FontSize(0), FontPosition(0, 0), Color(XMFLOAT4(0, 0, 0, 0)) {}

	~TextComponent() override = default;
	void Shutdown() override {}

	void Notify(ObserverEvent observerEvent) override
	{
		if (observerEvent.EventType == MOVED_MOUSE)
		{
			XMFLOAT2 mousePosition = observerEvent.GetObservableData<XMFLOAT2>();
			Text = "Mouse X: " + to_string(static_cast<int>(mousePosition.x)) + "    " + "Mouse Y: " + to_string(static_cast<int>(mousePosition.y));
		}
		else if (observerEvent.EventType == FRAMES_PER_SECOND)
		{
			int framesPerSecond = observerEvent.GetObservableData<int>();
			Text = "FPS: " + to_string(framesPerSecond);
		}
		else if (observerEvent.EventType == CPU_USAGE)
		{
			int cpuUsage = observerEvent.GetObservableData<int>();
			Text = "CPU: " + to_string(cpuUsage) + "%";
		}
		else if (observerEvent.EventType == RENDER_COUNT)
		{
			int renderCount = observerEvent.GetObservableData<int>();
			Text = "Rendered: " + to_string(renderCount);
		}
	}
};
