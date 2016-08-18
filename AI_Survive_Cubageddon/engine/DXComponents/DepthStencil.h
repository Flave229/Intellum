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
	ID3D11Texture2D* _depthStencilBuffer;
	
	ID3D11DepthStencilState* _depthStencilState;
	ID3D11DepthStencilState* _depthDisabledStencilState;

// Function Declarations
public:
	DepthStencil();
	DepthStencil(const DepthStencil&);
	~DepthStencil();

	bool Initialise(ID3D11Device*, ID3D11DeviceContext*, int, int);
	void Shutdown();

	void SetStencilType(ID3D11DeviceContext*, DepthStencilType) const;

	ID3D11Texture2D* GetDepthStencilBuffer() const;
};

