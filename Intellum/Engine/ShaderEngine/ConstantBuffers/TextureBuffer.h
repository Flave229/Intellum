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
		XMFLOAT3 padding;
	};

	ID3D11Buffer* _buffer;
private:
	void Initialise() override;

public:
	TextureBuffer(DirectX3D* direct3D);
	~TextureBuffer();

	void Shutdown() override;

	void SetShaderParameters(ShaderParameters parameters) override;
};