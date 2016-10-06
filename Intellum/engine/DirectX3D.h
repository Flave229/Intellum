#ifndef _DIRECTX3D_H_
#define  _DIRECTX3D_H_

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <DirectXMath.h>
#include "dx_components/HardwareDescription.h"
#include "dx_components/DepthStencil.h"
#include "dx_components/Rasterizer.h"

using namespace DirectX;

class DirectX3D
{
// Member Level Variables
private:
	bool _vsync_enabled;
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

// Function Declarations
public:
	DirectX3D();
	DirectX3D(const DirectX3D&);
	~DirectX3D();

	bool Initialise(int, int, bool, HWND, bool, float, float);
	void Shutdown();
	
	void BeginScene(float, float, float, float);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetProjectionMatrix(XMMATRIX&);
	void GetWorldMatrix(XMMATRIX&);
	void GetOrthoMatrix(XMMATRIX&);

	void GetVideoCardInfo(char*, int&);

	void TurnZBufferOn();
	void TurnZBufferOff();
};

#endif