#pragma once
#include <DirectXMath.h>
#include "ShaderParameters/GradientShaderParameters.h"
#include "ShaderParameters/ColorShaderParameters.h"
#include "ShaderParameters/TextureShaderParameters.h"

using namespace DirectX;

class ShaderResources
{
public:
	TextureShaderParameters TextureParameters;
	ColorShaderParameters ColorParameters;
	GradientShaderParameters GradientParameters;
};