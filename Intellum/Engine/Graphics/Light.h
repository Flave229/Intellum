#pragma once
#include <DirectXMath.h>
#include "../Objects/Transform/Transform.h"

using namespace DirectX;

class Light
{
public:
	Transform* Transform;
	XMFLOAT4 AmbientColor;
	XMFLOAT4 DiffuseColor;
	XMFLOAT4 SpecularColor;

	float SpecularPower;

	Light();
	~Light();

	void Shutdown();
	void Update(float delta);

	XMFLOAT3 GetDirection(XMMATRIX objectMatrix);
};