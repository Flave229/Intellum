#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class ColorOverload
{
public:
	XMFLOAT4 Color;
	bool Overload;

	ColorOverload() : Color(XMFLOAT4(0, 0, 0, 0)), Overload(false) {}
	ColorOverload(XMFLOAT4 color) : Color(color), Overload(true) {}
	~ColorOverload() {}
};
