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
	ID3D11Device* _device;
	ID3D11DeviceContext* _deviceContext;

	ID3D11RasterizerState* _defaultState;
	ID3D11RasterizerState* _wireframeState;

// Function Declarations
private:
	bool CreateRasterizerState(ID3D11RasterizerState* rasterizerState, D3D11_FILL_MODE fillMode);
public:
	Rasterizer(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	Rasterizer(const Rasterizer&);
	~Rasterizer();

	bool Initialise();
	void Shutdown();

	void SetStencilType(ID3D11DeviceContext*, RasterizerStateType) const;
};

