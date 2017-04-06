#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class GradientOverload
{
public:
	XMFLOAT4 ApexColor;
	XMFLOAT4 CenterColor;

	float CenterYCordinates;
	float Height;

	bool Overload;

	GradientOverload() : ApexColor(0, 0, 0, 0), CenterColor(0, 0, 0, 0), CenterYCordinates(0), Height(0), Overload(false) {}
	GradientOverload(XMFLOAT4 apexColor, XMFLOAT4 centerColor, float centerYCordinates, float height) : ApexColor(apexColor), CenterColor(centerColor), CenterYCordinates(centerYCordinates), Height(height), Overload(true) {}
	~GradientOverload() {}
};