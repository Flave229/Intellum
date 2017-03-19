#include "GridAppearance.h"

GridAppearance::GridAppearance(DirectX3D* direct3D, vector<char*> textureFiles, char* lightMapFile, Box gridSize, XMFLOAT2 cellCount) : _direct3D(direct3D), _geometry(new Geometry)
{
	Initialise(textureFiles, lightMapFile, gridSize, cellCount);
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

void GridAppearance::Initialise(vector<char*> textureFilenames, char* lightMapFile, Box gridSize, XMFLOAT2 cellCount)
{
	GenerateModel(gridSize, cellCount);
	LoadTextures(textureFilenames, lightMapFile);
}


void GridAppearance::LoadTextures(vector<char*> textureFiles, char* lightMapFile)
{
	_textures = CreateTexture::ListFrom(_direct3D, textureFiles);
	_lightMap = CreateTexture::From(_direct3D, lightMapFile);
}

void GridAppearance::GenerateModel(Box gridSize, XMFLOAT2 cellCount) const
{
	int halfWidth = gridSize.Width / 2;
	int halfDepth = gridSize.Height / 2;
	XMFLOAT2 cellSize = XMFLOAT2(gridSize.Width / cellCount.x, gridSize.Height / cellCount.y);

	vector<Vertex> vertices = vector<Vertex>();
	for(int row = 0; row <= cellCount.x; row++)
	{
		for(int column = 0; column <= cellCount.y; column++)
		{
			Vertex vertex;
			vertex.position = XMFLOAT3(column * cellSize.x - halfWidth, 0, row * cellSize.y - halfDepth);
			vertex.texture = XMFLOAT2(cellCount.x / gridSize.Width, cellCount.y / gridSize.Height);
			vertex.normal = XMFLOAT3(0, 1, 0);
			vertices.push_back(vertex);
		}
	}

	vector<unsigned short> indices = vector<unsigned short>();
	for (int row = 0; row < cellCount.x; row++)
	{
		for(int column = 0; column < cellCount.y; column++)
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

	ID3D11Buffer* vertexBuffer = CreateVertexBuffer(vertices.size(), &vertices[0]);
	ID3D11Buffer* indexBuffer = CreateIndexBuffer(indices.size(), &indices[0]);

	_geometry->VertexBuffer = vertexBuffer;
	_geometry->IndexBuffer = indexBuffer;
	_geometry->VBOffset = 0;
	_geometry->VBStride = sizeof(Vertex);
	_geometry->IndexCount = static_cast<UINT>(indices.size());
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
