#pragma once
#include "ShaderParameters.h"

class ShaderParameterConstructor
{
public:
	ShaderParameterConstructor();
	~ShaderParameterConstructor();

	static ShaderParameters ConstructDefaultBufferParameters(int bufferIndex);
	static ShaderParameters ConstructMatrixBufferParameters(int bufferIndex, XMMATRIX worldMatrix, XMMATRIX projectionMatrix, XMMATRIX viewMatrix);
	static ShaderParameters ConstructColorOverloadBufferParameters(int bufferIndex, XMFLOAT4 colorOverload, bool colorOverloadEnabled);
	static ShaderParameters ConstructTextureBufferParameters(int bufferIndex, int textureCount);
};
