#include "DirectX3D.h"
#include "../error_handling/Exception.h"

DirectX3D::DirectX3D(Box screenSize, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear)
	: _vsyncEnabled(false), _swapChain(nullptr), _device(nullptr), _deviceContext(nullptr), _renderTargetView(nullptr), _depthStencilView(nullptr), _hardware(nullptr), _depthStencil(nullptr), _rasterizer(nullptr)
{
	Initialise(screenSize, vsync, hwnd, fullscreen, screenDepth, screenNear);
}

DirectX3D::DirectX3D(const DirectX3D& other)  
	: _vsyncEnabled(other._vsyncEnabled), _swapChain(other._swapChain), _device(other._device), _deviceContext(other._deviceContext), _renderTargetView(other._renderTargetView), _depthStencilView(other._depthStencilView), _hardware(nullptr), _depthStencil(other._depthStencil), _rasterizer(other._rasterizer)
{
}

DirectX3D::~DirectX3D()
{
}

void DirectX3D::Initialise(Box screenSize, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear)
{
	try
	{
		HRESULT result;
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ID3D11Texture2D* backBufferPtr;
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		D3D11_VIEWPORT viewPort;

		_vsyncEnabled = vsync;

		_hardware = new HardwareDescription(screenSize.Width, screenSize.Height);

		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Width = screenSize.Width;
		swapChainDesc.BufferDesc.Height = screenSize.Height;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		if (_vsyncEnabled)
		{
			swapChainDesc.BufferDesc.RefreshRate.Numerator = _hardware->GetRefreshRateNumerator();
			swapChainDesc.BufferDesc.RefreshRate.Denominator = _hardware->GetRefreshRateDenominator();
		}
		else
		{
			swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		}

		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = hwnd;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.Windowed = !fullscreen;
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.Flags = 0;

		D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_1;

		result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &_swapChain, &_device, nullptr, &_deviceContext);
		if (FAILED(result)) throw Exception("Failed to create the device and swap chain.");

		result = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&backBufferPtr));
		if (FAILED(result)) throw Exception("Failed to get the buffer from the swap chain.");

		result = _device->CreateRenderTargetView(backBufferPtr, nullptr, &_renderTargetView);
		if (FAILED(result)) throw Exception("Failed to create the render target.");

		backBufferPtr->Release();
		backBufferPtr = nullptr;

		_depthStencil = new DepthStencil(_device, _deviceContext, screenSize);

		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		result = _device->CreateDepthStencilView(_depthStencil->GetDepthStencilBuffer(), &depthStencilViewDesc, &_depthStencilView);
		if (FAILED(result)) throw Exception("Failed to create the Depth Stencil View.");

		_rasterizer = new Rasterizer(_device, _deviceContext);

		_deviceContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);

		viewPort.Width = static_cast<float>(screenSize.Width);
		viewPort.Height = static_cast<float>(screenSize.Height);
		viewPort.MinDepth = 0.0f;
		viewPort.MaxDepth = 1.0f;
		viewPort.TopLeftX = 0.0f;
		viewPort.TopLeftY = 0.0f;

		_deviceContext->RSSetViewports(1, &viewPort);

		float fieldOfView = 3.141592654f / 4.0f;
		float screenAspect = static_cast<float>(screenSize.Width) / static_cast<float>(screenSize.Height);

		_projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);
		_worldMatrix = XMMatrixIdentity();
		_orthoMatrix = XMMatrixOrthographicLH(static_cast<float>(screenSize.Width), static_cast<float>(screenSize.Height), screenNear, screenDepth);
	}
	catch(Exception exception)
	{
		throw Exception("Failed to Initialise DirectX.", exception);
	}
	catch(...)
	{
		throw Exception("Failed to Initialise DirectX.");
	}
}

void DirectX3D::Shutdown()
{
	// Must set to windowed mode otherwise application will throw exception
	if(_swapChain)
	{
		_swapChain->SetFullscreenState(false, nullptr);
	}

	if (_depthStencilView)
	{
		_depthStencilView->Release();
		_depthStencilView = nullptr;
	}

	if (_depthStencil)
	{
		_depthStencil->Shutdown();
		_depthStencil = nullptr;
	}

	if (_renderTargetView)
	{
		_renderTargetView->Release();
		_renderTargetView = nullptr;
	}

	if (_deviceContext)
	{
		_deviceContext->Release();
		_deviceContext = nullptr;
	}

	if (_device)
	{
		_device->Release();
		_device = nullptr;
	}

	if (_swapChain)
	{
		_swapChain->Release();
		_swapChain = nullptr;
	}

	if (_hardware)
	{
		_hardware = nullptr;
	}
}


void DirectX3D::BeginScene(XMFLOAT4 color)
{
	float colorArray[4];

	colorArray[0] = color.x;
	colorArray[1] = color.y;
	colorArray[2] = color.z;
	colorArray[3] = color.w;

	_deviceContext->ClearRenderTargetView(_renderTargetView, colorArray);
	_deviceContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void DirectX3D::EndScene()
{
	_swapChain->Present(_vsyncEnabled ? 1 : 0, 0);
}

ID3D11Device* DirectX3D::GetDevice()
{
	return _device;
}

ID3D11DeviceContext* DirectX3D::GetDeviceContext()
{
	return _deviceContext;
}

void DirectX3D::MapProjectionMatrixInto(XMMATRIX& projectionMatrix)
{
	projectionMatrix = _projectionMatrix;
}

void DirectX3D::MapWorldMatrixInto(XMMATRIX& worldMatrix)
{
	worldMatrix = _worldMatrix;
}

void DirectX3D::MapOrthoMatrixInto(XMMATRIX& orthoMatrix)
{
	orthoMatrix = _orthoMatrix;
}

void DirectX3D::GetVideoCardInfo(char* cardname, int& memory)
{
	_hardware->GetVideoCardDescription(cardname);
	_hardware->MapVideoCardMemoryInto(memory);
}

void DirectX3D::TurnZBufferOn()
{
	try
	{
		_depthStencil->SetStencilType(STENCIL_STATE_DEFAULT);
	}
	catch(Exception& exception)
	{
		throw exception;
	}
}

void DirectX3D::TurnZBufferOff()
{
	try
	{
		_depthStencil->SetStencilType(STENCIL_STATE_DISABLED);
	}
	catch (Exception exception)
	{
		throw exception;
	}
}
