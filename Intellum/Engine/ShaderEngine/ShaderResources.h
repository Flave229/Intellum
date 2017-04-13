#pragma once
#include <vector>
#include <DirectXMath.h>
#include "../Objects/Appearance/ColorShaderParameters.h"
#include "../Objects/Appearance/GradientOverload.h"

using namespace DirectX;

class ShaderResources
{
public:
	std::vector<ID3D11ShaderResourceView*> TextureArray;
	ID3D11ShaderResourceView* LightMap;
	ID3D11ShaderResourceView* BumpMap;
	ColorShaderParameters ColorOverload;
	GradientOverload GradientOverload;
};