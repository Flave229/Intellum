#pragma once

#include <d3d11.h>

enum DepthStencilType
{
	STENCIL_STATE_DEFAULT,
	STENCIL_STATE_DISABLED
};

class DepthStencil
{
// Member Level Variables
private:
	ID3D11Device* _device;
	ID3D11DeviceContext* _deviceContext;

	ID3D11Texture2D* _depthStencilBuffer;
	
	ID3D11DepthStencilState* _depthStencilState;
	ID3D11DepthStencilState* _depthDisabledStencilState;

// Function Declarations
public:
	DepthStencil(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	DepthStencil(const DepthStencil&);
	~DepthStencil();

	bool Initialise(int, int);
	void Shutdown();

	void SetStencilType(DepthStencilType) const;

	ID3D11Texture2D* GetDepthStencilBuffer() const;
};

