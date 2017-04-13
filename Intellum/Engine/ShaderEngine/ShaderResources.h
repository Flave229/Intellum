#pragma once
#include <DirectXMath.h>
#include "ShaderParameters/GradientShaderParameters.h"
#include "ShaderParameters/ColorShaderParameters.h"
#include "ShaderParameters/TextureShaderParameters.h"
#include "ShaderParameters/MatrixShaderParameters.h"

using namespace DirectX;

class ShaderResources
{
public:
	MatrixShaderParameters MatrixParameters;
	TextureShaderParameters TextureParameters;
	ColorShaderParameters ColorParameters;
	GradientShaderParameters GradientParameters;
};