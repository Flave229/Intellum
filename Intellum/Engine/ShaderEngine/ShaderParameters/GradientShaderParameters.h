#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class GradientShaderParameters
{
public:
	XMFLOAT4 ApexColor;
	XMFLOAT4 CenterColor;

	float CenterYCordinates;
	float Height;

	bool Enabled;

	GradientShaderParameters() : ApexColor(0, 0, 0, 0), CenterColor(0, 0, 0, 0), CenterYCordinates(0), Height(0), Enabled(false) {}
	GradientShaderParameters(XMFLOAT4 apexColor, XMFLOAT4 centerColor, float centerYCordinates, float height) : ApexColor(apexColor), CenterColor(centerColor), CenterYCordinates(centerYCordinates), Height(height), Enabled(true) {}
	~GradientShaderParameters() {}
};