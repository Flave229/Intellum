#include "Bitmap.h"

Bitmap::Bitmap(): _vertexBuffer(nullptr), _indexBuffer(nullptr), _vertexCount(0), _indexCount(0),
					_screenWidth(0), _screenHeight(0), _bitmapWidth(0), _bitmapHeight(0), 
					_previousPosX(0), _previousPosY(0), _texture(nullptr)
{
}

Bitmap::Bitmap(const Bitmap& other): _vertexBuffer(other._vertexBuffer), _indexBuffer(other._indexBuffer), _vertexCount(other._vertexCount), _indexCount(other._indexCount),
										_screenWidth(other._screenWidth), _screenHeight(other._screenHeight), _bitmapWidth(other._bitmapWidth), _bitmapHeight(other._bitmapHeight),
										_previousPosX(other._previousPosX), _previousPosY(other._previousPosY), _texture(other._texture)
{
}


Bitmap::~Bitmap()
{
}

bool Bitmap::Initialise(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int screenWidth, int screenHeight, int bitmapWidth, int bitmapHeight, char* textureFilename)
{
	bool result;

	_screenWidth = screenWidth;
	_screenHeight = screenHeight;
	_bitmapWidth = bitmapWidth;
	_bitmapHeight = bitmapHeight;

	_previousPosX = -1;
	_previousPosY = -1;

	result = InitialiseBuffers(device);
	if (!result) return false;

	result = LoadTexture(device, deviceContext, textureFilename);
	if (!result) return false;

	return true;
}

void Bitmap::Shutdown()
{
	ReleaseTexture();
	ShutdownBuffers();
}

bool Bitmap::Render(ID3D11DeviceContext* deviceContext, int positionX, int positionY)
{
	bool result = UpdateBuffers(deviceContext, positionX, positionY);
	if (!result) return false;

	RenderBuffers(deviceContext);

	return true;
}

int Bitmap::GetIndexCount()
{
	return _indexCount;
}

ID3D11ShaderResourceView* Bitmap::GetTexture()
{
	return _texture->GetTexture();
}

bool Bitmap::InitialiseBuffers(ID3D11Device* device)
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
	if (!vertices) return false;

	indices = new unsigned long[_indexCount];
	if (!indices) return false;

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

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer);
	if (FAILED(result)) return false;

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

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &_indexBuffer);
	if (FAILED(result)) return false;

	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	return true;
}

void Bitmap::ShutdownBuffers()
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
}

bool Bitmap::UpdateBuffers(ID3D11DeviceContext* deviceContext, int positionX, int positionY)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	if ((positionX == _previousPosX) && (positionY == _previousPosY))
	{
		return true;
	}

	_previousPosX = positionX;
	_previousPosY = positionY;

	float left = static_cast<float>((_screenWidth / 2) * -1) + static_cast<float>(positionX);
	float right = left + static_cast<float>(_bitmapWidth);

	float top = static_cast<float>(_screenHeight / 2) - static_cast<float>(positionY);
	float bottom = top - static_cast<float>(_bitmapHeight);

	Vertex* vertices = new Vertex[_vertexCount];
	if (!vertices) return false;

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

	result = deviceContext->Map(_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) return false;

	Vertex* verticesPtr = static_cast<Vertex*>(mappedResource.pData);

	memcpy(verticesPtr, (void*)vertices, (sizeof(Vertex*) * _vertexCount));

	deviceContext->Unmap(_vertexBuffer, 0);

	delete[] vertices;
	vertices = nullptr;

	return true;
}

void Bitmap::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride = sizeof(Vertex*);
	unsigned int offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool Bitmap::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	bool result;

	_texture = new Texture;
	if (!_texture) return false;

	result = _texture->Initialise(device, deviceContext, filename);
	if (!result) return false;

	return true;
}

void Bitmap::ReleaseTexture()
{
	if (_texture)
	{
		_texture->Shutdown();
		delete _texture;
		_texture = nullptr;
	}
}
