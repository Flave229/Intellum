#pragma once
#include <d3d11.h>

class RasterizerComponent : public IComponent
{
public:
	D3D11_FILL_MODE FillMode;
	D3D11_CULL_MODE CullMode;

	RasterizerComponent()
		: IComponent(RASTERIZER), FillMode(D3D11_FILL_SOLID), CullMode(D3D11_CULL_BACK) {}

	~RasterizerComponent() override = default;

	void Shutdown() override {}
};