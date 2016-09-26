#pragma once

#include <d3d11.h>

enum RasterizerStateType
{
	RASTERIZER_STATE_DEFAULT,
	RASTERIZER_STATE_WIREFRAME,
};

class Rasterizer
{
// Member Level Variables
private:
	ID3D11RasterizerState* _defaultState;
	ID3D11RasterizerState* _wireframeState;

// Function Declarations
public:
	Rasterizer();
	Rasterizer(const Rasterizer&);
	~Rasterizer();

	bool Initialise(ID3D11Device*, ID3D11DeviceContext*);
	void Shutdown();

	void SetStencilType(ID3D11DeviceContext*, RasterizerStateType) const;
};

