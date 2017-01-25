#ifndef _DIRECTX3D_H_
#define  _DIRECTX3D_H_

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <DirectXMath.h>
#include "DxComponents/HardwareDescription.h"
#include "DxComponents/DepthStencil.h"
#include "DxComponents/Rasterizer.h"
#include "../common/Box.h"

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

private: 
	void Initialise(Box screenSize, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);

public:
	DirectX3D(Box screenSize, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);
	DirectX3D(const DirectX3D& other);
	~DirectX3D();

	void Shutdown();
	
	void BeginScene(XMFLOAT4 color);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void MapProjectionMatrixInto(XMMATRIX& projectionMatrix);
	void MapWorldMatrixInto(XMMATRIX& worldMatrix);
	void MapOrthoMatrixInto(XMMATRIX& orthoMatrix);

	void GetVideoCardInfo(char* cardname, int& memory);

	void TurnZBufferOn();
	void TurnZBufferOff();
};

#endif