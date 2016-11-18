#pragma once

#include <d3d11.h>
#include "../../common/Box.h"

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
private:
	void Initialise(Box screenSize);
	void CreateDepthStencil(ID3D11DepthStencilState* depthStencil, bool depthEnable);

public:
	DepthStencil(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Box screenSize);
	DepthStencil(const DepthStencil& other);
	~DepthStencil();

	void Shutdown();

	void SetStencilType(DepthStencilType) const;

	ID3D11Texture2D* GetDepthStencilBuffer() const;
};

