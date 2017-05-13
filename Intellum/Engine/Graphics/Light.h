#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class Light
{
public:
	XMFLOAT4 AmbientColor;
	XMFLOAT4 DiffuseColor;
	XMFLOAT4 SpecularColor;

	XMFLOAT3 Direction;
	float SpecularPower;

	Light();
	~Light();
};