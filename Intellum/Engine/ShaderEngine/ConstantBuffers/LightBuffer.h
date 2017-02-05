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

	void SetShaderParameters(int bufferIndex, XMMATRIX worldMatrix = XMMATRIX(), XMMATRIX projectionMatrix = XMMATRIX(), XMMATRIX viewMatrix = XMMATRIX(), bool colorEnabled = false, XMFLOAT4 colorOverload = XMFLOAT4()) override;
};