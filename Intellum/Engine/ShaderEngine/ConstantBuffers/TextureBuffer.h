#pragma once
#include <DirectXMath.h>
#include "IShaderBuffer.h"

using namespace DirectX;

class TextureBuffer : public IShaderBuffer
{
private:
	struct Buffer
	{
		float texturesIncluded;
		float lightMapEnabled;
		XMFLOAT2 padding;
	};

	ID3D11Buffer* _buffer;
private:
	void Initialise() override;

public:
	TextureBuffer(DirectX3D* direct3D);
	~TextureBuffer() override = default;

	void Shutdown() override;

	void SetShaderParameters(int bufferIndex, ShaderResources shaderResources) override;
};