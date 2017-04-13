#include "ShaderParameterConstructor.h"
#include "../../Objects/Appearance/ColorShaderParameters.h"

ShaderParameterConstructor::ShaderParameterConstructor()
{
}

ShaderParameterConstructor::~ShaderParameterConstructor()
{
}

ShaderParameters ShaderParameterConstructor::ConstructDefaultBufferParameters(int bufferIndex)
{
	ShaderParameters shaderParameters;
	shaderParameters.BufferIndex = bufferIndex;

	return shaderParameters;
}

ShaderParameters ShaderParameterConstructor::ConstructMatrixBufferParameters(int bufferIndex, XMMATRIX worldMatrix, XMMATRIX projectionMatrix, XMMATRIX viewMatrix)
{
	ShaderParameters shaderParameters;
	shaderParameters.BufferIndex = bufferIndex;
	shaderParameters.WorldMatrix = worldMatrix;
	shaderParameters.ProjectionMatrix = projectionMatrix;
	shaderParameters.ViewMatrix = viewMatrix;

	return shaderParameters;
}

ShaderParameters ShaderParameterConstructor::ConstructColorOverloadBufferParameters(int bufferIndex, ColorShaderParameters colorOverload)
{
	ShaderParameters shaderParameters;
	shaderParameters.BufferIndex = bufferIndex;
	shaderParameters.ColorOverload = colorOverload.Color;
	shaderParameters.ColorOverloadEnabled = colorOverload.Enabled;

	return shaderParameters;
}

ShaderParameters ShaderParameterConstructor::ConstructTextureBufferParameters(int bufferIndex, int textureCount, bool lightMapEnabled, bool bumpMapEnabled)
{
	ShaderParameters shaderParameters;
	shaderParameters.BufferIndex = bufferIndex;
	shaderParameters.TextureCount = textureCount;
	shaderParameters.LightMapEnabled = lightMapEnabled;
	shaderParameters.BufferMapEnabled = bumpMapEnabled;

	return shaderParameters;
}

ShaderParameters ShaderParameterConstructor::ConstructGradientOverloadBufferParameters(int bufferIndex, GradientShaderParameters gradientOverload)
{
	ShaderParameters shaderParameters;
	shaderParameters.BufferIndex = bufferIndex;
	shaderParameters.GradientApexOverload = gradientOverload.ApexColor;
	shaderParameters.GradientCenterOverload = gradientOverload.CenterColor;
	shaderParameters.GradientOverloadEnabled = gradientOverload.Enabled;
	shaderParameters.GradientHeight = gradientOverload.Height;
	shaderParameters.CenterPosition = gradientOverload.CenterYCordinates;

	return shaderParameters;
}