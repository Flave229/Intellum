#pragma once
#include "../../DirectX3D.h"
#include "../ShaderParameters/ShaderParameters.h"
#include "../ShaderResources.h"

class IShaderBuffer
{
protected:
	DirectX3D* _direct3D;

public:
	IShaderBuffer(DirectX3D* direct3D) : _direct3D(direct3D) {}
	virtual ~IShaderBuffer() {}

	virtual void Initialise() = 0;
	virtual void Shutdown() = 0;

	virtual void SetShaderParameters(int bufferIndex, ShaderResources shaderResources) = 0;
};
