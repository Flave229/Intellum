#include "Rasterizer.h"
#include "../../ErrorHandling/Exception.h"

Rasterizer::Rasterizer(ID3D11Device* device, ID3D11DeviceContext* deviceContext): _device(device), _deviceContext(deviceContext), _defaultState(nullptr), _wireframeState(nullptr)
{
	Initialise();
}

Rasterizer::Rasterizer(const Rasterizer& other): _device(other._device), _deviceContext(other._deviceContext), _defaultState(other._defaultState), _wireframeState(other._wireframeState)
{
}

Rasterizer::~Rasterizer()
{
}

void Rasterizer::Initialise()
{
	HRESULT result;
	D3D11_RASTERIZER_DESC defaultDesc;
	D3D11_RASTERIZER_DESC wireframeDesc;

	CreateRasterizerState(_defaultState, D3D11_FILL_SOLID);
	CreateRasterizerState(_wireframeState, D3D11_FILL_WIREFRAME);

	_deviceContext->RSSetState(_defaultState);
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

void Rasterizer::CreateRasterizerState(ID3D11RasterizerState* rasterizerState, D3D11_FILL_MODE fillMode)
{
	D3D11_RASTERIZER_DESC rasterizerDescription;

	rasterizerDescription.AntialiasedLineEnable = false;
	rasterizerDescription.CullMode = D3D11_CULL_BACK;
	rasterizerDescription.DepthBias = 0;
	rasterizerDescription.DepthBiasClamp = 0.0f;
	rasterizerDescription.DepthClipEnable = true;
	rasterizerDescription.FillMode = fillMode;
	rasterizerDescription.FrontCounterClockwise = false;
	rasterizerDescription.MultisampleEnable = false;
	rasterizerDescription.ScissorEnable = false;
	rasterizerDescription.SlopeScaledDepthBias = 0.0f;

	bool result = _device->CreateRasterizerState(&rasterizerDescription, &rasterizerState);
	if (FAILED(result)) throw Exception("Failed to create the rasterizer state");
}