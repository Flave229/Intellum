#pragma once
#include <d3d11.h>
#include <vector>
#include <DirectXMath.h>
#include "../../../../common/Vertex.h"
#include "../../../../common/Box.h"
#include "../../../../common/Geometry.h"

using namespace std;
using namespace DirectX;

class GridBuilder
{
private:
	ID3D11Device* _device;

	static vector<Vertex> BuildVertexList(Box gridSize, XMFLOAT2 cellCount);
	static vector<unsigned short> BuildIndexList(XMFLOAT2 cellCount);

	ID3D11Buffer* CreateVertexBuffer(unsigned long long vertexCount, Vertex* finalVerts) const;
	ID3D11Buffer* CreateIndexBuffer(int indexCount, unsigned short* indices) const;
public:
	GridBuilder(ID3D11Device* device);
	~GridBuilder();

	Geometry Build(Box gridSize, XMFLOAT2 cellCount) const;
};
