#include "GeometryBuilder.h"

GeometryBuilder::GeometryBuilder(ID3D11Device* device) : _device(device), _gridBuilder(GridBuilder(device))
{
}

GeometryBuilder::~GeometryBuilder()
{
}

Geometry GeometryBuilder::FromFile(char* string) const
{
	return OBJLoader::Load("data/models/sphere.obj", _device);
}

Geometry GeometryBuilder::ForGrid(Box gridSize, XMFLOAT2 cellCount) const
{
	return _gridBuilder.Build(gridSize, cellCount);
}

Geometry GeometryBuilder::ForUI()
{
	Vertex* vertices;
	D3D11_SUBRESOURCE_DATA vertexData;
	D3D11_SUBRESOURCE_DATA indexData;
	HRESULT result;

	Geometry geometry;
	geometry.VertexCount = 6;
	geometry.IndexCount = geometry.VertexCount;

	vertices = new Vertex[geometry.VertexCount];
	if (!vertices) throw Exception("Failed to initialise the vertex list.");

	unsigned short* indices = new unsigned short[geometry.IndexCount];
	if (!indices) throw Exception("Failed to initialise the index list.");

	memset(vertices, 0, sizeof(Vertex) * geometry.VertexCount);

	for (int i = 0; i < geometry.IndexCount; i++)
	{
		indices[i] = i;
	}

	// Setup Vertex Buffer Description
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * geometry.VertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = _device->CreateBuffer(&vertexBufferDesc, &vertexData, &geometry.VertexBuffer);
	if (FAILED(result)) throw Exception("Failed to create the vertex buffer.");

	// Setup Index Buffer Description
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(WORD) * geometry.IndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = _device->CreateBuffer(&indexBufferDesc, &indexData, &geometry.IndexBuffer);
	if (FAILED(result)) throw Exception("Failed to create the index buffer.");

	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return geometry;
}
