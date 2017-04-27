#pragma once
#include <DirectXMath.h>
#include "../Objects/Texture/Texture.h"
#include "../Objects/Geometry/Geometry.h"

using namespace DirectX;

class TextTexture
{
public:
	Texture* CharacterTexture;
	Geometry Model;
	XMFLOAT2 Position;
	float Size;
	XMFLOAT2 PreviousPosition;
	float PreviousSize;
	ColorShaderParameters Color;

	TextTexture(Texture* texture): CharacterTexture(texture), Model(Geometry()), Position(XMFLOAT2()), Size(0)
	{}

	~TextTexture() {}
};