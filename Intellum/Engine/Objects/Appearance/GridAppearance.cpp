#include "GridAppearance.h"

GridAppearance::GridAppearance(DirectX3D* direct3D, vector<char*> textureFiles, char* lightMapFile, Box gridSize) : _direct3D(direct3D), _geometry(new Geometry)
{
	Initialise(textureFiles, lightMapFile, gridSize);
}

void GridAppearance::Shutdown()
{
	for (unsigned long long i = _textures.size(); i > 0; i--)
	{
		_textures.back()->Shutdown();
		_textures.pop_back();
	}

	_textures.clear();

	if (_geometry)
	{
		_geometry->Shutdown();
		delete _geometry;
		_geometry = nullptr;
	}

	if (_lightMap)
	{
		_lightMap->Shutdown();
		delete _lightMap;
		_lightMap = nullptr;
	}
}

void GridAppearance::Initialise(vector<char*> textureFilenames, char* lightMapFile, Box gridSize)
{
	GenerateModel(gridSize);
	LoadTextures(textureFilenames, lightMapFile);
}


void GridAppearance::LoadTextures(vector<char*> textureFiles, char* lightMapFile)
{
	_textures = CreateTexture::ListFrom(_direct3D, textureFiles);
	_lightMap = CreateTexture::From(_direct3D, lightMapFile);
}

void GridAppearance::GenerateModel(Box gridSize) const
{
	int halfWidth = gridSize.Width / 2;
	int halfDepth = gridSize.Height / 2;

	Vertex* vertices = new Vertex[4];
	vertices[0].position = XMFLOAT3(halfWidth, 0, halfDepth);
	vertices[0].texture = XMFLOAT2(1, 1);
	vertices[0].normal = XMFLOAT3(0, 1, 0);

	vertices[1].position = XMFLOAT3(halfWidth, 0, -halfDepth);
	vertices[1].texture = XMFLOAT2(1, -1);
	vertices[1].normal = XMFLOAT3(0, 1, 0);

	vertices[2].position = XMFLOAT3(-halfWidth, 0, halfDepth);
	vertices[2].texture = XMFLOAT2(-1, 1);
	vertices[2].normal = XMFLOAT3(0, 1, 0);

	vertices[3].position = XMFLOAT3(-halfWidth, 0, -halfDepth);
	vertices[3].texture = XMFLOAT2(-1, -1);
	vertices[3].normal = XMFLOAT3(0, 1, 0);

	ID3D11Buffer* vertexBuffer = CreateVertexBuffer(4, vertices);

	int indexCount = 6;
	unsigned short* indices = new unsigned short[indexCount];
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 1;
	indices[4] = 3;
	indices[5] = 2;

	ID3D11Buffer* indexBuffer = CreateIndexBuffer(indexCount, indices);

	_geometry->VertexBuffer = vertexBuffer;
	_geometry->IndexBuffer = indexBuffer;
	_geometry->VBOffset = 0;
	_geometry->VBStride = sizeof(Vertex);
	_geometry->IndexCount = static_cast<UINT>(indexCount);

	delete[] vertices;
	delete[] indices;
}

ID3D11Buffer* GridAppearance::CreateVertexBuffer(unsigned long long vertexCount, Vertex* finalVerts) const
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
	_direct3D->GetDevice()->CreateBuffer(&bd, &InitData, &vertexBuffer);
	return vertexBuffer;
}

ID3D11Buffer* GridAppearance::CreateIndexBuffer(int indexCount, unsigned short* indices) const
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
	_direct3D->GetDevice()->CreateBuffer(&bd, &InitData, &indexBuffer);
	return indexBuffer;
}

void GridAppearance::Render() const
{
	ID3D11DeviceContext* deviceContext = _direct3D->GetDeviceContext();
	deviceContext->IASetVertexBuffers(0, 1, &_geometry->VertexBuffer, &_geometry->VBStride, &_geometry->VBOffset);
	deviceContext->IASetIndexBuffer(_geometry->IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void GridAppearance::Update(XMFLOAT2 position, Box bitmapSize)
{
	throw Exception("Update method not implemented in the Appearance Class");
}

int GridAppearance::GetIndexCount() const
{
	return _geometry->IndexCount;
}

vector<ID3D11ShaderResourceView*> GridAppearance::GetTextures() const
{
	vector<ID3D11ShaderResourceView*> textureViews;

	for (Texture* texture : _textures)
	{
		textureViews.push_back(texture->GetTexture());
	}

	return textureViews;
}

ID3D11ShaderResourceView* GridAppearance::GetLightMap() const
{
	if (_lightMap != nullptr)
		return _lightMap->GetTexture();

	return nullptr;
}
