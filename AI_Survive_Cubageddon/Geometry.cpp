#include <d3d11.h>

struct Geometry
{
	ID3D11Buffer * VertexBuffer;
	ID3D11Buffer * IndexBuffer;

	UINT VBStride;
	UINT VBOffset;
	UINT IndexCount;

	float width;
	float height;
	float depth;
};