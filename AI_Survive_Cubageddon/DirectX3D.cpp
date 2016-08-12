#include "DirectX3D.h"

DirectX3D::DirectX3D(): _vsync_enabled(false), _swapChain(nullptr), _device(nullptr), _deviceContext(nullptr),
							_renderTargetView(nullptr), _depthStencilView(nullptr), _rasterState(nullptr), _depthStencil(nullptr)
{

}

DirectX3D::DirectX3D(const DirectX3D& other) : _vsync_enabled(other._vsync_enabled), _swapChain(other._swapChain), _device(other._device), _deviceContext(other._deviceContext),
												_renderTargetView(other._renderTargetView), _depthStencilView(other._depthStencilView), _rasterState(other._rasterState), _depthStencil(other._depthStencil)
{
}

DirectX3D::~DirectX3D()
{
}

bool DirectX3D::Initialise(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear)
{
	HRESULT result;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ID3D11Texture2D* backBufferPtr;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewPort;

	_vsync_enabled = vsync;
	
	_hardware = new HardwareDescription;
	_hardware->Initialise(screenWidth, screenHeight);

	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	if (_vsync_enabled)
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
	if (FAILED(result)) return false;

	result = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&backBufferPtr));
	if (FAILED(result)) return false;

	result = _device->CreateRenderTargetView(backBufferPtr, nullptr, &_renderTargetView);
	if (FAILED(result)) return false;

	backBufferPtr->Release();
	backBufferPtr = nullptr;
	
	_depthStencil = new DepthStencil;
	_depthStencil->Initialise(_device, screenWidth, screenHeight);
	_depthStencil->SetStencilType(_deviceContext, STENCIL_STATE_DEFAULT);

	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	result = _device->CreateDepthStencilView(_depthStencil->GetDepthStencilBuffer(), &depthStencilViewDesc, &_depthStencilView);
	if (FAILED(result)) return false;

	_deviceContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);

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

	result = _device->CreateRasterizerState(&rasterDesc, &_rasterState);
	if (FAILED(result)) return false;

	_deviceContext->RSSetState(_rasterState);

	viewPort.Width = static_cast<float>(screenWidth);
	viewPort.Height = static_cast<float>(screenHeight);
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0.0f;
	viewPort.TopLeftY = 0.0f;

	_deviceContext->RSSetViewports(1, &viewPort);

	float fieldOfView = 3.141592654f / 4.0f;
	float screenAspect = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);

	_projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);
	_worldMatrix = XMMatrixIdentity();
	_orthoMatrix = XMMatrixOrthographicLH(static_cast<float>(screenWidth), static_cast<float>(screenHeight), screenNear, screenDepth);

	return true;
}

void DirectX3D::Shutdown()
{
	// Must set to windowed mode otherwise application will throw exception
	if(_swapChain)
	{
		_swapChain->SetFullscreenState(false, nullptr);
	}

	if(_rasterState)
	{
		_rasterState->Release();
		_rasterState = nullptr;
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
}


void DirectX3D::BeginScene(float red, float green, float blue, float alpha)
{
	float color[4];

	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	_deviceContext->ClearRenderTargetView(_renderTargetView, color);
	_deviceContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void DirectX3D::EndScene()
{
	_swapChain->Present(_vsync_enabled ? 1 : 0, 0);
}

ID3D11Device* DirectX3D::GetDevice()
{
	return _device;
}

ID3D11DeviceContext* DirectX3D::GetDeviceContext()
{
	return _deviceContext;
}

void DirectX3D::GetProjectionMatrix(XMMATRIX& projectionMatrix)
{
	projectionMatrix = _projectionMatrix;
}

void DirectX3D::GetWorldMatrix(XMMATRIX& worldMatrix)
{
	worldMatrix = _worldMatrix;
}

void DirectX3D::GetOrthoMatrix(XMMATRIX& orthoMatrix)
{
	orthoMatrix = _orthoMatrix;
}

void DirectX3D::GetVideoCardInfo(char* cardname, int& memory)
{
	_hardware->GetVideoCardDescription(cardname);
	_hardware->GetVideoCardMemory(memory);
}
