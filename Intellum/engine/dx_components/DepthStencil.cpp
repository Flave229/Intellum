#include "DepthStencil.h"
#include "../../error_handling/Exception.h"
#include <map>

DepthStencil::DepthStencil(ID3D11Device* device, ID3D11DeviceContext* deviceContext): _device(device), _deviceContext(deviceContext), _depthStencilBuffer(nullptr), _depthStencilState(nullptr), _depthDisabledStencilState(nullptr)
{
}

DepthStencil::DepthStencil(const DepthStencil& other): _device(other._device), _deviceContext(other._deviceContext), _depthStencilBuffer(other._depthStencilBuffer), _depthStencilState(other._depthStencilState), _depthDisabledStencilState(other._depthDisabledStencilState)
{
}

DepthStencil::~DepthStencil()
{
}

bool DepthStencil::Initialise(int screenWidth, int screenHeight)
{
	HRESULT result;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;

	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	result = _device->CreateTexture2D(&depthBufferDesc, nullptr, &_depthStencilBuffer);
	if (FAILED(result)) return false;

	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	result = _device->CreateDepthStencilState(&depthStencilDesc, &_depthStencilState);
	if (FAILED(result)) return false;

	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	result = _device->CreateDepthStencilState(&depthDisabledStencilDesc, &_depthDisabledStencilState);
	if (FAILED(result)) return false;

	_deviceContext->OMSetDepthStencilState(_depthStencilState, 1);

	return true;
}

void DepthStencil::Shutdown()
{
	if (_depthStencilBuffer)
	{
		_depthStencilBuffer->Release();
		_depthStencilBuffer = nullptr;
	}

	if (_depthStencilState)
	{
		_depthStencilState->Release();
		_depthStencilState = nullptr;
	}

	if (_depthDisabledStencilState)
	{
		_depthDisabledStencilState->Release();
		_depthDisabledStencilState = nullptr;
	}
}

void DepthStencil::SetStencilType(DepthStencilType stencilType) const
{
	try
	{
		if (!_deviceContext)
			throw Exception("The Depth Stencil has no reference to the device context.");

		switch (stencilType)
		{
		case STENCIL_STATE_DEFAULT:
			_deviceContext->OMSetDepthStencilState(_depthStencilState, 1);
			break;
		case STENCIL_STATE_DISABLED:
			_deviceContext->OMSetDepthStencilState(_depthDisabledStencilState, 1);
			break;
		default:
			_deviceContext->OMSetDepthStencilState(_depthStencilState, 1);
			break;
		}
	}
	catch(Exception& exception)
	{
		throw Exception("Failed to set the Stencil Type.", exception);
	}
	catch(exception& exception)
	{
		throw Exception(string("An error occured when setting the Stencil Type. Exception returned the following message: ").append(exception.what()));
	}
	catch(...)
	{
		throw Exception(string("An error occured when setting the Stencil Type."));
	}
}

ID3D11Texture2D* DepthStencil::GetDepthStencilBuffer() const
{
	return _depthStencilBuffer;
}
