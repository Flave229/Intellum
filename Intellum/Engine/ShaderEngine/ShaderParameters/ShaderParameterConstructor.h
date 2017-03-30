#pragma once
#include "ShaderParameters.h"

class ColorOverload;

class ShaderParameterConstructor
{
public:
	ShaderParameterConstructor();
	~ShaderParameterConstructor();

	static ShaderParameters ConstructDefaultBufferParameters(int bufferIndex);
	static ShaderParameters ConstructMatrixBufferParameters(int bufferIndex, XMMATRIX worldMatrix, XMMATRIX projectionMatrix, XMMATRIX viewMatrix);
	static ShaderParameters ConstructColorOverloadBufferParameters(int bufferIndex, ColorOverload colorOverload);
	static ShaderParameters ConstructTextureBufferParameters(int bufferIndex, int textureCount, bool lightMapEnabled, bool bumpMapEnabled);
};
