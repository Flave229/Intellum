#pragma once

#include <d3d11.h>

struct Geometry
{
	ID3D11Buffer* VertexBuffer;
	ID3D11Buffer* IndexBuffer;

	UINT VBStride;
	UINT VBOffset;

	UINT VertexCount;
	UINT IndexCount;

	float width;
	float height;
	float depth;

	void Shutdown()
	{
		if(VertexBuffer)
		{
			VertexBuffer->Release();
			VertexBuffer = nullptr;
		}

		if(IndexBuffer)
		{
			IndexBuffer->Release();
			IndexBuffer = nullptr;
		}
	}
};