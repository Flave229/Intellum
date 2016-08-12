#ifndef _DIRECTX3D_H_
#define  _DIRECTX3D_H_

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class DirectX3D
{
// Member Level Variables
private:
	bool _vsync_enabled;
	int _videoCardMemory;
	char _videoCardDescription[128];
	IDXGISwapChain* _swapChain;
	ID3D11Device* _device;
	ID3D11DeviceContext* _deviceContext;
	ID3D11RenderTargetView* _renderTargetView;
	ID3D11Texture2D* _depthStencilBuffer;
	ID3D11DepthStencilState* _depthStencilState;
	ID3D11DepthStencilState* _depthDisabledStencilState;
	ID3D11DepthStencilView* _depthStencilView;
	ID3D11RasterizerState* _rasterState;
	XMMATRIX _projectionMatrix;
	XMMATRIX _worldMatrix;
	XMMATRIX _orthoMatrix;

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