#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class ColorShaderParameters
{
public:
	XMFLOAT4 Color;
	bool Enabled;

	ColorShaderParameters() : Color(XMFLOAT4(0, 0, 0, 0)), Enabled(false) {}
	ColorShaderParameters(XMFLOAT4 color) : Color(color), Enabled(true) {}
	~ColorShaderParameters() {}
};
