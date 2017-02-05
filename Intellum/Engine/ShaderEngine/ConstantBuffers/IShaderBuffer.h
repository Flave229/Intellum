#pragma once
#include "../../DirectX3D.h"

class IShaderBuffer
{
protected:
	DirectX3D* _direct3D;

public:
	IShaderBuffer(DirectX3D* direct3D) : _direct3D(direct3D) {}
	virtual ~IShaderBuffer() {}

	virtual void Initialise() = 0;
	virtual void Shutdown() = 0;

	virtual void SetShaderParameters(int bufferIndex, XMMATRIX worldMatrix = XMMATRIX(), XMMATRIX projectionMatrix = XMMATRIX(), XMMATRIX viewMatrix = XMMATRIX(), bool colorEnabled = false, XMFLOAT4 colorOverload = XMFLOAT4()) = 0;
};

