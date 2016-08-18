#include "Rasterizer.h"



Rasterizer::Rasterizer(): _defaultState(nullptr), _wireframeState(nullptr)
{
}

Rasterizer::Rasterizer(const Rasterizer& other): _defaultState(other._defaultState), _wireframeState(other._wireframeState)
{
}

Rasterizer::~Rasterizer()
{
}

bool Rasterizer::Initialise(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	HRESULT result;
	D3D11_RASTERIZER_DESC defaultDesc;
	D3D11_RASTERIZER_DESC wireframeDesc;

	defaultDesc.AntialiasedLineEnable = false;
	defaultDesc.CullMode = D3D11_CULL_BACK;
	defaultDesc.DepthBias = 0;
	defaultDesc.DepthBiasClamp = 0.0f;
	defaultDesc.DepthClipEnable = true;
	defaultDesc.FillMode = D3D11_FILL_SOLID;
	defaultDesc.FrontCounterClockwise = false;
	defaultDesc.MultisampleEnable = false;
	defaultDesc.ScissorEnable = false;
	defaultDesc.SlopeScaledDepthBias = 0.0f;

	result = device->CreateRasterizerState(&defaultDesc, &_defaultState);
	if (FAILED(result)) return false;

	wireframeDesc.AntialiasedLineEnable = false;
	wireframeDesc.CullMode = D3D11_CULL_BACK;
	wireframeDesc.DepthBias = 0;
	wireframeDesc.DepthBiasClamp = 0.0f;
	wireframeDesc.DepthClipEnable = true;
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.MultisampleEnable = false;
	wireframeDesc.ScissorEnable = false;
	wireframeDesc.SlopeScaledDepthBias = 0.0f;

	result = device->CreateRasterizerState(&wireframeDesc, &_wireframeState);
	if (FAILED(result)) return false;

	deviceContext->RSSetState(_wireframeState);

	return true;
}

void Rasterizer::Shutdown()
{
	if (_defaultState)
	{
		_defaultState->Release();
		_defaultState = nullptr;
	}
	if (_wireframeState)
	{
		_wireframeState->Release();
		_wireframeState = nullptr;
	}
}

void Rasterizer::SetStencilType(ID3D11DeviceContext* deviceContext, RasterizerStateType rasterizerState) const
{
	switch (rasterizerState)
	{
	case RASTERIZER_STATE_DEFAULT:
		deviceContext->RSSetState(_defaultState);
		break;
	case RASTERIZER_STATE_WIREFRAME:
		deviceContext->RSSetState(_wireframeState);
	default:
		deviceContext->RSSetState(_defaultState);
		break;
	}
}
