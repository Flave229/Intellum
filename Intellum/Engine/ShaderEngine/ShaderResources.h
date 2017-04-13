#pragma once
#include <vector>
#include <DirectXMath.h>
#include "ShaderParameters/GradientShaderParameters.h"
#include "ShaderParameters/ColorShaderParameters.h"

using namespace DirectX;

class ShaderResources
{
public:
	std::vector<ID3D11ShaderResourceView*> TextureArray;
	ID3D11ShaderResourceView* LightMap;
	ID3D11ShaderResourceView* BumpMap;
	ColorShaderParameters ColorOverload;
	GradientShaderParameters GradientOverload;
};