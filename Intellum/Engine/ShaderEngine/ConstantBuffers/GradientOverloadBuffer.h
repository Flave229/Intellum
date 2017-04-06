#pragma once
#include "IShaderBuffer.h"

class GradientOverloadBuffer : public IShaderBuffer
{
private:
	struct Buffer
	{
		XMFLOAT4 apexColor;
		XMFLOAT4 centerColor;
		XMFLOAT3 apexPosition;
		float gradientOverloadEnabled;
		XMFLOAT3 centerPosition;
		float padding2;
	};

	ID3D11Buffer* _buffer;

	void Initialise() override;

public:
	GradientOverloadBuffer(DirectX3D* direct3D);
	~GradientOverloadBuffer() override = default;
	void Shutdown() override;
	void SetShaderParameters(ShaderParameters parameters) override;
};
