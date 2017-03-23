#include "Rasterizer.h"

Rasterizer::Rasterizer(ID3D11Device* device, ID3D11DeviceContext* deviceContext): _device(device), _deviceContext(deviceContext), _rasterizerState(map<D3D11_FILL_MODE, map<D3D11_CULL_MODE, ID3D11RasterizerState*>>())
{
	Initialise();
}

Rasterizer::~Rasterizer()
{
}

void Rasterizer::Initialise()
{
	CreateRasterizerState(D3D11_FILL_SOLID, D3D11_CULL_BACK);
	CreateRasterizerState(D3D11_FILL_WIREFRAME, D3D11_CULL_BACK);
	CreateRasterizerState(D3D11_FILL_SOLID, D3D11_CULL_FRONT);
	CreateRasterizerState(D3D11_FILL_WIREFRAME, D3D11_CULL_FRONT);
	CreateRasterizerState(D3D11_FILL_SOLID, D3D11_CULL_NONE);
	CreateRasterizerState(D3D11_FILL_WIREFRAME, D3D11_CULL_NONE);

	SetRasterizerState(D3D11_FILL_SOLID, D3D11_CULL_BACK);
}

void Rasterizer::Shutdown()
{
	map<D3D11_FILL_MODE, map<D3D11_CULL_MODE, ID3D11RasterizerState*>>::iterator iteratorFillMode;
	map<D3D11_CULL_MODE, ID3D11RasterizerState*>::iterator iteratorCullMode;
	for (iteratorFillMode = _rasterizerState.begin(); iteratorFillMode != _rasterizerState.end(); ++iteratorFillMode)
	{
		map<D3D11_CULL_MODE, ID3D11RasterizerState*> fillModes = _rasterizerState[iteratorFillMode->first];
		for (iteratorCullMode = fillModes.begin(); iteratorCullMode != fillModes.end(); ++iteratorCullMode)
		{
			delete iteratorCullMode->second;
			iteratorCullMode->second = nullptr;
		}
	}
}

void Rasterizer::SetRasterizerState(D3D11_FILL_MODE fillMode, D3D11_CULL_MODE cullMode)
{
	if (_currentFillMode == fillMode && _currentCullMode == cullMode)
		return;

	_deviceContext->RSSetState(_rasterizerState[fillMode][cullMode]);
	_currentFillMode = fillMode;
	_currentCullMode = cullMode;
}

void Rasterizer::SetRasterizerFillMode(D3D11_FILL_MODE fillMode)
{
	SetRasterizerState(fillMode, _currentCullMode);
}

void Rasterizer::SetRasterizerCullMode(D3D11_CULL_MODE cullMode)
{
	SetRasterizerState(_currentFillMode, cullMode);
}

void Rasterizer::ToggleFillMode()
{
	switch (_currentFillMode)
	{
	case D3D11_FILL_SOLID:
		SetRasterizerState(D3D11_FILL_WIREFRAME, _currentCullMode);
		return;
	case D3D11_FILL_WIREFRAME:
		SetRasterizerState(D3D11_FILL_SOLID, _currentCullMode);
		return;
	default:
		SetRasterizerState(D3D11_FILL_SOLID, _currentCullMode);
	}
}

void Rasterizer::CreateRasterizerState(D3D11_FILL_MODE fillMode, D3D11_CULL_MODE cullMode)
{
	D3D11_RASTERIZER_DESC rasterizerDescription;

	rasterizerDescription.AntialiasedLineEnable = false;
	rasterizerDescription.CullMode = cullMode;
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

	_rasterizerState[fillMode][cullMode] = rasterizerState;
}
