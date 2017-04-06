#pragma once
#include <DirectXMath.h>
#include "IShaderBuffer.h"

class ColorOverloadBufferParameters;
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
	~ColorOverrideBuffer() override = default;

	void Shutdown() override;

	void SetShaderParameters(ShaderParameters parameters) override;
};