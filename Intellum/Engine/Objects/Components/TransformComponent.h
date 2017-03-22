#pragma once
#include "IComponent.h"
#include <DirectXMath.h>

using namespace std;
using namespace DirectX;

class TransformComponent : public IComponent
{
public:
	XMMATRIX Transformation;

	XMFLOAT3 Position;
	XMFLOAT3 Rotation;
	XMFLOAT3 Scale;

	XMFLOAT3 Velocity;
	XMFLOAT3 AngularVelocity;

	TransformComponent();
	TransformComponent(XMFLOAT3 position, XMFLOAT3 rotation, XMFLOAT3 scale);
	~TransformComponent() override = default;
};