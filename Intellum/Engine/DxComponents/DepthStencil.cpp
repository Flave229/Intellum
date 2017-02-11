#include "DepthStencil.h"
#include "../../ErrorHandling/Exception.h"
#include <map>

DepthStencil::DepthStencil(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Box screenSize): _device(device), _deviceContext(deviceContext), _depthStencilBuffer(nullptr), _depthStencilState(nullptr), _depthDisabledStencilState(nullptr)
{
	Initialise(screenSize);
}

DepthStencil::DepthStencil(const DepthStencil& other): _device(other._device), _deviceContext(other._deviceContext), _depthStencilBuffer(other._depthStencilBuffer), _depthStencilState(other._depthStencilState), _depthDisabledStencilState(other._depthDisabledStencilState)
{
}

DepthStencil::~DepthStencil()
{
}

void DepthStencil::Initialise(Box screenSize)
{
	D3D11_TEXTURE2D_DESC depthBufferDesc;

	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = screenSize.Width;
	depthBufferDesc.Height = screenSize.Height;
	depthBufferDesc.MipLevels = static_cast<UINT>(1);
	depthBufferDesc.ArraySize = static_cast<UINT>(1);
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = static_cast<UINT>(1);
	depthBufferDesc.SampleDesc.Quality = static_cast<UINT>(0);
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = static_cast<UINT>(0);
	depthBufferDesc.MiscFlags = static_cast<UINT>(0);

	HRESULT result = _device->CreateTexture2D(&depthBufferDesc, nullptr, &_depthStencilBuffer);
	if (FAILED(result)) throw Exception("Failed to create the Depth Stencil Buffer");

	CreateDepthStencil(_depthStencilState, true);
	CreateDepthStencil(_depthDisabledStencilState, false);

	_deviceContext->OMSetDepthStencilState(_depthStencilState, 1);
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

void DepthStencil::CreateDepthStencil(ID3D11DepthStencilState* depthStencil, bool depthEnable)
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDescription;

	ZeroMemory(&depthStencilDescription, sizeof(depthStencilDescription));
	
	depthStencilDescription.DepthEnable = depthEnable;
	depthStencilDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDescription.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDescription.StencilEnable = true;
	depthStencilDescription.StencilReadMask = 0xFF;
	depthStencilDescription.StencilWriteMask = 0xFF;
	depthStencilDescription.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDescription.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDescription.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDescription.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDescription.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDescription.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDescription.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDescription.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	HRESULT result = _device->CreateDepthStencilState(&depthStencilDescription, &depthStencil);
	if (FAILED(result)) throw Exception("Failed to create the Depth Stencil State");
}