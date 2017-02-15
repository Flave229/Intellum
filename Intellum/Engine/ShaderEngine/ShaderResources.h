#pragma once
#include <vector>
#include <DirectXMath.h>

using namespace DirectX;

class ShaderResources
{
public:
	std::vector<ID3D11ShaderResourceView*> textureArray;
	ID3D11ShaderResourceView* lightMap;
	ID3D11ShaderResourceView* bumpMap;
};