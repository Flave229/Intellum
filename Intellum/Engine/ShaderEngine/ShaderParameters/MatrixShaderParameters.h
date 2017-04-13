#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class MatrixShaderParameters
{
public:
	XMMATRIX WorldMatrix;
	XMMATRIX ProjectionMatrix;
	XMMATRIX ViewMatrix;

	MatrixShaderParameters() : WorldMatrix(XMMATRIX()), ProjectionMatrix(XMMATRIX()), ViewMatrix(XMMATRIX()) {}
	~MatrixShaderParameters() {}
};