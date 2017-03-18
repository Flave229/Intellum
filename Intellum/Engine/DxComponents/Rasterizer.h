#pragma once

#include <d3d11.h>
#include <map>

class Rasterizer
{
private:
	ID3D11Device* _device;
	ID3D11DeviceContext* _deviceContext;

	std::map<D3D11_FILL_MODE, ID3D11RasterizerState*> _rasterizerState;
	D3D11_FILL_MODE _currentState;

private:
	void Initialise();
	void CreateRasterizerState(D3D11_FILL_MODE fillMode);
public:
	Rasterizer(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	~Rasterizer();
		
	void Shutdown();

	void SetRasterizerState(D3D11_FILL_MODE);
	void ToggleRasterizerState();
};