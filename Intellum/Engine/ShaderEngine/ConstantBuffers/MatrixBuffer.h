#pragma once
#include <DirectXMath.h>
#include "IShaderBuffer.h"

using namespace DirectX;

class MatrixBuffer : public IShaderBuffer
{
private:
	struct ConstantBuffer
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	ID3D11Buffer* _buffer;

private:
	void Initialise() override;

public:
	MatrixBuffer(DirectX3D* direct3D);
	~MatrixBuffer();
	
	void Shutdown() override;

	void SetShaderParameters(int bufferIndex, XMMATRIX worldMatrix = XMMATRIX(), XMMATRIX projectionMatrix = XMMATRIX(), XMMATRIX viewMatrix = XMMATRIX(), bool colorEnabled = false, XMFLOAT4 colorOverload = XMFLOAT4()) override;
};

