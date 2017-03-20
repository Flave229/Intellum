#pragma once

#include <d3d11.h>
#include <map>
#include "../../ErrorHandling/Exception.h"

class Rasterizer
{
private:
	ID3D11Device* _device;
	ID3D11DeviceContext* _deviceContext;

	map<D3D11_FILL_MODE, map<D3D11_CULL_MODE, ID3D11RasterizerState*>> _rasterizerState;
	D3D11_FILL_MODE _currentFillMode;
	D3D11_CULL_MODE _currentCullMode;

private:
	void Initialise();
	void CreateRasterizerState(D3D11_FILL_MODE fillMode, D3D11_CULL_MODE cullMode);
public:
	Rasterizer(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~Rasterizer();
		
	void Shutdown();

	void SetRasterizerState(D3D11_FILL_MODE fillMode, D3D11_CULL_MODE cullMode);
	void ToggleFillMode();
};