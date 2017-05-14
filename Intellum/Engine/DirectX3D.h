#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <DirectXMath.h>
#include "DxComponents/HardwareDescription.h"
#include "DxComponents/DepthStencil.h"
#include "DxComponents/Rasterizer.h"
#include "../common/Box.h"
#include "Input/Input.h"

using namespace DirectX;

class DirectX3D
{
private:
	bool _vsyncEnabled;
	IDXGISwapChain* _swapChain;
	ID3D11Device* _device;
	ID3D11DeviceContext* _deviceContext;
	ID3D11RenderTargetView* _renderTargetView;
	ID3D11DepthStencilView* _depthStencilView;
	XMMATRIX _projectionMatrix;
	XMMATRIX _worldMatrix;
	XMMATRIX _orthoMatrix;

	HardwareDescription* _hardware;
	DepthStencil* _depthStencil;
	Rasterizer* _rasterizer;
	float _rasterizerToggleCooldown;
	Input* _input;
private: 
	void Initialise(Box screenSize, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);

public:
	DirectX3D(Input* input, Box screenSize, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);
	~DirectX3D();

	void Shutdown();
	void Update(float delta);
	void BeginScene(XMFLOAT4 color);
	void EndScene() const;

	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;
	Rasterizer* GetRasterizer() const;

	XMMATRIX GetProjectionMatrix() const;
	XMMATRIX GetWorldMatrix() const;
	XMMATRIX GetOrthoMatrix() const;

	void GetVideoCardInfo(char* cardname, int& memory) const;

	void TurnZBufferOn() const;
	void TurnZBufferOff() const;
};