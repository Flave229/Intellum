#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class ShaderParameters
{
public:
	int BufferIndex;

	XMMATRIX WorldMatrix;
	XMMATRIX ProjectionMatrix;
	XMMATRIX ViewMatrix;

	bool ColorOverloadEnabled;
	XMFLOAT4 ColorOverload;

	int TextureCount;

	bool LightMapEnabled;
	bool BufferMapEnabled;
};