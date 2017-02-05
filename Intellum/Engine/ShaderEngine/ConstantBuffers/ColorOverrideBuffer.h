#pragma once
#include <DirectXMath.h>
#include "IShaderBuffer.h"

using namespace DirectX;

class ColorOverrideBuffer : public IShaderBuffer
{
private:
	struct Buffer
	{
		float colorOverloadEnabled;
		XMFLOAT3 padding;
		XMFLOAT4 colorOverload;
	};

	ID3D11Buffer* _buffer;
private:
	void Initialise() override;

public:
	ColorOverrideBuffer(DirectX3D* direct3D);
	~ColorOverrideBuffer();

	void Shutdown() override;

	void SetShaderParameters(int bufferIndex, XMMATRIX worldMatrix = XMMATRIX(), XMMATRIX projectionMatrix = XMMATRIX(), XMMATRIX viewMatrix = XMMATRIX(), bool colorEnabled = false, XMFLOAT4 colorOverload = XMFLOAT4()) override;
};