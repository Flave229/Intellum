#include "Rasterizer.h"



Rasterizer::Rasterizer(): _rasterState(nullptr) {
}

Rasterizer::Rasterizer(const Rasterizer& other): _rasterState(other._rasterState) {
}

Rasterizer::~Rasterizer()
{
}

bool Rasterizer::Initialise(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	HRESULT result;
	D3D11_RASTERIZER_DESC rasterDesc;

	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	result = device->CreateRasterizerState(&rasterDesc, &_rasterState);
	if (FAILED(result)) return false;

	deviceContext->RSSetState(_rasterState);

	return true;
}

void Rasterizer::Shutdown()
{
	if (_rasterState)
	{
		_rasterState->Release();
		_rasterState = nullptr;
	}
}

void Rasterizer::SetStencilType(ID3D11DeviceContext* deviceContext, RasterizerStateType rasterizerState) const
{
	switch (rasterizerState)
	{
	case RASTERIZER_STATE_DEFAULT:
		deviceContext->RSSetState(_rasterState);
		break;
	default:
		deviceContext->RSSetState(_rasterState);
		break;
	}
}
