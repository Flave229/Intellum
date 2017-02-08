#pragma once
#include <DirectXMath.h>
#include "IShaderBuffer.h"
#include "../../Graphics/Light.h"

using namespace DirectX;

class LightBuffer : public IShaderBuffer
{
private:
	struct Buffer
	{
		XMFLOAT4 ambientColor;
		XMFLOAT4 diffuseColor;
		XMFLOAT3 lightDirection;
		float specularPower;
		XMFLOAT4 specularColor;
	};

	ID3D11Buffer* _buffer;
	Light* _light;
private:
	void Initialise() override;

public:
	LightBuffer(DirectX3D* direct3D, Light* light);
	~LightBuffer();
	
	void Shutdown() override;

	void SetShaderParameters(ShaderParameters parameters) override;
};