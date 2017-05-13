#pragma once
#include "IShaderBuffer.h"

class GradientOverloadBuffer : public IShaderBuffer
{
private:
	struct Buffer
	{
		XMFLOAT4 apexColor;
		XMFLOAT4 centerColor;
		float apexPosition;
		float gradientOverloadEnabled;
		float centerPosition;
		float padding;
	};

	ID3D11Buffer* _buffer;

	void Initialise() override;

public:
	GradientOverloadBuffer(DirectX3D* direct3D);
	~GradientOverloadBuffer() override = default;
	void Shutdown() override;

	void SetShaderParameters(int bufferIndex, ShaderResources shaderResources) override;
};
