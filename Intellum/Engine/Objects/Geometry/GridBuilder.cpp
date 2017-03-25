#include "GridBuilder.h"

GridBuilder::GridBuilder(ID3D11Device* device) : _device(device)
{
}

GridBuilder::~GridBuilder()
{
}

Geometry GridBuilder::Build(Box gridSize, XMFLOAT2 cellCount) const
{
	vector<Vertex> vertices = BuildVertexList(gridSize, cellCount);
	vector<unsigned short> indices = BuildIndexList(cellCount);

	Geometry geometry;
	geometry.VertexBuffer = CreateVertexBuffer(vertices.size(), &vertices[0]);
	geometry.IndexBuffer = CreateIndexBuffer(indices.size(), &indices[0]);
	geometry.VBOffset = 0;
	geometry.VBStride = sizeof(Vertex);
	geometry.IndexCount = static_cast<UINT>(indices.size());
	geometry.Size = XMFLOAT3(gridSize.Width, 0.0f, gridSize.Height);
	return geometry;
}

vector<Vertex> GridBuilder::BuildVertexList(Box gridSize, XMFLOAT2 cellCount)
{
	int halfWidth = gridSize.Width / 2;
	int halfDepth = gridSize.Height / 2;
	XMFLOAT2 cellSize = XMFLOAT2(gridSize.Width / cellCount.x, gridSize.Height / cellCount.y);

	vector<Vertex> vertices = vector<Vertex>();
	for (int row = 0; row <= cellCount.x; row++)
	{
		for (int column = 0; column <= cellCount.y; column++)
		{
			Vertex vertex;
			vertex.position = XMFLOAT3(column * cellSize.x - halfWidth, 0, row * cellSize.y - halfDepth);
			vertex.texture = XMFLOAT2(row * (cellCount.x / gridSize.Width), column * (cellCount.y / gridSize.Height));
			vertex.normal = XMFLOAT3(0, 1, 0);
			vertices.push_back(vertex);
		}
	}

	return vertices;
}

vector<unsigned short> GridBuilder::BuildIndexList(XMFLOAT2 cellCount)
{
	vector<unsigned short> indices = vector<unsigned short>();
	for (int row = 0; row < cellCount.x; row++)
	{
		for (int column = 0; column < cellCount.y; column++)
		{
			unsigned short topLeftIndex = row * (cellCount.y + 1) + column;
			unsigned short topRightIndex = topLeftIndex + 1;
			unsigned short bottomLeftIndex = topLeftIndex + cellCount.y + 1;
			unsigned short bottomRightIndex = topLeftIndex + cellCount.y + 2;

			indices.push_back(topLeftIndex);
			indices.push_back(bottomRightIndex);
			indices.push_back(topRightIndex);
			indices.push_back(topLeftIndex);
			indices.push_back(bottomLeftIndex);
			indices.push_back(bottomRightIndex);
		}
	}

	return indices;
}

ID3D11Buffer* GridBuilder::CreateVertexBuffer(unsigned long long vertexCount, Vertex* finalVerts) const
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(Vertex) * static_cast<UINT>(vertexCount);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = finalVerts;

	ID3D11Buffer* vertexBuffer;
	_device->CreateBuffer(&bd, &InitData, &vertexBuffer);
	return vertexBuffer;
}

ID3D11Buffer* GridBuilder::CreateIndexBuffer(int indexCount, unsigned short* indices) const
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * static_cast<UINT>(indexCount);
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = indices;

	ID3D11Buffer* indexBuffer;
	_device->CreateBuffer(&bd, &InitData, &indexBuffer);
	return indexBuffer;
}