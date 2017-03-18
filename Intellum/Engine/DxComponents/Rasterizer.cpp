#include "Rasterizer.h"
#include "../../ErrorHandling/Exception.h"

Rasterizer::Rasterizer(ID3D11Device* device, ID3D11DeviceContext* deviceContext): _device(device), _deviceContext(deviceContext), _rasterizerState(map<D3D11_FILL_MODE, ID3D11RasterizerState*>())
{
	Initialise();
}

Rasterizer::~Rasterizer()
{
}

void Rasterizer::Initialise()
{
	CreateRasterizerState(D3D11_FILL_SOLID);
	CreateRasterizerState(D3D11_FILL_WIREFRAME);

	SetRasterizerState(D3D11_FILL_SOLID);
}

void Rasterizer::Shutdown()
{
	map<D3D11_FILL_MODE, ID3D11RasterizerState*>::iterator iterator;
	for (iterator = _rasterizerState.begin(); iterator != _rasterizerState.end(); ++iterator)
	{
		delete iterator->second;
		iterator->second = nullptr;
	}
}

void Rasterizer::SetRasterizerState(D3D11_FILL_MODE rasterizerState)
{
	_deviceContext->RSSetState(_rasterizerState[rasterizerState]);
	_currentState = rasterizerState;
}

void Rasterizer::ToggleRasterizerState()
{
	switch (_currentState)
	{
	case D3D11_FILL_SOLID:
		SetRasterizerState(D3D11_FILL_WIREFRAME);
		break;
	case D3D11_FILL_WIREFRAME:
		SetRasterizerState(D3D11_FILL_SOLID);
	default:
		SetRasterizerState(D3D11_FILL_SOLID);
		break;
	}
}

void Rasterizer::CreateRasterizerState(D3D11_FILL_MODE fillMode)
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

	ID3D11RasterizerState* rasterizerState;
	HRESULT result = _device->CreateRasterizerState(&rasterizerDescription, &rasterizerState);
	if (FAILED(result)) throw Exception("Failed to create the rasterizer state");

	_rasterizerState.insert(pair<D3D11_FILL_MODE, ID3D11RasterizerState*>(fillMode, rasterizerState));
}