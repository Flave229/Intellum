#pragma once
#include "IComponent.h"
#include <DirectXMath.h>

using namespace std;
using namespace DirectX;

class TransformComponent : public IComponent, public IObserver
{
public:
	XMMATRIX Transformation;

	XMFLOAT3 Position;
	XMFLOAT3 Rotation;
	XMFLOAT3 Scale;

	XMFLOAT3 Velocity;
	XMFLOAT3 AngularVelocity;

	TransformComponent() 
		: IComponent(TRANSFORM), Position(0, 0, 0), Rotation(0, 0, 0), Scale(1, 1, 1), Velocity(0, 0, 0), AngularVelocity(0, 0, 0) {}

	~TransformComponent() override = default;

	void Shutdown() override {}
	void Notify(ObserverEvent event) override 
	{
		if (event.EventType == MOVED_MOUSE)
		{
			XMFLOAT2 mousePosition = event.GetObservableData<XMFLOAT2>();
			Position = XMFLOAT3(mousePosition.x, mousePosition.y, 0);
		}
	}
};