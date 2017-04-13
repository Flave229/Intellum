#pragma once
#include "d3d11.h"
#include <DirectXMath.h>
#include <vector>

using namespace DirectX;

class TextureShaderParameters
{
public:
	std::vector<ID3D11ShaderResourceView*> TextureArray;
	ID3D11ShaderResourceView* LightMap;
	ID3D11ShaderResourceView* BumpMap;
	bool LightMapEnabled;
	bool BumpMapEnabled;

	TextureShaderParameters() : TextureArray(std::vector<ID3D11ShaderResourceView*>()), LightMap(nullptr), BumpMap(nullptr), LightMapEnabled(false), BumpMapEnabled(false) {}
	~TextureShaderParameters() {}
};
