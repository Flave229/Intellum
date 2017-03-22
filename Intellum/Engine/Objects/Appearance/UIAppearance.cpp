#include "UIAppearance.h"

UIAppearance::UIAppearance(DirectX3D* direct3D, Box screenSize, Box bitmapSize, vector<char*> textureFiles, char* lightMapFile) : _direct3D(direct3D), _vertexBuffer(nullptr), _indexBuffer(nullptr), _vertexCount(0), _indexCount(0), _screenSize(screenSize), _bitmapSize(bitmapSize), _previousPosition(XMFLOAT2(-1, -1))
{
	Initialise(textureFiles, lightMapFile);
}

void UIAppearance::Shutdown()
{
	if (_indexBuffer)
	{
		_indexBuffer->Release();
		_indexBuffer = nullptr;
	}

	if (_vertexBuffer)
	{
		_vertexBuffer->Release();
		_vertexBuffer = nullptr;
	}

	for (unsigned long long i = _textures.size(); i > 0; i--)
	{
		_textures.back()->Shutdown();
		_textures.pop_back();
	}

	_textures.clear();

	if (_lightMap != nullptr)
	{
		_lightMap->Shutdown();
		delete _lightMap;
		_lightMap = nullptr;
	}
}

void UIAppearance::Initialise(vector<char*> textureFiles, char* lightMapFile)
{
	InitialiseBuffers();
	LoadTextures(textureFiles, lightMapFile);
}

void UIAppearance::InitialiseBuffers()
{
	Vertex* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	D3D11_SUBRESOURCE_DATA indexData;
	HRESULT result;

	_vertexCount = 6;
	_indexCount = _vertexCount;

	vertices = new Vertex[_vertexCount];
	if (!vertices) throw Exception("Failed to initialise the vertex list.");

	indices = new unsigned long[_indexCount];
	if (!indices) throw Exception("Failed to initialise the index list.");

	memset(vertices, 0, sizeof(Vertex) * _vertexCount);

	for (int i = 0; i < _indexCount; i++)
	{
		indices[i] = i;
	}

	// Setup Vertex Buffer Description
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * _vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = _direct3D->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer);
	if (FAILED(result)) throw Exception("Failed to create the vertex buffer.");

	// Setup Index Buffer Description
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * _indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = _direct3D->GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &_indexBuffer);
	if (FAILED(result)) throw Exception("Failed to create the index buffer.");

	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;
}

void UIAppearance::LoadTextures(vector<char*> textureFiles, char* lightMapFile)
{
	_textures = CreateTexture::ListFrom(_direct3D, textureFiles);
	_lightMap = CreateTexture::From(_direct3D, lightMapFile);
}

void UIAppearance::Render() const
{
	_direct3D->GetRasterizer()->SetRasterizerCullMode(D3D11_CULL_BACK);

	unsigned int stride = sizeof(Vertex);
	unsigned int offset = 0;

	ID3D11DeviceContext* deviceContext = _direct3D->GetDeviceContext();
	deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void UIAppearance::Update(XMFLOAT2 position, Box bitmapSize)
{
	if ((position.x == static_cast<int>(_previousPosition.x)) && (position.y == static_cast<int>(_previousPosition.y)) 
		&& (bitmapSize.Width == _bitmapSize.Width) && (bitmapSize.Height == _bitmapSize.Height))
	{
		return;
	}

	_previousPosition = position;
	_bitmapSize = bitmapSize;

	float left = ((_screenSize.Width / 2) * -1) + (_previousPosition.x);
	float right = left + _bitmapSize.Width;

	float top = (_screenSize.Height / 2) - _previousPosition.y;
	float bottom = top - _bitmapSize.Height;

	Vertex* vertices = new Vertex[_vertexCount];
	if (!vertices) throw Exception("Failed to initialise vertices for bitmap");

	vertices[0].position = XMFLOAT3(left, top, 0.0f);
	vertices[0].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[1].position = XMFLOAT3(right, bottom, 0.0f);
	vertices[1].texture = XMFLOAT2(1.0f, 1.0f);

	vertices[2].position = XMFLOAT3(left, bottom, 0.0f);
	vertices[2].texture = XMFLOAT2(0.0f, 1.0f);

	vertices[3].position = XMFLOAT3(left, top, 0.0f);
	vertices[3].texture = XMFLOAT2(0.0f, 0.0f);

	vertices[4].position = XMFLOAT3(right, top, 0.0f);
	vertices[4].texture = XMFLOAT2(1.0f, 0.0f);

	vertices[5].position = XMFLOAT3(right, bottom, 0.0f);
	vertices[5].texture = XMFLOAT2(1.0f, 1.0f);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = _direct3D->GetDeviceContext()->Map(_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) throw Exception("Failed to map vertex buffer to the Device Context");

	Vertex* verticesPtr = static_cast<Vertex*>(mappedResource.pData);

	memcpy(verticesPtr, static_cast<void*>(vertices), (sizeof(Vertex) * _vertexCount));

	_direct3D->GetDeviceContext()->Unmap(_vertexBuffer, 0);

	delete[] vertices;
	vertices = nullptr;
}

int UIAppearance::GetIndexCount() const
{
	return _indexCount;
}

vector<ID3D11ShaderResourceView*> UIAppearance::GetTextures() const
{
	vector<ID3D11ShaderResourceView*> textureViews;

	for (Texture* texture : _textures)
	{
		textureViews.push_back(texture->GetTexture());
	}

	return textureViews;
}

ID3D11ShaderResourceView* UIAppearance::GetLightMap() const
{
	if (_lightMap != nullptr)
		return _lightMap->GetTexture();

	return nullptr;
}